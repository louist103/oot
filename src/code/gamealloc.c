#include "global.h"

void GameAlloc_Log(GameAlloc* self) {
    GameAllocEntry* iter;

    osSyncPrintf("self = %08x\n", self);

    iter = self->base.next;
    while (iter != &self->base) {
        osSyncPrintf("ptr = %08x size = %d\n", iter, iter->size);
        iter = iter->next;
    }
}

void* GameAlloc_MallocDebug(GameAlloc* self, u32 size, const char* file, s32 line) {
    GameAllocEntry* ptr = SystemArena_MallocDebug(size + sizeof(GameAllocEntry), file, line);

    if (ptr != NULL) {
        ptr->size = size;
        ptr->prev = self->head;
        self->head->next = ptr;
        self->head = ptr;
        ptr->next = &self->base;
        self->base.prev = self->head;
        return ptr + 1;
    } else {
        return NULL;
    }
}

void* GameAlloc_Malloc(GameAlloc* self, u32 size) {
    GameAllocEntry* ptr = SystemArena_MallocDebug(size + sizeof(GameAllocEntry), "../gamealloc.c", 93);

    if (ptr != NULL) {
        ptr->size = size;
        ptr->prev = self->head;
        self->head->next = ptr;
        self->head = ptr;
        ptr->next = &self->base;
        self->base.prev = self->head;
        return ptr + 1;
    } else {
        return NULL;
    }
}

void GameAlloc_Free(GameAlloc* self, void* data) {
    GameAllocEntry* ptr;

    if (data != NULL) {
        ptr = &((GameAllocEntry*)data)[-1];
        LogUtils_CheckNullPointer("ptr->prev", ptr->prev, "../gamealloc.c", 120);
        LogUtils_CheckNullPointer("ptr->next", ptr->next, "../gamealloc.c", 121);
        ptr->prev->next = ptr->next;
        ptr->next->prev = ptr->prev;
        self->head = self->base.prev;
        SystemArena_FreeDebug(ptr, "../gamealloc.c", 125);
    }
}

void GameAlloc_Cleanup(GameAlloc* self) {
    GameAllocEntry* next = self->base.next;
    GameAllocEntry* cur;

    while (&self->base != next) {
        cur = next;
        next = next->next;
        SystemArena_FreeDebug(cur, "../gamealloc.c", 145);
    }

    self->head = &self->base;
    self->base.next = &self->base;
    self->base.prev = &self->base;
}

void GameAlloc_Init(GameAlloc* self) {
    self->head = &self->base;
    self->base.next = &self->base;
    self->base.prev = &self->base;
}
