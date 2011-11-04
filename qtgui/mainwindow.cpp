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

// #include <QtGui>

#include <QList>
#include <QStyle>
#include <QSettings>
#include <KMessageBox>
#include <QSpacerItem>
#include <QProgressDialog>

#include <KDebug>
#include <KMenu>
#include <KMenuBar>
#include <KStatusBar>

#include <Akonadi/CollectionDialog>
#include <Akonadi/CollectionModel>
#include <Akonadi/CollectionView>

#include <KAboutApplicationDialog>
#include <KStandardDirs>

#ifndef NO_LIKEBACK
#include <LikeBack/LikeBack>
#endif

#include <library/syncsourceloader.h>
#include <library/syncsource.h>

#include "base/util/StringBuffer.h"
#include "spds/AccessConfig.h"
#include "spds/SyncReport.h"

#include "client/appsyncsource.h"
#include "client/appsyncsourcemanager.h"
#include "syncsource/KFunSyncConfig.h"
#include "../syncsource/sourcemanager.h"
#include "../syncsource/statuslistener.h"
#include "../syncsource/syncconfig.h"
// #include "../syncsource/contactssyncer.h"

#include "sourcepushbutton.h"
#include "aku-auto-config.h"
#include "settings.h"
#include "mainwindow.h"
#include <KAction>
#include <kstandardaction.h>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
    : KMainWindow(parent, flags)
{
    m_s = 0;
    m_syncDialog = 0;
    m_sourceManager = new SourceManager;
    
//     connect(ui.actionQuit, SIGNAL(triggered()), this, SLOT(close()));
//     connect(ui.actionConfigure_Akunambol, SIGNAL(triggered()), this, SLOT(launchConfigDialog()));
//     connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(launchAboutDialog()));
    QWidget *widget = new QWidget(this);
    
//     Create a button for each existing source
    m_controlsLayout = new QVBoxLayout(widget);
    
    QHBoxLayout *hLayout = new QHBoxLayout;
    QLabel* logo = new QLabel("cacca", widget);
    logo->setPixmap(KIcon("akunambol").pixmap(64));
    hLayout->addWidget(logo);
    hLayout->addWidget(new QLabel(QString("Akunambol %1").arg(AKU_VERSION), widget));
    m_controlsLayout->addLayout(hLayout);
    
    AppSyncSourceManager *manager = AppSyncSourceManager::getInstance();
    QList<AppSyncSource*> sources = manager->getRegisteredSources();
    foreach (AppSyncSource* source, sources) {
        LOG.debug("Creating a button for source: %s", source->getName());

//         Add the push button
        SourcePushButton* sourceButton = source->getPushButton();
        m_controlsLayout->addWidget(sourceButton);
        connect(sourceButton, SIGNAL(clicked(AppSyncSource*)), this, SLOT(sync(AppSyncSource*)));
    }
    
    m_controlsLayout->addStretch(2);
    widget->setLayout(m_controlsLayout);
    setCentralWidget(widget);
    
    loadConfig();
    
    statusBar()->showMessage(tr("Configuration loaded."));
    KMenu *file = new KMenu(i18n("File"));
    file->addAction(KStandardAction::quit(this, SLOT(close()), this));
    KMenu *settings = new KMenu(i18n("Settings"));
    
    KAction *configureAction = new KAction(KIcon("configure"), i18n("Configure Akunambol"), this);
    configureAction->setShortcut(QString("Ctrl+s"));
    connect(configureAction, SIGNAL(triggered()), this, SLOT(launchConfigDialog()));
    
    settings->addAction(configureAction);
    KMenu *help = helpMenu(i18n("Help"));
    
    menuBar()->addMenu(file);
    menuBar()->addMenu(settings);
    menuBar()->addMenu(help);

#ifndef NO_LIKEBACK 
    LikeBack *likeBack = new LikeBack(LikeBack::AllButtons, true);
    likeBack->setAcceptedLanguages( QStringList() << "en" << "it" );
    likeBack->setServer("aku-likeback.ruphy.org", "/send.php");
#endif

    resize(260, 230);
    
    m_loader = new SyncSourceLoader(this);
    connect(m_loader, SIGNAL(syncSourceLoaded(SyncSource2*)), SLOT(pluginLoaded(SyncSource2*)));
    m_loader->loadAllSavedSyncSources();
}

