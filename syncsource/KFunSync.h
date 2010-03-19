class KFunSync {

    public:

    enum LogLevel {
        LOG_NONE,
        LOG_ERROR,
        LOG_INFO,
        LOG_DEBUG
    };

    public:

        void setUrl(const char* url);
        void setUsername(const char* userName);
        void setPassword(const char* password);

        void setLogLevel(LogLevel level);



}
