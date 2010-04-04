
// Funambol includes
#include "base/adapter/PlatformAdapter.h"

// Qt/KDE
#include <KApplication>
#include <KCmdLineArgs>
#include <KLocalizedString>

// Akunambol
#include "../config.h"
#include "mainwindow.h"
#include "../syncsource/KFunSyncConfig.h"

int main(int argc, char *argv[])
{
    KCmdLineArgs::init(argc, argv, "akunambol", "", ki18n("Akunambol"), VERSION, ki18n("SyncML Client for Akonadi using Funambol libraries"));
    
    KApplication app;
    
    PlatformAdapter::init("akunambol");
    
    MainWindow w;
    w.show();
    
    return app.exec();
}
