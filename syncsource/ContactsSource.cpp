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

#include <Akonadi/ItemDeleteJob>
#include <Akonadi/ItemCreateJob>
#include <Akonadi/ItemModifyJob>
#include <Akonadi/ItemFetchScope>
#include <Akonadi/Collection>

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

#include "ContactsSource.h"
#include "../akonadi/contacts.h"
#include <Akonadi/ItemFetchJob>

using namespace KABC;
using namespace Funambol;

ContactsSource::ContactsSource ( const char* name, AbstractSyncSourceConfig* sc, KeyValueStore* cache )
        : CacheSyncSource(name, sc, cache)
{
    m_collectionId = 0;
}

void ContactsSource::setAkonadiItems(Akonadi::Item::List items)
{
    m_items = items;
}

Enumeration* ContactsSource::getAllItemList()
{
    LOG.info("ContactsSource::getAllItemsList");
    ArrayList items;
   
    foreach(Akonadi::Item item, m_items) {
        QString uid = QString::number(item.id());
        StringBuffer key((const char*)uid.toLatin1());
        LOG.info("Found contact: %s", key.c_str());
        items.add(key);
    }

    return new ArrayListEnumeration(items);
}

void* ContactsSource::getItemContent(StringBuffer& key, size_t* size)
{
    LOG.debug("ContactsSource::getItemContent for %s", key.c_str());
        
    QString uid(key);
//         kDebug() << uid;
    Akonadi::Item iid(uid.toLongLong());
    kDebug() << "before" << uid;
    Akonadi::ItemFetchJob *job = new Akonadi::ItemFetchJob(iid);
    job->fetchScope().fetchFullPayload();
    
    if (!job->exec()) {
        kDebug() << "Error!!! " << job->errorText();
        kDebug() << "I'm going to crash and can't avoid it... :(";
    }
    kDebug() << "after";
    
    Akonadi::Item i = job->items().first();
    
    if (!i.hasPayload<KABC::Addressee>()) {
        LOG.info("Invalid item");
        kDebug() << "INVALID";
    }
    
    KABC::Addressee contact = i.payload<KABC::Addressee>();

    if (contact.isEmpty()) {
        LOG.error("Cannot load contact with id: %s", key.c_str());
        return NULL;
    }

    // Now convert the item
    KABC::VCardConverter converter;
    QByteArray bytes = converter.createVCard(contact, KABC::VCardConverter::v2_1);
    const char* data = bytes.constData();
    // Since we have an interoperability issue on vCard with QP encoding and
    // folding, we perform an "unfold" (note that folding is not required by
    // vCard)
    const StringBuffer item = unfoldVCard(data);
    *size = item.length();

    char* res = new char[*size];
    for (int i=0;i<(int)*size;++i) {
        res[i] = ((char*)item.c_str())[i];
    }

    LOG.debug("Contact content: %s", res);
    
    return res;
}

int ContactsSource::insertItem(SyncItem& item)
{
    const char* data = (const char*)item.getData();
    LOG.info("ContactsSource: adding new item");
    LOG.debug("ContactsSource: %s", data);

    Akonadi::Item i;
    
    i.setMimeType("text/directory");
    KABC::VCardConverter converter;
    QByteArray bytes(data);
    Addressee contact = converter.parseVCard(bytes);
    if (contact.isEmpty()) {
        LOG.error("Cannot convert incoming item");
        return STC_COMMAND_FAILED;
    }
    i.setPayload(contact);
    
    Akonadi::Collection collection(m_collectionId);
    Akonadi::ItemCreateJob *job = new Akonadi::ItemCreateJob( i, collection );
    job->exec();
    
    Akonadi::Item newItem = job->item();
//     kDebug() << "ID now.." << newItem.id();
    item.setKey(QString::number(newItem.id()).toLatin1());
    
    return STC_ITEM_ADDED;
}

int ContactsSource::modifyItem(SyncItem& item)
{
    const char *key = item.getKey();

    LOG.info("ContactsSource: modifying item: %s", key);

    QString uid(key);
    Akonadi::Item iid(uid.toLongLong());
    Akonadi::ItemFetchJob *job = new Akonadi::ItemFetchJob(iid);
    job->fetchScope().fetchFullPayload();
    
    if (!job->exec()) {
        kDebug() << "Error!!! " << job->errorText();
    }
    
    Akonadi::Item i = job->items().first();
    
    if (!i.hasPayload<KABC::Addressee>()) {
        LOG.info("Invalid item");
        kDebug() << "INVALID";
    }

    KABC::Addressee contact = i.payload<KABC::Addressee>();
    const char* data = (const char*)item.getData();
    LOG.debug("ContactsSource: %s", data);

    KABC::VCardConverter converter;
    QByteArray bytes(data);
    contact = converter.parseVCard(bytes);
    if (contact.isEmpty()) {
        LOG.error("Cannot convert incoming item");
        return STC_COMMAND_FAILED;
    }
    
    i.setPayload(contact);
    Akonadi::ItemModifyJob *job2 = new Akonadi::ItemModifyJob(i);
    if (job2->exec()){
        return STC_OK;
    } else {
        return STC_COMMAND_FAILED;
    }
}

int ContactsSource::removeItem(SyncItem& item)
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

int ContactsSource::removeAllItems()
{
    LOG.info("ContactsSource: remove all items");
    Akonadi::ItemDeleteJob *job = new Akonadi::ItemDeleteJob(m_items);
    job->start();
    if (job->exec()){
        return STC_OK;
    } else {
        return STC_COMMAND_FAILED;
    }
}

const StringBuffer ContactsSource::unfoldVCard(const char* vcard)
{
    StringBuffer buf(vcard);
    buf.replaceAll("\r\n ", "");
    return buf;
}


// kate: indent-mode cstyle; space-indent on; indent-width 0; 
