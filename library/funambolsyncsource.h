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


#ifndef FUNAMBOLSYNCSOURCE_H
#define FUNAMBOLSYNCSOURCE_H

#include "syncsource.h"

#include <client/DMTClientConfig.h>

class FunambolManagerPrivate;
class QWidget;

class KDE_EXPORT FunambolSyncSource : public SyncSource2
{
    public:
        FunambolSyncSource (QObject* parent = 0, const QVariantList& args = QVariantList());
        virtual ~FunambolSyncSource();

        void setSyncData (QString username, QString password, QString url);
        void setSourceUID (QString uid);

        virtual QString controlText() = 0;
        virtual QWidget* configurationInterface();

    protected:
        virtual void doSync();

    private:
        FunambolManagerPrivate *d;

};

class FunambolSyncSouceConfig : public Funambol::DMTClientConfig
{

    public:
        FunambolSyncSouceConfig();
        ~FunambolSyncSouceConfig();

        // Overloaded methods from DMTClientConfig
        virtual bool read();
        virtual bool save();
 
        // Initialize the config: try to read it from file or generate a default one.
        void init();

    private:
        // Generate a default config
        void createConfig();
};


#endif // FUNAMBOLSYNCSOURCE_H
// kate: indent-mode cstyle; space-indent on; indent-width 4; replace-tabs on;  replace-tabs on;  replace-tabs on;
