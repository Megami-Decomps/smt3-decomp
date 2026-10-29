#include "common.h"
#include "fpu.h"
#include "basic.h"
#include "model.h"
#include "newdata.h"
#include "scr.h"
#include "field.h"
#include "ps2asm.h"

/* Field-only views, with offsets taken from the SMT3 retail accesses. */
typedef struct
{
    s32 flg;
    s32 id;
    f32 r;
    f32 x;
    f32 y;
    f32 z;
} fldSe3D_tag;

enum sl_slot_flag
{
    SL_SLOT_FLAG_CAN_READ_SAVE = 1,
    SL_SLOT_FLAG_DIRECTORY_READY = 2,
    SL_SLOT_FLAG_DELETE_BEFORE_WRITE = 8
};

extern fldSe3D_tag fldSe3DTable[8];

extern s32 gFldSe3DNum;

extern s32 fldBgmTrackTable[];

void p2sdPlayBGM(s32 MidiNo);

extern u32 fldSeLastBgm;

extern void p2sdSendStopCmd(u32 controlWord);

extern void p2sdStopBGM(s32 trackHandle);

extern void p2sdStopAllBgm(void);

extern void p2sdStopSe(u32);

extern s32 D_0054EE3C;

extern s32 D_0030A2D8[];

extern s32 fldBgmReloadStep;

extern s32 p2sdLoadCheckMidi(s32);

extern s32 MidiNo;

extern void p2sdLoadSMGFile(s32);

extern s32 fldBgmLoadStep;

extern s32 fldBgmSmgAddr;

extern s32 fldGb __attribute__((section(".data")));

void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);

void fldBgmLoad(void);

extern void fldSePlay(s32 seid);

extern void fldSeStop(s32 seid);

extern void fldMotionSePlay(s32 seid, s32 subid);

extern s32 (*proc_func_ans)(dds3ProcessID_t *process);

extern void fldBgmContinue(void);

extern void fldMotionSeStop(s32 seid, s32 subid);

typedef struct
{
    s32 smgId;
    s8 trans[16];
} fldBgmEntry_t;

extern fldBgmEntry_t gFldBgmTop[];

extern s32 fldGetBgmTrack(s32 code);

extern void p2sdSetBgmTrack(s32 track);

extern s32 evtBitCheck(s32 no);

extern u32 gFldBgmBit;
extern s16 gFldAreaBgm __attribute__((section(".data")));

extern void fldBgmPlay(void);

extern s32 fldSeSmgId;

extern s32 DNG_MAX[];

void fldSe_Init(void) {
    fldSe3D_tag *entry = fldSe3DTable;
    s32 remaining = 7;

    do {
        remaining -= 1;
        entry->flg = 0;
        entry->r = 0;
        entry->id = 0;
        entry += 1;
    } while (remaining >= 0);
    gFldSe3DNum = 0;
}

void fldAddSe3D(s32 soundId, f32 radius, f32 sourceX, f32 sourceY, f32 sourceZ) {
    s32 i = gFldSe3DNum;

    fldSe3DTable[i].flg = 0;
    fldSe3DTable[i].id = soundId;
    fldSe3DTable[i].r = radius;
    fldSe3DTable[i].x = sourceX;
    fldSe3DTable[i].y = sourceY;
    fldSe3DTable[i].z = sourceZ;
    gFldSe3DNum = i + 1;
}

void fldClearSe3D(void) {
    s32 i;

    for (i = 0; i < gFldSe3DNum; i++) {
        s32 flags = fldSe3DTable[i].flg;

        if (flags & 1) {
            fldSe3DTable[i].flg = flags & ~1;
            p2sdStopSe(gFldBgmTop[fldGlobalBase.area].smgId + fldSe3DTable[i].id);
        }
    }
}

extern f32 fldCamTarget[];
extern f32 D_002FC9C0[];
/* Native frame counters wrap and have no signed consumers. */
extern u32 D_0054EE38;
extern u32 D_0054EE34;
typedef struct {
    f32 v[4];
} fldSoundPosition_t;
extern const fldSoundPosition_t D_00532110;
void p2sdPlaySe3DEx(s32 trackId, s32 radiusInHundreds, f32 x, f32 y, f32 z);
void p2sdSetStandPos(f32 firstX, f32 firstY, f32 firstZ,
                    f32 secondX, f32 secondY, f32 secondZ);

/* Adapted from DDS func_001415F8: SMT3 always uses the camera listener and
 * a fixed 500-unit radius margin, with the current area resolved per call. */
