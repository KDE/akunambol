/*
 * Funambol is a mobile platform developed by Funambol, Inc.
 * Copyright (C) 2010 Funambol, Inc.
 * Copyright (C) 2010 Riccardo Iaconelli <riccardo@kde.org>
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

#include "base/fscapi.h"
#include "spdm/DMTreeFactory.h"
#include "spds/DefaultConfigFactory.h"
#include "spds/SyncItem.h"

#include <KDebug>

#include "KFunSyncConfig.h"

KFunSyncConfig *KFunSyncConfig::instance = NULL;

/**
 * Generate a unique device id
 *
 * @return  the deviceId
 */
static StringBuffer generateDeviceID() {
    StringBuffer devid;
    devid.sprintf("%s-%ld", KFUNSYNC_DEVICE_ID, time(NULL));
    return devid;
}

/**
 * Default constructor
 */
KFunSyncConfig::KFunSyncConfig(): DMTClientConfig(KFUNSYNC_APPLICATION_URI) {

}

KFunSyncConfig::~KFunSyncConfig() {
    instance = NULL;
}


/**
 * Singleton implementation: get the unique instance of the config.
 *
 * @return  the singleton object
 */
KFunSyncConfig *KFunSyncConfig::getInstance() {
    if (!instance) {
        instance = new KFunSyncConfig();
    }
    return instance;
}

/**
 * Singleton implementation: release the unique instance of the config.
 */
void KFunSyncConfig::dispose() {
    delete instance;
}


/**
 * Perform all the actions to initialize the configuration:
 * - read it from the storage, or create a default one
 * - handle backward compatibility
 */
void KFunSyncConfig::init() {
    // Read the configuration
    if (!read()) {
        // Config not found, generate a default one
        createConfig();
    }

    // Handle backward compatibility: if the stored version is
    // different from the current one, take the proper action
    if (strcmp(this->getDeviceConfig().getSwv(), KFUNSYNC_SW_VERSION) != 0) {
        // Handle differences in config. For instance, if a new parameter has
        // been added, initialize it so that it will be stored at the end.

        // TODO: just an example now, remember this at next version.
    }
}

/**
 * Overload the read() method, adding the code to retrieve client-specific
 * values.
 *
 * @return  true for success
 */
bool KFunSyncConfig::read() {

    if (!DMTClientConfig::read()) {
        return false; // error in the common config read.
    }

    if (!open()) {
        return false;
    }
    //
    // Read client-specific properties from the config
    //
    ManagementNode *node = dmt->readManagementNode(rootContext);

    if (node) {
        delete node;
        node = NULL;

        close();

        // return success
        return true;
    }

    close();

    // return failure
    return false;
}

/**
 * Overload the save() method, adding the code to store client-specific values.
 *
 * @return  true for success
 */
bool KFunSyncConfig::save() {

    if ( !DMTClientConfig::save() ) {
        return false; // error in the common config save.
    }

    if (!open()) {
        return false;
    }

    //
    // Write client-specific properties to the config
    //
    ManagementNode *node = dmt->readManagementNode(rootContext);
    if (node) {
        // cleanup
        delete node;
        node = NULL;

        close();

        // return success
        return true;
    }

    close();

    // return failure
    return false;
}

/**
 * Method to create a default config.
*/
void KFunSyncConfig::createConfig() {

    AccessConfig* ac = DefaultConfigFactory::getAccessConfig();
    ac->setMaxMsgSize(60000);
    ac->setUserAgent (KFUNSYNC_USER_AGENT);

    this->setAccessConfig(*ac);
    delete ac;

    DeviceConfig* dc = DefaultConfigFactory::getDeviceConfig();
    dc->setDevID(generateDeviceID());
    dc->setMan("Funambol");
    dc->setLoSupport(true);
    dc->setSwv(KFUNSYNC_SW_VERSION);
    this->setDeviceConfig(*dc);
    delete dc;

    // Configure the source to work with vCard 2.1
    SyncSourceConfig* sc = DefaultConfigFactory::getSyncSourceConfig(KFUNSYNC_SOURCE_NAME);
    sc->setType     ("text/x-vcard");
    sc->setURI      ("card");
    sc->setEncoding (SyncItem::encodings::plain);
    this->setSyncSourceConfig(*sc);
    delete sc;

    // Configure the source to work with ical
//     sc = DefaultConfigFactory::getSyncSourceConfig(KFUNSYNC_CAL_SOURCE_NAME);
//     sc->setType     ("text/calendar");
//     sc->setURI      ("event");
//     sc->setEncoding (SyncItem::encodings::plain);
//     this->setSyncSourceConfig(*sc);
//     delete sc;


    // save the configuration
    save();
}

// kate: indent-mode cstyle; space-indent on; indent-width 0; 
