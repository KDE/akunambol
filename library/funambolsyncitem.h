/*
    Akunambol - KDE client for syncing your data
    Copyright (C) 2010-2011  Riccardo Iaconelli <riccardo@kde.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef FUNAMBOLSYNCITEM_H
#define FUNAMBOLSYNCITEM_H

#include "syncitem.h"
#include <spds/SyncItem.h>
#include <qstring.h>

#include <kdemacros.h>

class KDE_EXPORT FunambolSyncItem : public SyncItem
{
    public:
        FunambolSyncItem();
        virtual ~FunambolSyncItem();
        Funambol::SyncItem* toNewFunambolItem();
        
        QString getKey();
        QByteArray getData();
        QString getMimeType();
        
        void setKey(const QString &key);
        void setData(const QByteArray &data);
        void setMimeType(const QString &mimeType);
        
        static FunambolSyncItem fromFunambolItem(Funambol::SyncItem &item);
        
    private:
        class Private;
        Private *d;
};

#endif // FUNAMBOLSYNCITEM_H
