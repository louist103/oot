#include "global.h"
#include "vt.h"

vu32 gIrqMgrResetStatus = 0;
volatile OSTime sIrqMgrResetTime = 0;
volatile OSTime gIrqMgrRetraceTime = 0;
u32 sIrqMgrRetraceCount = 0;

#define RETRACE_MSG 666
#define PRE_NMI_MSG 669
#define PRENMI450_MSG 671
#define PRENMI480_MSG 672
#define PRENMI500_MSG 673

#define STATUS_IDLE 0
#define STATUS_PRENMI 1
#define STATUS_NMI 2

void IrqMgr_AddClient(IrqMgr* self, IrqMgrClient* c, OSMesgQueue* msgQ) {
    u32 prevInt;

    LogUtils_CheckNullPointer("self", self, "../irqmgr.c", 96);
    LogUtils_CheckNullPointer("c", c, "../irqmgr.c", 97);
    LogUtils_CheckNullPointer("msgQ", msgQ, "../irqmgr.c", 98);

    prevInt = osSetIntMask(1);

    c->queue = msgQ;
    c->prev = self->clients;
    self->clients = c;

    osSetIntMask(prevInt);

    if (self->resetStatus > STATUS_IDLE) {
        osSendMesg(c->queue, (OSMesg) & self->prenmiMsg, OS_MESG_NOBLOCK);
    }

    if (self->resetStatus >= STATUS_NMI) {
        osSendMesg(c->queue, (OSMesg) & self->nmiMsg, OS_MESG_NOBLOCK);
    }
}

void IrqMgr_RemoveClient(IrqMgr* self, IrqMgrClient* c) {
    IrqMgrClient* iter = self->clients;
    IrqMgrClient* lastIter = NULL;
    u32 prevInt;

    LogUtils_CheckNullPointer("self", self, "../irqmgr.c", 129);
    LogUtils_CheckNullPointer("c", c, "../irqmgr.c", 130);

    prevInt = osSetIntMask(1);

    while (iter != NULL) {
        if (iter == c) {
            if (lastIter) {
                lastIter->prev = c->prev;
            } else {
                self->clients = c->prev;
            }
            break;
        }
        lastIter = iter;
        iter = iter->prev;
    }

    osSetIntMask(prevInt);
}

void IrqMgr_SendMesgForClient(IrqMgr* self, OSMesg msg) {
    IrqMgrClient* iter = self->clients;

    while (iter != NULL) {
        if (iter->queue->validCount >= iter->queue->msgCount) {
            // "irqmgr_SendMesgForClient: Message queue is overflowing mq=%08x cnt=%d"
            osSyncPrintf(
                VT_COL(RED, WHITE) "irqmgr_SendMesgForClient:メッセージキューがあふれています mq=%08x cnt=%d\n" VT_RST,
                iter->queue, iter->queue->validCount);
        } else {
            osSendMesg(iter->queue, msg, OS_MESG_NOBLOCK);
        }

        iter = iter->prev;
    }
}

void IrqMgr_JamMesgForClient(IrqMgr* self, OSMesg msg) {
    IrqMgrClient* iter = self->clients;

    while (iter != NULL) {
        if (iter->queue->validCount >= iter->queue->msgCount) {
            // "irqmgr_JamMesgForClient: Message queue is overflowing mq=%08x cnt=%d"
            osSyncPrintf(
                VT_COL(RED, WHITE) "irqmgr_JamMesgForClient:メッセージキューがあふれています mq=%08x cnt=%d\n" VT_RST,
                iter->queue, iter->queue->validCount);
        } else {
            // mistake? the function's name suggests it would use osJamMesg
            osSendMesg(iter->queue, msg, OS_MESG_NOBLOCK);
        }
        iter = iter->prev;
    }
}

void IrqMgr_HandlePreNMI(IrqMgr* self) {
    u64 temp = STATUS_PRENMI; // required to match

    gIrqMgrResetStatus = temp;
    self->resetStatus = STATUS_PRENMI;
    sIrqMgrResetTime = self->resetTime = osGetTime();

    osSetTimer(&self->timer, OS_USEC_TO_CYCLES(450000), 0ull, &self->queue, (OSMesg)PRENMI450_MSG);
    IrqMgr_JamMesgForClient(self, (OSMesg) & self->prenmiMsg);
}

void IrqMgr_CheckStack() {
    osSyncPrintf("irqmgr.c: PRENMIから0.5秒経過\n"); // "0.5 seconds after PRENMI"
    if (StackCheck_Check(NULL) == 0) {
        osSyncPrintf("スタックは大丈夫みたいです\n"); // "The stack looks ok"
    } else {
        osSyncPrintf("%c", 7);
        osSyncPrintf(VT_FGCOL(RED));
        // "Stack overflow or dangerous"
        osSyncPrintf("スタックがオーバーフローしたか危険な状態です\n");
        // "Increase stack size early or don't consume stack"
        osSyncPrintf("早々にスタックサイズを増やすか、スタックを消費しないようにしてください\n");
        osSyncPrintf(VT_RST);
    }
}

