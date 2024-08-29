//
// Created by os on 6/19/24.
//

#ifndef PROJECT_BASE_V1_1_THREAD_HPP
#define PROJECT_BASE_V1_1_THREAD_HPP

#include "./Scheduler.hpp"
#include "./MemoryAllocator.h"
#include "../lib/hw.h"


struct Threads{
    thread_t nit;
    Threads* next;
};

class ThreadA{

public:


    static thread_t running;

    using Body = void (*)(void *);


    static ThreadA* thread_create(void* stack, Body body, void* arg);

    static int exit();

    static void dispatch();

    bool isFinished(){
        return finished;
    }

    void setFinished(bool djerdap){
        finished = djerdap;
    }
    void setBody(Body body2){
        body = body2;
    }

    void setBlocked(bool djerdap){
        blocked  = djerdap;
    }

    bool isBlocked(){
        return blocked;
    }

private:

    //constructor
    static ThreadA* ThreadCreate(void* stack_space, Body body, void * arguments);

    struct Context {
        uint64 ra;
        uint64 sp;
    };

    static void contextSwitch(Context* oldContext,Context* newContext);

    static void threadWrapper();

    bool finished;
    Body body;
    void* stack;
    void* arguments;
    Context context;
    bool blocked;
};
#endif //PROJECT_BASE_V1_1_THREAD_HPP
