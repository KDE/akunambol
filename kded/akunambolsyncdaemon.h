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

#ifndef AKUNAMBOLSYNCDAEMON_H
#define AKUNAMBOLSYNCDAEMON_H

#include <kcmsync/syncserver.h>

#include <KDEDModule>

#include <QtCore/QVariantList>
#include <QtCore/QTimer>

class AkunambolSyncDaemon : public KDEDModule
{
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.kde.akunambolsync")

    public:
        AkunambolSyncDaemon(QObject * parent, const QVariantList&);
        virtual ~AkunambolSyncDaemon();

        void load();
        
    public slots:
        void reloadConfiguration();   

    private:
        QList<SyncServer*> m_serverList;
        SyncServer *m_nextSyncServer;
        QTimer m_syncTimer;
        
    private slots:
        void scheduleNextSync();
        void runSync();
        void syncStarted(SyncServer *);
        void syncFinished(SyncServer *);
        
    signals:
        void syncStarted(const QString &syncUrl);
        void syncCompleted(const QString &syncUrl, bool success);
        
};

#endif // AKUNAMBOLSYNCDAEMON_H
