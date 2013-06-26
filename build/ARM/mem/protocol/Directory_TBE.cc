/** \file Directory_TBE.cc
 *
 * Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/Type.py:402
 */

#include <iostream>

#include "mem/protocol/Directory_TBE.hh"

using namespace std;
/** \brief Print the state of this object */
void
Directory_TBE::print(ostream& out) const
{
    out << "[Directory_TBE: ";
    out << "PhysicalAddress = " << m_PhysicalAddress << " ";
    out << "TBEState = " << m_TBEState << " ";
    out << "DataBlk = " << m_DataBlk << " ";
    out << "Len = " << m_Len << " ";
    out << "DmaRequestor = " << m_DmaRequestor << " ";
    out << "]";
}
