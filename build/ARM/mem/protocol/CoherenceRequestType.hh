/** \file CoherenceRequestType.hh
 *
 * Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/Type.py:442
 */

#ifndef __CoherenceRequestType_HH__
#define __CoherenceRequestType_HH__

#include <iostream>
#include <string>


// Class definition
/** \enum CoherenceRequestType
 *  \brief ...
 */
enum CoherenceRequestType {
    CoherenceRequestType_FIRST,
    CoherenceRequestType_GETX = CoherenceRequestType_FIRST, /**< Get eXclusive */
    CoherenceRequestType_GETS, /**< Get Shared */
    CoherenceRequestType_PUTX, /**< Put eXclusive */
    CoherenceRequestType_PUTO, /**< Put Owned */
    CoherenceRequestType_WB_ACK, /**< Writeback ack */
    CoherenceRequestType_WB_NACK, /**< Writeback neg. ack */
    CoherenceRequestType_INV, /**< Invalidation */
    CoherenceRequestType_FWD, /**< Generic FWD */
    CoherenceRequestType_NUM
};

// Code to convert from a string to the enumeration
CoherenceRequestType string_to_CoherenceRequestType(const std::string& str);

// Code to convert state to a string
std::string CoherenceRequestType_to_string(const CoherenceRequestType& obj);

// Code to increment an enumeration type
CoherenceRequestType &operator++(CoherenceRequestType &e);
std::ostream& operator<<(std::ostream& out, const CoherenceRequestType& obj);

#endif // __CoherenceRequestType_HH__
