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

#include <qtgui/config.h>

#include <QtGui/QHBoxLayout>

#include <kdebug.h>
#include <kgenericfactory.h>
#include <kaboutdata.h>

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
    KConfig config("akunambolrc");
    config.deleteGroup("Servers");
    KConfigGroup serverConfig(&config, "Servers");
    for(int i = 0; i < ui.lwServerList->count(); i++){
        SyncServer *syncServer = qVariantValue<SyncServer*>(ui.lwServerList->item(i)->data(Qt::UserRole));
        config.deleteGroup(syncServer->syncUrl());
        syncServer->save(serverConfig);
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
    QPointer<Config> addServerDialog = new Config(this);
    addServerDialog->setSyncUrl("http://my.funambol.com/funambol/ds");

    KConfig config("akunambolrc");
    KConfigGroup serverGroup(&config, "Servers");
    if(addServerDialog->exec()){
        SyncServer *syncServer = new SyncServer();
        syncServer->setSyncUrl(addServerDialog->syncUrl());
        syncServer->setUsername(addServerDialog->user());
        syncServer->setPassword(addServerDialog->password());
        
        QListWidgetItem *item = new QListWidgetItem();
        item->setData(Qt::UserRole, qVariantFromValue(syncServer));
        ui.lwServerList->addItem(item);
        emit changed();
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
    QPointer<Config> editServerDialog = new Config(this);
    editServerDialog->setSyncUrl(syncServer->syncUrl());
    editServerDialog->setUser(syncServer->username());
    editServerDialog->setPassword(syncServer->password());
    
    if(editServerDialog->exec()){
        syncServer->setSyncUrl(editServerDialog->syncUrl());
        syncServer->setUsername(editServerDialog->user());
        syncServer->setPassword(editServerDialog->password());
        emit changed();
    }
}

void KCMSync::syncNow() {
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
    ui.pbSyncstate->setVisible(false);
    syncServer->save(KConfigGroup(&KConfig("akunambolrc"), "Servers"));
}

#include "kcmsync.moc"
