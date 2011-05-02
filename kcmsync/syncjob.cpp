/*
 * Copyright (C) 2010 Michael Zanetti <michael_zanetti@gmx.net>
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License version 3 as published by
 * the Free Software Foundation with the addition of the following permission
 * added to Section 15 as permitted in Section 7(a): FOR ANY PART OF THE COVERED
 * WORK IN WHICH THE COPYRIGHT IS OWNED BY FUNAMBOL, FUNAMBOL DISCLAIMS THE
 * WARRANTY OF NON INFRINGEMENT  OF THIRD PARTY RIGHTS.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, see http://www.gnu.org/licenses or write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA.
 */

#include "syncjob.h"

// #include <syncsource/contactssyncer.h>

SyncJob2::SyncJob2(SyncServer* syncServer): m_syncServer(syncServer) {
    m_sourceManager = new SourceManager;

    m_sourceManager->setData(syncServer->username(), syncServer->password(), syncServer->syncUrl());
    
    //TODO: this old code needs some redoing
//     ContactsSyncer *contactsSyncer = new ContactsSyncer(m_sourceManager);
//     connect(contactsSyncer, SIGNAL(startedSync()), SLOT(startedSync()));
//     connect(contactsSyncer, SIGNAL(finishedSync()), SLOT(finishedSync()));

}

SyncJob2::~SyncJob2() {
    delete m_sourceManager;
}

void SyncJob2::startedSync() {
    m_syncServer->syncing();
    emit syncStarted(m_syncServer);
}

void SyncJob2::finishedSync() {
    m_syncServer->synced(true); // TODO: No way to determine result yet...
    emit syncFinished(m_syncServer);
    sender()->deleteLater();
    deleteLater();
}

