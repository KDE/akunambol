#include <stdlib.h>

// Funambol includes
#include "base/adapter/PlatformAdapter.h"
#include "base/util/StringMap.h"
#include "base/Log.h"

// Qt/KDE
#include <KApplication>
#include <KCmdLineArgs>
#include <KLocalizedString>
#include <KStandardDirs>

// Akunambol
#include "../config.h"
#include "mainwindow.h"
#include "../syncsource/KFunSyncConfig.h"
#include "KDELog.h"

int main(int argc, char *argv[])
{
    KCmdLineArgs::init(argc, argv, "akunambol", "", ki18n("Akunambol"), VERSION, ki18n("SyncML Client for Akonadi using Funambol libraries"));
    
    KApplication app;

    // Set directories vars (overriding env settings)
    QString dName = KStandardDirs::locateLocal("appdata", "");
    const char* s = dName.toLatin1();
    StringBuffer dir(s);

    StringMap env;
    env.put("CONFIG_FOLDER", dir.c_str());
    env.put("HOME_FOLDER", dir.c_str());
    setenv("XDG_CONFIG_HOME", dir.c_str(), 1);
    setenv("HOME", dir.c_str(), 1);

    PlatformAdapter::init("Funambol/Akunambol", env);
    // Init the logger
    KDELog *KDELogger = new KDELog;
    Log::setLogger(KDELogger);
    LOG.setLevel(LOG_LEVEL_DEBUG);
    
    MainWindow w;
    w.show();
    
    return app.exec();
}
