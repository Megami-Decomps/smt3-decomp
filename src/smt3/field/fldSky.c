#include "fpu.h"
#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "ps2asm.h"
#include "basic.h"
#include "billboard.h"
#include "draw.h"
#include "scr.h"
#include "draw3d.h"
#include "field.h"
#include "model.h"
#include "motion.h"
#include "newdata.h"
#include "event.h"
#include "effect.h"

typedef struct fldLightSet_t fldLightSet_t;

struct fldLightSet_t {
    u8 fadeArea;
    u8 pad1[3];
    s32 displayStateSlot14;
    s32 fadeMode;
    s32 swayMode;
    u8 pad10[0xC];
    s32 fogNearValue;
    s32 depthNearValue;
    s32 fogFarValue;
    s32 depthFarValue;
    s32 drawColorFixedR;
    s32 drawColorFixedG;
    s32 drawColorFixedB;
    f32 lightColorAX;
    f32 lightColorAY;
    f32 lightColorAZ;
    f32 lightDirectionAX;
    f32 lightDirectionAY;
    f32 lightDirectionAZ;
    f32 lightColorBX;
    f32 lightColorBY;
    f32 lightColorBZ;
    f32 lightDirectionBX;
    f32 lightDirectionBY;
    f32 lightDirectionBZ;
    f32 lightColorCX;
    f32 lightColorCY;
    f32 lightColorCZ;
    f32 lightDirectionCX;
    f32 lightDirectionCY;
    f32 lightDirectionCZ;
    f32 backgroundColorX;
    f32 backgroundColorY;
    f32 backgroundColorZ;
    f32 unitColorA[3];
    f32 unitLightDirection[3];
    u8 padA4[0x30];
    f32 unitColorB[3];
};

enum {
    FIELD_VALUE_RECORD_STORAGE_SIZE = 0x72000,
    FIELD_AUX_RECORD_STORAGE_SIZE = 0x4A00,
    FIELD_ROOM_RECORD_COUNT = 64,
    FIELD_ROOM_CORNER_COUNT = 8,
    FIELD_ROOM_PLANE_COUNT = 6,
    FIELD_ACTOR_SLOT_COUNT = 256
};

typedef struct devState_t devState_t;

extern void *memset(void *, int, unsigned int);

f32 sdfSin(f32 aRadius);

extern u32 fldRunDanteGunFlag[];

extern void dds3ObjectBaseOffFlag(void *pObjectBasic, s32 flag);

extern void dds3ObjectBaseOnFlag(void *object, s32 mask);

extern u32 D_0054EAF0;

extern u32 fldPlayerUnit;

extern f32 D_002FCA60[];

extern evtUnit_t *evtGetUnitHandle2(dds3Basic_t *unit);

extern void evtUnitLightOn(evtUnit_t *h);

extern void evtSetUnitLightAndAmbient(evtUnit_t *h, s32 frame, u32 lrgb, u32 argb);

extern void evtSetUnitLightDir(evtUnit_t *h, s32 frame);

extern dds3Basic_t *D_0054EAE8;

extern void dds3UnitObjectBasicEL_LinkFree(dds3Basic_t *spawnedObject);

extern u32 fldSkyTable;

extern u32 fldSkyTableIdx;

extern u32 fldSkyLightMaTbl;

extern u32 D_002FCAC0[];

extern u32 D_002FCAA0[];

extern f32 fldSkyPlayerIdx[];

extern void *fldSkyDefaultData;

/* fldSkyDataTable stride, 292 bytes; only the filter mode at offset 0 is used here. */
typedef struct {
    s32 mode;
    s32 colors[8][9];
} fldSkyData_t;

extern fldSkyData_t *fldSkyDataTable;

extern void *fldSkyDefault;

extern s32 fldSkyTblUpperIdx;

extern s32 D_0054ED5C;

extern f32 D_0054ED60;

extern s32 D_0054ED64;

extern s32 D_0054ED68;

extern s32 D_0054ED6C;

extern f32 D_0054ED70;

extern f32 D_0054ED74;

extern s32 D_0054ED7C;

extern f32 D_0054ED80;

extern s32 D_0054ED84;

extern s32 D_0054ED8C;

extern void fldSkySetPlayer(s32 intr, f32 r0, f32 g0, f32 b0,
                            f32 r3, f32 g3, f32 b3,
                            f32 x0, f32 y0, f32 z0);

extern s32 fldSkyEnable;

extern s32 fldSkyFilter;

extern s32 fldSkyFilterPrev;

extern f32 fldSkyFilterRate;

extern f32 fldSkyFilterTime;

extern s32 fldLightMaState __attribute__((section(".data")));

extern s32 fldGb __attribute__((section(".data")));

extern void dds3SetFogColor(s32, f32 *);

extern s32 dds3SetLightColor(s32 frame, s32 no, f32 *col);

extern void dds3SetAmbientColor(s32, f32 *);

extern s32 dds3SetLightDir(s32 frame, s32 no, f32 *dir);

