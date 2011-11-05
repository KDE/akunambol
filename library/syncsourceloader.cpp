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

#include "syncsourceloader.h"
#include "syncsource.h"

// #include "boost/tuple/tuple.hpp"

#include <QHash>

#include <KDebug>
#include <KService>
#include <KServiceTypeTrader>

// typedef tuple<> wa;

class SyncSourceLoader::Private {
public:
    Private()
     : mainConfigGroup("AkuLibGlobalSourcesSettings")
    {}
    
    /**
     * Holds a map which contains, for every plugin, what has been the biggest
     * instance number ever seen. Saved and loaded from the configuration.
     */
    QHash<QString, int> biggestInstanceNumberKnown;
    
    /**
     * Holds the list of the sync sources written on disk, in the configuration.
     * It is a list of the uids directly, the plugin name is stored in a subgroup.
     */
    QStringList savedSyncSources;
    
    /**
     * List of Akunambol Syncsources, as KService(s)
     */
    KService::List services;
    
    /**
     * the name of the KConfigGroup used to store global configuration
     */
    const QString mainConfigGroup;
    
    void saveConfig();
    void loadConfig();
};

void SyncSourceLoader::Private::loadConfig()
{
    KConfigGroup cfg = KGlobal::config()->group(mainConfigGroup);
 
    QByteArray ba = cfg.readEntry("biggestInstanceNumberKnownHash", QByteArray());   
    QDataStream ds(&ba, QIODevice::ReadOnly);
    ds.setVersion(QDataStream::Qt_4_7);
    ds >> biggestInstanceNumberKnown;
    
    savedSyncSources = cfg.readEntry("syncSources", QStringList());
}

void SyncSourceLoader::Private::saveConfig()
{
    KConfigGroup cfg = KGlobal::config()->group(mainConfigGroup);
    
    QByteArray ba;
    QDataStream ds(&ba, QIODevice::WriteOnly);
    ds.setVersion(QDataStream::Qt_4_7);
    ds << biggestInstanceNumberKnown;
    
    cfg.writeEntry("biggestInstanceNumberKnownHash", ba);
    cfg.writeEntry("syncSources", savedSyncSources);
    
    cfg.sync();
}

SyncSourceLoader::SyncSourceLoader(QObject* parent)
    : QObject(parent),
      d(new SyncSourceLoader::Private)
{
    d->loadConfig();
}

SyncSourceLoader::~SyncSourceLoader()
{
    d->saveConfig();
}

void SyncSourceLoader::loadAllSavedSyncSources()
{   
    foreach (const QString &uid, d->savedSyncSources) {
        KConfigGroup sourceConfig = KGlobal::config()->group(d->mainConfigGroup).group(uid);
        
        QString name = sourceConfig.readEntry("Plugin name", QString());
        int instance = sourceConfig.readEntry("Instance Counter", -1);
        
        loadPlugin(name, uid, instance);
    }
}

void SyncSourceLoader::loadNewSyncSource(const QString &library)
{
    // instanceID is always 1 if it's the first time we load a syncsource of that
    // type (QHash has 0 as default value).
    int instanceID = d->biggestInstanceNumberKnown[library]+1;
    QString uid = QString("%1_%2").arg(library).arg(QString::number(instanceID));
        
    if (loadPlugin(library, uid, instanceID)) {
        // The plugin has been successfully loaded and the correct signals have been emitted.
        // Let's reflect the fact that we have a new source in internal data structures too.
        d->biggestInstanceNumberKnown[library]++;
        d->savedSyncSources.append(uid);
        // Create a new configuration group for the source, also write the needed data.
        KConfigGroup config = KGlobal::config()->group(d->mainConfigGroup).group(uid);
        config.writeEntry("Plugin name", library);
        config.writeEntry("Instance Counter", instanceID);
        config.sync();
        d->saveConfig();
    }
}

void SyncSourceLoader::removeSyncSource(const QString& uid)
{
    d->savedSyncSources.removeAll(uid);
    KConfigGroup config = KGlobal::config()->group(d->mainConfigGroup);
    config.deleteGroup(uid);
    config.sync();
    d->saveConfig();
}

void SyncSourceLoader::refreshSyncSourcesList()
{
    KServiceTypeTrader* trader = KServiceTypeTrader::self();
    d->services = trader->query("Akunambol/SyncSource");
}

QList< KPluginInfo > SyncSourceLoader::syncSourcesInfo()
{
    QList<KPluginInfo> list;
    
    // Populate the list of services in case it is empty.
    if (d->services.isEmpty()) {
        refreshSyncSourcesList();
    }
    
    foreach (const KService::Ptr &service, d->services) {
        list.append(KPluginInfo(service));
    }
    return list;
}

bool SyncSourceLoader::loadPlugin(const QString& name, const QString &uid, int instanceID)
{
    kDebug() << "Trying to load" << name << "plugin. UID =" << uid << "and instance =" << instanceID;
    
    if (name.isEmpty() || instanceID == -1) {
        kError() << "Invalid plugin:" << uid << "; name =" << name << "instance =" << instanceID;
        return false;
    }
    
    // Populate the list of services in case it is empty.
    if (d->services.isEmpty()) {
        refreshSyncSourcesList();
    }
        
    // It's somewhat inefficient to go through all the plugins all the time, but it allows for safer code,
    // and we don't expect to find more than a few dozens of plugins at maximum anyways.
    // This should be almost instantaneous, but you can come back and optimize me at a later time,
    // when you are sure you're not breaking stuff.
    
    foreach (const KService::Ptr &service, d->services) {
        
        kDebug() << "Got service" << service->library();
        
        if (service->library() != name) {
            continue;
        }
        
        KPluginFactory *factory = KPluginLoader(service->library()).factory();

        if (!factory) {
            kError() << "KPluginFactory could not load the plugin:" << service->library();
            continue;
        }

        SyncSource2 *plugin = factory->create<SyncSource2>(this);
        
        if (plugin) {
            plugin->setUID(uid);
            // TODO write a plugin->setConfig() or something like that?
            
            kDebug() << "Loaded plugin:" << plugin->uid();
            emit syncSourceLoaded(plugin);
            
            return true;
        }
    }
    
    return false;
}

