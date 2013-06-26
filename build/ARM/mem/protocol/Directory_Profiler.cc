// Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/StateMachine.py:1522
// Directory: Directory protocol

#include <cassert>

#include "mem/protocol/Directory_Profiler.hh"

Directory_Profiler::Directory_Profiler()
{
    for (int state = 0; state < Directory_State_NUM; state++) {
        for (int event = 0; event < Directory_Event_NUM; event++) {
            m_possible[state][event] = false;
            m_counters[state][event] = 0;
        }
    }
    for (int event = 0; event < Directory_Event_NUM; event++) {
        m_event_counters[event] = 0;
    }
}

void
Directory_Profiler::setVersion(int version)
{
    m_version = version;
}

void
Directory_Profiler::clearStats()
{
    for (int state = 0; state < Directory_State_NUM; state++) {
        for (int event = 0; event < Directory_Event_NUM; event++) {
            m_counters[state][event] = 0;
        }
    }

    for (int event = 0; event < Directory_Event_NUM; event++) {
        m_event_counters[event] = 0;
    }
}
void
Directory_Profiler::countTransition(Directory_State state, Directory_Event event)
{
    assert(m_possible[state][event]);
    m_counters[state][event]++;
    m_event_counters[event]++;
}
void
Directory_Profiler::possibleTransition(Directory_State state,
                                      Directory_Event event)
{
    m_possible[state][event] = true;
}

uint64
Directory_Profiler::getEventCount(Directory_Event event)
{
    return m_event_counters[event];
}

bool
Directory_Profiler::isPossible(Directory_State state, Directory_Event event)
{
    return m_possible[state][event];
}

uint64
Directory_Profiler::getTransitionCount(Directory_State state,
                                      Directory_Event event)
{
    return m_counters[state][event];
}

