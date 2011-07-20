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


#ifndef CACHEDFUNAMBOLBACKEND_H
#define CACHEDFUNAMBOLBACKEND_H

#include <library/funambolbackend.h>

class QStringList;

class CachedFunambolBackend : public FunambolBackend
{

public:
    virtual int deleteItem(Funambol::SyncItem& item);
    virtual int updateItem(Funambol::SyncItem& item);
    virtual int addItem(Funambol::SyncItem& item);
    virtual Funambol::SyncItem* getNextDeletedItem();
    virtual Funambol::SyncItem* getFirstDeletedItem();
    virtual Funambol::SyncItem* getNextUpdatedItem();
    virtual Funambol::SyncItem* getFirstUpdatedItem();
    virtual Funambol::SyncItem* getNextNewItem();
    virtual Funambol::SyncItem* getFirstNewItem();
    virtual Funambol::SyncItem* getNextItem();
    virtual Funambol::SyncItem* getFirstItem();
    virtual int removeAllItems();
    virtual int beginSync();
    virtual int endSync();
    
    /**
     * @return a list of UUIDs
     */
    QStringList getAllItems();
    
    CachedFunambolBackend(const char*, Funambol::AbstractSyncSourceConfig*);
    //virtual ~CachedFunambolBackend();
private:
    class Private;
    Private *d;
};

#endif // CACHEDFUNAMBOLBACKEND_H
