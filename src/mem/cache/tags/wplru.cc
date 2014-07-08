#include "mem/cache/tags/wplru.hh"
#include "base/intmath.hh"
#include "debug/CacheRepl.hh"
#include "mem/cache/tags/cacheset.hh"
#include "mem/cache/tags/lru.hh"
#include "mem/cache/base.hh"
#include "sim/core.hh"
#include "mem/cache/blk.hh"
#include <typeinfo>

WPLRU::WPLRU( unsigned _numSets,
        unsigned _blkSize,
        unsigned _assoc,
        unsigned _hit_latency,
        unsigned _num_tcs )
    : LRU(_numSets, _blkSize, _assoc, _hit_latency ),
      num_tcs( _num_tcs )
{
    init_sets();
}

CacheSet
WPLRU::get_set( int setnum, uint64_t tid, Addr addr ){
    CacheSet s = sets[tid][setnum];
#ifdef DEBUG_TP
    if( s.hasBlk(interesting) ){
        printf( "get_set on interesting @ %lu", curTick() );
        s.print();
    }
#endif
    return s;
}

int
WPLRU::assoc_of_tc( int tcid ){
    int a = assoc / num_tcs;
    if(tcid < (assoc%num_tcs)) a++;
    return a;
}

void
WPLRU::init_sets(){
    sets = new CacheSet*[num_tcs];
    for( int i=0; i< num_tcs; i++ ){ sets[i] = new CacheSet[numSets]; }

    numBlocks = numSets * assoc;
    blks = new BlkType[numBlocks];
    dataBlks = new uint8_t[numBlocks * blkSize];

    unsigned blkIndex = 0;
    for( unsigned tc=0; tc< num_tcs; tc++ ){
        for( unsigned i = 0; i< numSets; i++ ){
            int tc_assoc = assoc_of_tc(tc);
            sets[tc][i].assoc = tc_assoc;
            sets[tc][i].blks  = new BlkType*[tc_assoc];
            for( unsigned j = 0; j<tc_assoc; j++ ){
                BlkType *blk = &blks[blkIndex];
                blk->data = &dataBlks[blkSize*blkIndex];
                ++blkIndex;

                blk->status = 0;
                blk->tag = j;
                blk->whenReady = 0;
                blk->isTouched = false;
                blk->size = blkSize;
                blk->set = i;
                sets[tc][i].blks[j]=blk;
            }
        }
    }
}

