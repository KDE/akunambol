/*
 * Some code originally comes from the 'fsync' example to be found in Funambol's SDK.
 * 
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

#ifndef STATUSLISTENER_H
#define STATUSLISTENER_H

#include <event/SyncItemListener.h>
#include <event/SyncListener.h>

#warning remove
#include <cstdio>
#include <QObject>

class StatusListener : public QObject, public Funambol::SyncListener
{
    Q_OBJECT
    
    public:
        StatusListener(QObject *parent = 0) : QObject(parent), Funambol::SyncListener() {};

        void syncBegin(Funambol::SyncEvent& event);
        void syncEnd(Funambol::SyncEvent& event);
        void sendInitialization(Funambol::SyncEvent& event);
        void sendModifications(Funambol::SyncEvent& event);
        void sendFinalization(Funambol::SyncEvent& event);
        void syncError(Funambol::SyncEvent& event);
    signals:
        void newStatus(QString);
        void error(QString);
};

/**
* This class implements the SyncItemListener intrface in order to be notified
* of the SyncItemEvents. It simply displays the sync messages on the standard
* output.
*/
// class ItemListener : public SyncItemListener
// {
//     public:
//         
//         ItemListener() : SyncItemListener() { 
//             itemsAddedByServerCount   = 0;
//             itemsUpdatedByServerCount = 0;
//             itemsDeletedByServerCount = 0;
//             itemsAddedByClientCount   = 0;
//             itemsUpdatedByClientCount = 0;
//             itemsDeletedByClientCount = 0;
//         }
//         
//         void itemAddedByServer (SyncItemEvent& event) {
//             printf("new file received %d\n", ++itemsAddedByServerCount);
//         }
//         
//         void itemDeletedByServer (SyncItemEvent& event) {
//             printf("file \"%" WCHAR_PRINTF "\" deleted from server\n", event.getItemKey());
//         }
//         
//         void itemUpdatedByServer (SyncItemEvent& event) {
//             printf("updated file received \"%" WCHAR_PRINTF "\"\n", event.getItemKey());
//         }
//         
//         void itemAddedByClient (SyncItemEvent& event) {
//             printf("new file sent: \"%" WCHAR_PRINTF "\"\n", event.getItemKey());
//         }
//         
//         void itemDeletedByClient (SyncItemEvent& event) {
//             printf("deleted file sent \"%" WCHAR_PRINTF "\"\n", event.getItemKey());
//         }
//         
//         void itemUpdatedByClient (SyncItemEvent& event) {
//             printf("updated file sent \"%" WCHAR_PRINTF "\"\n", event.getItemKey());
//         }
//         
//     private:
//         
//         unsigned int itemsAddedByServerCount;
//         unsigned int itemsUpdatedByServerCount;
//         unsigned int itemsDeletedByServerCount;
//         unsigned int itemsAddedByClientCount;
//         unsigned int itemsUpdatedByClientCount;
//         unsigned int itemsDeletedByClientCount;
//         
// };

#endif // STATUSLISTENER_H