extern void dds3SetFogPara(s32 frame, f32 max, f32 min, f32 z, f32 w);

sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

u32 sdfMemLock(sdfMemHandle_t *aHandle);

extern u32 fldItemBoxObjBuf;

extern u32 fldMatterTable;

extern u32 D_0054EDA8;

extern u32 D_0054EDAC;

void sdfMemUnlock(sdfMemHandle_t *aHandle);

void sdfMemFreeDelayedDirect(sdfMemHandle_t *aHandle);

void fldMakeFilePath(char *pathstr, s32 field, s32 area);

devState_t *sdfFileOpen(const char *aFileName);

void sdfFileRead(devState_t *aHandle, void *aPtr, s32 aLeng);

void sdfFileClose(devState_t *aHandle);

extern s32 sprintf(char *, const char *, ...);

extern void *memcpy(void *, const void *, unsigned int);

s32 evtBitCheck(s32 no);

extern s32 gSkyTblAnime;

extern f32 gSkyTblAnimeCntI;

extern s32 gSkyTblBaseIdx;

extern s32 D_002FC0EC __attribute__((section(".data")));

extern void fldSkySetTbl(s32 idx, s32 hokan);

extern s32 gFilterAlpha;

extern s32 gFilterAnime;

extern f32 gFilterAnimeCntF;

extern s32 gFilterAnimeOfsA;

extern s32 fldSkyAlphaOfs __attribute__((section(".data")));

void fldSkyDefaultLoad(void) {
    devState_t *fd;

    if (fldSkyDefault == NULL) {
        fldSkyDefault = (void *)sdfMemLock(sdfMemAllocDirect(0xE000));
    }
    if (fldSkyDefaultData == NULL) {
        fldSkyDefaultData = (void *)sdfMemLock(sdfMemAllocDirect(0xE000));
    }
    if (fldSkyDataTable == NULL) {
        fldSkyDataTable = (fldSkyData_t *)sdfMemLock(sdfMemAllocDirect(0x12400));
    }
    fd = sdfFileOpen("/fld/f/bin/FILTER.FLD");
    sdfFileRead(fd, fldSkyDataTable, 0xE000);
    sdfFileClose(fd);
    fd = sdfFileOpen("/fld/f/bin/BATTLEBG.SKY");
    sdfFileRead(fd, fldSkyDefaultData, 0xE000);
    sdfFileClose(fd);
}

void fldSkyFieldLoad(s32 field) {
    char path[64];
    char directory[32];
    devState_t *fd;

    gFilterAlpha = 0x80;
    if (field < 200) {
        fldMakeFilePath(directory, field, 1);
        if (fldGlobalBase.area == 0x19 && evtBitCheck(0x599) != 0 && evtBitCheck(0x743) == 0) {
            sprintf(path, "%sF%03db.SKY", directory, field);
        } else if (fldGlobalBase.area == 0x18 && evtBitCheck(0x4F) != 0) {
            sprintf(path, "%sF%03db.SKY", directory, field);
        } else if (fldGlobalBase.area == 0x18 && evtBitCheck(0x16) != 0 && evtBitCheck(0x3E) == 0) {
            sprintf(path, "%sF%03dc.SKY", directory, field);
        } else {
            sprintf(path, "%sF%03d.SKY", directory, field);
        }
        fd = sdfFileOpen(path);
        sdfFileRead(fd, fldSkyDefault, 0xE000);
        sdfFileClose(fd);
    }
}

void fldGetSkyEntry(const void *src) {
    gFilterAlpha = 0x80;
    memcpy(fldSkyDefault, src, 0xE000);
}

void fldSkyCalcFilter(void) {
    gFilterAnimeOfsA = 0;
    switch (gFilterAnime) {
    case 1:
        gFilterAnimeCntF += 0.1f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 32.0f;
        break;
    case 2:
        gFilterAnimeCntF += 0.2f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 32.0f;
        break;
    case 3:
        gFilterAnimeCntF += 0.05f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 32.0f;
        break;
    case 4:
        gFilterAnimeCntF += 0.1f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 48.0f;
        break;
    case 5:
        gFilterAnimeCntF += 0.2f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 48.0f;
        break;
    case 6:
        gFilterAnimeCntF += 0.05f;
        gFilterAnimeOfsA = sdfSin(gFilterAnimeCntF) * 48.0f;
        break;
    }
}

INCLUDE_ASM(const s32, "field/fldSky", fldSkyDrawFilter);

void fldSkySetFilter(s32 idx, s32 mode, s32 hokan) {
    if (fldSkyEnable == 0 && fldGb < 0x32) {
        hokan = 0;
    }
    if (hokan == 0) {
        fldSkyFilter = idx;
        fldSkyFilterRate = 1.0f;
        fldSkyFilterTime = 1.0f;
    } else {
        fldSkyFilterRate = 0.0f;
        fldSkyFilter = fldSkyFilterPrev;
        fldSkyFilterTime = (f32)hokan;
    }
    fldSkyFilterPrev = idx;
    fldSkyDataTable[idx].mode = mode;
}

