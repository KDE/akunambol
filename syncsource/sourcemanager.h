
#ifndef SOURCEMANAGER_H
#define SOURCEMANAGER_H
#include <QObject>

class DMTClientConfig;
class QString;

class SourceManager : public QObject
{
    Q_OBJECT
    public:
        SourceManager(QObject *parent = 0);
        ~SourceManager();

        DMTClientConfig* config();
        void setData(QString username, QString password, QString url);

    public slots:
        void sync();

    private:
        void initConfig();

        DMTClientConfig *m_conf;
};

#endif // SOURCEMANAGER_H
