#include "common.h"
#include "basic.h"
#include "kernel.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "newdata.h"
#include "field.h"

extern s32 fldGb __attribute__((section(".data")));

extern s32 dds3EffectFldTypeIsSpecial(s32 charId);

extern s32 dds3EffectFldCharToType(s32 charId);

typedef struct {
    s16 fld;
    s16 area;
    u8 unk04[0x18];
} fldNameEntry_t;

extern fldNameEntry_t fldAreaNameTable[];

extern fldNameEntry_t fldPlaceNameTable[];

extern fldNameEntry_t fldFloorNameTable[];

typedef struct {
    s32 gunFlag;                /* 0x00 */
    s32 word04;
    s32 word08;
    s32 word0C;
    s32 word10;
    s32 word14;
    s32 word18;
    s32 word1C;
    s32 word20;
    s32 word24;
    s32 word28;
    s32 skyTable;               /* 0x2C */
    s32 word30;
    s32 filter;                 /* 0x34 */
    s32 word38;
    s32 word3C;
    s32 word40;
    s32 enabled;                /* 0x44 */
    u32 unk48;
    s32 counter;                /* 0x4C */
    s32 word50;
    s32 word54;
    u32 unk58;
    s32 word5C;
} fldDanteGunWork_t;

extern fldDanteGunWork_t fldRunDanteGunFlag;

extern s32 fldPlayerStepDist;

extern s32 sdfFileOpen(const char *path);

extern void sdfFileRead(s32 fd, void *buffer, s32 size);

extern void sdfFileClose(s32 fd);

extern char D_005315D0[];

extern u8 D_0030A670[], D_00311450[], D_00311850[], D_00314100[], D_002FC1A0[];

extern u8 fldTakaraTable[], fldAllCnt[];

extern void fldFindTakaraSlot(s32 id);

extern void fldGbInit_System(s32 mode);

extern void fldGbFlagClear(void);

extern void evtBitOn(s32 flag);

extern void fldAllTblLoad(void);

void fldRunDanteClear(void);

void fldAllTblLoad(void)
{
    s32 fd = sdfFileOpen(D_005315D0);

    sdfFileRead(fd, D_0030A670, 0x4E0);
    sdfFileRead(fd, fldAreaNameTable, 0x2BC0);
    sdfFileRead(fd, fldPlaceNameTable, 0x24C0);
    sdfFileRead(fd, fldFloorNameTable, 0x1880);
    sdfFileRead(fd, D_00311450, 0x400);
    sdfFileRead(fd, D_00311850, 0x800);
    sdfFileRead(fd, D_00314100, 0x2A00);
    sdfFileRead(fd, fldTakaraTable, 0x1600);
    sdfFileRead(fd, D_002FC1A0, 0x80);
    sdfFileRead(fd, fldAllCnt, 0x100);
    sdfFileClose(fd);
}

/* NON_MATCHING: 23 of 73 words differ (first at +0xC). */
#ifdef NON_MATCHING
void fldGbFlagClear(void)
{
    s32 i;
    s32 j;
    s32 k;

    for (i = 0; i < 0x20; i++) {
        DDS3_GBWK->takaraOpen[i] = 0;
    }
    for (i = 0; i < 8; i++) {
        DDS3_GBWK->takaraOpenEx[i] = 0;
    }
    for (k = 0; k < 0x28; k++) {
        for (j = 0; j < 0x20; j++) {
            DDS3_GBWK->gb[k].flags[j].modelFlag = 0;
            DDS3_GBWK->gb[k].flags[j].animeFlag = 0;
            DDS3_GBWK->gb[k].flags[j].atariFlag = 0;
            DDS3_GBWK->gb[k].flags[j].modelFlag2 = 0;
            DDS3_GBWK->gb[k].flags[j].animeFlag2 = 0;
            for (i = 0; i < 0x10; i++) {
                DDS3_GBWK->gb[k].flags[j].animeId2[i] = -1;
            }
            DDS3_GBWK->gb[k].flags[j].npcFlag = 0;
            DDS3_GBWK->gb[k].flags[j].hojiFlag = 0;
        }
    }
    fldAutoMapClear();
}
#else
INCLUDE_ASM(const s32, "field/fldGlobal", fldGbFlagClear);
#endif

