
#ifndef AKONADISOURCE_H
#define AKONADISOURCE_H

#include <client/CacheSyncSource.h>

class AkonadiSource : public CacheSyncSource
{
public:
    AkonadiSource(AbstractSyncSourceConfig *config);
};

#endif // AKONADISOURCE_H