void MainWindow::pluginLoaded(SyncSource2* s)
{
    QHBoxLayout *l = new QHBoxLayout;
    QPushButton *b = new QPushButton(s->controlText(), this);
    QPushButton *bc = new QPushButton(i18n("Configure"), this);
    
    l->addWidget(b);
    l->addWidget(bc);
    
    m_controlsLayout->addItem(l);
    
    connect(s, SIGNAL(newStatus(SyncSource2::SyncStatus)), this, SLOT(syncStatusChanged(SyncSource2::SyncStatus)));
    // add settings
    
    connect(b, SIGNAL(clicked()), s, SLOT(triggerSync()));
    
#if 0
    KSharedConfig::openConfig(KStandardDirs::locateLocal("appdata", s->))
//     KConfigSkeleton *skeleton = new KConfigSkeleton();
    // Show settings
    KConfigDialog *dialog = new KConfigDialog(this, i18n("Settings"), );
    s->createConfigurationInterface(dialog);
    connect(dialog, SIGNAL(settingsChanged(const QString&)), s, SLOT(configChanged()));
//     QWidget *w = s->configurationInterface();
//  FIXME: w must be a child window of "this"
    connect(bc, SIGNAL(clicked()), dialog, SLOT(show()));
#endif
//  FIXME: do what is written below:
//     connect(s, SIGNAL(newStatusMessage(QString)), m_syncDialog, SLOT(setLabelText(QString)));
    connect(s, SIGNAL(newStatusMessage(QString)), statusBar(), SLOT(showMessage(QString)));
}

void MainWindow::syncStatusChanged(SyncSource2::SyncStatus status)
{
    if (status == SyncSource2::SyncStarted) {
        QTimer::singleShot(0, this, SLOT(syncTriggered()));
    } else if (status == SyncSource2::SyncSuccess ||
               status == SyncSource2::SyncError) {
        QTimer::singleShot(0, this, SLOT(syncFinished()));
    }
}

void MainWindow::syncTriggered()
{   
//     connect(source, SIGNAL(error(QString)), SLOT(syncFinished()));
    
//     connect(source, SIGNAL(success()), 
  
//     SourceManager *syncer = new SourceManager(this);
//     
//     connect(this, SIGNAL(fireSync(AppSyncSource*)), syncer, SLOT(sync(AppSyncSource*)));
//     connect(syncer, SIGNAL(sourceStarted(AppSyncSource*)), this, SLOT(startedSync(AppSyncSource*)));
//     connect(syncer, SIGNAL(sourceEnded(AppSyncSource*, SyncReport*)), this, SLOT(finishedSync(AppSyncSource*, SyncReport*)));
//     connect(syncer, SIGNAL(addReceived(const char*)), this, SLOT(addReceived(const char*)));
//     connect(syncer, SIGNAL(delReceived(const char*)), this, SLOT(delReceived(const char*)));
//     connect(syncer, SIGNAL(updReceived(const char*)), this, SLOT(updReceived(const char*)));
//     connect(syncer, SIGNAL(addSent(const char*)), this, SLOT(addSent(const char*)));
//     connect(syncer, SIGNAL(delSent(const char*)), this, SLOT(delSent(const char*)));
//     connect(syncer, SIGNAL(updSent(const char*)), this, SLOT(updSent(const char*)));
//     connect(syncer, SIGNAL(totalServerItems(int)), this, SLOT(totalServerItems(int)));
//     connect(syncer, SIGNAL(totalClientItems(int)), this, SLOT(totalClientItems(int)));
//     emit fireSync(appSource);
    
//     numSent = 0;
//     numReceived = 0;
//     numServerItems = -1;
//     numClientItems = -1;
    
    m_syncDialog = new QProgressDialog(this);
    m_syncDialog->setLabelText(i18n("Connecting.."));
    m_syncDialog->setMinimum(0);
    m_syncDialog->setMaximum(0);
    // FIXME TODO: enable the cancel button to interrupt the sync
    m_syncDialog->setCancelButton(0);
    m_syncDialog->show();
}

void MainWindow::syncFinished()
{
    if (m_syncDialog) {
        m_syncDialog->cancel();
        m_syncDialog->deleteLater();
        m_syncDialog = 0;
    }
}

void MainWindow::sync(AppSyncSource* appSource)
{
    // Reapply the configuration
    m_sourceManager->setData(m_user, m_password, m_syncUrl);
    
    SourceManager *syncer = new SourceManager(this);

    connect(this, SIGNAL(fireSync(AppSyncSource*)), syncer, SLOT(sync(AppSyncSource*)));
    connect(syncer, SIGNAL(sourceStarted(AppSyncSource*)), this, SLOT(startedSync(AppSyncSource*)));
    connect(syncer, SIGNAL(sourceEnded(AppSyncSource*, SyncReport*)), this, SLOT(finishedSync(AppSyncSource*, SyncReport*)));
    connect(syncer, SIGNAL(addReceived(const char*)), this, SLOT(addReceived(const char*)));
    connect(syncer, SIGNAL(delReceived(const char*)), this, SLOT(delReceived(const char*)));
    connect(syncer, SIGNAL(updReceived(const char*)), this, SLOT(updReceived(const char*)));
    connect(syncer, SIGNAL(addSent(const char*)), this, SLOT(addSent(const char*)));
    connect(syncer, SIGNAL(delSent(const char*)), this, SLOT(delSent(const char*)));
    connect(syncer, SIGNAL(updSent(const char*)), this, SLOT(updSent(const char*)));
    connect(syncer, SIGNAL(totalServerItems(int)), this, SLOT(totalServerItems(int)));
    connect(syncer, SIGNAL(totalClientItems(int)), this, SLOT(totalClientItems(int)));
    emit fireSync(appSource);

    numSent = 0;
    numReceived = 0;
    numServerItems = -1;
    numClientItems = -1;

    m_syncDialog = new QProgressDialog(this);
    m_syncDialog->setLabelText(i18n("Connecting.."));
    m_syncDialog->setWindowModality(Qt::WindowModal);
    m_syncDialog->setMinimum(0);
    m_syncDialog->setMaximum(0);
    // FIXME TODO: enable the cancel button to interrupt the sync
    m_syncDialog->setCancelButton(0);
    m_syncDialog->exec();

}

