
#ifndef __MEM_MAGIC_BUS_HH__
#define __MEM_MAGIC_BUS_HH__

#include "mem/bus.hh"
#include "params/MagicBus.hh"
#include "mem/noncoherent_bus.hh"
class MagicBus : public NoncoherentBus
{
  public:

  MagicBus(const MagicBusParams *p);

  virtual bool recvTimingReq( PacketPtr pkt, PortID slave_port_id ){
    recvFunctional( pkt, slave_port_id );
    return true;
  }

  virtual bool recvTimingResp( PacketPtr pkt, PortID master_port_id ){
    recvFunctional( pkt, master_port_id );
    return true;
  }

};

#endif
