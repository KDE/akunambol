/*
    Akunambol - KDE client for syncing your data
    Copyright (C) 2011  Riccardo Iaconelli <riccardo@kde.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SYNCJOB_H
#define SYNCJOB_H

#include <KJob>

/**
 * Abstract class to represent a synchronization job
 */

class KDE_EXPORT SyncJob : public KJob
{
Q_OBJECT
public:
    SyncJob(QObject* parent = 0);
    virtual void start() = 0;
};

#endif // SYNCJOB_H
