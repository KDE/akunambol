#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"

class Config;
class SourceManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~MainWindow();

private slots:
    void launchConfigDialog();
    void launchAboutDialog();
    void syncContacts();
    void startedSync();
    void finishedSync();

private:
    void init();
    
    void parseConfigDialog();
    void loadConfig();
    void setIcons();
    void writeConfig();
    
    Ui::MainWindowClass ui;

    Config *m_c;
    SourceManager *m_sourceManager;

    QString m_user;
    QString m_password;
    QString m_syncUrl;
};

#endif // MAINWINDOW_H
