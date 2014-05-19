#include "mem/cache/split_mshr_cache.hh"
#include "mem/cache/base.hh"
#include "params/BaseCache.hh"

template<class TagStore>
SplitMSHRCache<TagStore>::SplitMSHRCache( const Params *p, TagStore *tags )
    : Cache<TagStore>( p, tags )
 {
          mshrQueues = malloc( (p->num_tcs) * sizeof( MSHRQueue ) );
          writeBuffers = malloc( (p-> num_tcs) * sizeof( MSHRQueue ) );
          for( int i=0; i < (p->num_tcs); i++ ){
              mshrQueues[i] = new MSHRQueue( "MSHRs", p->mshrs,
                      4, 0 );
              writeBuffers[i] =new MSHRQueue("write buffer",
                  p->write_buffers, p->mshrs+1000,
                  1);
          }
}
