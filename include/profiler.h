#ifndef _PROFILER_H_
#define _PROFILER_H_

#include <z64.h>
#include <ultra64.h>
#include <ultra64/gbi.h>
#include <ultra64/gs2dex.h>
#include <ultra64/controller.h>
#include <stack.h>

#define PROFILER_ENTRY_CAPACITY 128
#define PROFILERENTRY_CHILD_CAPACITY 24

typedef struct ProfilerEntry
{
    u64 startTime;
    u64 endTime;
    char* name;
    struct Profiler* profiler;
    struct ProfilerEntry* parent;
    struct ProfilerEntry* children[PROFILERENTRY_CHILD_CAPACITY];
    u8 childCount;
} ProfilerEntry;

typedef enum
{
    PROFILERTYPE_CPU,
    PROFILERTYPE_GPU // INCOMPLETE
} ProfilerType;

typedef struct Profiler
{
    ProfilerType profilerType;
    Stack* profilerEntryPool;
    ProfilerEntry* profilerEntryHead;
    ProfilerEntry* profilerEntryRoot;
} Profiler;


extern Profiler profilerCPU;
extern Profiler profilerGPU; // INCOMPLETE

void Profiler_Init(Profiler* profiler, u16 capacity, StackType stackType);
void Profiler_Push(Profiler* profiler, ProfilerEntry* entry);
ProfilerEntry* Profiler_Pop(Profiler* profiler);
void Profiler_ClearEntry(Profiler* profiler, ProfilerEntry* entry);

// "Static" Profiler Functions
void Profilers_Init();
void Profiler_CPU_Begin(char* name);
void Profiler_CPU_End();
void Profiler_CPU_Clear();
void Profiler_Draw(GfxPrint* gfxPrint);
s32 Profiler_DrawChild(GfxPrint* gfxPrint, ProfilerEntry* entry, s32 index, s32 depth);
void Profiler_GetTotalChildren(ProfilerEntry* entry, s32* totalCnt);
ProfilerEntry* Profiler_CPU_GetHead();
ProfilerEntry* Profiler_CPU_GetRoot();

ProfilerEntry* ProfilerEntry_Alloc(StackType stackType);
void ProfilerEntry_Init(ProfilerEntry* profilerEntry, ProfilerEntry* parent, char* name, u64 startEntry);
void ProfilerEntry_PushChild(ProfilerEntry* profilerEntry, ProfilerEntry* child);
ProfilerEntry* ProfilerEntry_PopChild(ProfilerEntry* profilerEntry);

#endif