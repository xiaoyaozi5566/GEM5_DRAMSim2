#ifndef __MEM_RRBUS_HH__
#define __MEM_RRBUS_HH__

#include <list>
#include <set>
#include <vector>

#include "base/range.hh"
#include "base/range_map.hh"
#include "base/types.hh"
#include "mem/mem_object.hh"
#include "params/RRBus.hh"

class RRBus : public MemObject
{

  protected:

    template <typename PortClass>
    class Layer
    {

      public:

        /**
         * Create a bus layer and give it a name. The bus layer uses
         * the bus an event manager.
         *
         * @param _bus the bus this layer belongs to
         * @param _name the layer's name
         * @param _clock clock period in ticks
         */
        Layer(RRBus& _bus, const std::string& _name, Tick _clock);

        /**
         * Drain according to the normal semantics, so that the bus
         * can tell the layer to drain, and pass an event to signal
         * back when drained.
         *
         * @param de drain event to call once drained
         *
         * @return 1 if busy or waiting to retry, or 0 if idle
         */
        unsigned int drain(Event *de);

        /**
         * Get the bus layer's name
         */
        const std::string name() const { return bus.name() + _name; }


        /**
         * Determine if the bus layer accepts a packet from a specific
         * port. If not, the port in question is also added to the
         * retry list. In either case the state of the layer is updated
         * accordingly.
         *
         * @param port Source port resenting the packet
         *
         * @return True if the bus layer accepts the packet
         */
        bool tryTiming(PortClass* port, int threadID);

        /**
         * Deal with a destination port accepting a packet by potentially
         * removing the source port from the retry list (if retrying) and
         * occupying the bus layer accordingly.
         *
         * @param busy_time Time to spend as a result of a successful send
         */
        void succeededTiming(Tick busy_time, int threadID);

        /**
         * Deal with a destination port not accepting a packet by
         * potentially adding the source port to the retry list (if
         * not already at the front) and occupying the bus layer
         * accordingly.
         *
         * @param busy_time Time to spend as a result of a failed send
         */
        void failedTiming(PortClass* port, Tick busy_time, int threadID);

        /** Occupy the bus layer until until */
        void occupyLayer(Tick until, int threadID);

        /**
         * Send a retry to the port at the head of the retryList. The
         * caller must ensure that the list is not empty.
         */
        void retryWaiting();

        /**
         * Handler a retry from a neighbouring module. Eventually this
         * should be all encapsulated in the bus. This wraps
         * retryWaiting by verifying that there are ports waiting
         * before calling retryWaiting.
         */
        void recvRetry(int threadID);

      private:

        /** The bus this layer is a part of. */
        RRBus& bus;

        /** A name for this layer. */
        std::string _name;

        /**
         * We declare an enum to track the state of the bus layer. The
         * starting point is an idle state where the bus layer is
         * waiting for a packet to arrive. Upon arrival, the bus layer
         * transitions to the busy state, where it remains either
         * until the packet transfer is done, or the header time is
         * spent. Once the bus layer leaves the busy state, it can
         * either go back to idle, if no packets have arrived while it
         * was busy, or the bus layer goes on to retry the first port
         * on the retryList. A similar transition takes place from
         * idle to retry if the bus layer receives a retry from one of
         * its connected ports. The retry state lasts until the port
         * in questions calls sendTiming and returns control to the
         * bus layer, or goes to a busy state if the port does not
         * immediately react to the retry by calling sendTiming.
         */
        enum State { IDLE, BUSY, RETRY };

        /** track the state of the bus layer */
        State * state;

		/** number of security domains */
		int num_pids;
		
		/** the clock speed for the bus layer */
        Tick clock;

        /** event for signalling when drained */
        Event * drainEvent;

        /**
         * An array of ports that retry should be called
         * on because the original send failed for whatever reason.
         */
        //std::vector<std::list<PortClass*> > retryList;
		std::list<PortClass*> * retryList;

        /**
         * Release the bus layer after being occupied and return to an
         * idle state where we proceed to send a retry to any
         * potential waiting port, or drain if asked to do so.
         */
        void releaseLayer();

        /** event used to schedule a release of the layer */
        EventWrapper<Layer, &Layer::releaseLayer> ** releaseEvent;
		//EventWrapper<Layer, &Layer::releaseLayer> releaseEvent1;
		EventWrapper<Layer, &Layer::retryWaiting> ** retryEvent;
		//EventWrapper<Layer, &Layer::retryWaiting> retryEvent1;

    };

