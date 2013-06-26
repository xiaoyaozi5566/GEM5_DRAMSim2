/** \file MaskPredictorTraining.hh
 *
 * Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/Type.py:442
 */

#ifndef __MaskPredictorTraining_HH__
#define __MaskPredictorTraining_HH__

#include <iostream>
#include <string>


// Class definition
/** \enum MaskPredictorTraining
 *  \brief ...
 */
enum MaskPredictorTraining {
    MaskPredictorTraining_FIRST,
    MaskPredictorTraining_Undefined = MaskPredictorTraining_FIRST, /**< Undefined */
    MaskPredictorTraining_None, /**< None */
    MaskPredictorTraining_Implicit, /**< Implicit */
    MaskPredictorTraining_Explicit, /**< Explicit */
    MaskPredictorTraining_Both, /**< Both */
    MaskPredictorTraining_NUM
};

// Code to convert from a string to the enumeration
MaskPredictorTraining string_to_MaskPredictorTraining(const std::string& str);

// Code to convert state to a string
std::string MaskPredictorTraining_to_string(const MaskPredictorTraining& obj);

// Code to increment an enumeration type
MaskPredictorTraining &operator++(MaskPredictorTraining &e);
std::ostream& operator<<(std::ostream& out, const MaskPredictorTraining& obj);

#endif // __MaskPredictorTraining_HH__
