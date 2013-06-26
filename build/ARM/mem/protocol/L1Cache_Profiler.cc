// Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/StateMachine.py:1522
// L1Cache: MI Example L1 Cache

#include <cassert>

#include "mem/protocol/L1Cache_Profiler.hh"

L1Cache_Profiler::L1Cache_Profiler()
{
    for (int state = 0; state < L1Cache_State_NUM; state++) {
        for (int event = 0; event < L1Cache_Event_NUM; event++) {
            m_possible[state][event] = false;
            m_counters[state][event] = 0;
        }
    }
    for (int event = 0; event < L1Cache_Event_NUM; event++) {
        m_event_counters[event] = 0;
    }
}

void
L1Cache_Profiler::setVersion(int version)
{
    m_version = version;
}

void
L1Cache_Profiler::clearStats()
{
    for (int state = 0; state < L1Cache_State_NUM; state++) {
        for (int event = 0; event < L1Cache_Event_NUM; event++) {
            m_counters[state][event] = 0;
        }
    }

    for (int event = 0; event < L1Cache_Event_NUM; event++) {
        m_event_counters[event] = 0;
    }
}
void
L1Cache_Profiler::countTransition(L1Cache_State state, L1Cache_Event event)
{
    assert(m_possible[state][event]);
    m_counters[state][event]++;
    m_event_counters[event]++;
}
void
L1Cache_Profiler::possibleTransition(L1Cache_State state,
                                      L1Cache_Event event)
{
    m_possible[state][event] = true;
}

uint64
L1Cache_Profiler::getEventCount(L1Cache_Event event)
{
    return m_event_counters[event];
}

bool
L1Cache_Profiler::isPossible(L1Cache_State state, L1Cache_Event event)
{
    return m_possible[state][event];
}

uint64
L1Cache_Profiler::getTransitionCount(L1Cache_State state,
                                      L1Cache_Event event)
{
    return m_counters[state][event];
}

