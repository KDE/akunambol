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


#include "funambolbackend.h"

FunambolBackend::FunambolBackend(const char* name, Funambol::AbstractSyncSourceConfig* sc)
 : SyncSource(name, sc)
{
}

int FunambolBackend::deleteItem(Funambol::SyncItem& item)
{
    FunambolSyncItem i = FunambolSyncItem::fromFunambolItem(item);
    int result = deleteItem(i); //TODO eventually do something more here
    return result;
}

int FunambolBackend::updateItem(Funambol::SyncItem& item)
{
    FunambolSyncItem i = FunambolSyncItem::fromFunambolItem(item);
    int result = updateItem(i); //TODO eventually do something more here - key might have changed?
    return result;
    
}

int FunambolBackend::addItem(Funambol::SyncItem& item)
{
    FunambolSyncItem i = FunambolSyncItem::fromFunambolItem(item);
    int result = addItem(i); //TODO eventually do something more here - key might have changed?
    return result;
}

Funambol::SyncItem* FunambolBackend::getNextDeletedItem()
{
    // NOTE: possible memory leak: have to understand if the sdk automatically deletes this object.
    return nextDeletedItem().toNewFunambolItem();
}

Funambol::SyncItem* FunambolBackend::getFirstDeletedItem()
{
    // NOTE: possible memory leak: have to understand if the sdk automatically deletes this object.
    return nextDeletedItem().toNewFunambolItem();
}

Funambol::SyncItem* FunambolBackend::getNextUpdatedItem()
{
    // NOTE: possible memory leak: have to understand if the sdk automatically deletes this object.
    return nextDeletedItem().toNewFunambolItem();
}

Funambol::SyncItem* FunambolBackend::getFirstUpdatedItem()
{
    // NOTE: possible memory leak: have to understand if the sdk automatically deletes this object.
    return nextDeletedItem().toNewFunambolItem();
}

Funambol::SyncItem* FunambolBackend::getNextNewItem()
{
    // NOTE: possible memory leak: have to understand if the sdk automatically deletes this object.
    return nextDeletedItem().toNewFunambolItem();
}

Funambol::SyncItem* FunambolBackend::getFirstNewItem()
{
    // NOTE: possible memory leak: have to understand if the sdk automatically deletes this object.
    return nextDeletedItem().toNewFunambolItem();
}

Funambol::SyncItem* FunambolBackend::getNextItem()
{
    // NOTE: possible memory leak: have to understand if the sdk automatically deletes this object.
    return nextDeletedItem().toNewFunambolItem();
}

Funambol::SyncItem* FunambolBackend::getFirstItem()
{
    // NOTE: possible memory leak: have to understand if the sdk automatically deletes this object.
    return nextDeletedItem().toNewFunambolItem();
}

