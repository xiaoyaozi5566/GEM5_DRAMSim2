/** \file GenericMachineType.hh
 *
 * Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/Type.py:442
 */

#ifndef __GenericMachineType_HH__
#define __GenericMachineType_HH__

#include <iostream>
#include <string>


// Class definition
/** \enum GenericMachineType
 *  \brief ...
 */
enum GenericMachineType {
    GenericMachineType_FIRST,
    GenericMachineType_L1Cache = GenericMachineType_FIRST, /**< L1 Cache Mach */
    GenericMachineType_L2Cache, /**< L2 Cache Mach */
    GenericMachineType_L3Cache, /**< L3 Cache Mach */
    GenericMachineType_Directory, /**< Directory Mach */
    GenericMachineType_DMA, /**< DMA Mach */
    GenericMachineType_Collector, /**< Collector Mach */
    GenericMachineType_L1Cache_wCC, /**< L1 Cache Mach with Cache Coherence (used for miss latency profile) */
    GenericMachineType_L2Cache_wCC, /**< L1 Cache Mach with Cache Coherence (used for miss latency profile) */
    GenericMachineType_NULL, /**< null mach type */
    GenericMachineType_NUM
};

// Code to convert from a string to the enumeration
GenericMachineType string_to_GenericMachineType(const std::string& str);

// Code to convert state to a string
std::string GenericMachineType_to_string(const GenericMachineType& obj);

// Code to increment an enumeration type
GenericMachineType &operator++(GenericMachineType &e);
std::ostream& operator<<(std::ostream& out, const GenericMachineType& obj);

#endif // __GenericMachineType_HH__
