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

#include <QDateTime>

#include <Akonadi/ItemDeleteJob>
#include <Akonadi/ItemCreateJob>
#include <Akonadi/ItemModifyJob>
#include <Akonadi/ItemFetchScope>
#include <Akonadi/Collection>
#include <Akonadi/ItemFetchJob>

#include <kabc/stdaddressbook.h>
#include <kabc/addressbook.h>
#include <kabc/addressee.h>
#include <kabc/vcardconverter.h>

#include <base/util/Enumeration.h>
#include <base/util/ArrayList.h>
#include <base/util/ArrayListEnumeration.h>
#include <base/util/StringBuffer.h>
#include <spds/SyncStatus.h>
#include <base/Log.h>

#include "akonadisource.h"

using namespace KABC;
using namespace Funambol;



AkonadiSource::AkonadiSource(const char* name, AbstractSyncSourceConfig* sc, KeyValueStore* cache )
        : CacheSyncSource(name, sc, cache)
{
    m_collectionId = 0;
}

Enumeration* AkonadiSource::getAllItemList()
{
    LOG.info("AkonadiSource::getAllItemsList");
    ArrayList items;

    // Get the items from Akonadi
    Akonadi::Item::List m_items = getItems();
   
    // Tranform the List into an ArrayList suitable for the sync engine
    foreach(Akonadi::Item item, m_items) {
        QString uid = QString::number(item.id());
        StringBuffer key((const char*)uid.toLatin1());
        LOG.debug("Found contact: %s", key.c_str());
        items.add(key);
    }

    return new ArrayListEnumeration(items);
}

Akonadi::Item AkonadiSource::fetchItem(QString uid) {
    LOG.debug("AkonadiSource::fetchItem for %s", uid.constData());
    Akonadi::Item iid(uid.toLongLong());
    Akonadi::ItemFetchJob *job = new Akonadi::ItemFetchJob(iid);
    job->fetchScope().fetchFullPayload();
    
    if (!job->exec()) {
        //LOG.error("Error!!! %s", job->errorText().toLatin1());
        LOG.error("I'm going to crash and can't avoid it... :(");
    }
    
    Akonadi::Item i = job->items().first();
    LOG.debug("DONE");
    return i;
}


int AkonadiSource::removeItem(SyncItem& item)
{
    const char *key = item.getKey();
    LOG.info("ContactsSource: removing item: %s", key);

    // Search the contact
    QString uid(key);
    Akonadi::Item i(uid.toLongLong());
    Akonadi::ItemDeleteJob *job = new Akonadi::ItemDeleteJob(i);
    if (job->exec()){
        return STC_OK;
    } else {
        return STC_COMMAND_FAILED;
    }
}

int AkonadiSource::removeAllItems()
{
    LOG.info("ContactsSource: remove all items");
    Akonadi::Item::List m_items = getItems();
    Akonadi::ItemDeleteJob *job = new Akonadi::ItemDeleteJob(m_items);
    job->start();
    if (job->exec()){
        return STC_OK;
    } else {
        return STC_COMMAND_FAILED;
    }
}

Akonadi::Item::List AkonadiSource::getItems() {
    //m_selectedCollection = id;
    LOG.debug("Getting items for collection: %lld", m_collectionId);
    Akonadi::ItemFetchJob *fetch = new Akonadi::ItemFetchJob(Akonadi::Collection(m_collectionId));

    //m_contactMonitor->setCollectionMonitored(Collection(id), true);
    fetch->fetchScope().fetchFullPayload();
    //connect( fetch, SIGNAL(result(KJob*)), SLOT(fetchDone(KJob*)) );
    if (!fetch->exec()) {
        LOG.error("Cannot retrieve items");
        Akonadi::Item::List m_items;
        return m_items;
    } else {
        Akonadi::Item::List m_items = fetch->items();
        return m_items;
    }
}

StringBuffer AkonadiSource::getItemSignature(StringBuffer& key) {
    Akonadi::Item item = fetchItem(key.c_str());
    // Get the item timestamp
    QDateTime ts = item.modificationTime();
    if (!ts.isValid()) {
        // if no modification date is available, always return the same 0-time stamp
        // to avoid that 2 calls deliver different times which would be treated as changed entry
        ts.setTime_t(0);
    }
    const QString fp = ts.toString();
    StringBuffer res(fp.toLatin1());

    LOG.info("******************** FP is %s ***********", res.c_str());

    return res;
}

