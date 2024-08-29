//
// Created by os on 6/19/24.
//

#ifndef PROJECT_BASE_V1_1_SCHEDULER_HPP
#define PROJECT_BASE_V1_1_SCHEDULER_HPP




class ThreadA;

typedef ThreadA* thread_t;

struct Schedule{
    thread_t thread;
    Schedule* next;
};

class Scheduler{

private:

    static Schedule* tail;
    static Schedule* head;

public:
    static Scheduler& getInstanceOfScheduler();

    static thread_t idle;
    static void idleJob(void * arg);

    //uzmi s pocetka
    static ThreadA *get();

    //dodaj na kraj
    static void put(ThreadA *tcb);

};

#endif //PROJECT_BASE_V1_1_SCHEDULER_HPP
