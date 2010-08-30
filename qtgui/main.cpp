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
#include "spds/SyncSource.h"
#include "spds/SyncSourceReport.h"

// Qt/KDE
#include <KApplication>
#include <KCmdLineArgs>
#include <KLocalizedString>
#include <KStandardDirs>
#include <QLabel>

// Akunambol
#include "../config.h"
#include "mainwindow.h"
#include "standardsourcesettings.h"
#include "KDELog.h"

#include "syncsource/KFunSyncConfig.h"
#include "syncsource/contactssource.h"
#include "syncsource/eventssource.h"
#include "syncsource/taskssource.h"

#include "client/appsyncsourcemanager.h"
#include "client/appsyncsource.h"
#include "client/appsyncsourceconfig.h"

#include "qtgui/sourcepushbutton.h"


void setupSources() {

    AppSyncSourceManager *manager = AppSyncSourceManager::getInstance();

    // Register all the sources
    // Contacts
    SyncSourceConfig *srcConfig = KFunSyncConfig::getInstance()->getSyncSourceConfig(KFUNSYNC_SOURCE_NAME);
    ContactsSource *cSource = new ContactsSource(KFUNSYNC_SOURCE_NAME, srcConfig, NULL);
    SyncSourceReport* report = new SyncSourceReport(KFUNSYNC_SOURCE_NAME);
    cSource->setReport(report);

    AppSyncSourceConfig* cConfig  = new AppSyncSourceConfig("Contacts");
    AppSyncSource* contactsSource = new AppSyncSource("Contacts", cSource);
    contactsSource->setConfig(cConfig);
    contactsSource->setAkonadiMimeType("text/directory");
    StandardSourceSettings *cSettings = new StandardSourceSettings(contactsSource);
    contactsSource->setSettingsTab(cSettings);
    SourcePushButton* cButton = new SourcePushButton(contactsSource, contactsSource->getName());
    contactsSource->setPushButton(cButton);
    manager->registerSource(contactsSource);

    // Events
    srcConfig = KFunSyncConfig::getInstance()->getSyncSourceConfig(KFUNSYNC_CAL_SOURCE_NAME);
    EventsSource *eSource = new EventsSource(KFUNSYNC_CAL_SOURCE_NAME, srcConfig, NULL);
    report = new SyncSourceReport(KFUNSYNC_CAL_SOURCE_NAME);
    eSource->setReport(report);

    AppSyncSourceConfig* eConfig  = new AppSyncSourceConfig("Events");
    AppSyncSource* eventsSource = new AppSyncSource("Events", eSource);
    eventsSource->setConfig(eConfig);
    eventsSource->setAkonadiMimeType("text/calendar");
    StandardSourceSettings *eSettings = new StandardSourceSettings(eventsSource);
    eventsSource->setSettingsTab(eSettings);
    SourcePushButton* eButton = new SourcePushButton(eventsSource, eventsSource->getName());
    eventsSource->setPushButton(eButton);
    manager->registerSource(eventsSource);

    // Tasks
    srcConfig = KFunSyncConfig::getInstance()->getSyncSourceConfig(KFUNSYNC_TASK_SOURCE_NAME);
    TasksSource *tSource = new TasksSource(KFUNSYNC_TASK_SOURCE_NAME, srcConfig, NULL);
    report = new SyncSourceReport(KFUNSYNC_TASK_SOURCE_NAME);
    tSource->setReport(report);

    AppSyncSourceConfig* tConfig  = new AppSyncSourceConfig("Tasks");
    AppSyncSource* tasksSource = new AppSyncSource("Tasks", tSource);
    tasksSource->setConfig(tConfig);
    tasksSource->setAkonadiMimeType("text/calendar");
    StandardSourceSettings *tSettings = new StandardSourceSettings(tasksSource);
    tasksSource->setSettingsTab(tSettings);
    SourcePushButton* tButton = new SourcePushButton(tasksSource, tasksSource->getName());
    tasksSource->setPushButton(tButton);
    manager->registerSource(tasksSource);

}

int main(int argc, char *argv[])
{
    KCmdLineArgs::init(argc, argv, "akunambol", "", ki18n("Akunambol"),
                       VERSION, ki18n("SyncML Client for Akonadi using Funambol libraries"));
    
    KApplication app;

    // Set directories vars (overriding env settings)
    QString dName = KStandardDirs::locateLocal("appdata", "");
    StringBuffer dir = (const char *)(dName.toLatin1());

    StringMap env;
    // Stuff needed by funambol
    env.put("CONFIG_FOLDER", dir.c_str());
    env.put("HOME_FOLDER", dir.c_str());
    PlatformAdapter::init("Funambol/Akunambol", env, true);
    
    // Init the logger
    KDELog *KDELogger = new KDELog;
    Log::setLogger(KDELogger);
    LOG.setLevel(LOG_LEVEL_INFO);

    // Init the configuration
    KFunSyncConfig::getInstance()->init();
    KFunSyncConfig::getInstance()->read();

    // Setup sources
    setupSources();
    
    // Fire the UI
    MainWindow w;
    w.show();
    
    return app.exec();
}


