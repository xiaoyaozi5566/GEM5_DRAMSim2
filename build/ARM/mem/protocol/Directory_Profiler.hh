// Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/StateMachine.py:1442
// Directory: Directory protocol

#ifndef __Directory_PROFILER_HH__
#define __Directory_PROFILER_HH__

#include <cassert>
#include <iostream>

#include "mem/protocol/Directory_Event.hh"
#include "mem/protocol/Directory_State.hh"
#include "mem/ruby/common/TypeDefines.hh"

class Directory_Profiler
{
  public:
    Directory_Profiler();
    void setVersion(int version);
    void countTransition(Directory_State state, Directory_Event event);
    void possibleTransition(Directory_State state, Directory_Event event);
    uint64 getEventCount(Directory_Event event);
    bool isPossible(Directory_State state, Directory_Event event);
    uint64 getTransitionCount(Directory_State state, Directory_Event event);
    void clearStats();

  private:
    int m_counters[Directory_State_NUM][Directory_Event_NUM];
    int m_event_counters[Directory_Event_NUM];
    bool m_possible[Directory_State_NUM][Directory_Event_NUM];
    int m_version;
};

#endif // __Directory_PROFILER_HH__
