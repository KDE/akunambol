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

#include<QList>
#include<QWidget>
#include<QTabWidget>
#include<QComboBox>

#include <Akonadi/Collection>
#include <Akonadi/CollectionFetchJob>

#include "standardsourcesettings.h"
#include "ui_standardsourcesettings.h"

#include<client/appsyncsourcemanager.h>
#include<client/appsyncsource.h>
#include<client/appsyncsourceconfig.h>
#include<client/collectionsfetcher.h>

using namespace Funambol;
using namespace Akonadi;

StandardSourceSettings::StandardSourceSettings(AppSyncSource* s, QWidget *parent) :
   QFrame(parent),
   m_ui(new Ui::StandardSourceSettings),
   source(s),
   idList()
{
    m_ui->setupUi(this);
    populateCollections();
}

StandardSourceSettings::~StandardSourceSettings()
{
    delete m_ui;
}

void StandardSourceSettings::populateCollections()
{ 
    LOG.debug("Fetching collections");

    CollectionsFetcher fetcher(source->getAkonadiMimeType());
    QList<Collection> colls = fetcher.fetch();

    QComboBox *collections = m_ui->collections;

    idList.clear();

    int i = 0;
    foreach( const Collection &collection, colls ) {
        if (collection.contentMimeTypes().contains(source->getAkonadiMimeType())) {
            i++;
            const char* dn = collection.name().toLatin1();
            LOG.debug("Adding collection id %lld named %s", collection.id(), dn);
            collections->addItem(dn);
            idList.append(collection.id());
        }
    }
    if (i != 1) {
        LOG.error("More than one address book, we need a way to pick one");
    }
    LOG.debug("Contact collections are in now");
}

void StandardSourceSettings::load() {
    AppSyncSourceConfig *config = source->getConfig();
    config->load();
    // Now set the value into the UI
    m_ui->remoteUri->setPlainText(config->getRemoteUri());
    // Search for the proper index
    qint64 id = config->getCollectionId();
    int idx = -1;
    int i = 0;
    foreach( qint64 currId, idList) {
        if (id == currId) {
            idx = i;
            break;
        }
        i++;
    }
    if (idx != -1) {
        m_ui->collections->setCurrentIndex(idx);
    }
}

void StandardSourceSettings::save() {
    AppSyncSourceConfig *config = source->getConfig();
    const char* uri = m_ui->remoteUri->toPlainText().toLatin1();
    config->setRemoteUri(uri);
    qint64 id = idList.at(m_ui->collections->currentIndex());
    config->setCollectionId(id);
    config->save();
}


void StandardSourceSettings::changeEvent(QEvent *e)
{
    switch(e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
