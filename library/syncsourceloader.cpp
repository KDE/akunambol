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

#include <QMap>

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
    
    QHash<QString, int> biggestInstanceNumber;
    
    /**
     * Holds the list of the sync sources written on disk, in the config.
     * It is a list of the uids directly, the plugin name is stored in the config.
     */
    QStringList syncSources;
    
    /**
     * List of Akunambol Syncsources, as KService
     */
    KService::List services;
    
    /**
     * Function: name --> uuid that uses a custom encoding called "underscore".
     * 
     * Takes a generic name of a SyncSource, and basing on what has already
     * been loaded, returns a uuid.
     * 
     * @note syncSourcesList MUST NOT BE EMPTY
     * 
     * This is the inverse function of uniqueUnderscoreDecode.
     */
    QString uniqueUnderscoreEncode(const QString &name) const;
    
    /**
     * Function: uuid --> name that uses a custom encoding called "underscore".
     * 
     * Takes an uuid encoded with uniqueUnderscoreEncode and returns its generic name.
     * 
     * Inverse function of uniqueUnderscoreEncode.
     */
    QString uniqueUnderscoreDecode(const QString &uuid) const;
    
    static KConfigGroup configGroupFor(const QString &uid) {
        return KGlobal::config()->group("GlobalSourcesSettings").group(uid);
    }
};

QString SyncSourceLoader::Private::uniqueUnderscoreEncode(const QString& name) const
{
    if (syncSources.isEmpty()) {
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

QString SyncSourceLoader::Private::uniqueUnderscoreDecode(const QString& uuid) const
{
    QStringList nameList = uuid.split("_");
    nameList.removeLast();
    return nameList.join("_");
}

SyncSourceLoader::SyncSourceLoader(QObject* parent)
    : QObject(parent),
      d(new SyncSourceLoader::Private)
{
    KConfigGroup cfg = KGlobal::config()->group("GlobalSourcesSettings");
    
    // Store the list of sync sources (uid) into the d-pointer, for later usage
    d->syncSources = cfg.readEntry("syncSources", QStringList());
    d->biggestInstanceNumber = QHash<QString, int>();
}

// TODO: NEED_UNIT_TEST
QString SyncSourceLoader::generateNewUUID(const QString& name) const
{
    return d->uniqueUnderscoreEncode(name);
}

unsigned int SyncSourceLoader::countIdenticalSources(const QString& uuid) const
{
     QStringList result;
     int counter = 0;
     QString decodedUUID = d->uniqueUnderscoreDecode(uuid);
     
     // Decode all the list
//      foreach (const QString &str, d->syncSourcesList) {
//         result += d->uniqueUnderscoreDecode(str);
//      }
     
     foreach (const QString &str, result) {
         if (str.contains(decodedUUID)) {
             counter++;
         }
     }

     return counter;
}

// TODO
void SyncSourceLoader::loadAllSyncSources()
{
    KServiceTypeTrader* trader = KServiceTypeTrader::self();
    d->services = trader->query("Akunambol/SyncSource");
    
    foreach (const QString &uid, d->syncSources) {
        KConfigGroup sourceConfig = Private::configGroupFor(uid);
        
        QString name = sourceConfig.readEntry("Plugin name", QString());
        int instance = sourceConfig.readEntry("Instance Counter", -1);
        
        loadPlugin(name, uid, instance);
    }
}

void SyncSourceLoader::loadSyncSource(const QString& name)
{

}

void SyncSourceLoader::loadPlugin(const QString& name, const QString &uid, int instance)
{
    if (name.isEmpty() || instance == -1) {
        kError() << "Invalid plugin:" << uid << "; name =" << name << "instance =" << instance;
        return;
    }
    
    // It's somewhat inefficient to go through all the plugins all the time, but it allows for safer code,
    // and we don't expect to find more than a few dozens of plugins at maximum anyways.
    // You can come back and optimize me at a later time, when you are sure you're not breaking stuff.
    
    foreach (const KService::Ptr &service, d->services) {
        
        if (service->name() != name) {
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
            
            // NOTE: in case the key is not there yet the value is automatically assigned to 0
            if (instance > d->biggestInstanceNumber[name]) {
                d->biggestInstanceNumber[name] = instance;
            }

            kDebug() << "Loaded plugin:" << plugin->uid();
            
            emit syncSourceLoaded(plugin);
        }
    }
}

