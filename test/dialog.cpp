#include "dialog.h"
#include "ui_dialog.h"

#include <KDebug>

#include "../syncsource/sourcemanager.h"
#include <client/FileSyncSource.h>
#include <client/SyncClient.h>
#include <client/DMTClientConfig.h>

//include

Dialog::Dialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);
    s = new SourceManager;
    connect(ui->syncButton, SIGNAL(clicked()), this, SLOT(sync()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::sync()
{
    DMTClientConfig conf("Funambol/test");
    kDebug() << "perform sync";
    s->setData(ui->usernameLineEdit->text(), ui->passwordLineEdit->text(), ui->serverLineEdit->text());
    FileSyncSource f(ui->pathLineEdit->text().toAscii(), s->config()->getSyncSourceConfig("akonadi") );

    SyncSource* ssArray[] = { &f };

    SyncClient sampleClient;

    // 5.Start the sync!
    if( sampleClient.sync(*s->config(), ssArray) ) {
        kDebug() << "Synkd!";
    }

    // 6.Print sync results.
    StringBuffer res;
	sampleClient.getSyncReport()->toString(res);
    printf("\n%s", res.c_str());

    kDebug() << "sync done";
//ui->pathLineEdit->text().toAscii().data()
}
