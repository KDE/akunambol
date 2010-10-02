/*
 * Copyright (C) 2010 Riccardo Iaconelli <riccardo@kde.org>
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

#ifndef SOURCEMANAGER_H
#define SOURCEMANAGER_H
#include <QObject>

#include <spds/SyncReport.h>
#include <client/SyncClient.h>

#include <client/appsyncsource.h>

namespace Akonadi {
    class Item;
}

class KFunSyncConfig;
class QString;

class AkunambolListener;
class AkunambolItemListener;
class AkunambolSourceListener;

class SourceManager : public QObject
{
    Q_OBJECT

    friend class AkunambolListener;
    friend class AkunambolItemListener;
    friend class AkunambolSourceListener;

    public:
        SourceManager(QObject *parent = 0);
        ~SourceManager();

        void setData(QString username, QString password, QString url);

    public slots:
        void sync(AppSyncSource* appSource);

        void emitSourceEnded(AppSyncSource* appSource, SyncReport *report);

    signals:
        void sourceStarted(AppSyncSource* source);
        void addReceived(const char* key);
        void updReceived(const char* key);
        void delReceived(const char* key);
        void addSent(const char* key);
        void updSent(const char* key);
        void delSent(const char* key);
        void sourceEnded(AppSyncSource* source, SyncReport* report);
        void totalClientItems(int n);
        void totalServerItems(int n);

    private:
        void setListeners();
        void unsetListeners();

        void emitSourceStarted(AppSyncSource* appSource);
        void emitAddReceived(const char* key);
        void emitDelReceived(const char* key);
        void emitUpdReceived(const char* key);
        void emitAddSent(const char* key);
        void emitDelSent(const char* key);
        void emitUpdSent(const char* key);
        void emitTotalClientItems(int n);
        void emitTotalServerItems(int n);


    private:
        AkunambolListener *syncListener;
        AkunambolItemListener *itemListener;
        AkunambolSourceListener *sourceListener;
        SyncClient *client;
};

#endif // SOURCEMANAGER_H
