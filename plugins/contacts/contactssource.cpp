/*
    Copyright (C) 2010-2011  Riccardo Iaconelli <riccardo@kde.org>

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

#include "contactssource.h"
#include "contactsbackend.h"

#include <funambolconfig.h>

#include <KDebug>
#include <QWidget>

ContactsSource::ContactsSource(QObject* parent, const QVariantList& args)
    : FunambolSyncSource(parent, args)
{
    
    setSourceUID("contacts-test-stub-plugin");
    setSyncMimeType("application/base64"); // test
    
    kDebug() << getConfig();

//     setBackend(new ContactsBackend);
    setRemoteURI("http://localhost", Base64);
}

ContactsSource::~ContactsSource()
{

}

QWidget* ContactsSource::configurationInterface()
{
    return new QWidget;
}

QString ContactsSource::controlText() const
{
    return "Sync contacts";
}


void ContactsSource::doSync()
{
    ContactsBackend *backend = new ContactsBackend("contacts-test-stub-plugin",
                                   dynamic_cast<Funambol::AbstractSyncSourceConfig *>(getConfig()));
    setBackend(backend);
//     kDebug() << 
    kDebug() << "sync";
}



EXPORT_AKUNAMBOL_SYNCSOURCE(contacts, ContactsSource)
