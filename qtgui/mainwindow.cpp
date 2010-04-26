
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
