//
// Created by os on 5/6/24.
//

#include "../h/MemoryAllocator.h"

MemoryAllocator::MemoryAllocator() {
    fmHead = (FreeMem*) ((uint64) HEAP_START_ADDR);
    fmHead->next = nullptr;
    fmHead->prev = nullptr;
    fmHead->size = (uint64) ((uint64) HEAP_END_ADDR - (uint64) HEAP_START_ADDR - sizeof(MemoryAllocator));
}

MemoryAllocator& MemoryAllocator::getMemAlloc() {
    static MemoryAllocator djerdap;
    return djerdap;
}

void* MemoryAllocator::mem_alloc(size_t size) {
    if(size <= 0) return nullptr;

    size_t newSize = size;

    FreeMem* cur = fmHead;
    FreeMem* prev = nullptr;
    for(;cur;prev = cur,cur=cur->next){
        if(cur->size < newSize){
            continue;
        }
        else{
            void* addr = cur;
            size_t remaining = cur->size-newSize;
            if(remaining > 0){
                FreeMem* newFreeMem = (FreeMem*) ((uint64) cur + newSize);
                newFreeMem->prev = prev;
                newFreeMem->next = cur->next;
                newFreeMem->size = remaining;
                if(prev){
                    prev->next = newFreeMem;
                }
                else{
                    fmHead = newFreeMem;
                }
                if(cur->next){
                    cur->next->prev = newFreeMem;
                }
            }
            else{
                if(prev){
                    prev->next = cur->next;
                }
                else{
                    fmHead = cur->next;
                }
                if(cur->next){
                    cur->next->prev = prev;
                }
            }
            cur->size = newSize;
            return (char*) addr + sizeof(FreeMem);
        }
    }
    return nullptr;
}

int MemoryAllocator::mem_free(void *addr) {

    if(!(uint64*)addr || (uint64*)addr > (uint64*)HEAP_END_ADDR || (uint64*)addr < (uint64*)HEAP_START_ADDR) return -1;

    FreeMem* djerdap = (FreeMem*)((uint64) addr - sizeof(FreeMem));


    if(fmHead == nullptr){
        djerdap->next = nullptr;
        djerdap->prev = nullptr;
        fmHead = djerdap;
        return 0;
    }
    FreeMem* cur = fmHead;
    for(;cur;cur=cur->next){
        if(cur < djerdap && cur->next == nullptr) {
            djerdap->next = nullptr;
            djerdap->prev = cur;
            cur->next = djerdap;
            if((size_t) cur + cur->size == (size_t) djerdap){
                cur->next = djerdap->next;
                cur->size += djerdap->size;
            }
            break;
        }
        else if(cur < djerdap) continue;
        else {
            if(cur->prev == nullptr) {
                fmHead = djerdap;
                djerdap->next = cur;
                djerdap->prev = nullptr;
                cur->prev = djerdap;
                if((size_t) cur == (size_t) djerdap + djerdap->size){
                    djerdap->size += cur->size;
                    djerdap->next = cur->next;
                    if(cur->next)
                    cur->next->prev = djerdap;

                }
            }
            else{
                djerdap->next = cur;
                djerdap->prev = cur->prev;
                cur->prev->next = djerdap;
                cur->prev = djerdap;
                if((size_t) cur == (size_t) djerdap + djerdap->size){
                    djerdap->size += cur->size;
                    djerdap->next = cur->next;
                    if(cur->next)
                        cur->next->prev = djerdap;
                }
                if((size_t) djerdap->prev + djerdap->prev->size == (size_t) djerdap){
                    djerdap->prev->next = djerdap->next;
                    djerdap->prev->size += djerdap->size;
                    if(djerdap->next)
                        djerdap->next->prev = djerdap->prev;
                }
            }

            break;
        }
    }
    return 0;
}

