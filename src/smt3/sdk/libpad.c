#include "sdk_libpad_layout.h"

/* padPortState is the per-port state array, padRpcPacket the RPC packet shared by
 * every call into the IOP module, padRpcClient its sceSif client data, and
 * padInitialized the "libpad is initialised" flag. padStateNames and padRequestNames are
 * the state and request name tables, D_0054AA80 the empty string both
 * stringify functions fall back to. */
extern padState padPortState[2][4];
extern rpcClient padRpcClient[2];
extern padRpc padRpcPacket;
extern s32 padInitialized __attribute__((section(".data")));

extern const char *padStateNames[8] __attribute__((section(".data")));
extern const char *padRequestNames[4] __attribute__((section(".data")));

extern void SyncDCache(void *start, void *end);
extern void *memcpy(void *a, const void *b, size_t c);
extern char *strcpy(char *dst, const char *src);
extern s32 sceSifCallRpc(void *cd, s32 fno, s32 mode, void *send, s32 ssize,
                         void *recv, s32 rsize, void *endfunc, void *endpara);
extern int _send_to_iop(int port, int slot);

/* padCmdBlock is the command-block array the IOP DMAs from, padDebugPrint the
 * debug-print flag used by initialization and command diagnostics. */
typedef struct sceSifDmaData {
    void *src;
    void *dest;
    s32 size;
    s32 attr;
} sceSifDmaData;

extern padCmd padCmdBlock[2][4];
extern s32 padDebugPrint __attribute__((section(".data")));
extern void *memset(void *a, int b, size_t c);
extern int scePrintf(const char *fmt, ...);
extern s32 sceSifDmaStat(u32 id);
extern u32 sceSifSetDma(sceSifDmaData *data, s32 count);

extern const char padDmaFailMsg[];

int _send_to_iop(int port, int slot)
{
    padCmd *c = padPortState[port][slot].cmd;
    sceSifDmaData dma[16];
    int id;
    u32 seq;
    void *destination;

    if (padPortState[port][slot].dmaId != 0) {
        if (sceSifDmaStat(padPortState[port][slot].dmaId) >= 0) {
            if (padDebugPrint != 0) {
                scePrintf(padDmaFailMsg);
            }
            return 0;
        }
    }
    seq = c->seq + 1;
    destination = EE_ADDR_AS(void *, (padPortState[port][slot].iopAddr + ((seq & 1) << 5)));
    c->seq = seq;
    SyncDCache(c, (u8 *)c + 0x20);
    dma[0].dest = destination;
    dma[0].src = c;
    dma[0].size = 0x20;
    dma[0].attr = 0;
    id = sceSifSetDma(dma, 1);
    if (id == 0) {
        if (padDebugPrint != 0) {
            scePrintf(padDmaFailMsg);
        }
        return 0;
    }
    padPortState[port][slot].dmaId = id;
    return 1;
}

extern s32 sceSifBindRpc(rpcClient *client, u32 service, s32 mode);
extern int scePadGetModVersion(void);
extern int scePadInit2(int mode);

const char padDmaFailMsg[72] = "libpad: sceSifSetDma faild\n\000\000\000\000\000libpad: tPadDma Structure Invalid\n";

int scePadInit(int mode)
{
    s32 delay;
    s32 version;
    s32 major;

    padInitialized = 1;
    for (;;) {
        sceSifBindRpc(&padRpcClient[0], 0x80000100, 0);
        if (padRpcClient[0].server != NULL) {
            break;
        }
        for (delay = 0x10000; --delay != -1;) {
        }
    }
    for (;;) {
        sceSifBindRpc(&padRpcClient[1], 0x80000101, 0);
        if (padRpcClient[1].server != NULL) {
            break;
        }
        for (delay = 0x10000; --delay != -1;) {
        }
    }
    version = scePadGetModVersion();
    major = version >> 8;
    if (major != 4) {
        if (padDebugPrint != 0) {
            scePrintf("libpad: Module version mismatch ");
            scePrintf("[libpad.a = %d.%d, padman.irx = %d.%d]\n",
                      4, 0, major, version & 0xFF);
        }
        return 0;
    }
    return scePadInit2(mode);
}

int scePadInit2(int mode)
{
    int i;

    for (i = 0; i < 4; i++) {
        padPortState[0][i].open = 0;
        padPortState[0][i].f18 = 0;
        padPortState[0][i].f14 = 0;
        padPortState[1][i].open = 0;
        padPortState[1][i].f18 = 0;
        padPortState[1][i].f14 = 0;
    }
    padRpcPacket.cmd = 0x10;
    padRpcPacket.a.w[1] = 0;
    if (sceSifCallRpc(padRpcClient, 1, 0, &padRpcPacket, 0x80, &padRpcPacket, 0x80, 0, 0) < 0) {
        return 0;
    }
    return padRpcPacket.a.w[0];
}

