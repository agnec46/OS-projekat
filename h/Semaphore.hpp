//
// Created by os on 6/18/24.
//

#ifndef PROJECT_BASE_V1_1_SEMAPHORE_HPP
#define PROJECT_BASE_V1_1_SEMAPHORE_HPP


#include "Thread.hpp"
#include "Scheduler.hpp"

class SemaphoreC;
typedef SemaphoreC* sem_t;

class SemaphoreC{


public:

    static sem_t semaphore_create(uint64 init);

    static int close(sem_t handle);

    static int wait(sem_t handle);

    static int signal(sem_t handle);

    static int trywait(sem_t handle);

private:
    int value;
    Schedule* head;
    Schedule* tail;
    
};


#endif //PROJECT_BASE_V1_1_SEMAPHORE_HPP
