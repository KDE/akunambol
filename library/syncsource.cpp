/*
    Copyright (C) 2010 Riccardo Iaconelli <riccardo@kde.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "syncsource.h"
#include "syncconfig.h"
#include <QMutex>

class SyncSource2::SyncSourcePrivate {
    public:
        SyncSourcePrivate(SyncSource2 *q) : q(q), config(0) {}
        SyncSource2 *q;
        SyncConfig *config;
        QMutex lock;
};

SyncSource2::SyncSource2(QObject* parent, const QVariantList& args)
    : QObject(parent)
{
    d = new SyncSourcePrivate(this);
    Q_UNUSED(args);
}

SyncSource2::~SyncSource2()
{
    delete d;
}

bool SyncSource2::isLocked()
{

}

void SyncSource2::setLocked(bool lock)
{

}

void SyncSource2::setConfig(SyncConfig* c)
{
    d->config = c;
}




