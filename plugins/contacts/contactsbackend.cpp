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


#include "contactsbackend.h"

#include <KDebug>

ContactsBackend::ContactsBackend(const char* name, Funambol::AbstractSyncSourceConfig* config)
    : CachedFunambolBackend(name, config)
{

}

FunambolSyncItem ContactsBackend::getItem(const QString& key)
{
    FunambolSyncItem item;
    item.setKey(key);
    item.setData("aaa");
    return item;
}

QStringList ContactsBackend::getAllItems()
{
    QStringList list;
    list << "a" << "b";
    return list;
}

int ContactsBackend::addItem(FunambolSyncItem& item)
{
    kDebug() << "adding item" << item.getKey();
    return 0;
}

int ContactsBackend::updateItem(FunambolSyncItem& item)
{
    kDebug() << "updating item" << item.getKey();
    return 0;
}

int ContactsBackend::deleteItem(FunambolSyncItem& item)
{
    kDebug() << "deleting item" << item.getKey();
    return 0;
}

