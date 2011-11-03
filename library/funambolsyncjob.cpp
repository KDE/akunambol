/*
    Akunambol - KDE client for syncing your data
    Copyright (C) 2011  Riccardo Iaconelli <riccardo@kde.org>

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


#include "funambolsyncjob.h"

// Funambol
#include "client/SyncClient.h"

#include "funambolconfig.h"
#include "funambolbackend.h"

#include <KDebug>
#include <KLocalizedString>
#include <QTimer>

class FunambolSyncJob::Private {
public:
    FunambolBackend *backend;
    FunambolConfig *config;
    Funambol::SyncClient *client;
};

FunambolSyncJob::FunambolSyncJob(QObject* parent)
    : SyncJob(parent),
    d(new FunambolSyncJob::Private)
{
    d->backend = 0;
    d->config = 0;
    
    setCapabilities(KJob::NoCapabilities); // TODO: at a later time, see if we can suspend the sync or kill it
}

FunambolBackend* FunambolSyncJob::backend()
{
    return d->backend;
}

void FunambolSyncJob::setBackend(FunambolBackend* backend)
{
    d->backend = backend;
}

FunambolConfig* FunambolSyncJob::config()
{
    return d->config;
}

void FunambolSyncJob::setConfig(FunambolConfig* config)
{
    d->config = config;
}

void FunambolSyncJob::doStart()
{
    // FIXME: move all this into FunambolSyncSource
    
    // The config() object is manipulated from the private class, and is not used directly.
    // FIXME: is this a good thing? This is not very elegant, so Riccardo accepts suggestions
    if (!d->config->isComplete()) {
        
        kDebug() << "#1: Configuration incomplete";
        setError(ConfigIncomplete);
        setErrorText(i18n("The configuration is incomplete"));
        emitResult();
        //setStatus(SyncError);
        //setStatusMessage(i18n("Please set your credentials and synchronization URL."));
        return; // TODO: maybe this should be moved to be handled from the individual sources?
    } else if (!d->backend) {
        qFatal("No backend set. This is a -very- bad thing.");
//         setStatus(SyncError);
        return;
    }

    //d->initConfig(); // read and eventually initialize the configuration.

    //     const char* remoteUri = sourceConfig->getRemoteUri();
//     if (remoteUri == NULL || strlen(remoteUri) == 0) {
//         sourceConfig->setRemoteUri(srcConfig->getURI());
//         sourceConfig->save();
//     } else {
//         srcConfig->setURI(remoteUri);
//     }
    
    Funambol::SyncSource* ssArray[] = { d->backend, NULL } ;
    
    if (d->client->sync(*(d->config), ssArray)) {
        kDebug() << "Sync Error";
        setError(SyncError);
        emitResult();
        //setStatus(SyncError);
        //setStatusMessage(i18n("Sync failed.")); // TODO: we need finer grained errors
    }
    
    d->config->save();
    setError(SyncSuccess);
    emitResult();
}

void FunambolSyncJob::start()
{
    kDebug() << "Starting the sync job...";
    QTimer::singleShot(0, this, SLOT(doStart()));
}

#include "funambolsyncjob.moc"


