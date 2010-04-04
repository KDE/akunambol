#ifndef DIALOG_H
#define DIALOG_H

#include <QtGui/QDialog>

class SourceManager;
class KJob;
class Contacts;

namespace Ui
{
    class Dialog;
}

namespace Akonadi {
    class Item;
    class Monitor;
}

class Dialog : public QDialog
{
    Q_OBJECT

    public:
        Dialog(SourceManager *s, QWidget *parent = 0);
        ~Dialog();
        
    signals:
        void noCollections();
        
    private slots:
        void init();
        void loadContactsFor(int id);
        void displayContacts();
        void startSync();

    private:
        void populateTable();
        Contacts *c;
        Ui::Dialog *ui;
	SourceManager *m_sourceManager;
};

#endif // DIALOG_H
