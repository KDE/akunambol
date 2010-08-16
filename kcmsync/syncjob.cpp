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

#include "syncjob.h"

#include <syncsource/contactssyncer.h>

SyncJob::SyncJob(SyncServer* syncServer): m_syncServer(syncServer) {
    m_sourceManager = new SourceManager;

    m_sourceManager->setData(syncServer->username(), syncServer->password(), syncServer->syncUrl());
    
    //ContactsSyncer *contactsSyncer = new ContactsSyncer(m_sourceManager);
    //connect(contactsSyncer, SIGNAL(startedSync()), SLOT(startedSync()));
    //connect(contactsSyncer, SIGNAL(finishedSync()), SLOT(finishedSync()));

}

SyncJob::~SyncJob() {
    delete m_sourceManager;
}

void SyncJob::startedSync() {
    m_syncServer->syncing();
    emit syncStarted(m_syncServer);
}

void SyncJob::finishedSync() {
    m_syncServer->synced(true); // TODO: No way to determine result yet...
    emit syncFinished(m_syncServer);
    sender()->deleteLater();
    deleteLater();
}