void fldPlaySe3D(void) {
    fldSoundPosition_t position = D_00532110;
    f32 listenerX, listenerY, listenerZ;
    f32 dx, dy, dz, distance;
    f32 sourceX, sourceY, sourceZ;
    u32 tick;
    s32 nearby;
    s32 flags;
    s32 i;
    fldSe3D_tag *entry;

    tick = D_0054EE38 + 1;
    nearby = 0;
    D_0054EE38 = tick;
    listenerX = fldCamTarget[0] - D_002FC9C0[0];
    listenerY = D_002FC9C0[1];
    listenerZ = fldCamTarget[2] - D_002FC9C0[2];
    distance = fsqrtf(listenerX * listenerX + listenerZ * listenerZ);
    if (distance > 200.0f) {
        listenerX = D_002FC9C0[0] + (listenerX / distance) * 200.0f;
        listenerZ = D_002FC9C0[2] + (listenerZ / distance) * 200.0f;
    } else {
        listenerX = fldCamTarget[0];
        listenerZ = fldCamTarget[2];
    }
    for (i = 0; i < gFldSe3DNum; i++) {
        dx = listenerX - fldSe3DTable[i].x;
        dy = listenerY - fldSe3DTable[i].y;
        dz = listenerZ - fldSe3DTable[i].z;
        distance = fsqrtf(dx * dx + dy * dy + dz * dz);
        if (distance < fldSe3DTable[i].r + 500.0f) {
            nearby = 1;
        }
    }
    if (nearby != 0 && (tick & 1)) {
        p2sdSetStandPos(listenerX, listenerY, listenerZ,
            D_002FC9C0[0], D_002FC9C0[1], D_002FC9C0[2]);
    }
    for (i = 0; i < gFldSe3DNum; i++) {
        entry = &fldSe3DTable[i];
        sourceX = fldSe3DTable[i].x;
        sourceY = fldSe3DTable[i].y;
        sourceZ = fldSe3DTable[i].z;
        dx = listenerX - sourceX;
        dy = listenerY - sourceY;
        dz = listenerZ - sourceZ;
        distance = fsqrtf(dx * dx + dy * dy + dz * dz);
        flags = entry->flg;
        if (!(flags & 1)) {
            if (distance < entry->r + 500.0f) {
                position.v[0] = sourceX;
                position.v[1] = sourceY;
                position.v[2] = sourceZ;
                entry->flg = flags | 1;
                p2sdPlaySe3DEx(gFldBgmTop[fldGlobalBase.area].smgId + entry->id,
                    (s32)(entry->r / 100.0f), position.v[0], position.v[1], position.v[2]);
            }
        } else if (distance < entry->r + 500.0f) {
            position.v[0] = sourceX;
            position.v[1] = sourceY;
            position.v[2] = sourceZ;
        } else {
            entry->flg = flags & ~1;
            p2sdStopSe(gFldBgmTop[fldGlobalBase.area].smgId + entry->id);
        }
    }
    D_0054EE34++;
}

INCLUDE_RODATA(const s32, "field/fldSe", D_00532110);

s32 fldGetBgmTrack(s32 code) {
    s32 track;

    switch (code) {
    case 0x80:
        track = gFldBgmTop[50].smgId + 1;
        break;
    case 0x81:
        track = gFldBgmTop[51].smgId + 1;
        break;
    case 0x82:
        track = gFldBgmTop[52].smgId + 1;
        break;
    case 0x83:
        track = gFldBgmTop[51].smgId + 2;
        break;
    case 0x84:
        track = gFldBgmTop[53].smgId + 1;
        break;
    case 0x85:
        track = gFldBgmTop[53].smgId + 2;
        break;
    case 0x86:
        track = gFldBgmTop[54].smgId + 1;
        break;
    case 0x87:
        track = gFldBgmTop[56].smgId + 1;
        break;
    case 0x88:
        track = gFldBgmTop[56].smgId + 2;
        break;
    case 0x89:
        track = gFldBgmTop[56].smgId + 3;
        break;
    case 0x8A:
        return 0x700001;
    case 0x8B:
        return 0x710001;
    case 0x8C:
        return 0x6E0000;
    case 0x8D:
        return 0x6E0001;
    default:
        track = -1;
        break;
    }
    return track;
}

INCLUDE_ASM(const s32, "field/fldSe", fldBgmPlay);

void fldPlayFieldBgm(void) {
    if (fldGlobalBase.sequenceCode == 0x80) {
        p2sdPlayBGM(fldBgmTrackTable[0] + 1);
    }
}

