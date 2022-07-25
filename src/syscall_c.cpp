//
// Created by os on 6/9/22.
//

#include "../h/syscall_c.hpp"

void* mem_alloc (size_t size) {

    uint64 numberOfBlocks = size/MEM_BLOCK_SIZE;
    if(size%MEM_BLOCK_SIZE)
        numberOfBlocks++;
    uint64 code=1;
    asm volatile("mv a0,%0" : : "r" (code));
    asm volatile("mv a1,%0" : : "r" (numberOfBlocks));
    asm volatile("ecall");
    asm volatile("mv %0,a0" : "=r" (code));
    return (void*)code;
}

int mem_free(void *ptr) {
    uint64 code=2;
    uint64 parameter=(uint64)ptr;
    asm volatile("mv a1,%0" : : "r" (parameter));
    asm volatile("mv a0,%0" : : "r" (code));
    asm volatile("ecall");
    asm volatile("mv %0,a0" : "=r" (code));
    return (int)code;
}

void thread_dispatch() {
    uint64 code=0x13;
    asm volatile("mv a0,%0" : : "r" (code));
    asm volatile("ecall");
}

int thread_create(thread_t *handle, void (*start_routine)(void *), void *arg) {
    void *stack_space = mem_alloc(DEFAULT_STACK_SIZE * sizeof(uint64));
    uint64 code=0x11;
    asm volatile("mv a4,%0" : : "r" (stack_space));
    asm volatile("mv a3,%0" : : "r" (arg));
    asm volatile("mv a2,%0" : : "r" (start_routine));
    asm volatile("mv a1,%0" : : "r" (handle));
    asm volatile("mv a0,%0" : : "r" (code));
    asm volatile("ecall");
    asm volatile("mv %0,a0" : "=r" (code));
    return (int)code;
}

int thread_exit() {
    uint64 code=0x12;
    asm volatile("mv a0,%0" : : "r" (code));
    asm volatile("ecall");
    asm volatile("mv %0,a0" : "=r" (code));
    return (int)code;
}

int time_sleep(time_t time) {
    uint64 code=0x31;
    asm volatile("mv a1,%0" : : "r" (time));
    asm volatile("mv a0,%0" : : "r" (code));
    asm volatile("ecall");
    asm volatile("mv %0,a0" : "=r" (code));
    return (int)code;
}

int sem_open(sem_t *handle, unsigned int init) {
    uint64 code=0x21;
    asm volatile("mv a2,%0" : : "r" (init));
    asm volatile("mv a1,%0" : : "r" (handle));
    asm volatile("mv a0,%0" : : "r" (code));
    asm volatile("ecall");
    asm volatile("mv %0,a0" : "=r" (code));
    return (int)code;
}

int sem_close(sem_t handle) {
    uint64 code=0x22;
    asm volatile("mv a1,%0" : : "r" (handle));
    asm volatile("mv a0,%0" : : "r" (code));
    asm volatile("ecall");
    asm volatile("mv %0,a0" : "=r" (code));
    return (int)code;
}

int sem_wait(sem_t id) {
    uint64 code=0x23;
    asm volatile("mv a1,%0" : : "r" (id));
    asm volatile("mv a0,%0" : : "r" (code));
    asm volatile("ecall");
    asm volatile("mv %0,a0" : "=r" (code));
    return (int)code;
}

int sem_signal(sem_t id) {
    uint64 code=0x24;
    asm volatile("mv a1,%0" : : "r" (id));
    asm volatile("mv a0,%0" : : "r" (code));
    asm volatile("ecall");
    asm volatile("mv %0,a0" : "=r" (code));
    return (int)code;
}

char getc() {
    uint64 code=0x41;
    asm volatile("mv a0,%0" : : "r" (code));
    asm volatile("ecall");
    asm volatile("mv %0,a0" : "=r" (code));
    return (char)code;
}

void putc(char character) {
    uint64 code=0x42;
    asm volatile("mv a1,%0" : : "r" ((uint64)character));
    asm volatile("mv a0,%0" : : "r" (code));
    asm volatile("ecall");
}

void thread_start(thread_t handle) {
    uint64 code=0x14;
    asm volatile("mv a1,%0" : : "r" (handle));
    asm volatile("mv a0,%0" : : "r" (code));
    asm volatile("ecall");
}

void thread_create_without_starting(thread_t *handle, void (*start_routine)(void *), void *arg) {
    void *stack_space = mem_alloc(DEFAULT_STACK_SIZE * sizeof(uint64));
    uint64 code=0x15;
    asm volatile("mv a4,%0" : : "r" (stack_space));
    asm volatile("mv a3,%0" : : "r" (arg));
    asm volatile("mv a2,%0" : : "r" (start_routine));
    asm volatile("mv a1,%0" : : "r" (handle));
    asm volatile("mv a0,%0" : : "r" (code));
    asm volatile("ecall");
    asm volatile("mv %0,a0" : "=r" (code));
}

void delete_thread_t(thread_t handle) {
    uint64 code=0x03;
    asm volatile("mv a1,%0" : : "r" (handle));
    asm volatile("mv a0,%0" : : "r" (code));
    asm volatile("ecall");
}
void delete_sem_t(sem_t handle) {
    uint64 code=0x04;
    asm volatile("mv a1,%0" : : "r" (handle));
    asm volatile("mv a0,%0" : : "r" (code));
    asm volatile("ecall");
}