void fldSetFilterAnime(s32 anime)
{
    gFilterAnime = anime;
    gFilterAnimeCntF = 0;
    gFilterAnimeOfsA = 0;
}

void fldSkySetAlpha(s32 alp)
{
    gFilterAlpha = alp;
}

s32 fldSkyGetAlpha(void)
{
    return gFilterAlpha;
}

void fldSkySetAlphaOfs(s32 alp)
{
    fldSkyAlphaOfs = alp;
}

s32 fldSkyGetAlphaOfs(void)
{
    return fldSkyAlphaOfs;
}

void fldSkySetTableAnime(s32 type)
{
    s32 idx = D_002FC0EC;

    gSkyTblAnime = type;
    gSkyTblAnimeCntI = 0;
    gSkyTblBaseIdx = idx;
    fldSkySetTbl(idx, 1);
}

/* The dispatch runs ahead of all three bodies, which a switch or an if chain
 * over the same three values does not reproduce. */
void fldSkyCalcTable(void) {
    s32 anime = gSkyTblAnime;

    if (anime == 1) {
        goto anime1;
    }
    if (anime < 2) {
        return;
    }
    if (anime == 2) {
        goto anime2;
    }
    if (anime != 3) {
        return;
    }
    goto anime3;

anime1:
    gSkyTblAnimeCntI += 1.0f;
    if (gSkyTblAnimeCntI == 30.0f) {
        fldSkySetTbl((gSkyTblBaseIdx + 1) & 0xFF, 30);
        return;
    }
    if (gSkyTblAnimeCntI == 60.0f) {
        fldSkySetTbl(gSkyTblBaseIdx, 30);
        gSkyTblAnimeCntI = 0.0f;
    }
    return;

anime2:
    gSkyTblAnimeCntI += 1.0f;
    if (gSkyTblAnimeCntI == 15.0f) {
        fldSkySetTbl((gSkyTblBaseIdx + 1) & 0xFF, 1);
        return;
    }
    if (gSkyTblAnimeCntI == 30.0f) {
        fldSkySetTbl(gSkyTblBaseIdx, 1);
        gSkyTblAnimeCntI = 0.0f;
    }
    return;

anime3:
    gSkyTblAnimeCntI += 1.0f;
    if (gSkyTblAnimeCntI == 60.0f) {
        fldSkySetTbl((gSkyTblBaseIdx + 1) & 0xFF, 60);
        return;
    }
    if (gSkyTblAnimeCntI == 120.0f) {
        fldSkySetTbl(gSkyTblBaseIdx, 60);
        gSkyTblAnimeCntI = 0.0f;
    }
}

void fld_136800(s32 duration, f32 lightR, f32 lightG, f32 lightB,
                f32 ambientR, f32 ambientG, f32 ambientB) {
    evtUnit_t *unit;
    s32 r, g, b;
    u32 lightColor, ambientColor;

    if (D_0054EAF0 != 0) {
        unit = evtGetUnitHandle2((dds3Basic_t *)D_0054EAF0);
        r = (s32)(lightR * 128.0f);
        g = (s32)(lightG * 128.0f);
        b = (s32)(lightB * 128.0f);
        lightColor = r | (b << 16) | (g << 8) | 0x80000000;
        r = (s32)(ambientR * 128.0f);
        g = (s32)(ambientG * 128.0f);
        b = (s32)(ambientB * 128.0f);
        ambientColor = r | (b << 16) | (g << 8) | 0x80000000;
        evtSetUnitLightAndAmbient(unit, duration, lightColor, ambientColor);
    }
}

