#include "global.h"

ListAlloc* ListAlloc_Init(ListAlloc* self) {
    self->prev = NULL;
    self->next = NULL;
    return self;
}

void* ListAlloc_Alloc(ListAlloc* self, u32 size) {
    ListAlloc* ptr = SystemArena_MallocDebug(size + sizeof(ListAlloc), "../listalloc.c", 40);
    ListAlloc* next;

    if (ptr == NULL) {
        return NULL;
    }

    next = self->next;
    if (next != NULL) {
        next->next = ptr;
    }

    ptr->prev = next;
    ptr->next = NULL;
    self->next = ptr;

    if (self->prev == NULL) {
        self->prev = ptr;
    }

    return (u8*)ptr + sizeof(ListAlloc);
}

void ListAlloc_Free(ListAlloc* self, void* data) {
    ListAlloc* ptr = &((ListAlloc*)data)[-1];

    if (ptr->prev != NULL) {
        ptr->prev->next = ptr->next;
    }

    if (ptr->next != NULL) {
        ptr->next->prev = ptr->prev;
    }

    if (self->prev == ptr) {
        self->prev = ptr->next;
    }

    if (self->next == ptr) {
        self->next = ptr->prev;
    }

    SystemArena_FreeDebug(ptr, "../listalloc.c", 72);
}

void ListAlloc_FreeAll(ListAlloc* self) {
    ListAlloc* iter = self->prev;

    while (iter != NULL) {
        ListAlloc_Free(self, (u8*)iter + sizeof(ListAlloc));
        iter = self->prev;
    }
}