void IrqMgr_HandlePRENMI450(IrqMgr* self) {
    u64 temp = STATUS_NMI; // required to match
    gIrqMgrResetStatus = temp;
    self->resetStatus = STATUS_NMI;

    osSetTimer(&self->timer, OS_USEC_TO_CYCLES(30000), 0ull, &self->queue, (OSMesg)PRENMI480_MSG);
    IrqMgr_SendMesgForClient(self, (OSMesg) & self->nmiMsg);
}

void IrqMgr_HandlePRENMI480(IrqMgr* self) {
    u32 ret;

    osSetTimer(&self->timer, OS_USEC_TO_CYCLES(20000), 0ull, &self->queue, (OSMesg)PRENMI500_MSG);
    ret = osAfterPreNMI();
    if (ret) {
        // "osAfterPreNMI returned %d !?"
        osSyncPrintf("osAfterPreNMIが %d を返しました！？\n", ret);
        osSetTimer(&self->timer, OS_USEC_TO_CYCLES(1000), 0ull, &self->queue, (OSMesg)PRENMI480_MSG);
    }
}

void IrqMgr_HandlePRENMI500(IrqMgr* self) {
    IrqMgr_CheckStack();
}

void IrqMgr_HandleRetrace(IrqMgr* self) {
    if (gIrqMgrRetraceTime == 0ull) {
        if (self->retraceTime == 0) {
            self->retraceTime = osGetTime();
        } else {
            gIrqMgrRetraceTime = osGetTime() - self->retraceTime;
        }
    }
    sIrqMgrRetraceCount++;
    IrqMgr_SendMesgForClient(self, (OSMesg) & self->retraceMsg);
}

void IrqMgr_ThreadEntry(void* arg0) {
    OSMesg msg;
    IrqMgr* self = (IrqMgr*)arg0;
    u8 exit;

    msg = 0;
    osSyncPrintf("ＩＲＱマネージャスレッド実行開始\n"); // "Start IRQ manager thread execution"
    exit = false;

    while (!exit) {
        osRecvMesg(&self->queue, &msg, OS_MESG_BLOCK);
        switch ((u32)msg) {
            case RETRACE_MSG:
                IrqMgr_HandleRetrace(self);
                break;
            case PRE_NMI_MSG:
                osSyncPrintf("PRE_NMI_MSG\n");
                // "Scheduler: Receives PRE_NMI message"
                osSyncPrintf("スケジューラ：PRE_NMIメッセージを受信\n");
                IrqMgr_HandlePreNMI(self);
                break;
            case PRENMI450_MSG:
                osSyncPrintf("PRENMI450_MSG\n");
                // "Scheduler: Receives PRENMI450 message"
                osSyncPrintf("スケジューラ：PRENMI450メッセージを受信\n");
                IrqMgr_HandlePRENMI450(self);
                break;
            case PRENMI480_MSG:
                osSyncPrintf("PRENMI480_MSG\n");
                // "Scheduler: Receives PRENMI480 message"
                osSyncPrintf("スケジューラ：PRENMI480メッセージを受信\n");
                IrqMgr_HandlePRENMI480(self);
                break;
            case PRENMI500_MSG:
                osSyncPrintf("PRENMI500_MSG\n");
                // "Scheduler: Receives PRENMI500 message"
                osSyncPrintf("スケジューラ：PRENMI500メッセージを受信\n");
                exit = true;
                IrqMgr_HandlePRENMI500(self);
                break;
            default:
                // "Unexpected message received"
                osSyncPrintf("irqmgr.c:予期しないメッセージを受け取りました(%08x)\n", msg);
                break;
        }
    }

    osSyncPrintf("ＩＲＱマネージャスレッド実行終了\n"); // "End of IRQ manager thread execution"
}

void IrqMgr_Init(IrqMgr* self, void* stack, OSPri pri, u8 retraceCount) {
    LogUtils_CheckNullPointer("self", self, "../irqmgr.c", 346);
    LogUtils_CheckNullPointer("stack", stack, "../irqmgr.c", 347);

    self->clients = NULL;
    self->retraceMsg.type = OS_SC_RETRACE_MSG;
    self->prenmiMsg.type = OS_SC_PRE_NMI_MSG;
    self->nmiMsg.type = OS_SC_NMI_MSG;
    self->resetStatus = STATUS_IDLE;
    self->resetTime = 0;

    osCreateMesgQueue(&self->queue, self->msgBuf, ARRAY_COUNT(self->msgBuf));
    osSetEventMesg(OS_EVENT_PRENMI, &self->queue, (OSMesg)PRE_NMI_MSG);
    osViSetEvent(&self->queue, (OSMesg)RETRACE_MSG, retraceCount);
    osCreateThread(&self->thread, 0x13, IrqMgr_ThreadEntry, self, stack, pri);
    osStartThread(&self->thread);
}
