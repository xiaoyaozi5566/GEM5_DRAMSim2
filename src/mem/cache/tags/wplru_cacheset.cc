#include "mem/cache/tags/wplru_cacheset.hh"

CacheBlk*
WPLRUCacheSet::findBlk( Addr tag ) const{
    fprintf( stderr, "\x1B[32mCacheSet::findBlk\x1B[0m\n");
    return CacheSet::findBlk( tag );
}

