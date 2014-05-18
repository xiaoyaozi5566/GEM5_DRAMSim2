#include "mem/cache/tags/cacheset.hh"

class WPLRUCacheSet : public CacheSet{

    public:

        WPLRUCacheSet(){
        }

    /**
     * Find a block matching the tag in this set.
     * @param asid The address space ID.
     * @param tag The Tag to find.
     * @return Pointer to the block if found.
     */
    virtual CacheBlk* findBlk(Addr tag) const override;
    // virtual CacheBlk* findBlk(Addr tag, uint64_t tid) const;

    /**
     * Move the given block to the head of the list.
     * @param blk The block to move.
     */
    virtual void moveToHead(CacheBlk *blk) override{
        fprintf( stderr, "\x1B[32mCacheSet::moveToHead\x1B[0m\n");
        CacheSet::moveToHead( blk );
    }
    // virtual void moveToHead(CacheBlk *blk, uint64_t tid);

    /**
     * Move the given block to the tail of the list.
     * @param blk The block to move
     */
    virtual void moveToTail(CacheBlk *blk) override{
        fprintf( stderr, "\x1B[32mCacheSet::moveToTail\x1B[0m\n");
        CacheSet::moveToTail( blk );
    }
    // virtual void moveToTail(CacheBlk *blk, uint64_t tid);

};
