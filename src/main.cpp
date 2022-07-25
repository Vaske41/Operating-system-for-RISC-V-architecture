#include "../h/syscall_c.hpp"
#include "../h/Riscv.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/Scheduler.hpp"
#include "../test/userMain.hpp"


void main(){
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    void* stack1 = MemoryAllocator::allocateMemory(DEFAULT_STACK_SIZE * sizeof(uint64));
    void *stack2 = MemoryAllocator::allocateMemory(DEFAULT_STACK_SIZE * sizeof(uint64));
    PCB* mainPCB = new PCB(nullptr, nullptr, nullptr);
    PCB *consolePCB = new PCB(Riscv::systemPutcWrapper, nullptr,stack1);
    PCB *userPCB = new PCB(Riscv::userWrapper, nullptr,stack2);
    mainPCB->setUser(false);
    consolePCB->setUser(false);
    userPCB->setUser(true);
    Scheduler::put(mainPCB);
    Scheduler::put(userPCB);
    Scheduler::put(consolePCB);
    PCB::running=Scheduler::get();

    Riscv::enableInterrupts();
    while(!userPCB->isFinished()){
        thread_dispatch();
    }
    while(!kConsole::finished())
        thread_dispatch();
    Riscv::disableInterrupts();

}
