/*
    Copyright (C) 2010 Riccardo Iaconelli <riccardo@kde.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "sourcemanager.h"

#include <QString>
#include <KDebug>

#include <Akonadi/Item>

#include <base/adapter/PlatformAdapter.h>
#include <client/DMTClientConfig.h>
#include <spds/DefaultConfigFactory.h>
#include <client/SyncClient.h>

#include "syncconfig.h"
#include "ContactsSource.h"

#include "../config.h"

// Stolen from Funambol's fsync example.
static StringBuffer generateDeviceID()
{
    StringBuffer devid;
    devid.sprintf("%s-%ld", "akonadi", time(NULL));
    return devid;
}

SourceManager::SourceManager(QObject *parent)
    : QObject(parent)
{
//     m_conf = new DMTClientConfig;
    initConfig();
}

SourceManager::~SourceManager()
{
    delete m_conf;
}

SyncConfig* SourceManager::config()
{
    return m_conf;
}

// This is probably unuseful? clean this part
void SourceManager::setData(QString username, QString password, QString url)
{
    m_conf->getAccessConfig().setUsername(username.toAscii());
    m_conf->getAccessConfig().setPassword(password.toAscii());
    m_conf->getAccessConfig().setSyncURL(url.toAscii());
}

void SourceManager::initConfig()
{
    m_conf = SyncConfig::getInstance();
    // Initialize it (read from file or create the default one
    m_conf->init();
}

void SourceManager::setAkonadiItems(QList<Akonadi::Item> items)
{
    m_items = items;
}

void SourceManager::sync()
{
    kDebug();
    // Create the contact sync source passing its name, the SyncSourceConfig 
    SyncSourceConfig *srcConfig = SyncConfig::getInstance()->getSyncSourceConfig(KFUNSYNC_SOURCE_NAME);
    ContactsSource contactsSource(KFUNSYNC_SOURCE_NAME, srcConfig, NULL);
    contactsSource.setAkonadiItems(m_items);
    contactsSource.setCollectionId(m_collectionId);
 
    
    // Create the calendar sync source passing its name, the SyncSourceConfig 
//     srcConfig = KFunSyncConfig::getInstance()->getSyncSourceConfig(KFUNSYNC_CAL_SOURCE_NAME);
//     CalendarSource calendarSource(KFUNSYNC_CAL_SOURCE_NAME, srcConfig, NULL);
    
    // Initialize the SyncSource array to sync
    SyncSource* ssArray[] = { &contactsSource, NULL } ;
//     SyncSource* ssArray[] = { &calendarSource, NULL } ;
    
    // Create the SyncClient
    SyncClient client;
    
    // SYNC!
    if (client.sync(*SyncConfig::getInstance(), ssArray)) {
        LOG.error("Error during sync.\n");
    }
    
    // Save the anchors
    SyncConfig::getInstance()->save();
}

#include "sourcemanager.moc"
