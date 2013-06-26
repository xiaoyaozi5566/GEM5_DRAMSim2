/** \file L1Cache_Controller.cc
 *
 * Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/StateMachine.py:436
 * Created by slicc definition of Module "MI Example L1 Cache"
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
#include "mem/protocol/L1Cache_Controller.hh"
#include "mem/protocol/L1Cache_Event.hh"
#include "mem/protocol/L1Cache_State.hh"
#include "mem/protocol/Types.hh"
#include "mem/ruby/common/Global.hh"
#include "mem/ruby/slicc_interface/RubySlicc_includes.hh"
#include "mem/ruby/system/System.hh"

using namespace std;
#include "mem/protocol/MessageBuffer.hh"
#include "mem/protocol/TBETable.hh"
L1Cache_Controller *
L1Cache_ControllerParams::create()
{
    return new L1Cache_Controller(this);
}

int L1Cache_Controller::m_num_controllers = 0;
L1Cache_ProfileDumper L1Cache_Controller::s_profileDumper;

// for adding information to the protocol debug trace
stringstream L1Cache_transitionComment;
#define APPEND_TRANSITION_COMMENT(str) (L1Cache_transitionComment << str)

/** \brief constructor */
L1Cache_Controller::L1Cache_Controller(const Params *p)
    : AbstractController(p)
{
    m_version = p->version;
    m_transitions_per_cycle = p->transitions_per_cycle;
    m_buffer_size = p->buffer_size;
    m_recycle_latency = p->recycle_latency;
    m_number_of_TBEs = p->number_of_TBEs;
    m_is_blocking = false;
    m_name = "L1Cache";
    m_max_in_port_rank = 1;
    m_sequencer_ptr = p->sequencer;
    m_cacheMemory_ptr = p->cacheMemory;
    m_cache_response_latency = p->cache_response_latency;
    m_issue_latency = p->issue_latency;
    m_send_evictions = p->send_evictions;
    m_sequencer_ptr->setController(this);
        
    m_num_controllers++;
    m_L1Cache_mandatoryQueue_ptr = new MessageBuffer();
}

