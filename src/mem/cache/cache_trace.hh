#include <list>
#include "mem/cache/base.hh"

class TraceNode {
    uint64_t address;
    uint64_t time;
    bool isRead;
    bool isReq;
    bool isResp;

    public:

    std::string to_s(){
        std::ostringstream r;
        std::string rw = isRead ? " (R) " : " (W) ";
        std::string req = 
            (isReq && isResp) ? " [both] " :
            isReq  ? " [req ] " :
            isResp ? " [resp] " :
            " [none] ";
        r << address << " @ " << time << rw << req;
        return r.str();
    }
    
    TraceNode(PacketPtr ptr):
        address(ptr->getAddr()),
        time(curTick()),
        isRead(ptr->isRead()),
        isReq(ptr->isRequest()),
        isResp(ptr->isResponse())
    {}
};

class CacheTrace : public std::list<TraceNode*>{
    std::string outFile;
    public:

    CacheTrace( std::string outFile )
        :outFile(outFile)
    {}

    std::string to_s(){
        std::string return_val = "";
        for( CacheTrace::iterator it=this->begin();
                it != this->end(); it++ ){
            return_val += (*it)->to_s() + "\n";
        }
        return return_val;
    }
    void print(){
        // printf( to_s().c_str() );
        FILE * of = fopen( outFile.c_str() , "w" );
        fprintf( of, to_s().c_str() );
        fclose( of );
    }
};
