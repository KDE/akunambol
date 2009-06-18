
#include "sourcemanager.h"

#include <KDebug>

#include <base/adapter/PlatformAdapter.h>
#include <client/DMTClientConfig.h>
#include <spds/DefaultConfigFactory.h>

USE_FUNAMBOL_NAMESPACE

SourceManager::SourceManager()
{
    PlatformAdapter::init("Akunambol/Akonadi");
    m_conf = new DMTClientConfig;
    initConfig();
}

SourceManager::~SourceManager()
{
    delete m_conf;
}

void SourceManager::initConfig()
{
    if (!m_conf->read()) {
        AccessConfig *ac = DefaultConfigFactory::getAccessConfig();
        ac->setUserAgent("Akunambol");
        m_conf->setAccessConfig(*ac);

        DeviceConfig *dc = DefaultConfigFactory::getDeviceConfig();

        SyncSourceConfig *sc = DefaultConfigFactory::getSyncSourceConfig("akonadi");
        kDebug() << "wwaa";
        delete ac;
        delete dc;
        delete sc;
    }
}