int scePadEnd(void)
{
    padRpcPacket.cmd = 0xF;
    if (sceSifCallRpc(padRpcClient, 1, 0, &padRpcPacket, 0x80, &padRpcPacket, 0x80, 0, 0) < 0) {
        return 0;
    }
    if (padRpcPacket.a.w[0] == 1) {
        padInitialized = 0;
    }
    return padRpcPacket.a.w[0];
}

/* NON_MATCHING: 120 words against retail's 122; the saved-register pair is
 * swapped as in _send_to_iop, and the double-buffer loop runs one register
 * short. */
#ifdef NON_MATCHING
int scePadPortOpen(int port, int slot, void *addr)
{
    padDma *d;
    padCmd *c;
    int i;

    if ((EE_ADDR(addr) & 0x3F) != 0) {
        if (padDebugPrint != 0) {
            scePrintf("libpad: buffer addr is not 64 byte align. %08x\n", addr);
        }
        return 0;
    }
    if (padPortState[port][slot].open == 1) {
        if (padDebugPrint != 0) {
            scePrintf("libpad: pad port is already open [%d][%d]\n", port, slot);
        }
        return 0;
    }
    d = (padDma *)addr;
    for (i = 0; i < 2; i++) {
        d->frame = 0;
        d->state = 5;
        d->reqState = 2;
        d->f67 = 0;
        memset(d, 0xFF, 0x20);
        d->length = 0;
        d++;
    }
    padRpcPacket.cmd = 1;
    padRpcPacket.port = port;
    padRpcPacket.slot = slot;
    padRpcPacket.a.w[1] = EE_ADDR_S(addr);
    if (sceSifCallRpc(padRpcClient, 1, 0, &padRpcPacket, 0x80, &padRpcPacket, 0x80, 0, 0) < 0) {
        return 0;
    }
    c = &padCmdBlock[port][slot];
    padPortState[port][slot].open = 1;
    c->seq = 0;
    padPortState[port][slot].dmaId = 0;
    padPortState[port][slot].iopAddr = padRpcPacket.a.w[2];
    padPortState[port][slot].dma = (padDma *)addr;
    padPortState[port][slot].cmd = c;
    return padRpcPacket.a.w[0];
}
#else
/* Native diagnostic formats used by the retained function. */
const char D_0054A9F8[] = "libpad: buffer addr is not 64 byte align. %08x\n";
const char D_0054AA28[] = "libpad: pad port is already open [%d][%d]\n";

INCLUDE_ASM(const s32, "sdk/libpad", scePadPortOpen);
#endif

int scePadPortClose(int port, int slot)
{
    if (padPortState[port][slot].open == 0) {
        return 0;
    }
    padRpcPacket.cmd = 0xE;
    padRpcPacket.port = port;
    padRpcPacket.slot = slot;
    padRpcPacket.a.w[1] = 1;
    if (sceSifCallRpc(padRpcClient, 1, 0, &padRpcPacket, 0x80, &padRpcPacket, 0x80, 0, 0) < 0) {
        return 0;
    }
    padPortState[port][slot].open = 0;
    return padRpcPacket.a.w[0];
}

/* The IOP writes the newer frame counter into whichever half it filled last,
 * so the reader picks the half with the higher count. */
padDma *scePadGetDmaStr(int port, int slot)
{
    padDma *d = padPortState[port][slot].dma;
    int newer;

    SyncDCache(d, d + 2);
    newer = d[0].frame < d[1].frame;
    return (padDma *)((u8 *)d + (newer << 7));
}

u32 scePadGetFrameCount(int port, int slot)
{
    if (padPortState[port][slot].open == 0) {
        return 0;
    }
    return scePadGetDmaStr(port, slot)->frame;
}

int scePadRead(int port, int slot, u8 *data)
{
    padDma *d;

    if (padPortState[port][slot].open == 0) {
        return 0;
    }
    d = scePadGetDmaStr(port, slot);
    memcpy(data, d, d->length);
    return d->length;
}

int scePadGetState(int port, int slot)
{
    padDma *d;

    if (padPortState[port][slot].open == 0) {
        return 0x63;
    }
    d = scePadGetDmaStr(port, slot);
    if (d->state == 6) {
        if (d->reqState == 2) {
            return 5;
        }
    }
    return d->state;
}

const char padStateNameError[8] __attribute__((section(".rodata"))) = "ERROR";

