#include <list>
#include <iomanip>
#include "stdio.h"
#include "mem/cache/base.hh"
#include "params/BaseCache.hh"
// #include <boost/archive/binary_oarchive.hpp>
// #include <boost/serialization/list.hpp>

class TraceNode {
    uint64_t address;
    uint64_t time;
    bool isRead;
    bool isReq;
    bool isResp;
    std::string op;

    // friend class boost::serialization::access;
    // template<class Archive>
    // void serialize( Archive & ar, const unsigned int version ){
    //     ar & address;
    //     ar & time;
    //     ar & isRead;
    //     ar & isReq;
    //     ar & isResp;
    //     ar & isResp;
    //     ar & op;
    // }

    public:

    std::string to_s(){
        std::ostringstream r;
        std::string rw = isRead ? " (R) " : " (W) ";
        std::string req = 
            (isReq && isResp) ? " [both] " :
            isReq  ? " [req ] " :
            isResp ? " [resp] " :
            " [none] ";
        r << std::hex << "0x" << std::setw(7) << address << std::dec
          << " @ " << std::setw(13) << time 
          << rw << req << op;
        return r.str();
    }
    
    TraceNode(PacketPtr ptr):
        TraceNode(ptr, "")
    {}

    TraceNode(PacketPtr ptr, std::string op):
        address(ptr->getAddr()),
        time(curTick()),
        isRead(ptr->isRead()),
        isReq(ptr->isRequest()),
        isResp(ptr->isResponse()),
        op(op)
    {}
};

class CacheTrace: public std::list<TraceNode*>{
    std::string outFile;
    typedef BaseCacheParams Params;
    const Params * params;

    public:

    CacheTrace( std::string outFile, const Params * params )
        :outFile(outFile),
        params(params)
    {}

    void add( PacketPtr pkt, std::string op ){
        if( params->do_cache_trace && pkt->threadID==0 ){
            push_back( new TraceNode( pkt, op ) );
        }
    }

    std::string to_s(){
        std::string return_val = "";
        for( CacheTrace::iterator it=this->begin();
                it != this->end(); it++ ){
            return_val += (*it)->to_s() + "\n";
        }
        return return_val;
    }
    void print(){
        if( params->do_cache_trace ){
            FILE * of = fopen( outFile.c_str() , "w" );
            fprintf( of, to_s().c_str() );
            fclose( of );
        } 
    }
};
