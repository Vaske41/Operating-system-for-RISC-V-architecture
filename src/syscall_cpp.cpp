//
// Created by os on 4/12/22.
//

#include "../h/syscall_cpp.hpp"


struct PeriodicThreadArg{
    PeriodicThreadArg(PeriodicThread *thread,time_t period) : thread(thread),period(period){}
    PeriodicThread *thread;
    time_t period;
};

void* operator new (size_t size){
    return mem_alloc(size);
}

void operator delete (void* fragment){
    mem_free(fragment);
}

void *operator new[](size_t size) {
    return mem_alloc(size);
}

void operator delete[](void *fragment) {
    mem_free(fragment);
}

Thread::Thread(void (*body)(void *), void *arg){
    thread_create_without_starting(&myHandle,body,arg);
}

void Thread::start() {
    thread_start(myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

void Thread::sleep(time_t time) {
    if(time>0)
        time_sleep(time);
}

Thread::~Thread() {
    delete_thread_t(myHandle);
}

void Thread::wrapper(void *arg) {
    Thread* t=(Thread*)arg;
    if(t)
        t->run();
}

Thread::Thread() {
    thread_create_without_starting(&myHandle,wrapper,this);
}


Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle,init);
}

Semaphore::~Semaphore() {
    delete_sem_t(myHandle);
}

void Semaphore::wait() {
    sem_wait(myHandle);
}

void Semaphore::signal() {
    sem_signal(myHandle);
}

PeriodicThread::PeriodicThread(time_t period) : Thread(wrapper,new PeriodicThreadArg(this,period)){

}

void PeriodicThread::wrapper(void* arg) {
    PeriodicThreadArg* ptArg = (PeriodicThreadArg*)(arg);
    PeriodicThread* pt = ptArg->thread;
    while(true){
        pt->periodicActivation();
        pt->sleep(ptArg->period);
    }
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char character) {
    ::putc(character);
}
