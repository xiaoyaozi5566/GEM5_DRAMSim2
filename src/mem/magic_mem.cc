#include "base/random.hh"
#include "mem/simple_mem.hh"
#include "mem/magic_mem.hh"

MagicMemory::MagicMemory(const Params* p):
  SimpleMemory(p)
{
  this->port = new MagicMemoryPort(name() + ".port", *this);
}

MagicMemory::MagicMemoryPort::MagicMemoryPort(const std::string& _name,
    SimpleMemory& _memory) :
  SimpleMemory::MemoryPort(_name, _memory)
{ }

MagicMemory*
MagicMemoryParams::create()
{
  return new MagicMemory(this);
}
