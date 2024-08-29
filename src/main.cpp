//
// Created by os on 6/17/24.
//
#include "../h/riscv.hpp"
#include "../lib/console.h"
#include "../h/syscall_c.hpp"

extern void userMain();

void mainDjerdap(void* args){
    __putc('d');
    __putc('j');
    __putc('e');__putc('r');

    __putc('d');
    __putc('a');
    __putc('p');

}
void mainDjeronimo(void * args){
    __putc('D');
    __putc('J');
    __putc('E');__putc('R');
    __putc('O');
    __putc('N');
    __putc('I');
    __putc('M');
    __putc('O');
}

void userMainWrapper(void * arg){
    userMain();
}
thread_t  t;thread_t usermain;thread_t djerdap;
int main(){


    // Kreirajte instancu MemoryAllocator
    //MemoryAllocator& allocator = MemoryAllocator::getMemAlloc();
    Riscv::w_stvec((uint64) &Riscv::vectorTable & ~3ul);
    //omogucava prekid od tajmera
   // Riscv::ms_sie(Riscv::SIE_SEIE);

    //omoguci prekid od tajmera
    //Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    thread_create(&djerdap, nullptr,nullptr);
    ThreadA::running = djerdap;
    switchUser();
    //thread_create(&t,&mainDjerdap,nullptr);





    thread_create(&usermain, &userMainWrapper, nullptr);

    while(!usermain->isFinished()) {
        thread_dispatch();
    }

    //    void* djerdap = mem_alloc(100);
//    if(djerdap != nullptr) __putc('d');
//
//    int djeronimo = mem_free(djerdap);
//    if(djeronimo == 0)
//    __putc('q');
//    thread_t t;
//    thread_create(&t,&mainDjerdap,nullptr);
//    ThreadA::running = t;
//
//    while(!t->isFinished()){
//    thread_dispatch();
//    }

    //thread_create(&t2,&mainDjeronimo,nullptr);
//


    //thread_t djerdap = Scheduler::idle;
    //idle thread
    //thread_create(&djerdap, reinterpret_cast<void (*)(void *)>(Scheduler::idleJob), nullptr);
    // Scheduler::put(djerdap);



    // Testiranje mem_alloc
//    void* allocatedMemory = allocator.mem_alloc(100); // Pravimo rezervaciju od 100 bajta
//    if (allocatedMemory == nullptr) {
//        __putc('d');
//    } else {
//        __putc('n');
//    }
//
//    // Testiranje mem_free
//    int freeResult = allocator.mem_free(allocatedMemory);
//    if (freeResult == 0) {
//        __putc('n');
//    } else {
//        __putc('d');
//    }

    return 0;
}