void fldGbInit_System(s32 mode)
{
    s32 *p;
    s32 i;
    f32 dist;
    f32 ang;

    fldGlobalBase.area = 0;
    dist = 100.0f;
    ang = 180.0f;
    fldGlobalBase.wordA4 = 0x10;
    fldGlobalBase.floor = 0;
    fldGlobalBase.blockID = 0;
    fldGlobalBase.word10 = 0;
    fldGlobalBase.consumedFlags = 0;
    fldGlobalBase.mode = 0;
    fldGlobalBase.word1C = 0;
    fldGlobalBase.word5C = 0;
    fldGlobalBase.menuOpenFlag = 0;
    fldGlobalBase.encountState = 0;
    fldGlobalBase.playerSeTable = 0;
    fldGlobalBase.word44 = 0;
    fldGlobalBase.hitMatterIndex = -1;
    fldGlobalBase.word4C = -1;
    fldGlobalBase.nameU = -1;
    fldGlobalBase.nameD = -1;
    fldGlobalBase.word98 = -1;
    fldGlobalBase.floatB4 = dist;
    fldGlobalBase.word90 = 1;
    fldGlobalBase.word94 = 0;
    fldGlobalBase.cameraParamIndex = 0;
    fldGlobalBase.sequenceCode = 1;
    fldGlobalBase.reuseAreaResources = 0;
    fldGlobalBase.wordAC = 0;
    fldGlobalBase.wordB0 = 0;
    fldGlobalBase.word78 = 0;
    fldGlobalBase.word80 = 0;
    fldGlobalBase.lightMaIdx = 0;
    fldGlobalBase.wordBC = 0;
    fldGlobalBase.wordC0 = 0;
    fldGlobalBase.wordC8 = 0;
    fldGlobalBase.wordCC = 0;
    fldGlobalBase.wordF8 = 0;
    fldGlobalBase.camAngle2 = ang;
    fldGlobalBase.x = 0.0f;
    fldGlobalBase.y = 0.0f;
    fldGlobalBase.z = 0.0f;
    fldGlobalBase.prevX = 0.0f;
    fldGlobalBase.prevY = 0.0f;
    fldGlobalBase.prevZ = 0.0f;
    fldGlobalBase.hitX = 0.0f;
    fldGlobalBase.hitY = 0.0f;
    fldGlobalBase.hitZ = 0.0f;
    fldGlobalBase.camAngle = 0.0f;
    fldGlobalBase.float12C = 0.0f;
    fldGlobalBase.float130 = 0.0f;
    fldGlobalBase.hitValid = 0;
    fldRunDanteGunFlag.gunFlag = 0;
    fldRunDanteGunFlag.word04 = 0;
    fldRunDanteGunFlag.word08 = 0;
    fldRunDanteGunFlag.word0C = 0;
    fldRunDanteGunFlag.word10 = 0;
    fldRunDanteGunFlag.word14 = 0;
    fldRunDanteGunFlag.word18 = 0;
    fldRunDanteGunFlag.word1C = 0;
    fldRunDanteGunFlag.word20 = 0;
    fldRunDanteGunFlag.word24 = 1;
    fldRunDanteGunFlag.word28 = 1;
    fldRunDanteGunFlag.skyTable = 0;
    fldRunDanteGunFlag.word30 = 0;
    fldRunDanteGunFlag.filter = 0;
    fldRunDanteGunFlag.word38 = 0;
    fldRunDanteGunFlag.word3C = 0;
    fldRunDanteGunFlag.word40 = 0;
    fldRunDanteGunFlag.counter = 1;
    fldRunDanteGunFlag.unk48 = 0;
    fldRunDanteGunFlag.word50 = 0;
    fldRunDanteGunFlag.word5C = 1;
    fldRunDanteGunFlag.unk58 = 0;
    fldRunDanteGunFlag.word54 = 0;
    if (mode == 0) {
        fldGbFlagClear();
        DDS3_GBWK->previewX = 0;
        DDS3_GBWK->previewY = 0;
        p = (s32 *)((u8 *)DDS3_GBWK + 0x11A8C);
        for (i = 14; i >= 0; i--) {
            *p = 0;
            p++;
        }
        DDS3_GBWK->ver[0] = 0x56;
        DDS3_GBWK->ver[1] = 0x45;
        DDS3_GBWK->ver[2] = 0x52;
        DDS3_GBWK->ver[3] = 1;
        DDS3_GBWK->ver[4] = 0;
        DDS3_GBWK->ver[5] = 0;
        DDS3_GBWK->ver[6] = 0;
        DDS3_GBWK->ver[7] = 0;
        DDS3_GBWK->word08 = 0;
        DDS3_GBWK->half0C = 0;
        DDS3_GBWK->transition = 0;
    }
    fldRunDanteClear();
}

