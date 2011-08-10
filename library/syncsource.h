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

#ifndef SYNCSOURCE_H
#define SYNCSOURCE_H

#include <QtCore/QObject>
#include <kdemacros.h>
#include <KGenericFactory>
#include <KJob>
#include <KConfigDialog>

class SyncJob;
class SyncCredentials;
class QWidget;

#warning fix version

class KDE_EXPORT SyncSource2 : public QObject
{
    Q_OBJECT
    
    public:
        /**
        * Status of the synchronization
        */
        enum SyncStatus {
            NoSync = 1, /** No synchronization is in progess. **/
            SyncStarted = 2, /** A synchronization has started. **/
            SyncError = 4, /** An error has happened. After this status is set,
                                it will be changed immediately to NoSync **/
            SyncSuccess = 8 /** Synchronization successful. After this status
                                is set, it will be changed immediately to NoSync **/
        };
    
        SyncSource2(QObject* parent = 0, const QVariantList &args = QVariantList());
        ~SyncSource2();
        
        virtual void setCredentials(SyncCredentials *c);
        virtual SyncCredentials* credentials();
        
        // TODO: return a KPluginInfo instead
        /**
         * Returns the text that should be shown to the user in order to activate
         * this control.
         * 
         * Example: "Synchronize contacts"
         */
        virtual QString controlText() const = 0;
        
        /**
         * @brief Sets the status of the source as "locked", aka a sync is in progress.
         * Will deadlock if already locked. Also see documentation from QMutex, which is used
         * internally.
         */
        void lock();
        /**
         * Unlocks the source, so that it can be reused. Unlocks the QMutex, internally.
         */
        void unlock();
        /**
         * Tries to lock the syncsource. Operates in the same way as you can see in
         * the QMutex documentation.
         */
        bool tryLock();
        
        /**
         * Reimplement to provide a configuration interface to the user.
         */
//         virtual QWidget* configurationInterface() = 0;
        virtual void createConfigurationInterface(KConfigDialog *parent) = 0;
        
        // TODO: do like Plasma::Applet here: KConfig config();
        
        SyncStatus status();
        QString statusMessage() const;
        
        /**
         * Return a KJob that represents the synchronization.
         */
        virtual SyncJob *syncJob() = 0;
        
        /**
         * Returns a UUID that uniquely identifies the SyncSource. It is guaranteed
         * to be unique.
         */
        QString uuid() const;
        
    public Q_SLOTS:
        /**
         * Triggers a sync. It will fail with a meaningful error if the source is already locked.
         */
        void triggerSync();
        
    protected Q_SLOTS:
        void setStatus(SyncSource2::SyncStatus newStatus);
        void setStatusMessage(const QString& newMessage);
        
        /**
         * configChanged() is called whenever the configuration has changed (such as the user
         * clicked on the apply button of the configure dialog).
         */
        virtual void configChanged();
        
    Q_SIGNALS:
        /**
         * This signal is emitted whenever we have a new message state.
         */
        void newStatusMessage(QString);
        /**
         * This signal is emitted whenever we have a new status, except for "NoSync".
         * If you want to see if a synchronization is finished check for SyncError or SyncSuccess.
         */
        void newStatus(SyncSource2::SyncStatus);
    private Q_SLOTS:
        void handleJobResult(KJob *);
        
    private:
        friend class SyncSourceLoader; // needed to set the UUID -only- from there
        void setUUID(const QString &uuid);
        
        class SyncSourcePrivate;
        SyncSourcePrivate * const d;
};

Q_DECLARE_METATYPE(SyncSource2::SyncStatus)

#define EXPORT_AKUNAMBOL_SYNCSOURCE(libname, classname) \
K_PLUGIN_FACTORY(SyncSourceFactory, registerPlugin<classname>();) \
K_EXPORT_PLUGIN(SyncSourceFactory("akunambol_syncsource_" #libname)) \
K_EXPORT_PLUGIN_VERSION(0.1)


#endif // SYNCSOURCE_H
