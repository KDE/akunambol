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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QProgressDialog>
#include <KMainWindow>
#include "ui_mainwindow.h"
#include <base/Log.h>

#include <spds/SyncReport.h>

#include <client/appsyncsource.h>
#include <library/syncsource.h>
#include <library/syncsourceloader.h>

class KPluginSelector;
class Config;
class Settings;
class SourceManager;
class SyncSource2;

class MainWindow : public KMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~MainWindow();

private slots:
    void pluginLoaded(SyncSource2 *s);
    void syncTriggered();
    void syncFinished();
    
    // old stuff
    void launchConfigDialog();
    void sync(AppSyncSource* source);
    void startedSync(AppSyncSource* source);
    void finishedSync(AppSyncSource* source, SyncReport* report);

    void addReceived(const char* key);
    void delReceived(const char* key);
    void updReceived(const char* key);
    void addSent(const char* key);
    void delSent(const char* key);
    void updSent(const char* key);

    void totalServerItems(int n);
    void totalClientItems(int n);
    
    void syncStatusChanged(SyncSource2::SyncStatus);

signals:
    void fireSync(AppSyncSource* appSource);

private:
    void parseConfigDialog();
    void loadConfig();
    void writeConfig();
    void changeSent(const char* key);
    void changeReceived(const char* key);

private:
    Settings *m_s;
    SourceManager *m_sourceManager;
    SyncSourceLoader *m_loader;
    
    KPluginSelector *m_selector;
    KDialog *m_selectorDialog;
    
    QVBoxLayout *m_controlsLayout;
    
    QString m_user;
    QString m_password;
    QString m_syncUrl;
    QProgressDialog *m_syncDialog;

    int numSent;
    int numReceived;

    int numServerItems;
    int numClientItems;
    Funambol::LogLevel m_logLevel;
};

#endif // MAINWINDOW_H
