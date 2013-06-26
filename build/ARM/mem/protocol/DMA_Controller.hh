/** \file DMA_Controller.hh
 *
 * Auto generated C++ code started by /Users/yaowang/Desktop/GitHub/GEM5_DRAMSim2/src/mem/slicc/symbols/StateMachine.py:235
 * Created by slicc definition of Module "DMA Controller"
 */

#ifndef __DMA_CONTROLLER_HH__
#define __DMA_CONTROLLER_HH__

#include <iostream>
#include <sstream>
#include <string>

#include "mem/protocol/DMA_ProfileDumper.hh"
#include "mem/protocol/DMA_Profiler.hh"
#include "mem/protocol/TransitionResult.hh"
#include "mem/protocol/Types.hh"
#include "mem/ruby/common/Consumer.hh"
#include "mem/ruby/common/Global.hh"
#include "mem/ruby/slicc_interface/AbstractController.hh"
#include "params/DMA_Controller.hh"
#include "mem/protocol/MessageBuffer.hh"
#include "mem/protocol/DMA_State.hh"
extern std::stringstream DMA_transitionComment;

class DMA_Controller : public AbstractController
{
// the coherence checker needs to call isBlockExclusive() and isBlockShared()
// making the Chip a friend class is an easy way to do this for now

public:
    typedef DMA_ControllerParams Params;
    DMA_Controller(const Params *p);
    static int getNumControllers();
    void init();
    MessageBuffer* getMandatoryQueue() const;
    const int & getVersion() const;
    const std::string toString() const;
    const std::string getName() const;
    void stallBuffer(MessageBuffer* buf, Address addr);
    void wakeUpBuffers(Address addr);
    void wakeUpAllBuffers();
    void initNetworkPtr(Network* net_ptr) { m_net_ptr = net_ptr; }
    void print(std::ostream& out) const;
    void printConfig(std::ostream& out) const;
    void wakeup();
    void printStats(std::ostream& out) const;
    void clearStats();
    void blockOnQueue(Address addr, MessageBuffer* port);
    void unblock(Address addr);
    void recordCacheTrace(int cntrl, CacheRecorder* tr);
    Sequencer* getSequencer() const;

private:
    DMASequencer* m_dma_sequencer_ptr;
    int m_request_latency;
    int m_number_of_TBEs;

    TransitionResult doTransition(DMA_Event event,
                                  const Address& addr);

    TransitionResult doTransitionWorker(DMA_Event event,
                                        DMA_State state,
                                        DMA_State& next_state,
                                        const Address& addr);

    std::string m_name;
    int m_transitions_per_cycle;
    int m_buffer_size;
    int m_recycle_latency;
    std::map<std::string, std::string> m_cfg;
    NodeID m_version;
    Network* m_net_ptr;
    MachineID m_machineID;
    bool m_is_blocking;
    std::map<Address, MessageBuffer*> m_block_map;
    typedef std::vector<MessageBuffer*> MsgVecType;
    typedef std::map< Address, MsgVecType* > WaitingBufType;
    WaitingBufType m_waiting_buffers;
    int m_max_in_port_rank;
    int m_cur_in_port_rank;
    static DMA_ProfileDumper s_profileDumper;
    DMA_Profiler m_profiler;
    static int m_num_controllers;

    // Internal functions
    DMA_State getState(const Address& param_addr);
    void setState(const Address& param_addr, const DMA_State& param_state);
    AccessPermission getAccessPermission(const Address& param_addr);
    void setAccessPermission(const Address& param_addr, const DMA_State& param_state);
    DataBlock& getDataBlock(const Address& param_addr);

    // Actions
    /** \brief Send a DMA read request to memory */
    void s_sendReadRequest(const Address& addr);
    /** \brief Send a DMA write request to memory */
    void s_sendWriteRequest(const Address& addr);
    /** \brief Notify dma controller that write request completed */
    void a_ackCallback(const Address& addr);
    /** \brief Write data to dma sequencer */
    void d_dataCallback(const Address& addr);
    /** \brief Pop request queue */
    void p_popRequestQueue(const Address& addr);
    /** \brief Pop request queue */
    void p_popResponseQueue(const Address& addr);

    // Objects
    MessageBuffer* m_DMA_responseFromDir_ptr;
    MessageBuffer* m_DMA_reqToDirectory_ptr;
    MessageBuffer* m_DMA_mandatoryQueue_ptr;
    DMA_State* m_DMA_cur_state_ptr;
};
#endif // __DMA_CONTROLLER_H__
