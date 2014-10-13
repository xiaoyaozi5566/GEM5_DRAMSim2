#include "mem/simple_mem.hh"
#include "params/MagicMemory.hh"

class MagicMemory : public SimpleMemory
{
  protected:

  class MagicMemoryPort : public SimpleMemory::MemoryPort
  {

    public:
    MagicMemoryPort( const std::string& _name, SimpleMemory& _memory );

    protected:
    virtual bool recvTimingReq(PacketPtr pkt){
      SimpleMemory::recvFunctional(pkt);
      return true;
    }

  };
    
  public:
  MagicMemory(const Params *p);
};
