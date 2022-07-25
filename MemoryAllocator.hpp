//
// Created by os on 4/22/22.
//
#include "../lib/hw.h"
#ifndef PROJECT_BASE_V1_0_MEMORYALLOCATOR_H
#define PROJECT_BASE_V1_0_MEMORYALLOCATOR_H

struct BlockHeader {
    struct BlockHeader* next;
    size_t size;
};
class MemoryAllocator {
public:
    static void allocateMemory();
    static void freeMemory();
    static void* allocateMemory(size_t size);
    static int freeMemory(void* memory);

private:
    MemoryAllocator()= default;
    static BlockHeader* freeMemoryHead,*allocatedMemoryHead;
    static bool initialized;
};


#endif //PROJECT_BASE_V1_0_MEMORYALLOCATOR_H
