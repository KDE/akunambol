
#ifndef SOURCEMANAGER_H
#define SOURCEMANAGER_H
#include <QObject>

namespace Akonadi {
    class Item;
}

class KFunSyncConfig;
class QString;

class SourceManager : public QObject
{
    Q_OBJECT
    public:
        SourceManager(QObject *parent = 0);
        ~SourceManager();

        KFunSyncConfig* config();
        void setData(QString username, QString password, QString url);
	void setAkonadiItems(QList<Akonadi::Item> items);
	
    public slots:
        void sync();

    private:
        void initConfig();

        KFunSyncConfig *m_conf;
        QList< Akonadi::Item > m_items;
};

#endif // SOURCEMANAGER_H
