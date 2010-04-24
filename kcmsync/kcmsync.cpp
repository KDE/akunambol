/*****************************************************************************
 *  Copyright (C) 2010  Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 *  This program is free software: you can redistribute it and/or modify     *
 *  it under the terms of the GNU General Public License as published by     *
 *  the Free Software Foundation, either version 3 of the License, or        *
 *  (at your option) any later version.                                      *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU General Public License for more details.                             *
 *                                                                           *
 *  You should have received a copy of the GNU General Public License        *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
 *****************************************************************************/

#include "kcmsync.h"
#include "syncserver.h"
#include "syncjob.h"
#include "serverproperties.h"

#include <QtGui/QHBoxLayout>

#include <kdebug.h>
#include <kgenericfactory.h>
#include <kaboutdata.h>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusConnection>
#include <qdbusreply.h>

K_PLUGIN_FACTORY( KCMSyncFactory, registerPlugin<KCMSync>();)
K_EXPORT_PLUGIN( KCMSyncFactory( "kcm_sync" ) )

KCMSync::KCMSync(QWidget *parent, const QVariantList &args) :
        KCModule(KCMSyncFactory::componentData(), parent, args)
{

    KGlobal::locale()->insertCatalog("kcm_sync");
    setAboutData(new KAboutData("kcm_sync", 0, ki18n("Synchronisation"), "0.1",
                                ki18n("The KDE Synchronisation System"), KAboutData::License_GPL_V2,
                                ki18n("2010 Authors info"),
                                ki18n("Use this to configure your sync servers."),
                                "http://is.there.any.web.site"));

    QHBoxLayout *layout = new QHBoxLayout(this);
    QWidget *widget = new QWidget(this);
    ui.setupUi(widget);
    layout->addWidget(widget);
    ui.pbSyncstate->setVisible(false);
    ui.lwServerList->setItemDelegate(new SyncServerDelegate());
    ui.pbSyncstate->setMaximumWidth(ui.pbSyncNow->width());
    
    connect(ui.pbAddServer, SIGNAL(clicked()), SLOT(addServer()));
    connect(ui.pbRemoveServer, SIGNAL(clicked()), SLOT(removeServer()));
    connect(ui.pbEditServer, SIGNAL(clicked()), SLOT(editServer()));
    connect(ui.pbSyncNow, SIGNAL(clicked()), SLOT(syncNow()));
    
    connect(ui.lwServerList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), SLOT(updateButtons()));
    updateButtons();
    
    QDBusConnection::sessionBus().connect("org.kde.kded", "/modules/akunambolsync", "org.kde.akunambolsync", "syncStarted",  this, SLOT(autosyncStarted(const QString &)));
    QDBusConnection::sessionBus().connect("org.kde.kded", "/modules/akunambolsync", "org.kde.akunambolsync", "syncCompleted",  this, SLOT(autosyncCompleted(const QString &, bool)));

}

KCMSync::~KCMSync() {
    while(ui.lwServerList->count() > 0){
        QListWidgetItem *item = ui.lwServerList->takeItem(0);
        delete qVariantValue<SyncServer*>(item->data(Qt::UserRole));
        delete item;
    }
}

void KCMSync::load() {
    QString configFile = "akunambolrc";
    KConfig config(configFile, KConfig::NoGlobals);
    KConfigGroup serverConfig(&config, "Servers");
    foreach(const QString groupName, serverConfig.groupList()){
        SyncServer *syncServer = new SyncServer();
        syncServer->load(serverConfig, groupName);
        QListWidgetItem *item = new QListWidgetItem();
        item->setData(Qt::UserRole, qVariantFromValue(syncServer));
        ui.lwServerList->addItem(item);
    }
}

void KCMSync::save() {
    bool autosyncsconfigured = false;
    KConfig config("akunambolrc");
    config.deleteGroup("Servers");
    KConfigGroup serverConfig(&config, "Servers");
    for(int i = 0; i < ui.lwServerList->count(); i++){
        SyncServer *syncServer = qVariantValue<SyncServer*>(ui.lwServerList->item(i)->data(Qt::UserRole));
        config.deleteGroup(syncServer->syncUrl());
        syncServer->save(serverConfig);
        if(syncServer->autoSyncEnabled()){
            autosyncsconfigured = true;
        }
    }
    
    serverConfig.sync(); // Sync config to disk because kded module reloads it
    
    // check if there are any autosyncs configured. If yes, start the kded module, if no, stop it
    if(autosyncsconfigured){
        kDebug() << "autosyncs conigured... loading daemon...";
        QDBusMessage m = QDBusMessage::createMethodCall("org.kde.kded", "/kded", "org.kde.kded", "loadModule");
        m << "akunambolsync";
        QDBusReply<bool> reply = QDBusConnection::sessionBus().call(m);
        if(!reply.isValid() || !reply.value()){
            kDebug() << "Could not load kded module. kded running?";
        } else {

            m = QDBusMessage::createMethodCall("org.kde.kded", "/kded", "org.kde.kded", "setModuleAutoloading");
            m << "akunambolsync" << true;
            QDBusConnection::sessionBus().call(m);
            
            m = QDBusMessage::createMethodCall("org.kde.kded", "/modules/akunambolsync", "org.kde.akunambolsync", "reloadConfiguration");
            QDBusConnection::sessionBus().call(m);
            
            QDBusConnection::sessionBus().connect("org.kde.kded", "/modules/akunambolsync", "org.kde.akunambolsync", "syncStarted",  this, SLOT(autosyncStarted(const QString &)));
            QDBusConnection::sessionBus().connect("org.kde.kded", "/modules/akunambolsync", "org.kde.akunambolsync", "syncCompleted",  this, SLOT(autosyncCompleted(const QString &, bool)));
        }
    } else {
        kDebug() << "no autosyncs conigured... unloading daemon...";
        QDBusMessage m = QDBusMessage::createMethodCall("org.kde.kded", "/kded", "org.kde.kded", "unloadModule");
        m << "akunambolsync";
        QDBusReply<bool> reply = QDBusConnection::sessionBus().call(m);
        if(!reply.isValid() || !reply.value()){
            kDebug() << "Could not unload kded module... nothing to worry about...";
        }

        m = QDBusMessage::createMethodCall("org.kde.kded", "/kded", "org.kde.kded", "setModuleAutoloading");
        m << "akunambolsync" << false;
        QDBusConnection::sessionBus().call(m);          
    }
    
}

