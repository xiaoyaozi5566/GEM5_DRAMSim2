/** \file Directory_Controller.cc
 *
 * Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/StateMachine.py:436
 * Created by slicc definition of Module "Directory protocol"
 */

#include <sys/types.h>
#include <unistd.h>

#include <cassert>
#include <sstream>
#include <string>

#include "base/compiler.hh"
#include "base/cprintf.hh"
#include "debug/RubyGenerated.hh"
#include "debug/RubySlicc.hh"
#include "mem/protocol/Directory_Controller.hh"
#include "mem/protocol/Directory_Event.hh"
#include "mem/protocol/Directory_State.hh"
#include "mem/protocol/Types.hh"
#include "mem/ruby/common/Global.hh"
#include "mem/ruby/slicc_interface/RubySlicc_includes.hh"
#include "mem/ruby/system/System.hh"

using namespace std;
#include "mem/protocol/MessageBuffer.hh"
#include "mem/protocol/TBETable.hh"
Directory_Controller *
Directory_ControllerParams::create()
{
    return new Directory_Controller(this);
}

int Directory_Controller::m_num_controllers = 0;
Directory_ProfileDumper Directory_Controller::s_profileDumper;

// for adding information to the protocol debug trace
stringstream Directory_transitionComment;
#define APPEND_TRANSITION_COMMENT(str) (Directory_transitionComment << str)

/** \brief constructor */
Directory_Controller::Directory_Controller(const Params *p)
    : AbstractController(p)
{
    m_version = p->version;
    m_transitions_per_cycle = p->transitions_per_cycle;
    m_buffer_size = p->buffer_size;
    m_recycle_latency = p->recycle_latency;
    m_number_of_TBEs = p->number_of_TBEs;
    m_is_blocking = false;
    m_name = "Directory";
    m_max_in_port_rank = 1;
    m_directory_ptr = p->directory;
    m_memBuffer_ptr = p->memBuffer;
    m_directory_latency = p->directory_latency;
    m_num_controllers++;
}

