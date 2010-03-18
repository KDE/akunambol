
#include "sourcemanager.h"

#include <QString>
#include <KDebug>

#include <base/adapter/PlatformAdapter.h>
#include <client/DMTClientConfig.h>
#include <spds/DefaultConfigFactory.h>

#include "KFunSyncConfig.h"

#include "../config.h"

USE_FUNAMBOL_NAMESPACE

// Stolen from Funambol's fsync example.
static StringBuffer generateDeviceID()
{
    StringBuffer devid;
    devid.sprintf("%s-%ld", "akonadi", time(NULL));
    return devid;
}

SourceManager::SourceManager()
{
    PlatformAdapter::init(KFUNSYNC_APPLICATION_URI);
//     m_conf = new DMTClientConfig;
    initConfig();
}

SourceManager::~SourceManager()
{
    delete m_conf;
}

DMTClientConfig* SourceManager::config()
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
    kDebug();
    KFunSyncConfig *config = KFunSyncConfig::getInstance();
    kDebug();
    // Initialize it (read from file or create the default one
    config->init();
    kDebug();
}
