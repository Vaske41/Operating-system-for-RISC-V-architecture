//
// Created by os on 4/27/22.
//

#include "./PCB.hpp"
#ifndef PROJECT_BASE_V1_0_SCHEDULER_H
#define PROJECT_BASE_V1_0_SCHEDULER_H


class Scheduler{
public:
    static PCB* get();
    static void put(PCB* pcb);
private:
    static PCB* head;
    static PCB* tail;
    Scheduler() = default;
};


#endif //PROJECT_BASE_V1_0_SCHEDULER_H
