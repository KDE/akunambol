
// Funambol includes
#include "base/adapter/PlatformAdapter.h"
#include "base/Log.h"

// Qt/KDE
#include <KApplication>
#include <KCmdLineArgs>
#include <KLocalizedString>

// Akunambol
#include "../config.h"
#include "mainwindow.h"
#include "../syncsource/KFunSyncConfig.h"
#include "KDELog.h"

int main(int argc, char *argv[])
{
    KCmdLineArgs::init(argc, argv, "akunambol", "", ki18n("Akunambol"), VERSION, ki18n("SyncML Client for Akonadi using Funambol libraries"));
    
    KApplication app;
    
    PlatformAdapter::init("akunambol");
    // Init the logger
    KDELog *KDELogger = new KDELog;
    Log::setLogger(KDELogger);
    LOG.setLevel(LOG_LEVEL_DEBUG);
    
    MainWindow w;
    w.show();
    
    return app.exec();
}
