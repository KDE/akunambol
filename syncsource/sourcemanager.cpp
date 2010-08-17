
#include "sourcemanager.h"

#include <QThread>
#include <QString>
#include <QMessageBox>
#include <KDebug>

#include <Akonadi/Item>

#include <base/adapter/PlatformAdapter.h>
#include <client/DMTClientConfig.h>
#include <spds/DefaultConfigFactory.h>
#include <spds/SyncReport.h>
#include <client/SyncClient.h>
#include <event/SyncItemListener.h>
#include <event/SyncListener.h>
#include <event/SyncSourceListener.h>
#include <event/TransportListener.h>
#include <event/ManageListener.h>

#include "KFunSyncConfig.h"

#include "client/appsyncsource.h"
#include "client/collectionsfetcher.h"

#include "../config.h"

class SyncThread : public QThread {

    public:

        SyncThread(SourceManager* m, SyncClient* c, AppSyncSource* a) : manager(m),
                                                                        client(c),
                                                                        appSource(a)
        {
        }

        void run() {
            AkonadiSource *source = appSource->getSyncSource();
            AppSyncSourceConfig* sourceConfig = appSource->getConfig();
            source->setCollectionId(sourceConfig->getCollectionId());
            SyncSourceConfig *srcConfig = KFunSyncConfig::getInstance()->getSyncSourceConfig(source->getName());
            if (srcConfig != NULL) {
                source->setConfig(srcConfig);
            }
            const char* remoteUri = sourceConfig->getRemoteUri();
            if (remoteUri == NULL || strlen(remoteUri) == 0) {
                sourceConfig->setRemoteUri(srcConfig->getURI());
                sourceConfig->save();
            } else {
                srcConfig->setURI(remoteUri);
            }

            SyncSource* ssArray[] = { source, NULL } ;
            if (client->sync(*KFunSyncConfig::getInstance(), ssArray)) {
                LOG.error("Error during sync.\n");
            }

            // Save the anchors
            KFunSyncConfig::getInstance()->save();
            manager->emitSourceEnded(appSource, client->getSyncReport());
        }

    private:
        SourceManager* manager;
        SyncClient*    client;
        AppSyncSource* appSource;
};


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
    setListeners();
    client = new SyncClient();
}

SourceManager::~SourceManager()
{
    unsetListeners();
}

void SourceManager::setData(QString username, QString password, QString url)
{
    KFunSyncConfig* m_conf = KFunSyncConfig::getInstance();
    m_conf->getAccessConfig().setUsername(username.toAscii());
    m_conf->getAccessConfig().setPassword(password.toAscii());
    m_conf->getAccessConfig().setSyncURL(url.toAscii());
}

