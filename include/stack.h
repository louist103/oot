#ifndef _STACK_H_
#define _STACK_H_

#include <ultra64.h>

typedef enum
{
    STACKTYPE_DEBUG,
    STACKTYPE_ZELDA
} StackType;

typedef struct
{
    u16 entriesCapacity; // How many entries can our stack hold?
    u16 entryIndex; // Where is the top of the stack currently?
    StackType stackType; // On what heap is this stack allocated?
    void** entries; // The actual data for the stack, dynamically allocated.
} Stack;

Stack* Stack_Alloc(u16 capacity, u32 entryTypeSize, StackType stackType);
void Stack_Push(Stack* stack, void* item);
void* Stack_Pop(Stack* stack);

#endif