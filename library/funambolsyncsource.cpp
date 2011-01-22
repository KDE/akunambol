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
#include "akunambol_macros.h"

void FunambolSyncSouceConfig::init()
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

void FunambolSyncSouceConfig::createConfig()
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

class FunambolManagerPrivate
{
    public:
        FunambolManagerPrivate() {
            config = new FunambolSyncSouceConfig;
        }
        
        void initConfig() {
            if (sourceName.isEmpty() ||
                syncMimeType.isEmpty() ||
                remoteURI.isEmpty()) {
                qFatal("Dear fellow developer, the mandatory parameters (sourceUID, syncType, remoteURI) are not set. This will screw things up.");
            } else {
                config->m_remoteURI = remoteURI;
                config->m_sourceName = sourceName;
                config->m_syncMimeType = syncMimeType;
                switch (encoding) {
                    case FunambolSyncSource::None:
                        config->m_encoding = Funambol::SyncItem::encodings::plain;
                    case FunambolSyncSource::Base64:
                        config->m_encoding = Funambol::SyncItem::encodings::escaped;
                    case FunambolSyncSource::EncryptedDES:
                        config->m_encoding = Funambol::SyncItem::encodings::des;
                    default: // even if we should never get here...
                        config->m_encoding = Funambol::SyncItem::encodings::plain;
                }
                config->init();
            }
        }

        FunambolSyncSouceConfig *config;
        QString sourceName, syncMimeType, remoteURI;
        FunambolSyncSource::Encoding encoding;
};

// TODO make me a thread?
FunambolSyncSource::FunambolSyncSource(QObject* parent, const QVariantList& args)
        : SyncSource2(parent, args)
{
    d = new FunambolManagerPrivate;
}

FunambolSyncSource::~FunambolSyncSource()
{

}

void FunambolSyncSource::setSyncData(QString username, QString password, QString url)
{
//     d->config;
}

void FunambolSyncSource::setSourceUID(QString uid)
{
    d->sourceName = "aku-" + uid;
}

void FunambolSyncSource::setRemoteURI(QString uri, Encoding encoding)
{
    d->remoteURI = uri;
    d->encoding = encoding;
}

void FunambolSyncSource::doSync()
{
    d->initConfig();

}

QWidget* FunambolSyncSource::configurationInterface()
{
    return (new QWidget);
}

// kate: indent-mode cstyle; space-indent on; indent-width 4; replace-tabs on;
