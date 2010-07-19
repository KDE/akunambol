
#ifndef SOURCEMANAGER_H
#define SOURCEMANAGER_H
#include <QObject>

namespace Akonadi {
    class Item;
}

class SyncConfig;
class QString;

class SourceManager : public QObject
{
    Q_OBJECT
    public:
        SourceManager(QObject *parent = 0);
        ~SourceManager();

        SyncConfig* config();
        void setData(QString username, QString password, QString url);
	void setAkonadiItems(QList<Akonadi::Item> items);
        void setCollectionId(qint64 id) { m_collectionId = id; }
	
    public slots:
        void sync();

    private:
        void initConfig();

        SyncConfig *m_conf;
        QList< Akonadi::Item > m_items;
        qint64 m_collectionId;
};

#endif // SOURCEMANAGER_H
