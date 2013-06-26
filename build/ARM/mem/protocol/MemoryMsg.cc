/** \file MemoryMsg.cc
 *
 * Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/Type.py:402
 */

#include <iostream>

#include "mem/protocol/MemoryMsg.hh"

using namespace std;
/** \brief Print the state of this object */
void
MemoryMsg::print(ostream& out) const
{
    out << "[MemoryMsg: ";
    out << "Address = " << m_Address << " ";
    out << "Type = " << m_Type << " ";
    out << "Sender = " << m_Sender << " ";
    out << "OriginalRequestorMachId = " << m_OriginalRequestorMachId << " ";
    out << "DataBlk = " << m_DataBlk << " ";
    out << "MessageSize = " << m_MessageSize << " ";
    out << "Prefetch = " << m_Prefetch << " ";
    out << "ReadX = " << m_ReadX << " ";
    out << "Acks = " << m_Acks << " ";
    out << "Time = " << getTime() * g_system_ptr->getClock() << " ";
    out << "]";
}
