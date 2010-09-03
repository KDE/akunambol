#include <assert.h>
#include <kdatetime.h>
#include <kcal/calendarresources.h>
#include <kcal/icalformat.h>
#include <kcal/calendarlocal.h>
#include <kcal/event.h>

#include <base/util/Enumeration.h>
#include <base/util/ArrayList.h>
#include <base/util/ArrayListEnumeration.h>
#include <base/util/StringBuffer.h>
#include <spds/SyncStatus.h>
#include <base/Log.h>

#include "CalendarSource.h"

int CalendarSource::beginSync()
{
    LOG.info("CalendarSource::beginSync");

    calendar = new KCal::CalendarResources(QString::fromLatin1( "UTC" ));
    if (!calendar) {
        return 1;
    }
    calendar->readConfig();
    calendar->load();
    calendar->setStandardDestinationPolicy();
    calendar->setModified(false);

    return 0;
}

int CalendarSource::endSync() {
    delete calendar;
    calendar = NULL;
    return 0;
}

Enumeration* CalendarSource::getAllItemList() {

    LOG.info("CalendarSource::getAllItemList");

    KCal::Event::List events = calendar->events();
    KCal::Event::List::const_iterator it = events.begin();
    KCal::Event::List::const_iterator end = events.end();
    ArrayList items;
    for(;it != end;++it) {
        KCal::Event *e = *it;
        QString uid = e->uid();
        StringBuffer key((const char*)uid.toUtf8());
        LOG.info("Found event: %s", key.c_str());
        items.add(key);
    }

    return new ArrayListEnumeration(items);
}

void* CalendarSource::getItemContent(StringBuffer& key, size_t* size) {

    LOG.debug("CalendarSource::getItemContent for %s", key.c_str());

    KCal::Event *event = calendar->event(key.c_str());
    if (event == NULL) {
        LOG.error("Cannot load event with id: %s", key.c_str());
        return NULL;
    }

    KCal::CalendarLocal eventCal(calendar->timeZoneId());
    eventCal.addIncidence(event->clone());
    KCal::ICalFormat formatter;
    QString vCal = formatter.toString(&eventCal);
    QByteArray bytes = vCal.toUtf8();
    const char* data = bytes.constData();
    const StringBuffer item(data);
    *size = item.length();

    char* res = new char[*size];
    for(int i=0;i<*size;++i) {
        res[i] = ((char*)item.c_str())[i];
    }

    LOG.debug("Event content: %s", res);
    return res;
}

int CalendarSource::insertItem(SyncItem& item) {

    const char* data = (const char*)item.getData();
    LOG.info("CalendarSource: adding new item");
    LOG.debug("CalendarSource: %s", data);

    KCal::ICalFormat converter;
    QString vCard(data);
    KCal::CalendarLocal eventCal(calendar->timeZoneId());
    bool success = converter.fromString(&eventCal, vCard);
    if (!success) {
        LOG.error("Cannot convert incoming event");
        return STC_COMMAND_FAILED;
    }

    KCal::Event::List evts = eventCal.events();
    KCal::Event::List::const_iterator it = evts.begin();
    KCal::Event::List::const_iterator end = evts.end();

    if (it == end) {
        LOG.error("Cannot convert incoming event");
        return STC_COMMAND_FAILED;
    }

    KCal::Event *e = (*it)->clone();
    calendar->addIncidence(e);
    calendar->save();
    QString uid = e->uid();
    StringBuffer key((const char*)uid.toUtf8());
    LOG.info("New event key: %s", key.c_str());
    item.setKey(key.c_str());

    if (++it != end) {
        LOG.error("Unexpected number of items after calendar conversion");
    }

    return STC_ITEM_ADDED;
}

int CalendarSource::modifyItem(SyncItem& item) {

    const char* data = (const char*)item.getData();
    LOG.info("CalendarSource: adding new item");
    LOG.debug("CalendarSource: %s", data);

    KCal::ICalFormat converter;
    QString vCard(data);
    KCal::CalendarLocal eventCal(calendar->timeZoneId());
    bool success = converter.fromString(&eventCal, vCard);
    if (!success) {
        LOG.error("Cannot convert incoming event");
        return STC_COMMAND_FAILED;
    }

    KCal::Event::List evts = eventCal.events();
    KCal::Event::List::const_iterator it = evts.begin();
    KCal::Event::List::const_iterator end = evts.end();

    if (it == end) {
        LOG.error("Cannot convert incoming event");
        return STC_COMMAND_FAILED;
    }

    // Remove the old instance
    KCal::Event *event = calendar->event(item.getKey());
    if (event == NULL) {
        LOG.error("Cannot remove old event with id: %s", item.getKey());
    } else {
        calendar->deleteEvent(event);
    }

    // Add the new one
    KCal::Event *e = (*it)->clone();
    // Set the event uid so that it will replace the old one
    e->setUid(item.getKey());
    calendar->addIncidence(e);
    calendar->save();

    if (++it != end) {
        LOG.error("Unexpected number of items after calendar conversion");
    }

    return STC_OK;
}

int CalendarSource::removeItem(SyncItem& item) {

    LOG.debug("CalendarSource::removeItem %s", item.getKey());

    KCal::Event *event = calendar->event(item.getKey());
    if (event == NULL) {
        LOG.error("Cannot find event with id: %s", item.getKey());
        return STC_COMMAND_FAILED;
    }

    calendar->deleteEvent(event);
    calendar->save();
    return STC_OK;
}

int CalendarSource::removeAllItems() {
    LOG.info("CalendarSource::removeAllItems");
    calendar->deleteAllEvents();
    calendar->save();
    return STC_OK;
}

StringBuffer CalendarSource::getItemSignature(StringBuffer& key) {

    KCal::Event *e = calendar->event(key.c_str());
    KDateTime d = e->lastModified();
    if (!d.isValid()) {
        // if no modification date is available, always return the same 0-time stamp
        // to avoid that 2 calls deliver different times which would be treated as changed entry
        d.setTime_t(0);
    }
    const QString fp = d.toString();
    StringBuffer res(fp.toUtf8());
    return res;
}


