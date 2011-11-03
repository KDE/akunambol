/*
    Akunambol - KDE client for syncing your data
    Copyright (C) 2010-2011  Riccardo Iaconelli <riccardo@kde.org>

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


#ifndef FUNAMBOLSYNCSOURCE_H
#define FUNAMBOLSYNCSOURCE_H

#include "syncsource.h"
#include "funambolbackend.h"

//#include <spds/SyncItem.h>

class QWidget;
class FunambolConfig;

class KDE_EXPORT FunambolSyncSource : public SyncSource2
{
    public:
        enum Encoding {
            None = 1, /** No encoding will be made and the data will be sent as-is */
            Base64 = 2, /** The data will be base64 encoded during the transfer */
            EncryptedDES = 4  /** Data will be DES-encrypted, base64 encoded, and then transferred */
        };

        FunambolSyncSource(QObject* parent = 0, const QVariantList& args = QVariantList());
        virtual ~FunambolSyncSource();

        virtual void setCredentials(SyncCredentials *c);
        
        virtual QString controlText() const = 0;
        virtual QWidget* configurationInterface();
        
        virtual SyncJob* syncJob();

        virtual void createConfigurationInterface(KConfigDialog *parent);
        
    protected:
        
        FunambolConfig* getConfig();
        
        // TODO: implement a public getter for each setter, and vice versa
        void setSourceUID(const QString &uid);
        void setSyncMimeType(const QString &mimeType);
        void setBackend(FunambolBackend *backend);
        void setRemoteURI(const QString &uri, Encoding encodingType);

    private:
        class Private;
        FunambolSyncSource::Private *d;

};


#endif // FUNAMBOLSYNCSOURCE_H
// kate: indent-mode cstyle; space-indent on; indent-width 4; replace-tabs on;  replace-tabs on;  replace-tabs on;  replace-tabs on;
