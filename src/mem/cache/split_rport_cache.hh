#include "mem/cache/base.hh"
#include "mem/cache/cache.hh"
#include "params/BaseCache.hh"
#include "stdio.h"

typedef BaseCacheParams Params;
template <class TagStore>
class SplitRPortCache : public SplitMSHRCache<TagStore>
{

    public:
    SplitRPortCache( const Params *p, TagStore *tags );

    protected:
    //virtual bool isSplitRPort() {return true;}
    class SRCpuSidePort : public SplitMSHRCache<TagStore>::CpuSidePort{
        SlavePacketQueue** respQueues;

        virtual void schedTimingResp(PacketPtr pkt, Tick when, int threadID)
        { 
            this->respQueues[threadID]->schedSendTiming(pkt, when);
        }

        virtual void recvRetry(int threadID) { 
            this->respQueues[threadID]->retry();
        }
        
        public:
        SRCpuSidePort(const std::string &_name, Cache<TagStore> *_cache,
                    const std::string &_label) :
        SplitMSHRCache<TagStore>::CpuSidePort( _name, _cache, _label ){

	        this->respQueues = new SlavePacketQueue*[_cache->params->num_tcs];
            for( int i=0; i < (_cache->params->num_tcs); i++ ){
                this->respQueues[i] = new SlavePacketQueue(*_cache, *this, "SlavePacketQueue", i);
            }
        }
    };

};