const char padStateNameStable[8] __attribute__((section(".rodata"))) = "STABLE";

const char padStateNameExecCmd[8] __attribute__((section(".rodata"))) = "EXECCMD";

const char padStateNameFindCtp1[16] = "FINDCTP1";

void scePadStateIntToStr(u32 state, char *str)
{
    if (state < 8) {
        strcpy(str, padStateNames[state]);
    } else {
        strcpy(str, "");
    }
}

int scePadSetReqState(int port, int slot, int state)
{
    if (padPortState[port][slot].open == 0) {
        return 0;
    }
    scePadGetDmaStr(port, slot)->reqState = state;
    return 1;
}

int scePadGetReqState(int port, int slot)
{
    if (padPortState[port][slot].open == 0) {
        return 0;
    }
    return scePadGetDmaStr(port, slot)->reqState;
}

void scePadReqIntToStr(u32 req, char *str)
{
    if (req < 4) {
        strcpy(str, padRequestNames[req]);
    } else {
        strcpy(str, "");
    }
}

int scePadInfoAct(int port, int slot, int actno, int term)
{
    padDma *d;

    if (padPortState[port][slot].open == 0) {
        return 0;
    }
    d = scePadGetDmaStr(port, slot);
    if (d->ok != 1) {
        return 0;
    }
    if (d->ok2 < 2) {
        return 0;
    }
    if (actno >= d->nrAct) {
        return 0;
    }
    if (actno == -1) {
        return d->nrAct;
    }
    switch (term) {
    case 1:
        return d->actInfo[actno][0];
    case 2:
        return d->actInfo[actno][1];
    case 3:
        return d->actInfo[actno][2];
    case 4:
        return d->actInfo[actno][3];
    }
    return 0;
}

int scePadInfoComb(int port, int slot, int listno, int offs)
{
    padDma *d;

    if (padPortState[port][slot].open == 0) {
        return 0;
    }
    d = scePadGetDmaStr(port, slot);
    if (d->ok != 1) {
        return 0;
    }
    if (d->ok2 < 2) {
        return 0;
    }
    if (listno == -1) {
        return d->nrComb;
    }
    if (listno >= d->nrComb) {
        return 0;
    }
    switch (offs) {
    case -1:
        return d->combInfo[listno][0];
    case 0:
        return d->combInfo[listno][1];
    case 1:
        return d->combInfo[listno][2];
    case 2:
        return d->combInfo[listno][3];
    }
    return 0;
}

int scePadInfoMode(int port, int slot, int term, int offs)
{
    padDma *d;

    if (padPortState[port][slot].open == 0) {
        return 0;
    }
    d = scePadGetDmaStr(port, slot);
    if (d->ok != 1) {
        return 0;
    }
    if (d->reqState == 2) {
        return 0;
    }
    switch (term) {
    case 1:
        if (d->modeCurId == 0xF3) {
            return 0;
        }
        return d->modeCurId >> 4;
    case 2:
        if (d->ok2 == 1) {
            return 0;
        }
        return d->modeTable[d->modeCurOffs];
    case 3:
        if (d->ok2 == 1) {
            return 0;
        }
        return d->modeCurOffs;
    case 4:
        if (d->ok2 == 1) {
            return 0;
        }
        if (offs == -1) {
            return d->nrMode;
        }
        if (offs >= d->nrMode) {
            return 0;
        }
        return d->modeTable[offs];
    }
    return 0;
}

int scePadSetMainMode(int port, int slot, int offs, int lock)
{
    padRpcPacket.a.w[0] = offs;
    padRpcPacket.a.w[1] = lock;
    padRpcPacket.cmd = 6;
    padRpcPacket.port = port;
    padRpcPacket.slot = slot;
    if (sceSifCallRpc(padRpcClient, 1, 0, &padRpcPacket, 0x80, &padRpcPacket, 0x80, 0, 0) < 0) {
        return 0;
    }
    if (padRpcPacket.a.w[2] == 1) {
        scePadSetReqState(port, slot, 2);
    }
    return padRpcPacket.a.w[2];
}

int scePadSetActDirect(int port, int slot, const u8 *data)
{
    padCmd *c;
    int i;
    int ret;

    if (scePadGetDmaStr(port, slot)->ok != 1) {
        return 0;
    }
    c = padPortState[port][slot].cmd;
    for (i = 0; i < 6; i++) {
        c->data[i] = data[i];
    }
    c->cmd = 1;
    c->len = 6;
    ret = 1;
    if (_send_to_iop(port, slot) != 1) {
        ret = 0;
    }
    return ret;
}

