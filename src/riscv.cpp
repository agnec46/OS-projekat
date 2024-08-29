//
// Created by os on 6/18/24.
//
#include "../h/riscv.hpp"
#include "../lib/console.h"
#include "../h/MemoryAllocator.h"
#include "../h/Thread.hpp"
#include "../h/Semaphore.hpp"

using Body = void (*)(void *);

void Riscv::popSppSpie() {
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap(){

    uint64 volatile a4, a3, a2, a1, a0;
    __asm__ volatile ("mv %[a4], a4" : [a4] "=r" (a4));
    __asm__ volatile ("mv %[a3], a3" : [a3] "=r" (a3));
    __asm__ volatile ("mv %[a2], a2" : [a2] "=r" (a2));
    __asm__ volatile ("mv %[a1], a1" : [a1] "=r" (a1));
    __asm__ volatile ("mv %[a0], a0" : [a0] "=r" (a0));

    uint64 volatile scause = r_scause();

    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL){
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        //mem alloc
        if(a0 == 0x01){
            uint64 size = a1;
            void * res = MemoryAllocator::getMemAlloc().mem_alloc(size*MEM_BLOCK_SIZE);
            __asm__ volatile ("mv a0, %[res]" : : [res]"r" (res));
        }
        //mem free
        else if(a0 == 0x02){
            void* djerdap = (void*) a1;
            int res = MemoryAllocator::getMemAlloc().mem_free(djerdap);
            __asm__ volatile ("mv a0, %[res]" : : [res]"r" (res));
        }
        //thread_create
        else if(a0 == 0x11){
            thread_t* handle = (thread_t* )a1;
            Body body = (Body) a2;
            void* arg = (void*) a3;
            void* stack = (void*) a4;
            *handle = ThreadA::thread_create(stack,body,arg);
            int res = 0;
            if(*handle == nullptr) res = -1;
            __asm__ volatile ("mv a0, %[res]" : : [res]"r" (res));
        }
        //thread_exit
        else if(a0 == 0x12){
            uint64 res = ThreadA::exit();
            __asm__ volatile("mv a0, %[res]" : : [res]"r" (res));
        }
        //thread_dispatch
        else if(a0 == 0x13){
            ThreadA::dispatch();
        }
        //sem open
        else if(a0 == 0x21){
            uint64 init = a2;
            sem_t* handle = (sem_t*) a1;
            *handle = SemaphoreC::semaphore_create(init);
            uint64 res = 0;
            if(!*handle) res = -1;
            __asm__ volatile("mv a0, %[res]" : : [res]"r" (res));
        }
        //sem close
        else if(a0 == 0x22){
            sem_t handle = (sem_t) a1;
            uint64 res = SemaphoreC::close(handle);
            __asm__ volatile("mv a0, %[res]" : : [res]"r" (res));
        }
        //sem wait
        else if(a0 == 0x23){
            sem_t handle = (sem_t) a1;
            uint64 res = SemaphoreC::wait(handle);
            __asm__ volatile("mv a0, %[res]" : : [res]"r" (res));
        }
        //sem signal
        else if(a0 == 0x24){
            sem_t handle = (sem_t) a1;
            uint64 res = SemaphoreC::signal(handle);
            __asm__ volatile("mv a0, %[res]" : : [res]"r" (res));
        }
        //sem try wait
        else if(a0 == 0x26){
            sem_t handle = (sem_t) a1;
            uint64 res = SemaphoreC::trywait(handle);
            __asm__ volatile("mv a0, %[res]" : : [res]"r" (res));
        }
        //getc
        else if(a0 == 0x41){
            char c = __getc();
            __asm__ volatile ("mv a0, %[c]" : : [c]"r" (c));
        }
        //putc
        else if(a0 == 0x42){
            char c = (char)a1;
            __putc(c);
        }
        //switch to user
        else if(a0 == 0x46){
            uint64 mask = 1 << 8;
            uint64 mask1 = 1 << 1;
            mc_sip(mask1);
            w_sstatus(sstatus);
            mc_sstatus(mask);
            w_sepc(sepc);
            return;
        }

        w_sepc(sepc);
        w_sstatus(sstatus);

    }
    else if (scause == 0x8000000000000001UL)
    {
//        uint64 volatile sepc = r_sepc() + 4;
//        uint64 volatile sstatus = r_sstatus();
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
        /* TCB::timeSliceCounter++;
         if (TCB::timeSliceCounter >= TCB::runningThread->getTimeSlice())
         {
             uint64 volatile sepc = r_sepc();
             uint64 volatile sstatus = r_sstatus();
             TCB::timeSliceCounter = 0;
             TCB::dispatch();
             w_sstatus(sstatus);
             w_sepc(sepc);
         }
         w_sstatus(sstatus);
         w_sepc(sepc);*/
    }
    else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    }
    else
    {
        __putc(scause%16+'0');
    }
}