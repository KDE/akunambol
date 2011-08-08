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

#include "funambolbackend.h"

#include <QStringList>
#include <kdemacros.h>

class KDE_EXPORT CachedFunambolBackend : public FunambolBackend
{

public:
    CachedFunambolBackend(const char*, Funambol::AbstractSyncSourceConfig*);
    //virtual ~CachedFunambolBackend();
    
    virtual int deleteItem(FunambolSyncItem& item) = 0;
    
    /**
     * If you can delete an item just by its key, reimplement this. If you don't do it,
     * this is equivalent to deleteItem(getItem(key));
     */
    virtual int deleteItem(const QString &key);
    
    virtual int updateItem(FunambolSyncItem& item) = 0;
    virtual int addItem(FunambolSyncItem& item) = 0;
    virtual FunambolSyncItem nextDeletedItem();
    virtual FunambolSyncItem firstDeletedItem();
    virtual FunambolSyncItem nextUpdatedItem();
    virtual FunambolSyncItem firstUpdatedItem();
    virtual FunambolSyncItem nextNewItem();
    virtual FunambolSyncItem firstNewItem();
    virtual FunambolSyncItem nextItem();
    virtual FunambolSyncItem firstItem();
    
    /**
     * Reimplement this function if you can do it in a more efficient way
     * than simply calling deleteItem on every item.
     */
    virtual int removeAllItems();
    
    virtual int beginSync();
    virtual int endSync();
    
    /**
     * @return a list of UUIDs
     */
    virtual QStringList getAllItems() = 0;
    
    virtual FunambolSyncItem getItem(const QString &key) = 0;
    
private:
    void init();
    
    class Private;
    Private *d;
};

#endif // CACHEDFUNAMBOLBACKEND_H
