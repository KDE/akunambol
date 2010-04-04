#include <QtGui/QApplication>
#include "akonadi-dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ContactsSyncer w;
    w.show();
    return a.exec();
}
