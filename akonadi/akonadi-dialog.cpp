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
    kDebug();
    c->loadContactsForCollection(id);
    QTimer *t = new QTimer;
    t->setSingleShot(true);
    connect(c, SIGNAL(loadedCollection()), t, SLOT(start()));
    connect(t, SIGNAL(timeout()), SLOT(startSync()));
}

void ContactsSyncer::startSync()
{
    emit startedSync();
    //TODO check if we did select anything or we should abort
    m_sourceManager->setAkonadiItems(c->itemsForLoadedCollection());
    m_sourceManager->setCollectionId(c->selectedCollection());
    m_sourceManager->sync();
    emit finishedSync();
}


