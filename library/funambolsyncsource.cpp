/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "funambolsyncsource.h"

// Funambol
#include "base/fscapi.h"
#include "spdm/DMTreeFactory.h"
#include "spds/DefaultConfigFactory.h"
#include "spds/SyncItem.h"

// Qt/KDE
#include <QWidget>

// Akunambol
#include <aku-auto-config.h>


FunambolSyncSouceConfig::FunambolSyncSouceConfig()
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

bool FunambolSyncSouceConfig::read()
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

bool FunambolSyncSouceConfig::save()
{
    if ( !DMTClientConfig::save() ) {
        return false; // error in the common config save.
    }
    if (!open()) {
        return false;
    }
    
    // Write client-specific properties to the config
    ManagementNode *node = dmt->readManagementNode(rootContext);
    if (node) {
        delete node;
        close();
        return true; // success!
    }
    
    close();
    return false; // failure :(
}

void FunambolSyncSouceConfig::createConfig()
{
    Funambol::AccessConfig* ac = Funambol::DefaultConfigFactory::getAccessConfig();
    ac->setMaxMsgSize(60000);
    ac->setUserAgent("Akunambol " AKU_VERSION);
    
    this->setAccessConfig(*ac);
    delete ac;
    
    Funambol::DeviceConfig* dc = Funambol::DefaultConfigFactory::getDeviceConfig();
    dc->setDevID(generateDeviceID());
    dc->setMan("Funambol");
    dc->setLoSupport(true);
    dc->setSwv(AKU_VERSION);
    this->setDeviceConfig(*dc);
    delete dc;
    
    // Create a node for this specific source
    
    // Configure the source to work with vCard 2.1
    Funambol::SyncSourceConfig* sc = Funambol::DefaultConfigFactory::getSyncSourceConfig(KFUNSYNC_SOURCE_NAME);
    sc->setType     ("text/x-vcard");
    sc->setURI      ("card");
    sc->setEncoding (Funambol::SyncItem::encodings::plain);
    this->setSyncSourceConfig(*sc);
    delete sc;
    
    // save the configuration
    save();
}

class FunambolManagerPrivate
{
    public:
        FunambolManagerPrivate() {
            config = new FunambolSyncSouceConfig;
        }
        
        FunambolSyncSouceConfig *config;
        QString name;

};

// TODO make me a thread?
FunambolSyncSource::FunambolSyncSource (QObject* parent, const QVariantList& args)
        : SyncSource2 (parent, args)
{
    d = new FunambolManagerPrivate;
}

FunambolSyncSource::~FunambolSyncSource()
{

}

void FunambolSyncSource::setSyncData (QString username, QString password, QString url)
{
    d->config;
}

void FunambolSyncSource::setSourceUID (QString uid)
{
    d->name = "aku-" + uid;
}

void FunambolSyncSource::doSync()
{

}

QWidget* FunambolSyncSource::configurationInterface()
{
    return (new QWidget);
}

// kate: indent-mode cstyle; space-indent on; indent-width 4; replace-tabs on; 
