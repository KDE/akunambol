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

void SyncSourceLoader::loadAllSyncSources()
{
    KServiceTypeTrader* trader = KServiceTypeTrader::self();
    
    KService::List services = trader->query("Akunambol/SyncSource");
    KService::List::const_iterator iter;
    
    foreach (const KService::Ptr &service, services) {
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
            kDebug() << "Load plugin:" << service->name();
            emit syncSourceLoaded(plugin);
        } else {
            kDebug() << error;
        }
    }
}
