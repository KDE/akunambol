
#ifndef SOURCEMANAGER_H
#define SOURCEMANAGER_H
#include <QObject>

#include <spds/SyncReport.h>

#include <client/appsyncsource.h>

namespace Akonadi {
    class Item;
}

class KFunSyncConfig;
class QString;

class AkunambolListener;
class AkunambolItemListener;
class AkunambolSourceListener;

class SourceManager : public QObject
{
    Q_OBJECT

    friend class AkunambolListener;
    friend class AkunambolItemListener;
    friend class AkunambolSourceListener;

    public:
        SourceManager(QObject *parent = 0);
        ~SourceManager();

        void setData(QString username, QString password, QString url);

    public slots:
        void sync(AppSyncSource* appSource);

        void emitSourceEnded(AppSyncSource* appSource, SyncReport* report);

    signals:
        void sourceStarted(AppSyncSource* source);
        void addReceived(const char* key);
        void updReceived(const char* key);
        void delReceived(const char* key);
        void addSent(const char* key);
        void updSent(const char* key);
        void delSent(const char* key);
        void sourceEnded(AppSyncSource* source, SyncReport* report);
        void totalClientItems(int n);
        void totalServerItems(int n);

    private:
        void setListeners();
        void unsetListeners();

        void emitSourceStarted(AppSyncSource* appSource);
        void emitAddReceived(const char* key);
        void emitDelReceived(const char* key);
        void emitUpdReceived(const char* key);
        void emitAddSent(const char* key);
        void emitDelSent(const char* key);
        void emitUpdSent(const char* key);
        void emitTotalClientItems(int n);
        void emitTotalServerItems(int n);


    private:
        AkunambolListener *syncListener;
        AkunambolItemListener *itemListener;
        AkunambolSourceListener *sourceListener;
};

#endif // SOURCEMANAGER_H
