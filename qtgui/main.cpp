
// Funambol includes
#include "base/adapter/PlatformAdapter.h"

// Qt/KDE
#include <QtGui/QApplication>

// Akunambol
#include "mainwindow.h"
#include "../syncsource/KFunSyncConfig.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    PlatformAdapter::init("akunambol");
    
    // Get the config instance
//     KFunSyncConfig *config = KFunSyncConfig::getInstance();
//     // Initialize it (read from file or create the default one
//     config->init();

    MainWindow w;
    w.show();
    
    // Init LOG
//     LOG.setLevel(config->getDeviceConfig().getLogLevel());
//     LOG.reset(KFUNSYNC_LOG_TITLE);
    
    // Initialize the command line options handler
//     KFunSyncOpt opts(progname); 
    
    // Parse command line options
/*    if (opts.parseCmdline(argc, argv) == false) {
        fprintf(stderr, "error parsing options: %s\n", opts.getErr());
        exit(EXIT_FAILURE);
    }
    
    // On user help request exit smoothly without doing anything 
    if (opts.optionSet("help")) { 
        exit(EXIT_SUCCESS);
    }
    
    // Initialize the view class to handle the feedbacks to the user.
    //FSyncUpdater updater;
    //updater.setListeners(opts.getVerbosity());
    
    // Sync
    if (doSync(opts) == false) {
        // Sync failed
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
    */
    
    return a.exec();
}
