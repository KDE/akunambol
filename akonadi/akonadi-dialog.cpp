#include "akonadi-dialog.h"
#include "ui_akonadi-dialog.h"
#include "contacts.h"

#include <QtGui>

#include <Akonadi/CollectionFetchJob>
#include <Akonadi/ItemFetchJob>
#include <Akonadi/ItemFetchScope>
#include <Akonadi/Monitor>
#include <Akonadi/CollectionDialog>

#include <kabc/addressee.h>
#include <kabc/phonenumber.h>
#include <kabc/picture.h>
#include <kabc/key.h>

#include "../syncsource/sourcemanager.h"

#include <KDebug>

using namespace Akonadi;

ContactsSyncer::ContactsSyncer(SourceManager* s, QObject* parent)
    : QObject(parent)
{
    m_sourceManager = s;
   
    c = new Contacts;
    connect(c, SIGNAL(ready()), SLOT(init()));
}

ContactsSyncer::~ContactsSyncer()
{
    delete c;
}

void ContactsSyncer::init()
{
    if (c->collections().size() == 0) { // Can't sync
        emit noCollections();
    } else if (c->collections().size() == 1) { // Just one collection, don't show the dialog
        prepareSyncFor(c->collections().first().id());
    } else { // Which collection should I use?
        CollectionDialog dlg;
        dlg.setMimeTypeFilter( QStringList() << KABC::Addressee::mimeType() );
        dlg.setAccessRightsFilter( Collection::CanCreateItem );
        dlg.setDescription( i18n( "Select an address book for saving:" ) );
        dlg.exec();
        prepareSyncFor(dlg.selectedCollection().id());
    }
    
    disconnect(c, SIGNAL(ready()), this, SLOT(init())); // ready has served its purposes
}

void ContactsSyncer::prepareSyncFor(qint64 id)
{
    c->loadContactsForCollection(id);
    connect(c, SIGNAL(loadedCollection()), SLOT(startSync()));
}

void ContactsSyncer::startSync()
{
    //TODO check if we did select anything or we should abort
    m_sourceManager->setAkonadiItems(c->itemsForLoadedCollection());
    m_sourceManager->setCollectionId(c->selectedCollection());
    m_sourceManager->sync();
    emit finishedSync();
}


