#include "base/random.hh"
#include "mem/simple_mem.hh"
#include "mem/magic_mem.hh"

MagicMemory::MagicMemory(const Params* p):
  SimpleMemory(p)
{ }

MagicMemory*
MagicMemoryParams::create()
{
  return new MagicMemory(this);
}
