/*
    Akunambol - KDE client for syncing your data
    Copyright (C) 2010-2011  Riccardo Iaconelli <riccardo@kde.org>

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

#include "cachedfunambolbackend.h"

#include <spds/AbstractSyncSourceConfig.h>

#include <QStringList>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>

class CachedFunambolBackend::Private {
    Private(CachedFunambolBackend *p) { parent = p; }

    void openDatabase() {
        QString path = ""; // appdata+syncsource name+something?
        bool newDB = true; // Check if the database file exists or it's a new one

        // Find QSLite driver
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(path);
    
        // Open database
        if (db.open()) {
            if (newDB) {
                QSqlQuery query;
                query.exec("CREATE TABLE keys "
                           "(key varchar(10))"); // NOTE: key size is arbitrary (from SQLite's FAQ #9)
            }
            retrieveAllKeys();
        } else {
            // OUCH!
        }
    }

    void retrieveAllKeys() {
        QSqlQuery query("SELECT * FROM keys");
        if (query.next()) {
            itemsList.append(query.value(0).toString());
        }
        
    }

    QStringList itemsList;
    CachedFunambolBackend *parent;
    friend class CachedFunambolBackend;
};

CachedFunambolBackend::CachedFunambolBackend(const char* name, Funambol::AbstractSyncSourceConfig* sc)
 : FunambolBackend(name, sc)
{
    d = new Private(this);
    d->openDatabase(); // wait until we're ready? (we've loaded all the keys)
    
    init(); // make it an async slot?
    
}

void CachedFunambolBackend::init()
{
    d->itemsList = getAllItems();
}

/*
FunambolSyncItem* CachedFunambolBackend::nextDeletedItem()
{
}

FunambolSyncItem* CachedFunambolBackend::firstDeletedItem()
{
    return FunambolBackend::getFirstDeletedItem();
}

FunambolSyncItem* CachedFunambolBackend::nextUpdatedItem()
{
    return FunambolBackend::getNextUpdatedItem();
}

FunambolSyncItem* CachedFunambolBackend::firstUpdatedItem()
{
    return FunambolBackend::getFirstUpdatedItem();
}

FunambolSyncItem* CachedFunambolBackend::nextNewItem()
{
    return FunambolBackend::getNextNewItem();
}

FunambolSyncItem* CachedFunambolBackend::firstNewItem()
{
    return FunambolBackend::getFirstNewItem();
}

FunambolSyncItem* CachedFunambolBackend::nextItem()
{
    return FunambolBackend::getNextItem();
}

FunambolSyncItem* CachedFunambolBackend::firstItem()
{
    return FunambolBackend::getFirstItem();
}

int CachedFunambolBackend::removeAllItems()
{

    return 0; // WRONG.
}*/

int CachedFunambolBackend::beginSync()
{
    return Funambol::SyncSource::beginSync();
}

int CachedFunambolBackend::endSync()
{
    return Funambol::SyncSource::endSync();
}


// 
// CachedFunambolBackend::~CachedFunambolBackend()
// {
// 
// }

