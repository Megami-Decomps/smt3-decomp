#ifndef SDK_LIBPAD_LAYOUT_H
#define SDK_LIBPAD_LAYOUT_H

#include "sdk_rpc_client_layout.h"

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

typedef struct vrefParam {
    u8 b[12];
} vrefParam;

typedef union padRpcArgs {
    s32 w[5];
    u8 b[0x14];
    vrefParam vref;
} padRpcArgs;

typedef struct padRpc {
    s32 cmd;      /* 0x00 */
    s32 port;     /* 0x04 */
    s32 slot;     /* 0x08 */
    padRpcArgs a; /* 0x0C */
    u8 pad20[0x60];
} padRpc;

SIZE_CHECK(padDma_size, padDma, 0x80);
SIZE_CHECK(padState_size, padState, 0x1C);
SIZE_CHECK(padCmd_size, padCmd, 0x20);
SIZE_CHECK(padRpc_size, padRpc, 0x80);

#endif
