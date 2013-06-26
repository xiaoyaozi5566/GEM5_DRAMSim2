// Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/StateMachine.py:1015
// Directory: Directory protocol

#include <sys/types.h>
#include <unistd.h>

#include <cassert>

#include "base/misc.hh"
#include "debug/RubySlicc.hh"
#include "mem/protocol/Directory_Controller.hh"
#include "mem/protocol/Directory_Event.hh"
#include "mem/protocol/Directory_State.hh"
#include "mem/protocol/Types.hh"
#include "mem/ruby/common/Global.hh"
#include "mem/ruby/slicc_interface/RubySlicc_includes.hh"
#include "mem/ruby/system/System.hh"

using namespace std;

void
Directory_Controller::wakeup()
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
            // DirectoryInPort dmaRequestQueue_in
            m_cur_in_port_rank = 0;
                        if ((((*m_Directory_dmaRequestToDir_ptr)).isReady())) {
                            {
                                // Declare message
                                const DMARequestMsg* in_msg_ptr M5_VAR_USED;
                                in_msg_ptr = dynamic_cast<const DMARequestMsg *>(((*m_Directory_dmaRequestToDir_ptr)).peek());
                                assert(in_msg_ptr != NULL); // Check the cast result
                            Directory_TBE* tbe
                             = (((*m_Directory_TBEs_ptr)).lookup(((*in_msg_ptr)).m_LineAddress));
                                if ((((*in_msg_ptr)).m_Type == DMARequestType_READ)) {
                                    {
                                        Address addr = ((*in_msg_ptr)).m_LineAddress;

                                        TransitionResult result = doTransition(Directory_Event_DMA_READ, tbe, addr);

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
                                        if ((((*in_msg_ptr)).m_Type == DMARequestType_WRITE)) {
                                            {
                                                Address addr = ((*in_msg_ptr)).m_LineAddress;

                                                TransitionResult result = doTransition(Directory_Event_DMA_WRITE, tbe, addr);

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
                                            cerr << "Runtime Error at MI_example-dir.sm:169, Ruby Time: "
                                                 << g_system_ptr->getTime() << ": "
                                                 << ("Invalid message")
                                                 << ", PID: " << getpid() << endl
                                                 << "press return to continue." << endl;
                                            cin.get(c);
                                            abort();
                                            ;
                                        }
                                    }
                                    }
                                }
            // DirectoryInPort requestQueue_in
            m_cur_in_port_rank = 0;
                        if ((((*m_Directory_requestToDir_ptr)).isReady())) {
                            {
                                // Declare message
                                const RequestMsg* in_msg_ptr M5_VAR_USED;
                                in_msg_ptr = dynamic_cast<const RequestMsg *>(((*m_Directory_requestToDir_ptr)).peek());
                                assert(in_msg_ptr != NULL); // Check the cast result
                            Directory_TBE* tbe
                             = (((*m_Directory_TBEs_ptr)).lookup(((*in_msg_ptr)).m_Address));
                                if ((((*in_msg_ptr)).m_Type == CoherenceRequestType_GETS)) {
                                    {
                                        Address addr = ((*in_msg_ptr)).m_Address;

                                        TransitionResult result = doTransition(Directory_Event_GETS, tbe, addr);

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
                                        if ((((*in_msg_ptr)).m_Type == CoherenceRequestType_GETX)) {
                                            {
                                                Address addr = ((*in_msg_ptr)).m_Address;

                                                TransitionResult result = doTransition(Directory_Event_GETX, tbe, addr);

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
                                                if ((((*in_msg_ptr)).m_Type == CoherenceRequestType_PUTX)) {
                                                        if ((((*(getDirectoryEntry(((*in_msg_ptr)).m_Address))).m_Owner).isElement(((*in_msg_ptr)).m_Requestor))) {
                                                            {
                                                                Address addr = ((*in_msg_ptr)).m_Address;

                                                                TransitionResult result = doTransition(Directory_Event_PUTX, tbe, addr);

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
                                                                Address addr = ((*in_msg_ptr)).m_Address;

                                                                TransitionResult result = doTransition(Directory_Event_PUTX_NotOwner, tbe, addr);

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
                                                    } else {
                                                        char c;
                                                        cerr << "Runtime Error at MI_example-dir.sm:190, Ruby Time: "
                                                             << g_system_ptr->getTime() << ": "
                                                             << ("Invalid message")
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
            // DirectoryInPort memQueue_in
            m_cur_in_port_rank = 0;
                        if ((((*m_memBuffer_ptr)).isReady())) {
                            {
                                // Declare message
                                const MemoryMsg* in_msg_ptr M5_VAR_USED;
                                in_msg_ptr = dynamic_cast<const MemoryMsg *>(((*m_memBuffer_ptr)).peek());
                                assert(in_msg_ptr != NULL); // Check the cast result
                            Directory_TBE* tbe
                             = (((*m_Directory_TBEs_ptr)).lookup(((*in_msg_ptr)).m_Address));
                                if ((((*in_msg_ptr)).m_Type == MemoryRequestType_MEMORY_READ)) {
                                    {
                                        Address addr = ((*in_msg_ptr)).m_Address;

                                        TransitionResult result = doTransition(Directory_Event_Memory_Data, tbe, addr);

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
                                        if ((((*in_msg_ptr)).m_Type == MemoryRequestType_MEMORY_WB)) {
                                            {
                                                Address addr = ((*in_msg_ptr)).m_Address;

                                                TransitionResult result = doTransition(Directory_Event_Memory_Ack, tbe, addr);

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
                                            DPRINTF(RubySlicc, "MI_example-dir.sm:207: %s\n", ((*in_msg_ptr)).m_Type);
                                            char c;
                                            cerr << "Runtime Error at MI_example-dir.sm:208, Ruby Time: "
                                                 << g_system_ptr->getTime() << ": "
                                                 << ("Invalid message")
                                                 << ", PID: " << getpid() << endl
                                                 << "press return to continue." << endl;
                                            cin.get(c);
                                            abort();
                                            ;
                                        }
                                    }
                                    }
                                }
        break;  // If we got this far, we have nothing left todo
    }
}
