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

class SyncSource::SyncSourcePrivate {
    public:
        SyncSourcePrivate(SyncSource *q) : q(q), config(0) {}
        SyncSource *q;
        SyncConfig *config;
};

SyncSource::SyncSource(QObject* parent, const QVariantList& args)
    : QObject(parent)
{
    d = new SyncSourcePrivate(this);
}

void SyncSource::setConfig(SyncConfig* c)
{
    d->config = c;
}

SyncSource::~SyncSource()
{
    delete d;
}




