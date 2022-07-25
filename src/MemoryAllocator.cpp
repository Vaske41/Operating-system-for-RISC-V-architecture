//
// Created by os on 4/22/22.
//

#include "../h/MemoryAllocator.hpp"
BlockHeader* MemoryAllocator::allocatedMemoryHead = nullptr;
BlockHeader* MemoryAllocator::freeMemoryHead = (BlockHeader*)HEAP_START_ADDR;
bool MemoryAllocator::initialized=false;
void MemoryAllocator::freeMemory() {
    if(!initialized)
    {
        allocatedMemoryHead= nullptr;
        freeMemoryHead=(BlockHeader*)HEAP_START_ADDR;
        freeMemoryHead->size= (size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR - sizeof(BlockHeader);
        freeMemoryHead->next= nullptr;
        initialized=true;
    }
    void* fragment;
    asm volatile("mv %0, a1" : "=r" (fragment));
    BlockHeader *block = allocatedMemoryHead, *prev = nullptr;
    for (; block != nullptr; prev=block, block=block->next){
        if((char*)block + sizeof(BlockHeader) == (char*)fragment)
            break;
    }
    if(!block){
        asm volatile("mv a0,%0" : :"r" (-1));
        return;
    }
    if(prev)
        prev->next=block->next;
    else
        allocatedMemoryHead=block->next;
    block->next=freeMemoryHead;
    freeMemoryHead=block;
    asm volatile("mv a0,%0" : :"r" (0));
}

void MemoryAllocator::allocateMemory() {
    if(!initialized)
    {
        allocatedMemoryHead= nullptr;
        freeMemoryHead=(BlockHeader*)HEAP_START_ADDR;
        freeMemoryHead->size= (size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR - sizeof(BlockHeader);
        freeMemoryHead->next= nullptr;
        initialized=true;
    }
    uint64 size;
    asm volatile("mv %0, a1" : "=r" (size));
    size=size*MEM_BLOCK_SIZE;
    BlockHeader *block = freeMemoryHead, *prev = nullptr;
    while(block!= nullptr){
        if (block->size>=size)
            break;
        prev=block;
        block=block->next;
    }
    if (block == nullptr) {
        uint64 retValue=0;
        asm volatile("mv a0,%0" : : "r" (retValue));
        return;
    }


    size_t remainder = block->size - size;
    if (remainder >= sizeof(BlockHeader) + MEM_BLOCK_SIZE) {

        block->size = size;
        size_t offset = sizeof(BlockHeader) + size;

        BlockHeader* newBlk = (BlockHeader*)((char*)block + offset);
        if (prev)
            prev->next = newBlk;
        else
            freeMemoryHead = newBlk;
        newBlk->next = block->next;
        newBlk->size = remainder - sizeof(BlockHeader);
    } else {

        if (prev)
            prev->next = block->next;
        else
            freeMemoryHead = block->next;
    }

    block->next = allocatedMemoryHead;
    allocatedMemoryHead=block;
    uint64 retValue=(uint64)((char*)block + sizeof(BlockHeader));
    asm volatile("mv a0,%0" : : "r" (retValue));
}

void *MemoryAllocator::allocateMemory(size_t size) {
    if(!initialized)
    {
        allocatedMemoryHead= nullptr;
        freeMemoryHead=(BlockHeader*)HEAP_START_ADDR;
        freeMemoryHead->size= (size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR - sizeof(BlockHeader);
        freeMemoryHead->next= nullptr;
        initialized=true;
    }

    uint64 numberOfBlocks=size/MEM_BLOCK_SIZE;
    if(size%MEM_BLOCK_SIZE)
        numberOfBlocks++;
    size=numberOfBlocks*MEM_BLOCK_SIZE;
    BlockHeader *block = freeMemoryHead, *prev = nullptr;
    while(block!= nullptr){
        if (block->size>=size)
            break;
        prev=block;
        block=block->next;
    }

    if (block == nullptr) {

        return nullptr;
    }


    size_t remainder = block->size - size;
    if (remainder >= sizeof(BlockHeader) + MEM_BLOCK_SIZE) {

        block->size = size;
        size_t offset = sizeof(BlockHeader) + size;

        BlockHeader* newBlk = (BlockHeader*)((char*)block + offset);
        if (prev)
            prev->next = newBlk;
        else
            freeMemoryHead = newBlk;
        newBlk->next = block->next;
        newBlk->size = remainder - sizeof(BlockHeader);
    } else {

        if (prev)
            prev->next = block->next;
        else
            freeMemoryHead = block->next;
    }

    block->next = allocatedMemoryHead;

    allocatedMemoryHead=block;
    void* retValue=(void*)((char*)block + sizeof(BlockHeader));
    return retValue;

}

int MemoryAllocator::freeMemory(void *memory) {
    if(!initialized)
    {
        allocatedMemoryHead= nullptr;
        freeMemoryHead=(BlockHeader*)HEAP_START_ADDR;
        freeMemoryHead->size= (size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR - sizeof(BlockHeader);
        freeMemoryHead->next= nullptr;
        initialized=true;
    }
    void* fragment=memory;
    BlockHeader *block = allocatedMemoryHead, *prev = nullptr;
    for (; block != nullptr; prev=block, block=block->next){
        if((char*)block + sizeof(BlockHeader) == (char*)fragment)
            break;
    }
    if(!block){
        return -1;
    }
    if(prev)
        prev->next=block->next;
    else
        allocatedMemoryHead=block->next;
    block->next=freeMemoryHead;
    freeMemoryHead=block;
    return 0;
}
