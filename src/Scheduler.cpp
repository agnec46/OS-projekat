//
// Created by os on 6/19/24.
//
#include "../h/Scheduler.hpp"
#include "../h/MemoryAllocator.h"
#include "../h/Thread.hpp"


Schedule* Scheduler::head = nullptr;
Schedule* Scheduler::tail = nullptr;

Scheduler& Scheduler::getInstanceOfScheduler(){
    static Scheduler djerdap;
    return djerdap;
}

ThreadA* Scheduler::get(){
    if(!head) return nullptr;
    Schedule* temp = head;
    head = head->next;
    if(!head) tail = nullptr;
    temp->next = nullptr;
    thread_t djerdap = temp->thread;
    MemoryAllocator::getMemAlloc().mem_free(temp);
    return djerdap;
}

void Scheduler::put(ThreadA *thread){
    if(!thread) return;
    Schedule* elem = (Schedule*) MemoryAllocator::getMemAlloc().mem_alloc(sizeof(Scheduler)+(MEM_BLOCK_SIZE-sizeof(Scheduler))%MEM_BLOCK_SIZE);
    elem->thread = thread;
    elem->next = nullptr;

    if(!head) {
        head = elem;
        //ThreadA::running = elem->thread;
    }
    else
        tail->next = elem;

    tail = elem;
}

void Scheduler::idleJob(void *arg) {

    while(true){
        ThreadA::dispatch();
    }
}