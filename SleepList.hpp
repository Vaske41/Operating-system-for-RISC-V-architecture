//
// Created by os on 5/9/22.
//
#include "./PCB.hpp"
#ifndef PROJECT_BASE_V1_0_SLEEPLIST_H
#define PROJECT_BASE_V1_0_SLEEPLIST_H


class SleepList {
public:
    static void awake();
    static void put(PCB* pcb);
private:
    static PCB* head;
    SleepList() = default;
};


#endif //PROJECT_BASE_V1_0_SLEEPLIST_H
