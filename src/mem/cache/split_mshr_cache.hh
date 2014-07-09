#include "mem/cache/base.hh"
#include "mem/cache/cache.hh"
#include "params/BaseCache.hh"
#include "stdio.h"

typedef BaseCacheParams Params;
template <class TagStore>
class SplitMSHRCache : public Cache<TagStore>
{
    typedef typename Cache<TagStore>::MemSidePacketQueue MemSidePacketQueue;
  private:
        MSHRQueue** mshrQueues;
        MSHRQueue** writeBuffers;
    int num_tcs;

    public:
    SplitMSHRCache( const Params *p, TagStore *tags );

    protected:
    class SplitMemSidePort : public Cache<TagStore>::MemSidePort{
        private:
        MasterPacketQueue** reqQueues;

        public:
        virtual void recvRetry( int threadID ){
            this->reqQueues[threadID]->retry();
        }

        virtual void requestBus(BaseCache::RequestCause cause, Tick time, int 
            threadID, bool isInteresting){
            DPRINTF(CachePort, "Asserting bus request for cause %d\n", cause);
            if( isInteresting ){
              printf( "interesting in split requestBus with time %llu\n", time );
              if( reqQueues[threadID]->sendEvent.scheduled() ){
                printf( "sendEvent was already scheduled at %llu\n", 
                    reqQueues[threadID]->sendEvent.when() );
              } else {
                printf("scheduled at time %llu\n", time);
              }
            }
            reqQueues[threadID]->schedSendEvent(time, isInteresting);
        }

        SplitMemSidePort(const std::string &_name, Cache<TagStore> *_cache,
                    const std::string &_label) :
          Cache<TagStore>::MemSidePort( _name, _cache, _label ){
          this->reqQueues = new MasterPacketQueue*[_cache->params->num_tcs];
          for( int i=0; i < (_cache->params->num_tcs); i++){
            this->reqQueues[i] = new MemSidePacketQueue(
                          *_cache, *this, "MasterPacketQueue", i);
          }
        }
    };


    virtual MSHRQueue* getMSHRQueue( int threadID ){
        return mshrQueues[threadID];
    }
    virtual MSHRQueue* getWriteBuffer( int threadID ){
        return writeBuffers[threadID];
    }
	
  //virtual bool isSplitMSHR() {return true;}
	
  virtual int get_num_tcs() {return num_tcs;}
};
