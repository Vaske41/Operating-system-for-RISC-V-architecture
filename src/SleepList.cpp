//
// Created by os on 5/9/22.
//

#include "../h/SleepList.hpp"
#include "../h/Scheduler.hpp"
PCB* SleepList::head = nullptr;
void SleepList::awake() {
    if(head)
        head->sleepingTime--;
    while(head && head->sleepingTime == 0){
        PCB* oldHead = head;
        head = head->sleepNext;
        Scheduler::put(oldHead);
    }
}

void SleepList::put(PCB *pcb) {
    PCB* prev= nullptr,*curr=head;
    while(curr && curr->sleepingTime < pcb->sleepingTime){
        pcb->sleepingTime -= curr->sleepingTime;
        prev=curr;
        curr=curr->sleepNext;
    }
    pcb->sleepNext=curr;
    if(prev)
        prev->sleepNext=pcb;
    else
        head=pcb;
    if(curr)
        curr->sleepingTime -= pcb->sleepingTime;
}
