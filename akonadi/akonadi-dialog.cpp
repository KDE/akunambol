#include "akonadi-dialog.h"
#include "ui_akonadi-dialog.h"
#include "contacts.h"

#include <QtGui>

#include <Akonadi/CollectionFetchJob>
#include <Akonadi/ItemFetchJob>
#include <Akonadi/ItemFetchScope>
#include <Akonadi/Monitor>
#include <Akonadi/CollectionDialog>

#include <kabc/addressee.h>
#include <kabc/phonenumber.h>
#include <kabc/picture.h>
#include <kabc/key.h>

#include "../syncsource/sourcemanager.h"

#include <KDebug>

using namespace Akonadi;

Dialog::Dialog(SourceManager *s, QWidget *parent)
    : QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);
    m_sourceManager = s;
    
    
    CollectionDialog dlg( this );
    dlg.setMimeTypeFilter( QStringList() << KABC::Addressee::mimeType() );
    //dlg.setAccessRightsFilter( Collection::CanCreateItem );
    dlg.setDescription( i18n( "Select an address book for saving:" ) );
    dlg.exec();
    
    c = new Contacts(this);
    connect(c, SIGNAL(ready()), SLOT(init()));
    connect(ui->tableWidget, SIGNAL(cellClicked(int, int)), SLOT(loadContactsFor(int)));
    connect(ui->syncButton, SIGNAL(clicked()), this, SLOT(startSync()));
}

Dialog::~Dialog()
{
    delete ui;
    delete c;
}

void Dialog::init()
{
    populateTable();
    disconnect(c, SIGNAL(ready()), this, SLOT(init())); // ready has served its purposes
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

void Dialog::displayContacts(int wa)
{
    ui->contactsTable->setRowCount(c->itemsForCollection(0).count()); //FIXME
    ui->contactsTable->setColumnCount(3);
    ui->contactsTable->horizontalHeader()->setStretchLastSection(true);
    QStringList headers;
    headers << "ID" << "Name" << "E-Mail";
    ui->contactsTable->setHorizontalHeaderLabels(headers);
//     kDebug() << "waa";
    int i = 0;
    foreach(Item item, c->itemsForCollection(0)) {
        KABC::Addressee a = item.payload<KABC::Addressee>();
        ui->contactsTable->setItem(i, 0, new QTableWidgetItem(QString::number(item.id())));
        ui->contactsTable->setItem(i, 1, new QTableWidgetItem(a.realName()));
        ui->contactsTable->setItem(i, 2, new QTableWidgetItem(a.fullEmail()));
//         kDebug() << a.formattedName();
        i++;
        kDebug() << "items added:" << i;
    }
}

void Dialog::loadContactsFor(int id)
{
    c->loadContactsForCollection(ui->tableWidget->item(id, 0)->text().toInt());
    connect(c, SIGNAL(loadedCollection(int)), SLOT(displayContacts(int)));
}

void Dialog::startSync()
{
    //TODO check if we did select anything or we should abort
    m_sourceManager->setAkonadiItems(c->itemsForCollection(0));
    m_sourceManager->sync();
    
}


