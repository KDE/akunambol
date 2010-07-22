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

#define K_EXPORT_AKUNAMBOL_SYNCSOURCE(libname, classname) \
K_PLUGIN_FACTORY(factory, registerPlugin<classname>();) \
K_EXPORT_PLUGIN(factory("akunambol_syncsource_" #libname)) \
K_EXPORT_PLUGIN_VERSION(VERSION)

#warning fix version


class KDE_EXPORT SyncSource : public QObject
{
    Q_OBJECT
    
    public:
        explicit SyncSource(QObject* parent = 0, const QVariantList &args = QVariantList());
        virtual ~SyncSource();
        
        void setConfig(SyncConfig *c);
        
        /*
         * Reimplement this function.
         * Use the signals to notify the UI.
         */
        void doSync();
        
    signals:
        void newStatus(QString);
        void error(QString);
        void success();
        
    private:
        class SyncSourcePrivate;
        SyncSourcePrivate *d;
};

#endif // SYNCSOURCE_H
