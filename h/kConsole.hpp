//
// Created by os on 5/30/22.
//

#ifndef PROJECT_BASE_V1_0_KCONSOLE_HPP
#define PROJECT_BASE_V1_0_KCONSOLE_HPP

#include "./kSemaphore.hpp"

const uint64 N = 9216;

class kConsole {
public:
    static void getc();

    static void putc();

    static bool finished(){
        return outputItemAvailable.val<0;
    }



protected:
public:
    static char inputBuffer[N], outputBuffer[N];
    static kSemaphore inputItemAvailable,
        outputItemAvailable;
    static uint64 inputHead, inputTail, outputHead, outputTail;
    friend class Riscv;
};


#endif //PROJECT_BASE_V1_0_KCONSOLE_HPP
