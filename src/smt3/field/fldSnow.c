#include "common.h"
#include "basic.h"
#include "model.h"
#include "newdata.h"
#include "scr.h"
#include "field.h"
#include "ps2asm.h"

enum sl_slot_flag
{
    SL_SLOT_FLAG_CAN_READ_SAVE = 1,
    SL_SLOT_FLAG_DIRECTORY_READY = 2,
    SL_SLOT_FLAG_DELETE_BEFORE_WRITE = 8
};

extern s32 fldSnowActive;

sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

void *sdfMemLock(sdfMemHandle_t *aHandle);

extern s32 (*proc_func_ans)(dds3ProcessID_t *process);

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
    s16 flg;
    u8 spdx;
    u8 spdy;
    s16 life;
    s16 alp;
} fldSeedWork_t;

extern fldSeedWork_t *gFldSeedBuff;

extern sdfMemHandle_t *fldSnowSeedBuf;

extern void sdfMemUnlock(sdfMemHandle_t *aHandle);

extern void sdfMemFreeDelayedDirect(sdfMemHandle_t *aHandle);

extern s32 evtBitCheck(s32 no);

extern void *sdfGrAllocPacketBuff(s32 aSize);

extern void sdfDrawTagInit(void *a);

extern void sdfDrawTagLink(void *aTag, void *aPacket);

extern void sdfBillBoardMake1(void *aPacket, const sdfVector4_t *aCenter, u32 aColor,
                              const void *aUVList, const void *aXYOffsetList);

extern sdfGrOt_t sdfGrOtTable[85];

extern s32 gFldSnowLife;

extern s32 gFldSnowMode;

extern f32 gFldSnowX;

extern f32 gFldSnowY;

extern f32 gFldSnowZ;

extern f32 gFldSnowYD;

extern f32 gFldSnowSpd;

extern f32 gFldSnowW;

extern void fldSnowSeedReset(void);

extern s32 dds3GetRandA(s32 name);

extern u32 sdfTexGetPacket(sdfTexHandle_t *aHandle);

extern s32 sdfTexGetPacketLength(sdfTexHandle_t *aHandle);

extern void sdf3DMakeDirectRefPacket(void *aPacket, u32 aData, s32 aLeng);

extern void sdfDrawTagLinkRef(void *aTag, void *aPacket);

extern void SetRotateMatrixZ(f32 aAngle);

extern void sdfBillBoardMakeTMPacket(void *aPacket, const f32 *aTM);


void fldSetSnowActive(void) {
    if (fldSnowActive == 0) {
        fldSnowActive = 1;
    }
}


/* NON_MATCHING: 28 of 110 words, two words over; blocks are ordered differently. */
#ifdef NON_MATCHING
void fldSnowInit(void) {
    s32 floor;

    if (gFldSeedBuff != NULL) {
        return;
    }
    if (evtBitCheck(0x16) == 0) {
        return;
    }
    if (evtBitCheck(0x3E) != 0) {
        return;
    }
    if (fldGlobalBase.area == 0x1F) {
        floor = fldGlobalBase.floor;
        if (floor == 0xE) {
            return;
        }
        gFldSnowZ = -640.0f;
        gFldSnowY = -6900.0f;
        gFldSnowSpd = 4.0f;
        gFldSnowLife = 0x5DC;
        gFldSnowW = 5.0f;
        gFldSnowMode = 0;
        gFldSnowX = -640.0f;
        if (floor == 0) {
            gFldSnowYD = 10.0f;
        } else {
            gFldSnowYD = 10000.0f;
        }
    } else if (fldGlobalBase.area == 0x18) {
        f32 width = 25.0f;
        f32 y;
        s32 life;

        floor = fldGlobalBase.floor;
        if (floor == 0x11) {
            return;
        }
        if (floor == 0x18) {
            return;
        }
        gFldSnowYD = 10000.0f;
        gFldSnowMode = 1;
        gFldSnowSpd = -3.0f;
        if (floor == 5) {
            y = 800.0f;
            life = 0x1C2;
        } else {
            y = 100.0f;
            life = 0xF0;
        }
        gFldSnowY = y;
        if (floor == 9) {
            gFldSnowLife = 0x12C;
            width = 23.0f;
        } else {
            gFldSnowLife = life;
        }
        if (floor == 0x15) {
            gFldSnowW = 15.0f;
        } else {
            gFldSnowW = width;
        }
        gFldSnowZ = -(gFldSnowW * 256.0f * 0.5f);
        gFldSnowX = gFldSnowZ;
    } else {
        return;
    }
    fldSnowSeedBuf = sdfMemAllocDirect(0x36B0);
    gFldSeedBuff = sdfMemLock(fldSnowSeedBuf);
    fldSnowSeedInit();
    fldSnowSeedReset();
}
#else
INCLUDE_ASM(const s32, "field/fldSnow", fldSnowInit);
#endif


