#include "common.h"
#include "basic.h"
#include "model.h"
#include "newdata.h"
#include "scr.h"
#include "field.h"
#include "ps2asm.h"
#include "billboard.h"

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
    s8 spdx;
    s8 spdy;
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

extern u32 dds3GetRandA(void *name);

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


/* NON_MATCHING: 107 of 109 instruction words match at the native 436-byte size. */
#ifdef NON_MATCHING
extern void fldSnowSeedInit(void);

void fldSnowInit(void) {

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
        s32 floor;
        floor = fldGlobalBase.floor;
        if (floor == 0xE) {
            return;
        }
        gFldSnowMode = 0;
        gFldSnowZ = gFldSnowX = -640.0f;
        gFldSnowY = -6900.0f;
        gFldSnowSpd = 4.0f;
        gFldSnowLife = 0x5DC;
        gFldSnowW = 5.0f;
        if (floor == 0) {
            gFldSnowYD = 10.0f;
            goto initSeeds;
        }
    } else if (fldGlobalBase.area == 0x18) {
        s32 floor;
        f32 width;
        f32 y;
        s32 life;

        floor = fldGlobalBase.floor;
        if (floor == 0x11) {
            return;
        }
        if (floor == 0x18) {
            return;
        }
        gFldSnowSpd = -3.0f;
        gFldSnowMode = 1;
        gFldSnowYD = 10000.0f;
        life = 0xF0;
        width = 25.0f;
        if (floor == 5) {
            y = 800.0f;
            life = 0x1C2;
        } else {
            y = 100.0f;
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
        gFldSnowZ = gFldSnowX = -(gFldSnowW * 256.0f * 0.5f);
        goto initSeeds;
    } else {
        return;
    }
    gFldSnowYD = 10000.0f;
initSeeds:
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

void fldSnowSpriteTex(sdfTexHandle_t *th) {
    void *tag = sdfGrAllocPacketBuff(0x20);
    void *header;
    void *packet;
    f32 mat[16];
    sdfGrOt_t *ot;

    sdfDrawTagInit(tag);
    header = sdfGrAllocPacketBuff(0x20);
    sdf3DMakeDirectRefPacket(header, sdfTexGetPacket(th), sdfTexGetPacketLength(th));
    sdfDrawTagLinkRef(tag, header);
    SetRotateMatrixZ(0.0f);
    SDF_VU0_STOREMATRIX(mat);
    packet = sdfGrAllocPacketBuff(0x38);
    sdfBillBoardMakeTMPacket(packet, mat);
    sdfDrawTagLink(tag, packet);
    ot = &sdfGrOtTable[44];
    ((void (*)(sdfGrOt_t *, void *))ot->regFunc)(ot, tag);
}

void fldSnowSprite3D(const Sprite3D_t *spr3d) {
    void *tag = sdfGrAllocPacketBuff(0x20);
    void *packet;
    sdfGrOt_t *ot;

    sdfDrawTagInit(tag);
    packet = sdfGrAllocPacketBuff(0x80);
    sdfBillBoardMake1(packet, &spr3d->pos, spr3d->col, spr3d->uv, spr3d->vt);
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


extern fldUnitControl_t *fldGetUnitControl(void);
extern void dds3GetPosPoseSet(dds3Basic_t *basic);
extern void *memset(void *dst, s32 value, u32 size);

void fldSnowSeedReset(void)
{
    fldSeedWork_t *seed;
    s32 i;
    s32 attempt;
    /* Shared spawn X survives the second RNG call in both modes. Qualified
     * cc1probe --alloc --no-dumps assigns its pseudo 87 to $f20; see
     * docs/cc1probe.md. */
    f32 x;
    f32 drift;

    if (gFldSeedBuff == NULL) {
        return;
    }

    if (gFldSnowMode == 0) {
        for (i = 0; i < gFldSnowLife; i++) {
            for (attempt = 0; attempt < 2; attempt++) {
                if ((dds3GetRandA(NULL) & 3) == 0) {
                    u32 randomValue;
                    f32 z;

                    randomValue = dds3GetRandA(NULL) & 0xFF;
                    x = gFldSnowX + (f32)randomValue * gFldSnowW;
                    randomValue = dds3GetRandA(NULL) & 0xFF;
                    z = gFldSnowZ + (f32)randomValue * gFldSnowW;
                    fldInitSnowSeed(x, gFldSnowY, z);
                }
            }

            drift = 0.125f;
            for (seed = gFldSeedBuff, attempt = 0; attempt < 700; attempt++, seed++) {
                if ((seed->flg & 1) != 0) {
                    seed->life--;
                    seed->y += gFldSnowSpd + (f32)(s8)seed->spdx * drift;
                    if (seed->life < 0x40) {
                        seed->alp -= 2;
                    }
                    if (seed->life <= 0) {
                        seed->flg = 0;
                    }
                }
            }
        }
    }

    if (gFldSnowMode != 1) {
        return;
    }

    {
        sdfVector4_t base;
        fldUnitControl_t *control;

        memset(&base, 0, sizeof(base));
        base.w = 1.0f;
        control = fldGetUnitControl();
        if (control != NULL) {
            dds3GetPosPoseSet(control->handle);
            SDF_VU0_STOREVECTOR_NOMEM(&base);
        }

        for (i = 0; i < gFldSnowLife * 2; i++) {
            for (attempt = 0; attempt < 12; attempt++) {
                if ((dds3GetRandA(NULL) & 3) == 0) {
                    u32 randomValue;
                    f32 z;

                    fldGlobalBase.x = base.x;
                    fldGlobalBase.z = base.z;
                    randomValue = dds3GetRandA(NULL) & 0xFF;
                    x = base.x + gFldSnowX + (f32)randomValue * gFldSnowW;
                    randomValue = dds3GetRandA(NULL) & 0xFF;
                    z = base.z + gFldSnowZ + (f32)randomValue * gFldSnowW;
                    fldInitSnowSeed(x, gFldSnowY, z);
                }
            }

            drift = 0.125f;
            for (seed = gFldSeedBuff, attempt = 0; attempt < 700; attempt++, seed++) {
                if ((seed->flg & 1) != 0) {
                    seed->life--;
                    seed->y += gFldSnowSpd - (f32)(s8)seed->spdx * drift;
                    if (seed->life < 0x40) {
                        seed->alp -= 2;
                    }
                    if (seed->life <= 0) {
                        seed->flg = 0;
                    }
                }
            }
        }
    }
}

#ifdef NON_MATCHING
extern void rsetOtOfs(s32 offset);
extern void rsetTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst);
extern void rsetAlpha(s32 mode);
extern sdfTexHandle_t *fldSnowBlurHandle;
extern volatile sdfVector4_t D_002FC9C0;
extern volatile sdfVector4_t fldCamTarget;

void fldSnowTask(void)
{
    Sprite3D_t sprite;
    volatile sdfVector4_t direction;
    fldSeedWork_t *seed;
    s32 i;
    s32 mode;
    f32 speed;
    f32 cameraX;
    f32 cameraZ;
    f32 eyeX;
    f32 eyeZ;

    if (gFldSeedBuff == NULL) {
        return;
    }
    if (gFldSnowMode == 0) {
        for (i = 0; i < 2; i++) {
            if ((dds3GetRandA(NULL) & 3) == 0) {
                u32 random = dds3GetRandA(NULL) & 0xFF;
                f32 x = gFldSnowX + (f32)random * gFldSnowW;
                f32 z;
                random = dds3GetRandA(NULL) & 0xFF;
                z = gFldSnowZ + (f32)random * gFldSnowW;
                fldInitSnowSeed(x, gFldSnowY, z);
            }
        }
    } else {
        for (i = 0; i < 12; i++) {
            if ((dds3GetRandA(NULL) & 3) == 0) {
                u32 random = dds3GetRandA(NULL) & 0xFF;
                f32 x = fldGlobalBase.x + gFldSnowX + (f32)random * gFldSnowW;
                f32 z;
                random = dds3GetRandA(NULL) & 0xFF;
                z = fldGlobalBase.z + gFldSnowZ + (f32)random * gFldSnowW;
                fldInitSnowSeed(x, gFldSnowY, z);
            }
        }
    }
    rsetOtOfs(0x2C);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 2);
    rsetAlpha(1);
    cameraZ = fldCamTarget.z;
    cameraX = fldCamTarget.x;
    eyeZ = D_002FC9C0.z;
    eyeX = D_002FC9C0.x;
    direction.x = eyeX - cameraX;
    direction.y = 0.0f;
    direction.z = eyeZ - cameraZ;
    direction.w = 1.0f;
    sprite.uv[0].u = 0;
    sprite.uv[0].v = 0;
    sprite.uv[1].u = 0x100;
    sprite.uv[1].v = 0;
    sprite.uv[2].u = 0x100;
    sprite.uv[2].v = 0x100;
    sprite.uv[3].u = 0;
    sprite.uv[3].v = 0x100;
    sprite.vt[0].x = -12.0f;
    sprite.vt[0].y = -12.0f;
    sprite.vt[1].x = 12.0f;
    sprite.vt[1].y = -12.0f;
    sprite.vt[2].x = 12.0f;
    sprite.vt[2].y = 12.0f;
    sprite.vt[3].x = -12.0f;
    sprite.vt[3].y = 12.0f;
    sprite.rot = 0.0f;
    mode = gFldSnowMode;
    if (mode == 0) {
        speed = gFldSnowSpd;
        seed = gFldSeedBuff;
        for (i = 0; i < 700; i++, seed++) {
            if (seed->flg & 1) {
                seed->life--;
                seed->y += speed + (f32)seed->spdx * 0.125f;
                if (seed->life < 0x40) {
                    seed->alp -= 2;
                }
                if (seed->life <= 0) {
                    seed->flg = 0;
                }
            }
        }
        fldSnowSpriteTex(fldSnowBlurHandle);
        for (i = 0; i < 700; i++) {
            volatile fldSeedWork_t *item = &gFldSeedBuff[i];
            if ((item->flg & 1) && item->y < gFldSnowYD) {
                s32 color;
                s32 alpha;
                sprite.pos.x = item->x;
                color = 0x80 - item->spdy;
                alpha = item->alp;
                sprite.pos.y = item->y;
                sprite.col = ((u32)alpha << 24) | (u32)color | ((u32)color << 8) | ((u32)color << 16);
                sprite.pos.z = item->z;
                fldSnowSprite3D(&sprite);
            }
        }
        mode = gFldSnowMode;
    }
    if (mode == 1) {
        speed = gFldSnowSpd;
        seed = gFldSeedBuff;
        for (i = 0; i < 700; i++, seed++) {
            if (seed->flg & 1) {
                seed->life--;
                seed->y += speed - (f32)seed->spdx * 0.125f;
                if (seed->life < 0x40) {
                    seed->alp -= 2;
                }
                if (seed->life <= 0) {
                    seed->flg = 0;
                }
            }
        }
        fldSnowSpriteTex(fldSnowBlurHandle);
        for (i = 0; i < 700; i++) {
            volatile fldSeedWork_t *item = &gFldSeedBuff[i];
            if ((item->flg & 1) && item->y < gFldSnowYD) {
                s32 color;
                s32 alpha;
                sprite.pos.x = item->x;
                color = 0x80 - item->spdy;
                alpha = item->alp;
                sprite.pos.y = item->y;
                sprite.col = ((u32)alpha << 24) | (u32)color | ((u32)color << 8) | ((u32)color << 16);
                sprite.pos.z = item->z;
                fldSnowSprite3D(&sprite);
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "field/fldSnow", fldSnowTask);
#endif
