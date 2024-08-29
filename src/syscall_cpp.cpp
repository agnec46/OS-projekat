//
// Created by os on 6/24/24.
//
#include "../h/syscall_cpp.hpp"

void* operator new (size_t size){
    return mem_alloc(size);
}
void  operator delete (void* djerdap){
    mem_free(djerdap);
}

Thread::Thread (void (*body)(void*), void* arg){
    this->body = body;
    this->arg = arg;
}
Thread::~Thread () {
    if(myHandle)
        delete(myHandle);
}
int Thread::start (){
    return thread_create(&myHandle,&tStart,this);
}
void Thread::dispatch (){
    thread_dispatch();
}
int Thread::sleep (time_t){
    return 0;
}

Thread::Thread (){
    this->body = nullptr;
    this->arg = nullptr;
}

void Thread::tStart(void* arg){
    Thread* t = (Thread*) arg;
    if(t->body){
        t->body(t->arg);
    } else {
        t->run();
    }
};

Semaphore::Semaphore(unsigned init){
    sem_open(&myHandle,init);
}
Semaphore::~Semaphore (){
    if(myHandle)
        delete(myHandle);
}
int Semaphore::wait (){
    return sem_wait(myHandle);
}

int Semaphore::signal (){
    return sem_signal(myHandle);
}
int Semaphore::timedWait (time_t){
    return 0;
}
//int Semaphore::tryWait(){
//    return trywait(myHandle);
//}

char Console::getc (){
    return ::getc();
}
void Console::putc (char c){
    ::putc(c);
}