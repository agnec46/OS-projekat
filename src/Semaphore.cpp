//
// Created by os on 6/18/24.
//
#include "../h/Semaphore.hpp"
#include "../h/riscv.hpp"

sem_t SemaphoreC::semaphore_create(uint64 init){
    sem_t djerdap =(sem_t) MemoryAllocator::getMemAlloc().mem_alloc(sizeof(SemaphoreC)
            +sizeof (FreeMem)+(MEM_BLOCK_SIZE-sizeof(SemaphoreC)-sizeof (FreeMem))%MEM_BLOCK_SIZE);
    if(!djerdap) return nullptr;
    djerdap->value = init;
    djerdap->head = nullptr;
    djerdap->tail = nullptr;
    return djerdap;
}
int SemaphoreC::close(sem_t handle){
    while(handle->head){
        handle->head->thread->setBlocked(false);
        Scheduler::put(handle->head->thread);
        Schedule* temp = handle->head;
        handle->head = handle->head->next;
        MemoryAllocator::getMemAlloc().mem_free(temp);
    }
    handle->tail = nullptr;
    MemoryAllocator::getMemAlloc().mem_free(handle);
    return 0;
}

int SemaphoreC::wait(sem_t handle){
    if(!handle) return -1;
    handle->value--;
    if(handle->value < 0){
        ThreadA::running->setBlocked(true);
        Schedule* elem = (Schedule*) MemoryAllocator::getMemAlloc().mem_alloc(
                sizeof(Scheduler)+(MEM_BLOCK_SIZE-sizeof(Scheduler))%MEM_BLOCK_SIZE);
        elem->thread = ThreadA::running;
        elem->next = nullptr;
        if(!handle->head){
            handle->head = elem;
            handle->tail = elem;
        }
        else{
            handle->tail->next = elem;
            handle->tail = elem;
        }

        ThreadA::dispatch();
    }
    return 0;
}

int SemaphoreC::signal(sem_t handle) {
    if(!handle) return -1;
    handle->value++;
    if(handle->value <= 0){
        Scheduler::put(handle->head->thread);
        handle->head->thread->setBlocked(false);
        Schedule* temp = handle->head;
        handle->head = handle->head->next;
        MemoryAllocator::getMemAlloc().mem_free(temp);
    }
    return 0;
}

int SemaphoreC::trywait(sem_t handle){
    if(!handle) return -1;
    handle->value--;
    if(handle->value < 0) {
        ThreadA::dispatch();
        return -1;
    }
    else if(handle->value == 0) return 0;
    else return 1;
}