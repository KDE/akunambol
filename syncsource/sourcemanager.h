


#ifndef SOURCEMANAGER_H
#define SOURCEMANAGER_H

class DMTClientConfig;
class QString;

class SourceManager
{
    public:
        SourceManager();
        ~SourceManager();

        DMTClientConfig* config();
        void setData(QString username, QString password, QString url);

    private:
        void initConfig();

        DMTClientConfig *m_conf;
};

#endif // SOURCEMANAGER_H
