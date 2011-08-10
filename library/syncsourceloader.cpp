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
    QStringList uuidList;
    QHash<QString, int> pluginsHash;
    QStringList syncSourcesList;
};

SyncSourceLoader::SyncSourceLoader(QObject* parent)
    : QObject(parent),
      d(new SyncSourceLoader::Private)
{
    
}

QString SyncSourceLoader::generateUUID(const QString& name) const
{
    QString uuid = name; // let's mangle this
    QRegExp regExp("*_\\d+"); // anything followed by an underscore and digit
    if (!regExp.exactMatch(uuid)) {
        uuid += "_0";
    }
    
//     KConfigGroup cfg = KGlobal::config()->group("GlobalSourcesSettings");
//     QStringList uuidList = cfg.readEntry("uuid-list", QStringList());
    
    if (d->uuidList.contains(uuid)) {
        QStringList tempUUID = uuid.split("_");
        QString lastDigit = tempUUID.takeLast();
        tempUUID.append(QString::number(lastDigit.toInt()+1));
        uuid = tempUUID.join("_");
    }
    
    kDebug() << "New source uuid:" << uuid;
    d->uuidList.append(uuid);
    return uuid;
//     cfg.writeEntry("uuid-list", uuidList);
//     cfg.sync();
}

void SyncSourceLoader::loadAllSyncSources()
{
    
    KConfigGroup cfg = KGlobal::config()->group("GlobalSourcesSettings");
    d->syncSourcesList = cfg.readEntry("syncSourceList", QStringList());
    
    KServiceTypeTrader* trader = KServiceTypeTrader::self();
    KService::List services = trader->query("Akunambol/SyncSource");
    
    foreach (const KService::Ptr &service, services) {
        for (int i = 0; i < d->pluginsHash[service->name()]; i++) {
            QString error;

            KPluginFactory *factory = KPluginLoader(service->library()).factory();

            if (!factory) {
                //KMessageBox::error(0, i18n("<html><p>KPluginFactory could not load the plugin:<br/><i>%1</i></p></html>",
                //                         service->library()));
                kError(5001) << "KPluginFactory could not load the plugin:" << service->library();
                continue;
            }

            SyncSource2 *plugin = factory->create<SyncSource2>(this);
            if (plugin) {
                plugin->setUUID( QString("%1_%2").arg(service->name(), d->pluginsHash[service->name()]) );
                kDebug() << "Load plugin:" << plugin->uuid();
                
                emit syncSourceLoaded(plugin);
            } else {
                kDebug() << error;
            }
        }
//         if (!d->seenPlugins.contains(service->name()))  {
//             d->pluginCount[service->name()] = 0;
//         }
//         for (int i = 0; )
    }
}

void SyncSourceLoader::loadPlugin(const KService::Ptr* &service)
{

}

void SyncSourceLoader::loadSyncSource(const QString& name)
{

}

