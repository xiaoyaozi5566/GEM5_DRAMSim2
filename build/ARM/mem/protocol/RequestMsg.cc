/** \file RequestMsg.cc
 *
 * Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/Type.py:402
 */

#include <iostream>

#include "mem/protocol/RequestMsg.hh"

using namespace std;
/** \brief Print the state of this object */
void
RequestMsg::print(ostream& out) const
{
    out << "[RequestMsg: ";
    out << "Address = " << m_Address << " ";
    out << "Type = " << m_Type << " ";
    out << "Requestor = " << m_Requestor << " ";
    out << "Destination = " << m_Destination << " ";
    out << "DataBlk = " << m_DataBlk << " ";
    out << "MessageSize = " << m_MessageSize << " ";
    out << "Time = " << getTime() * g_system_ptr->getClock() << " ";
    out << "]";
}
