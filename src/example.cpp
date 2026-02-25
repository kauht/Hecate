#include "memory/memory.h"

auto main() -> int {
    Memory mem("cs2.exe", "client.dll");
    // or
    Memory mem("cs2");
    
    uintptr_t base = mem.get_base();
}
