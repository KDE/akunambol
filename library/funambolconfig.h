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


#ifndef FUNAMBOLCONFIG_H
#define FUNAMBOLCONFIG_H

#include <client/DMTClientConfig.h>
#include "synccredentials.h"

class QString;

class FunambolConfig : public Funambol::DMTClientConfig, public SyncCredentials
{

    public:
        FunambolConfig() {};

        // Overloaded methods from DMTClientConfig
        virtual bool read();
        virtual bool save();

        // Initialize the config: try to read it from file or generate a default one.
        void init();

        QString m_sourceName, m_syncMimeType, m_remoteURI;
        const char* m_encoding;

    private:
        // Generate a default config
        void createConfig();
};

#endif
