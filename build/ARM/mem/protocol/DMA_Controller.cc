/** \file DMA_Controller.cc
 *
 * Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/StateMachine.py:436
 * Created by slicc definition of Module "DMA Controller"
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
#include "mem/protocol/DMA_Controller.hh"
#include "mem/protocol/DMA_Event.hh"
#include "mem/protocol/DMA_State.hh"
#include "mem/protocol/Types.hh"
#include "mem/ruby/common/Global.hh"
#include "mem/ruby/slicc_interface/RubySlicc_includes.hh"
#include "mem/ruby/system/System.hh"

using namespace std;
#include "mem/protocol/MessageBuffer.hh"
#include "mem/protocol/DMA_State.hh"
DMA_Controller *
DMA_ControllerParams::create()
{
    return new DMA_Controller(this);
}

int DMA_Controller::m_num_controllers = 0;
DMA_ProfileDumper DMA_Controller::s_profileDumper;

// for adding information to the protocol debug trace
stringstream DMA_transitionComment;
#define APPEND_TRANSITION_COMMENT(str) (DMA_transitionComment << str)

/** \brief constructor */
DMA_Controller::DMA_Controller(const Params *p)
    : AbstractController(p)
{
    m_version = p->version;
    m_transitions_per_cycle = p->transitions_per_cycle;
    m_buffer_size = p->buffer_size;
    m_recycle_latency = p->recycle_latency;
    m_number_of_TBEs = p->number_of_TBEs;
    m_is_blocking = false;
    m_name = "DMA";
    m_max_in_port_rank = 1;
    m_dma_sequencer_ptr = p->dma_sequencer;
    m_request_latency = p->request_latency;
    m_dma_sequencer_ptr->setController(this);
    m_num_controllers++;
    m_DMA_mandatoryQueue_ptr = new MessageBuffer();
}

void
DMA_Controller::init()
{
    MachineType machine_type;
    int base;

    m_machineID.type = MachineType_DMA;
    m_machineID.num = m_version;

    // initialize objects
    m_profiler.setVersion(m_version);
    s_profileDumper.registerProfiler(&m_profiler);

    machine_type = string_to_MachineType("DMA");
    base = MachineType_base_number(machine_type);
    m_DMA_responseFromDir_ptr = m_net_ptr->getFromNetQueue(m_version + base, true, 1, "response");
    assert(m_DMA_responseFromDir_ptr != NULL);
    m_DMA_responseFromDir_ptr->setOrdering(true);
    if (m_buffer_size > 0) {
        m_DMA_responseFromDir_ptr->resize(m_buffer_size);
    }
    m_DMA_responseFromDir_ptr->setDescription("[Version " + to_string(m_version) + ", DMA, name=DMA_responseFromDir]");

    m_DMA_responseFromDir_ptr->setRecycleLatency(m_recycle_latency);
    machine_type = string_to_MachineType("DMA");
    base = MachineType_base_number(machine_type);
    m_DMA_reqToDirectory_ptr = m_net_ptr->getToNetQueue(m_version + base, false, 0, "request");
    assert(m_DMA_reqToDirectory_ptr != NULL);
    m_DMA_reqToDirectory_ptr->setOrdering(false);
    if (m_buffer_size > 0) {
        m_DMA_reqToDirectory_ptr->resize(m_buffer_size);
    }
    m_DMA_reqToDirectory_ptr->setDescription("[Version " + to_string(m_version) + ", DMA, name=DMA_reqToDirectory]");

    m_DMA_reqToDirectory_ptr->setRecycleLatency(m_recycle_latency);
    assert(m_DMA_mandatoryQueue_ptr != NULL);
    m_DMA_mandatoryQueue_ptr->setOrdering(false);
    m_DMA_mandatoryQueue_ptr->setRecycleLatency(m_recycle_latency);
    m_DMA_cur_state_ptr  = new DMA_State();
    assert(m_DMA_cur_state_ptr != NULL);
    *m_DMA_cur_state_ptr = DMA_State_READY; // Type State default

    (*m_DMA_mandatoryQueue_ptr).setConsumer(this);
    (*m_DMA_responseFromDir_ptr).setConsumer(this);

    (*m_DMA_mandatoryQueue_ptr).setDescription("[Version " + to_string(m_version) + ", DMA, dmaRequestQueue_in]");
    (*m_DMA_responseFromDir_ptr).setDescription("[Version " + to_string(m_version) + ", DMA, dmaResponseQueue_in]");

    m_profiler.possibleTransition(DMA_State_READY, DMA_Event_ReadRequest);
    m_profiler.possibleTransition(DMA_State_READY, DMA_Event_WriteRequest);
    m_profiler.possibleTransition(DMA_State_BUSY_RD, DMA_Event_Data);
    m_profiler.possibleTransition(DMA_State_BUSY_WR, DMA_Event_Ack);
}
int
DMA_Controller::getNumControllers()
{
    return m_num_controllers;
}

