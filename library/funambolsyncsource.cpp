/*
    Akunambol - KDE client for syncing your data
    Copyright (C) 2010-2011  Riccardo Iaconelli <riccardo@kde.org>

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
#include "spds/SyncItem.h"
#include "client/SyncClient.h"

// Qt/KDE
#include <QWidget>

// Akunambol
#include "akunambol_macros.h"
#include "funambolsyncjob.h"
#include "funambolconfig.h"

class FunambolSyncSource::Private
{
public:
    Private(FunambolSyncSource *parent) {
        this->parent = parent;
        config = new FunambolConfig;
        backend = 0;
        client = new Funambol::SyncClient;
    }

    void initConfig() { // TODO: init only if we haven't before.
        // These parameters are set from the plugin developer, and are mandatory.
        // This is a safety net so that the developer is warned, and has a clue, when
        // he finds out that nothing works as expected.
        if (sourceName.isEmpty() ||
                syncMimeType.isEmpty() ||
                remoteURI.isEmpty()) {
            qFatal("Dear fellow developer, the mandatory parameters (sourceUID, syncType,"
                   "remoteURI) are not set. This will screw things up.");
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

    FunambolSyncSource *parent;
    FunambolConfig *config;
    QString sourceName, syncMimeType, remoteURI;
    FunambolSyncSource::Encoding encoding;
    FunambolBackend *backend;
    Funambol::SyncClient *client;
    FunambolSyncJob *job;
};

// -------------------
// End of private classes
// -------------------

// TODO make me a thread?
FunambolSyncSource::FunambolSyncSource(QObject* parent, const QVariantList& args)
        : SyncSource2(parent, args),
        d(new FunambolSyncSource::Private(this))
{
}

FunambolSyncSource::~FunambolSyncSource()
{
    delete d;
}

void FunambolSyncSource::setCredentials(SyncCredentials *c)
{
    // These values are set from the user. We could fail here, but it's not particularly important.
    // These values should be set at every execution, but this (and error reporting) is already
    // taken care of by FunambolSyncSource::doSync()
    d->config->getAccessConfig().setUsername(c->user().toUtf8());
    d->config->getAccessConfig().setPassword(c->password().toUtf8());
    d->config->getAccessConfig().setSyncURL(c->syncUrl().toUtf8());
    SyncSource2::setCredentials(c);
}

void FunambolSyncSource::setSourceUID(const QString &uid)
{
    d->sourceName = "aku-" + uid;
}

void FunambolSyncSource::setRemoteURI(const QString &uri, Encoding encoding)
{
    d->remoteURI = uri;
    d->encoding = encoding;
}

void FunambolSyncSource::setSyncMimeType(const QString &mimeType)
{
    d->syncMimeType = mimeType;
}

void FunambolSyncSource::setBackend(FunambolBackend* backend)
{
    d->backend = backend;
}

void FunambolSyncSource::doSync()
{
    // The config() object is manipulated from the private class, and is not used directly.
    // FIXME: is this a good thing? This is not very elegant, so Riccardo accepts suggestions
    if (!credentials()->isComplete()) {
        setStatus(SyncError);
        setStatusMessage(i18n("Please set your credentials and synchronization URL."));
        return; // TODO: maybe this should be moved to be handled from the individual sources?
    } else if (!d->backend) {
        qFatal("No backend set. This is a -very- bad thing.");
        return;
    }

    d->initConfig(); // read and eventually initialize the configuration.

    //     const char* remoteUri = sourceConfig->getRemoteUri();
//     if (remoteUri == NULL || strlen(remoteUri) == 0) {
//         sourceConfig->setRemoteUri(srcConfig->getURI());
//         sourceConfig->save();
//     } else {
//         srcConfig->setURI(remoteUri);
//     }
    
    Funambol::SyncSource* ssArray[] = { d->backend, NULL } ;
    
    if (d->client->sync(*(d->config), ssArray)) {
        setStatus(SyncError);
        setStatusMessage(i18n("Sync failed.")); // TODO: we need finer grained errors
    }
    
    d->config->save();
    setStatus(SyncSuccess);
    
    

//     AkonadiSource *source = appSource->getSyncSource();
//     AppSyncSourceConfig* sourceConfig = appSource->getConfig();
//     source->setCollectionId(sourceConfig->getCollectionId());
//     SyncSourceConfig *srcConfig = d->config->getSyncSourceConfig(d->sourceName);
//     if (srcConfig != NULL) {
//         source->setConfig(srcConfig);
//     }
    
//     const char* remoteUri = sourceConfig->getRemoteUri();
//     if (remoteUri == NULL || strlen(remoteUri) == 0) {
//         sourceConfig->setRemoteUri(srcConfig->getURI());
//         sourceConfig->save();
//     } else {
//         srcConfig->setURI(remoteUri);
//     }
//
//     SyncSource* ssArray[] = { source, NULL } ;
//     if (client->sync(*KFunSyncConfig::getInstance(), ssArray)) {
//         LOG.error("Error during sync.\n");
//     }
//
//     // Save the anchors
//     KFunSyncConfig::getInstance()->save();
//     manager->emitSourceEnded(appSource, client->getSyncReport());

}

SyncJob* FunambolSyncSource::syncJob()
{
    return d->job; //FIXME
}

QWidget* FunambolSyncSource::configurationInterface()
{
    return (new QWidget);
}

// kate: indent-mode cstyle; space-indent on; indent-width 0;  replace-tabs on;
