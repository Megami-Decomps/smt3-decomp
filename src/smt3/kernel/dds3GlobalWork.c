#include "common.h"
#include "basic.h"
#include "kernel.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "newdata.h"
#include "field.h"

extern s32 evtBitCheck(s32 no);

extern void dds3GlobalWorkUserInit2(s32 mode);

extern void dds3GlobalWorkClearFlags(void);

extern void evtInitGlobalWork(void);

extern void datInitPlayer(void);

extern void datInitBossPress(void);

extern void dds3ClearStockTable(void);

extern void frInitNameData(void);

extern void fclEncycInitRecord(s32 mode);

extern void NewGame2Push(void);

extern void NewGame2Pop(void);

extern void *sdfMemAllocDirect(s32 aSize);

extern void *sdfMemGetPtr(sdfMemHandle_t *aHandle);

extern void fldGbInit_System(s32 mode);

extern void evtBitOn(s32 no);

typedef void (*sdfGrOtRegFunc_t)(sdfGrOt_t *ot, sdfDrawTag_t *tag);

/* The font node frFont hands back. It accumulates the drawn string's width at
 * +0xC, which the centring path in dds3FontReqAddString reads. */
typedef struct
{
    u8 unk00[0xC];
    s32 width;
} _FRQ;

typedef struct dds3FontReq_t dds3FontReq_t;

/* destroy runs when the request leaves the list, draw once per frame while it
 * is still on it. */
typedef struct
{
    void (*destroy)(dds3FontReq_t *req);
    void (*draw)(dds3FontReq_t *req);
} dds3FontReqFunc_t;

struct dds3FontReq_t
{
    dds3FontReq_t *prev;
    dds3FontReq_t *next;
    s32 group;
    dds3FontReqFunc_t *func;
};

typedef struct
{
    dds3FontReq_t req;
    _FRQ *frq;
} dds3FontReqString_t;

typedef struct
{
    dds3FontReq_t req;
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    u32 z;
    u32 color;
} dds3FontReqBox_t;

typedef struct
{
    dds3FontReq_t *first;
    dds3FontReq_t *last;
} dds3FontReqList_t;

extern dds3FontReqList_t D_00551C80;

extern dds3FontReqFunc_t dds3FontReqTable;

extern dds3FontReqFunc_t D_0054EA30;

extern sdfGrOt_t sdfGrOtTable[];

extern s8 slNewGameBit2208, slNewGameBit2209, slNewGameBit2210, slNewGameBit2211;

extern s8 slNewGameBit2212, slNewGameBit2213, slNewGameBit2323, slNewGameBit162;

extern s8 slNewGameBit2561[0x18];

extern u16 D_0054EA16;

extern s32 D_0054EA18;

extern sdfMemHandle_t *D_0054EA1C;

extern sdfMemHandle_t *D_00551C7C;

