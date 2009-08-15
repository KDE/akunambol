#include "akonadi-dialog.h"
#include "ui_akonadi-dialog.h"
#include "contacts.h"

#include <QtGui>

#include <Akonadi/CollectionFetchJob>
#include <Akonadi/ItemFetchJob>
#include <Akonadi/ItemFetchScope>
#include <Akonadi/Monitor>

#include <kabc/addressee.h>
#include <kabc/phonenumber.h>
#include <kabc/picture.h>
#include <kabc/key.h>

#include <KDebug>

using namespace Akonadi;

Dialog::Dialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);
    
    c = new Contacts(this);
    connect(c, SIGNAL(ready()), SLOT(init()));
    connect(ui->tableWidget, SIGNAL(cellClicked(int, int)), SLOT(loadContactsFor(int)));
}

Dialog::~Dialog()
{
    delete ui;
    delete c;
}

void Dialog::init()
{
    populateTable();
}

void Dialog::populateTable()
{
    ui->tableWidget->setRowCount(c->collections().count());
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    QStringList headers;
    headers << "ID" << "Name";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    
    int i = 0;
    foreach(Collection collection, c->collections()) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(collection.id())));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(collection.name()));
        i++;
        kDebug() << "collection added: id" << collection.id();
    }
}

void Dialog::loadContactsFor(int id)
{
    kDebug() << ui->tableWidget->item(id, 0)->text().toInt();
}

