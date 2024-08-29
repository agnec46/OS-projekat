//
// Created by os on 6/17/24.
//
#include "../h/syscall_c.hpp"

void* mem_alloc (size_t size){

    uint64  sifra = 0x01;
    uint64 newSize = (size + MEM_BLOCK_SIZE -1 + sizeof(FreeMem)) / MEM_BLOCK_SIZE;
    __asm__ volatile ("mv a1, %[newSize]" : : [newSize]"r" (newSize));
    __asm__ volatile ("mv a0, %[sifra]" : : [sifra]"r" (sifra));
    __asm__ volatile ("ecall");
    uint64 res;
    __asm__ volatile ("mv %[res], a0" :  [res]"=r" (res));
    return (void*) res;
}

int mem_free (void* addr){
    uint64 sifra = 0x02;
    void* djerdap = addr;
    __asm__ volatile ("mv a1, %[addr]" : : [addr]"r" (djerdap));
    __asm__ volatile ("mv a0, %[sifra]" : : [sifra]"r" (sifra));
    __asm__ volatile ("ecall");
    uint64  res;
    __asm__ volatile ("mv %[res], a0" : [res]"=r" (res));
    return res;
}

int thread_create ( thread_t* handle, void(*start_routine)(void*), void* arg){
    uint64 sifra = 0x11;
    void* stack = nullptr;
    if(start_routine) {
        stack = mem_alloc(DEFAULT_STACK_SIZE);
        if(stack == nullptr) return -1;
    }
    __asm__ volatile("mv a4, %[stack]" : : [stack]"r" (stack));
    __asm__ volatile("mv a3, %[arg]" : : [arg]"r" (arg));
    __asm__ volatile("mv a2, %[start_routine]" : : [start_routine]"r" (start_routine));
    __asm__ volatile("mv a1, %[handle]" : : [handle]"r" (handle));
    __asm__ volatile ("mv a0, %[sifra]" : : [sifra]"r" (sifra));
    __asm__ volatile("ecall");
    uint64 res;
    __asm__ volatile ("mv %[res], a0" : [res]"=r" (res));
    return res;
}

int thread_exit (){
    uint64 sifra = 0x12;
    __asm__ volatile ("mv a0, %[sifra]" : : [sifra]"r" (sifra));
    __asm__ volatile ("ecall");
    uint64 res;
    __asm__ volatile ("mv %[res], a0" : [res]"=r" (res));
    return res;
}

void thread_dispatch (){
    uint64 sifra = 0x13;
    __asm__ volatile ("mv a0, %[sifra]" : : [sifra]"r" (sifra));
    __asm__ volatile ("ecall");
}

int sem_open(sem_t* handle,unsigned init){
    uint64 sifra = 0x21;
    __asm__ volatile("mv a2, %[init]" : : [init]"r" (init));
    __asm__ volatile("mv a1, %[handle]" : : [handle]"r" (handle));
    __asm__ volatile ("mv a0, %[sifra]" : : [sifra]"r" (sifra));
    __asm__ volatile("ecall");
    uint64 res;
    __asm__ volatile ("mv %[res], a0" : [res]"=r" (res));
    return res;
}

int sem_close (sem_t handle){
    uint64 sifra = 0x22;
    __asm__ volatile("mv a1, %[handle]" : : [handle]"r" (handle));
    __asm__ volatile ("mv a0, %[sifra]" : : [sifra]"r" (sifra));
    __asm__ volatile ("ecall");
    uint64 res;
    __asm__ volatile ("mv %[res], a0" : [res]"=r" (res));
    return res;
}

int sem_wait(sem_t id){
    uint64 sifra = 0x23;
    __asm__ volatile("mv a1, %[id]" : : [id]"r" (id));
    __asm__ volatile ("mv a0, %[sifra]" : : [sifra]"r" (sifra));
    __asm__ volatile ("ecall");
    uint64 res;
    __asm__ volatile ("mv %[res], a0" : [res]"=r" (res));
    return res;
}

int sem_signal(sem_t id){
    uint64 sifra = 0x24;
    __asm__ volatile("mv a1, %[id]" : : [id]"r" (id));
    __asm__ volatile ("mv a0, %[sifra]" : : [sifra]"r" (sifra));
    __asm__ volatile ("ecall");
    uint64 res;
    __asm__ volatile ("mv %[res], a0" : [res]"=r" (res));
    return res;
}

int sem_timedwait(sem_t id,time_t timeout){
//    uint64 sifra = 0x25;
//    __asm__ volatile("mv a2, %[timeout]" : : [timeout]"r" (timeout));
//    __asm__ volatile("mv a1, %[id]" : : [id]"r" (id));
//    __asm__ volatile ("mv a0, %[sifra]" : : [sifra]"r" (sifra));
//    __asm__ volatile ("ecall");
//    uint64 res;
//    __asm__ volatile ("mv %[res], a0" : [res]"=r" (res));
//    return res;
    return 0;
}

int sem_trywait(sem_t id){
    uint64 sifra = 0x26;
    __asm__ volatile("mv a1, %[id]" : : [id]"r" (id));
    __asm__ volatile ("mv a0, %[sifra]" : : [sifra]"r" (sifra));
    __asm__ volatile ("ecall");
    uint64 res;
    __asm__ volatile ("mv %[res], a0" : [res]"=r" (res));
    return res;
}

int time_sleep (time_t t){
    return 0;
}

char getc (){
    uint64 oper = 0x41;
    __asm__ volatile ("mv a0, %[oper]" : : [oper]"r" (oper));
    __asm__ volatile ("ecall");
    char res;
    __asm__ volatile ("mv %[res], a0" : [res]"=r" (res));
    return res;
}
void putc (char ch){
    char c = ch;
    uint64 oper = 0x42;
    __asm__ volatile ("mv a1, %[c]" : : [c] "r" (c));
    __asm__ volatile ("mv a0, %[oper]" : : [oper]"r" (oper));
    __asm__ volatile ("ecall");
}

void switchUser(){
    uint64 oper = 0x46;
    __asm__ volatile ("mv a0, %[oper]" : : [oper]"r" (oper));
    __asm__ volatile ("ecall");
}