
#ifndef AKONADISOURCE_H
#define AKONADISOURCE_H

#include <client/CacheSyncSource.h>

class AkonadiSource : public CacheSyncSource
{
public:
    AkonadiSource(AbstractSyncSourceConfig *config);
    
    /**
    * Get the content of an item given the key. It is used to populate
    * the SyncItem before the engine uses it in the usual flow of the sync.      
    *
    * @param key      the local key of the item
    * @param size     OUT: the size of the content
    */
    virtual void* getItemContent(StringBuffer& key, size_t* size) = 0;
    
            
    /**
     * Returns an Enumeration containing the StringBuffer keys of all items. 
     *
     * It is used both for the full sync, where all items are sent to the server,
     * and for the fast sync to calculate the modification since the last 
     * successful sync.
     * 
     * @return a newly allocated Enumeration that is free'd by the CacheSyncSource
     *         CacheSyncSource. 
     *         Return NULL in case of error, an empty Enumeration
     *         if there are no items.     
     */
    virtual Enumeration* getAllItemList() = 0;      
    
    /**
     * Called by the sync engine to add an item that the server has sent.
     * The sync source is expected to add it to its database, then set the
     * key to the local key assigned to the new item. Alternatively
     * the sync source can match the new item against one of the existing
     * items and return that key.
     *
     * @param item  the item as sent by the server
     * @return      SyncML status code 
     */
    virtual int insertItem(SyncItem& item) = 0;

    /**
     * Called by the sync engine to update an item that the source already
     * should have. The item's key is the local key of that item.
     *
     * @param item  the item as sent by the server
     * @return      SyncML status code
     */
    virtual int modifyItem(SyncItem& item) = 0;

    /**
     * Called by the sync engine to update an item that the source already
     * should have. The item's key is the local key of that item, no data is
     * provided.
     *
     * @param item  the item as sent by the server
     * @return      SyncML status code
     */
    virtual int removeItem(SyncItem& item) = 0;
};

#endif // AKONADISOURCE_H
