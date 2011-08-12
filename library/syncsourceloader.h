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
#include <KService>

class SyncSource2;

/**
 * A class to load sync sources. Uses standard dirs, for now.
 * 
 * FIXME: do we want all sync sources to have a single appdata or not?
 * 
 */

class KDE_EXPORT SyncSourceLoader : public QObject
{
    Q_OBJECT
    public:
        SyncSourceLoader(QObject* parent = 0);
//         virtual ~SyncSourceLoader();
        
        /**
         * Load all the Sync Sources that are saved in the configuration.
         */
        void loadAllSyncSources();
        
        /**
         * Load a new Sync Source with name "name"
         */
        void loadSyncSource(const QString &name);
        
    private:
        /**
         * Generates an UUID for a given syncsource given:
         * - name, as provided by KService
         * - the list of sync sources already loaded (retrieved by the 
         * d-pointer)
         */
        QString generateUUID(const QString &name) const;
        void loadPlugin(const KService::Ptr*& service);
        class Private;
        Private *d;
        
    signals:
        void syncSourceLoaded(SyncSource2 *s);
};

#endif // SYNCSOURCELOADER_H
