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
    //setenv("XDG_CONFIG_HOME", dir.c_str(), 1);
    //setenv("HOME", dir.c_str(), 1);

    PlatformAdapter::init("Funambol/Akunambol", env, true);
    // Init the logger
    KDELog *KDELogger = new KDELog;
    Log::setLogger(KDELogger);
    LOG.setLevel(LOG_LEVEL_DEBUG);
    
    MainWindow w;
    w.show();
    
    return app.exec();
}