void fldFreeSnowSeed(void) {
    if (gFldSeedBuff != NULL) {
        sdfMemUnlock(fldSnowSeedBuf);
        sdfMemFreeDelayedDirect(fldSnowSeedBuf);
        fldSnowSeedBuf = NULL;
        gFldSeedBuff = NULL;
    }
}

void fldSnowSpriteTex(sdfTexHandle_t *texture) {
    void *tag = sdfGrAllocPacketBuff(0x20);
    void *header;
    void *packet;
    f32 mat[16];
    sdfGrOt_t *ot;

    sdfDrawTagInit(tag);
    header = sdfGrAllocPacketBuff(0x20);
    sdf3DMakeDirectRefPacket(header, sdfTexGetPacket(texture), sdfTexGetPacketLength(texture));
    sdfDrawTagLinkRef(tag, header);
    SetRotateMatrixZ(0.0f);
    SDF_VU0_STOREMATRIX(mat);
    packet = sdfGrAllocPacketBuff(0x38);
    sdfBillBoardMakeTMPacket(packet, mat);
    sdfDrawTagLink(tag, packet);
    ot = &sdfGrOtTable[44];
    ((void (*)(sdfGrOt_t *, void *))ot->regFunc)(ot, tag);
}

void fldSnowSprite3D(const sdfVector4_t *work) {
    void *tag = sdfGrAllocPacketBuff(0x20);
    void *packet;
    sdfGrOt_t *ot;

    sdfDrawTagInit(tag);
    packet = sdfGrAllocPacketBuff(0x80);
    sdfBillBoardMake1(packet, work, *(u32 *)((u8 *)work + 0x40), (u8 *)work + 0x10,
                      (u8 *)work + 0x20);
    sdfDrawTagLink(tag, packet);
    ot = &sdfGrOtTable[44];
    ((void (*)(sdfGrOt_t *, void *))ot->regFunc)(ot, tag);
}

void fldSnowSeedInit(void) {
    s32 i;

    if (gFldSeedBuff != NULL) {
        for (i = 0; i < 700; i++) {
            gFldSeedBuff[i].flg = 0;
        }
    }
}

void fldInitSnowSeed(f32 x, f32 y, f32 z) {
    s32 i;

    if (gFldSeedBuff == NULL) {
        return;
    }
    for (i = 0; i < 700; i++) {
        if (gFldSeedBuff[i].flg == 0) {
            gFldSeedBuff[i].x = x;
            gFldSeedBuff[i].y = y;
            gFldSeedBuff[i].z = z;
            gFldSeedBuff[i].flg = 1;
            gFldSeedBuff[i].life = gFldSnowLife;
            gFldSeedBuff[i].alp = 0x80;
            gFldSeedBuff[i].spdx = dds3GetRandA(0) & 0x1F;
            gFldSeedBuff[i].spdy = dds3GetRandA(0) & 0xF;
            return;
        }
    }
}

void func_00151018(s32 index) {
    if (gFldSeedBuff != NULL) {
        gFldSeedBuff[index].flg = 0;
    }
}

INCLUDE_ASM(const s32, "field/fldSnow", fldSnowSeedReset);

INCLUDE_ASM(const s32, "field/fldSnow", fldSnowTask);