INCLUDE_ASM(const s32, "field/fldSe", fldBgmContinue);

void fldPauseBgm(void) {
    p2sdSendStopCmd(fldSeLastBgm);
    fldSeLastBgm = 0;
}

void fldBgmFadeOut(void) {
    p2sdStopBGM(fldSeLastBgm);
    if (fldGlobalBase.sequenceCode >= 0x80) {
        fldGlobalBase.sequenceCode = 1;
    }
    fldSeLastBgm = 0;
}

s32 fldBgmTransChk(s32 id1, s32 id2) {
    s8 first = gFldBgmTop[fldGb].trans[id1];
    s8 second = gFldBgmTop[fldGb].trans[id2];

    if (first != 0 && second != 0) {
        return 1;
    }
    return 0;
}

void fldBgmTrans(void) {
    s32 track = fldGetBgmTrack(fldGlobalBase.sequenceCode);

    if (track == -1) {
        if (fldGlobalBase.area >= 50) {
            return;
        }
        track = gFldBgmTop[fldGlobalBase.area].smgId + fldGlobalBase.sequenceCode;
    }
    p2sdSetBgmTrack(track);
}

void fldBgmStopAll(void) {
    p2sdStopAllBgm();
}

void fldBgmStop(void) {
    p2sdStopSe(fldSeLastBgm);
    fldGlobalBase.sequenceCode = 0;
}

void fldSePlay(s32 seid) {
    s32 id;

    if (fldGlobalBase.area >= 50) {
        return;
    }
    if (seid < 0x10) {
        return;
    }
    if (seid == 0x2D) {
        if (fldGlobalBase.area < 10) {
            return;
        }
    }
    if (seid == 0x80) {
        p2sdPlaySE(0x660060, 0x7F, 0x3F);
        return;
    }
    if (seid == 0x81) {
        p2sdPlaySE(0x660061, 0x7F, 0x3F);
        return;
    }
    if (seid == 0x82) {
        p2sdPlaySE(0x660064, 0x7F, 0x3F);
        return;
    }
    if (seid == 0x83) {
        p2sdPlaySE(0x6E0001, 0x7F, 0x3F);
        return;
    }
    id = fldSeSmgId + seid;
    if (fldGlobalBase.area == 20) {
        s32 floor = fldGlobalBase.floor;

        if (floor == 5 || floor == 8 || floor == 13 || floor == 14 || floor == 15 ||
            floor == 24 || (floor == 12 && evtBitCheck(0x16) != 0)) {
            id = DNG_MAX[0] + seid;
        }
    }
    p2sdPlaySE(id, 0x7F, 0x3F);
}

void fldSeStop(s32 seid) {
    s32 id;

    if (fldGlobalBase.area >= 50) {
        return;
    }
    if (seid == 0x100) {
        p2sdStopSe(0x660060);
        return;
    }
    if (seid == 0x101) {
        p2sdStopSe(0x660061);
        return;
    }
    if (seid == 0x102) {
        p2sdStopSe(0x660064);
        return;
    }
    id = fldSeSmgId + seid;
    if (fldGlobalBase.area == 20) {
        s32 floor = fldGlobalBase.floor;

        if (floor == 5 || floor == 8 || floor == 13 || floor == 14 || floor == 15 ||
            floor == 24 || (floor == 12 && evtBitCheck(0x16) != 0)) {
            id = DNG_MAX[0] + seid;
        }
    }
    p2sdStopSe(id);
}

void fldSeVol(s32 seid, s32 vol) {
    s32 id;

    if (fldGlobalBase.area >= 50) {
        return;
    }
    id = fldSeSmgId + seid;
    if (fldGlobalBase.area == 20) {
        s32 floor = fldGlobalBase.floor;

        if (floor == 5 || floor == 8 || floor == 13 || floor == 14 || floor == 15 ||
            floor == 24 || (floor == 12 && evtBitCheck(0x16) != 0)) {
            id = DNG_MAX[0] + seid;
        }
    }
    p2sdPlaySE(id, vol, 0x3F);
}

void fld_1457F8(s32 mode) {
    switch (mode) {
    case -1:
        if (D_0054EE3C != mode) {
            p2sdStopSe(D_0054EE3C);
            D_0054EE3C = mode;
        }
        break;
    case 0:
        mode = -1;
        if (D_0054EE3C != mode) {
            p2sdStopBGM(D_0054EE3C);
            D_0054EE3C = mode;
        }
        break;
    case 1:
        mode = fldBgmTrackTable[0] + 1;
        p2sdPlayBGM(mode);
        D_0054EE3C = mode;
        break;
    case 2:
        mode = D_0030A2D8[0] + 1;
        p2sdPlayBGM(mode);
        D_0054EE3C = mode;
        break;
    }
}

