
#include "sourcemanager.h"

#include <QString>
#include <KDebug>

#include <base/adapter/PlatformAdapter.h>
#include <client/DMTClientConfig.h>
#include <spds/DefaultConfigFactory.h>

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
    PlatformAdapter::init("Funambol/Akunambol");
    m_conf = new DMTClientConfig;
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
    if (!m_conf->read()) {
        AccessConfig *ac = DefaultConfigFactory::getAccessConfig();
        ac->setUserAgent("Akunambol " VERSION);
        m_conf->setAccessConfig(*ac);

        DeviceConfig *dc = DefaultConfigFactory::getDeviceConfig();
        dc->setDevID(generateDeviceID());
        dc->setMan("Akunambol");
        dc->setSwv(VERSION);
        m_conf->setDeviceConfig(*dc);

        SyncSourceConfig *sc = DefaultConfigFactory::getSyncSourceConfig("akonadi");
        m_conf->setSyncSourceConfig(*sc);

        m_conf->save();
        kDebug() << "wwaa";
        delete ac;
        delete dc;
        delete sc;
    }
}