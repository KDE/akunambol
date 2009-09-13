#include "akonadisource.h"

#include <Akonadi/Item>
#include <QDebug>

USE_NAMESPACE

AkonadiSource::AkonadiSource(AbstractSyncSourceConfig *config)
    : CacheSyncSource("akonadi", config)
{
}

int AkonadiSource::insertItem(SyncItem &item)
{    
    if (!report->checkState()) {
	return STC_COMMAND_FAILED;
    }

    int ret = STC_COMMAND_FAILED;
    Akonadi::Item *akItem = 0;
    
    // Get data content.
    char* charData = (char*)item.getData();
    if (!charData) {
	qDebug() << "No data content";
	return STC_COMMAND_FAILED;
    }
    
    QString data = charData;


    // TODO Check if is an accepted mime type
    // JUst spit it out for the moment
    qDebug() << item.getDataType();
}