INCLUDE_ASM(const s32, "field/fldSe", fldBgmLoad);

s32 fldWaitBgmReload(void) {
    switch (fldBgmReloadStep) {
    case 0:
        if (p2sdLoadCheckMidi(MidiNo) == 0) {
            p2sdLoadSMGFile(MidiNo);
            fldBgmReloadStep = fldBgmReloadStep + 1;
        } else {
            fldBgmReloadStep = fldBgmReloadStep + 2;
        }
        break;
    case 1:
        if (p2sdLoadCheckMidi(MidiNo) == 1) {
            fldBgmReloadStep = fldBgmReloadStep + 1;
        }
        break;
    default:
        fldBgmReloadStep = -1;
        return 1;
    }
    return 0;
}

/* NON_MATCHING: 1 of 31 words differs; see docs/nonmatching/field/fldSe.md. */
#ifdef NON_MATCHING
void fldBgmBitChk(void)
{
    if (fldGb != 0x22) {
        return;
    }
    if (gFldBgmBit == 0) {
        if (evtBitCheck(gFldAreaBgm) != 0) {
            fldBgmFadeOut();
            fldBgmPlay();
        } else {
            return;
        }
    } else {
        if (evtBitCheck(gFldAreaBgm) != 0) {
            return;
        }
        fldBgmFadeOut();
        fldBgmPlay();
    }
}
#else
INCLUDE_ASM(const s32, "field/fldSe", fldBgmBitChk);
#endif


void fldMotionSePlay(s32 seid, s32 subid) {
    s32 cmd = 0x9010000;

    if (seid != 0) {
        cmd = 0x9000000 + ((seid << 16) + subid);
    }
    p2sdPlaySE(cmd, 0x7F, 0x3F);
}

void fldMotionSeStop(s32 seid, s32 subid) {
    s32 cmd = 0x9010000;

    if (seid != 0) {
        cmd = 0x9000000 + ((seid << 16) + subid);
    }
    p2sdStopSe(cmd);
}

s32 fldWaitBgmLoad(void) {
    switch (fldBgmLoadStep) {
    case 0:
        if (p2sdLoadCheckMidi(fldBgmSmgAddr) == 0) {
            p2sdLoadSMGFile(fldBgmSmgAddr);
            fldBgmLoadStep = fldBgmLoadStep + 1;
        } else {
            fldBgmLoadStep = fldBgmLoadStep + 2;
        }
        break;
    case 1:
        if (p2sdLoadCheckMidi(fldBgmSmgAddr) == 1) {
            fldBgmLoadStep = fldBgmLoadStep + 1;
        }
        break;
    default:
        fldBgmLoadStep = -1;
        return 1;
    }
    return 0;
}

s32 fldBgmIsLoading(void) {
    return fldBgmLoadStep != -1;
}

void fldStartBgmLoad(s32 kind) {
    if (fldBgmLoadStep != -1) {
        return;
    }
    switch (kind) {
    case 0:
        fldBgmSmgAddr = 0x310000;
        break;
    case 1:
        fldBgmSmgAddr = 0x390000;
        break;
    case 2:
        return;
    case 3:
    case 4:
        fldBgmSmgAddr = 0x20000;
        break;
    case 5:
        fldBgmSmgAddr = 0x300000;
        break;
    }
    fldBgmLoadStep = 0;
    fldWaitBgmLoad();
}

INCLUDE_ASM(const s32, "field/fldSe", fld_145DC0);
INCLUDE_SDATA(const s32, "field/fldSe", gFldBgmBit);

INCLUDE_SDATA(const s32, "field/fldSe", fldSeLastBgm);

INCLUDE_SDATA(const s32, "field/fldSe", fldSeSmgId);

INCLUDE_SDATA(const s32, "field/fldSe", gFldSndTransSrc);

INCLUDE_SDATA(const s32, "field/fldSe", gFldSe3DNum);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EE34);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EE38);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EE3C);

INCLUDE_SDATA(const s32, "field/fldSe", MidiNo);

INCLUDE_SDATA(const s32, "field/fldSe", fldBgmReloadStep);

INCLUDE_SDATA(const s32, "field/fldSe", fldBgmSmgAddr);

