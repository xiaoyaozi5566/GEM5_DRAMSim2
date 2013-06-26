// Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/StateMachine.py:1522
// DMA: DMA Controller

#include <cassert>

#include "mem/protocol/DMA_Profiler.hh"

DMA_Profiler::DMA_Profiler()
{
    for (int state = 0; state < DMA_State_NUM; state++) {
        for (int event = 0; event < DMA_Event_NUM; event++) {
            m_possible[state][event] = false;
            m_counters[state][event] = 0;
        }
    }
    for (int event = 0; event < DMA_Event_NUM; event++) {
        m_event_counters[event] = 0;
    }
}

void
DMA_Profiler::setVersion(int version)
{
    m_version = version;
}

void
DMA_Profiler::clearStats()
{
    for (int state = 0; state < DMA_State_NUM; state++) {
        for (int event = 0; event < DMA_Event_NUM; event++) {
            m_counters[state][event] = 0;
        }
    }

    for (int event = 0; event < DMA_Event_NUM; event++) {
        m_event_counters[event] = 0;
    }
}
void
DMA_Profiler::countTransition(DMA_State state, DMA_Event event)
{
    assert(m_possible[state][event]);
    m_counters[state][event]++;
    m_event_counters[event]++;
}
void
DMA_Profiler::possibleTransition(DMA_State state,
                                      DMA_Event event)
{
    m_possible[state][event] = true;
}

uint64
DMA_Profiler::getEventCount(DMA_Event event)
{
    return m_event_counters[event];
}

bool
DMA_Profiler::isPossible(DMA_State state, DMA_Event event)
{
    return m_possible[state][event];
}

uint64
DMA_Profiler::getTransitionCount(DMA_State state,
                                      DMA_Event event)
{
    return m_counters[state][event];
}

