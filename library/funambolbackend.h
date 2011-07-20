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


#ifndef FUNAMBOLBACKEND_H
#define FUNAMBOLBACKEND_H

#include <spds/SyncSource.h>
#include "funambolsyncitem.h"


class FunambolBackend : public Funambol::SyncSource
{
    public:
        FunambolBackend(const char* name, Funambol::AbstractSyncSourceConfig* sc);
        
        virtual int deleteItem(FunamboSyncItem& item) = 0;
        virtual int updateItem(FunambolSyncItem& item) = 0;
        virtual int addItem(FunambolSyncItem& item) = 0;
        virtual FunambolSyncItem* getNextDeletedItem() = 0;
        virtual FunambolSyncItem* getFirstDeletedItem() = 0;
        virtual FunambolSyncItem* getNextUpdatedItem() = 0;
        virtual FunambolSyncItem* getFirstUpdatedItem() = 0;
        virtual FunambolSyncItem* getNextNewItem() = 0;
        virtual FunambolSyncItem* getFirstNewItem() = 0;
        virtual FunambolSyncItem* getNextItem() = 0;
        virtual FunambolSyncItem* getFirstItem() = 0;
        virtual int removeAllItems() = 0;
        
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
};

#endif // FUNAMBOLBACKEND_H
