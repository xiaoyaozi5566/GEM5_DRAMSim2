#include <list>

class TraceNode {
    uint64_t address;
    uint64_t time;

    public:

    std::string to_s(){
        std::ostringstream r;
        r << address << " @ " << time;
        return r.str();
    }
    
    TraceNode(uint64_t address, uint64_t time):
        address(address),
        time(time){}
};

class CacheTrace : public std::list<TraceNode>{
    public:
    std::string to_s(){
        std::string return_val = "";
        for( CacheTrace::iterator it=this->begin();
                it != this->end(); it++ ){
            return_val += it->to_s() + "\n";
        }
        return return_val;
    }
};
