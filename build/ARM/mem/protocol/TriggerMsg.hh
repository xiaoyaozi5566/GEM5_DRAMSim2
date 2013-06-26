/** \file TriggerMsg.hh
 *
 *
 * Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/Type.py:214
 */

#ifndef __TriggerMsg_HH__
#define __TriggerMsg_HH__

#include <iostream>

#include "mem/ruby/common/Global.hh"
#include "mem/protocol/Address.hh"
#include "mem/protocol/TriggerType.hh"
#include "mem/protocol/Message.hh"
class TriggerMsg :  public Message
{
  public:
    TriggerMsg()
    {
        // m_Address has no default
        m_Type = TriggerType_NUM; // default value of TriggerType
    }
    TriggerMsg(const TriggerMsg&other)
        : Message(other)
    {
        m_Address = other.m_Address;
        m_Type = other.m_Type;
    }
    TriggerMsg(const Address& local_Address, const TriggerType& local_Type)
        : Message()
    {
        m_Address = local_Address;
        m_Type = local_Type;
    }
    static TriggerMsg*
    create()
    {
        return new TriggerMsg();
    }

    TriggerMsg*
    clone() const
    {
         return new TriggerMsg(*this);
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
    const TriggerType&
    getType() const
    {
        return m_Type;
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
    TriggerType&
    getType()
    {
        return m_Type;
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
    setType(const TriggerType& local_Type)
    {
        m_Type = local_Type;
    }
    void print(std::ostream& out) const;
  //private:
    /** Physical address for this request */
    Address m_Address;
    /** Type of trigger */
    TriggerType m_Type;
};
inline std::ostream&
operator<<(std::ostream& out, const TriggerMsg& obj)
{
    obj.print(out);
    out << std::flush;
    return out;
}

#endif // __TriggerMsg_HH__
