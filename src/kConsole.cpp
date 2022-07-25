//
// Created by os on 5/30/22.
//

#include "../h/kConsole.hpp"
kSemaphore kConsole::inputItemAvailable(0);
kSemaphore kConsole::outputItemAvailable(0);
char kConsole::inputBuffer[N];
char kConsole::outputBuffer[N];
uint64 kConsole::inputHead=0;
uint64 kConsole::outputHead=0;
uint64 kConsole::inputTail=0;
uint64 kConsole::outputTail=0;

void kConsole::getc() {
    inputItemAvailable.wait();
    char c = inputBuffer[inputHead];
    inputHead=(inputHead+1)%N;
    asm volatile("mv a0,%0" : : "r" ((uint64)c));
}

void kConsole::putc() {
    char c;
    asm volatile("mv %0,a1" : "=r" (c));
    outputBuffer[outputTail]=c;
    outputTail=(outputTail+1)%N;
    outputItemAvailable.signal();
}





