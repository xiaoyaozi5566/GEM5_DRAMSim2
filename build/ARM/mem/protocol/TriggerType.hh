/** \file TriggerType.hh
 *
 * Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/Type.py:442
 */

#ifndef __TriggerType_HH__
#define __TriggerType_HH__

#include <iostream>
#include <string>


// Class definition
/** \enum TriggerType
 *  \brief ...
 */
enum TriggerType {
    TriggerType_FIRST,
    TriggerType_ALL_ACKS = TriggerType_FIRST, /**< See corresponding event */
    TriggerType_NUM
};

// Code to convert from a string to the enumeration
TriggerType string_to_TriggerType(const std::string& str);

// Code to convert state to a string
std::string TriggerType_to_string(const TriggerType& obj);

// Code to increment an enumeration type
TriggerType &operator++(TriggerType &e);
std::ostream& operator<<(std::ostream& out, const TriggerType& obj);

#endif // __TriggerType_HH__
