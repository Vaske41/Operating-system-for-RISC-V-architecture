//
// Created by os on 5/9/22.
//

#include "../h/kSemaphore.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/Scheduler.hpp"
#include "../h/syscall_c.hpp"
#include "../h/Riscv.hpp"
void kSemaphore::wait() {
    kSemaphore* handle = this;
    if(handle){
        if(--(handle->val)<0){
            handle->block();
        }
        if(PCB::running->semaphoreFault)
            PCB::running->semaphoreFault=false;
    }
}

void kSemaphore::signal() {
    kSemaphore* handle = this;
    if(handle) {
        if (++(handle->val) <= 0) {
            handle->unblock();
        }
    }
}

void kSemaphore::block() {
    PCB::running->blockedNext= nullptr;
    if(head)
        tail->blockedNext=PCB::running;
    else
        head=PCB::running;
    tail=PCB::running;
    uint64 volatile sepc = Riscv::r_sepc();
    uint64 volatile sstatus = Riscv::r_sstatus();
    PCB::suspend();
    Riscv::w_sstatus(sstatus);
    Riscv::w_sepc(sepc);
}

void kSemaphore::unblock() {
    if(head){
        Scheduler::put(head);
        head=head->blockedNext;
        if(!head)
            tail=nullptr;
    }
}

void kSemaphore::close() {
    kSemaphore* handle;
    asm volatile("mv %0,a1" : "=r" (handle));
    if(handle){
        while(handle->head){
            handle->head->semaphoreFault=true;
            handle->unblock();
        }
        asm volatile("mv a0,%0" : : "r" (0));
    }
    else{
        asm volatile("mv a0,%0" : : "r" (-1));
    }
}

void kSemaphore::open() {
    kSemaphore** handle;
    uint64 init;
    asm volatile("mv %0,a2" : "=r" (init));
    asm volatile("mv %0,a1" : "=r" (handle));
    if(!handle){
        asm volatile("mv a0,%0" : : "r" (-1));
        return;
    }
    *handle = new kSemaphore(init);
    if(*handle)
        asm volatile("mv a0,%0" : : "r" (0));
    else
        asm volatile("mv a0,%0" : : "r" (-1));
}

void *kSemaphore::operator new(size_t size) {
    return MemoryAllocator::allocateMemory(size);
}

void *kSemaphore::operator new[](size_t size) {
    return MemoryAllocator::allocateMemory(size);
}

void kSemaphore::operator delete(void *memory) {
    MemoryAllocator::freeMemory(memory);
}

void kSemaphore::operator delete[](void *memory) {
    MemoryAllocator::freeMemory(memory);
}

void kSemaphore::systemWait() {
    kSemaphore* handle;
    asm volatile("mv %0,a1" : "=r" (handle));
    if(handle){
        if(--(handle->val)<0){
            handle->block();
        }
        if(PCB::running->semaphoreFault)
        {
            PCB::running->semaphoreFault=false;
            asm volatile("mv a0,%0" : : "r" (-1));
        }
        else
            asm volatile("mv a0,%0" : : "r" (0));
    }
    else
        asm volatile("mv a0,%0" : : "r" (-1));
}

void kSemaphore::systemSignal() {
    kSemaphore* handle;
    asm volatile("mv %0,a1" : "=r" (handle));
    if(handle) {
        if (++(handle->val) <= 0) {
            handle->unblock();
        }
        asm volatile("mv a0,%0" : : "r" (0));
    }
    else
        asm volatile("mv a0,%0" : : "r" (-1));
}

void kSemaphore::deleteHandle() {
    kSemaphore* handle;
    asm volatile("mv %0,a1" : "=r" (handle));
    delete handle;
}
