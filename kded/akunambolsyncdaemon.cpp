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

#include "akunambolsyncdaemon.h"
#include "kcmsync/syncserver.h"
#include "kcmsync/syncjob.h"
#include "akunambolsyncadaptor.h"

#include "KConfigGroup"
#include "KConfig"
#include <qtimer.h>
#include <KDebug>
#include <kdemacros.h>
#include <kpluginfactory.h>

K_PLUGIN_FACTORY(AkunambolSyncDaemonFactory, registerPlugin<AkunambolSyncDaemon>();)
K_EXPORT_PLUGIN(AkunambolSyncDaemonFactory("akunambolsync"))


AkunambolSyncDaemon::AkunambolSyncDaemon(QObject* parent, const QVariantList& ): KDEDModule(parent ) {
  
    new AkunambolsyncAdaptor(this);
  
    load();
    kDebug() << "akunambolsyncdaemon loaded...";
    m_nextSyncServer = 0;
    
    m_syncTimer.setSingleShot(true);
    connect(&m_syncTimer, SIGNAL(timeout()), this, SLOT(runSync()));
    
    // Start with autosync in 5 minutes to let the system finish booting up before syncing.
//    QTimer::singleShot(300000, this, SLOT(scheduleNextSync()));
    scheduleNextSync();
}

AkunambolSyncDaemon::~AkunambolSyncDaemon() {

}

void AkunambolSyncDaemon::load() {
    QString configFile = "akunambolrc";
    KConfig config(configFile, KConfig::NoGlobals);
    KConfigGroup serverConfig(&config, "Servers");
    foreach(const QString groupName, serverConfig.groupList()){
        SyncServer *syncServer = new SyncServer();
        syncServer->load(serverConfig, groupName);
        m_serverList.append(syncServer);
        kDebug() << "appending syncserver:" << syncServer->syncUrl();
    }
}

void AkunambolSyncDaemon::reloadConfiguration() {
    kDebug() << "reloading config...";
    m_serverList.clear();
    load();
    scheduleNextSync();
}

void AkunambolSyncDaemon::scheduleNextSync(){
  
    kDebug() << "scheduling next sync";
    SyncServer *server = 0;
    
    // Find nearest autosync schedule
    foreach(SyncServer *tmp, m_serverList){
        if(tmp->autoSyncEnabled()){
            if(server){
                if(tmp->nextSyncTime() < server->nextSyncTime()){
                    server = tmp;
                }
            } else {
                server = tmp;
            }
        }
    }
    
    unsigned int maxInt = -1;
    if(server){
        kDebug() << "Next autosync server:" << server->syncUrl();
        m_nextSyncServer = server;
        if(QDateTime::currentDateTime().secsTo(server->nextSyncTime()) * 1000 > (maxInt / 2)){
            QTimer::singleShot(maxInt / 2, this, SLOT(scheduleNextSync()));
            kDebug() << "Next sync time too far in future. rescheduling in" << maxInt / 2000 / 60 << "minutes";
        } else {
            int nextSyncTime = QDateTime::currentDateTime().secsTo(server->nextSyncTime()) * 1000;
            //nextSyncTime = 60 * 1000; // Sync every minute... for debugging
            m_syncTimer.start(qAbs(nextSyncTime));
            kDebug() << "nextSyncTime" << QDateTime::currentDateTime().addMSecs(qAbs(nextSyncTime));
        }
    } else {
        kDebug() << "No autosync server found";
    }
}

void AkunambolSyncDaemon::runSync() {
    kDebug() << "running sync";
    if(m_nextSyncServer){
        SyncJob *syncJob = new SyncJob(m_nextSyncServer);
        connect(syncJob, SIGNAL(syncStarted(SyncServer*)), SLOT(syncStarted(SyncServer*)));
        connect(syncJob, SIGNAL(syncFinished(SyncServer*)), SLOT(syncFinished(SyncServer*)));
    } else {
        kDebug() << "No next sync server set!!!";
    }
    kDebug() << "Rescheduling...";
    scheduleNextSync();
}

void AkunambolSyncDaemon::syncStarted(SyncServer* server) {
    emit syncStarted(server->syncUrl());
}

void AkunambolSyncDaemon::syncFinished(SyncServer* server) {
    server->synced(false);
    emit syncCompleted(server->syncUrl(), server->lastSyncState() == SyncServer::Successful ? true : false);
    QString configFile = "akunambolrc";
    KConfig config(configFile, KConfig::NoGlobals);
    KConfigGroup serverConfig(&config, "Servers");
    server->save(serverConfig);
}

