/** \file RequestMsg.hh
 *
 *
 * Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/Type.py:214
 */

#ifndef __RequestMsg_HH__
#define __RequestMsg_HH__

#include <iostream>

#include "mem/ruby/common/Global.hh"
#include "mem/protocol/Address.hh"
#include "mem/protocol/CoherenceRequestType.hh"
#include "mem/protocol/MachineID.hh"
#include "mem/protocol/NetDest.hh"
#include "mem/protocol/DataBlock.hh"
#include "mem/protocol/MessageSizeType.hh"
#include "mem/protocol/NetworkMessage.hh"
class RequestMsg :  public NetworkMessage
{
  public:
    RequestMsg()
    {
        // m_Address has no default
        m_Type = CoherenceRequestType_NUM; // default value of CoherenceRequestType
        // m_Requestor has no default
        // m_Destination has no default
        // m_DataBlk has no default
        m_MessageSize = MessageSizeType_Undefined; // default value of MessageSizeType
    }
    RequestMsg(const RequestMsg&other)
        : NetworkMessage(other)
    {
        m_Address = other.m_Address;
        m_Type = other.m_Type;
        m_Requestor = other.m_Requestor;
        m_Destination = other.m_Destination;
        m_DataBlk = other.m_DataBlk;
        m_MessageSize = other.m_MessageSize;
    }
    RequestMsg(const Address& local_Address, const CoherenceRequestType& local_Type, const MachineID& local_Requestor, const NetDest& local_Destination, const DataBlock& local_DataBlk, const MessageSizeType& local_MessageSize)
        : NetworkMessage()
    {
        m_Address = local_Address;
        m_Type = local_Type;
        m_Requestor = local_Requestor;
        m_Destination = local_Destination;
        m_DataBlk = local_DataBlk;
        m_MessageSize = local_MessageSize;
    }
    static RequestMsg*
    create()
    {
        return new RequestMsg();
    }

    RequestMsg*
    clone() const
    {
         return new RequestMsg(*this);
    }
    // Const accessors methods for each field
    /** \brief Const accessor method for Address field.
     *  \return Address field
     */
    const Address&
    getAddress() const
    {
        return m_Address;
    }
    /** \brief Const accessor method for Type field.
     *  \return Type field
     */
    const CoherenceRequestType&
    getType() const
    {
        return m_Type;
    }
    /** \brief Const accessor method for Requestor field.
     *  \return Requestor field
     */
    const MachineID&
    getRequestor() const
    {
        return m_Requestor;
    }
    /** \brief Const accessor method for Destination field.
     *  \return Destination field
     */
    const NetDest&
    getDestination() const
    {
        return m_Destination;
    }
    /** \brief Const accessor method for DataBlk field.
     *  \return DataBlk field
     */
    const DataBlock&
    getDataBlk() const
    {
        return m_DataBlk;
    }
    /** \brief Const accessor method for MessageSize field.
     *  \return MessageSize field
     */
    const MessageSizeType&
    getMessageSize() const
    {
        return m_MessageSize;
    }
    // Non const Accessors methods for each field
    /** \brief Non-const accessor method for Address field.
     *  \return Address field
     */
    Address&
    getAddress()
    {
        return m_Address;
    }
    /** \brief Non-const accessor method for Type field.
     *  \return Type field
     */
    CoherenceRequestType&
    getType()
    {
        return m_Type;
    }
    /** \brief Non-const accessor method for Requestor field.
     *  \return Requestor field
     */
    MachineID&
    getRequestor()
    {
        return m_Requestor;
    }
    /** \brief Non-const accessor method for Destination field.
     *  \return Destination field
     */
    NetDest&
    getDestination()
    {
        return m_Destination;
    }
    /** \brief Non-const accessor method for DataBlk field.
     *  \return DataBlk field
     */
    DataBlock&
    getDataBlk()
    {
        return m_DataBlk;
    }
    /** \brief Non-const accessor method for MessageSize field.
     *  \return MessageSize field
     */
    MessageSizeType&
    getMessageSize()
    {
        return m_MessageSize;
    }
    // Mutator methods for each field
    /** \brief Mutator method for Address field */
    void
    setAddress(const Address& local_Address)
    {
        m_Address = local_Address;
    }
    /** \brief Mutator method for Type field */
    void
    setType(const CoherenceRequestType& local_Type)
    {
        m_Type = local_Type;
    }
    /** \brief Mutator method for Requestor field */
    void
    setRequestor(const MachineID& local_Requestor)
    {
        m_Requestor = local_Requestor;
    }
    /** \brief Mutator method for Destination field */
    void
    setDestination(const NetDest& local_Destination)
    {
        m_Destination = local_Destination;
    }
    /** \brief Mutator method for DataBlk field */
    void
    setDataBlk(const DataBlock& local_DataBlk)
    {
        m_DataBlk = local_DataBlk;
    }
    /** \brief Mutator method for MessageSize field */
    void
    setMessageSize(const MessageSizeType& local_MessageSize)
    {
        m_MessageSize = local_MessageSize;
    }
    void print(std::ostream& out) const;
  //private:
    /** Physical address for this request */
    Address m_Address;
    /** Type of request (GetS, GetX, PutX, etc) */
    CoherenceRequestType m_Type;
    /** Node who initiated the request */
    MachineID m_Requestor;
    /** Multicast destination mask */
    NetDest m_Destination;
    /** data for the cache line */
    DataBlock m_DataBlk;
    /** size category of the message */
    MessageSizeType m_MessageSize;
};
inline std::ostream&
operator<<(std::ostream& out, const RequestMsg& obj)
{
    obj.print(out);
    out << std::flush;
    return out;
}

#endif // __RequestMsg_HH__
