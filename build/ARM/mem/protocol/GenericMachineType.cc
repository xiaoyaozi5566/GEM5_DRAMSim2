/** \file GenericMachineType.hh
 *
 * Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/Type.py:554
 */

#include <cassert>
#include <iostream>
#include <string>

#include "base/misc.hh"
#include "mem/protocol/GenericMachineType.hh"

using namespace std;

// Code for output operator
ostream&
operator<<(ostream& out, const GenericMachineType& obj)
{
    out << GenericMachineType_to_string(obj);
    out << flush;
    return out;
}

// Code to convert state to a string
string
GenericMachineType_to_string(const GenericMachineType& obj)
{
    switch(obj) {
      case GenericMachineType_L1Cache:
        return "L1Cache";
      case GenericMachineType_L2Cache:
        return "L2Cache";
      case GenericMachineType_L3Cache:
        return "L3Cache";
      case GenericMachineType_Directory:
        return "Directory";
      case GenericMachineType_DMA:
        return "DMA";
      case GenericMachineType_Collector:
        return "Collector";
      case GenericMachineType_L1Cache_wCC:
        return "L1Cache_wCC";
      case GenericMachineType_L2Cache_wCC:
        return "L2Cache_wCC";
      case GenericMachineType_NULL:
        return "NULL";
      default:
        panic("Invalid range for type GenericMachineType");
    }
}

// Code to convert from a string to the enumeration
GenericMachineType
string_to_GenericMachineType(const string& str)
{
    if (str == "L1Cache") {
        return GenericMachineType_L1Cache;
    } else if (str == "L2Cache") {
        return GenericMachineType_L2Cache;
    } else if (str == "L3Cache") {
        return GenericMachineType_L3Cache;
    } else if (str == "Directory") {
        return GenericMachineType_Directory;
    } else if (str == "DMA") {
        return GenericMachineType_DMA;
    } else if (str == "Collector") {
        return GenericMachineType_Collector;
    } else if (str == "L1Cache_wCC") {
        return GenericMachineType_L1Cache_wCC;
    } else if (str == "L2Cache_wCC") {
        return GenericMachineType_L2Cache_wCC;
    } else if (str == "NULL") {
        return GenericMachineType_NULL;
    } else {
        panic("Invalid string conversion for %s, type GenericMachineType", str);
    }
}

// Code to increment an enumeration type
GenericMachineType&
operator++(GenericMachineType& e)
{
    assert(e < GenericMachineType_NUM);
    return e = GenericMachineType(e+1);
}
