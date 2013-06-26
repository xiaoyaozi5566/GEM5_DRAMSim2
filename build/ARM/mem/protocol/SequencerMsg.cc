/** \file SequencerMsg.cc
 *
 * Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/Type.py:402
 */

#include <iostream>

#include "mem/protocol/SequencerMsg.hh"

using namespace std;
/** \brief Print the state of this object */
void
SequencerMsg::print(ostream& out) const
{
    out << "[SequencerMsg: ";
    out << "LineAddress = " << m_LineAddress << " ";
    out << "PhysicalAddress = " << m_PhysicalAddress << " ";
    out << "Type = " << m_Type << " ";
    out << "ProgramCounter = " << m_ProgramCounter << " ";
    out << "AccessMode = " << m_AccessMode << " ";
    out << "DataBlk = " << m_DataBlk << " ";
    out << "Len = " << m_Len << " ";
    out << "Prefetch = " << m_Prefetch << " ";
    out << "Time = " << getTime() * g_system_ptr->getClock() << " ";
    out << "]";
}
