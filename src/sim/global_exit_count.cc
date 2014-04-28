#include "global_exit_count.hh"
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
