// Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/StateMachine.py:1442
// L1Cache: MI Example L1 Cache

#ifndef __L1Cache_PROFILER_HH__
#define __L1Cache_PROFILER_HH__

#include <cassert>
#include <iostream>

#include "mem/protocol/L1Cache_Event.hh"
#include "mem/protocol/L1Cache_State.hh"
#include "mem/ruby/common/TypeDefines.hh"

class L1Cache_Profiler
{
  public:
    L1Cache_Profiler();
    void setVersion(int version);
    void countTransition(L1Cache_State state, L1Cache_Event event);
    void possibleTransition(L1Cache_State state, L1Cache_Event event);
    uint64 getEventCount(L1Cache_Event event);
    bool isPossible(L1Cache_State state, L1Cache_Event event);
    uint64 getTransitionCount(L1Cache_State state, L1Cache_Event event);
    void clearStats();

  private:
    int m_counters[L1Cache_State_NUM][L1Cache_Event_NUM];
    int m_event_counters[L1Cache_Event_NUM];
    bool m_possible[L1Cache_State_NUM][L1Cache_Event_NUM];
    int m_version;
};

#endif // __L1Cache_PROFILER_HH__
