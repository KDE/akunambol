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

#ifndef SYNCSOURCELOADER_H
#define SYNCSOURCELOADER_H

#include <QObject>
#include <kdemacros.h>

class SyncSource2;

class KDE_EXPORT SyncSourceLoader : public QObject
{
    Q_OBJECT
    public:
        SyncSourceLoader(QObject* parent = 0) {};
//         virtual ~SyncSourceLoader();
        
        void loadAllSyncSources();
        
    signals:
        void syncSourceLoaded(SyncSource2 *s);
};

#endif // SYNCSOURCELOADER_H
