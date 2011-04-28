/*
    Akunambol - KDE client for syncing your data
    Copyright (C) 2011  Riccardo Iaconelli <riccardo@kde.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef FUNAMBOLSYNCJOB_H
#define FUNAMBOLSYNCJOB_H

#include "syncjob.h"

class FunambolBackend;

class FunambolSyncJob : public SyncJob
{
public:
    FunambolSyncJob(QObject *parent);
    virtual void start();
private:
    void setBackend(FunambolBackend *backend);
    FunambolBackend *backend();
    
    class Private;
    FunambolSyncJob::Private *d;
    
    friend class FunambolSyncSource;
};

#endif // FUNAMBOLSYNCJOB_H
