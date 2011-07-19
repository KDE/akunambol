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


#include "funambolsyncitem.h"

class FunambolSyncItem::Private {
public:
    QByteArray data;
    QString key;
    QString mimeType;
};

FunambolSyncItem::FunambolSyncItem()
{
    d = new FunambolSyncItem::Private;
}

FunambolSyncItem::~FunambolSyncItem()
{
    delete d;
}

FunambolSyncItem FunambolSyncItem::fromFunambolItem(Funambol::SyncItem &item)
{
    QByteArray data = QByteArray::fromRawData((const char*)item.getData(), item.getDataSize());
    QString key;
    
    if (item.getKey()) {
        key = QString::fromLocal8Bit(item.getKey()); // FIXME: is local8bit the right method?
    }
    
    QString mimetype = QString::fromLocal8Bit(item.getDataType()); // FIXME: is local8bit the right method?
    
    FunambolSyncItem i;
    i.setData(data);
    i.setKey(key);
    i.setMimeType(mimetype);
    
    return i;
}

QByteArray FunambolSyncItem::getData()
{
    return d->data;
}

QString FunambolSyncItem::getKey()
{
    return d->key;
}

QString FunambolSyncItem::getMimeType()
{
    return d->mimeType;
}
void FunambolSyncItem::setData(const QByteArray& data)
{
    d->data = data;
}
void FunambolSyncItem::setKey(const QString& key)
{
    d->key = key;
}
void FunambolSyncItem::setMimeType(const QString& mimeType)
{
    d->mimeType = mimeType;
}

