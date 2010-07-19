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

#include "statuslistener.h"
#include <KLocalizedString>

using namespace Funambol;

void StatusListener::syncBegin(SyncEvent& event)
{
    Funambol::SyncListener::syncBegin(event);
    emit newStatus(i18n("Beginning syncronization..."));
}

void StatusListener::sendInitialization(SyncEvent& event)
{
    Funambol::SyncListener::sendInitialization(event);
    emit newStatus(i18n("Connecting..."));
}

void StatusListener::syncEnd(SyncEvent& event)
{
    Funambol::SyncListener::syncEnd(event);
    emit newStatus(i18n("Syncronization finished."));
}

void StatusListener::sendFinalization(SyncEvent& event)
{
    Funambol::SyncListener::sendFinalization(event);
    emit newStatus(i18n("Finalizing..."));
}

void StatusListener::sendModifications(SyncEvent& event)
{
    Funambol::SyncListener::sendModifications(event);
    emit newStatus(i18n("Sending the modifications"));
}

void StatusListener::syncError(SyncEvent& event)
{
    Funambol::SyncListener::syncError(event);
    emit error(i18n("Syncronization error. The problem is:\n%1", event.getMessage()));
}

#include "statuslistener.moc"
