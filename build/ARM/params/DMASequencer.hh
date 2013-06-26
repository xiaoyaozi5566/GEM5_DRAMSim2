#ifndef __PARAMS__DMASequencer__
#define __PARAMS__DMASequencer__

class DMASequencer;


#include "params/RubyPort.hh"

struct DMASequencerParams
    : public RubyPortParams
{
    DMASequencer * create();
};

#endif // __PARAMS__DMASequencer__