void
Directory_Controller::init()
{
    MachineType machine_type;
    int base;

    m_machineID.type = MachineType_Directory;
    m_machineID.num = m_version;

    // initialize objects
    m_profiler.setVersion(m_version);
    s_profileDumper.registerProfiler(&m_profiler);

    machine_type = string_to_MachineType("Directory");
    base = MachineType_base_number(machine_type);
    m_Directory_forwardFromDir_ptr = m_net_ptr->getToNetQueue(m_version + base, false, 3, "forward");
    assert(m_Directory_forwardFromDir_ptr != NULL);
    m_Directory_forwardFromDir_ptr->setOrdering(false);
    if (m_buffer_size > 0) {
        m_Directory_forwardFromDir_ptr->resize(m_buffer_size);
    }
    m_Directory_forwardFromDir_ptr->setDescription("[Version " + to_string(m_version) + ", Directory, name=Directory_forwardFromDir]");

    m_Directory_forwardFromDir_ptr->setRecycleLatency(m_recycle_latency);
    machine_type = string_to_MachineType("Directory");
    base = MachineType_base_number(machine_type);
    m_Directory_responseFromDir_ptr = m_net_ptr->getToNetQueue(m_version + base, false, 4, "response");
    assert(m_Directory_responseFromDir_ptr != NULL);
    m_Directory_responseFromDir_ptr->setOrdering(false);
    if (m_buffer_size > 0) {
        m_Directory_responseFromDir_ptr->resize(m_buffer_size);
    }
    m_Directory_responseFromDir_ptr->setDescription("[Version " + to_string(m_version) + ", Directory, name=Directory_responseFromDir]");

    m_Directory_responseFromDir_ptr->setRecycleLatency(m_recycle_latency);
    machine_type = string_to_MachineType("Directory");
    base = MachineType_base_number(machine_type);
    m_Directory_dmaResponseFromDir_ptr = m_net_ptr->getToNetQueue(m_version + base, true, 1, "response");
    assert(m_Directory_dmaResponseFromDir_ptr != NULL);
    m_Directory_dmaResponseFromDir_ptr->setOrdering(true);
    if (m_buffer_size > 0) {
        m_Directory_dmaResponseFromDir_ptr->resize(m_buffer_size);
    }
    m_Directory_dmaResponseFromDir_ptr->setDescription("[Version " + to_string(m_version) + ", Directory, name=Directory_dmaResponseFromDir]");

    m_Directory_dmaResponseFromDir_ptr->setRecycleLatency(m_recycle_latency);
    machine_type = string_to_MachineType("Directory");
    base = MachineType_base_number(machine_type);
    m_Directory_requestToDir_ptr = m_net_ptr->getFromNetQueue(m_version + base, true, 2, "request");
    assert(m_Directory_requestToDir_ptr != NULL);
    m_Directory_requestToDir_ptr->setOrdering(true);
    if (m_buffer_size > 0) {
        m_Directory_requestToDir_ptr->resize(m_buffer_size);
    }
    m_Directory_requestToDir_ptr->setDescription("[Version " + to_string(m_version) + ", Directory, name=Directory_requestToDir]");

    m_Directory_requestToDir_ptr->setRecycleLatency(m_recycle_latency);
    machine_type = string_to_MachineType("Directory");
    base = MachineType_base_number(machine_type);
    m_Directory_dmaRequestToDir_ptr = m_net_ptr->getFromNetQueue(m_version + base, true, 0, "request");
    assert(m_Directory_dmaRequestToDir_ptr != NULL);
    m_Directory_dmaRequestToDir_ptr->setOrdering(true);
    if (m_buffer_size > 0) {
        m_Directory_dmaRequestToDir_ptr->resize(m_buffer_size);
    }
    m_Directory_dmaRequestToDir_ptr->setDescription("[Version " + to_string(m_version) + ", Directory, name=Directory_dmaRequestToDir]");

    m_Directory_dmaRequestToDir_ptr->setRecycleLatency(m_recycle_latency);
    m_Directory_TBEs_ptr  = new TBETable<Directory_TBE>(m_number_of_TBEs);
    assert(m_Directory_TBEs_ptr != NULL);

    (*m_Directory_dmaRequestToDir_ptr).setConsumer(this);
    (*m_Directory_requestToDir_ptr).setConsumer(this);
    (*m_memBuffer_ptr).setConsumer(this);

    (*m_Directory_dmaRequestToDir_ptr).setDescription("[Version " + to_string(m_version) + ", Directory, dmaRequestQueue_in]");
    (*m_Directory_requestToDir_ptr).setDescription("[Version " + to_string(m_version) + ", Directory, requestQueue_in]");
    (*m_memBuffer_ptr).setDescription("[Version " + to_string(m_version) + ", Directory, memQueue_in]");

    m_profiler.possibleTransition(Directory_State_M_DRD, Directory_Event_GETX);
    m_profiler.possibleTransition(Directory_State_M_DWR, Directory_Event_GETX);
    m_profiler.possibleTransition(Directory_State_M_DWRI, Directory_Event_GETX);
    m_profiler.possibleTransition(Directory_State_M_DRDI, Directory_Event_GETX);
    m_profiler.possibleTransition(Directory_State_IM, Directory_Event_GETX);
    m_profiler.possibleTransition(Directory_State_IM, Directory_Event_GETS);
    m_profiler.possibleTransition(Directory_State_IM, Directory_Event_PUTX);
    m_profiler.possibleTransition(Directory_State_IM, Directory_Event_PUTX_NotOwner);
    m_profiler.possibleTransition(Directory_State_MI, Directory_Event_GETX);
    m_profiler.possibleTransition(Directory_State_MI, Directory_Event_GETS);
    m_profiler.possibleTransition(Directory_State_MI, Directory_Event_PUTX);
    m_profiler.possibleTransition(Directory_State_MI, Directory_Event_PUTX_NotOwner);
    m_profiler.possibleTransition(Directory_State_ID, Directory_Event_GETX);
    m_profiler.possibleTransition(Directory_State_ID, Directory_Event_GETS);
    m_profiler.possibleTransition(Directory_State_ID, Directory_Event_PUTX);
    m_profiler.possibleTransition(Directory_State_ID, Directory_Event_PUTX_NotOwner);
    m_profiler.possibleTransition(Directory_State_ID_W, Directory_Event_GETX);
    m_profiler.possibleTransition(Directory_State_ID_W, Directory_Event_GETS);
    m_profiler.possibleTransition(Directory_State_ID_W, Directory_Event_PUTX);
    m_profiler.possibleTransition(Directory_State_ID_W, Directory_Event_PUTX_NotOwner);
    m_profiler.possibleTransition(Directory_State_IM, Directory_Event_DMA_READ);
    m_profiler.possibleTransition(Directory_State_IM, Directory_Event_DMA_WRITE);
    m_profiler.possibleTransition(Directory_State_MI, Directory_Event_DMA_READ);
    m_profiler.possibleTransition(Directory_State_MI, Directory_Event_DMA_WRITE);
    m_profiler.possibleTransition(Directory_State_ID, Directory_Event_DMA_READ);
    m_profiler.possibleTransition(Directory_State_ID, Directory_Event_DMA_WRITE);
    m_profiler.possibleTransition(Directory_State_ID_W, Directory_Event_DMA_READ);
    m_profiler.possibleTransition(Directory_State_ID_W, Directory_Event_DMA_WRITE);
    m_profiler.possibleTransition(Directory_State_I, Directory_Event_GETX);
    m_profiler.possibleTransition(Directory_State_IM, Directory_Event_Memory_Data);
    m_profiler.possibleTransition(Directory_State_I, Directory_Event_DMA_READ);
    m_profiler.possibleTransition(Directory_State_ID, Directory_Event_Memory_Data);
    m_profiler.possibleTransition(Directory_State_I, Directory_Event_DMA_WRITE);
    m_profiler.possibleTransition(Directory_State_ID_W, Directory_Event_Memory_Ack);
    m_profiler.possibleTransition(Directory_State_M, Directory_Event_DMA_READ);
    m_profiler.possibleTransition(Directory_State_M_DRD, Directory_Event_PUTX);
    m_profiler.possibleTransition(Directory_State_M_DRDI, Directory_Event_Memory_Ack);
    m_profiler.possibleTransition(Directory_State_M, Directory_Event_DMA_WRITE);
    m_profiler.possibleTransition(Directory_State_M_DWR, Directory_Event_PUTX);
    m_profiler.possibleTransition(Directory_State_M_DWRI, Directory_Event_Memory_Ack);
    m_profiler.possibleTransition(Directory_State_M, Directory_Event_GETX);
    m_profiler.possibleTransition(Directory_State_M, Directory_Event_PUTX);
    m_profiler.possibleTransition(Directory_State_MI, Directory_Event_Memory_Ack);
    m_profiler.possibleTransition(Directory_State_M, Directory_Event_PUTX_NotOwner);
    m_profiler.possibleTransition(Directory_State_I, Directory_Event_PUTX_NotOwner);
}
int
Directory_Controller::getNumControllers()
{
    return m_num_controllers;
}