extern void *sdfChipAllocZeroDirect(s32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern void *sdfGrAllocPacketBuff(s32 aSize);

extern s32 sdfDrawTagInit(sdfDrawTag_t *a);

extern sdfDrawTag_t *sdfPrimAllocDrawTag(void *aAllocator);

extern void sdfPrimLinkBoxLine(sdfDrawTag_t *aTag, u32 aColor, u32 aPrim, s32 aX0, s32 aY0, s32 aX1,
                               s32 aY1, u32 aZ, void *aAllocator);

extern s32 frDrawFont(_FRQ *pFRQ);

extern void frSetFontColorNo(_FRQ *pFRQ, u8 ColorNo);

extern _FRQ *frSetFontString(char *str, s32 style, s32 colorno, s32 tp, _FRQ *frq, s32 mode);

extern void frSetDrawPosition(_FRQ *pFRQ, s32 X, s32 Y);

extern _FRQ *frDestroyFontRequestAfter(_FRQ *pFRQ);

INCLUDE_ASM(const s32, "kernel/dds3GlobalWork", dds3GlobalWorkInit);

void dds3GlobalWorkUserInit(void) {
}

void NewGame2Push(void)
{
    s32 i;

    slNewGameBit2208 = 0;
    slNewGameBit2209 = 0;
    slNewGameBit2210 = 0;
    slNewGameBit2211 = 0;
    slNewGameBit2212 = 0;
    slNewGameBit2213 = 0;
    slNewGameBit2323 = 0;
    slNewGameBit162 = 0;
    D_0054EA16 = 0;
    D_0054EA18 = 0;
    if (evtBitCheck(0x8A0) != 0) {
        slNewGameBit2208 = 1;
    }
    if (evtBitCheck(0x8A1) != 0) {
        slNewGameBit2209 = 1;
    }
    if (evtBitCheck(0x8A2) != 0) {
        slNewGameBit2210 = 1;
    }
    if (evtBitCheck(0x8A3) != 0) {
        slNewGameBit2211 = 1;
    }
    if (evtBitCheck(0x8A4) != 0) {
        slNewGameBit2212 = 1;
    }
    if (evtBitCheck(0x8A5) != 0) {
        slNewGameBit2213 = 1;
    }
    if (evtBitCheck(0x913) != 0) {
        slNewGameBit2323 = 1;
    }
    if (evtBitCheck(0xA2) != 0) {
        slNewGameBit162 = 1;
    }
    D_0054EA16 = DDS3_GBWK->half0C;
    D_0054EA18 = DDS3_GBWK->unk3C;
    D_0054EA1C = sdfMemAllocDirect(0x7E90);
    memcpy(sdfMemGetPtr(D_0054EA1C), (u8 *)DDS3_GBWK + 0x11E80, 0x7E90);
    D_00551C7C = sdfMemAllocDirect(0xA0);
    memcpy(sdfMemGetPtr(D_00551C7C), DDS3_GBWK->boss_press, 0xA0);
    for (i = 0; i < 0x18; i++) {
        slNewGameBit2561[i] = 0;
    }
    if (evtBitCheck(0xA01) != 0) {
        slNewGameBit2561[0] = 1;
    }
    if (evtBitCheck(0xA03) != 0) {
        slNewGameBit2561[1] = 1;
    }
    if (evtBitCheck(0xA05) != 0) {
        slNewGameBit2561[2] = 1;
    }
    if (evtBitCheck(0xA07) != 0) {
        slNewGameBit2561[3] = 1;
    }
    if (evtBitCheck(0xA09) != 0) {
        slNewGameBit2561[4] = 1;
    }
    if (evtBitCheck(0xA0B) != 0) {
        slNewGameBit2561[5] = 1;
    }
    if (evtBitCheck(0xA0D) != 0) {
        slNewGameBit2561[6] = 1;
    }
    if (evtBitCheck(0xA0F) != 0) {
        slNewGameBit2561[7] = 1;
    }
    if (evtBitCheck(0xA11) != 0) {
        slNewGameBit2561[8] = 1;
    }
    if (evtBitCheck(0xA13) != 0) {
        slNewGameBit2561[9] = 1;
    }
    if (evtBitCheck(0xA15) != 0) {
        slNewGameBit2561[10] = 1;
    }
    if (evtBitCheck(0xA17) != 0) {
        slNewGameBit2561[11] = 1;
    }
    if (evtBitCheck(0xA19) != 0) {
        slNewGameBit2561[12] = 1;
    }
    if (evtBitCheck(0xA1B) != 0) {
        slNewGameBit2561[13] = 1;
    }
    if (evtBitCheck(0xA1D) != 0) {
        slNewGameBit2561[14] = 1;
    }
    if (evtBitCheck(0xA1F) != 0) {
        slNewGameBit2561[15] = 1;
    }
    if (evtBitCheck(0xA21) != 0) {
        slNewGameBit2561[16] = 1;
    }
    if (evtBitCheck(0xA23) != 0) {
        slNewGameBit2561[17] = 1;
    }
    if (evtBitCheck(0xA25) != 0) {
        slNewGameBit2561[18] = 1;
    }
    if (evtBitCheck(0xA27) != 0) {
        slNewGameBit2561[19] = 1;
    }
    if (evtBitCheck(0xA29) != 0) {
        slNewGameBit2561[20] = 1;
    }
    if (evtBitCheck(0xA2B) != 0) {
        slNewGameBit2561[21] = 1;
    }
    if (evtBitCheck(0xA2D) != 0) {
        slNewGameBit2561[22] = 1;
    }
    if (evtBitCheck(0xA2F) != 0) {
        slNewGameBit2561[23] = 1;
    }
}

void NewGame2Pop(void)
{
    s32 i;

    if (slNewGameBit2208 != 0) {
        evtBitOn(0x8A0);
    }
    if (slNewGameBit2209 != 0) {
        evtBitOn(0x8A1);
    }
    if (slNewGameBit2210 != 0) {
        evtBitOn(0x8A2);
    }
    if (slNewGameBit2211 != 0) {
        evtBitOn(0x8A3);
    }
    if (slNewGameBit2212 != 0) {
        evtBitOn(0x8A4);
    }
    if (slNewGameBit2213 != 0) {
        evtBitOn(0x8A5);
    }
    if (slNewGameBit2323 != 0) {
        evtBitOn(0x913);
    }
    if (slNewGameBit162 != 0) {
        evtBitOn(0xA2);
    }
    DDS3_GBWK->unk3C = D_0054EA18;
    DDS3_GBWK->word08 = D_0054EA18;
    DDS3_GBWK->half0C = D_0054EA16;
    memcpy((u8 *)DDS3_GBWK + 0x11E80, sdfMemGetPtr(D_0054EA1C), 0x7E90);
    sdfMemFreeDirect(D_0054EA1C);
    memcpy(DDS3_GBWK->boss_press, sdfMemGetPtr(D_00551C7C), 0xA0);
    sdfMemFreeDirect(D_00551C7C);
    if (slNewGameBit2561[0] != 0) {
        evtBitOn(0xA01);
    }
    if (slNewGameBit2561[1] != 0) {
        evtBitOn(0xA03);
    }
    if (slNewGameBit2561[2] != 0) {
        evtBitOn(0xA05);
    }
    if (slNewGameBit2561[3] != 0) {
        evtBitOn(0xA07);
    }
    if (slNewGameBit2561[4] != 0) {
        evtBitOn(0xA09);
    }
    if (slNewGameBit2561[5] != 0) {
        evtBitOn(0xA0B);
    }
    if (slNewGameBit2561[6] != 0) {
        evtBitOn(0xA0D);
    }
    if (slNewGameBit2561[7] != 0) {
        evtBitOn(0xA0F);
    }
    if (slNewGameBit2561[8] != 0) {
        evtBitOn(0xA11);
    }
    if (slNewGameBit2561[9] != 0) {
        evtBitOn(0xA13);
    }
    if (slNewGameBit2561[10] != 0) {
        evtBitOn(0xA15);
    }
    if (slNewGameBit2561[11] != 0) {
        evtBitOn(0xA17);
    }
    if (slNewGameBit2561[12] != 0) {
        evtBitOn(0xA19);
    }
    if (slNewGameBit2561[13] != 0) {
        evtBitOn(0xA1B);
    }
    if (slNewGameBit2561[14] != 0) {
        evtBitOn(0xA1D);
    }
    if (slNewGameBit2561[15] != 0) {
        evtBitOn(0xA1F);
    }
    if (slNewGameBit2561[16] != 0) {
        evtBitOn(0xA21);
    }
    if (slNewGameBit2561[17] != 0) {
        evtBitOn(0xA23);
    }
    if (slNewGameBit2561[18] != 0) {
        evtBitOn(0xA25);
    }
    if (slNewGameBit2561[19] != 0) {
        evtBitOn(0xA27);
    }
    if (slNewGameBit2561[20] != 0) {
        evtBitOn(0xA29);
    }
    if (slNewGameBit2561[21] != 0) {
        evtBitOn(0xA2B);
    }
    if (slNewGameBit2561[22] != 0) {
        evtBitOn(0xA2D);
    }
    if (slNewGameBit2561[23] != 0) {
        evtBitOn(0xA2F);
    }
}

void dds3GlobalWorkUserInit2(s32 mode)
{
    s32 saved;

    if (mode == 1) {
        NewGame2Push();
        saved = DDS3_GBWK->memHandle;
        memset(DDS3_GBWK, 0, 0x1A510);
        DDS3_GBWK->memHandle = saved;
    }
    DDS3_GBWK->unk38 = 0;
    DDS3_GBWK->unk3C = 0;
    dds3GlobalWorkClearFlags();
    evtInitGlobalWork();
    DDS3_GBWK->akuma[0].unk00 = 8;
    datInitPlayer();
    datInitBossPress();
    dds3ClearStockTable();
    DDS3_GBWK->heartsequip = 0;
    DDS3_GBWK->heartscnt = 0;
    frInitNameData();
    fldGbInit_System(0);
    fclEncycInitRecord(0);
    if (mode == 1) {
        NewGame2Pop();
    }
}

void dds3FontReqListInsert(dds3FontReqList_t *list, void *node, s32 linkofs)
{
    dds3FontReq_t *link = (dds3FontReq_t *)((u8 *)node + linkofs);
    dds3FontReq_t *last = list->last;

    if (last == NULL) {
        list->first = node;
    } else {
        ((dds3FontReq_t *)((u8 *)last + linkofs))->next = node;
    }
    link->prev = last;
    link->next = NULL;
    list->last = node;
}

void dds3FontReqListRemove(dds3FontReqList_t *list, void *node, s32 linkofs)
{
    dds3FontReq_t *link = (dds3FontReq_t *)((u8 *)node + linkofs);
    dds3FontReq_t *prev = link->prev;
    dds3FontReq_t *next = link->next;

    if (prev == NULL) {
        list->first = next;
    } else {
        ((dds3FontReq_t *)((u8 *)prev + linkofs))->next = next;
    }
    if (next == NULL) {
        list->last = prev;
    } else {
        ((dds3FontReq_t *)((u8 *)next + linkofs))->prev = prev;
    }
}

void dds3FontReqListAdd(dds3FontReq_t *req, dds3FontReqFunc_t *func)
{
    dds3FontReqListInsert(&D_00551C80, req, 0);
    req->func = func;
}

void dds3FontReqListRunOne(dds3FontReq_t *req)
{
    dds3FontReqListRemove(&D_00551C80, req, 0);
    req->func->destroy(req);
}

void dds3FontReqListRunAll(void)
{
    dds3FontReq_t *req;

    while ((req = D_00551C80.first) != NULL) {
        dds3FontReqListRunOne(req);
    }
}

void func_00117830(dds3FontReq_t *req, s32 group)
{
    req->group = group;
}

void dds3FontReqListRunTo(s32 group)
{
    dds3FontReq_t *req;
    dds3FontReq_t *next;

    for (req = D_00551C80.first; req != NULL; req = next) {
        next = req->next;
        if (req->group == group) {
            dds3FontReqListRunOne(req);
        }
    }
}

void dds3FontReqListCount(void)
{
    dds3FontReq_t *req;

    for (req = D_00551C80.first; req != NULL; req = req->next) {
        req->func->draw(req);
    }
}

void dds3FontReqDestroy(dds3FontReqString_t *req)
{
    frDestroyFontRequestAfter(req->frq);
    sdfChipFreeDirect(req);
}

s32 dds3FontReqDrawFont(dds3FontReqString_t *req)
{
    return frDrawFont(req->frq);
}

dds3FontReq_t *dds3FontReqAddString(s32 x, s32 y, char *str)
{
    dds3FontReqString_t *req;
    _FRQ *frq;

    frq = frSetFontString(str, 0, 0, 0, NULL, 0);
    if (x == 0x800000) {
        x = (0x200 - frq->width) * 8;
    }
    frSetDrawPosition(frq, x, y);
    req = sdfChipAllocZeroDirect(sizeof(dds3FontReqString_t));
    req->frq = frq;
    dds3FontReqListAdd(&req->req, &dds3FontReqTable);
    return &req->req;
}

void dds3FontReqSetColorNo(dds3FontReqString_t *req, u8 colorno)
{
    frSetFontColorNo(req->frq, colorno);
}

void dds3FontReqFree(dds3FontReqBox_t *req)
{
    sdfChipFreeDirect(req);
}

void dds3FontReqDrawBoxLine(dds3FontReqBox_t *req)
{
    sdfDrawTag_t *tag;
    sdfGrOt_t *ot;

    tag = sdfPrimAllocDrawTag(NULL);
    sdfPrimLinkBoxLine(tag, req->color, 0, req->x, req->y, req->x + req->w, req->y + req->h,
                       req->z, NULL);
    ot = &sdfGrOtTable[71];
    ((sdfGrOtRegFunc_t)ot->regFunc)(ot, tag);
}

dds3FontReq_t *dds3FontReqAddBox(s32 x, s32 y, u32 z, s32 w, s32 h, u32 color)
{
    dds3FontReqBox_t *req = sdfChipAllocZeroDirect(sizeof(dds3FontReqBox_t));

    req->x = x;
    req->y = y;
    req->w = w;
    req->h = h;
    req->z = z;
    req->color = color;
    dds3FontReqListAdd(&req->req, &D_0054EA30);
    return &req->req;
}

void func_00117AF8(dds3FontReqBox_t *req, s32 x, s32 y)
{
    req->x = x;
    req->y = y;
}

sdfDrawTag_t *dds3FontReqAllocDrawTag(void)
{
    sdfDrawTag_t *tag = sdfGrAllocPacketBuff(0x20);

    sdfDrawTagInit(tag);
    return tag;
}

u64 *dds3PrimFrameRect(s32 aX, s32 aY, s32 aZ, s32 aW, s32 aH, u32 aBgCol, u32 aFrameCol) {
    u64 *packet;
    s32 right;
    s32 bottom;
    u64 depthHigh;

    packet = (u64 *)sdfGrAllocPacketBuff(0x80);
    packet[0] = 7;
    right = aX + aW;
    bottom = aY + aH;
    depthHigh = (u64)(u32)aZ << 32;
    packet[1] = 0x5000000700000000ULL;
    packet[2] = 0xB400000000008001ULL;
    packet[3] = 0x55555105510ULL;
    packet[4] = 0x146;
    packet[5] = aBgCol | 0x3F80000000000000ULL;
    packet[6] = (u32)(((u32)aX & 0xFFFF) | ((u32)aY << 16)) | depthHigh;
    packet[7] = (u32)(((u32)right & 0xFFFF) | ((u32)bottom << 16)) | depthHigh;
    packet[8] = 0x142;
    packet[9] = aFrameCol | 0x3F80000000000000ULL;
    packet[14] = (u32)(((u32)(aX - 0x10) & 0xFFFF) | ((u32)(aY - 8) << 16)) | depthHigh;
    packet[10] = packet[14];
    packet[11] = (u32)(((u32)right & 0xFFFF) | ((u32)(aY - 8) << 16)) | depthHigh;
    packet[12] = (u32)(((u32)right & 0xFFFF) | ((u32)bottom << 16)) | depthHigh;
    packet[13] = (u32)(((u32)(aX - 0x10) & 0xFFFF) | ((u32)bottom << 16)) | depthHigh;
    packet[15] = 0;
    return packet;
}

u64 *dds3PrimLine(s32 aX0, s32 aY0, s32 aZ0, s32 aColor0, s32 aX1, s32 aY1, s32 aZ1, s32 aColor1, s32 aMode) {
    u64 *packet = (u64 *)sdfGrAllocPacketBuff(0x50);

    packet[0] = 4;
    packet[1] = 0x5000000400000000ULL;
    packet[2] = 0x5400000000008001ULL;
    packet[3] = 0x51510;
    packet[4] = aMode | 0x109;
    packet[5] = (u32)aColor0 | ((u64)0xFE00 << 46);
    packet[6] = (u32)((aX0 & 0xFFFF) | (aY0 << 16)) | ((u64)((u32)aZ0 >> 4) << 32);
    packet[7] = (u32)aColor1 | ((u64)0xFE00 << 46);
    packet[8] = (u32)((aX1 & 0xFFFF) | (aY1 << 16)) | ((u64)((u32)aZ1 >> 4) << 32);
    packet[9] = 0;
    return packet;
}

INCLUDE_ASM(const s32, "kernel/dds3GlobalWork", dds3PrimLine3D);
INCLUDE_SDATA(const s32, "kernel/dds3GlobalWork", D_0054EA08);

INCLUDE_SDATA(const s32, "kernel/dds3GlobalWork", slNewGameBit2211);

INCLUDE_SDATA(const s32, "kernel/dds3GlobalWork", slNewGameBit2212);

INCLUDE_SDATA(const s32, "kernel/dds3GlobalWork", slNewGameBit2213);

INCLUDE_SDATA(const s32, "kernel/dds3GlobalWork", slNewGameBit2323);

INCLUDE_SDATA(const s32, "kernel/dds3GlobalWork", slNewGameBit162);

INCLUDE_SDATA(const s32, "kernel/dds3GlobalWork", D_0054EA15);

INCLUDE_SDATA(const s32, "kernel/dds3GlobalWork", D_0054EA16);

INCLUDE_SDATA(const s32, "kernel/dds3GlobalWork", D_0054EA18);

INCLUDE_SDATA(const s32, "kernel/dds3GlobalWork", D_0054EA1C);

INCLUDE_SDATA(const s32, "kernel/dds3GlobalWork", DDS3_GBWK);

INCLUDE_SDATA(const s32, "kernel/dds3GlobalWork", D_0054EA24);

INCLUDE_SDATA(const s32, "kernel/dds3GlobalWork", dds3FontReqTable);

INCLUDE_SDATA(const s32, "kernel/dds3GlobalWork", D_0054EA2C);

INCLUDE_SDATA(const s32, "kernel/dds3GlobalWork", D_0054EA30);

