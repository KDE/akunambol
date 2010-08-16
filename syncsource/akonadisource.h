/*
 * Funambol is a mobile platform developed by Funambol, Inc.
 * Copyright (C) 2010 Funambol, Inc.
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
 *
 * You can contact Funambol, Inc. headquarters at 643 Bair Island Road, Suite
 * 305, Redwood City, CA 94063, USA, or at email address info@funambol.com.
 *
 * The interactive user interfaces in modified source and object code versions
 * of this program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU Affero General Public License version 3.
 *
 * In accordance with Section 7(b) of the GNU Affero General Public License
 * version 3, these Appropriate Legal Notices must retain the display of the
 * "Powered by Funambol" logo. If the display of the logo is not reasonably
 * feasible for technical reasons, the Appropriate Legal Notices must display
 * the words "Powered by Funambol".
 */

#ifndef AKONADISOURCE_H
#define AKONADISOURCE_H

#undef UTC
#include <kabc/addressbook.h>
#include <QObject>

#include <client/CacheSyncSource.h>
#include <spds/SyncItem.h>
#include <base/util/Enumeration.h>
#include <Akonadi/Item>
#include <QObject>

#include <base/Log.h>


using namespace Funambol;

class AkonadiSource : public CacheSyncSource
{
    public:

        AkonadiSource(const WCHAR* name, AbstractSyncSourceConfig *sc, KeyValueStore* cache);

        virtual ~AkonadiSource() {}

        /**
         * Returns an Enumeration containing the StringBuffer keys of all items.
         *
         * It is used both for the full sync, where all items are sent to the server,
         * and for the fast sync to calculate the modification since the last
         * successful sync.
         *
         * @return a newly allocated Enumeration that is free'd by the CacheSyncSource
         *         CacheSyncSource.
         *         Return NULL in case of error, an empty Enumeration
         *         if there are no items.
         */
        Enumeration* getAllItemList();

        int removeItem(SyncItem& item);

        int removeAllItems();

        void setCollectionId(qint64 id) { m_collectionId = id; }
    protected:
        Akonadi::Item fetchItem(QString uid);

        virtual Akonadi::Item::List getItems();

    protected:
        quint64 m_collectionId;
        QList< Akonadi::Item > m_items;


};

#endif
