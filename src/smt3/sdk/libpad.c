#include "common.h"

/* One half of a port's double-buffered pad data DMA buffer. */
typedef struct padDma {
    u8 data[0x30];     /* 0x00 button data, scePadRead copies length bytes */
    u8 actInfo[4][4];  /* 0x30 */
    u8 combInfo[4][4]; /* 0x40 */
    u16 modeTable[4];  /* 0x50 */
    s32 frame;         /* 0x58 */
    u8 pad5c[4];
    u32 length;        /* 0x60 */
    u8 ok2;            /* 0x64 */
    u8 modeCurId;      /* 0x65 */
    u8 f66;            /* 0x66 */
    u8 f67;
    u8 nrMode;         /* 0x68 */
    u8 modeCurOffs;    /* 0x69 */
    u8 nrAct;          /* 0x6A */
    u8 nrComb;         /* 0x6B */
    u8 pad6c[4];
    u8 state;          /* 0x70 */
    u8 reqState;       /* 0x71 */
    u8 ok;             /* 0x72 */
    u8 pad73[6];
    u8 mask[4];        /* 0x79 */
    u8 pad7d[3];
} padDma;

/* The 0x20-byte command block DMAd to the IOP. */
typedef struct padCmd {
    u32 seq;       /* 0x00 */
    u32 cmd;       /* 0x04 */
    u32 len;       /* 0x08 */
    u8 data[0x14]; /* 0x0C */
} padCmd;

typedef struct padState {
    padDma *dma; /* 0x00 */
    padCmd *cmd; /* 0x04 */
    u32 iopAddr; /* 0x08 */
    s32 dmaId;   /* 0x0C */
    s32 open;    /* 0x10 */
    s32 f14;
    s32 f18;
} padState;

typedef union padRpcArgs {
    s32 w[5];
    u8 b[0x14];
} padRpcArgs;

typedef struct padRpc {
    s32 cmd;      /* 0x00 */
    s32 port;     /* 0x04 */
    s32 slot;     /* 0x08 */
    padRpcArgs a; /* 0x0C */
    u8 pad20[0x60];
} padRpc;

typedef struct vrefParam {
    u8 b[12];
} vrefParam;

/* D_005989D0 is the per-port state array, D_00598BC0 the RPC packet shared by
 * every call into the IOP module, D_00598980 its sceSif client data, and
 * D_00529FA8 the "libpad is initialised" flag. D_00529FB0 and D_00529FD0 are
 * the state and request name tables, D_0054AA80 the empty string both
 * stringify functions fall back to. */
extern padState D_005989D0[2][4] __attribute__((section(".data")));
extern u8 D_00598980[0x18] __attribute__((section(".data")));
extern padRpc D_00598BC0 __attribute__((section(".data")));
extern s32 D_00529FA8 __attribute__((section(".data")));

extern const char *D_00529FB0[8] __attribute__((section(".data")));
extern const char *D_00529FD0[4] __attribute__((section(".data")));

extern void SyncDCache(void *start, void *end);
extern void *memcpy(void *dst, const void *src, u32 len);
extern char *strcpy(char *dst, const char *src);
extern s32 sceSifCallRpc(void *cd, u32 fno, u32 mode, void *send, s32 ssize,
                         void *recv, s32 rsize, void *endfunc, void *endpara);
extern int _send_to_iop(int port, int slot);

INCLUDE_ASM(const s32, "sdk/libpad", _send_to_iop);

INCLUDE_ASM(const s32, "sdk/libpad", scePadInit);

int scePadInit2(int mode)
{
    int i;

    for (i = 0; i < 4; i++) {
        D_005989D0[0][i].open = 0;
        D_005989D0[0][i].f18 = 0;
        D_005989D0[0][i].f14 = 0;
        D_005989D0[1][i].open = 0;
        D_005989D0[1][i].f18 = 0;
        D_005989D0[1][i].f14 = 0;
    }
    D_00598BC0.cmd = 0x10;
    D_00598BC0.a.w[1] = 0;
    if (sceSifCallRpc(D_00598980, 1, 0, &D_00598BC0, 0x80, &D_00598BC0, 0x80, 0, 0) < 0) {
        return 0;
    }
    return D_00598BC0.a.w[0];
}