MessageBuffer*
Directory_Controller::getMandatoryQueue() const
{
    return NULL;
}

Sequencer*
Directory_Controller::getSequencer() const
{
    return NULL;
}

const int &
Directory_Controller::getVersion() const
{
    return m_version;
}

const string
Directory_Controller::toString() const
{
    return "Directory_Controller";
}

const string
Directory_Controller::getName() const
{
    return m_name;
}

void
Directory_Controller::stallBuffer(MessageBuffer* buf, Address addr)
{
    if (m_waiting_buffers.count(addr) == 0) {
        MsgVecType* msgVec = new MsgVecType;
        msgVec->resize(m_max_in_port_rank, NULL);
        m_waiting_buffers[addr] = msgVec;
    }
    (*(m_waiting_buffers[addr]))[m_cur_in_port_rank] = buf;
}

void
Directory_Controller::wakeUpBuffers(Address addr)
{
    if (m_waiting_buffers.count(addr) > 0) {
        //
        // Wake up all possible lower rank (i.e. lower priority) buffers that could
        // be waiting on this message.
        //
        for (int in_port_rank = m_cur_in_port_rank - 1;
             in_port_rank >= 0;
             in_port_rank--) {
            if ((*(m_waiting_buffers[addr]))[in_port_rank] != NULL) {
                (*(m_waiting_buffers[addr]))[in_port_rank]->reanalyzeMessages(addr);
            }
        }
        delete m_waiting_buffers[addr];
        m_waiting_buffers.erase(addr);
    }
}

void
Directory_Controller::wakeUpAllBuffers()
{
    //
    // Wake up all possible buffers that could be waiting on any message.
    //

    std::vector<MsgVecType*> wokeUpMsgVecs;
    
    if(m_waiting_buffers.size() > 0) {
        for (WaitingBufType::iterator buf_iter = m_waiting_buffers.begin();
             buf_iter != m_waiting_buffers.end();
             ++buf_iter) {
             for (MsgVecType::iterator vec_iter = buf_iter->second->begin();
                  vec_iter != buf_iter->second->end();
                  ++vec_iter) {
                  if (*vec_iter != NULL) {
                      (*vec_iter)->reanalyzeAllMessages();
                  }
             }
             wokeUpMsgVecs.push_back(buf_iter->second);
        }

        for (std::vector<MsgVecType*>::iterator wb_iter = wokeUpMsgVecs.begin();
             wb_iter != wokeUpMsgVecs.end();
             ++wb_iter) {
             delete (*wb_iter);
        }

        m_waiting_buffers.clear();
    }
}

void
Directory_Controller::blockOnQueue(Address addr, MessageBuffer* port)
{
    m_is_blocking = true;
    m_block_map[addr] = port;
}

void
Directory_Controller::unblock(Address addr)
{
    m_block_map.erase(addr);
    if (m_block_map.size() == 0) {
       m_is_blocking = false;
    }
}

void
Directory_Controller::print(ostream& out) const
{
    out << "[Directory_Controller " << m_version << "]";
}

void
Directory_Controller::printConfig(ostream& out) const
{
    out << "Directory_Controller config: " << m_name << endl;
    out << "  version: " << m_version << endl;
    map<string, string>::const_iterator it;
    for (it = m_cfg.begin(); it != m_cfg.end(); it++)
        out << "  " << it->first << ": " << it->second << endl;
}

void
Directory_Controller::printStats(ostream& out) const
{
    m_directory_ptr->printStats(out);
    m_memBuffer_ptr->printStats(out);
    if (m_version == 0) {
        s_profileDumper.dumpStats(out);
    }
}

void Directory_Controller::clearStats() {
    m_memBuffer_ptr->clearStats();
    m_profiler.clearStats();
}

// Set and Reset for tbe variable
void
Directory_Controller::set_tbe(Directory_TBE*& m_tbe_ptr, Directory_TBE* m_new_tbe)
{
  m_tbe_ptr = m_new_tbe;
}

