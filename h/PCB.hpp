//
// Created by os on 4/27/22.
//

#include "../lib/hw.h"
#include "./MemoryAllocator.hpp"
#include "./syscall_c.hpp"

#ifndef PROJECT_BASE_V1_0_PCB_H
#define PROJECT_BASE_V1_0_PCB_H

using Body = void (*)(void*);

class PCB {
public:
    PCB(Body body,void* arg,void *stack_space);
    static PCB* running;
    static void start ();

    static void sleep();
    static void dispatch ();
    static void suspend();
    static void createThread();
    static void createThreadWithoutStarting();
    static void deleteHandle();
    void* operator new(size_t size);
    void* operator new[](size_t size);
    void operator delete(void*);
    void operator delete[](void*);
    bool isFinished() const{return finished;}
    void setFinished(bool value) {finished=value;}
    uint64 getTimeSlice() const { return timeSlice; }
    ~PCB() { mem_free(stack); }
    static uint64 timeSliceCounter;
    void setUser(bool user) { this->user=user; }

protected:

private:
    static uint64 a4;
    friend class PeriodicThread;
    friend class Riscv;
    friend class Scheduler;
    friend class SleepList;
    friend class kSemaphore;
    static void PCBwrapper();
    struct Context{
        uint64 ra, sp;
    };
    uint64* stack;
    Context context;
    bool finished=false;
    Body body;
    void* arg;
    uint64 timeSlice = DEFAULT_TIME_SLICE;
    static void yield(PCB* oldPCB,PCB* newPCB);
    static void contextSwitch(Context *oldContext, Context *runningContext);
    PCB* schedulerNext = nullptr;
    PCB* sleepNext = nullptr;
    PCB* blockedNext= nullptr;
    uint64 sleepingTime = 0;
    bool semaphoreFault=false;
    bool user = true;
};


#endif //PROJECT_BASE_V1_0_PCB_H
