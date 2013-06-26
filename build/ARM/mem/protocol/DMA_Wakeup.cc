// Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/StateMachine.py:1015
// DMA: DMA Controller

#include <sys/types.h>
#include <unistd.h>

#include <cassert>

#include "base/misc.hh"
#include "debug/RubySlicc.hh"
#include "mem/protocol/DMA_Controller.hh"
#include "mem/protocol/DMA_Event.hh"
#include "mem/protocol/DMA_State.hh"
#include "mem/protocol/Types.hh"
#include "mem/ruby/common/Global.hh"
#include "mem/ruby/slicc_interface/RubySlicc_includes.hh"
#include "mem/ruby/system/System.hh"

using namespace std;

void
DMA_Controller::wakeup()
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
            // DMAInPort dmaRequestQueue_in
            m_cur_in_port_rank = 0;
                        if ((((*m_DMA_mandatoryQueue_ptr)).isReady())) {
                            {
                                // Declare message
                                const SequencerMsg* in_msg_ptr M5_VAR_USED;
                                in_msg_ptr = dynamic_cast<const SequencerMsg *>(((*m_DMA_mandatoryQueue_ptr)).peek());
                                assert(in_msg_ptr != NULL); // Check the cast result
                                if ((((*in_msg_ptr)).m_Type == SequencerRequestType_LD)) {
                                    {
                                        Address addr = ((*in_msg_ptr)).m_LineAddress;

                                        TransitionResult result = doTransition(DMA_Event_ReadRequest, addr);

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
                                        if ((((*in_msg_ptr)).m_Type == SequencerRequestType_ST)) {
                                            {
                                                Address addr = ((*in_msg_ptr)).m_LineAddress;

                                                TransitionResult result = doTransition(DMA_Event_WriteRequest, addr);

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
                                            cerr << "Runtime Error at MI_example-dma.sm:54, Ruby Time: "
                                                 << g_system_ptr->getTime() << ": "
                                                 << ("Invalid request type")
                                                 << ", PID: " << getpid() << endl
                                                 << "press return to continue." << endl;
                                            cin.get(c);
                                            abort();
                                            ;
                                        }
                                    }
                                    }
                                }
            // DMAInPort dmaResponseQueue_in
            m_cur_in_port_rank = 0;
                        if ((((*m_DMA_responseFromDir_ptr)).isReady())) {
                            {
                                // Declare message
                                const DMAResponseMsg* in_msg_ptr M5_VAR_USED;
                                in_msg_ptr = dynamic_cast<const DMAResponseMsg *>(((*m_DMA_responseFromDir_ptr)).peek());
                                assert(in_msg_ptr != NULL); // Check the cast result
                                if ((((*in_msg_ptr)).m_Type == DMAResponseType_ACK)) {
                                    {
                                        Address addr = ((*in_msg_ptr)).m_LineAddress;

                                        TransitionResult result = doTransition(DMA_Event_Ack, addr);

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
                                        if ((((*in_msg_ptr)).m_Type == DMAResponseType_DATA)) {
                                            {
                                                Address addr = ((*in_msg_ptr)).m_LineAddress;

                                                TransitionResult result = doTransition(DMA_Event_Data, addr);

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
                                            cerr << "Runtime Error at MI_example-dma.sm:68, Ruby Time: "
                                                 << g_system_ptr->getTime() << ": "
                                                 << ("Invalid response type")
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
