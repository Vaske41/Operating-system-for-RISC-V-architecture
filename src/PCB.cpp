//
// Created by os on 4/27/22.
//

#include "../h/PCB.hpp"
#include "../h/Scheduler.hpp"
#include "../h/syscall_c.hpp"
#include "../h/Riscv.hpp"
#include "../h/SleepList.hpp"
PCB* PCB::running=nullptr;
uint64 PCB::timeSliceCounter=0;
uint64 PCB::a4=0;
PCB::PCB(Body body, void *arg, void *stack_space) :
        stack((uint64*)stack_space),
        context({(uint64)(&PCBwrapper),
                 stack_space != nullptr ? (uint64) &((char*)stack_space)[DEFAULT_STACK_SIZE] : 0}),
                 body(body),
                 arg(arg)
{

}

void PCB::dispatch() {
    uint64 volatile sepc = Riscv::r_sepc();
    uint64 volatile sstatus = Riscv::r_sstatus();
    PCB* oldRunning=PCB::running;
    if(!oldRunning->finished)
        Scheduler::put(oldRunning);
    PCB::running=Scheduler::get();
    Riscv::switchMode();
    yield(oldRunning,PCB::running);
    Riscv::w_sstatus(sstatus);
    Riscv::w_sepc(sepc);
}

void PCB::PCBwrapper() {
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    thread_dispatch();
}

void PCB::yield(PCB *oldPCB, PCB *newPCB) {
    contextSwitch(&oldPCB->context,&newPCB->context);
}

void PCB::createThread() {
    PCB** handle;
    Body body;
    void* arg;
    void* stack_space;
    asm volatile("mv %0,a1" : "=r" (handle));
    asm volatile("mv %0,a2" : "=r" (body));
    asm volatile("mv %0,a3" : "=r" (arg));
    stack_space=(void*)PCB::a4;
    *handle = new PCB(body,arg,stack_space);
    if(*handle){
        Scheduler::put(*handle);
        asm volatile("mv a0,%0" : : "r" (0));
    }
    else
        asm volatile("mv a0,%0" : : "r" (-1));
}

void *PCB::operator new(size_t size) {
    return MemoryAllocator::allocateMemory(size);
}

void *PCB::operator new[](size_t size) {
    return MemoryAllocator::allocateMemory(size);
}

void PCB::operator delete(void* space) {
    MemoryAllocator::freeMemory(space);
}

void PCB::operator delete[](void * space) {
    MemoryAllocator::freeMemory(space);
}

void PCB::sleep() {
    uint64 time;
    asm volatile("mv %0,a1" : "=r" (time));
    PCB::running->sleepingTime=time;
    SleepList::put(PCB::running);

    PCB::suspend();

}

void PCB::suspend() {
    uint64 volatile sepc = Riscv::r_sepc();
    uint64 volatile sstatus = Riscv::r_sstatus();
    PCB* oldRunning=PCB::running;
    PCB::running=Scheduler::get();
    Riscv::switchMode();
    yield(oldRunning,PCB::running);
    Riscv::w_sstatus(sstatus);
    Riscv::w_sepc(sepc);
}

void PCB::start() {
    PCB* handle;
    asm volatile("mv %0,a1" : "=r" (handle));
    if(handle){
        Scheduler::put(handle);
    }
}

void PCB::createThreadWithoutStarting() {
    PCB** handle;
    Body body;
    void* arg;
    void* stack_space;
    asm volatile("mv %0,a1" : "=r" (handle));
    asm volatile("mv %0,a2" : "=r" (body));
    asm volatile("mv %0,a3" : "=r" (arg));
    stack_space=(void*)PCB::a4;
    *handle = new PCB(body,arg,stack_space);
}

void PCB::deleteHandle() {
    PCB* handle;
    asm volatile("mv %0,a1" : "=r" (handle));
    MemoryAllocator::freeMemory(handle->stack);
    MemoryAllocator::freeMemory(handle);
}
