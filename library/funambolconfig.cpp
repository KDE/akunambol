/*
    Akunambol - KDE client for syncing your data
    Copyright (C) 2010-2011  Riccardo Iaconelli <riccardo@kde.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "funambolconfig.h"

#include "spdm/DMTreeFactory.h"
#include "spds/DefaultConfigFactory.h"

#include <aku-auto-config.h>

void FunambolConfig::init()
{
    // Read the configuration. If not found, generate a default one
    if (!read()) {
        createConfig();
    }

    // Handle backward compatibility: if the stored version is
    // different from the current one, take the proper action
    if (strcmp(this->getClientConfig().getSwv(), AKU_VERSION) != 0) {
    }
}

bool FunambolConfig::read()
{
    if (!DMTClientConfig::read()) {
        return false; // error in the common config read.
    }
    if (!open()) {
        return false;
    }

    // Read client-specific properties from the config
    Funambol::ManagementNode *node = dmt->readManagementNode(rootContext);

    if (node) {
        delete node;
        close();
        return true; // success!
    }

    close();
    return false; // failure :(
}

bool FunambolConfig::save()
{
    if (!DMTClientConfig::save()) {
        return false; // error in the common config save.
    }
    if (!open()) {
        return false;
    }

    // Write client-specific properties to the config
    Funambol::ManagementNode *node = dmt->readManagementNode(rootContext);
    if (node) {
        delete node;
        close();
        return true; // success!
    }

    close();
    return false; // failure :(
}

void FunambolConfig::createConfig()
{
    Funambol::AccessConfig* ac = Funambol::DefaultConfigFactory::getAccessConfig();
    ac->setMaxMsgSize(60000);
    ac->setUserAgent("Akunambol " AKU_VERSION);

    this->setAccessConfig(*ac);
    delete ac;

    Funambol::DeviceConfig* dc = Funambol::DefaultConfigFactory::getDeviceConfig();
    dc->setDevID(AkuGlobal::deviceUid().toUtf8());
    dc->setMan("Funambol");
    dc->setLoSupport(true);
    dc->setSwv(AKU_VERSION);
    this->setDeviceConfig(*dc);
    delete dc;

    // Create a node for this specific source
    Funambol::SyncSourceConfig* sc = Funambol::DefaultConfigFactory::getSyncSourceConfig(m_sourceName.toUtf8());
    sc->setType(m_syncMimeType.toUtf8());
    sc->setURI(m_remoteURI.toUtf8());
    sc->setEncoding(m_encoding);
    this->setSyncSourceConfig(*sc);
    delete sc;

    // save the configuration
    save();
}
