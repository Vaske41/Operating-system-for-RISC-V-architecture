//
// Created by os on 6/9/22.
//
#include "../lib/hw.h"
#ifndef PROJECT_BASE_V1_0_SYSCALL_C_HPP
#define PROJECT_BASE_V1_0_SYSCALL_C_HPP
class PCB;
using thread_t=PCB*;

void* mem_alloc (size_t size);

int mem_free (void*);

int thread_create (
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
);
void thread_create_without_starting (
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
);
int thread_exit ();
void thread_dispatch ();
void thread_start(thread_t handle);
class kSemaphore;
using sem_t = kSemaphore*;
int sem_open (
        sem_t* handle,
        unsigned init
);
int sem_close (sem_t handle);
int sem_wait (sem_t id);
int sem_signal (sem_t id);
typedef unsigned long time_t;
int time_sleep (time_t);
const int EOF = -1;
char getc ();
void putc (char);
void delete_thread_t(thread_t handle);
void delete_sem_t(sem_t handle);
#endif //PROJECT_BASE_V1_0_SYSCALL_C_HPP
