#include "global_exit_count.hh"
#include <stddef.h>
ExitCounter* ExitCounter::instance = NULL;
int ExitCounter::get()
{
    if(!instance)
        instance = new ExitCounter;
    return instance->count;
}

int ExitCounter::dec()
{
    if(!instance)
        instance = new ExitCounter;
    return --(instance->count);
}
