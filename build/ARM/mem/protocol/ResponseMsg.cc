/** \file ResponseMsg.cc
 *
 * Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/Type.py:402
 */

#include <iostream>

#include "mem/protocol/ResponseMsg.hh"

using namespace std;
/** \brief Print the state of this object */
void
ResponseMsg::print(ostream& out) const
{
    out << "[ResponseMsg: ";
    out << "Address = " << m_Address << " ";
    out << "Type = " << m_Type << " ";
    out << "Sender = " << m_Sender << " ";
    out << "Destination = " << m_Destination << " ";
    out << "DataBlk = " << m_DataBlk << " ";
    out << "Dirty = " << m_Dirty << " ";
    out << "MessageSize = " << m_MessageSize << " ";
    out << "Time = " << getTime() * g_system_ptr->getClock() << " ";
    out << "]";
}
