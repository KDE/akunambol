#ifndef SOURCEMANAGER_H
#define SOURCEMANAGER_H

class DMTClientConfig;

class SourceManager
{
    public:
        SourceManager();
        ~SourceManager();

    private:
        void initConfig();

        DMTClientConfig *m_conf;
};

#endif // SOURCEMANAGER_H