void fldResetSequenceParams(void)
{
    f32 dist;

    fldGlobalBase.consumedFlags = 0;
    dist = 100.0f;
    fldGlobalBase.word1C = 0;
    fldGlobalBase.word44 = 0;
    fldGlobalBase.hitMatterIndex = -1;
    fldGlobalBase.word4C = -1;
    fldGlobalBase.word94 = 0;
    fldGlobalBase.wordB0 = 0;
    fldGlobalBase.word78 = 0;
    fldPlayerStepDist = 0;
    fldGlobalBase.word80 = 0;
    fldGlobalBase.wordF8 = 0;
    fldRunDanteGunFlag.gunFlag = 0;
    fldRunDanteGunFlag.word04 = 0;
    fldRunDanteGunFlag.word08 = 0;
    fldRunDanteGunFlag.word0C = 0;
    fldRunDanteGunFlag.word10 = 0;
    fldRunDanteGunFlag.word14 = 0;
    fldRunDanteGunFlag.skyTable = 0;
    fldRunDanteGunFlag.word30 = 0;
    fldRunDanteGunFlag.filter = 0;
    fldRunDanteGunFlag.word38 = 0;
    fldGlobalBase.word90 = 1;
    fldGlobalBase.word98 = -1;
    fldGlobalBase.wordA4 = 0x10;
    fldGlobalBase.floatB4 = dist;
    fldRunDanteGunFlag.word40 = 0;
    fldGlobalBase.wordC8 = 0;
}

INCLUDE_RODATA(const s32, "field/fldGlobal", D_005315D0);

s32 dds3EffectFldCharToType(s32 charId)
{
    if (charId < 0x28) {
        return charId % 100;
    }
    switch (charId) {
    case 0x28:
        return 2;
    case 0x29:
        return 5;
    case 0x2A:
        return 7;
    case 0x2B:
        return 9;
    case 0x2C:
        return 10;
    case 0x2D:
        return 30;
    }
    return 0;
}

s32 dds3EffectFldTypeIsSpecial(s32 type)
{
    switch (type) {
    case 2:
    case 5:
    case 7:
    case 9:
    case 10:
    case 30:
        return 1;
    }
    return 0;
}

void fldSetModelFlag(s32 charId, s32 idx, s32 bit, s32 on)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        if (on != 0) {
            DDS3_GBWK->gb[type].flags[idx].modelFlag |= 1 << bit;
        } else {
            DDS3_GBWK->gb[type].flags[idx].modelFlag &= ~(1 << bit);
        }
        return;
    }
    type = dds3EffectFldCharToType(charId);
    if (on != 0) {
        DDS3_GBWK->gb[type].flags[idx].modelFlag |= 1 << bit;
    } else {
        DDS3_GBWK->gb[type].flags[idx].modelFlag &= ~(1 << bit);
    }
}

s32 fldGetModelFlag(s32 charId, s32 idx, s32 bit)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return 0;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        return ((u32)DDS3_GBWK->gb[type].flags[idx].modelFlag >> bit) & 1;
    }
    type = dds3EffectFldCharToType(charId);
    return ((u32)DDS3_GBWK->gb[type].flags[idx].modelFlag >> bit) & 1;
}

void fldSetAnimeFlag(s32 charId, s32 idx, s32 bit, s32 on)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        if (on != 0) {
            DDS3_GBWK->gb[type].flags[idx].animeFlag |= 1 << bit;
        } else {
            DDS3_GBWK->gb[type].flags[idx].animeFlag &= ~(1 << bit);
        }
        return;
    }
    type = dds3EffectFldCharToType(charId);
    if (on != 0) {
        DDS3_GBWK->gb[type].flags[idx].animeFlag |= 1 << bit;
    } else {
        DDS3_GBWK->gb[type].flags[idx].animeFlag &= ~(1 << bit);
    }
}