void fldSkySetPlayer(s32 intr, f32 r0, f32 g0, f32 b0,
                     f32 r3, f32 g3, f32 b3,
                     f32 x0, f32 y0, f32 z0) {
    f32 dir[4];
    evtUnit_t *playerUnit = NULL;
    evtUnit_t *secondUnit = NULL;
    evtUnit_t *mainUnit;
    u32 lightColor, ambientColor;
    s32 r, g, b;

    if (fldPlayerUnit != 0) {
        playerUnit = evtGetUnitHandle2((dds3Basic_t *)fldPlayerUnit);
        evtUnitLightOn(playerUnit);
    }
    if (D_0054EAF0 != 0) {
        secondUnit = evtGetUnitHandle2((dds3Basic_t *)D_0054EAF0);
        evtUnitLightOn(secondUnit);
    }
    mainUnit = evtGetUnitHandle2(D_0054EAE8);
    evtUnitLightOn(mainUnit);

    r = (s32)(r0 * 128.0f);
    g = (s32)(g0 * 128.0f);
    b = (s32)(b0 * 128.0f);
    lightColor = r | (b << 16) | (g << 8) | 0x80000000;
    r = (s32)(r3 * 128.0f);
    g = (s32)(g3 * 128.0f);
    b = (s32)(b3 * 128.0f);
    ambientColor = r | (b << 16) | (g << 8) | 0x80000000;
    evtSetUnitLightAndAmbient(mainUnit, intr, lightColor, ambientColor);
    if (fldPlayerUnit != 0) {
        evtSetUnitLightAndAmbient(playerUnit, intr, lightColor, ambientColor);
    }
    if (D_0054EAF0 != 0) {
        evtSetUnitLightAndAmbient(secondUnit, intr, lightColor, ambientColor);
    }

    dir[0] = x0;
    dir[1] = y0;
    dir[2] = z0;
    dir[3] = 0.0f;
    SDF_VU0_LOADVECTOR_MEMOPS(*(const Quad *)dir);
    evtSetUnitLightDir(mainUnit, intr);
    if (fldPlayerUnit != 0) {
        evtSetUnitLightDir(playerUnit, intr);
    }
    if (D_0054EAF0 != 0) {
        evtSetUnitLightDir(secondUnit, intr);
    }
    SDF_VU0_LOADVECTOR_MEMOPS(*(const Quad *)dir);
    evtSetUnitLightDir(mainUnit, 0);
    if (fldPlayerUnit != 0) {
        evtSetUnitLightDir(playerUnit, 0);
    }
    if (D_0054EAF0 != 0) {
        evtSetUnitLightDir(secondUnit, 0);
    }

    D_002FCA60[0] = r0;
    D_002FCA60[1] = g0;
    D_002FCA60[2] = b0;
    D_002FCA60[4] = x0;
    D_002FCA60[5] = y0;
    D_002FCA60[6] = z0;
    fldSkyPlayerIdx[0] = r3;
    fldSkyPlayerIdx[1] = g3;
    fldSkyPlayerIdx[2] = b3;
}

void fldSkySetTbl(s32 idx, s32 hokan) {
    fldAreaWork_t *state = &fldGlobalBase;
    fldLightSet_t *light;
    f32 vec[4];
    f32 dir[4];
    s32 duration;
    s32 defaultDuration;
    s32 area;
    s32 value;

    if (state->area < 200) {
        if (fldSkyEnable == 0 && state->area < 0x32) {
            hokan = 0;
        }
        defaultDuration = 15;
        duration = 0;
        if (hokan != 0) {
            duration = defaultDuration;
        }
        if (hokan >= 2) {
            duration = hokan;
        }
        fldSkyTable = idx;
        fldSkyTableIdx = idx;
        fldSkyTblUpperIdx = 0;
        if (state->lightMaIdx == 0) {
            light = &((fldLightSet_t *)fldSkyDefault)[idx];
            area = light->fadeArea;
            fldRunDanteGunFlag[13] = area;
            fldRunDanteGunFlag[14] = light->displayStateSlot14;
            value = light->fadeMode;
            fldRunDanteGunFlag[15] = value;
            fldRunDanteGunFlag[16] = light->swayMode;
            fldSkySetFilter(area, value, duration);
            fldSetFilterAnime(fldRunDanteGunFlag[16]);
            vec[0] = light->drawColorFixedR * 0.00390625f;
            vec[1] = light->drawColorFixedG * 0.00390625f;
            vec[2] = light->drawColorFixedB * 0.00390625f;
            vec[3] = 0;
            dds3SetFogColor(duration, vec);
            dds3SetFogPara(duration, light->fogNearValue, light->fogFarValue, light->depthNearValue, light->depthFarValue);
            dir[0] = light->lightDirectionAX;
            dir[1] = light->lightDirectionAY;
            dir[2] = light->lightDirectionAZ;
            dir[3] = 0;
            dds3SetLightDir(duration, 0, dir);
            vec[0] = light->lightColorAX;
            vec[1] = light->lightColorAY;
            vec[2] = light->lightColorAZ;
            vec[3] = 0;
            dds3SetLightColor(duration, 0, vec);
            dir[0] = light->lightDirectionBX;
            dir[1] = light->lightDirectionBY;
            dir[2] = light->lightDirectionBZ;
            dir[3] = 0;
            dds3SetLightDir(duration, 1, dir);
            vec[0] = light->lightColorBX;
            vec[1] = light->lightColorBY;
            vec[2] = light->lightColorBZ;
            vec[3] = 0;
            dds3SetLightColor(duration, 1, vec);
            dir[0] = light->lightDirectionCX;
            dir[1] = light->lightDirectionCY;
            dir[2] = light->lightDirectionCZ;
            dir[3] = 0;
            dds3SetLightDir(duration, 2, dir);
            vec[0] = light->lightColorCX;
            vec[1] = light->lightColorCY;
            vec[2] = light->lightColorCZ;
            vec[3] = 0;
            dds3SetLightColor(duration, 2, vec);
            vec[0] = light->backgroundColorX;
            vec[1] = light->backgroundColorY;
            vec[2] = light->backgroundColorZ;
            vec[3] = 1.0f;
            dds3SetAmbientColor(duration, vec);
            fldSkySetPlayer(duration, light->unitColorA[0], light->unitColorA[1], light->unitColorA[2],
                            light->unitColorB[0], light->unitColorB[1], light->unitColorB[2],
                            light->unitLightDirection[0], light->unitLightDirection[1],
                            light->unitLightDirection[2]);
        }
    }
}

