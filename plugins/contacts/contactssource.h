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

#ifndef CONTACTSSOURCE_H
#define CONTACTSSOURCE_H

#include <QObject>
#include <funambolsyncsource.h>

class ContactsSource : public FunambolSyncSource
{
    Q_OBJECT
    public:
        ContactsSource(QObject* parent = 0, const QVariantList& args = QVariantList());
        ~ContactsSource();
        
        QString controlText();
        
        QWidget* configurationInterface();
        
    protected:
        void doSync();
    
};


#endif // CONTACTSSOURCE2_H
