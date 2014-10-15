
#ifndef __MEM_MAGIC_BUS_HH__
#define __MEM_MAGIC_BUS_HH__

#include "mem/bus.hh"
#include "params/MagicBus.hh"
#include "mem/noncoherent_bus.hh"
#include "stdio.h"
class MagicBus : public NoncoherentBus
{
  public:

  MagicBus(const MagicBusParams *p);

  virtual Tick calcPacketTiming( PacketPtr pkt ){
    pkt->firstWordTime = nextCycle();
    pkt->finishTime = nextCycle();
    return nextCycle();
  }

  virtual bool recvTimingReq( PacketPtr pkt, PortID slave_port_id ){
    //Set the source port of the packet
    pkt->setSrc(slave_port_id);

    //Set packet finish and first word time to next cycle
    calcPacketTiming(pkt);

    //Send the packet to the master port
    PortID dest_id = findPort( pkt->getAddr() );
    bool success = masterPorts[dest_id]->sendTimingReq(pkt);

    //Explode on failure
    assert( success );
    // Explode on failure in gem5.fast
    if(!success){
      fprintf(stderr,"%s","unmagical recvTimingReq\n");
      exit(1);
    }

    return true;
  }

  virtual bool recvTimingResp( PacketPtr pkt, PortID master_port_id ){
    //Set packet finish and first word time to next cycle
    calcPacketTiming(pkt);

    //Send the packet to the slave port
    bool success = slavePorts[pkt->getDest()]->sendTimingResp(pkt);

    //Explode on failure
    assert( success );
    //Explode on failure in gem5.fast
    if(!success){
      fprintf(stderr,"%s","unmagical recvTimingResp\n");
      exit(1);
    }

    return true;
  }

};

#endif
