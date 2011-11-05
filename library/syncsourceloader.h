/*
    Copyright (C) 2010 Riccardo Iaconelli <riccardo@kde.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 2 of the
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
#include <KPluginInfo>

class SyncSource2;

/**
 * A class to load sync sources. Uses standard dirs, for now.
 * 
 * FIXME: for now assuming all sync sources to have a single appdata.
 * 
 */

class KDE_EXPORT SyncSourceLoader : public QObject
{
    Q_OBJECT
    public:
        SyncSourceLoader(QObject* parent = 0);
        virtual ~SyncSourceLoader();
        
        /**
         * Load all the Sync Sources.
         * 
         * This function gets a list of sync sources that are saved in the
         * configuration, accessed through KGlobal::config().
         */
        void loadAllSavedSyncSources();
        
        /**
         * Load a new Sync Source with X-KDE-Library=library
         */
        void loadNewSyncSource(const QString& library);
        
        /**
         * Refresh the list of sync sources from KSyCoCa.
         */
        void refreshSyncSourcesList();
        
        /**
         * @returns a list of KPluginInfo about all the syncsources that we
         * know of, to be used e.g. to feed KPluginSelector.
         */
        QList<KPluginInfo> syncSourcesInfo();
        
        /**
         * Removes all references to a syncSource with uid @param uid from the
         * configuration; The caller must make sure to also delete any no longer
         * needed data that the source might have created.
         * 
         * @note The plugin is NOT unloaded.
         */
        void removeSyncSource(const QString &uid);
        
    private:        
        /**
         * Load a new Sync Source with name "name".
         * 
         * @param name The plugin name (identical to KService::name()) to load
         * 
         * @param instanceUID The instance number: a counter which knows how many plugins
         * of the same kind have ever been loaded. No plugins can share the same instance
         * number, and numbers of plugins deleted from the config are never taken again.
         * 
         * @param uid An unique id, which identifies the plugin. It is used as basename
         * for stuff like configuration storage. Can be composed of something like
         * name+"_"+instance
         * 
         */
        bool loadPlugin(const QString& name, const QString& uid, int instanceID);
        
        class Private;
        Private * const d;
        
    signals:
        void syncSourceLoaded(SyncSource2 *s);
};

#endif // SYNCSOURCELOADER_H
