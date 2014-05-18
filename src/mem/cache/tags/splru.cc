#include "mem/cache/tags/splru.hh"
#include "base/intmath.hh"
#include "debug/CacheRepl.hh"
#include "mem/cache/tags/cacheset.hh"
#include "mem/cache/tags/lru.hh"
#include "mem/cache/base.hh"
#include "sim/core.hh"
#include <typeinfo>


SPLRU::SPLRU( unsigned _numSets,
        unsigned _blkSize,
        unsigned _assoc,
        unsigned _hit_latency,
        unsigned num_tcs )
    : LRU(_numSets/num_tcs, _blkSize, _assoc, _hit_latency ),
    num_tcs(num_tcs)
{
    init_sets();
    assert( ( numSets % num_tcs ) == 0 );
    warmupBound = numSets * num_tcs * assoc;
}


int
tid_from_addr( Addr addr ){
    if( addr < 2 * pow( 1024, 3 ) ) return 0;
    else return 1;
}

CacheSet
SPLRU::get_set( int setnum, uint64_t tid, Addr addr ){
    // assert( &sets[setnum][tid_from_addr(addr)] != 0);
    // return sets[setnum][tid_from_addr(addr)];
    return LRU::get_set( setnum, tid, addr );
}

void
SPLRU::init_sets(){
    sets = new CacheSet*[numSets];
    for( int i=0; i< numSets; ++i ) sets[i] = new CacheSet[num_tcs];
    blks = new BlkType[numSets * num_tcs * assoc];
    // allocate data storage in one big chunk
    numBlocks = numSets * num_tcs * assoc;
    dataBlks = new uint8_t[numBlocks * blkSize];

    unsigned blkIndex = 0;       // index into blks array
    for (unsigned i = 0; i < numSets; ++i) {
        for( unsigned k=0; k < num_tcs; k++ ){
            sets[i][k].assoc = assoc;

            sets[i][k].blks = new BlkType*[assoc];

            // link in the data blocks
            for (unsigned j = 0; j < assoc; ++j) {
                // locate next cache block
                BlkType *blk = &blks[blkIndex];
                blk->data = &dataBlks[blkSize*blkIndex];
                ++blkIndex;

                // invalidate new cache block
                blk->status = 0;

                //EGH Fix Me : do we need to initialize blk?

                // Setting the tag to j is just to prevent long chains in the hash
                // table; won't matter because the block is invalid
                blk->tag = j;
                blk->whenReady = 0;
                blk->isTouched = false;
                blk->size = blkSize;
                sets[i][k].blks[j]=blk;
                blk->set = i;
            }
        }
    }
}

void
SPLRU::cleanupRefs()
{
    for (unsigned i = 0; i < numSets*num_tcs*assoc; ++i) {
        if (blks[i].isValid()) {
            totalRefs += blks[i].refCount;
            ++sampledRefs;
        }
    }
}