INCLUDE_SDATA(const s32, "field/fldSe", fldBgmLoadStep);

INCLUDE_SDATA(const s32, "field/fldSe", fldAutoMapField);

INCLUDE_SDATA(const s32, "field/fldSe", fldAutoMapSeqStep);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EE58);

INCLUDE_SDATA(const s32, "field/fldSe", fldAutoMapSeqState);

INCLUDE_SDATA(const s32, "field/fldSe", AutoMapSeq);

INCLUDE_SDATA(const s32, "field/fldSe", fldAutoMapHeader);

INCLUDE_SDATA(const s32, "field/fldSe", fldAutoMapData);

INCLUDE_SDATA(const s32, "field/fldSe", fldAutoMapBuf);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EE70);

INCLUDE_SDATA(const s32, "field/fldSe", fldAutoMapBBox);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EE78);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EE7C);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EE80);

INCLUDE_SDATA(const s32, "field/fldSe", fldAutoMapDrawX);

INCLUDE_SDATA(const s32, "field/fldSe", fldAutoMapDrawY);

INCLUDE_SDATA(const s32, "field/fldSe", fldAutoMapDrawState);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EE90);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EE94);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EE98);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EE9C);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EEA0);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EEA4);

INCLUDE_SDATA(const s32, "field/fldSe", fldAreaFontWork);

INCLUDE_SDATA(const s32, "field/fldSe", fldNpcCamTarget);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EEB0);

INCLUDE_SDATA(const s32, "field/fldSe", fldNpcModelTable);

INCLUDE_SDATA(const s32, "field/fldSe", fldUiTexture);

INCLUDE_SDATA(const s32, "field/fldSe", fldEATexture);

INCLUDE_SDATA(const s32, "field/fldSe", fldVLTexture);

INCLUDE_SDATA(const s32, "field/fldSe", fldVLBlurHandle);

INCLUDE_SDATA(const s32, "field/fldSe", fldSnowBlurHandle);

INCLUDE_SDATA(const s32, "field/fldSe", fldNpcLoadIdx);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EED0);

INCLUDE_SDATA(const s32, "field/fldSe", fldDmgBlurHandle);

INCLUDE_SDATA(const s32, "field/fldSe", fldNpcBlurHandle);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EEDC);

INCLUDE_SDATA(const s32, "field/fldSe", fldNpcFreeCount);

INCLUDE_SDATA(const s32, "field/fldSe", fldNpcModelFreeIdx);

INCLUDE_SDATA(const s32, "field/fldSe", fldNpcDrawCount);

INCLUDE_SDATA(const s32, "field/fldSe", fldNpcModelCount);

INCLUDE_SDATA(const s32, "field/fldSe", fldGenNpcData);

INCLUDE_SDATA(const s32, "field/fldSe", fldGenNpcSize);

INCLUDE_SDATA(const s32, "field/fldSe", fldTakara1File);

INCLUDE_SDATA(const s32, "field/fldSe", fldTakara1Data);

INCLUDE_SDATA(const s32, "field/fldSe", fldTakara1Size);

INCLUDE_SDATA(const s32, "field/fldSe", fldTakara3File);

INCLUDE_SDATA(const s32, "field/fldSe", fldTakara3Data);

INCLUDE_SDATA(const s32, "field/fldSe", fldTakara3Size);

INCLUDE_SDATA(const s32, "field/fldSe", fldTakara2File);

INCLUDE_SDATA(const s32, "field/fldSe", fldTakara2Data);

INCLUDE_SDATA(const s32, "field/fldSe", fldTakara2Size);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF1C);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF20);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF24);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF28);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF2C);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF30);

INCLUDE_SDATA(const s32, "field/fldSe", fldItemBoxCount);

INCLUDE_SDATA(const s32, "field/fldSe", hEffPart);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF3C);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF40);

INCLUDE_SDATA(const s32, "field/fldSe", fldTakaraSlotCount);

INCLUDE_SDATA(const s32, "field/fldSe", fldTakaraCount);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF4C);

INCLUDE_SDATA(const s32, "field/fldSe", fldEADrawState);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF54);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF58);

INCLUDE_SDATA(const s32, "field/fldSe", fldEADrawWork);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF60);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF64);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF68);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF6C);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF70);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF74);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF78);

INCLUDE_SDATA(const s32, "field/fldSe", fldCamNpcTarget);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF80);

INCLUDE_SDATA(const s32, "field/fldSe", fldCamNpcTarget2);

INCLUDE_SDATA(const s32, "field/fldSe", fldTitleState);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF8C);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF90);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF94);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF98);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EF9C);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EFA0);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EFA4);

