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
#include <Akonadi/CollectionFetchJob>
#include <Akonadi/ItemFetchJob>

#include <kabc/stdaddressbook.h>
#include <kabc/addressbook.h>
#include <kabc/addressee.h>
#include <kabc/vcardconverter.h>
#include <kdeversion.h>

#include <base/util/Enumeration.h>
#include <base/util/ArrayList.h>
#include <base/util/ArrayListEnumeration.h>
#include <base/util/StringBuffer.h>
#include <base/Log.h>
#include <spds/SyncStatus.h>
#include <vocl/VConverter.h>
#include <vocl/VObject.h>
#include <vocl/VProperty.h>

#include "contactssource.h"

using namespace KABC;
using namespace Funambol;
using namespace Akonadi;

ContactsSource::ContactsSource ( const char* name, AbstractSyncSourceConfig* sc, KeyValueStore* cache )
        : AkonadiSource(name, sc, cache)
{
    m_collectionId = 0;
}

void* ContactsSource::getItemContent(StringBuffer& key, size_t* size)
{
    LOG.debug("Getting item content");

    QString uid(key);
    Akonadi::Item i = fetchItem(uid);
   
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

    const char* processedData = processIncomingContact(data);

    if (processedData != data) {
        LOG.debug("ContactsSource: processedData: %s", processedData);
    }

    Akonadi::Item i;
    i.setMimeType(Addressee::mimeType());
    KABC::VCardConverter converter;
    QByteArray bytes(processedData);

    Addressee contact = converter.parseVCard(bytes);
    if (contact.isEmpty()) {
        LOG.error("Cannot convert incoming item");
        return STC_COMMAND_FAILED;
    }
    i.setPayload(contact);
    
    Collection collection(m_collectionId);
    ItemCreateJob *job = new ItemCreateJob( i, collection );
    job->exec();
    
    Akonadi::Item newItem = job->item();
//     kDebug() << "ID now.." << newItem.id();
    item.setKey(QString::number(newItem.id()).toUtf8());
    
    return STC_ITEM_ADDED;
}

int ContactsSource::modifyItem(SyncItem& item)
{
    const char *key = item.getKey();

    LOG.info("ContactsSource: modifying item: %s", key);

    QString uid(key);
    Akonadi::Item i = fetchItem(uid);
    
    if (!i.hasPayload<KABC::Addressee>()) {
        LOG.info("Invalid item");
        kDebug() << "INVALID";
    }

    KABC::Addressee contact = i.payload<KABC::Addressee>();
    const char* data = (const char*)item.getData();
    LOG.debug("ContactsSource: %s", data);
    const char* processedData = processIncomingContact(data);

    if (processedData != data) {
        LOG.debug("ContactsSource: processedData: %s", processedData);
    }

    KABC::VCardConverter converter;
    QByteArray bytes(processedData);
    contact = converter.parseVCard(bytes);
    if (contact.isEmpty()) {
        LOG.error("Cannot convert incoming item");
        return STC_COMMAND_FAILED;
    }
    
    i.setMimeType(Addressee::mimeType());
    i.setPayload(contact);
    Akonadi::ItemModifyJob *job2 = new Akonadi::ItemModifyJob(i);
    if (job2->exec()){
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

const char* ContactsSource::processIncomingContact(const char* vcard) {

    // Preprocess any incoming item and adapt it to the KDE libs, working around
    // interoperability issues and bugs


    bool changed = false;
    const char* newData = vcard;

    // In version 4.4.x of KDE libs, a contact with emtpy TZ causes the parsing
    // to fail. If this vcard has an empty TZ, we remove it
    if (KDE::versionMajor() < 4 ||
        (KDE::versionMajor() == 4 && KDE::versionMinor() < 5)) {

        LOG.debug("Applying patch for TZ on KDE <= 4.4.5");

        VObject* contact = VConverter::parse(vcard);
        VProperty* tzProperty = contact->getProperty("TZ");
        if (tzProperty != NULL) {
            LOG.debug("TZ field found");
            // Is it empty?
            const char* value = tzProperty->getValue();
            if (value == NULL || strlen(value) == 0) {
                LOG.debug("TZ field empty");
                // We must remove this property
                contact->removeProperty("TZ");
                changed = true;
            }
        }

        if (changed) {
            newData = contact->toString();
        }

        delete contact;
    }

    return newData;
}



// kate: indent-mode cstyle; space-indent on; indent-width 0; 
