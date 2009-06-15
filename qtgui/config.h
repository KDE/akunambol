#ifndef CONFIG_H
#define CONFIG_H

#include <QtGui/QDialog>

namespace Ui {
    class Config;
}

class Config : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(Config)
public:
    explicit Config(QWidget *parent = 0);
    virtual ~Config();

    void setUser(const QString &text);
    void setPassword(const QString &text);
    QString user();
    QString password();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::Config *m_ui;
};

#endif // CONFIG_H