void
L1Cache_Controller::init()
{
    MachineType machine_type;
    int base;

    m_machineID.type = MachineType_L1Cache;
    m_machineID.num = m_version;

    // initialize objects
    m_profiler.setVersion(m_version);
    s_profileDumper.registerProfiler(&m_profiler);

    machine_type = string_to_MachineType("L1Cache");
    base = MachineType_base_number(machine_type);
    m_L1Cache_requestFromCache_ptr = m_net_ptr->getToNetQueue(m_version + base, true, 2, "request");
    assert(m_L1Cache_requestFromCache_ptr != NULL);
    m_L1Cache_requestFromCache_ptr->setOrdering(true);
    if (m_buffer_size > 0) {
        m_L1Cache_requestFromCache_ptr->resize(m_buffer_size);
    }
    m_L1Cache_requestFromCache_ptr->setDescription("[Version " + to_string(m_version) + ", L1Cache, name=L1Cache_requestFromCache]");

    m_L1Cache_requestFromCache_ptr->setRecycleLatency(m_recycle_latency);
    machine_type = string_to_MachineType("L1Cache");
    base = MachineType_base_number(machine_type);
    m_L1Cache_responseFromCache_ptr = m_net_ptr->getToNetQueue(m_version + base, true, 4, "response");
    assert(m_L1Cache_responseFromCache_ptr != NULL);
    m_L1Cache_responseFromCache_ptr->setOrdering(true);
    if (m_buffer_size > 0) {
        m_L1Cache_responseFromCache_ptr->resize(m_buffer_size);
    }
    m_L1Cache_responseFromCache_ptr->setDescription("[Version " + to_string(m_version) + ", L1Cache, name=L1Cache_responseFromCache]");

    m_L1Cache_responseFromCache_ptr->setRecycleLatency(m_recycle_latency);
    machine_type = string_to_MachineType("L1Cache");
    base = MachineType_base_number(machine_type);
    m_L1Cache_forwardToCache_ptr = m_net_ptr->getFromNetQueue(m_version + base, true, 3, "forward");
    assert(m_L1Cache_forwardToCache_ptr != NULL);
    m_L1Cache_forwardToCache_ptr->setOrdering(true);
    if (m_buffer_size > 0) {
        m_L1Cache_forwardToCache_ptr->resize(m_buffer_size);
    }
    m_L1Cache_forwardToCache_ptr->setDescription("[Version " + to_string(m_version) + ", L1Cache, name=L1Cache_forwardToCache]");

    m_L1Cache_forwardToCache_ptr->setRecycleLatency(m_recycle_latency);
    machine_type = string_to_MachineType("L1Cache");
    base = MachineType_base_number(machine_type);
    m_L1Cache_responseToCache_ptr = m_net_ptr->getFromNetQueue(m_version + base, true, 4, "response");
    assert(m_L1Cache_responseToCache_ptr != NULL);
    m_L1Cache_responseToCache_ptr->setOrdering(true);
    if (m_buffer_size > 0) {
        m_L1Cache_responseToCache_ptr->resize(m_buffer_size);
    }
    m_L1Cache_responseToCache_ptr->setDescription("[Version " + to_string(m_version) + ", L1Cache, name=L1Cache_responseToCache]");

    m_L1Cache_responseToCache_ptr->setRecycleLatency(m_recycle_latency);
    assert(m_L1Cache_mandatoryQueue_ptr != NULL);
    m_L1Cache_mandatoryQueue_ptr->setOrdering(false);
    m_L1Cache_mandatoryQueue_ptr->setRecycleLatency(m_recycle_latency);
    m_L1Cache_TBEs_ptr  = new TBETable<L1Cache_TBE>(m_number_of_TBEs);
    assert(m_L1Cache_TBEs_ptr != NULL);

    (*m_L1Cache_forwardToCache_ptr).setConsumer(this);
    (*m_L1Cache_responseToCache_ptr).setConsumer(this);
    (*m_L1Cache_mandatoryQueue_ptr).setConsumer(this);

    (*m_L1Cache_forwardToCache_ptr).setDescription("[Version " + to_string(m_version) + ", L1Cache, forwardRequestNetwork_in]");
    (*m_L1Cache_responseToCache_ptr).setDescription("[Version " + to_string(m_version) + ", L1Cache, responseNetwork_in]");
    (*m_L1Cache_mandatoryQueue_ptr).setDescription("[Version " + to_string(m_version) + ", L1Cache, mandatoryQueue_in]");

    m_profiler.possibleTransition(L1Cache_State_MI, L1Cache_Event_Inv);
    m_profiler.possibleTransition(L1Cache_State_M, L1Cache_Event_Store);
    m_profiler.possibleTransition(L1Cache_State_M, L1Cache_Event_Load);
    m_profiler.possibleTransition(L1Cache_State_M, L1Cache_Event_Ifetch);
    m_profiler.possibleTransition(L1Cache_State_I, L1Cache_Event_Inv);
    m_profiler.possibleTransition(L1Cache_State_I, L1Cache_Event_Store);
    m_profiler.possibleTransition(L1Cache_State_I, L1Cache_Event_Load);
    m_profiler.possibleTransition(L1Cache_State_I, L1Cache_Event_Ifetch);
    m_profiler.possibleTransition(L1Cache_State_IS, L1Cache_Event_Data);
    m_profiler.possibleTransition(L1Cache_State_IM, L1Cache_Event_Data);
    m_profiler.possibleTransition(L1Cache_State_M, L1Cache_Event_Fwd_GETX);
    m_profiler.possibleTransition(L1Cache_State_I, L1Cache_Event_Replacement);
    m_profiler.possibleTransition(L1Cache_State_M, L1Cache_Event_Replacement);
    m_profiler.possibleTransition(L1Cache_State_M, L1Cache_Event_Inv);
    m_profiler.possibleTransition(L1Cache_State_MI, L1Cache_Event_Writeback_Ack);
    m_profiler.possibleTransition(L1Cache_State_MI, L1Cache_Event_Fwd_GETX);
    m_profiler.possibleTransition(L1Cache_State_MI, L1Cache_Event_Writeback_Nack);
    m_profiler.possibleTransition(L1Cache_State_MII, L1Cache_Event_Fwd_GETX);
    m_profiler.possibleTransition(L1Cache_State_II, L1Cache_Event_Writeback_Nack);
}
int
L1Cache_Controller::getNumControllers()
{
    return m_num_controllers;
}

