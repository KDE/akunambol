#include "akonadi-dialog.h"
#include "ui_akonadi-dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}
