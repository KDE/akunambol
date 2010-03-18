/*
 * Funambol is a mobile platform developed by Funambol, Inc. 
 * Copyright (C) 2010 Funambol, Inc.
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License version 3 as published by
 * the Free Software Foundation with the addition of the following permission 
 * added to Section 15 as permitted in Section 7(a): FOR ANY PART OF THE COVERED
 * WORK IN WHICH THE COPYRIGHT IS OWNED BY FUNAMBOL, FUNAMBOL DISCLAIMS THE 
 * WARRANTY OF NON INFRINGEMENT  OF THIRD PARTY RIGHTS.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program; if not, see http://www.gnu.org/licenses or write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA.
 * 
 * You can contact Funambol, Inc. headquarters at 643 Bair Island Road, Suite 
 * 305, Redwood City, CA 94063, USA, or at email address info@funambol.com.
 * 
 * The interactive user interfaces in modified source and object code versions
 * of this program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU Affero General Public License version 3.
 * 
 * In accordance with Section 7(b) of the GNU Affero General Public License
 * version 3, these Appropriate Legal Notices must retain the display of the
 * "Powered by Funambol" logo. If the display of the logo is not reasonably 
 * feasible for technical reasons, the Appropriate Legal Notices must display
 * the words "Powered by Funambol".
 */

#include <Qt/qapplication.h>
#include <kapplication.h>
#include <kcmdlineargs.h>

#include "base/fscapi.h"

#include "base/adapter/PlatformAdapter.h"
#include "client/SyncClient.h"
#include "client/FileSyncSource.h"
#include "base/util/StringBuffer.h"
#include "base/util/utils.h"
#include "base/Log.h"

#include "KFunSyncConfig.h"
#include "KFunSyncOpt.h"

#include "ContactsSource.h"
#include "CalendarSource.h"

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif


USE_NAMESPACE

static const char *progname = "kfunsync";

bool doSync(KFunSyncOpt& options) 
{
    // Create the contact sync source passing its name, the SyncSourceConfig 
    SyncSourceConfig *srcConfig = KFunSyncConfig::getInstance()->getSyncSourceConfig(KFUNSYNC_SOURCE_NAME);
    ContactsSource contactsSource(KFUNSYNC_SOURCE_NAME, srcConfig , NULL);

    // Create the calendar sync source passing its name, the SyncSourceConfig 
    srcConfig = KFunSyncConfig::getInstance()->getSyncSourceConfig(KFUNSYNC_CAL_SOURCE_NAME);
    CalendarSource calendarSource(KFUNSYNC_CAL_SOURCE_NAME, srcConfig, NULL);
    
    // Initialize the SyncSource array to sync
    //SyncSource* ssArray[] = { &contactsSource, NULL } ;
    SyncSource* ssArray[] = { &calendarSource, NULL } ;

    // Create the SyncClient
    SyncClient client;

    // SYNC!
    if (client.sync(*KFunSyncConfig::getInstance(), ssArray)) {
        LOG.error("Error during sync.\n");
        return false;
    }

    // Save the anchors
    KFunSyncConfig::getInstance()->save();

    return true;
}

//------------------------------------------------------------------------ Main
int main(int argc, char* argv[]) 
{
    QApplication app(argc, argv);
    //KApplication(false);
    //KCmdLineArgs::init( argc, argv, NULL );

    PlatformAdapter::init(KFUNSYNC_APPLICATION_URI);

    // Get the config instance
    KFunSyncConfig *config = KFunSyncConfig::getInstance();
    // Initialize it (read from file or create the default one
    config->init();

    // Init LOG
    LOG.setLevel(config->getDeviceConfig().getLogLevel());
    LOG.reset(KFUNSYNC_LOG_TITLE);

    // Initialize the command line options handler
    KFunSyncOpt opts(progname); 

    // Parse command line options
    if (opts.parseCmdline(argc, argv) == false) {
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
}

