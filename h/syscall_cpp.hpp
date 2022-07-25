//
// Created by os on 4/12/22.
//
#include "../lib/hw.h"
#include "./PCB.hpp"
#include "./kSemaphore.hpp"
#ifndef PROJECT_BASE_V1_0_SYSCALL_CPP_H
#define PROJECT_BASE_V1_0_SYSCALL_CPP_H


void* operator new (size_t);
void* operator new[] (size_t);
void operator delete (void*);
void operator delete[] (void*);
class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    void start ();
    static void dispatch ();
    static void sleep (time_t);

protected:
    Thread();
    virtual void run () {}
private:
    PCB* myHandle = nullptr;
    static void wrapper(void*);
};

class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    void wait ();
    void signal ();
private:
    kSemaphore* myHandle= nullptr;
};
class PeriodicThread : public Thread {
protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation () {}
private:
    static void wrapper(void*);
};
class Console {
public:
    static char getc ();
    static void putc (char);
};
#endif //PROJECT_BASE_V1_0_SYSCALL_CPP_H
