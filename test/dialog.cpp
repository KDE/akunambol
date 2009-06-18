#include "dialog.h"
#include "ui_dialog.h"

#include "../syncsource/sourcemanager.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);
    SourceManager s;

}

Dialog::~Dialog()
{
    delete ui;
}
