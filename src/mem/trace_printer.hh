#include "stdio.h"
#include <iomanip>
#include "params/MemObject.hh"
class TracePrinter {
    typedef MemObjectParams Params;
    const Params * params;
    FILE *of;

    public:
    TracePrinter( std::string outFile, const Params * params ) :
        params( params )
    {
        if ( params->save_trace ){
            of = fopen( outFile.c_str(), "w" );
        }
    }

    std::string packet_to_str( PacketPtr pkt ){
        std::ostringstream r;
        std::string rw = pkt->isRead() ? " (R) " : " (W) ";
        bool isReq  = pkt->isRequest();
        bool isResp = pkt->isResponse();
        std::string req =
            (isReq && isResp) ? " [both] " :
            isReq  ? " [req ] " :
            isResp ? " [resp] " :
            " [none] ";
        uint64_t address = pkt->getAddr();
        uint64_t time    = curTick();
        r << "0x" << std::hex << std::left << std::setw(9) << address 
          << std::dec << " @ " << std::setw(13) << time 
          << rw << req;
        return r.str();

    }

    void addTrace( PacketPtr pkt, std::string op ){
        assert( pkt );
        if( params->save_trace && pkt->threadID==0 ){
            std::string trace = packet_to_str( pkt ) + op +
                *( new std::string("\n") );
            fprintf( of, trace.c_str() );
        }
    }
    
};