s32 fldGetAnimeFlag(s32 charId, s32 idx, s32 bit)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return 0;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        return ((u32)DDS3_GBWK->gb[type].flags[idx].animeFlag >> bit) & 1;
    }
    type = dds3EffectFldCharToType(charId);
    return ((u32)DDS3_GBWK->gb[type].flags[idx].animeFlag >> bit) & 1;
}

void fldSetAtariFlag(s32 charId, s32 idx, s32 bit, s32 on)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        if (on != 0) {
            DDS3_GBWK->gb[type].flags[idx].atariFlag |= 1 << bit;
        } else {
            DDS3_GBWK->gb[type].flags[idx].atariFlag &= ~(1 << bit);
        }
        return;
    }
    type = dds3EffectFldCharToType(charId);
    if (on != 0) {
        DDS3_GBWK->gb[type].flags[idx].atariFlag |= 1 << bit;
    } else {
        DDS3_GBWK->gb[type].flags[idx].atariFlag &= ~(1 << bit);
    }
}

s32 fldGbChkHit(s32 charId, s32 idx, s32 bit)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return 0;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        return ((u32)DDS3_GBWK->gb[type].flags[idx].atariFlag >> bit) & 1;
    }
    type = dds3EffectFldCharToType(charId);
    return ((u32)DDS3_GBWK->gb[type].flags[idx].atariFlag >> bit) & 1;
}

void fldSetModelFlag2(s32 charId, s32 idx, s32 bit, s32 on)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        if (on != 0) {
            DDS3_GBWK->gb[type].flags[idx].modelFlag2 |= 1 << bit;
        } else {
            DDS3_GBWK->gb[type].flags[idx].modelFlag2 &= ~(1 << bit);
        }
        return;
    }
    type = dds3EffectFldCharToType(charId);
    if (on != 0) {
        DDS3_GBWK->gb[type].flags[idx].modelFlag2 |= 1 << bit;
    } else {
        DDS3_GBWK->gb[type].flags[idx].modelFlag2 &= ~(1 << bit);
    }
}

s32 fldGetModelFlag2(s32 charId, s32 idx, s32 bit)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return 0;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        return ((u32)DDS3_GBWK->gb[type].flags[idx].modelFlag2 >> bit) & 1;
    }
    type = dds3EffectFldCharToType(charId);
    return ((u32)DDS3_GBWK->gb[type].flags[idx].modelFlag2 >> bit) & 1;
}

void fldSetAnimeFlag2(s32 charId, s32 idx, s32 bit, s32 on)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        if (on != 0) {
            DDS3_GBWK->gb[type].flags[idx].animeFlag2 |= 1 << bit;
        } else {
            DDS3_GBWK->gb[type].flags[idx].animeFlag2 &= ~(1 << bit);
        }
        return;
    }
    type = dds3EffectFldCharToType(charId);
    if (on != 0) {
        DDS3_GBWK->gb[type].flags[idx].animeFlag2 |= 1 << bit;
    } else {
        DDS3_GBWK->gb[type].flags[idx].animeFlag2 &= ~(1 << bit);
    }
}

s32 fldGetAnimeFlag2(s32 charId, s32 idx, s32 bit)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return 0;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        return ((u32)DDS3_GBWK->gb[type].flags[idx].animeFlag2 >> bit) & 1;
    }
    type = dds3EffectFldCharToType(charId);
    return ((u32)DDS3_GBWK->gb[type].flags[idx].animeFlag2 >> bit) & 1;
}

void fldSetAnimeId2(s32 charId, s32 idx, s32 slot, s32 id)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        DDS3_GBWK->gb[type].flags[idx].animeId2[slot] = id;
        return;
    }
    type = dds3EffectFldCharToType(charId);
    DDS3_GBWK->gb[type].flags[idx].animeId2[slot] = id;
}

s32 fldGetAnimeId2(s32 charId, s32 idx, s32 slot)
{
    s32 type;
    u8 id;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return -1;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        id = DDS3_GBWK->gb[type].flags[idx].animeId2[slot];
    } else {
        type = dds3EffectFldCharToType(charId);
        id = DDS3_GBWK->gb[type].flags[idx].animeId2[slot];
    }
    return id != 0xFF ? id : -1;
}

