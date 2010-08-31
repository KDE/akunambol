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

#ifndef INCL_FSYNCCONFIG
#define INCL_FSYNCCONFIG

#include "base/fscapi.h"
#include "base/util/StringBuffer.h"
#include "client/DMTClientConfig.h"

USE_NAMESPACE

// This is relative to the config root specific for each platform.
// E.g. HKCU/Software on Windows or $HOME/.config on Unix
#define AKUNAMBOL_APPLICATION_URI         "Funambol/Akunambol"

// Log settings
#define KFUNSYNC_LOG_TITLE          "Funambol FileSync Log"
#define KFUNSYNC_LOG_PATH           "."
#define KFUNSYNC_LOG_LEVEL          LOG_LEVEL_DEBUG

// Name of the sync source
#define KFUNSYNC_SOURCE_NAME           "contact"
#define KFUNSYNC_CAL_SOURCE_NAME       "calendar"
#define KFUNSYNC_TASK_SOURCE_NAME      "task"

// Device info: stored in the client config and sent to the DS server.
#define KFUNSYNC_DEVICE_ID         "aku"
#define KFUNSYNC_SW_VERSION        "0.1"
#define KFUNSYNC_USER_AGENT        "Akunambol " KFUNSYNC_SW_VERSION

/**
 * This class extends DMTClientConfig to store the default configuration suited to this client.
 *
 * It implements the Singleton pattern to be available from any point of
 * the application.
 */
class KFunSyncConfig : public DMTClientConfig {

public:

    /**
     * Singleton implementation: get the unique instance of the config.
     */
    static KFunSyncConfig *getInstance();

    /**
     * Singleton implementation: release the unique instance of the config.
     */
    static void dispose();
    
    ~KFunSyncConfig();

    // Overloaded methods from DMTClientConfig
    virtual bool read();
    virtual bool save();

    /**
     * Initialize the config: try to read it from file or generate a default one.
     */
    void init();

private:

    //------------------------------------------------------------------

    /** Singleton implementation: the unique FSyncConfig instance */
    static KFunSyncConfig *instance;

    /**
     * Default constructor: uses the macros KFUNSYNC_APPLICATION_URI and
     * KFUNSYNC_DEFAULT_PATH to initialize the config.
     * By default also reads the config from the store.
     */
    KFunSyncConfig();

    /**
     * Generates a default config.
     */
    void createConfig();

};

#endif
// kate: indent-mode cstyle; space-indent on; indent-width 0; 