void fldSkySetTblModosu(s32 hokan) {
    if (fldSkyEnable == 0 && fldGb < 0x32) {
        hokan = 0;
    }
    if (fldSkyTableIdx != 0 && fldSkyTable != fldSkyTableIdx) {
        if (fldLightMaState == 0) {
            D_002FC0EC = fldSkyTableIdx;
        }
        fldSkySetTbl(fldSkyTableIdx, hokan);
    }
}

void fldSkySetTblUpper(s32 idx, s32 hokan) {
    fldAreaWork_t *state = &fldGlobalBase;
    fldLightSet_t *light;
    f32 vec[4];
    f32 dir[4];
    s32 duration;
    s32 defaultDuration;
    s32 area;
    s32 value;
    u32 previousIndex;

    if (state->area < 200) {
        if (fldSkyEnable == 0 && state->area < 0x32) {
            hokan = 0;
        }
        defaultDuration = 15;
        duration = 0;
        if (hokan != 0) {
            duration = defaultDuration;
        }
        if (hokan >= 2) {
            duration = hokan;
        }
        if (fldSkyTable != idx) {
            previousIndex = fldSkyTableIdx;
            if (fldSkyTblUpperIdx == 0) {
                previousIndex = fldSkyTable;
            }
            D_0054ED5C = gFilterAnime;
            D_0054ED60 = gFilterAnimeCntF;
            D_0054ED64 = gFilterAnimeOfsA;
            D_0054ED68 = fldSkyFilterPrev;
            D_0054ED6C = fldSkyFilter;
            D_0054ED70 = fldSkyFilterRate;
            D_0054ED74 = fldSkyFilterTime;
            fldSkyTableIdx = previousIndex;
            D_0054ED7C = gSkyTblAnime;
            D_0054ED80 = gSkyTblAnimeCntI;
            D_0054ED84 = gSkyTblBaseIdx;
            fldSkyTblUpperIdx = 1;
            fldSkyTable = idx;
            if (state->lightMaIdx == 0) {
                light = &((fldLightSet_t *)fldSkyDefault)[idx];
                fldRunDanteGunFlag[11] = idx;
            area = light->fadeArea;
            fldRunDanteGunFlag[13] = area;
            fldRunDanteGunFlag[14] = light->displayStateSlot14;
            value = light->fadeMode;
            fldRunDanteGunFlag[15] = value;
            fldRunDanteGunFlag[16] = light->swayMode;
            fldSkySetFilter(area, value, duration);
            fldSetFilterAnime(fldRunDanteGunFlag[16]);
            vec[0] = light->drawColorFixedR * 0.00390625f;
            vec[1] = light->drawColorFixedG * 0.00390625f;
            vec[2] = light->drawColorFixedB * 0.00390625f;
            vec[3] = 0;
            dds3SetFogColor(duration, vec);
            dds3SetFogPara(duration, light->fogNearValue, light->fogFarValue, light->depthNearValue, light->depthFarValue);
            dir[0] = light->lightDirectionAX;
            dir[1] = light->lightDirectionAY;
            dir[2] = light->lightDirectionAZ;
            dir[3] = 0;
            dds3SetLightDir(duration, 0, dir);
            vec[0] = light->lightColorAX;
            vec[1] = light->lightColorAY;
            vec[2] = light->lightColorAZ;
            vec[3] = 0;
            dds3SetLightColor(duration, 0, vec);
            dir[0] = light->lightDirectionBX;
            dir[1] = light->lightDirectionBY;
            dir[2] = light->lightDirectionBZ;
            dir[3] = 0;
            dds3SetLightDir(duration, 1, dir);
            vec[0] = light->lightColorBX;
            vec[1] = light->lightColorBY;
            vec[2] = light->lightColorBZ;
            vec[3] = 0;
            dds3SetLightColor(duration, 1, vec);
            dir[0] = light->lightDirectionCX;
            dir[1] = light->lightDirectionCY;
            dir[2] = light->lightDirectionCZ;
            dir[3] = 0;
            dds3SetLightDir(duration, 2, dir);
            vec[0] = light->lightColorCX;
            vec[1] = light->lightColorCY;
            vec[2] = light->lightColorCZ;
            vec[3] = 0;
            dds3SetLightColor(duration, 2, vec);
            vec[0] = light->backgroundColorX;
            vec[1] = light->backgroundColorY;
            vec[2] = light->backgroundColorZ;
            vec[3] = 1.0f;
            dds3SetAmbientColor(duration, vec);
            fldSkySetPlayer(duration, light->unitColorA[0], light->unitColorA[1], light->unitColorA[2],
                            light->unitColorB[0], light->unitColorB[1], light->unitColorB[2],
                            light->unitLightDirection[0], light->unitLightDirection[1],
                            light->unitLightDirection[2]);
            }
        }
    }
}