void KCMSync::updateButtons() {
    if(ui.lwServerList->currentIndex().isValid()){
        ui.pbEditServer->setEnabled(true);
        ui.pbRemoveServer->setEnabled(true);
        ui.pbSyncNow->setEnabled(true);
    } else {
        ui.pbEditServer->setEnabled(false);
        ui.pbRemoveServer->setEnabled(false);
        ui.pbSyncNow->setEnabled(false);      
    }
}

void KCMSync::addServer() {
    SyncServer *syncServer = new SyncServer();

    QPointer<ServerProperties> addServerDialog = new ServerProperties(syncServer, this);
    //addServerDialog->setSyncUrl("http://my.funambol.com/funambol/ds");

    if(addServerDialog->exec() == KDialog::Ok){
        
        QListWidgetItem *item = new QListWidgetItem();
        item->setData(Qt::UserRole, qVariantFromValue(syncServer));
        ui.lwServerList->addItem(item);
        emit changed();
    } else {
        delete syncServer;
    }

    delete addServerDialog;
}

void KCMSync::removeServer() {
    int index = ui.lwServerList->currentRow();
    ui.lwServerList->takeItem(index);
    emit changed();
}

void KCMSync::editServer() {
    SyncServer *syncServer = qVariantValue<SyncServer*>(ui.lwServerList->currentItem()->data(Qt::UserRole));
    QPointer<ServerProperties> editServerDialog = new ServerProperties(syncServer, this);
    
    if(editServerDialog->exec() == KDialog::Accepted){
        emit changed();
    }
}

void KCMSync::syncNow() {
    ui.pbSyncNow->setEnabled(false);
    SyncServer *syncServer = qVariantValue<SyncServer*>(ui.lwServerList->currentItem()->data(Qt::UserRole));

    SyncJob *syncJob = new SyncJob(syncServer);
    connect(syncJob, SIGNAL(syncStarted(SyncServer*)), SLOT(syncStarted(SyncServer*)));
    connect(syncJob, SIGNAL(syncFinished(SyncServer*)), SLOT(syncFinished(SyncServer*)));
}

void KCMSync::syncStarted(SyncServer* syncServer) {
    ui.pbSyncstate->setVisible(true);
    ui.lwServerList->currentItem()->setData(Qt::UserRole, qVariantFromValue(syncServer));
}

void KCMSync::syncFinished(SyncServer* syncServer) {
    ui.pbSyncNow->setEnabled(true);
    ui.pbSyncstate->setVisible(false);
    syncServer->save(KConfigGroup(&KConfig("akunambolrc"), "Servers"));
}

void KCMSync::autosyncStarted(const QString& syncUrl) {
    kDebug() << "received autosync started event for server" << syncUrl;
    ui.pbSyncstate->setVisible(true);
    ui.pbSyncNow->setEnabled(false);
    
    for(int i = 0; i < ui.lwServerList->count(); i++){
        SyncServer *syncServer = qVariantValue<SyncServer*>(ui.lwServerList->item(i)->data(Qt::UserRole));
        if(syncServer->syncUrl() == syncUrl){
            syncServer->syncing();
        }
    }    
}

void KCMSync::autosyncCompleted(const QString& syncUrl, bool success) {
    kDebug() << "received autosync completed event for server" << syncUrl << "with success" << success;
    ui.pbSyncstate->setVisible(false);
    ui.pbSyncNow->setEnabled(true);
    for(int i = 0; i < ui.lwServerList->count(); i++){
        SyncServer *syncServer = qVariantValue<SyncServer*>(ui.lwServerList->item(i)->data(Qt::UserRole));
        if(syncServer->syncUrl() == syncUrl){
            syncServer->synced(success);
        }
    }
}


#include "kcmsync.moc"
