//
// Created by os on 4/27/22.
//

#include "../h/Scheduler.hpp"
PCB* Scheduler::head= nullptr;
PCB* Scheduler::tail= nullptr;
PCB *Scheduler::get() {
    if(!head)
        return nullptr;
    else{
        PCB* retVal = head;
        head=head->schedulerNext;
        if(!head)
            tail= nullptr;
        return retVal;
    }
}

void Scheduler::put(PCB *pcb) {
    pcb->schedulerNext= nullptr;
    if(head)
        tail->schedulerNext=pcb;
    else
        head=pcb;
    tail=pcb;
}

