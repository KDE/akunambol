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

class ContactsSyncer : public QObject
{
    Q_OBJECT

    public:
        ContactsSyncer(SourceManager *s, QObject *parent = 0);
        ~ContactsSyncer();
        
    signals:
        void noCollections();
        void finishedSync();
        
    private slots:
        void init();
        void prepareSyncFor(int id);
        void startSync();

    private:
        void populateTable();
        Contacts *c;
        Ui::Dialog *ui;
	SourceManager *m_sourceManager;
};

#endif // DIALOG_H
