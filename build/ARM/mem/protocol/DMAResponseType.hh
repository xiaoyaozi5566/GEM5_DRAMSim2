/** \file DMAResponseType.hh
 *
 * Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/Type.py:442
 */

#ifndef __DMAResponseType_HH__
#define __DMAResponseType_HH__

#include <iostream>
#include <string>


// Class definition
/** \enum DMAResponseType
 *  \brief ...
 */
enum DMAResponseType {
    DMAResponseType_FIRST,
    DMAResponseType_DATA = DMAResponseType_FIRST, /**< DATA read */
    DMAResponseType_ACK, /**< ACK write */
    DMAResponseType_NULL, /**< Invalid */
    DMAResponseType_NUM
};

// Code to convert from a string to the enumeration
DMAResponseType string_to_DMAResponseType(const std::string& str);

// Code to convert state to a string
std::string DMAResponseType_to_string(const DMAResponseType& obj);

// Code to increment an enumeration type
DMAResponseType &operator++(DMAResponseType &e);
std::ostream& operator<<(std::ostream& out, const DMAResponseType& obj);

#endif // __DMAResponseType_HH__
