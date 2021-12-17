#include <stack.h>
#include <functions.h>

Stack* Stack_Alloc(u16 capacity, u32 entryTypeSize, StackType stackType) {
    Stack* stack = NULL;

    if (stackType == STACKTYPE_DEBUG) {
        stack = DebugArena_Malloc(sizeof(Stack));
        stack->entries = DebugArena_Malloc(entryTypeSize * capacity);
    } else {
        stack = ZeldaArena_Malloc(sizeof(Stack));
        stack->entries = ZeldaArena_Malloc(entryTypeSize * capacity);
    }

    stack->entriesCapacity = capacity;
    // stack->entryIndex = capacity - 1;

    return stack;
}

void Stack_Push(Stack* stack, void* item) {
    stack->entries[stack->entryIndex] = item;
    stack->entryIndex++;
}

void* Stack_Pop(Stack* stack) {
    stack->entryIndex--;
    return stack->entries[stack->entryIndex];
}