void SourceManager::sync(AppSyncSource* appSource) {
    LOG.info("Starting sync for source: %s", appSource->getName());

    // Before firing any sync we must make sure that the client is properly
    // configured

    // Step 1: check account info
    KFunSyncConfig *config = KFunSyncConfig::getInstance();
    AccessConfig &ac = config->getAccessConfig();
    const char* username = ac.getUsername();
    const char* pwd      = ac.getPassword();
    const char* url      = ac.getSyncURL();

    if (username == NULL || strlen(username) == 0 ||
        pwd      == NULL || strlen(pwd)      == 0 ||
        url      == NULL || strlen(url)      == 0)
    {
        QMessageBox msgBox(QMessageBox::Warning, "Warning", "Please set your account info first", QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    // Step 2: check source configuration
    AppSyncSourceConfig* sourceConfig = appSource->getConfig();
    qint64 collId = sourceConfig->getCollectionId();
    // If the user did not set any collection id, we check if there is only one
    // available for this source. In this case we auto set it
    if (collId == -1) {
        CollectionsFetcher fetcher(appSource->getAkonadiMimeType());
        QList<Collection> collections = fetcher.fetch();
        if (collections.size() == 1) {
            Collection c = collections.front();
            sourceConfig->setCollectionId(c.id());
            LOG.info("Auto selecting resource with id: %lld", c.id());
            collId = c.id();
            sourceConfig->save();
        } else {
            QMessageBox msgBox(QMessageBox::Warning, "Warning", "Please select a resource to sync first", QMessageBox::Ok);
            msgBox.exec();
            return;
        }
    }

    // At this point the sync is about to start
    //emit sourceStarted(appSource);
    // Run the synchronization in a separate thread
    // TODO: how do we release this object?
    SyncThread *st = new SyncThread(this, client, appSource);
    st->start();
}

void SourceManager::emitSourceStarted(AppSyncSource* appSource) {
    emit sourceStarted(appSource);
}

void SourceManager::emitSourceEnded(AppSyncSource* appSource, SyncReport* report) {
    emit sourceEnded(appSource, report);
}

void SourceManager::emitAddReceived(const char* key) {
    emit addReceived(key);
}

void SourceManager::emitUpdReceived(const char* key) {
    emit updReceived(key);
}

void SourceManager::emitDelReceived(const char* key) {
    emit delReceived(key);
}

void SourceManager::emitAddSent(const char* key) {
    emit addSent(key);
}

void SourceManager::emitUpdSent(const char* key) {
    emit updSent(key);
}

void SourceManager::emitDelSent(const char* key) {
    emit delSent(key);
}

void SourceManager::emitTotalServerItems(int n) {
    emit totalServerItems(n);
}

void SourceManager::emitTotalClientItems(int n) {
    emit totalClientItems(n);
}

/**
 * This class implements the SyncListener interface in order to be notified of
 * the SyncEvents. It simply displays the sync messages on the standard
 * output.
 */
class AkunambolListener : public SyncListener {
    public:

        AkunambolListener(SourceManager* m) : SyncListener("Akunambol"),
                                                               manager(m)
        {
        }

        void syncBegin (SyncEvent& /*event*/) {
        }

        void syncEnd (SyncEvent& /*event*/) {
        }

        void sendInitialization (SyncEvent& /*event*/) {
            printf("\nConnecting to the server");
        }
        void sendFinalization (SyncEvent& /*event*/) {
            printf("Sending finalization");
        }
        void syncError (SyncEvent& event) {
            printf("\nSynchronization error: %s\n", event.getMessage());
        }
        void setSource(AppSyncSource* s) {
            source = s;
        }
    private:
        SourceManager* manager;
        AppSyncSource* source;
};




/**
 * This class implements the SyncItemListener intrface in order to be notified
 * of the SyncItemEvents. It simply displays the sync messages on the standard
 * output.
 */
class AkunambolItemListener : public SyncItemListener
{
    public:

        AkunambolItemListener(SourceManager* m) : SyncItemListener("Akunambol"),
                                                  manager(m)
        { 
            itemsAddedByServerCount   = 0;
            itemsUpdatedByServerCount = 0;
            itemsDeletedByServerCount = 0;
            itemsAddedByClientCount   = 0;
            itemsUpdatedByClientCount = 0;
            itemsDeletedByClientCount = 0;
        }

        void itemAddedByServer (SyncItemEvent& event) {
            manager->emitAddReceived(event.getItemKey());
        }

        void itemDeletedByServer (SyncItemEvent& event) {
            manager->emitDelReceived(event.getItemKey());
        }

        void itemUpdatedByServer (SyncItemEvent& event) {
            manager->emitUpdReceived(event.getItemKey());
        }

        void itemAddedByClient (SyncItemEvent& event) {
            manager->emitAddSent(event.getItemKey());
        }

        void itemDeletedByClient (SyncItemEvent& event) {
            manager->emitDelSent(event.getItemKey());
        }

        void itemUpdatedByClient (SyncItemEvent& event) {
            manager->emitUpdSent(event.getItemKey());
        }

    private:

        unsigned int itemsAddedByServerCount;
        unsigned int itemsUpdatedByServerCount;
        unsigned int itemsDeletedByServerCount;
        unsigned int itemsAddedByClientCount;
        unsigned int itemsUpdatedByClientCount;
        unsigned int itemsDeletedByClientCount;

        SourceManager* manager;

};

/**
 * This class implements the SyncSourceListener interface in order to be
 * notified of the SyncSourceEvents. It simply displays the sync messages on
 * the standard output.
 */
class AkunambolSourceListener : public SyncSourceListener
{
    public:
  
        AkunambolSourceListener(SourceManager* m) : SyncSourceListener("Akunambol"),
                                                    manager(m)
        {
        }

        void syncSourceBegin (SyncSourceEvent& /*event*/) {
            //printf("Begin sync of files\n");
        }
        void syncSourceEnd (SyncSourceEvent& /*event*/) {
            //printf("End sync of files\n");
        }
        void syncSourceSyncModeRequested (SyncSourceEvent& /* ev */) {
            //SyncMode mode = (SyncMode)ev.getSyncMode();
            //printf("\nPerforming a %s sync.\n", syncModeKeyword(mode));
        }
        void syncSourceTotalClientItems (SyncSourceEvent& ev) {
            manager->emitTotalClientItems(ev.getData());
        }
        void syncSourceTotalServerItems (SyncSourceEvent& ev) {
            manager->emitTotalServerItems(ev.getData());
        }

    private:
        SourceManager* manager;

};

class AkunambolTransportListener : public TransportListener
{
    public:
  
        AkunambolTransportListener() : TransportListener("Akunambol") {} ;

//        void sendDataBegin    (TransportEvent& event) { putchar('.'); };
//        void syncDataEnd      (TransportEvent& event) { putchar('.'); };
//        void receiveDataBegin (TransportEvent& event) { putchar('.'); };
//        void receivingData    (TransportEvent& event) { putchar('.'); };
//        void receiveDataEnd   (TransportEvent& event) { putchar('.'); };

};

//----------------------------------------------------------------------- 

void SourceManager::setListeners() {
    ManageListener& lman = ManageListener::getInstance();
    
    LOG.debug("Set listeners.");

    syncListener = new AkunambolListener(this);
    lman.setSyncListener(syncListener);
    itemListener = new AkunambolItemListener(this);
    lman.setSyncItemListener(itemListener);
    sourceListener = new AkunambolSourceListener(this);
    lman.setSyncSourceListener(sourceListener);
    //lman.setTransportListener(new AkunambolTransportListener());
}

void SourceManager::unsetListeners() {
    LOG.debug("Unset listeners.");
    ManageListener::releaseAllListeners();
}



#include "sourcemanager.moc"
