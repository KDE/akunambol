
#include "contacts.h"

#include <Akonadi/CollectionFetchJob>
#include <Akonadi/ItemFetchJob>
#include <Akonadi/ItemFetchScope>
#include <Akonadi/Monitor>

#include <kabc/addressee.h>
#include <kabc/phonenumber.h>
#include <kabc/picture.h>
#include <kabc/key.h>

#include <KDebug>

using namespace Akonadi;

Contacts::Contacts(QObject *parent)
    : QObject(parent)
{   
    initContacts();
    
    fetchContactsCollections();
}

Contacts::~Contacts()
{
}


QList<Akonadi::Collection> Contacts::collections()
{
    return m_collections;
}

QList<Akonadi::Item> Contacts::itemsFor(int id)
{
    return m_items;
}
      

void Contacts::initContacts()
{
    m_contactMonitor = new Monitor( this );
    m_contactMonitor->setMimeTypeMonitored("text/directory");
    m_contactMonitor->setCollectionMonitored(Collection::root(), false);
    m_contactMonitor->itemFetchScope().fetchFullPayload();
    connect(m_contactMonitor, SIGNAL(itemAdded(Akonadi::Item, Akonadi::Collection)),
            SLOT(contactItemAdded(Akonadi::Item)) );
//    connect(m_emailMonitor, SIGNAL(itemChanged(Akonadi::Item, QSet<QByteArray>)),
//            SLOT(contactItemAdded(Akonadi::Item)) );
    // remove the monitor on a source that's not used
//    connect(this, SIGNAL(sourceRemoved(QString)), SLOT(stopMonitor(QString)));
}

void Contacts::fetchContactsCollections()
{ 
    Collection contactCollection(Collection::root());
    contactCollection.setContentMimeTypes(QStringList() << "text/directory");

    CollectionFetchJob* fetch = new CollectionFetchJob( contactCollection, CollectionFetchJob::Recursive);
//     int id = 2;
//     ItemFetchJob *fetch = new ItemFetchJob( Collection( id ), this );
//     m_contactMonitor->setCollectionMonitored(Collection(id), true);
//     fetch->fetchScope().fetchFullPayload();
    connect( fetch, SIGNAL(result(KJob*)), SLOT(fetchContactCollectionsDone(KJob*)) );
}


void Contacts::loadContactsFor(int id)
{
    kDebug();
    ItemFetchJob *fetch = new ItemFetchJob(Collection(id), this);
    m_contactMonitor->setCollectionMonitored(Collection(id), true);
    fetch->fetchScope().fetchFullPayload();
    connect( fetch, SIGNAL(result(KJob*)), SLOT(fetchContactCollectionDone(KJob*)) );
//     Q_UNUSED(i)
    kDebug() << id;
}

void Contacts::contactItemAdded( const Akonadi::Item &item )
{
    kDebug() << "WAAAAAAA";
    if (item.hasPayload<KABC::Addressee>()) {
        //kDebug() << item.id() << "item has payload ...";
        KABC::Addressee a = item.payload<KABC::Addressee>();
        if (!a.isEmpty()) {
            const QString source = QString("Contact-%1").arg(item.id());
            // Phone and related
            QStringList phoneNumbers;
            foreach (const KABC::PhoneNumber &pn, a.phoneNumbers()) {
                const QString key = QString("Phone-%1").arg(pn.typeLabel());
//                setData(source, key, a.phoneNumber(pn.type()).number());
                phoneNumbers << a.phoneNumber(pn.type()).number();
            }
        }
    }
}

void Contacts::fetchContactCollectionsDone(KJob* job)
{
    kDebug();
    // called when the job fetching contact collections from Akonadi emits result()
    if ( job->error() ) {
        kDebug() << "Job Error:" << job->errorString();
    } else {
        CollectionFetchJob* cjob = static_cast<CollectionFetchJob*>( job );
        int i = 0;
        foreach( const Collection &collection, cjob->collections() ) {
            if (collection.contentMimeTypes().contains("text/directory")) {
                i++;
                m_collections << collection;
                kDebug() << "Adding collection id " << collection.id() << " named " << collection.name();
            }
        }
        kDebug() << i << "Contact collections are in now";
    }
    emit ready();
}

void Contacts::fetchContactCollectionDone(KJob* job)
{
    if ( job->error() ) {
        return;
    }
//     ItemFetchJob *j = static_cast<ItemFetchJob*>( job );
    Item::List items = static_cast<ItemFetchJob*>( job )->items();
    m_items = items;
//     foreach ( const Item &item, items ) {
//         m_items << item; //contactItemAdded( item );
//     }
    emit loadedCollection(items.first().id());
}

void Contacts::wa()
{
    kDebug() << "WA";
}