void fldSetNpcFlag(s32 charId, s32 idx, s32 bit, s32 on)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        if (on != 0) {
            DDS3_GBWK->gb[type].flags[idx].npcFlag |= 1 << bit;
        } else {
            DDS3_GBWK->gb[type].flags[idx].npcFlag &= ~(1 << bit);
        }
        return;
    }
    type = dds3EffectFldCharToType(charId);
    if (on != 0) {
        DDS3_GBWK->gb[type].flags[idx].npcFlag |= 1 << bit;
    } else {
        DDS3_GBWK->gb[type].flags[idx].npcFlag &= ~(1 << bit);
    }
}

s32 fldGbChkNpc(s32 charId, s32 idx, s32 bit)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return 0;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        return ((u32)DDS3_GBWK->gb[type].flags[idx].npcFlag >> bit) & 1;
    }
    type = dds3EffectFldCharToType(charId);
    return ((u32)DDS3_GBWK->gb[type].flags[idx].npcFlag >> bit) & 1;
}

void fldGbSetGimic(s32 charId, s32 idx, s32 bit, s32 on)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        if (on != 0) {
            DDS3_GBWK->gb[type].flags[idx].hojiFlag |= 1 << bit;
        } else {
            DDS3_GBWK->gb[type].flags[idx].hojiFlag &= ~(1 << bit);
        }
        return;
    }
    type = dds3EffectFldCharToType(charId);
    if (on != 0) {
        DDS3_GBWK->gb[type].flags[idx].hojiFlag |= 1 << bit;
    } else {
        DDS3_GBWK->gb[type].flags[idx].hojiFlag &= ~(1 << bit);
    }
}

s32 fldGetHojiFlag(s32 charId, s32 idx, s32 bit)
{
    s32 type;

    if (dds3EffectFldTypeIsSpecial(charId) != 0) {
        return 0;
    }
    if (fldGb < 0x28) {
        type = charId % 100;
        return ((u32)DDS3_GBWK->gb[type].flags[idx].hojiFlag >> bit) & 1;
    }
    type = dds3EffectFldCharToType(charId);
    return ((u32)DDS3_GBWK->gb[type].flags[idx].hojiFlag >> bit) & 1;
}

void fldSetTakaraOpen(s32 id)
{
    s32 idx;
    s32 bit;

    if (id >= 0x100) {
        idx = (id - 0x100) >> 3;
        bit = (id - 0x100) & 7;
        DDS3_GBWK->takaraOpen[idx + 0x28] |= 1 << bit;
        fldFindTakaraSlot(id);
        return;
    }
    idx = id >> 3;
    bit = id & 7;
    DDS3_GBWK->takaraOpen[idx] |= 1 << bit;
    fldFindTakaraSlot(id);
    if (id >= 0xF0 && id < 0x100) {
        evtBitOn(id + 0x610);
    }
}

s32 fldGetTakaraOpen(s32 id)
{
    s32 idx;
    s32 bit;

    if (id >= 0x100) {
        idx = (id - 0x100) >> 3;
        bit = (id - 0x100) & 7;
        return ((u32)DDS3_GBWK->takaraOpenEx[idx] >> bit) & 1;
    }
    idx = id >> 3;
    bit = id & 7;
    return ((u32)DDS3_GBWK->takaraOpen[idx] >> bit) & 1;
}

s32 fldAutoMapFindArea(s32 fld, s32 area)
{
    s32 i;

    for (i = 1; i < 0x190; i++) {
        if (fld == fldAreaNameTable[i].fld && area == fldAreaNameTable[i].area) {
            return i;
        }
    }
    return 0;
}

s32 fldAutoMapFindPlace(s32 fld, s32 area)
{
    s32 i;

    for (i = 1; i < 0x150; i++) {
        if (fld == fldPlaceNameTable[i].fld && area == fldPlaceNameTable[i].area) {
            return i;
        }
    }
    return 0;
}

s32 fldAutoMapFindFloor(s32 fld, s32 area)
{
    s32 i;

    for (i = 1; i < 0xE0; i++) {
        if (fld == fldFloorNameTable[i].fld && area == fldFloorNameTable[i].area) {
            return i;
        }
    }
    return 0;
}
