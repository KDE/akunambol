
#include "sourcemanager.h"

#include <QString>
#include <KDebug>

#include <Akonadi/Item>

#include <base/adapter/PlatformAdapter.h>
#include <client/DMTClientConfig.h>
#include <spds/DefaultConfigFactory.h>
#include <client/SyncClient.h>
// #include <client/CacheSyncSource.h>

#include "KFunSyncConfig.h"
#include "ContactsSource.h"

#include "../config.h"

USE_FUNAMBOL_NAMESPACE

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
    PlatformAdapter::init(KFUNSYNC_APPLICATION_URI);
//     m_conf = new DMTClientConfig;
    initConfig();
}

SourceManager::~SourceManager()
{
    delete m_conf;
}

KFunSyncConfig* SourceManager::config()
{
    return m_conf;
}

void SourceManager::setData(QString username, QString password, QString url)
{
    m_conf->getAccessConfig().setUsername(username.toAscii());
    m_conf->getAccessConfig().setPassword(password.toAscii());
    m_conf->getAccessConfig().setSyncURL(url.toAscii());
}

void SourceManager::initConfig()
{
    m_conf = KFunSyncConfig::getInstance();
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
    SyncSourceConfig *srcConfig = KFunSyncConfig::getInstance()->getSyncSourceConfig(KFUNSYNC_SOURCE_NAME);
    ContactsSource contactsSource(KFUNSYNC_SOURCE_NAME, srcConfig, NULL);
    contactsSource.setAkonadiItems(m_items);
 
    
    // Create the calendar sync source passing its name, the SyncSourceConfig 
//     srcConfig = KFunSyncConfig::getInstance()->getSyncSourceConfig(KFUNSYNC_CAL_SOURCE_NAME);
//     CalendarSource calendarSource(KFUNSYNC_CAL_SOURCE_NAME, srcConfig, NULL);
    
    // Initialize the SyncSource array to sync
    SyncSource* ssArray[] = { &contactsSource, NULL } ;
//     SyncSource* ssArray[] = { &calendarSource, NULL } ;
    
    // Create the SyncClient
    SyncClient client;
    
    // SYNC!
    if (client.sync(*KFunSyncConfig::getInstance(), ssArray)) {
        LOG.error("Error during sync.\n");
//         return false;
    }
    
    // Save the anchors
    KFunSyncConfig::getInstance()->save();
}

#include "sourcemanager.moc"
