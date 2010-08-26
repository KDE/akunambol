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

#ifndef FUNAMBOLSYNCSOURCE_H
#define FUNAMBOLSYNCSOURCE_H

#include <QtCore/QObject>
#include <kdemacros.h>
#include <KGenericFactory>

#include "syncsource.h"

class SyncConfig;

#warning fix version

class KDE_EXPORT FunambolSyncSource : public SyncSource
{
    Q_OBJECT
    
    public:
        FunambolSyncSource(QObject* parent = 0, const QVariantList &args = QVariantList());
        virtual ~FunambolSyncSource() {}
        
        /*
         * Reimplement this function.
         * Use the signals to notify the UI.
         */
        void doSync();
};


#endif // FUNAMBOLSYNCSOURCE_H
