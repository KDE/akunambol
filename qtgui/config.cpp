#include "config.h"
#include "ui_config.h"

Config::Config(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::Config)
{
    m_ui->setupUi(this);
    m_ui->syncUrl->setText("http://my.funambol.com/sync"); // Default value (TODO use KConfig)
}

Config::~Config()
{
    delete m_ui;
}

void Config::setUser(const QString &text)
{
    m_ui->user->setText(text);
}

void Config::setPassword(const QString &text)
{
    m_ui->password->setText(text);
}

void Config::setSyncUrl(const QString& text)
{
    m_ui->syncUrl->setText(text);
}

QString Config::user()
{
    return m_ui->user->text();
}

QString Config::password()
{
    return m_ui->password->text();
}

QString Config::syncUrl()
{
    return m_ui->syncUrl->text();
}

void Config::changeEvent(QEvent *e)
{
    switch(e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
