/*
 * Funambol is a mobile platform developed by Funambol, Inc. 
 * Copyright (C) 2010 Funambol, Inc.
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

using namespace KABC;

ContactsSource::ContactsSource ( const char* name, AbstractSyncSourceConfig* sc, KeyValueStore* cache )
    : CacheSyncSource(name, sc, cache)
{

}


Enumeration* ContactsSource::getAllItemList() {

    LOG.info("ContactsSource::getAllItemsList");

    StdAddressBook* ab = StdAddressBook::self();
    AddressBook::ConstIterator it = ab->begin();
    AddressBook::ConstIterator end = ab->end();
    ArrayList items;
    for(;it != end;++it) {
        Addressee contact = *it;
        QString uid = contact.uid();
        StringBuffer key((const char*)uid.toLatin1());
        LOG.info("Found contact: %s", key.c_str());
        items.add(key);
    }

    return new ArrayListEnumeration(items);
}

void* ContactsSource::getItemContent(StringBuffer& key, size_t* size) {

    LOG.debug("ContactsSource::getItemContent for %s", key.c_str());

    StdAddressBook* ab = StdAddressBook::self();
    // Load the contact
    Addressee contact = ab->findByUid(key.c_str());
    if (contact.isEmpty()) {
        LOG.error("Cannot load contact with id: %s", key.c_str());
        return NULL;
    }

    // Now convert the item
    VCardConverter converter;
    QByteArray bytes = converter.createVCard(contact, VCardConverter::v2_1);
    const char* data = bytes.constData();
    // Since we have an interoperability issue on vCard with QP encoding and
    // folding, we perform an "unfold" (note that folding is not required by
    // vCard)
    const StringBuffer item = unfoldVCard(data);
    *size = item.length();

    char* res = new char[*size];
    for(int i=0;i<*size;++i) {
        res[i] = ((char*)item.c_str())[i];
    }

    LOG.debug("Contact content: %s", res);
    return res;
}

int ContactsSource::insertItem(SyncItem& item) {
    const char* data = (const char*)item.getData();
    LOG.info("ContactsSource: adding new item");
    LOG.debug("ContactsSource: %s", data);

    VCardConverter converter;
    QByteArray bytes(data);
    Addressee contact = converter.parseVCard(bytes);
    if (contact.isEmpty()) {
        LOG.error("Cannot convert incoming item");
        return STC_COMMAND_FAILED;
    }
    StdAddressBook* ab = StdAddressBook::self();
    ab->insertAddressee(contact);
    QString uid = contact.uid();
    item.setKey(uid.toLatin1());
    ab->save();

    return STC_ITEM_ADDED;
}

int ContactsSource::modifyItem(SyncItem& item) {
    const char *key = item.getKey();

    LOG.info("ContactsSource: modifying item: %s", key);

    const char* data = (const char*)item.getData();
    LOG.debug("ContactsSource: %s", data);

    VCardConverter converter;
    QByteArray bytes(data);
    Addressee contact = converter.parseVCard(bytes);
    if (contact.isEmpty()) {
        LOG.error("Cannot convert incoming item");
        return STC_COMMAND_FAILED;
    }
    QString uid(key);
    contact.setUid(uid);
    StdAddressBook* ab = StdAddressBook::self();
    ab->insertAddressee(contact);
    ab->save();
    
    return STC_OK;;
}

int ContactsSource::removeItem(SyncItem& item) {
    const char *key = item.getKey();
    LOG.info("ContactsSource: removing item: %s", key);

    StdAddressBook* ab = StdAddressBook::self();
    // Search the contact
    QString uid(key);
    Addressee contact = ab->findByUid(uid);
    if (contact.isEmpty()) {
        LOG.error("Cannot find contact with id: %s", item.getKey());
        return STC_COMMAND_FAILED;
    }

    AddressBook::Iterator it = ab->find(contact);
    if (it == ab->end()) {
        LOG.error("Cannot find contact iterator with id: %s", item.getKey());
        return STC_COMMAND_FAILED;
    }

    ab->removeAddressee(it);
    ab->save();
    return STC_OK;
}

int ContactsSource::removeAllItems() {
    LOG.info("ContactsSource: remove all items");
    StdAddressBook* ab = StdAddressBook::self();
    ab->clear();
    ab->save();
}

const StringBuffer ContactsSource::unfoldVCard(const char* vcard) {
    StringBuffer buf(vcard);
    buf.replaceAll("\r\n ", "");
    return buf;
}