INCLUDE_SDATA(const s32, "field/fldSe", fldTitleMiniState);

INCLUDE_SDATA(const s32, "field/fldSe", fldSnowActive);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EFB0);

INCLUDE_SDATA(const s32, "field/fldSe", fldTitleMiniTex);

INCLUDE_SDATA(const s32, "field/fldSe", fldTitleMiniWork);

INCLUDE_SDATA(const s32, "field/fldSe", gFldSeedBuff);

INCLUDE_SDATA(const s32, "field/fldSe", fldSnowSeedBuf);

INCLUDE_SDATA(const s32, "field/fldSe", gFldSnowMode);

INCLUDE_SDATA(const s32, "field/fldSe", gFldSnowX);

INCLUDE_SDATA(const s32, "field/fldSe", gFldSnowZ);

INCLUDE_SDATA(const s32, "field/fldSe", gFldSnowY);

INCLUDE_SDATA(const s32, "field/fldSe", gFldSnowYD);

INCLUDE_SDATA(const s32, "field/fldSe", gFldSnowSpd);

INCLUDE_SDATA(const s32, "field/fldSe", gFldSnowLife);

INCLUDE_SDATA(const s32, "field/fldSe", gFldSnowW);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054EFE4);

INCLUDE_SDATA(const s32, "field/fldSe", slDDS3To31Flg);

INCLUDE_SDATA(const s32, "field/fldSe", slProcShutDownFlg);

INCLUDE_SDATA(const s32, "field/fldSe", slProcPtr);

INCLUDE_SDATA(const s32, "field/fldSe", _saveUI);

INCLUDE_SDATA(const s32, "field/fldSe", slExitWaitCnt);

INCLUDE_SDATA(const s32, "field/fldSe", baseIconFileHandle);

INCLUDE_SDATA(const s32, "field/fldSe", gslCheckCardWait);

INCLUDE_SDATA(const s32, "field/fldSe", gslDrawFlagFile);

INCLUDE_SDATA(const s32, "field/fldSe", gslDrawFlagBg);

INCLUDE_SDATA(const s32, "field/fldSe", gslAnimCnt);

INCLUDE_SDATA(const s32, "field/fldSe", gSlSelCnt);

INCLUDE_SDATA(const s32, "field/fldSe", gSlNowCardCheck);

INCLUDE_SDATA(const s32, "field/fldSe", gSlExitMode);

INCLUDE_SDATA(const s32, "field/fldSe", gSlFileErroeFlg);

INCLUDE_SDATA(const s32, "field/fldSe", gSlSaveMode);

INCLUDE_SDATA(const s32, "field/fldSe", gSlEndingMode);

INCLUDE_SDATA(const s32, "field/fldSe", slChooseSaveIndex);

INCLUDE_SDATA(const s32, "field/fldSe", slQueryYesNoCursor);

INCLUDE_SDATA(const s32, "field/fldSe", slQueryYesNoMsg);

INCLUDE_SDATA(const s32, "field/fldSe", slQueryYesNoKeyFlag);

INCLUDE_SDATA(const s32, "field/fldSe", slFileSelError);

INCLUDE_SDATA(const s32, "field/fldSe", gslFlushCnt);

INCLUDE_SDATA(const s32, "field/fldSe", gslMode);

INCLUDE_SDATA(const s32, "field/fldSe", gslInfoLoadFlg);

INCLUDE_SDATA(const s32, "field/fldSe", gslExitType);

INCLUDE_SDATA(const s32, "field/fldSe", gslInfoCode);

INCLUDE_SDATA(const s32, "field/fldSe", gslErrorCode);

INCLUDE_SDATA(const s32, "field/fldSe", gslErrorKeyFlag);

INCLUDE_SDATA(const s32, "field/fldSe", version_convert_ver);

INCLUDE_SDATA(const s32, "field/fldSe", slNextPageDraw_fc);

INCLUDE_SDATA(const s32, "field/fldSe", gSlMcAccessCnt);

INCLUDE_SDATA(const s32, "field/fldSe", slMcAccessDraw_fc);

INCLUDE_SDATA(const s32, "field/fldSe", gSlWindowCnt);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054F06C);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054F070);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054F074);

INCLUDE_SDATA(const s32, "field/fldSe", slFileSelWork);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054F07C);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054F080);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054F088);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054F090);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054F098);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054F0A0);

INCLUDE_SDATA(const s32, "field/fldSe", D_0054F0A8);