int scePadSetActAlign(int port, int slot, const u8 *align)
{
    padRpc *rpc = &padRpcPacket;
    int i;

    padRpcPacket.cmd = 8;
    rpc->port = port;
    rpc->slot = slot;
    for (i = 0; i < 6; i++) {
        rpc->a.b[i] = align[i];
    }
    if (sceSifCallRpc(padRpcClient, 1, 0, rpc, 0x80, rpc, 0x80, 0, 0) < 0) {
        return 0;
    }
    if (rpc->a.w[2] == 1) {
        scePadSetReqState(port, slot, 2);
    }
    return rpc->a.w[2];
}

u32 scePadGetButtonMask(int port, int slot)
{
    padDma *d;

    if (padPortState[port][slot].open == 0) {
        return 0;
    }
    d = scePadGetDmaStr(port, slot);
    if (d->ok != 1) {
        return 0;
    }
    if (d->ok2 < 2) {
        return 0;
    }
    if (d->f66 < 2) {
        return 0;
    }
    return (long)d->mask[0] + ((long)d->mask[1] << 8) + ((long)d->mask[2] << 16) +
           ((long)d->mask[3] << 24);
}

int scePadSetButtonInfo(int port, int slot, u32 buttonInfo)
{
    padRpcPacket.a.w[0] = buttonInfo;
    padRpcPacket.cmd = 0xA;
    padRpcPacket.port = port;
    padRpcPacket.slot = slot;
    if (sceSifCallRpc(padRpcClient, 1, 0, &padRpcPacket, 0x80, &padRpcPacket, 0x80, 0, 0) < 0) {
        return 0;
    }
    if (padRpcPacket.a.w[1] == 1) {
        scePadSetReqState(port, slot, 2);
    }
    return padRpcPacket.a.w[1];
}

int scePadInfoPressMode(int port, int slot)
{
    if (padPortState[port][slot].open == 0) {
        return 0;
    }
    return scePadGetButtonMask(port, slot) == 0x3FFFF;
}

int scePadEnterPressMode(int port, int slot)
{
    if (padPortState[port][slot].open == 0) {
        return 0;
    }
    return scePadSetButtonInfo(port, slot, 0xFFF);
}

int scePadExitPressMode(int port, int slot)
{
    if (padPortState[port][slot].open == 0) {
        return 0;
    }
    return scePadSetButtonInfo(port, slot, 0);
}

int scePadSetVrefParam(int port, int slot, const vrefParam *param)
{
    padRpcPacket.port = port;
    padRpcPacket.slot = slot;
    padRpcPacket.cmd = 0xB;
    padRpcPacket.a.vref = *param;
    if (sceSifCallRpc(padRpcClient, 1, 0, &padRpcPacket, 0x80, &padRpcPacket, 0x80, 0, 0) < 0) {
        return 0;
    }
    if (padRpcPacket.a.w[4] == 1) {
        scePadSetReqState(port, slot, 2);
    }
    return padRpcPacket.a.w[4];
}

int scePadGetPortMax(void)
{
    padRpcPacket.cmd = 0xC;
    if (sceSifCallRpc(padRpcClient, 1, 0, &padRpcPacket, 0x80, &padRpcPacket, 0x80, 0, 0) < 0) {
        return 0;
    }
    return padRpcPacket.a.w[0];
}

int scePadGetSlotMax(int port)
{
    padRpcPacket.port = port;
    padRpcPacket.cmd = 0xD;
    if (sceSifCallRpc(padRpcClient, 1, 0, &padRpcPacket, 0x80, &padRpcPacket, 0x80, 0, 0) < 0) {
        return 0;
    }
    return padRpcPacket.a.w[0];
}

int scePadGetModVersion(void)
{
    padRpcPacket.cmd = 0x12;
    if (sceSifCallRpc(padRpcClient, 1, 0, &padRpcPacket, 0x80, &padRpcPacket, 0x80, 0, 0) < 0) {
        return 0;
    }
    return padRpcPacket.a.w[0];
}

int scePadSetWarningLevel(int level)
{
    padRpcPacket.port = level;
    padRpcPacket.cmd = 0x14;
    if (sceSifCallRpc(padRpcClient, 1, 0, &padRpcPacket, 0x80, &padRpcPacket, 0x80, 0, 0) < 0) {
        return 0;
    }
    return padRpcPacket.slot;
}

const char padStateNameDisconnect[16] = "DISCONNECT";

const char padRequestNameBusy[8] __attribute__((section(".rodata"))) = "BUSY";

const char padRequestNameFailed[8] __attribute__((section(".rodata"))) = "FAILED";

const char padRequestNameComplete[16] = "COMPLETE";