void
Directory_Controller::unset_tbe(Directory_TBE*& m_tbe_ptr)
{
  m_tbe_ptr = NULL;
}

void
Directory_Controller::recordCacheTrace(int cntrl, CacheRecorder* tr)
{
}

// Actions
/** \brief Send writeback ack to requestor */
void
Directory_Controller::a_sendWriteBackAck(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing a_sendWriteBackAck\n");
    {
    // Declare message
    const RequestMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const RequestMsg *>(((*m_Directory_requestToDir_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
{
    RequestMsg *out_msg = new RequestMsg;
    (*out_msg).m_Address = addr;
    (*out_msg).m_Type = CoherenceRequestType_WB_ACK;
    (*out_msg).m_Requestor = ((*in_msg_ptr)).m_Requestor;
    (((*out_msg).m_Destination).add(((*in_msg_ptr)).m_Requestor));
    (*out_msg).m_MessageSize = MessageSizeType_Writeback_Control;
    ((*m_Directory_forwardFromDir_ptr)).enqueue(out_msg, m_directory_latency);
}
}

}

/** \brief Send writeback ack to requestor */
void
Directory_Controller::l_sendWriteBackAck(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing l_sendWriteBackAck\n");
    {
    // Declare message
    const MemoryMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const MemoryMsg *>(((*m_memBuffer_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
{
    RequestMsg *out_msg = new RequestMsg;
    (*out_msg).m_Address = addr;
    (*out_msg).m_Type = CoherenceRequestType_WB_ACK;
    (*out_msg).m_Requestor = ((*in_msg_ptr)).m_OriginalRequestorMachId;
    (((*out_msg).m_Destination).add(((*in_msg_ptr)).m_OriginalRequestorMachId));
    (*out_msg).m_MessageSize = MessageSizeType_Writeback_Control;
    ((*m_Directory_forwardFromDir_ptr)).enqueue(out_msg, 1);
}
}

}

/** \brief Send writeback nack to requestor */
void
Directory_Controller::b_sendWriteBackNack(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing b_sendWriteBackNack\n");
    {
    // Declare message
    const RequestMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const RequestMsg *>(((*m_Directory_requestToDir_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
{
    RequestMsg *out_msg = new RequestMsg;
    (*out_msg).m_Address = addr;
    (*out_msg).m_Type = CoherenceRequestType_WB_NACK;
    (*out_msg).m_Requestor = ((*in_msg_ptr)).m_Requestor;
    (((*out_msg).m_Destination).add(((*in_msg_ptr)).m_Requestor));
    (*out_msg).m_MessageSize = MessageSizeType_Writeback_Control;
    ((*m_Directory_forwardFromDir_ptr)).enqueue(out_msg, m_directory_latency);
}
}

}

/** \brief Clear the owner field */
void
Directory_Controller::c_clearOwner(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing c_clearOwner\n");
    (((*(getDirectoryEntry(addr))).m_Owner).clear());

}

/** \brief Send data to requestor */
void
Directory_Controller::d_sendData(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing d_sendData\n");
    {
    // Declare message
    const MemoryMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const MemoryMsg *>(((*m_memBuffer_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
{
    ResponseMsg *out_msg = new ResponseMsg;
    (*out_msg).m_Address = addr;
    (*out_msg).m_Type = CoherenceResponseType_DATA;
    (*out_msg).m_Sender = m_machineID;
    (((*out_msg).m_Destination).add(((*in_msg_ptr)).m_OriginalRequestorMachId));
    (*out_msg).m_DataBlk = ((*in_msg_ptr)).m_DataBlk;
    (*out_msg).m_MessageSize = MessageSizeType_Response_Data;
    ((*m_Directory_responseFromDir_ptr)).enqueue(out_msg, 1);
}
}

}

/** \brief Send Data to DMA controller from directory */
void
Directory_Controller::dr_sendDMAData(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing dr_sendDMAData\n");
    {
    // Declare message
    const MemoryMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const MemoryMsg *>(((*m_memBuffer_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
{
    DMAResponseMsg *out_msg = new DMAResponseMsg;
    #ifndef NDEBUG
    if (!((m_tbe_ptr != NULL))) {
        char c;
        cerr << "Runtime Error at MI_example-dir.sm:272, Ruby Time: "
             << g_system_ptr->getTime() << ": "
             << "assert failure"
             << ", PID: " << getpid() << endl
             << "press return to continue." << endl;
        cin.get(c);
        abort();

    }
    #endif
    ;
    (*out_msg).m_PhysicalAddress = addr;
    (*out_msg).m_LineAddress = addr;
    (*out_msg).m_Type = DMAResponseType_DATA;
    (*out_msg).m_DataBlk = ((*in_msg_ptr)).m_DataBlk;
    (((*out_msg).m_Destination).add((*m_tbe_ptr).m_DmaRequestor));
    (*out_msg).m_MessageSize = MessageSizeType_Response_Data;
    ((*m_Directory_dmaResponseFromDir_ptr)).enqueue(out_msg, 1);
}
}

}

/** \brief Send Data to DMA controller from incoming PUTX */
void
Directory_Controller::drp_sendDMAData(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing drp_sendDMAData\n");
    {
    // Declare message
    const RequestMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const RequestMsg *>(((*m_Directory_requestToDir_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
{
    DMAResponseMsg *out_msg = new DMAResponseMsg;
    #ifndef NDEBUG
    if (!((m_tbe_ptr != NULL))) {
        char c;
        cerr << "Runtime Error at MI_example-dir.sm:288, Ruby Time: "
             << g_system_ptr->getTime() << ": "
             << "assert failure"
             << ", PID: " << getpid() << endl
             << "press return to continue." << endl;
        cin.get(c);
        abort();

    }
    #endif
    ;
    (*out_msg).m_PhysicalAddress = addr;
    (*out_msg).m_LineAddress = addr;
    (*out_msg).m_Type = DMAResponseType_DATA;
    (*out_msg).m_DataBlk = ((*in_msg_ptr)).m_DataBlk;
    (((*out_msg).m_Destination).add((*m_tbe_ptr).m_DmaRequestor));
    (*out_msg).m_MessageSize = MessageSizeType_Response_Data;
    ((*m_Directory_dmaResponseFromDir_ptr)).enqueue(out_msg, 1);
}
}

}

/** \brief Send Ack to DMA controller */
void
Directory_Controller::da_sendDMAAck(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing da_sendDMAAck\n");
    {
    DMAResponseMsg *out_msg = new DMAResponseMsg;
    #ifndef NDEBUG
    if (!((m_tbe_ptr != NULL))) {
        char c;
        cerr << "Runtime Error at MI_example-dir.sm:301, Ruby Time: "
             << g_system_ptr->getTime() << ": "
             << "assert failure"
             << ", PID: " << getpid() << endl
             << "press return to continue." << endl;
        cin.get(c);
        abort();

    }
    #endif
    ;
    (*out_msg).m_PhysicalAddress = addr;
    (*out_msg).m_LineAddress = addr;
    (*out_msg).m_Type = DMAResponseType_ACK;
    (((*out_msg).m_Destination).add((*m_tbe_ptr).m_DmaRequestor));
    (*out_msg).m_MessageSize = MessageSizeType_Writeback_Control;
    ((*m_Directory_dmaResponseFromDir_ptr)).enqueue(out_msg, 1);
}

}

/** \brief The owner is now the requestor */
void
Directory_Controller::e_ownerIsRequestor(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing e_ownerIsRequestor\n");
    {
    // Declare message
    const RequestMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const RequestMsg *>(((*m_Directory_requestToDir_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
(((*(getDirectoryEntry(addr))).m_Owner).clear());
(((*(getDirectoryEntry(addr))).m_Owner).add(((*in_msg_ptr)).m_Requestor));
}

}

/** \brief Forward request to owner */
void
Directory_Controller::f_forwardRequest(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing f_forwardRequest\n");
    {
    // Declare message
    const RequestMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const RequestMsg *>(((*m_Directory_requestToDir_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
APPEND_TRANSITION_COMMENT(("Own: "));
APPEND_TRANSITION_COMMENT((*(getDirectoryEntry(((*in_msg_ptr)).m_Address))).m_Owner);
APPEND_TRANSITION_COMMENT(("Req: "));
APPEND_TRANSITION_COMMENT(((*in_msg_ptr)).m_Requestor);
{
    RequestMsg *out_msg = new RequestMsg;
    (*out_msg).m_Address = addr;
    (*out_msg).m_Type = ((*in_msg_ptr)).m_Type;
    (*out_msg).m_Requestor = ((*in_msg_ptr)).m_Requestor;
    (*out_msg).m_Destination = (*(getDirectoryEntry(((*in_msg_ptr)).m_Address))).m_Owner;
    (*out_msg).m_MessageSize = MessageSizeType_Writeback_Control;
    ((*m_Directory_forwardFromDir_ptr)).enqueue(out_msg, m_directory_latency);
}
}

}

/** \brief Invalidate a cache block */
void
Directory_Controller::inv_sendCacheInvalidate(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing inv_sendCacheInvalidate\n");
    {
    // Declare message
    const DMARequestMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const DMARequestMsg *>(((*m_Directory_dmaRequestToDir_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
{
    RequestMsg *out_msg = new RequestMsg;
    (*out_msg).m_Address = addr;
    (*out_msg).m_Type = CoherenceRequestType_INV;
    (*out_msg).m_Requestor = m_machineID;
    (*out_msg).m_Destination = (*(getDirectoryEntry(((*in_msg_ptr)).m_PhysicalAddress))).m_Owner;
    (*out_msg).m_MessageSize = MessageSizeType_Writeback_Control;
    ((*m_Directory_forwardFromDir_ptr)).enqueue(out_msg, m_directory_latency);
}
}

}

/** \brief Pop incoming request queue */
void
Directory_Controller::i_popIncomingRequestQueue(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing i_popIncomingRequestQueue\n");
    (((*m_Directory_requestToDir_ptr)).dequeue());

}

/** \brief Pop incoming DMA queue */
void
Directory_Controller::p_popIncomingDMARequestQueue(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing p_popIncomingDMARequestQueue\n");
    (((*m_Directory_dmaRequestToDir_ptr)).dequeue());

}

/** \brief Write PUTX data to memory */
void
Directory_Controller::l_writeDataToMemory(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing l_writeDataToMemory\n");
    {
    // Declare message
    const RequestMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const RequestMsg *>(((*m_Directory_requestToDir_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
(*(getDirectoryEntry(((*in_msg_ptr)).m_Address))).m_DataBlk = ((*in_msg_ptr)).m_DataBlk;
}

}

/** \brief DMA Write data to memory from TBE */
void
Directory_Controller::dwt_writeDMADataFromTBE(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing dwt_writeDMADataFromTBE\n");
    #ifndef NDEBUG
if (!((m_tbe_ptr != NULL))) {
    char c;
    cerr << "Runtime Error at MI_example-dir.sm:363, Ruby Time: "
         << g_system_ptr->getTime() << ": "
         << "assert failure"
         << ", PID: " << getpid() << endl
         << "press return to continue." << endl;
    cin.get(c);
    abort();

}
#endif
;
(((*(getDirectoryEntry(addr))).m_DataBlk).copyPartial((*m_tbe_ptr).m_DataBlk, (addressOffset((*m_tbe_ptr).m_PhysicalAddress)), (*m_tbe_ptr).m_Len));

}

/** \brief Allocate TBE */
void
Directory_Controller::v_allocateTBE(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing v_allocateTBE\n");
    {
    // Declare message
    const DMARequestMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const DMARequestMsg *>(((*m_Directory_dmaRequestToDir_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
(((*m_Directory_TBEs_ptr)).allocate(addr));
set_tbe(m_tbe_ptr, (((*m_Directory_TBEs_ptr)).lookup(addr)));;
(*m_tbe_ptr).m_DataBlk = ((*in_msg_ptr)).m_DataBlk;
(*m_tbe_ptr).m_PhysicalAddress = ((*in_msg_ptr)).m_PhysicalAddress;
(*m_tbe_ptr).m_Len = ((*in_msg_ptr)).m_Len;
(*m_tbe_ptr).m_DmaRequestor = ((*in_msg_ptr)).m_Requestor;
}

}

/** \brief Allocate TBE for DMA Read */
void
Directory_Controller::r_allocateTbeForDmaRead(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing r_allocateTbeForDmaRead\n");
    {
    // Declare message
    const DMARequestMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const DMARequestMsg *>(((*m_Directory_dmaRequestToDir_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
(((*m_Directory_TBEs_ptr)).allocate(addr));
set_tbe(m_tbe_ptr, (((*m_Directory_TBEs_ptr)).lookup(addr)));;
(*m_tbe_ptr).m_DmaRequestor = ((*in_msg_ptr)).m_Requestor;
}

}

/** \brief Allocate TBE */
void
Directory_Controller::v_allocateTBEFromRequestNet(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing v_allocateTBEFromRequestNet\n");
    {
    // Declare message
    const RequestMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const RequestMsg *>(((*m_Directory_requestToDir_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
(((*m_Directory_TBEs_ptr)).allocate(addr));
set_tbe(m_tbe_ptr, (((*m_Directory_TBEs_ptr)).lookup(addr)));;
(*m_tbe_ptr).m_DataBlk = ((*in_msg_ptr)).m_DataBlk;
}

}

/** \brief Deallocate TBE */
void
Directory_Controller::w_deallocateTBE(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing w_deallocateTBE\n");
    (((*m_Directory_TBEs_ptr)).deallocate(addr));
unset_tbe(m_tbe_ptr);;

}

/** \brief recycle request queue */
void
Directory_Controller::z_recycleRequestQueue(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing z_recycleRequestQueue\n");
    (((*m_Directory_requestToDir_ptr)).recycle());

}

/** \brief recycle dma request queue */
void
Directory_Controller::y_recycleDMARequestQueue(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing y_recycleDMARequestQueue\n");
    (((*m_Directory_dmaRequestToDir_ptr)).recycle());

}

/** \brief Queue off-chip fetch request */
void
Directory_Controller::qf_queueMemoryFetchRequest(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing qf_queueMemoryFetchRequest\n");
    {
    // Declare message
    const RequestMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const RequestMsg *>(((*m_Directory_requestToDir_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
{
    MemoryMsg *out_msg = new MemoryMsg;
    (*out_msg).m_Address = addr;
    (*out_msg).m_Type = MemoryRequestType_MEMORY_READ;
    (*out_msg).m_Sender = m_machineID;
    (*out_msg).m_OriginalRequestorMachId = ((*in_msg_ptr)).m_Requestor;
    (*out_msg).m_MessageSize = ((*in_msg_ptr)).m_MessageSize;
    (*out_msg).m_DataBlk = (*(getDirectoryEntry(((*in_msg_ptr)).m_Address))).m_DataBlk;
    DPRINTF(RubySlicc, "MI_example-dir.sm:417: %s\n", *out_msg);
    ((*m_memBuffer_ptr)).enqueue(out_msg, 1);
}
}

}

/** \brief Queue off-chip fetch request */
void
Directory_Controller::qf_queueMemoryFetchRequestDMA(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing qf_queueMemoryFetchRequestDMA\n");
    {
    // Declare message
    const DMARequestMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const DMARequestMsg *>(((*m_Directory_dmaRequestToDir_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
{
    MemoryMsg *out_msg = new MemoryMsg;
    (*out_msg).m_Address = addr;
    (*out_msg).m_Type = MemoryRequestType_MEMORY_READ;
    (*out_msg).m_Sender = m_machineID;
    (*out_msg).m_MessageSize = ((*in_msg_ptr)).m_MessageSize;
    (*out_msg).m_DataBlk = (*(getDirectoryEntry(addr))).m_DataBlk;
    DPRINTF(RubySlicc, "MI_example-dir.sm:431: %s\n", *out_msg);
    ((*m_memBuffer_ptr)).enqueue(out_msg, 1);
}
}

}

/** \brief Queue off-chip writeback request */
void
Directory_Controller::qw_queueMemoryWBRequest_partial(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing qw_queueMemoryWBRequest_partial\n");
    {
    // Declare message
    const DMARequestMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const DMARequestMsg *>(((*m_Directory_dmaRequestToDir_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
{
    MemoryMsg *out_msg = new MemoryMsg;
    (*out_msg).m_Address = addr;
    (*out_msg).m_Type = MemoryRequestType_MEMORY_WB;
    (((*out_msg).m_DataBlk).copyPartial(((*in_msg_ptr)).m_DataBlk, (addressOffset(((*in_msg_ptr)).m_PhysicalAddress)), ((*in_msg_ptr)).m_Len));
    (*out_msg).m_MessageSize = ((*in_msg_ptr)).m_MessageSize;
    DPRINTF(RubySlicc, "MI_example-dir.sm:447: %s\n", *out_msg);
    ((*m_memBuffer_ptr)).enqueue(out_msg, 1);
}
}

}

/** \brief Queue off-chip writeback request */
void
Directory_Controller::qw_queueMemoryWBRequest_partialTBE(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing qw_queueMemoryWBRequest_partialTBE\n");
    {
    // Declare message
    const RequestMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const RequestMsg *>(((*m_Directory_requestToDir_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
{
    MemoryMsg *out_msg = new MemoryMsg;
    #ifndef NDEBUG
    if (!((m_tbe_ptr != NULL))) {
        char c;
        cerr << "Runtime Error at MI_example-dir.sm:455, Ruby Time: "
             << g_system_ptr->getTime() << ": "
             << "assert failure"
             << ", PID: " << getpid() << endl
             << "press return to continue." << endl;
        cin.get(c);
        abort();

    }
    #endif
    ;
    (*out_msg).m_Address = addr;
    (*out_msg).m_Type = MemoryRequestType_MEMORY_WB;
    (*out_msg).m_OriginalRequestorMachId = ((*in_msg_ptr)).m_Requestor;
    (((*out_msg).m_DataBlk).copyPartial((*m_tbe_ptr).m_DataBlk, (addressOffset((*m_tbe_ptr).m_PhysicalAddress)), (*m_tbe_ptr).m_Len));
    (*out_msg).m_MessageSize = ((*in_msg_ptr)).m_MessageSize;
    DPRINTF(RubySlicc, "MI_example-dir.sm:465: %s\n", *out_msg);
    ((*m_memBuffer_ptr)).enqueue(out_msg, 1);
}
}

}

/** \brief Write PUTX data to memory */
void
Directory_Controller::l_queueMemoryWBRequest(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing l_queueMemoryWBRequest\n");
    {
    // Declare message
    const RequestMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const RequestMsg *>(((*m_Directory_requestToDir_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
{
    MemoryMsg *out_msg = new MemoryMsg;
    (*out_msg).m_Address = addr;
    (*out_msg).m_Type = MemoryRequestType_MEMORY_WB;
    (*out_msg).m_Sender = m_machineID;
    (*out_msg).m_OriginalRequestorMachId = ((*in_msg_ptr)).m_Requestor;
    (*out_msg).m_DataBlk = ((*in_msg_ptr)).m_DataBlk;
    (*out_msg).m_MessageSize = ((*in_msg_ptr)).m_MessageSize;
    DPRINTF(RubySlicc, "MI_example-dir.sm:483: %s\n", *out_msg);
    ((*m_memBuffer_ptr)).enqueue(out_msg, 1);
}
}

}

/** \brief Pop off-chip request queue */
void
Directory_Controller::l_popMemQueue(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing l_popMemQueue\n");
    (((*m_memBuffer_ptr)).dequeue());

}

/** \brief Write date to directory memory from TBE */
void
Directory_Controller::w_writeDataToMemoryFromTBE(Directory_TBE*& m_tbe_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing w_writeDataToMemoryFromTBE\n");
    #ifndef NDEBUG
if (!((m_tbe_ptr != NULL))) {
    char c;
    cerr << "Runtime Error at MI_example-dir.sm:494, Ruby Time: "
         << g_system_ptr->getTime() << ": "
         << "assert failure"
         << ", PID: " << getpid() << endl
         << "press return to continue." << endl;
    cin.get(c);
    abort();

}
#endif
;
(((*(getDirectoryEntry(addr))).m_DataBlk).copyPartial((*m_tbe_ptr).m_DataBlk, (addressOffset((*m_tbe_ptr).m_PhysicalAddress)), (*m_tbe_ptr).m_Len));

}

Directory_Entry*
Directory_Controller::getDirectoryEntry(const Address& param_addr)
{
Directory_Entry* dir_entry
 = static_cast<Directory_Entry *>((((*m_directory_ptr)).lookup(param_addr)))
;
    if ((dir_entry != NULL)) {
        return dir_entry;
    }
    dir_entry = static_cast<Directory_Entry *>((((*m_directory_ptr)).allocate(param_addr, new Directory_Entry)))
    ;
    return dir_entry;

}
Directory_State
Directory_Controller::getState(Directory_TBE* param_tbe, const Address& param_addr)
{
    if ((param_tbe != NULL)) {
        return (*param_tbe).m_TBEState;
    } else {
            if ((((*m_directory_ptr)).isPresent(param_addr))) {
                return (*(getDirectoryEntry(param_addr))).m_DirectoryState;
            } else {
                return Directory_State_I;
            }
        }

}
void
Directory_Controller::setState(Directory_TBE* param_tbe, const Address& param_addr, const Directory_State& param_state)
{
    if ((param_tbe != NULL)) {
        (*param_tbe).m_TBEState = param_state;
    }
        if ((((*m_directory_ptr)).isPresent(param_addr))) {
                if ((param_state == Directory_State_M)) {
                    #ifndef NDEBUG
                    if (!(((((*(getDirectoryEntry(param_addr))).m_Owner).count()) == (1)))) {
                        char c;
                        cerr << "Runtime Error at MI_example-dir.sm:113, Ruby Time: "
                             << g_system_ptr->getTime() << ": "
                             << "assert failure"
                             << ", PID: " << getpid() << endl
                             << "press return to continue." << endl;
                        cin.get(c);
                        abort();

                    }
                    #endif
                    ;
                    #ifndef NDEBUG
                    if (!(((((*(getDirectoryEntry(param_addr))).m_Sharers).count()) == (0)))) {
                        char c;
                        cerr << "Runtime Error at MI_example-dir.sm:114, Ruby Time: "
                             << g_system_ptr->getTime() << ": "
                             << "assert failure"
                             << ", PID: " << getpid() << endl
                             << "press return to continue." << endl;
                        cin.get(c);
                        abort();

                    }
                    #endif
                    ;
                }
                (*(getDirectoryEntry(param_addr))).m_DirectoryState = param_state;
                    if ((param_state == Directory_State_I)) {
                        #ifndef NDEBUG
                        if (!(((((*(getDirectoryEntry(param_addr))).m_Owner).count()) == (0)))) {
                            char c;
                            cerr << "Runtime Error at MI_example-dir.sm:120, Ruby Time: "
                                 << g_system_ptr->getTime() << ": "
                                 << "assert failure"
                                 << ", PID: " << getpid() << endl
                                 << "press return to continue." << endl;
                            cin.get(c);
                            abort();

                        }
                        #endif
                        ;
                        #ifndef NDEBUG
                        if (!(((((*(getDirectoryEntry(param_addr))).m_Sharers).count()) == (0)))) {
                            char c;
                            cerr << "Runtime Error at MI_example-dir.sm:121, Ruby Time: "
                                 << g_system_ptr->getTime() << ": "
                                 << "assert failure"
                                 << ", PID: " << getpid() << endl
                                 << "press return to continue." << endl;
                            cin.get(c);
                            abort();

                        }
                        #endif
                        ;
                        (((*m_directory_ptr)).invalidateBlock(param_addr));
                    }
                }

}
AccessPermission
Directory_Controller::getAccessPermission(const Address& param_addr)
{
Directory_TBE* tbe
 = (((*m_Directory_TBEs_ptr)).lookup(param_addr));
    if ((tbe != NULL)) {
        return (Directory_State_to_permission((*tbe).m_TBEState));
    }
        if ((((*m_directory_ptr)).isPresent(param_addr))) {
            return (Directory_State_to_permission((*(getDirectoryEntry(param_addr))).m_DirectoryState));
        }
        return AccessPermission_NotPresent;

}
void
Directory_Controller::setAccessPermission(const Address& param_addr, const Directory_State& param_state)
{
    if ((((*m_directory_ptr)).isPresent(param_addr))) {
        ((*((getDirectoryEntry(param_addr)))).changePermission((Directory_State_to_permission(param_state))));
    }

}
DataBlock&
Directory_Controller::getDataBlock(const Address& param_addr)
{
return (*(getDirectoryEntry(param_addr))).m_DataBlk;

}
