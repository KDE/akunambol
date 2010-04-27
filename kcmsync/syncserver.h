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


#ifndef SYNCSERVER_H
#define SYNCSERVER_H

#include <QtGui/QStyledItemDelegate>
#include <QtCore/QDateTime>
#include <QtCore/QMetaType>

#include <KConfigGroup>
#include <kwallet.h>

class SyncServer
{

    public:
    
        enum SyncState {Never = 0, Successful, Failed, Running};
        enum SyncRecurrance {Hourly = 0, Daily, Weekly, Monthly};
      
        SyncServer();
        ~SyncServer();
        
        void setSyncUrl(const QString &url);
        QString syncUrl() const;
        
        void setUsername(const QString &username);
        QString username() const;
        
        void setPassword(const QString &password);
        QString password() const;
        
        void setAutoSyncEnabled(bool enable);
        bool autoSyncEnabled();
        
        void setAutoSyncRecurrance(SyncRecurrance recurrance);
        SyncRecurrance autoSyncRecurrance();
        
        void setAutoSyncDay(int day);
        int autoSyncDay();
        
        void setAutoSyncMinute(int minute);
        int autoSyncMinute();
        
        void setAutoSyncTime(const QTime &time);
        QTime autoSyncTime();
        
        void syncing();
        void synced(bool successful);
        QDateTime lastSyncTime();
        SyncState lastSyncState();
        QDateTime nextSyncTime();
        
        void load(KConfigGroup config, const QString &syncUrl);
        void save(KConfigGroup config);

    private:
        QString m_syncUrl;
        QString m_username;
        QString m_password;
        QDateTime m_lastSyncTime;
        SyncState m_lastSyncState;
        
        bool m_autoSyncEnabled;
        SyncRecurrance m_autoSyncRecurrance;
        int m_autoSyncDay;
        int m_autoSyncMinute;
        QTime m_autoSyncTime;
        
        KConfigGroup m_config;
        KWallet::Wallet *m_wallet;
    
};

Q_DECLARE_METATYPE(SyncServer*)

class SyncServerDelegate : public QStyledItemDelegate
{
     Q_OBJECT

    public:
        SyncServerDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}
        ~SyncServerDelegate(){};

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

};

#endif
