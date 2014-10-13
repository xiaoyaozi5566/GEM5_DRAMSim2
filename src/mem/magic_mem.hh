#include "mem/simple_mem.hh"
#include "params/MagicMemory.hh"

class MagicMemory : public SimpleMemory
{
  protected:

  virtual Tick calculateLatency(PacketPtr pkt){
    return 0;
  }

  public:
  MagicMemory(const Params *p);
};
