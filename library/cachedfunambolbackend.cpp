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

class CachedFunambolBackend::Private {
public:
    Private(CachedFunambolBackend *p) { parent = p; }
    
    QStringList itemsList;
    
    CachedFunambolBackend *parent;
    friend class CachedFunambolBackend;
};

CachedFunambolBackend::CachedFunambolBackend(const char* name, Funambol::AbstractSyncSourceConfig* sc)
 : FunambolBackend(name, sc)
{
    d = new Private(this);
    d->itemsList = getAllItems();
}

int CachedFunambolBackend::deleteItem(Funambol::SyncItem& item)
{
    return FunambolBackend::deleteItem(item);
}

int CachedFunambolBackend::updateItem(Funambol::SyncItem& item)
{
    return FunambolBackend::updateItem(item);
}

int CachedFunambolBackend::addItem(Funambol::SyncItem& item)
{
    return FunambolBackend::addItem(item);
}

Funambol::SyncItem* CachedFunambolBackend::getNextDeletedItem()
{
    return FunambolBackend::getNextDeletedItem();
}

Funambol::SyncItem* CachedFunambolBackend::getFirstDeletedItem()
{
    return FunambolBackend::getFirstDeletedItem();
}

Funambol::SyncItem* CachedFunambolBackend::getNextUpdatedItem()
{
    return FunambolBackend::getNextUpdatedItem();
}

Funambol::SyncItem* CachedFunambolBackend::getFirstUpdatedItem()
{
    return FunambolBackend::getFirstUpdatedItem();
}

Funambol::SyncItem* CachedFunambolBackend::getNextNewItem()
{
    return FunambolBackend::getNextNewItem();
}

Funambol::SyncItem* CachedFunambolBackend::getFirstNewItem()
{
    return FunambolBackend::getFirstNewItem();
}

Funambol::SyncItem* CachedFunambolBackend::getNextItem()
{
    return FunambolBackend::getNextItem();
}

Funambol::SyncItem* CachedFunambolBackend::getFirstItem()
{
    return FunambolBackend::getFirstItem();
}

int CachedFunambolBackend::removeAllItems()
{
    return FunambolBackend::removeAllItems();
}

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

