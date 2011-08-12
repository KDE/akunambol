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

#include "syncsourceloader.h"
#include "syncsource.h"

#include <KDebug>
#include <KService>
#include <KServiceTypeTrader>

class SyncSourceLoader::Private {
public:
    /**
     * Holds the list of the sync sources loaded until now.
     */
    QStringList uuidList;
    
    QHash<QString, QString> pluginsHash;
    
    /**
     * Holds the list of the sync sources written on disk, in the config.
     */
    QStringList syncSourcesList;
};

SyncSourceLoader::SyncSourceLoader(QObject* parent)
    : QObject(parent),
      d(new SyncSourceLoader::Private)
{
    
}

// TODO: NEED_UNIT_TEST
QString SyncSourceLoader::generateUUID(const QString& name) const
{
    QString uuid = name; // we can't modify a const...
    
    QStringList sameName = d->uuidList.filter(uuid);
    
    if (!sameName.isEmpty()) {
        sameName.sort();
        QStringList tempUUID = sameName.last().split("_");
        QString lastDigit = tempUUID.takeLast();
        tempUUID.append(QString::number(lastDigit.toInt()+1));
        uuid = tempUUID.join("_");
    } else {
        uuid += "_0";
    }
    
    return uuid;
}

// TODO
void SyncSourceLoader::loadAllSyncSources()
{
    KConfigGroup cfg = KGlobal::config()->group("GlobalSourcesSettings");
    d->syncSourcesList = cfg.readEntry("syncSourceList", QStringList());
    
    KServiceTypeTrader* trader = KServiceTypeTrader::self();
    KService::List services = trader->query("Akunambol/SyncSource");
    
    foreach (const KService::Ptr &service, services) {
        
        int count = d->syncSourcesList.filter(service->name()).size();
        
        for (int i = 0; i < count; i++) {
            KPluginFactory *factory = KPluginLoader(service->library()).factory();

            if (!factory) {
                kError() << "KPluginFactory could not load the plugin:" << service->library();
                continue;
            }

            SyncSource2 *plugin = factory->create<SyncSource2>(this);
            if (plugin) {
                QString uuid = generateUUID(service->name());
                plugin->setUUID(uuid);
                d->uuidList.append(uuid);
                
                kDebug() << "Load plugin:" << plugin->uuid();
                
                emit syncSourceLoaded(plugin);
            }
        }
    }
}


// TODO
void SyncSourceLoader::loadSyncSource(const QString& name)
{

}

