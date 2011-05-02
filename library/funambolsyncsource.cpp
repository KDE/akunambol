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
//#include "spds/SyncItem.h"

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
        config = new FunambolConfig;
        backend = 0;
    }
    
    ~Private() {
        delete config;
    }

    void initConfig() {
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
            
            config->init(); // This operation is safe: if you init() when you already have, this will do nothing.
        }
    }

    FunambolConfig *config;
    QString sourceName, syncMimeType, remoteURI;
    FunambolSyncSource::Encoding encoding;
    FunambolBackend *backend;
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
    SyncSource2::setCredentials(c);
    
    // These values are set from the user. We could fail here, but it's not particularly important.
    // These values should be set at every execution, but this (and error reporting) is already
    // taken care of by FunambolSyncSource::doSync()
    
    // FIXME: we should not write these things twice.
    
    d->config->getAccessConfig().setUsername(c->user().toUtf8());
    d->config->getAccessConfig().setPassword(c->password().toUtf8());
    d->config->getAccessConfig().setSyncURL(c->syncUrl().toUtf8());
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

SyncJob* FunambolSyncSource::syncJob()
{
    d->initConfig(); // Initialize the configuration (if it's needed)
    FunambolSyncJob *job = new FunambolSyncJob(this);
    job->setBackend(d->backend);
    job->setConfig(d->config);
    
    return job; //FIXME make sure this job is created, and initialized.
}

QWidget* FunambolSyncSource::configurationInterface()
{
    return (new QWidget);
}

// kate: indent-mode cstyle; space-indent on; indent-width 0;  replace-tabs on;
