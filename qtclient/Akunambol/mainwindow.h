#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"

class Config;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~MainWindow();

private slots:
    void launchConfigDialog();

private:
    void parseConfigDialog();
    void loadConfig();
    Ui::MainWindowClass ui;

    Config *m_c;

    QString m_user;
    QString m_password;
};

#endif // MAINWINDOW_H