MessageBuffer*
L1Cache_Controller::getMandatoryQueue() const
{
    return m_L1Cache_mandatoryQueue_ptr;
}

Sequencer*
L1Cache_Controller::getSequencer() const
{
    return m_sequencer_ptr;
}

const int &
L1Cache_Controller::getVersion() const
{
    return m_version;
}

const string
L1Cache_Controller::toString() const
{
    return "L1Cache_Controller";
}

const string
L1Cache_Controller::getName() const
{
    return m_name;
}

void
L1Cache_Controller::stallBuffer(MessageBuffer* buf, Address addr)
{
    if (m_waiting_buffers.count(addr) == 0) {
        MsgVecType* msgVec = new MsgVecType;
        msgVec->resize(m_max_in_port_rank, NULL);
        m_waiting_buffers[addr] = msgVec;
    }
    (*(m_waiting_buffers[addr]))[m_cur_in_port_rank] = buf;
}

void
L1Cache_Controller::wakeUpBuffers(Address addr)
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
L1Cache_Controller::wakeUpAllBuffers()
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
L1Cache_Controller::blockOnQueue(Address addr, MessageBuffer* port)
{
    m_is_blocking = true;
    m_block_map[addr] = port;
}

void
L1Cache_Controller::unblock(Address addr)
{
    m_block_map.erase(addr);
    if (m_block_map.size() == 0) {
       m_is_blocking = false;
    }
}

void
L1Cache_Controller::print(ostream& out) const
{
    out << "[L1Cache_Controller " << m_version << "]";
}

void
L1Cache_Controller::printConfig(ostream& out) const
{
    out << "L1Cache_Controller config: " << m_name << endl;
    out << "  version: " << m_version << endl;
    map<string, string>::const_iterator it;
    for (it = m_cfg.begin(); it != m_cfg.end(); it++)
        out << "  " << it->first << ": " << it->second << endl;
}

void
L1Cache_Controller::printStats(ostream& out) const
{
    m_cacheMemory_ptr->printStats(out);
    if (m_version == 0) {
        s_profileDumper.dumpStats(out);
    }
}

void L1Cache_Controller::clearStats() {
    m_cacheMemory_ptr->clearStats();
    m_profiler.clearStats();
}

// Set and Reset for cache_entry variable
void
L1Cache_Controller::set_cache_entry(L1Cache_Entry*& m_cache_entry_ptr, AbstractCacheEntry* m_new_cache_entry)
{
  m_cache_entry_ptr = (L1Cache_Entry*)m_new_cache_entry;
}

void
L1Cache_Controller::unset_cache_entry(L1Cache_Entry*& m_cache_entry_ptr)
{
  m_cache_entry_ptr = 0;
}

// Set and Reset for tbe variable
void
L1Cache_Controller::set_tbe(L1Cache_TBE*& m_tbe_ptr, L1Cache_TBE* m_new_tbe)
{
  m_tbe_ptr = m_new_tbe;
}

void
L1Cache_Controller::unset_tbe(L1Cache_TBE*& m_tbe_ptr)
{
  m_tbe_ptr = NULL;
}

void
L1Cache_Controller::recordCacheTrace(int cntrl, CacheRecorder* tr)
{
    m_cacheMemory_ptr->recordCacheContents(cntrl, tr);
}

// Actions
/** \brief Issue a request */
void
L1Cache_Controller::a_issueRequest(L1Cache_TBE*& m_tbe_ptr, L1Cache_Entry*& m_cache_entry_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing a_issueRequest\n");
    {
    RequestMsg *out_msg = new RequestMsg;
    (*out_msg).m_Address = addr;
    (*out_msg).m_Type = CoherenceRequestType_GETX;
    (*out_msg).m_Requestor = m_machineID;
    (((*out_msg).m_Destination).add((map_Address_to_Directory(addr))));
    (*out_msg).m_MessageSize = MessageSizeType_Control;
    ((*m_L1Cache_requestFromCache_ptr)).enqueue(out_msg, m_issue_latency);
}

}

