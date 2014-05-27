#include <stddef.h>
#include "sim/system.hh"
class ExitCounter{
    public:
        static int get();
        static int dec();
    private:
        ExitCounter(){ 
            count=0;
            if( System::is_fast_forward )
                count = 1;
        };
        static ExitCounter * instance;
        int count;

};