void fldSkySetTblLightMaDZ(s32 idx) {
    if (idx == 0) {
        dds3ObjectBaseOffFlag(D_0054EAE8, 0x100);
        return;
    }
    dds3ObjectBaseOnFlag(D_0054EAE8, 0x100);
    dds3UnitObjectBasicEL_LinkFree(D_0054EAE8);
}

void fldSkySetTblLightMa(s32 idx, s32 hokan) {
    fldLightSet_t *light;
    f32 vec[4];
    f32 dir[4];
    s32 duration;
    s32 defaultDuration;
    s32 area;
    s32 value;

    if (fldGb < 200) {
        if (fldSkyEnable == 0 && fldGb < 0x32) {
            hokan = 0;
        }
        defaultDuration = 15;
        duration = 0;
        if (hokan != 0) {
            duration = defaultDuration;
        }
        if (hokan >= 2) {
            duration = hokan;
        }
        if (fldSkyLightMaTbl != idx) {
            if (idx == 0) {
                idx = fldSkyTable;
            }
            light = &((fldLightSet_t *)fldSkyDefault)[idx];
            D_0054ED8C = fldRunDanteGunFlag[11];
            fldRunDanteGunFlag[11] = idx;
            fldSkyLightMaTbl = idx;
            area = light->fadeArea;
            fldRunDanteGunFlag[13] = area;
            fldRunDanteGunFlag[14] = light->displayStateSlot14;
            value = light->fadeMode;
            fldRunDanteGunFlag[15] = value;
            fldRunDanteGunFlag[16] = light->swayMode;
            fldSkySetFilter(area, value, duration);
            fldSetFilterAnime(fldRunDanteGunFlag[16]);
            vec[0] = light->drawColorFixedR * 0.00390625f;
            vec[1] = light->drawColorFixedG * 0.00390625f;
            vec[2] = light->drawColorFixedB * 0.00390625f;
            vec[3] = 0;
            dds3SetFogColor(duration, vec);
            dds3SetFogPara(duration, light->fogNearValue, light->fogFarValue, light->depthNearValue, light->depthFarValue);
            dir[0] = light->lightDirectionAX;
            dir[1] = light->lightDirectionAY;
            dir[2] = light->lightDirectionAZ;
            dir[3] = 0;
            dds3SetLightDir(duration, 0, dir);
            vec[0] = light->lightColorAX;
            vec[1] = light->lightColorAY;
            vec[2] = light->lightColorAZ;
            vec[3] = 0;
            dds3SetLightColor(duration, 0, vec);
            dir[0] = light->lightDirectionBX;
            dir[1] = light->lightDirectionBY;
            dir[2] = light->lightDirectionBZ;
            dir[3] = 0;
            dds3SetLightDir(duration, 1, dir);
            vec[0] = light->lightColorBX;
            vec[1] = light->lightColorBY;
            vec[2] = light->lightColorBZ;
            vec[3] = 0;
            dds3SetLightColor(duration, 1, vec);
            dir[0] = light->lightDirectionCX;
            dir[1] = light->lightDirectionCY;
            dir[2] = light->lightDirectionCZ;
            dir[3] = 0;
            dds3SetLightDir(duration, 2, dir);
            vec[0] = light->lightColorCX;
            vec[1] = light->lightColorCY;
            vec[2] = light->lightColorCZ;
            vec[3] = 0;
            dds3SetLightColor(duration, 2, vec);
            vec[0] = light->backgroundColorX;
            vec[1] = light->backgroundColorY;
            vec[2] = light->backgroundColorZ;
            vec[3] = 1.0f;
            dds3SetAmbientColor(duration, vec);
            fldSkySetPlayer(duration, light->unitColorA[0], light->unitColorA[1], light->unitColorA[2],
                            light->unitColorB[0], light->unitColorB[1], light->unitColorB[2],
                            light->unitLightDirection[0], light->unitLightDirection[1],
                            light->unitLightDirection[2]);
        }
    }
}

void fldSetFileResolverIdx(u32 state) {
    fldSkyTable = state;
    fldSkyTableIdx = 0;
    fldRunDanteGunFlag[12] = 0;
    fldRunDanteGunFlag[11] = state;
    fldSkyLightMaTbl = 0;
}

void fld_1374F0(void) {
    u32 *displayPointers = D_002FCAC0;

    memset(displayPointers, 0, 0x14);
    displayPointers[0] = (u32)D_002FCAA0;
    displayPointers[1] = (u32)fldSkyPlayerIdx;
}

