#ifndef __ENUM__MemoryMode__
#define __ENUM__MemoryMode__

namespace Enums {
    enum MemoryMode {
        invalid = 0,
        atomic = 1,
        timing = 2,
        Num_MemoryMode = 3
    };
extern const char *MemoryModeStrings[Num_MemoryMode];
}

#endif // __ENUM__MemoryMode__
