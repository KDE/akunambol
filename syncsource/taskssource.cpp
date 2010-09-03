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

#include <kdatetime.h>
#include <kcal/calendarresources.h>
#include <kcal/icalformat.h>
#include <kcal/calendarlocal.h>
#include <kcal/todo.h>

#include <base/util/Enumeration.h>
#include <base/util/ArrayList.h>
#include <base/util/ArrayListEnumeration.h>
#include <base/util/StringBuffer.h>
#include <spds/SyncStatus.h>
#include <base/Log.h>

#include "taskssource.h"

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<KCal::Incidence> IncidencePtr;

using namespace Funambol;
using namespace Akonadi;

TasksSource::TasksSource ( const char* name, AbstractSyncSourceConfig* sc, KeyValueStore* cache )
        : AkonadiSource(name, sc, cache)
{
    m_collectionId = 0;
}

Akonadi::Item::List TasksSource::getItems() {
    LOG.debug("TasksSource: getting items for collection: %lld", m_collectionId);

    Akonadi::Item::List m_items = AkonadiSource::getItems();
    Akonadi::Item::List filtered;
    // We must filter events/todo
    foreach(Akonadi::Item i, m_items) {
        if (i.hasPayload<IncidencePtr>()) {
            IncidencePtr ptrEvent = i.payload<IncidencePtr>();
            KCal::Todo *todo = dynamic_cast<KCal::Todo *>(ptrEvent.get());
            if (todo != NULL) {
                filtered.append(i);
            }
        }
    }
    return filtered;
}

void* TasksSource::getItemContent(StringBuffer& key, size_t* size)
{
    LOG.debug("TasksSource: getting item content %s", key.c_str());

    QString uid(key);
    Akonadi::Item i = fetchItem(uid);
   
    if (!i.hasPayload<IncidencePtr>()) {
        LOG.error("Invalid item");
        return NULL;
    }

    IncidencePtr ptrTodo = i.payload<IncidencePtr>();
    KCal::Todo *todo = dynamic_cast<KCal::Todo *>(ptrTodo.get());

    LOG.debug("TasksSource: todo = %p", todo);

    //KCal::CalendarLocal eventCal(calendar->timeZoneId());
    KCal::CalendarLocal todoCal("GMT");
    todoCal.addIncidence(todo->clone());
    KCal::ICalFormat formatter;
    QString vCal = formatter.toString(&todoCal);
    QByteArray bytes = vCal.toUtf8();
    const char* data = bytes.constData();
    const StringBuffer item(data);
    *size = item.length();

    char* res = new char[*size];
    for(unsigned int i=0;i<*size;++i) {
        res[i] = ((char*)item.c_str())[i];
    }

    LOG.debug("Task content: %s", res);
    return res;
}

int TasksSource::insertItem(SyncItem& item)
{
    const char* data = (const char*)item.getData();
    LOG.info("CalendarSource: adding new item");
    LOG.debug("CalendarSource: %s", data);

    KCal::ICalFormat converter;
    QString vCal(data);
    KCal::CalendarLocal todoCal("GMT");
    bool success = converter.fromString(&todoCal, vCal);
    if (!success) {
        LOG.error("Cannot convert incoming event");
        return STC_COMMAND_FAILED;
    }

    KCal::Todo::List todos = todoCal.todos();
    KCal::Todo::List::const_iterator it = todos.begin();
    KCal::Todo::List::const_iterator end = todos.end();

    if (it == end) {
        LOG.error("Cannot convert incoming todo");
        return STC_COMMAND_FAILED;
    }

    KCal::Todo *t = (*it)->clone();

    Akonadi::Item i;
    
    i.setMimeType("text/calendar");
    i.setPayload(IncidencePtr(t));
    
    Collection collection(m_collectionId);
    ItemCreateJob *job = new ItemCreateJob( i, collection );
    job->exec();
    
    Akonadi::Item newItem = job->item();
    item.setKey(QString::number(newItem.id()).toUtf8());
    return STC_ITEM_ADDED;
}

int TasksSource::modifyItem(SyncItem& item)
{
    const char *key = item.getKey();
    LOG.info("EventsSource: modifying item: %s", key);

    QString uid(key);
    Akonadi::Item i = fetchItem(uid);
    
    if (!i.hasPayload<IncidencePtr>()) {
        LOG.error("Invalid item");
        return STC_COMMAND_FAILED;
    }

    const char* data = (const char*)item.getData();
    LOG.debug("ContactsSource: %s", data);

    KCal::ICalFormat converter;
    QString vCal(data);
    KCal::CalendarLocal todoCal("GMT");
    bool success = converter.fromString(&todoCal, vCal);
    if (!success) {
        LOG.error("Cannot convert incoming todo");
        return STC_COMMAND_FAILED;
    }

    KCal::Todo::List todos = todoCal.todos();
    KCal::Todo::List::const_iterator it = todos.begin();
    KCal::Todo::List::const_iterator end = todos.end();

    if (it == end) {
        LOG.error("Cannot convert incoming task");
        return STC_COMMAND_FAILED;
    }

    KCal::Todo *t = (*it)->clone();
    i.setMimeType("text/calendar");
    i.setPayload(IncidencePtr(t));
    Akonadi::ItemModifyJob *job2 = new Akonadi::ItemModifyJob(i);
    if (job2->exec()){
        return STC_OK;
    } else {
        return STC_COMMAND_FAILED;
    }
}