void fldSkyRefreshTbl(void) {
    fldLightSet_t *light;
    f32 vec[4];
    f32 dir[4];
    f32 *dirp;
    evtUnit_t *unit;
    s32 r, g, b;
    u32 lightColor, ambientColor;

    if (fldGb < 200) {
        if (fldSkyLightMaTbl != 0) {
            light = &((fldLightSet_t *)fldSkyDefault)[fldSkyLightMaTbl];
        } else {
            light = &((fldLightSet_t *)fldSkyDefault)[fldSkyTable];
        }
        fldSkySetFilter(fldRunDanteGunFlag[13], fldRunDanteGunFlag[15], 0);
        fldSetFilterAnime(fldRunDanteGunFlag[16]);
        vec[0] = light->drawColorFixedR * 0.00390625f;
        vec[1] = light->drawColorFixedG * 0.00390625f;
        vec[2] = light->drawColorFixedB * 0.00390625f;
        vec[3] = 0;
        dds3SetFogColor(0, vec);
        dds3SetFogPara(0, light->fogNearValue, light->fogFarValue, light->depthNearValue, light->depthFarValue);
        dir[0] = light->lightDirectionAX;
        dir[1] = light->lightDirectionAY;
        dir[2] = light->lightDirectionAZ;
        dir[3] = 0;
        dirp = dir;
        dds3SetLightDir(0, 0, dirp);
        vec[0] = light->lightColorAX;
        vec[1] = light->lightColorAY;
        vec[2] = light->lightColorAZ;
        vec[3] = 0;
        dds3SetLightColor(0, 0, vec);
        dir[0] = light->lightDirectionBX;
        dir[1] = light->lightDirectionBY;
        dir[2] = light->lightDirectionBZ;
        dir[3] = 0;
        dds3SetLightDir(0, 1, dirp);
        vec[0] = light->lightColorBX;
        vec[1] = light->lightColorBY;
        vec[2] = light->lightColorBZ;
        vec[3] = 0;
        dds3SetLightColor(0, 1, vec);
        dir[0] = light->lightDirectionCX;
        dir[1] = light->lightDirectionCY;
        dir[2] = light->lightDirectionCZ;
        dir[3] = 0;
        dds3SetLightDir(0, 2, dirp);
        vec[0] = light->lightColorCX;
        vec[1] = light->lightColorCY;
        vec[2] = light->lightColorCZ;
        vec[3] = 0;
        dds3SetLightColor(0, 2, vec);
        vec[0] = light->backgroundColorX;
        vec[1] = light->backgroundColorY;
        vec[2] = light->backgroundColorZ;
        vec[3] = 1.0f;
        dds3SetAmbientColor(0, vec);
        unit = evtGetUnitHandle2(D_0054EAE8);
        evtUnitLightOn(unit);
        r = (s32)(light->unitColorA[0] * 128.0f);
        g = (s32)(light->unitColorA[1] * 128.0f);
        b = (s32)(light->unitColorA[2] * 128.0f);
        lightColor = r | (b << 16) | (g << 8) | 0x80000000;
        r = (s32)(light->unitColorB[0] * 128.0f);
        g = (s32)(light->unitColorB[1] * 128.0f);
        b = (s32)(light->unitColorB[2] * 128.0f);
        ambientColor = r | (b << 16) | (g << 8) | 0x80000000;
        evtSetUnitLightAndAmbient(unit, 0, lightColor, ambientColor);
        dir[0] = light->unitLightDirection[0];
        dir[1] = light->unitLightDirection[1];
        dir[2] = light->unitLightDirection[2];
        dir[3] = 0;
        SDF_VU0_LOADVECTOR(dirp);
        evtSetUnitLightDir(unit, 0);
        SDF_VU0_LOADVECTOR(dir);
        evtSetUnitLightDir(unit, 0);
    }
}

void fldSkyBattle(void) {
    fldLightSet_t *light = &((fldLightSet_t *)fldSkyDefaultData)[fldSkyTable];
    f32 vec[4];
    f32 dir[4];
    s32 area;
    s32 value;

    area = light->fadeArea;
    fldRunDanteGunFlag[13] = area;
    fldRunDanteGunFlag[14] = light->displayStateSlot14;
    value = light->fadeMode;
    fldRunDanteGunFlag[15] = value;
    fldRunDanteGunFlag[16] = light->swayMode;
    fldSkySetFilter(area, value, 0);
    fldSetFilterAnime(fldRunDanteGunFlag[16]);
    vec[0] = light->drawColorFixedR * 0.00390625f;
    vec[1] = light->drawColorFixedG * 0.00390625f;
    vec[2] = light->drawColorFixedB * 0.00390625f;
    vec[3] = 0;
    dds3SetFogColor(0, vec);
    dds3SetFogPara(0, light->fogNearValue, light->fogFarValue, light->depthNearValue, light->depthFarValue);
    dir[0] = light->lightDirectionAX;
    dir[1] = light->lightDirectionAY;
    dir[2] = light->lightDirectionAZ;
    dir[3] = 0;
    dds3SetLightDir(0, 0, dir);
    vec[0] = light->lightColorAX;
    vec[1] = light->lightColorAY;
    vec[2] = light->lightColorAZ;
    vec[3] = 0;
    dds3SetLightColor(0, 0, vec);
    dir[0] = light->lightDirectionBX;
    dir[1] = light->lightDirectionBY;
    dir[2] = light->lightDirectionBZ;
    dir[3] = 0;
    dds3SetLightDir(0, 1, dir);
    vec[0] = light->lightColorBX;
    vec[1] = light->lightColorBY;
    vec[2] = light->lightColorBZ;
    vec[3] = 0;
    dds3SetLightColor(0, 1, vec);
    dir[0] = light->lightDirectionCX;
    dir[1] = light->lightDirectionCY;
    dir[2] = light->lightDirectionCZ;
    dir[3] = 0;
    dds3SetLightDir(0, 2, dir);
    vec[0] = light->lightColorCX;
    vec[1] = light->lightColorCY;
    vec[2] = light->lightColorCZ;
    vec[3] = 0;
    dds3SetLightColor(0, 2, vec);
    vec[0] = light->backgroundColorX;
    vec[1] = light->backgroundColorY;
    vec[2] = light->backgroundColorZ;
    vec[3] = 1.0f;
    dds3SetAmbientColor(0, vec);
}

