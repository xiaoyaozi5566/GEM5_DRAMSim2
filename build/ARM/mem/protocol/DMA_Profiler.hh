// Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/StateMachine.py:1442
// DMA: DMA Controller

#ifndef __DMA_PROFILER_HH__
#define __DMA_PROFILER_HH__

#include <cassert>
#include <iostream>

#include "mem/protocol/DMA_Event.hh"
#include "mem/protocol/DMA_State.hh"
#include "mem/ruby/common/TypeDefines.hh"

class DMA_Profiler
{
  public:
    DMA_Profiler();
    void setVersion(int version);
    void countTransition(DMA_State state, DMA_Event event);
    void possibleTransition(DMA_State state, DMA_Event event);
    uint64 getEventCount(DMA_Event event);
    bool isPossible(DMA_State state, DMA_Event event);
    uint64 getTransitionCount(DMA_State state, DMA_Event event);
    void clearStats();

  private:
    int m_counters[DMA_State_NUM][DMA_Event_NUM];
    int m_event_counters[DMA_Event_NUM];
    bool m_possible[DMA_State_NUM][DMA_Event_NUM];
    int m_version;
};

#endif // __DMA_PROFILER_HH__