int scePadEnd(void)
{
    D_00598BC0.cmd = 0xF;
    if (sceSifCallRpc(D_00598980, 1, 0, &D_00598BC0, 0x80, &D_00598BC0, 0x80, 0, 0) < 0) {
        return 0;
    }
    if (D_00598BC0.a.w[0] == 1) {
        D_00529FA8 = 0;
    }
    return D_00598BC0.a.w[0];
}

INCLUDE_ASM(const s32, "sdk/libpad", scePadPortOpen);

int scePadPortClose(int port, int slot)
{
    if (D_005989D0[port][slot].open == 0) {
        return 0;
    }
    D_00598BC0.cmd = 0xE;
    D_00598BC0.port = port;
    D_00598BC0.slot = slot;
    D_00598BC0.a.w[1] = 1;
    if (sceSifCallRpc(D_00598980, 1, 0, &D_00598BC0, 0x80, &D_00598BC0, 0x80, 0, 0) < 0) {
        return 0;
    }
    D_005989D0[port][slot].open = 0;
    return D_00598BC0.a.w[0];
}

/* The IOP writes the newer frame counter into whichever half it filled last,
 * so the reader picks the half with the higher count. */
padDma *scePadGetDmaStr(int port, int slot)
{
    padDma *d = D_005989D0[port][slot].dma;
    int newer;

    SyncDCache(d, d + 2);
    newer = d[0].frame < d[1].frame;
    return (padDma *)((u8 *)d + (newer << 7));
}

u32 scePadGetFrameCount(int port, int slot)
{
    if (D_005989D0[port][slot].open == 0) {
        return 0;
    }
    return scePadGetDmaStr(port, slot)->frame;
}

int scePadRead(int port, int slot, u8 *data)
{
    padDma *d;

    if (D_005989D0[port][slot].open == 0) {
        return 0;
    }
    d = scePadGetDmaStr(port, slot);
    memcpy(data, d, d->length);
    return d->length;
}

