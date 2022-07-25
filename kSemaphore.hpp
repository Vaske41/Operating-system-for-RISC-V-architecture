//
// Created by os on 5/9/22.
//
#include "../lib/hw.h"
#include "./PCB.hpp"
#ifndef PROJECT_BASE_V1_0_KSEMAPHORE_H
#define PROJECT_BASE_V1_0_KSEMAPHORE_H


class kSemaphore {
public:
    kSemaphore (unsigned init=1) : val(init) {}
    static void systemWait();
    static void systemSignal();
    void wait();
    void signal();
    static void close();
    static void open();
    static void deleteHandle();

    void* operator new(size_t size);
    void* operator new[](size_t size);
    void operator delete(void*);
    void operator delete[](void*);
    int value() const { return val; }
protected:
    void block();
    void unblock();
private:
    int val;
    PCB* head= nullptr,*tail= nullptr;
    friend class kConsole;
};


#endif //PROJECT_BASE_V1_0_KSEMAPHORE_H