	/** number of security domains */
	int num_pids;
	/** cycles of overhead per transaction */
    int headerCycles;
    /** the width of the bus in bytes */
    int width;

    typedef range_map<Addr, PortID>::iterator PortMapIter;
    typedef range_map<Addr, PortID>::const_iterator PortMapConstIter;
    range_map<Addr, PortID> portMap;

    AddrRangeList defaultRange;

    /**
     * Function called by the port when the bus is recieving a range change.
     *
     * @param master_port_id id of the port that received the change
     */
    void recvRangeChange(PortID master_port_id);

    /** Find which port connected to this bus (if any) should be given a packet
     * with this address.
     * @param addr Address to find port for.
     * @return id of port that the packet should be sent out of.
     */
    PortID findPort(Addr addr);

    // Cache for the findPort function storing recently used ports from portMap
    struct PortCache {
        bool valid;
        PortID id;
        Addr start;
        Addr end;
    };

    PortCache portCache[3];

    // Checks the cache and returns the id of the port that has the requested
    // address within its range
    inline PortID checkPortCache(Addr addr) {
        if (portCache[0].valid && addr >= portCache[0].start &&
            addr < portCache[0].end) {
            return portCache[0].id;
        }
        if (portCache[1].valid && addr >= portCache[1].start &&
                   addr < portCache[1].end) {
            return portCache[1].id;
        }
        if (portCache[2].valid && addr >= portCache[2].start &&
            addr < portCache[2].end) {
            return portCache[2].id;
        }

        return InvalidPortID;
    }

    // Clears the earliest entry of the cache and inserts a new port entry
    inline void updatePortCache(short id, Addr start, Addr end) {
        portCache[2].valid = portCache[1].valid;
        portCache[2].id    = portCache[1].id;
        portCache[2].start = portCache[1].start;
        portCache[2].end   = portCache[1].end;

        portCache[1].valid = portCache[0].valid;
        portCache[1].id    = portCache[0].id;
        portCache[1].start = portCache[0].start;
        portCache[1].end   = portCache[0].end;

        portCache[0].valid = true;
        portCache[0].id    = id;
        portCache[0].start = start;
        portCache[0].end   = end;
    }

    // Clears the cache. Needs to be called in constructor.
    inline void clearPortCache() {
        portCache[2].valid = false;
        portCache[1].valid = false;
        portCache[0].valid = false;
    }

    /**
     * Return the address ranges the bus is responsible for.
     *
     * @return a list of non-overlapping address ranges
     */
    AddrRangeList getAddrRanges() const;

    /** Calculate the timing parameters for the packet.  Updates the
     * firstWordTime and finishTime fields of the packet object.
     * Returns the tick at which the packet header is completed (which
     * will be all that is sent if the target rejects the packet).
     */
    Tick calcPacketTiming(PacketPtr pkt, int threadID);

    /**
     * Ask everyone on the bus what their size is
     *
     * @return the max of all the sizes
     */
    unsigned findBlockSize();

    std::set<PortID> inRecvRangeChange;

    /** The master and slave ports of the bus */
    std::vector<SlavePort*> slavePorts;
    std::vector<MasterPort*> masterPorts;

    /** Convenience typedefs. */
    typedef std::vector<SlavePort*>::iterator SlavePortIter;
    typedef std::vector<MasterPort*>::iterator MasterPortIter;
    typedef std::vector<SlavePort*>::const_iterator SlavePortConstIter;
    typedef std::vector<MasterPort*>::const_iterator MasterPortConstIter;

    /** Port that handles requests that don't match any of the interfaces.*/
    PortID defaultPortID;

    /** If true, use address range provided by default device.  Any
       address not handled by another port and not in default device's
       range will cause a fatal error.  If false, just send all
       addresses not handled by another port to default device. */
    bool useDefaultRange;

    unsigned defaultBlockSize;
    unsigned cachedBlockSize;
    bool cachedBlockSizeValid;

    RRBus(const RRBusParams *p);

    virtual ~RRBus();

  public:

    /** A function used to return the port associated with this bus object. */
    virtual MasterPort& getMasterPort(const std::string& if_name, int idx = -1);
    virtual SlavePort& getSlavePort(const std::string& if_name, int idx = -1);

    virtual unsigned int drain(Event *de) = 0;

};

#endif