#ifndef DIALOG_H
#define DIALOG_H

#include <QtGui/QDialog>

namespace Ui
{
    class Dialog;
}

class SourceManager;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void sync();

private:
    Ui::Dialog *ui;
    SourceManager *s;
};

#endif // DIALOG_H
