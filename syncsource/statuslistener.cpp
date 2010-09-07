/*
 * Copyright (C) 2010 Riccardo Iaconelli <riccardo@kde.org>
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License version 3 as published by
 * the Free Software Foundation with the addition of the following permission
 * added to Section 15 as permitted in Section 7(a): FOR ANY PART OF THE COVERED
 * WORK IN WHICH THE COPYRIGHT IS OWNED BY FUNAMBOL, FUNAMBOL DISCLAIMS THE
 * WARRANTY OF NON INFRINGEMENT  OF THIRD PARTY RIGHTS.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, see http://www.gnu.org/licenses or write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA.
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