void MainWindow::startedSync(AppSyncSource* /*appSource*/)
{

    // show a modal progress dialog during syncs (the sync process must run in a
    // different thread)
    //m_syncDialog = new QProgressDialog();
    //m_syncDialog->setLabelText(i18n("Connecting.."));
    //m_syncDialog->setWindowModality(Qt::WindowModal);
    //m_syncDialog->setMinimum(0);
    //m_syncDialog->setMaximum(0);
    //m_syncDialog->exec();

    //statusBar()->showMessage(i18n("Syncing..."));
}

void MainWindow::finishedSync(AppSyncSource* /* appSource */, SyncReport* report)
{
    if (m_syncDialog) {
        m_syncDialog->cancel();
        delete m_syncDialog;
        m_syncDialog = NULL;
    }
    sender()->deleteLater();

    // Log a report of the sync
    StringBuffer r;
    report->toString(r, false);
    LOG.info(r.c_str());

    if (report->getLastErrorCode() == 0) {
        statusBar()->showMessage(i18n("Last sync successful"));
    } else {
        //const char* errorMsg = report->getLastErrorMsg();
        statusBar()->showMessage(i18n("Last sync failed"));
    }
}

void MainWindow::addReceived(const char* key) {
    changeReceived(key);
}

void MainWindow::delReceived(const char* key) {
    changeReceived(key);
}

void MainWindow::updReceived(const char* key) {
    changeReceived(key);
}

void MainWindow::changeReceived(const char* /* key */) {
    if (m_syncDialog) {
        //QString msg = "Receiving " + (++numReceived); 
        QString msg = i18n("Receiving ");
        ++numReceived;
        char num[16];
        sprintf(num, "%d", numReceived);
        msg.append(num);

        if (numServerItems > 0) {
            sprintf(num, "/%d", numServerItems);
            msg.append(num);
        }

        m_syncDialog->setLabelText(msg);
    }
}

void MainWindow::addSent(const char* key) {
    changeSent(key);
}

void MainWindow::delSent(const char* key) {
    changeSent(key);
}

void MainWindow::updSent(const char* key) {
    changeSent(key);
}

void MainWindow::changeSent(const char* /* key */) {
    if (m_syncDialog) {
        QString msg = i18n("Sending ");
        ++numSent;
        char num[16];
        sprintf(num, "%d", numSent);
        msg.append(num); 

        if (numClientItems > 0) {
            sprintf(num, "/%d", numClientItems);
            msg.append(num);
        }

        m_syncDialog->setLabelText(msg);
    }
}

void MainWindow::totalServerItems(int n) {
    numServerItems = n;
}

void MainWindow::totalClientItems(int n) {
    numClientItems = n;
}

void MainWindow::launchConfigDialog()
{
    if (!m_s) {
        m_s = new Settings(this);
    }

    m_s->setUser(m_user);
    m_s->setPassword(m_password);
    m_s->setSyncUrl(m_syncUrl);
    m_s->setLogLevel(m_logLevel);
    m_s->exec();
    parseConfigDialog();
}

// TODO: make these methods use KConfig instead?

void MainWindow::loadConfig()
{
    // The configuration is stored into the DMTClientConfig
    KFunSyncConfig *config = KFunSyncConfig::getInstance();
    config->read();

    AccessConfig &ac = config->getAccessConfig();
    m_user = ac.getUsername();
    m_password = ac.getPassword();
    m_syncUrl = ac.getSyncURL();
    m_logLevel = config->getClientConfig().getLogLevel();
}

void MainWindow::writeConfig()
{
    KFunSyncConfig *config = KFunSyncConfig::getInstance();
    AccessConfig &ac = config->getAccessConfig();
    ac.setUsername(m_user.toUtf8());
    ac.setPassword(m_password.toUtf8());
    ac.setSyncURL(m_syncUrl.toUtf8());
    config->getClientConfig().setLogLevel(m_logLevel);
    config->save();
} 

void MainWindow::parseConfigDialog()
{
    if (m_s->result() == QDialog::Accepted) {
        m_user = m_s->user();
        m_password = m_s->password();
        m_syncUrl = m_s->syncUrl();
        m_logLevel = m_s->logLevel();
        LOG.setLevel(m_logLevel);
        writeConfig();
    }
}

MainWindow::~MainWindow()
{

}
