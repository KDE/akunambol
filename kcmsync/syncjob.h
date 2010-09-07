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
 
#ifndef SYNCJOB_H
#define SYNCJOB_H

#include "syncserver.h"

#include <syncsource/sourcemanager.h>

class SyncJob: public QObject
{
    Q_OBJECT
    public:
        SyncJob(SyncServer *syncServer);
        virtual ~SyncJob();
        
    private:
        SyncServer *m_syncServer;
        SourceManager *m_sourceManager;

    private slots:
        void startedSync();
        void finishedSync();
        
    signals:
        void syncStarted(SyncServer *syncServer);
        void syncFinished(SyncServer *syncServer);

};

#endif // SYNCJOB_H
