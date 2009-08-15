#include "akonadi-dialog.h"
#include "ui_akonadi-dialog.h"

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

Dialog::Dialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);
    
    initContacts();
    
    fetchContactsCollections();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::initContacts()
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

void Dialog::fetchContactsCollections()
{ 
    int id = 2;
    ItemFetchJob *fetch = new ItemFetchJob( Collection( id ), this );
    m_contactMonitor->setCollectionMonitored(Collection(id), true);
    fetch->fetchScope().fetchFullPayload();
    connect( fetch, SIGNAL(result(KJob*)), SLOT(wa()) );
}

void Dialog::contactItemAdded( const Akonadi::Item &item )
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

void Dialog::fetchContactCollectionsDone(KJob* job)
{
    // called when the job fetching contact collections from Akonadi emits result()
    if ( job->error() ) {
        kDebug() << "Job Error:" << job->errorString();
    } else {
        CollectionFetchJob* cjob = static_cast<CollectionFetchJob*>( job );
        int i = 0;
        foreach( const Collection &collection, cjob->collections() ) {
            if (collection.contentMimeTypes().contains("text/directory")) {
                //kDebug() << "ContactCollection setting data:" << collection.name() << collection.url() << collection.contentMimeTypes();
                i++;
                //setData("ContactCollections", QString("ContactCollection-%1").arg(collection.id()), collection.name());
            }
        }
        kDebug() << i << "Contact collections are in now";
        //scheduleSourcesUpdated();
    }
}

void Dialog::wa()
{
    kDebug() << "WA";
}
