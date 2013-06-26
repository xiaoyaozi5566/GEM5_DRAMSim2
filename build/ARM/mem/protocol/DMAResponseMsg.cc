/** \file DMAResponseMsg.cc
 *
 * Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/Type.py:402
 */

#include <iostream>

#include "mem/protocol/DMAResponseMsg.hh"

using namespace std;
/** \brief Print the state of this object */
void
DMAResponseMsg::print(ostream& out) const
{
    out << "[DMAResponseMsg: ";
    out << "Type = " << m_Type << " ";
    out << "PhysicalAddress = " << m_PhysicalAddress << " ";
    out << "LineAddress = " << m_LineAddress << " ";
    out << "Destination = " << m_Destination << " ";
    out << "DataBlk = " << m_DataBlk << " ";
    out << "MessageSize = " << m_MessageSize << " ";
    out << "Time = " << getTime() * g_system_ptr->getClock() << " ";
    out << "]";
}
