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

class SyncConfig;
class QWidget;

#warning fix version

class KDE_EXPORT SyncSource2 : public QObject
{
    Q_OBJECT
    
    public:
        SyncSource2(QObject* parent = 0, const QVariantList &args = QVariantList());
        ~SyncSource2();
        
        void setConfig(SyncConfig *c);
        
        /*
         * Returns the text that should be shown to the user in order to activate
         * this control.
         * 
         * Example: "Synchronize contacts"
         */
        virtual QString controlText() = 0;
        
        /*
         * Sets the status of the source as "locked", aka a sync is in progress.
         * Will deadlock if already locked.
         */
        void lock();
        /*
         * Unlocks the source.
         */
        void unlock();
        /*
         * Operates in the same way as you can see in the QMutex documentation.
         */
        bool tryLock();
        
        /*
         * Reimplement to provide a configuration interface to the user.
         */
        QWidget* configurationInterface();
        
    public slots:
        /*
         * Triggers a sync. It will fail with a meaningful error if the source is already locked.
         */
        void triggerSync();
        
    protected:
    
        /*
        * This function is called when the user, or any other event, triggers a sync.
        * This should launch the sync until finished, and should use the signals to notify the UI.
        */
        virtual void doSync() = 0;
    
    signals:
        void newStatus(QString);
        void error(QString);
        void success();
        
        /*
         * Internally used
         */
        void started();
        
    private:
        class SyncSourcePrivate;
        SyncSourcePrivate *d;
};


#define EXPORT_AKUNAMBOL_SYNCSOURCE(libname, classname) \
K_PLUGIN_FACTORY(SyncSourceFactory, registerPlugin<classname>();) \
K_EXPORT_PLUGIN(SyncSourceFactory("akunambol_syncsource_" #libname)) \
K_EXPORT_PLUGIN_VERSION(0.1)


#endif // SYNCSOURCE_H