MessageBuffer*
DMA_Controller::getMandatoryQueue() const
{
    return m_DMA_mandatoryQueue_ptr;
}

Sequencer*
DMA_Controller::getSequencer() const
{
    return NULL;
}

const int &
DMA_Controller::getVersion() const
{
    return m_version;
}

const string
DMA_Controller::toString() const
{
    return "DMA_Controller";
}

const string
DMA_Controller::getName() const
{
    return m_name;
}

void
DMA_Controller::stallBuffer(MessageBuffer* buf, Address addr)
{
    if (m_waiting_buffers.count(addr) == 0) {
        MsgVecType* msgVec = new MsgVecType;
        msgVec->resize(m_max_in_port_rank, NULL);
        m_waiting_buffers[addr] = msgVec;
    }
    (*(m_waiting_buffers[addr]))[m_cur_in_port_rank] = buf;
}

void
DMA_Controller::wakeUpBuffers(Address addr)
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
DMA_Controller::wakeUpAllBuffers()
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
DMA_Controller::blockOnQueue(Address addr, MessageBuffer* port)
{
    m_is_blocking = true;
    m_block_map[addr] = port;
}

void
DMA_Controller::unblock(Address addr)
{
    m_block_map.erase(addr);
    if (m_block_map.size() == 0) {
       m_is_blocking = false;
    }
}

void
DMA_Controller::print(ostream& out) const
{
    out << "[DMA_Controller " << m_version << "]";
}

void
DMA_Controller::printConfig(ostream& out) const
{
    out << "DMA_Controller config: " << m_name << endl;
    out << "  version: " << m_version << endl;
    map<string, string>::const_iterator it;
    for (it = m_cfg.begin(); it != m_cfg.end(); it++)
        out << "  " << it->first << ": " << it->second << endl;
}

void
DMA_Controller::printStats(ostream& out) const
{
    if (m_version == 0) {
        s_profileDumper.dumpStats(out);
    }
}

void DMA_Controller::clearStats() {
    m_profiler.clearStats();
}

void
DMA_Controller::recordCacheTrace(int cntrl, CacheRecorder* tr)
{
}

