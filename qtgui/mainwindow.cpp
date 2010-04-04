
// #include <QtGui>

#include <QStyle>
#include <QSettings>
#include <QMessageBox>

#include <KDebug>

#include <Akonadi/CollectionDialog>
#include <Akonadi/CollectionModel>
#include <Akonadi/CollectionView>

#include "../syncsource/sourcemanager.h"
#include "../akonadi/akonadi-dialog.h"

#include "config.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    m_c = 0;
    m_sourceManager = 0;
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
    m_sourceManager->setData(m_user, m_password, "http://my.funambol.com/sync"); //FIXME UNHARDCODE!
    
    kDebug();
    ContactsSyncer *contactsSyncer = new ContactsSyncer(m_sourceManager);
    connect(contactsSyncer, SIGNAL(finishedSync()), this, SLOT(finishedSync()));
//     w.exec();
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
    m_c->exec();
    parseConfigDialog();
}

void MainWindow::loadConfig()
{
    m_sourceManager = new SourceManager;
    
    // TODO: Read config from disk
    QSettings s("Funambol", "Akunambol");
    m_user = s.value("user").toString();
    m_password = s.value("password").toString(); // TODO decrypt?
}

void MainWindow::writeConfig()
{
    QSettings s("Funambol", "Akunambol");
    s.setValue("user", m_user);
    s.setValue("password", m_password); // TODO encrypt?
}

void MainWindow::parseConfigDialog()
{
    if (m_c->result() == QDialog::Accepted) {
        m_user = m_c->user();
        m_password = m_c->password();
        writeConfig();
    }
}

MainWindow::~MainWindow()
{

}
