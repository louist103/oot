#include <profiler.h>
#include <z64.h>
#include <ultra64.h>
#include <variables.h>
#include <functions.h>

Profiler profilerCPU;
// Profiler profilerGPU; // INCOMPLETE

static s32 profilerInitHack = 0;

#define MS_PER_CYCLE 0.00000002133f * 1000.0f

void Profiler_Init(Profiler* profiler, u16 capacity, StackType stackType) {
    s32 i;
    ProfilerEntry* root;

    profiler->profilerType = PROFILERTYPE_CPU;
    profiler->profilerEntryPool = Stack_Alloc(capacity, sizeof(ProfilerEntry*), stackType);

    // Populate the stack with placehodler entries
    // TODO: Turn this into a proper pool instad of using this weird workaround
    for (i = 0; i < capacity; i++) {
        ProfilerEntry* placeholder = ProfilerEntry_Alloc(STACKTYPE_DEBUG);
        Stack_Push(profiler->profilerEntryPool, placeholder);
    }

    // Use one of the entries from the pool as a "root" entry that will never be removed.
    root = Stack_Pop(profiler->profilerEntryPool);
    ProfilerEntry_Init(root, NULL, "CPU ROOT", 0);
    profiler->profilerEntryHead = root;
    profiler->profilerEntryRoot = root;
}

void Profiler_Push(Profiler* profiler, ProfilerEntry* entry) {
    // Stack_Push(profiler->profilerEntries, entry);
}

ProfilerEntry* Profiler_Pop(Profiler* profiler) {
    // return Stack_Pop(profiler->profilerEntries);
}

void Profilers_Init() {
    Profiler_Init(&profilerCPU, PROFILER_ENTRY_CAPACITY, STACKTYPE_DEBUG);
    // Profiler_Init(&profilerGPU, PROFILER_ENTRY_CAPACITY, STACKTYPE_DEBUG);

    profilerInitHack = true;
}

void Profiler_CPU_Begin(char* name) {
    ProfilerEntry* entry = Stack_Pop(profilerCPU.profilerEntryPool);
    ProfilerEntry_Init(entry, profilerCPU.profilerEntryHead, name, osGetTime());
    profilerCPU.profilerEntryHead = entry;
}

void Profiler_CPU_End() {
    ProfilerEntry* currentHead = profilerCPU.profilerEntryHead;
    currentHead->endTime = osGetTime();
    profilerCPU.profilerEntryHead = profilerCPU.profilerEntryHead->parent;

    // TEST: If the code block used up a negligible amount of CPU time, exclude it.
    if (((currentHead->endTime - currentHead->startTime) * MS_PER_CYCLE) < 0.50) {
        profilerCPU.profilerEntryHead->childCount--;
        Stack_Push(profilerCPU.profilerEntryPool, currentHead);
    }
}

void Profiler_CPU_Clear() {
    Profiler_ClearEntry(&profilerCPU, profilerCPU.profilerEntryRoot);
    profilerCPU.profilerEntryHead = profilerCPU.profilerEntryRoot;
    // profilerCPU.profilerEntryHead->name = "CLEARED";
}

void Profiler_ClearEntry(Profiler* profiler, ProfilerEntry* entry) {
    s32 i;

    for (i = 0; i < entry->childCount; i++) {
        // Return these entries back to our pool for reuse
        Stack_Push(profiler->profilerEntryPool, entry->children[i]);
        Profiler_ClearEntry(profiler, entry->children[i]);
    }

    entry->childCount = 0;
    entry->parent = NULL;
}

void Profiler_Draw(GfxPrint* gfxPrint) {
    ProfilerEntry* root = Profiler_CPU_GetRoot();
     GfxPrint_SetColor(gfxPrint, 0xFF, 0xFF, 0xFF, 0xFF);
     GfxPrint_SetPos(gfxPrint, 3, 4);
    // GfxPrint_Printf(gfxPrint, "PROFILER CPU HEAD: %s, Cnt: %i", root->name, root->childCount);

    // HACK: Fix the root node elapsed time
    if (root->childCount > 0) {
        root->startTime = root->children[0]->startTime;
        root->endTime = root->children[root->childCount - 1]->endTime;
    } else {
        root->startTime = 0;
        root->endTime = 0;
    }

    Profiler_DrawChild(gfxPrint, root, 0, 0);
}

s32 Profiler_DrawChild(GfxPrint* gfxPrint, ProfilerEntry* entry, s32 index, s32 depth) {
    s32 i;
    f32 elapsedMS;

    if (entry->endTime == 0 && entry->startTime == 0)
        return 0;

    elapsedMS = (f32)(entry->endTime - entry->startTime) * MS_PER_CYCLE;
    if (entry->parent != NULL) {
        GfxPrint_SetColor(gfxPrint, 0xFF, 0xFF, 0xFF, 0xFF);
        GfxPrint_SetPos(gfxPrint, 2 + (depth * 1), 2 + index + depth);
        GfxPrint_Printf(gfxPrint, "%s %.3f ms", entry->name, elapsedMS);
    }

    for (i = 0; i < entry->childCount; i++) {
        s32 totalCnt = 1;

        Profiler_DrawChild(gfxPrint, entry->children[i], index, depth + 1);
        Profiler_GetTotalChildren(entry->children[i], &totalCnt);
        index += totalCnt;
    }

    return entry->childCount;
}

void Profiler_GetTotalChildren(ProfilerEntry* entry, s32* totalCnt) {
    s32 i;

    for (i = 0; i < entry->childCount; i++)
        Profiler_GetTotalChildren(entry->children[i], totalCnt);

    *totalCnt += entry->childCount;
}

ProfilerEntry* Profiler_CPU_GetHead() {
    return profilerCPU.profilerEntryHead;
}

ProfilerEntry* Profiler_CPU_GetRoot() {
    return profilerCPU.profilerEntryRoot;
}

ProfilerEntry* ProfilerEntry_Alloc(StackType stackType) {
    ProfilerEntry* entry = NULL;

    if (stackType == STACKTYPE_DEBUG)
        entry = DebugArena_Malloc(sizeof(ProfilerEntry));
    else
        entry = ZeldaArena_Malloc(sizeof(ProfilerEntry));

    return entry;
}

void ProfilerEntry_Init(ProfilerEntry* profilerEntry, ProfilerEntry* parent, char* name, u64 startTime) {
    profilerEntry->name = name;
    profilerEntry->startTime = startTime;
    profilerEntry->endTime = 0;
    profilerEntry->parent = parent;
    profilerEntry->childCount = 0;

    if (parent != NULL)
        ProfilerEntry_PushChild(parent, profilerEntry);
}

void ProfilerEntry_PushChild(ProfilerEntry* profilerEntry, ProfilerEntry* child) {
    child->parent = profilerEntry;
    profilerEntry->children[profilerEntry->childCount] = child;
    profilerEntry->childCount++;
}

ProfilerEntry* ProfilerEntry_PopChild(ProfilerEntry* profilerEntry) {
    ProfilerEntry* child = profilerEntry->children[profilerEntry->childCount];
    profilerEntry->childCount--;
}
