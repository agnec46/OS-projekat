//
// Created by os on 6/19/24.
//
#include "../h/Thread.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"


thread_t ThreadA::running = nullptr;

ThreadA* ThreadA::thread_create(void* stack, ThreadA::Body body, void* arg){
    thread_t djerdap = ThreadCreate(stack,body,arg);
    if(body != nullptr) {
        Scheduler::put(djerdap);
    }
    return djerdap;

}

ThreadA* ThreadA::ThreadCreate(void* stack, ThreadA::Body body, void * arguments){
    thread_t djerdap =(thread_t) MemoryAllocator::getMemAlloc().mem_alloc(sizeof(ThreadA)+sizeof (FreeMem)+(MEM_BLOCK_SIZE-sizeof(ThreadA)-sizeof (FreeMem))%MEM_BLOCK_SIZE);
    djerdap->setBody(body);
    djerdap->stack = stack;
    djerdap->arguments = arguments;
    djerdap->context.ra = (uint64) &threadWrapper;
    djerdap->context.sp = djerdap->body ? (uint64) djerdap->stack + DEFAULT_STACK_SIZE : 0;
    djerdap->finished = false;
    djerdap->blocked = false;
    return djerdap;
}

int ThreadA::exit(){
    if(!ThreadA::running->isFinished()){
        ThreadA::running->setFinished(true);
    }
    //MemoryAllocator::getMemAlloc().mem_free(ThreadA::running);
    thread_dispatch();
    return 0;
}

void ThreadA::threadWrapper(){
    //Riscv::popSppSpie();
    switchUser();
    running->body(running->arguments);
    thread_exit();
}

void ThreadA::dispatch(){

    thread_t old = running;
    if(!old->isFinished() && !old->isBlocked()){
        Scheduler::put(old);
    }
    running = Scheduler::get();
    contextSwitch(&old->context,&running->context);
}
