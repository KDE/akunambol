
#ifndef SOURCEMANAGER_H
#define SOURCEMANAGER_H
#include <QObject>

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

    public slots:
        void sync();

    private:
        void initConfig();

        KFunSyncConfig *m_conf;
};

#endif // SOURCEMANAGER_H
