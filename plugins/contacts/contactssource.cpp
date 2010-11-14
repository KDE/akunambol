/*
    Copyright (C) <year>  <name of author>

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
#include <syncsource.h>
#include <KDebug>

ContactsSource::ContactsSource(QObject* parent, const QVariantList& args)
    : SyncSource2(parent, args)
{
    
}

ContactsSource::~ContactsSource()
{

}

QString ContactsSource::controlText()
{
    return "Sync contacts";
}


void ContactsSource::doSync()
{
    kDebug() << "sync";
    emit success();
}



EXPORT_AKUNAMBOL_SYNCSOURCE(contacts, ContactsSource)
