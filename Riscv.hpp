//
// Created by os on 4/27/22.
//
#include "../lib/hw.h"
#include "../lib/console.h"
#include "../test/userMain.hpp"
#include "./kConsole.hpp"
#ifndef PROJECT_BASE_V1_0_RISCV_HPP
#define PROJECT_BASE_V1_0_RISCV_HPP


class Riscv {

public:

    static void handleSupervisorTrap();
    static void systemPutcWrapper(void*);
    static void supervisorTrap();
    static void pushRegisters();
    static void popRegisters();
    static void userWrapper(void*);


    static void switchMode();
    static uint64 r_scause();
    static void w_scause(uint64 scause);
    static uint64 r_sepc();
    static void w_sepc(uint64 sepc);
    static uint64 r_stvec();
    static void w_stvec(uint64 stvec);
    static uint64 r_stval();
    static void w_stval(uint64 stval);
    static uint64 r_sip();
    static void w_sip(uint64 sip);
    static void inc4_sepc();
    static void printString(const char*);
    static void printInteger(uint64);
    static void enableInterrupts();
    static void disableInterrupts();

    enum BitMaskSip{
        SIP_SSIE = (1<<1),
        SIP_STIE = (1<<5),
        SIP_SEIE = (1<<9)
    };

    enum BitMaskSstatus{
        SSTATUS_SIE = (1<<1),
        SSTATUS_SPIE = (1<<5),
        SSTATUS_SPP = (1<<8)
    };

    static uint64 r_sstatus();
    static void w_sstatus(uint64 sstatus);

    static void ms_sstatus(uint64 mask);
    static void mc_sstatus(uint64 mask);
    static void mc_sip(uint64 mask);
    static void ms_sip(uint64 mask);
    static void popSppSpie();
};
inline uint64 Riscv::r_scause() {
    uint64 scause;
    asm volatile("csrr %0,scause" : "=r" (scause));
    return scause;
}

inline void Riscv::w_scause(uint64 scause) {
    asm volatile("csrw scause,%0" : : "r" (scause));
}
inline uint64 Riscv::r_sepc() {
    uint64 volatile sepc;
    asm volatile("csrr %0,sepc" : "=r" (sepc));
    return sepc;
}

inline void Riscv::w_sepc(uint64 sepc) {
    asm volatile("csrw sepc,%0" : : "r" (sepc));
}
inline uint64 Riscv::r_sstatus() {
    uint64 sstatus;
    asm volatile("csrr %0,sstatus" : "=r" (sstatus));
    return sstatus;
}

inline void Riscv::w_sstatus(uint64 sstatus) {
    asm volatile("csrw sstatus,%0" : : "r" (sstatus));
}
inline uint64 Riscv::r_stval() {
    uint64 stval;
    asm volatile("csrr %0,stval" : "=r" (stval));
    return stval;
}

inline void Riscv::w_stval(uint64 stval) {
    asm volatile("csrw stval,%0" : : "r" (stval));
}
inline uint64 Riscv::r_stvec() {
    uint64 stvec;
    asm volatile("csrr %0,stvec" : "=r" (stvec));
    return stvec;
}

inline void Riscv::w_stvec(uint64 stvec) {
    asm volatile("csrw stvec,%0" : : "r" (stvec));
}
inline void Riscv::ms_sstatus(uint64 mask) {
    asm volatile("csrs sstatus,%0" : : "r" (mask));
}
inline void Riscv::mc_sstatus(uint64 mask) {
    asm volatile("csrc sstatus,%0" : : "r" (mask));
}
inline void Riscv::mc_sip(uint64 mask) {
    asm volatile("csrc sip,%0" : : "r" (mask));
}
inline void Riscv::ms_sip(uint64 mask) {
    asm volatile("csrs sip,%0" : : "r" (mask));
}
inline uint64 Riscv::r_sip() {
    uint64 sip;
    asm volatile("csrr %0,sip" : "=r" (sip));
    return sip;
}
inline void Riscv::w_sip(uint64 sip) {
    asm volatile("csrw sip,%0" : : "r" (sip));
}

inline void Riscv::inc4_sepc() {
    uint64 sepc=Riscv::r_sepc();
    sepc+=4;
    Riscv::w_sepc(sepc);
}

inline void Riscv::enableInterrupts() {
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
}

inline void Riscv::disableInterrupts() {
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
}




#endif //PROJECT_BASE_V1_0_RISCV_H
