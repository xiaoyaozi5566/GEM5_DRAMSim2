/** \file Directory_Entry.cc
 *
 * Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/Type.py:402
 */

#include <iostream>

#include "mem/protocol/Directory_Entry.hh"

using namespace std;
/** \brief Print the state of this object */
void
Directory_Entry::print(ostream& out) const
{
    out << "[Directory_Entry: ";
    out << "DirectoryState = " << m_DirectoryState << " ";
    out << "DataBlk = " << m_DataBlk << " ";
    out << "Sharers = " << m_Sharers << " ";
    out << "Owner = " << m_Owner << " ";
    out << "]";
}
