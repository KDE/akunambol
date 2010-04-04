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
        QList<Akonadi::Item> itemsForLoadedCollection();
        
        void loadContactsForCollection(int id);
        qint64 selectedCollection() { return m_selectedCollection; }
        
    private slots:
        void contactItemAdded(const Akonadi::Item & item);
        void fetchCollectionsDone(KJob* job);
        void fetchContactsDone(KJob* job);
        
    signals:
        void ready();
        void loadedCollection();

    private:
        void initContacts();
        void fetchCollections();
        Akonadi::Monitor *m_contactMonitor;
        QList<Akonadi::Collection> m_collections;
        QList<Akonadi::Item> m_items;
        qint64 m_selectedCollection;
};

#endif
