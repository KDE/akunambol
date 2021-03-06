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


#ifndef CONTACTSBACKEND_H
#define CONTACTSBACKEND_H

#include <cachedfunambolbackend.h>


class ContactsBackend : public CachedFunambolBackend
{
public:
    ContactsBackend(const char* , Funambol::AbstractSyncSourceConfig* );
    
    virtual FunambolSyncItem getItem(const QString& key);
    virtual QStringList getAllItems();
    virtual int addItem(FunambolSyncItem& item);
    virtual int updateItem(FunambolSyncItem& item);
    virtual int deleteItem(FunambolSyncItem& item);
};

#endif // CONTACTSBACKEND_H