void fldAllocPlaneTable(void) {
    sdfMemHandle_t *allocation = sdfMemAllocDirect(FIELD_VALUE_RECORD_STORAGE_SIZE);
    void *storage;

    D_0054EDAC = (u32)allocation;
    storage = (void *)sdfMemLock(allocation);
    fldMatterTable = (u32)storage;
    memset(storage, 0, FIELD_VALUE_RECORD_STORAGE_SIZE);
    allocation = sdfMemAllocDirect(FIELD_AUX_RECORD_STORAGE_SIZE);
    D_0054EDA8 = (u32)allocation;
    storage = (void *)sdfMemLock(allocation);
    fldItemBoxObjBuf = (u32)storage;
    memset(storage, 0, FIELD_AUX_RECORD_STORAGE_SIZE);
}

void fldFreeMatterTable(void) {
    sdfMemUnlock((sdfMemHandle_t *)D_0054EDAC);
    sdfMemFreeDelayedDirect((sdfMemHandle_t *)D_0054EDAC);
    fldMatterTable = 0;
    sdfMemUnlock((sdfMemHandle_t *)D_0054EDA8);
    sdfMemFreeDelayedDirect((sdfMemHandle_t *)D_0054EDA8);
    fldItemBoxObjBuf = 0;
}

f32 fldVecDot(sdfVector4_t *a, sdfVector4_t *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}
INCLUDE_SDATA(const s32, "field/fldSky", fldSkyDefault);

INCLUDE_SDATA(const s32, "field/fldSky", fldSkyDefaultData);

INCLUDE_SDATA(const s32, "field/fldSky", fldSkyDataTable);

INCLUDE_SDATA(const s32, "field/fldSky", gFilterAlpha);

INCLUDE_SDATA(const s32, "field/fldSky", gFilterAnime);

INCLUDE_SDATA(const s32, "field/fldSky", gFilterAnimeCntF);

INCLUDE_SDATA(const s32, "field/fldSky", gFilterAnimeOfsA);

INCLUDE_SDATA(const s32, "field/fldSky", fldSkyFilterPrev);

INCLUDE_SDATA(const s32, "field/fldSky", fldSkyFilter);

INCLUDE_SDATA(const s32, "field/fldSky", fldSkyFilterRate);

INCLUDE_SDATA(const s32, "field/fldSky", fldSkyFilterTime);

INCLUDE_SDATA(const s32, "field/fldSky", fldSkyTable);

INCLUDE_SDATA(const s32, "field/fldSky", gSkyTblAnime);

INCLUDE_SDATA(const s32, "field/fldSky", gSkyTblAnimeCntI);

INCLUDE_SDATA(const s32, "field/fldSky", gSkyTblBaseIdx);

INCLUDE_SDATA(const s32, "field/fldSky", fldSkyTblUpperIdx);

INCLUDE_SDATA(const s32, "field/fldSky", D_0054ED5C);

INCLUDE_SDATA(const s32, "field/fldSky", D_0054ED60);

INCLUDE_SDATA(const s32, "field/fldSky", D_0054ED64);

INCLUDE_SDATA(const s32, "field/fldSky", D_0054ED68);

INCLUDE_SDATA(const s32, "field/fldSky", D_0054ED6C);

INCLUDE_SDATA(const s32, "field/fldSky", D_0054ED70);

INCLUDE_SDATA(const s32, "field/fldSky", D_0054ED74);

INCLUDE_SDATA(const s32, "field/fldSky", fldSkyTableIdx);

INCLUDE_SDATA(const s32, "field/fldSky", D_0054ED7C);

INCLUDE_SDATA(const s32, "field/fldSky", D_0054ED80);

INCLUDE_SDATA(const s32, "field/fldSky", D_0054ED84);

INCLUDE_SDATA(const s32, "field/fldSky", fldSkyLightMaTbl);

INCLUDE_SDATA(const s32, "field/fldSky", D_0054ED8C);

INCLUDE_SDATA(const s32, "field/fldSky", D_0054ED90);

INCLUDE_SDATA(const s32, "field/fldSky", fldItemBoxObjBuf);