/** \brief Issue a PUT request */
void
L1Cache_Controller::b_issuePUT(L1Cache_TBE*& m_tbe_ptr, L1Cache_Entry*& m_cache_entry_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing b_issuePUT\n");
    {
    RequestMsg *out_msg = new RequestMsg;
    #ifndef NDEBUG
    if (!((m_cache_entry_ptr != NULL))) {
        char c;
        cerr << "Runtime Error at MI_example-cache.sm:241, Ruby Time: "
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
    (*out_msg).m_Type = CoherenceRequestType_PUTX;
    (*out_msg).m_Requestor = m_machineID;
    (((*out_msg).m_Destination).add((map_Address_to_Directory(addr))));
    (*out_msg).m_DataBlk = (*m_cache_entry_ptr).m_DataBlk;
    (*out_msg).m_MessageSize = MessageSizeType_Data;
    ((*m_L1Cache_requestFromCache_ptr)).enqueue(out_msg, m_issue_latency);
}

}

/** \brief Send data from cache to requestor */
void
L1Cache_Controller::e_sendData(L1Cache_TBE*& m_tbe_ptr, L1Cache_Entry*& m_cache_entry_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing e_sendData\n");
    {
    // Declare message
    const RequestMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const RequestMsg *>(((*m_L1Cache_forwardToCache_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
{
    ResponseMsg *out_msg = new ResponseMsg;
    #ifndef NDEBUG
    if (!((m_cache_entry_ptr != NULL))) {
        char c;
        cerr << "Runtime Error at MI_example-cache.sm:254, Ruby Time: "
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
    (*out_msg).m_Type = CoherenceResponseType_DATA;
    (*out_msg).m_Sender = m_machineID;
    (((*out_msg).m_Destination).add(((*in_msg_ptr)).m_Requestor));
    (*out_msg).m_DataBlk = (*m_cache_entry_ptr).m_DataBlk;
    (*out_msg).m_MessageSize = MessageSizeType_Response_Data;
    ((*m_L1Cache_responseFromCache_ptr)).enqueue(out_msg, m_cache_response_latency);
}
}

}

/** \brief Send data from TBE to requestor */
void
L1Cache_Controller::ee_sendDataFromTBE(L1Cache_TBE*& m_tbe_ptr, L1Cache_Entry*& m_cache_entry_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing ee_sendDataFromTBE\n");
    {
    // Declare message
    const RequestMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const RequestMsg *>(((*m_L1Cache_forwardToCache_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
{
    ResponseMsg *out_msg = new ResponseMsg;
    #ifndef NDEBUG
    if (!((m_tbe_ptr != NULL))) {
        char c;
        cerr << "Runtime Error at MI_example-cache.sm:268, Ruby Time: "
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
    (*out_msg).m_Type = CoherenceResponseType_DATA;
    (*out_msg).m_Sender = m_machineID;
    (((*out_msg).m_Destination).add(((*in_msg_ptr)).m_Requestor));
    (*out_msg).m_DataBlk = (*m_tbe_ptr).m_DataBlk;
    (*out_msg).m_MessageSize = MessageSizeType_Response_Data;
    ((*m_L1Cache_responseFromCache_ptr)).enqueue(out_msg, m_cache_response_latency);
}
}

}

/** \brief Allocate a cache block */
void
L1Cache_Controller::i_allocateL1CacheBlock(L1Cache_TBE*& m_tbe_ptr, L1Cache_Entry*& m_cache_entry_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing i_allocateL1CacheBlock\n");
        if ((m_cache_entry_ptr != NULL)) {
    } else {
        set_cache_entry(m_cache_entry_ptr, (((*m_cacheMemory_ptr)).allocate(addr, new L1Cache_Entry)));;
    }

}

/** \brief deallocate a cache block */
void
L1Cache_Controller::h_deallocateL1CacheBlock(L1Cache_TBE*& m_tbe_ptr, L1Cache_Entry*& m_cache_entry_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing h_deallocateL1CacheBlock\n");
        if ((m_cache_entry_ptr != NULL)) {
        (((*m_cacheMemory_ptr)).deallocate(addr));
        unset_cache_entry(m_cache_entry_ptr);;
    }

}

/** \brief Pop the mandatory request queue */
void
L1Cache_Controller::m_popMandatoryQueue(L1Cache_TBE*& m_tbe_ptr, L1Cache_Entry*& m_cache_entry_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing m_popMandatoryQueue\n");
    (((*m_L1Cache_mandatoryQueue_ptr)).dequeue());

}

/** \brief Pop the response queue */
void
L1Cache_Controller::n_popResponseQueue(L1Cache_TBE*& m_tbe_ptr, L1Cache_Entry*& m_cache_entry_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing n_popResponseQueue\n");
    (profileMsgDelay((1), (((*m_L1Cache_responseToCache_ptr)).dequeue_getDelayCycles())));

}

/** \brief Pop the forwarded request queue */
void
L1Cache_Controller::o_popForwardedRequestQueue(L1Cache_TBE*& m_tbe_ptr, L1Cache_Entry*& m_cache_entry_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing o_popForwardedRequestQueue\n");
    (profileMsgDelay((2), (((*m_L1Cache_forwardToCache_ptr)).dequeue_getDelayCycles())));

}

/** \brief Profile cache miss */
void
L1Cache_Controller::p_profileMiss(L1Cache_TBE*& m_tbe_ptr, L1Cache_Entry*& m_cache_entry_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing p_profileMiss\n");
    {
    // Declare message
    const RubyRequest* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const RubyRequest *>(((*m_L1Cache_mandatoryQueue_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
(((*m_cacheMemory_ptr)).profileMiss((*in_msg_ptr)));
}

}

/** \brief Notify sequencer the load completed. */
void
L1Cache_Controller::r_load_hit(L1Cache_TBE*& m_tbe_ptr, L1Cache_Entry*& m_cache_entry_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing r_load_hit\n");
    #ifndef NDEBUG
if (!((m_cache_entry_ptr != NULL))) {
    char c;
    cerr << "Runtime Error at MI_example-cache.sm:312, Ruby Time: "
         << g_system_ptr->getTime() << ": "
         << "assert failure"
         << ", PID: " << getpid() << endl
         << "press return to continue." << endl;
    cin.get(c);
    abort();

}
#endif
;
DPRINTF(RubySlicc, "MI_example-cache.sm:313: %s\n", (*m_cache_entry_ptr).m_DataBlk);
(((*m_sequencer_ptr)).readCallback(addr, GenericMachineType_L1Cache, (*m_cache_entry_ptr).m_DataBlk));

}

/** \brief External load completed. */
void
L1Cache_Controller::rx_load_hit(L1Cache_TBE*& m_tbe_ptr, L1Cache_Entry*& m_cache_entry_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing rx_load_hit\n");
    {
    // Declare message
    const ResponseMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const ResponseMsg *>(((*m_L1Cache_responseToCache_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
#ifndef NDEBUG
if (!((m_cache_entry_ptr != NULL))) {
    char c;
    cerr << "Runtime Error at MI_example-cache.sm:321, Ruby Time: "
         << g_system_ptr->getTime() << ": "
         << "assert failure"
         << ", PID: " << getpid() << endl
         << "press return to continue." << endl;
    cin.get(c);
    abort();

}
#endif
;
DPRINTF(RubySlicc, "MI_example-cache.sm:322: %s\n", (*m_cache_entry_ptr).m_DataBlk);
(((*m_sequencer_ptr)).readCallback(addr, (getNondirectHitMachType(((*in_msg_ptr)).m_Sender)), (*m_cache_entry_ptr).m_DataBlk));
}

}

/** \brief Notify sequencer that store completed. */
void
L1Cache_Controller::s_store_hit(L1Cache_TBE*& m_tbe_ptr, L1Cache_Entry*& m_cache_entry_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing s_store_hit\n");
    #ifndef NDEBUG
if (!((m_cache_entry_ptr != NULL))) {
    char c;
    cerr << "Runtime Error at MI_example-cache.sm:330, Ruby Time: "
         << g_system_ptr->getTime() << ": "
         << "assert failure"
         << ", PID: " << getpid() << endl
         << "press return to continue." << endl;
    cin.get(c);
    abort();

}
#endif
;
DPRINTF(RubySlicc, "MI_example-cache.sm:331: %s\n", (*m_cache_entry_ptr).m_DataBlk);
(((*m_sequencer_ptr)).writeCallback(addr, GenericMachineType_L1Cache, (*m_cache_entry_ptr).m_DataBlk));

}

/** \brief External store completed. */
void
L1Cache_Controller::sx_store_hit(L1Cache_TBE*& m_tbe_ptr, L1Cache_Entry*& m_cache_entry_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing sx_store_hit\n");
    {
    // Declare message
    const ResponseMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const ResponseMsg *>(((*m_L1Cache_responseToCache_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
#ifndef NDEBUG
if (!((m_cache_entry_ptr != NULL))) {
    char c;
    cerr << "Runtime Error at MI_example-cache.sm:339, Ruby Time: "
         << g_system_ptr->getTime() << ": "
         << "assert failure"
         << ", PID: " << getpid() << endl
         << "press return to continue." << endl;
    cin.get(c);
    abort();

}
#endif
;
DPRINTF(RubySlicc, "MI_example-cache.sm:340: %s\n", (*m_cache_entry_ptr).m_DataBlk);
(((*m_sequencer_ptr)).writeCallback(addr, (getNondirectHitMachType(((*in_msg_ptr)).m_Sender)), (*m_cache_entry_ptr).m_DataBlk));
}

}

/** \brief Write data to the cache */
void
L1Cache_Controller::u_writeDataToCache(L1Cache_TBE*& m_tbe_ptr, L1Cache_Entry*& m_cache_entry_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing u_writeDataToCache\n");
    {
    // Declare message
    const ResponseMsg* in_msg_ptr M5_VAR_USED;
    in_msg_ptr = dynamic_cast<const ResponseMsg *>(((*m_L1Cache_responseToCache_ptr)).peek());
    assert(in_msg_ptr != NULL); // Check the cast result
#ifndef NDEBUG
if (!((m_cache_entry_ptr != NULL))) {
    char c;
    cerr << "Runtime Error at MI_example-cache.sm:349, Ruby Time: "
         << g_system_ptr->getTime() << ": "
         << "assert failure"
         << ", PID: " << getpid() << endl
         << "press return to continue." << endl;
    cin.get(c);
    abort();

}
#endif
;
(*m_cache_entry_ptr).m_DataBlk = ((*in_msg_ptr)).m_DataBlk;
}

}

/** \brief sends eviction information to the processor */
void
L1Cache_Controller::forward_eviction_to_cpu(L1Cache_TBE*& m_tbe_ptr, L1Cache_Entry*& m_cache_entry_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing forward_eviction_to_cpu\n");
        if (m_send_evictions) {
        DPRINTF(RubySlicc, "MI_example-cache.sm:356: Sending invalidation for %s to the CPU\n", addr);
        (((*m_sequencer_ptr)).evictionCallback(addr));
    }

}

/** \brief Allocate TBE */
void
L1Cache_Controller::v_allocateTBE(L1Cache_TBE*& m_tbe_ptr, L1Cache_Entry*& m_cache_entry_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing v_allocateTBE\n");
    (((*m_L1Cache_TBEs_ptr)).allocate(addr));
set_tbe(m_tbe_ptr, (((*m_L1Cache_TBEs_ptr)).lookup(addr)));;

}

/** \brief Deallocate TBE */
void
L1Cache_Controller::w_deallocateTBE(L1Cache_TBE*& m_tbe_ptr, L1Cache_Entry*& m_cache_entry_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing w_deallocateTBE\n");
    (((*m_L1Cache_TBEs_ptr)).deallocate(addr));
unset_tbe(m_tbe_ptr);;

}

/** \brief Copy data from cache to TBE */
void
L1Cache_Controller::x_copyDataFromCacheToTBE(L1Cache_TBE*& m_tbe_ptr, L1Cache_Entry*& m_cache_entry_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing x_copyDataFromCacheToTBE\n");
    #ifndef NDEBUG
if (!((m_cache_entry_ptr != NULL))) {
    char c;
    cerr << "Runtime Error at MI_example-cache.sm:372, Ruby Time: "
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
if (!((m_tbe_ptr != NULL))) {
    char c;
    cerr << "Runtime Error at MI_example-cache.sm:373, Ruby Time: "
         << g_system_ptr->getTime() << ": "
         << "assert failure"
         << ", PID: " << getpid() << endl
         << "press return to continue." << endl;
    cin.get(c);
    abort();

}
#endif
;
(*m_tbe_ptr).m_DataBlk = (*m_cache_entry_ptr).m_DataBlk;

}

/** \brief stall */
void
L1Cache_Controller::z_stall(L1Cache_TBE*& m_tbe_ptr, L1Cache_Entry*& m_cache_entry_ptr, const Address& addr)
{
    DPRINTF(RubyGenerated, "executing z_stall\n");
    
}

L1Cache_Entry*
L1Cache_Controller::getCacheEntry(const Address& param_address)
{
return static_cast<L1Cache_Entry *>((((*m_cacheMemory_ptr)).lookup(param_address)));

}
L1Cache_Event
L1Cache_Controller::mandatory_request_type_to_event(const RubyRequestType& param_type)
{
    if ((param_type == RubyRequestType_LD)) {
        return L1Cache_Event_Load;
    } else {
            if ((param_type == RubyRequestType_IFETCH)) {
                return L1Cache_Event_Ifetch;
            } else {
                    if (((param_type == RubyRequestType_ST) || (param_type == RubyRequestType_ATOMIC))) {
                        return L1Cache_Event_Store;
                    } else {
                        char c;
                        cerr << "Runtime Error at MI_example-cache.sm:94, Ruby Time: "
                             << g_system_ptr->getTime() << ": "
                             << ("Invalid RubyRequestType")
                             << ", PID: " << getpid() << endl
                             << "press return to continue." << endl;
                        cin.get(c);
                        abort();
                        ;
                    }
                }
            }

}
L1Cache_State
L1Cache_Controller::getState(L1Cache_TBE* param_tbe, L1Cache_Entry* param_cache_entry, const Address& param_addr)
{
    if ((param_tbe != NULL)) {
        return (*param_tbe).m_TBEState;
    } else {
            if ((param_cache_entry != NULL)) {
                return (*param_cache_entry).m_CacheState;
            } else {
                return L1Cache_State_I;
            }
        }

}
void
L1Cache_Controller::setState(L1Cache_TBE* param_tbe, L1Cache_Entry* param_cache_entry, const Address& param_addr, const L1Cache_State& param_state)
{
    if ((param_tbe != NULL)) {
        (*param_tbe).m_TBEState = param_state;
    }
        if ((param_cache_entry != NULL)) {
            (*param_cache_entry).m_CacheState = param_state;
        }

}
AccessPermission
L1Cache_Controller::getAccessPermission(const Address& param_addr)
{
L1Cache_TBE* tbe
 = (((*m_L1Cache_TBEs_ptr)).lookup(param_addr));
    if ((tbe != NULL)) {
        return (L1Cache_State_to_permission((*tbe).m_TBEState));
    }
    L1Cache_Entry* cache_entry
     = (getCacheEntry(param_addr));
        if ((cache_entry != NULL)) {
            return (L1Cache_State_to_permission((*cache_entry).m_CacheState));
        }
        return AccessPermission_NotPresent;

}
void
L1Cache_Controller::setAccessPermission(L1Cache_Entry* param_cache_entry, const Address& param_addr, const L1Cache_State& param_state)
{
    if ((param_cache_entry != NULL)) {
        ((*(param_cache_entry)).changePermission((L1Cache_State_to_permission(param_state))));
    }

}
DataBlock&
L1Cache_Controller::getDataBlock(const Address& param_addr)
{
return (*(getCacheEntry(param_addr))).m_DataBlk;

}
GenericMachineType
L1Cache_Controller::getNondirectHitMachType(const MachineID& param_sender)
{
    if (((machineIDToMachineType(param_sender)) == MachineType_L1Cache)) {
        return GenericMachineType_L1Cache_wCC;
    } else {
        return (ConvertMachToGenericMach((machineIDToMachineType(param_sender))));
    }

}
