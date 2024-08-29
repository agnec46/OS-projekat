//
// Created by os on 5/6/24.
//

#ifndef PROJECT_BASE_V1_1_MEMORYALLOCATOR_H
#define PROJECT_BASE_V1_1_MEMORYALLOCATOR_H

#include "../lib/hw.h"

struct FreeMem{
    size_t size;
    FreeMem* next;
    FreeMem* prev;
};

class MemoryAllocator{

public:
    void* mem_alloc(size_t size);

    int mem_free (void* addr);

    static MemoryAllocator& getMemAlloc();

private:
    FreeMem* fmHead = nullptr;

    explicit MemoryAllocator();


};
#endif //PROJECT_BASE_V1_1_MEMORYALLOCATOR_H

