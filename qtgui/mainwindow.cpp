
// #include <QtGui>

#include <QStyle>
#include <QSettings>
#include "config.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    m_c = 0;
    ui.setupUi(this);
    connect(ui.actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui.actionConfigure_Akunambol, SIGNAL(triggered()), this, SLOT(launchConfigDialog()));

//    resize(minimumSizeHint()); // This looks like a sensible default
    setIcons();
    loadConfig();
}

void MainWindow::setIcons()
{
    ui.actionQuit->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
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
    } else {
        // Don't do anything?
    }
}

MainWindow::~MainWindow()
{

}
