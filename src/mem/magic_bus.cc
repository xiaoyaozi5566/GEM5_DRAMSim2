#include "base/misc.hh"
#include "base/trace.hh"
#include "debug/Bus.hh"
#include "debug/BusAddrRanges.hh"
#include "debug/NoncoherentBus.hh"
#include "mem/noncoherent_bus.hh"
#include "mem/magic_bus.hh"

MagicBus::MagicBus(const MagicBusParams *p)
  : NoncoherentBus(p)
{
}

MagicBus *
MagicBusParams::create(){
  return new MagicBus(this);
}
