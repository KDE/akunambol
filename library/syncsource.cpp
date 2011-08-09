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

#include "syncsource.h"
#include "syncjob.h"

#include <QMutex>
#include <QTimer>

class SyncSource2::SyncSourcePrivate {
    public:
        SyncSourcePrivate(SyncSource2 *q) : q(q), config(0) {}
        SyncSource2 *q;
        SyncCredentials *config;
        QMutex lock;
        QString statusMessage;
        SyncSource2::SyncStatus status;
};

SyncSource2::SyncSource2(QObject* parent, const QVariantList& args)
    : QObject(parent),
    d(new SyncSourcePrivate(this))
{
    d->status = NoSync;
    qRegisterMetaType<SyncSource2::SyncStatus>();
    Q_UNUSED(args);
}

SyncSource2::~SyncSource2()
{
    delete d;
}

void SyncSource2::triggerSync()
{
    setStatus(SyncStarted); // TODO: refactor this code: is locking really necessary?
                            // we should do that with the jobs
    if (tryLock()) {
        syncJob()->start(); // TODO: make me run in another thread
        connect(syncJob(), SIGNAL(finished(KJob*)), this, SLOT(handleJobResult(KJob*)));
        unlock();
    } else {
        setStatus(SyncError);
        setStatusMessage(i18n("A synchronization is already in progress."));
    }
}

void SyncSource2::handleJobResult(KJob *job)
{
    if (job->error()) {
        kDebug() << "Sync has finished with an error.";
        setStatus(SyncError);
        setStatusMessage(job->errorString());
    } else {
        kDebug() << "Sync has finished successfully.";
        setStatus(SyncSuccess);
        setStatusMessage(i18n("Synchronization successful."));
    }
    job->deleteLater();
}

void SyncSource2::setStatus(SyncSource2::SyncStatus _newStatus)
{
    if (_newStatus == d->status) {
        return;
    }
    
    d->status = _newStatus;
    
    if (_newStatus != NoSync) {
        emit newStatus(_newStatus);
    }
    
    if (_newStatus == SyncError || _newStatus == SyncSuccess) {
        QMetaObject::invokeMethod(this, "setStatus", Qt::QueuedConnection, Q_ARG(SyncSource2::SyncStatus, NoSync));
    }
}

void SyncSource2::setStatusMessage(const QString &newMessage)
{
    if (newMessage == d->statusMessage) {
        return;
    }
        
    d->statusMessage = newMessage;
    emit newStatusMessage(newMessage);
}

SyncSource2::SyncStatus SyncSource2::status()
{
    return d->status;
}

QString SyncSource2::statusMessage()
{
    return d->statusMessage;
}

bool SyncSource2::tryLock()
{
    return d->lock.tryLock();
}

void SyncSource2::lock()
{
    d->lock.lock();
}

void SyncSource2::unlock()
{
    d->lock.unlock();
}

void SyncSource2::setCredentials(SyncCredentials* c)
{
    d->config = c;
}

SyncCredentials* SyncSource2::credentials()
{
    return d->config;
}

void SyncSource2::configChanged()
{

}

#include "syncsource.moc"

