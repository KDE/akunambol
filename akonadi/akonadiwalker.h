#ifndef AKONADIWALKER_H
#define AKONADIWALKER_H

#include <QObject>

#include <Akonadi/Item>
#include <Akonadi/Monitor>

#include <kmime/kmime_message.h>
#include <kabc/addressee.h>

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<KMime::Message> MessagePtr;

class KJob;

class AkonadiWalker : public QObject
{
        Q_OBJECT

    public:
        AkonadiWalker( QObject* parent, const QVariantList& args );
        ~AkonadiWalker();
        QStringList sources() const;

    protected:
        bool sourceRequestEvent(const QString &name);

    private slots:

        void stopMonitor(const QString &name);

        void fetchEmailCollectionDone(KJob* job); // done retrieving whole collection
        void fetchContactCollectionDone(KJob* job); // done retrieving a whole contact collection
        void fetchMicroBlogDone(KJob* job);

        void emailItemsReceived(const Akonadi::Item::List &items);

        void fetchEmailCollectionsDone(KJob* job); // got list of collections
        void fetchContactCollectionsDone(KJob* job);
        void fetchMicroBlogCollectionsDone(KJob* job);

        void emailItemAdded(const Akonadi::Item &item, const QString &collection = QString());
        void contactItemAdded(const Akonadi::Item & item);
        void microBlogItemAdded(const Akonadi::Item &item);

    private:
        void initEmailMonitor();
        void initContactMonitor();
        void initMicroBlogMonitor();
        // useful for debugging
        void printMessage(MessagePtr msg);
        void printContact(const QString &source, const KABC::Addressee &a);

        Akonadi::Monitor* m_emailMonitor;
        Akonadi::Monitor* m_contactMonitor;
        Akonadi::Monitor* m_microBlogMonitor;

        QHash<KJob*, QString> m_jobCollections;
};

#endif // AKONADIWALKER_H