int scePadGetState(int port, int slot)
{
    padDma *d;

    if (D_005989D0[port][slot].open == 0) {
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

INCLUDE_RODATA(const s32, "sdk/libpad", D_0054AA58);

INCLUDE_RODATA(const s32, "sdk/libpad", D_0054AA60);

INCLUDE_RODATA(const s32, "sdk/libpad", D_0054AA68);

INCLUDE_RODATA(const s32, "sdk/libpad", D_0054AA70);

void scePadStateIntToStr(u32 state, char *str)
{
    if (state < 8) {
        strcpy(str, D_00529FB0[state]);
    } else {
        strcpy(str, "");
    }
}

int scePadSetReqState(int port, int slot, int state)
{
    if (D_005989D0[port][slot].open == 0) {
        return 0;
    }
    scePadGetDmaStr(port, slot)->reqState = state;
    return 1;
}

int scePadGetReqState(int port, int slot)
{
    if (D_005989D0[port][slot].open == 0) {
        return 0;
    }
    return scePadGetDmaStr(port, slot)->reqState;
}

void scePadReqIntToStr(u32 req, char *str)
{
    if (req < 4) {
        strcpy(str, D_00529FD0[req]);
    } else {
        strcpy(str, "");
    }
}

int scePadInfoAct(int port, int slot, int actno, int term)
{
    padDma *d;

    if (D_005989D0[port][slot].open == 0) {
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

    if (D_005989D0[port][slot].open == 0) {
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

    if (D_005989D0[port][slot].open == 0) {
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
    D_00598BC0.a.w[0] = offs;
    D_00598BC0.a.w[1] = lock;
    D_00598BC0.cmd = 6;
    D_00598BC0.port = port;
    D_00598BC0.slot = slot;
    if (sceSifCallRpc(D_00598980, 1, 0, &D_00598BC0, 0x80, &D_00598BC0, 0x80, 0, 0) < 0) {
        return 0;
    }
    if (D_00598BC0.a.w[2] == 1) {
        scePadSetReqState(port, slot, 2);
    }
    return D_00598BC0.a.w[2];
}

int scePadSetActDirect(int port, int slot, const u8 *data)
{
    padCmd *c;
    int i;
    int ret;

    if (scePadGetDmaStr(port, slot)->ok != 1) {
        return 0;
    }
    c = D_005989D0[port][slot].cmd;
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
    padRpc *rpc = &D_00598BC0;
    int i;

    D_00598BC0.cmd = 8;
    rpc->port = port;
    rpc->slot = slot;
    for (i = 0; i < 6; i++) {
        rpc->a.b[i] = align[i];
    }
    if (sceSifCallRpc(D_00598980, 1, 0, rpc, 0x80, rpc, 0x80, 0, 0) < 0) {
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

    if (D_005989D0[port][slot].open == 0) {
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
    D_00598BC0.a.w[0] = buttonInfo;
    D_00598BC0.cmd = 0xA;
    D_00598BC0.port = port;
    D_00598BC0.slot = slot;
    if (sceSifCallRpc(D_00598980, 1, 0, &D_00598BC0, 0x80, &D_00598BC0, 0x80, 0, 0) < 0) {
        return 0;
    }
    if (D_00598BC0.a.w[1] == 1) {
        scePadSetReqState(port, slot, 2);
    }
    return D_00598BC0.a.w[1];
}

int scePadInfoPressMode(int port, int slot)
{
    if (D_005989D0[port][slot].open == 0) {
        return 0;
    }
    return scePadGetButtonMask(port, slot) == 0x3FFFF;
}

int scePadEnterPressMode(int port, int slot)
{
    if (D_005989D0[port][slot].open == 0) {
        return 0;
    }
    return scePadSetButtonInfo(port, slot, 0xFFF);
}

int scePadExitPressMode(int port, int slot)
{
    if (D_005989D0[port][slot].open == 0) {
        return 0;
    }
    return scePadSetButtonInfo(port, slot, 0);
}

int scePadSetVrefParam(int port, int slot, const vrefParam *param)
{
    D_00598BC0.port = port;
    D_00598BC0.slot = slot;
    D_00598BC0.cmd = 0xB;
    *(vrefParam *)D_00598BC0.a.b = *param;
    if (sceSifCallRpc(D_00598980, 1, 0, &D_00598BC0, 0x80, &D_00598BC0, 0x80, 0, 0) < 0) {
        return 0;
    }
    if (D_00598BC0.a.w[4] == 1) {
        scePadSetReqState(port, slot, 2);
    }
    return D_00598BC0.a.w[4];
}

int scePadGetPortMax(void)
{
    D_00598BC0.cmd = 0xC;
    if (sceSifCallRpc(D_00598980, 1, 0, &D_00598BC0, 0x80, &D_00598BC0, 0x80, 0, 0) < 0) {
        return 0;
    }
    return D_00598BC0.a.w[0];
}

int scePadGetSlotMax(int port)
{
    D_00598BC0.port = port;
    D_00598BC0.cmd = 0xD;
    if (sceSifCallRpc(D_00598980, 1, 0, &D_00598BC0, 0x80, &D_00598BC0, 0x80, 0, 0) < 0) {
        return 0;
    }
    return D_00598BC0.a.w[0];
}

int scePadGetModVersion(void)
{
    D_00598BC0.cmd = 0x12;
    if (sceSifCallRpc(D_00598980, 1, 0, &D_00598BC0, 0x80, &D_00598BC0, 0x80, 0, 0) < 0) {
        return 0;
    }
    return D_00598BC0.a.w[0];
}

int scePadSetWarningLevel(int level)
{
    D_00598BC0.port = level;
    D_00598BC0.cmd = 0x14;
    if (sceSifCallRpc(D_00598980, 1, 0, &D_00598BC0, 0x80, &D_00598BC0, 0x80, 0, 0) < 0) {
        return 0;
    }
    return D_00598BC0.slot;
}

INCLUDE_RODATA(const s32, "sdk/libpad", D_0054AA88);

INCLUDE_RODATA(const s32, "sdk/libpad", D_0054AA98);

INCLUDE_RODATA(const s32, "sdk/libpad", D_0054AAA0);

INCLUDE_RODATA(const s32, "sdk/libpad", D_0054AAA8);

