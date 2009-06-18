#include "akonadisource.h"

AkonadiSource::AkonadiSource(AbstractSyncSourceConfig *config)
    : CacheSyncSource("akonadi", config)
{
}
