#ifndef CONTACTS_H
#define CONTACTS_H

#include <QObject>

class KJob;

namespace Akonadi {
    class Item;
    class Monitor;
    class Collection;
}

class Contacts : public QObject
{
    Q_OBJECT

    public:
        Contacts(QObject *parent = 0);
        ~Contacts();

        QList<Akonadi::Collection> collections();
        QList<Akonadi::Item> itemsFor(int id);
        
        void loadContactsFor(int id);
        
    private slots:
        void contactItemAdded(const Akonadi::Item & item);
        void fetchContactCollectionsDone(KJob* job);
        void fetchContactCollectionDone(KJob* job);
        
    signals:
        void ready();
        void loadedCollection(int);

    private:
        void initContacts();
        void fetchContactsCollections();
        Akonadi::Monitor *m_contactMonitor;
        QList<Akonadi::Collection> m_collections;
        QList<Akonadi::Item> m_items;
};

#endif
