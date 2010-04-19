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
