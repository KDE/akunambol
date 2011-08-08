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

#include <KStandardDirs>

class CachedFunambolBackend::Private {
    Private(CachedFunambolBackend *p) { parent = p; }

    void openDatabase() {
        QString path = KStandardDirs::locateLocal("appdata", QString("%1/cachedb.sqlite").arg(sourceUID));

        // Find QSLite driver
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(path);
    
        // Open database
        if (db.open()) {
            if (KStandardDirs::exists(path)) {
                QSqlQuery query;
                query.exec("CREATE TABLE keys "
                           "(key varchar(10))"); // NOTE: key size is arbitrary, more than 10 char are allowed (from SQLite's FAQ #9)
                                                 // NOTE #2: Looks like Funambol doesn't accept keys bigger than 256. See SyncItem.h
            }
            retrieveAllKeys();
        } else {
            // OUCH!
        }
    }

    void retrieveAllKeys() {
        QSqlQuery query("SELECT * FROM keys");
        if (query.next()) {
            itemList.append(query.value(0).toString());
        }
    }

    void removeKey(const QString &key) {
        QSqlQuery query(QString("DELETE from keys WHERE key='%1'").arg(key));
    }

    void insertKey(const QString &key) {
        QSqlQuery query(QString("INSERT into keys VALUES ('%1')").arg(key));
    }

    QStringList itemList, newItems, deletedItems;
    QString sourceUID;
    CachedFunambolBackend *parent;
    friend class CachedFunambolBackend;
};

CachedFunambolBackend::CachedFunambolBackend(const char* name, Funambol::AbstractSyncSourceConfig* sc)
 : FunambolBackend(name, sc)
{
    d = new Private(this);
    d->sourceUID = name; // FIXME: is this an UID?
    d->openDatabase(); // wait until we're ready? (we've loaded all the keys)
    
    init(); // make it an async slot?
    
}

void CachedFunambolBackend::init()
{
    QStringList newItemList = getAllItems();
    QStringList cachedItemList = d->itemList;
    QStringList deletedItems;
    QStringList newItems;
    int i, j;
    
    // We use qSort so that we sort using operator>()
    qSort(newItemList.begin(), newItemList.end());
    qSort(cachedItemList.begin(), cachedItemList.end());
    
    // i -> newItemList
    // j -> cachedItemList
    while (i < newItemList.size() && j < cachedItemList.size()) {
    
        if (newItemList.at(i) == cachedItemList.at(j)) {
            i++;
            j++;
            continue;
        }
        
        if (newItemList.at(i) < cachedItemList.at(j)) {
            newItems.append(newItemList.at(i));
            i++;
            continue;
        } else {
            deletedItems.append(cachedItemList.at(j));
            j++;
            continue;
        }
        
    }
    
    d->deletedItems = deletedItems;
    d->newItems = newItems;
    
    // Now that we have seen what has changed, let's update the database.
    foreach (const QString key, d->newItems) {
        d->insertKey(key);
    }
    
    foreach (const QString key, d->deletedItems) {
        d->removeKey(key);
    }
    
    d->itemList = newItemList;
}

FunambolSyncItem CachedFunambolBackend::nextDeletedItem()
{
    QString key = d->deletedItems.takeFirst();
    return getItem(key);
}

FunambolSyncItem CachedFunambolBackend::firstDeletedItem()
{
    QString key = d->deletedItems.takeFirst();
    return getItem(key);
}

FunambolSyncItem CachedFunambolBackend::nextUpdatedItem()
{
    return getItem(""); // WRONG.
}

FunambolSyncItem CachedFunambolBackend::firstUpdatedItem()
{
    return getItem(""); // WRONG.
}

FunambolSyncItem CachedFunambolBackend::nextNewItem()
{
    QString key = d->newItems.takeFirst();
    return getItem(key);
}

// FIXME: check - do we need to takeFirst or just return the first?
FunambolSyncItem CachedFunambolBackend::firstNewItem()
{
    QString key = d->newItems.takeFirst();
    return getItem(key);
}

FunambolSyncItem CachedFunambolBackend::nextItem()
{
    QString key = d->itemList.takeFirst();
    return getItem(key);
}

FunambolSyncItem CachedFunambolBackend::firstItem()
{
    QString key = d->itemList.takeFirst();
    return getItem(key);
}

int CachedFunambolBackend::removeAllItems()
{
    int error;
    foreach (const QString key, d->itemList) {
        error = deleteItem(key);
        if (error) {
            return error;
        }
    }
    return STC_OK; // FIXME Marco: is STC_OK the right code?
}

int CachedFunambolBackend::beginSync()
{
    return Funambol::SyncSource::beginSync();
}

int CachedFunambolBackend::endSync()
{
    return Funambol::SyncSource::endSync();
}

int CachedFunambolBackend::deleteItem(const QString& key)
{
    FunambolSyncItem item = getItem(key);
    return deleteItem(item);
}


// 
// CachedFunambolBackend::~CachedFunambolBackend()
// {
// 
// }

