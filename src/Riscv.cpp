//
// Created by os on 4/27/22.
//

#include "../h/Riscv.hpp"
#include "../h/SleepList.hpp"
#include "../h/kSemaphore.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/PCB.hpp"
#include "../h/Scheduler.hpp"
void Riscv::popSppSpie() {
    asm volatile("csrw sepc, ra");
    asm volatile ("sret");
}
void Riscv::handleSupervisorTrap() {
    uint64 volatile a4;

    asm volatile("mv %0, a4" : "=r" (a4));
    PCB::a4 = a4;
    uint64 volatile scause=Riscv::r_scause();
    if (scause == 0x0000000000000009UL || scause == 0x0000000000000008UL) {
        uint64 volatile sepc = Riscv::r_sepc();
        uint64 volatile sstatus = Riscv::r_sstatus();
        uint64 volatile code;
        __asm__ volatile("mv %0, a0" : "=r" (code));
        switch (code) {
            case 0x01:
                MemoryAllocator::allocateMemory();
                break;
            case 0x02:
                MemoryAllocator::freeMemory();
                break;
            case 0x03:
                PCB::deleteHandle();
                break;
            case 0x04:
                kSemaphore::deleteHandle();
                break;
            case 0x11:
                PCB::createThread();
                break;
            case 0x12:
                PCB::running->setFinished(true);
                PCB::dispatch();
                break;
            case 0x13:
                PCB::dispatch();
                break;
            case 0x14:
                PCB::start();
                break;
            case 0x15:
                PCB::createThreadWithoutStarting();
                break;
            case 0x21:
                kSemaphore::open();
                break;
            case 0x22:
                kSemaphore::close();
                break;
            case 0x23:
                kSemaphore::systemWait();
                break;
            case 0x24:
                kSemaphore::systemSignal();
                break;
            case 0x31:
                PCB::sleep();
                break;
            case 0x41:
                kConsole::getc();
                break;
            case 0x42:
                kConsole::putc();
                break;
            default:
                break;
        }
        Riscv::w_sstatus(sstatus);
        Riscv::w_sepc(sepc);
        Riscv::inc4_sepc();
    }else if (scause == 0x8000000000000001UL) {
        Riscv::mc_sip(Riscv::SIP_SSIE);
        SleepList::awake();
        PCB::timeSliceCounter++;
        if (PCB::timeSliceCounter >= PCB::running->getTimeSlice()) {
            uint64 volatile sepc = Riscv::r_sepc();
            uint64 volatile sstatus = Riscv::r_sstatus();
            PCB::timeSliceCounter = 0;
            PCB::dispatch();
            Riscv::w_sstatus(sstatus);
            Riscv::w_sepc(sepc);
        }
    } else if (scause == 0x8000000000000009UL) {
        int volatile device = plic_claim();
        while(*(char*)CONSOLE_STATUS & 1){
            char character=*(char*)CONSOLE_TX_DATA;
            kConsole::outputBuffer[kConsole::outputTail] = character;
            kConsole::outputTail=(kConsole::outputTail+1)%N;
            kConsole::outputItemAvailable.signal();
            kConsole::inputItemAvailable.signal();
            kConsole::inputBuffer[kConsole::inputTail]=character;
            kConsole::inputTail=(kConsole::inputTail+1)%N;
        }
        plic_complete(device);
    } else {
        PCB::running->setFinished(true);
        PCB::dispatch();
    }
    //ovde a0 da se upise na stek; mozda ne treba uvek
    switchMode();
}
void Riscv::printString(const char *string) {
    while (*string != '\0')
    {
        putc(*string);
        string++;
    }
}

void Riscv::printInteger(uint64 num)
{
    static char digits[] = "0123456789";
    char buf[16];
    int i, neg;
    uint x;

    neg = 0;
    if(num < 0)
    {
        neg = 1;
        x = -num;
    }
    else
        x = num;

    i = 0;
    do
    {
        buf[i++] = digits[x%10];
    }while((x/=10) != 0);
    if(neg)
        buf[i++] = '-';

    while(--i >= 0)
        putc(buf[i]);

    putc('\n');

}
void Riscv::userWrapper(void *arg) {
    userMain();
}

void Riscv::systemPutcWrapper(void* arg) {
    while(true){
        sem_wait(&kConsole::outputItemAvailable);
        while(!(*((char*)CONSOLE_STATUS) & (1<<5))){
            thread_dispatch();
        }
        Riscv::disableInterrupts();
        char data=kConsole::outputBuffer[kConsole::outputHead];
        kConsole::outputHead=(kConsole::outputHead+1)%N;
        *(char*)CONSOLE_RX_DATA=data;
        Riscv::enableInterrupts();

    }
}

void Riscv::switchMode() {
    if(PCB::running->user){
        Riscv::mc_sstatus(Riscv::SSTATUS_SPP);
    }
    else{
        Riscv::ms_sstatus(Riscv::SSTATUS_SPP);
    }
}

