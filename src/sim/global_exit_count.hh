#include <stddef.h>
#include "sim/system.hh"
class ExitCounter{
    public:
        static int get();
        static int dec();
    private:
        // If fastforwarding is turned of the count
        // needs to be manually changed to 0 here.
        ExitCounter(){ 
            count=0;
            if( System::is_fast_forward )
                count = 1;
        };
        static ExitCounter * instance;
        int count;

};
