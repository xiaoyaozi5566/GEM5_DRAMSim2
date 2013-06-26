// Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/StateMachine.py:1015
// L1Cache: MI Example L1 Cache

#include <sys/types.h>
#include <unistd.h>

#include <cassert>

#include "base/misc.hh"
#include "debug/RubySlicc.hh"
#include "mem/protocol/L1Cache_Controller.hh"
#include "mem/protocol/L1Cache_Event.hh"
#include "mem/protocol/L1Cache_State.hh"
#include "mem/protocol/Types.hh"
#include "mem/ruby/common/Global.hh"
#include "mem/ruby/slicc_interface/RubySlicc_includes.hh"
#include "mem/ruby/system/System.hh"

using namespace std;

void
L1Cache_Controller::wakeup()
{
    int counter = 0;
    while (true) {
        // Some cases will put us into an infinite loop without this limit
        assert(counter <= m_transitions_per_cycle);
        if (counter == m_transitions_per_cycle) {
            // Count how often we are fully utilized
            g_system_ptr->getProfiler()->controllerBusy(m_machineID);

            // Wakeup in another cycle and try again
            scheduleEvent(this, 1);
            break;
        }
            // L1CacheInPort forwardRequestNetwork_in
            m_cur_in_port_rank = 0;
                        if ((((*m_L1Cache_forwardToCache_ptr)).isReady())) {
                            {
                                // Declare message
                                const RequestMsg* in_msg_ptr M5_VAR_USED;
                                in_msg_ptr = dynamic_cast<const RequestMsg *>(((*m_L1Cache_forwardToCache_ptr)).peek());
                                assert(in_msg_ptr != NULL); // Check the cast result
                                if ( (m_is_blocking == true) &&
                                     (m_block_map.count(in_msg_ptr->m_Address) == 1) ) {
                                     if (m_block_map[in_msg_ptr->m_Address] != &(*m_L1Cache_forwardToCache_ptr)) {
                                        (*m_L1Cache_forwardToCache_ptr).delayHead();
                                        continue;
                                     }
                                }
                                        
                            L1Cache_Entry* cache_entry
                             = (getCacheEntry(((*in_msg_ptr)).m_Address));
                            L1Cache_TBE* tbe
                             = (((*m_L1Cache_TBEs_ptr)).lookup(((*in_msg_ptr)).m_Address));
                                if ((((*in_msg_ptr)).m_Type == CoherenceRequestType_GETX)) {
                                    {
                                        Address addr = ((*in_msg_ptr)).m_Address;

                                        TransitionResult result = doTransition(L1Cache_Event_Fwd_GETX, cache_entry, tbe, addr);

                                        if (result == TransitionResult_Valid) {
                                            counter++;
                                            continue; // Check the first port again
                                        }

                                        if (result == TransitionResult_ResourceStall) {
                                            scheduleEvent(this, 1);

                                            // Cannot do anything with this transition, go check next doable transition (mostly likely of next port)
                                        }
                                    }
                                    ;
                                } else {
                                        if ((((*in_msg_ptr)).m_Type == CoherenceRequestType_WB_ACK)) {
                                            {
                                                Address addr = ((*in_msg_ptr)).m_Address;

                                                TransitionResult result = doTransition(L1Cache_Event_Writeback_Ack, cache_entry, tbe, addr);

                                                if (result == TransitionResult_Valid) {
                                                    counter++;
                                                    continue; // Check the first port again
                                                }

                                                if (result == TransitionResult_ResourceStall) {
                                                    scheduleEvent(this, 1);

                                                    // Cannot do anything with this transition, go check next doable transition (mostly likely of next port)
                                                }
                                            }
                                            ;
                                        } else {
                                                if ((((*in_msg_ptr)).m_Type == CoherenceRequestType_WB_NACK)) {
                                                    {
                                                        Address addr = ((*in_msg_ptr)).m_Address;

                                                        TransitionResult result = doTransition(L1Cache_Event_Writeback_Nack, cache_entry, tbe, addr);

                                                        if (result == TransitionResult_Valid) {
                                                            counter++;
                                                            continue; // Check the first port again
                                                        }

                                                        if (result == TransitionResult_ResourceStall) {
                                                            scheduleEvent(this, 1);

                                                            // Cannot do anything with this transition, go check next doable transition (mostly likely of next port)
                                                        }
                                                    }
                                                    ;
                                                } else {
                                                        if ((((*in_msg_ptr)).m_Type == CoherenceRequestType_INV)) {
                                                            {
                                                                Address addr = ((*in_msg_ptr)).m_Address;

                                                                TransitionResult result = doTransition(L1Cache_Event_Inv, cache_entry, tbe, addr);

                                                                if (result == TransitionResult_Valid) {
                                                                    counter++;
                                                                    continue; // Check the first port again
                                                                }

                                                                if (result == TransitionResult_ResourceStall) {
                                                                    scheduleEvent(this, 1);

                                                                    // Cannot do anything with this transition, go check next doable transition (mostly likely of next port)
                                                                }
                                                            }
                                                            ;
                                                        } else {
                                                            char c;
                                                            cerr << "Runtime Error at MI_example-cache.sm:183, Ruby Time: "
                                                                 << g_system_ptr->getTime() << ": "
                                                                 << ("Unexpected message")
                                                                 << ", PID: " << getpid() << endl
                                                                 << "press return to continue." << endl;
                                                            cin.get(c);
                                                            abort();
                                                            ;
                                                        }
                                                    }
                                                }
                                            }
                                            }
                                        }
            // L1CacheInPort responseNetwork_in
            m_cur_in_port_rank = 0;
                        if ((((*m_L1Cache_responseToCache_ptr)).isReady())) {
                            {
                                // Declare message
                                const ResponseMsg* in_msg_ptr M5_VAR_USED;
                                in_msg_ptr = dynamic_cast<const ResponseMsg *>(((*m_L1Cache_responseToCache_ptr)).peek());
                                assert(in_msg_ptr != NULL); // Check the cast result
                                if ( (m_is_blocking == true) &&
                                     (m_block_map.count(in_msg_ptr->m_Address) == 1) ) {
                                     if (m_block_map[in_msg_ptr->m_Address] != &(*m_L1Cache_responseToCache_ptr)) {
                                        (*m_L1Cache_responseToCache_ptr).delayHead();
                                        continue;
                                     }
                                }
                                        
                            L1Cache_Entry* cache_entry
                             = (getCacheEntry(((*in_msg_ptr)).m_Address));
                            L1Cache_TBE* tbe
                             = (((*m_L1Cache_TBEs_ptr)).lookup(((*in_msg_ptr)).m_Address));
                                if ((((*in_msg_ptr)).m_Type == CoherenceResponseType_DATA)) {
                                    {
                                        Address addr = ((*in_msg_ptr)).m_Address;

                                        TransitionResult result = doTransition(L1Cache_Event_Data, cache_entry, tbe, addr);

                                        if (result == TransitionResult_Valid) {
                                            counter++;
                                            continue; // Check the first port again
                                        }

                                        if (result == TransitionResult_ResourceStall) {
                                            scheduleEvent(this, 1);

                                            // Cannot do anything with this transition, go check next doable transition (mostly likely of next port)
                                        }
                                    }
                                    ;
                                } else {
                                    char c;
                                    cerr << "Runtime Error at MI_example-cache.sm:200, Ruby Time: "
                                         << g_system_ptr->getTime() << ": "
                                         << ("Unexpected message")
                                         << ", PID: " << getpid() << endl
                                         << "press return to continue." << endl;
                                    cin.get(c);
                                    abort();
                                    ;
                                }
                                }
                            }
            // L1CacheInPort mandatoryQueue_in
            m_cur_in_port_rank = 0;
                        if ((((*m_L1Cache_mandatoryQueue_ptr)).isReady())) {
                            {
                                // Declare message
                                const RubyRequest* in_msg_ptr M5_VAR_USED;
                                in_msg_ptr = dynamic_cast<const RubyRequest *>(((*m_L1Cache_mandatoryQueue_ptr)).peek());
                                assert(in_msg_ptr != NULL); // Check the cast result
                                if ( (m_is_blocking == true) &&
                                     (m_block_map.count(in_msg_ptr->m_LineAddress) == 1) ) {
                                     if (m_block_map[in_msg_ptr->m_LineAddress] != &(*m_L1Cache_mandatoryQueue_ptr)) {
                                        (*m_L1Cache_mandatoryQueue_ptr).delayHead();
                                        continue;
                                     }
                                }
                                        
                            L1Cache_Entry* cache_entry
                             = (getCacheEntry(((*in_msg_ptr)).m_LineAddress));
                                if (((cache_entry == NULL) && ((((*m_cacheMemory_ptr)).cacheAvail(((*in_msg_ptr)).m_LineAddress)) == (false)))) {
                                    {
                                        Address addr = (((*m_cacheMemory_ptr)).cacheProbe(((*in_msg_ptr)).m_LineAddress));

                                        TransitionResult result = doTransition(L1Cache_Event_Replacement, (getCacheEntry((((*m_cacheMemory_ptr)).cacheProbe(((*in_msg_ptr)).m_LineAddress)))), (((*m_L1Cache_TBEs_ptr)).lookup((((*m_cacheMemory_ptr)).cacheProbe(((*in_msg_ptr)).m_LineAddress)))), addr);

                                        if (result == TransitionResult_Valid) {
                                            counter++;
                                            continue; // Check the first port again
                                        }

                                        if (result == TransitionResult_ResourceStall) {
                                            scheduleEvent(this, 1);

                                            // Cannot do anything with this transition, go check next doable transition (mostly likely of next port)
                                        }
                                    }
                                    ;
                                } else {
                                    {
                                        Address addr = ((*in_msg_ptr)).m_LineAddress;

                                        TransitionResult result = doTransition((mandatory_request_type_to_event(((*in_msg_ptr)).m_Type)), cache_entry, (((*m_L1Cache_TBEs_ptr)).lookup(((*in_msg_ptr)).m_LineAddress)), addr);

                                        if (result == TransitionResult_Valid) {
                                            counter++;
                                            continue; // Check the first port again
                                        }

                                        if (result == TransitionResult_ResourceStall) {
                                            scheduleEvent(this, 1);

                                            // Cannot do anything with this transition, go check next doable transition (mostly likely of next port)
                                        }
                                    }
                                    ;
                                }
                                }
                            }
        break;  // If we got this far, we have nothing left todo
    }
}
