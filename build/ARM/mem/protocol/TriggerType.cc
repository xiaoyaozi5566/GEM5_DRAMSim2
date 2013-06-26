/** \file TriggerType.hh
 *
 * Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/Type.py:554
 */

#include <cassert>
#include <iostream>
#include <string>

#include "base/misc.hh"
#include "mem/protocol/TriggerType.hh"

using namespace std;

// Code for output operator
ostream&
operator<<(ostream& out, const TriggerType& obj)
{
    out << TriggerType_to_string(obj);
    out << flush;
    return out;
}

// Code to convert state to a string
string
TriggerType_to_string(const TriggerType& obj)
{
    switch(obj) {
      case TriggerType_ALL_ACKS:
        return "ALL_ACKS";
      default:
        panic("Invalid range for type TriggerType");
    }
}

// Code to convert from a string to the enumeration
TriggerType
string_to_TriggerType(const string& str)
{
    if (str == "ALL_ACKS") {
        return TriggerType_ALL_ACKS;
    } else {
        panic("Invalid string conversion for %s, type TriggerType", str);
    }
}

// Code to increment an enumeration type
TriggerType&
operator++(TriggerType& e)
{
    assert(e < TriggerType_NUM);
    return e = TriggerType(e+1);
}
