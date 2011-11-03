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
    
    /**
     * List of Akunambol Syncsources
     */
    KService::List services;
    
    /**
     * Function: name --> uuid that uses a custom encoding called "underscore".
     * 
     * Takes a generic name of a SyncSource, and basing on what has already
     * been loaded, returns a uuid.
     * 
     * @note syncSourcesList MUST NOT BE EMPTY
     */
    QString underscoreEncode(const QString &name) const;
    QString underscoreDecode(const QString &uuid) const;
};

QString SyncSourceLoader::Private::underscoreEncode ( const QString& name ) const
{
    if (syncSourcesList.isEmpty()) {
        kError() << "d->syncSourcesList must be filled before calling underscoreEncode!!!";
        return QString();
    }
    
    QString uuid = name; // we can't modify a const...
    
    QStringList sameName = uuidList.filter(uuid);
    
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

QString SyncSourceLoader::Private::underscoreDecode(const QString& uuid) const
{
    QStringList nameList = uuid.split("_");
    nameList.removeLast();
    return nameList.join("_");
}

SyncSourceLoader::SyncSourceLoader(QObject* parent)
    : QObject(parent),
      d(new SyncSourceLoader::Private)
{   
}

// TODO: NEED_UNIT_TEST
QString SyncSourceLoader::generateNewUUID(const QString& name) const
{
    return d->underscoreEncode(name);
}

// TODO
void SyncSourceLoader::loadAllSyncSources()
{
    KConfigGroup cfg = KGlobal::config()->group("GlobalSourcesSettings");
    d->syncSourcesList = cfg.readEntry("syncSourceList", QStringList());
    
    KServiceTypeTrader* trader = KServiceTypeTrader::self();
    d->services = trader->query("Akunambol/SyncSource");
    
    foreach (const QString &source, d->syncSourcesList) {
        loadSyncSource(source);   
    }
    
}


// TODO
void SyncSourceLoader::loadSyncSource(const QString& name)
{
    foreach (const KService::Ptr &service, d->services) {
        
        if (service->name() != name) {
            continue;
        }
        
        int count = d->syncSourcesList.filter(service->name()).size();
        
        for (int i = 0; i < count; i++) {
            KPluginFactory *factory = KPluginLoader(service->library()).factory();

            if (!factory) {
                kError() << "KPluginFactory could not load the plugin:" << service->library();
                continue;
            }

            SyncSource2 *plugin = factory->create<SyncSource2>(this);
            if (plugin) {
                QString uuid = generateNewUUID(service->name());
                plugin->setUUID(uuid);
                d->uuidList.append(uuid);
                
                kDebug() << "Load plugin:" << plugin->uuid();
                
                emit syncSourceLoaded(plugin);
            }
        }
    }
}

