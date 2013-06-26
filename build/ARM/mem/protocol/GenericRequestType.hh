/** \file GenericRequestType.hh
 *
 * Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/Type.py:442
 */

#ifndef __GenericRequestType_HH__
#define __GenericRequestType_HH__

#include <iostream>
#include <string>


// Class definition
/** \enum GenericRequestType
 *  \brief ...
 */
enum GenericRequestType {
    GenericRequestType_FIRST,
    GenericRequestType_GETS = GenericRequestType_FIRST, /**< gets request */
    GenericRequestType_GET_INSTR, /**< get instr request */
    GenericRequestType_GETX, /**< getx request */
    GenericRequestType_UPGRADE, /**< upgrade request */
    GenericRequestType_DOWNGRADE, /**< downgrade request */
    GenericRequestType_INV, /**< invalidate request */
    GenericRequestType_INV_S, /**< invalidate shared copy request */
    GenericRequestType_PUTS, /**< puts request */
    GenericRequestType_PUTO, /**< puto request */
    GenericRequestType_PUTX, /**< putx request */
    GenericRequestType_L2_PF, /**< L2 prefetch */
    GenericRequestType_LD, /**< Load */
    GenericRequestType_ST, /**< Store */
    GenericRequestType_ATOMIC, /**< Atomic Load/Store */
    GenericRequestType_IFETCH, /**< Instruction fetch */
    GenericRequestType_IO, /**< I/O */
    GenericRequestType_NACK, /**< Nack */
    GenericRequestType_REPLACEMENT, /**< Replacement */
    GenericRequestType_WB_ACK, /**< WriteBack ack */
    GenericRequestType_EXE_ACK, /**< Execlusive ack */
    GenericRequestType_COMMIT, /**< Commit version */
    GenericRequestType_LD_XACT, /**< Transactional Load */
    GenericRequestType_LDX_XACT, /**< Transactional Load-Intend-Modify */
    GenericRequestType_ST_XACT, /**< Transactional Store */
    GenericRequestType_BEGIN_XACT, /**< Begin Transaction */
    GenericRequestType_COMMIT_XACT, /**< Commit Transaction */
    GenericRequestType_ABORT_XACT, /**< Abort Transaction */
    GenericRequestType_DMA_READ, /**< DMA READ */
    GenericRequestType_DMA_WRITE, /**< DMA WRITE */
    GenericRequestType_NULL, /**< null request type */
    GenericRequestType_NUM
};

// Code to convert from a string to the enumeration
GenericRequestType string_to_GenericRequestType(const std::string& str);

// Code to convert state to a string
std::string GenericRequestType_to_string(const GenericRequestType& obj);

// Code to increment an enumeration type
GenericRequestType &operator++(GenericRequestType &e);
std::ostream& operator<<(std::ostream& out, const GenericRequestType& obj);

#endif // __GenericRequestType_HH__