// Actions
/** \brief Send a DMA read request to memory */
void
DMA_Controller::s_sendReadRequest(const Address& addr)
{
    DPRINTF(RubyGenerated, "executing s_sendReadRequest\n");
    {
    // Declare message
    const SequencerMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const SequencerMsg *>(((*m_DMA_mandatoryQueue_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
{
    DMARequestMsg *out_msg = new DMARequestMsg;
    (*out_msg).m_PhysicalAddress = ((*in_msg_ptr)).m_PhysicalAddress;
    (*out_msg).m_LineAddress = ((*in_msg_ptr)).m_LineAddress;
    (*out_msg).m_Type = DMARequestType_READ;
    (*out_msg).m_Requestor = m_machineID;
    (*out_msg).m_DataBlk = ((*in_msg_ptr)).m_DataBlk;
    (*out_msg).m_Len = ((*in_msg_ptr)).m_Len;
    (((*out_msg).m_Destination).add((map_Address_to_Directory(addr))));
    (*out_msg).m_MessageSize = MessageSizeType_Writeback_Control;
    ((*m_DMA_reqToDirectory_ptr)).enqueue(out_msg, m_request_latency);
}
}

}

/** \brief Send a DMA write request to memory */
void
DMA_Controller::s_sendWriteRequest(const Address& addr)
{
    DPRINTF(RubyGenerated, "executing s_sendWriteRequest\n");
    {
    // Declare message
    const SequencerMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const SequencerMsg *>(((*m_DMA_mandatoryQueue_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
{
    DMARequestMsg *out_msg = new DMARequestMsg;
    (*out_msg).m_PhysicalAddress = ((*in_msg_ptr)).m_PhysicalAddress;
    (*out_msg).m_LineAddress = ((*in_msg_ptr)).m_LineAddress;
    (*out_msg).m_Type = DMARequestType_WRITE;
    (*out_msg).m_Requestor = m_machineID;
    (*out_msg).m_DataBlk = ((*in_msg_ptr)).m_DataBlk;
    (*out_msg).m_Len = ((*in_msg_ptr)).m_Len;
    (((*out_msg).m_Destination).add((map_Address_to_Directory(addr))));
    (*out_msg).m_MessageSize = MessageSizeType_Writeback_Control;
    ((*m_DMA_reqToDirectory_ptr)).enqueue(out_msg, m_request_latency);
}
}

}

/** \brief Notify dma controller that write request completed */
void
DMA_Controller::a_ackCallback(const Address& addr)
{
    DPRINTF(RubyGenerated, "executing a_ackCallback\n");
    {
    // Declare message
    const DMAResponseMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const DMAResponseMsg *>(((*m_DMA_responseFromDir_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
(((*m_dma_sequencer_ptr)).ackCallback());
}

}

/** \brief Write data to dma sequencer */
void
DMA_Controller::d_dataCallback(const Address& addr)
{
    DPRINTF(RubyGenerated, "executing d_dataCallback\n");
    {
    // Declare message
    const DMAResponseMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const DMAResponseMsg *>(((*m_DMA_responseFromDir_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
(((*m_dma_sequencer_ptr)).dataCallback(((*in_msg_ptr)).m_DataBlk));
}

}

/** \brief Pop request queue */
void
DMA_Controller::p_popRequestQueue(const Address& addr)
{
    DPRINTF(RubyGenerated, "executing p_popRequestQueue\n");
    (((*m_DMA_mandatoryQueue_ptr)).dequeue());

}

/** \brief Pop request queue */
void
DMA_Controller::p_popResponseQueue(const Address& addr)
{
    DPRINTF(RubyGenerated, "executing p_popResponseQueue\n");
    (((*m_DMA_responseFromDir_ptr)).dequeue());

}

DMA_State
DMA_Controller::getState(const Address& param_addr)
{
return (*m_DMA_cur_state_ptr);

}
void
DMA_Controller::setState(const Address& param_addr, const DMA_State& param_state)
{
(*m_DMA_cur_state_ptr) = param_state;

}
AccessPermission
DMA_Controller::getAccessPermission(const Address& param_addr)
{
return AccessPermission_NotPresent;

}
void
DMA_Controller::setAccessPermission(const Address& param_addr, const DMA_State& param_state)
{

}
DataBlock&
DMA_Controller::getDataBlock(const Address& param_addr)
{
char c;
cerr << "Runtime Error at MI_example-dma.sm:41, Ruby Time: "
     << g_system_ptr->getTime() << ": "
     << ("DMA Controller does not support getDataBlock function.\n")
     << ", PID: " << getpid() << endl
     << "press return to continue." << endl;
cin.get(c);
abort();
;

}
