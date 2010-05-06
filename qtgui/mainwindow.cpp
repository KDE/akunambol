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

#include <QStyle>
#include <QSettings>
#include <QMessageBox>

#include <KDebug>

#include <Akonadi/CollectionDialog>
#include <Akonadi/CollectionModel>
#include <Akonadi/CollectionView>

#include "spds/AccessConfig.h"

#include "../syncsource/sourcemanager.h"
#include "../syncsource/KFunSyncConfig.h"
#include "../akonadi/akonadi-dialog.h"

#include "config.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    m_c = 0;
    m_sourceManager = new SourceManager;
    ui.setupUi(this);
    connect(ui.actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui.actionConfigure_Akunambol, SIGNAL(triggered()), this, SLOT(launchConfigDialog()));
    connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(launchAboutDialog()));
    connect(ui.contactsButton, SIGNAL(clicked()), this, SLOT(syncContacts()));
    
//    resize(minimumSizeHint()); // This looks like a sensible default
    setIcons();
    loadConfig();
    statusBar()->showMessage(tr("Configuration loaded."));
    init();
//    SourceManager s;
}

void MainWindow::init()
{
    
}

void MainWindow::setIcons()
{
    ui.actionQuit->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
}

void MainWindow::launchAboutDialog()
{
    QMessageBox::about(this, tr("About Akunambol"), tr("this release will eat your babies :)"));
}

void MainWindow::syncContacts()
{
    kDebug() << m_user << m_password << m_syncUrl;
    m_sourceManager->setData(m_user, m_password, m_syncUrl); //FIXME UNHARDCODE!
    
    kDebug();
    ContactsSyncer *contactsSyncer = new ContactsSyncer(m_sourceManager);
    connect(contactsSyncer, SIGNAL(startedSync()), SLOT(startedSync()));
    connect(contactsSyncer, SIGNAL(finishedSync()), SLOT(finishedSync()));
//     w.exec();
}

void MainWindow::startedSync()
{
    statusBar()->showMessage(i18n("Syncing..."));
}

void MainWindow::finishedSync()
{
    sender()->deleteLater();
    statusBar()->showMessage(i18n("Finished syncing."));
}

void MainWindow::launchConfigDialog()
{
    if (!m_c) {
        m_c = new Config(this);
    }

    m_c->setUser(m_user);
    m_c->setPassword(m_password);
    m_c->setSyncUrl(m_syncUrl);
    m_c->exec();
    parseConfigDialog();
}

void MainWindow::loadConfig()
{
    // The configuration is stored into the DMTClientConfig
    KFunSyncConfig *config = m_sourceManager->config();
    config->read();

    AccessConfig &ac = config->getAccessConfig();
    m_user = ac.getUsername();
    m_password = ac.getPassword();
    m_syncUrl = ac.getSyncURL();
}

void MainWindow::writeConfig()
{
    KFunSyncConfig *config = m_sourceManager->config();
    AccessConfig &ac = config->getAccessConfig();
    ac.setUsername(m_user.toLatin1());
    ac.setPassword(m_password.toLatin1());
    ac.setSyncURL(m_syncUrl.toLatin1());
    config->save();
}

void MainWindow::parseConfigDialog()
{
    if (m_c->result() == QDialog::Accepted) {
        m_user = m_c->user();
        m_password = m_c->password();
        m_syncUrl = m_c->syncUrl();
        writeConfig();
    }
}

MainWindow::~MainWindow()
{

}
