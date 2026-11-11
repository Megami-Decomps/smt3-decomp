#include "common.h"
#include "fpu.h"
#include "basic.h"
#include "model.h"
#include "newdata.h"
#include "scr.h"
#include "field.h"
#include "ps2asm.h"
#include "sdfFrame.h"

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

extern s32 fldSeBgmId;

extern s32 fldSceneBgmTrackTable[];

extern s32 fldBgmReloadStep;

extern s32 p2sdLoadCheckMidi(s32);

extern s32 MidiNo;

extern void p2sdLoadSMGFile(s32);

extern s32 fldBgmLoadStep;

extern s32 fldBgmSmgAddr;

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
extern f32 fldCamEye[];
/* Native frame counters wrap and have no signed consumers. */
extern u32 fldSe3DTick;
extern u32 fldSe3DCalls;
typedef struct {
    f32 v[4];
} fldSoundPosition_t;
extern const fldSoundPosition_t fldSe3DDefaultPos;
void p2sdPlaySe3DEx(s32 trackId, s32 radiusInHundreds, f32 x, f32 y, f32 z);
void p2sdSetStandPos(f32 firstX, f32 firstY, f32 firstZ,
                    f32 secondX, f32 secondY, f32 secondZ);

/* Adapted from DDS func_001415F8: SMT3 always uses the camera listener and
 * a fixed 500-unit radius margin, with the current area resolved per call. */
void fldPlaySe3D(void) {
    fldSoundPosition_t position = fldSe3DDefaultPos;
    f32 listenerX, listenerY, listenerZ;
    f32 dx, dy, dz, distance;
    f32 sourceX, sourceY, sourceZ;
    u32 tick;
    s32 nearby;
    s32 flags;
    s32 i;
    fldSe3D_tag *entry;

    tick = fldSe3DTick + 1;
    nearby = 0;
    fldSe3DTick = tick;
    listenerX = fldCamTarget[0] - fldCamEye[0];
    listenerY = fldCamEye[1];
    listenerZ = fldCamTarget[2] - fldCamEye[2];
    distance = fsqrtf(listenerX * listenerX + listenerZ * listenerZ);
    if (distance > 200.0f) {
        listenerX = fldCamEye[0] + (listenerX / distance) * 200.0f;
        listenerZ = fldCamEye[2] + (listenerZ / distance) * 200.0f;
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
    if (nearby != 0 && (SDF_FRAME_TICK(tick) & 1) && SDF_FRAME_DUE(tick)) {
        p2sdSetStandPos(listenerX, listenerY, listenerZ,
            fldCamEye[0], fldCamEye[1], fldCamEye[2]);
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
    fldSe3DCalls++;
}

const fldSoundPosition_t fldSe3DDefaultPos = { { 0.0f, -100.0f, 0.0f, 1.0f } };

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

typedef struct {
    s16 bit;
    s8 normal[32];
    s8 alternate[32];
} fldBgmParam_t;

extern fldBgmParam_t fldBgmParamTable[];
extern s32 gFldStart;
extern void fldBgmStop(void);
extern void p2sdResumeBgm(s32 track);

/* Actual bank snapshots have separate phases; see docs/matching/field/fldSe.md. */
void fldBgmPlay(void)
{
    s32 track;
    s32 base;
    s32 floor;

    if (fldGlobalBase.area == 20 && fldGlobalBase.floor == 2) {
        return;
    }
    track = fldGetBgmTrack(fldGlobalBase.sequenceCode);
    if (track == -1) {
        if (fldGlobalBase.area >= 50) {
            return;
        }
        base = gFldBgmTop[fldGlobalBase.area].smgId;
        fldSeSmgId = base;
        /* The published bank also feeds the track sum. Keep those two real
         * uses on either side of this local reconstruction aid, matching
         * native publication-before-add order; this is not a memory barrier. */
        FAKEMATCH_ASM_LIVE(base);
        track = base + fldGlobalBase.sequenceCode;
        {
            /* Actual area-14 comparison value, dead before evtBitCheck or
             * any other call. The binding preserves the native operand choice. */
            FAKEMATCH_ASM_REG(s32, specialArea, "$4") = 14;
            if (fldGlobalBase.area == specialArea && evtBitCheck(5) != 0) {
                if (fldGlobalBase.sequenceCode != 1) {
                    fldBgmStop();
                }
                fldGlobalBase.sequenceCode = 1;
                {
                    s32 refreshBase = gFldBgmTop[fldGlobalBase.area].smgId;
                    fldSeSmgId = refreshBase;
                    track = refreshBase + 1;
                }
            }
        }
        if (fldGlobalBase.area == 20) {
            floor = fldGlobalBase.floor;
            if (floor == 5 || floor == 8 || floor == 13 || floor == 14 ||
                floor == 15 || floor == 24 || (floor == 12 && evtBitCheck(0x16) != 0)) {
                fldSeSmgId = gFldBgmTop[55].smgId;
                track = fldSeSmgId + fldGlobalBase.sequenceCode;
            }
        }
        if (fldGlobalBase.area == 0x22 || fldGlobalBase.area == 0x20 ||
            fldGlobalBase.area == 0x25 || fldGlobalBase.area == 0x24 ||
            fldGlobalBase.area == 0x29 || fldGlobalBase.area == 0x2A ||
            fldGlobalBase.area == 0x2B || fldGlobalBase.area == 0x2C ||
            fldGlobalBase.area == 0x2D) {
            if (evtBitCheck(fldBgmParamTable[fldGlobalBase.area].bit) != 0) {
                gFldBgmBit = 1;
                fldGlobalBase.sequenceCode = fldBgmParamTable[fldGlobalBase.area].alternate[fldGlobalBase.floor + 1];
            } else {
                gFldBgmBit = 0;
                fldGlobalBase.sequenceCode = fldBgmParamTable[fldGlobalBase.area].normal[fldGlobalBase.floor + 1];
            }
            {
                s32 selectedBase = gFldBgmTop[fldGlobalBase.area].smgId;
                fldSeSmgId = selectedBase;
                track = selectedBase + fldGlobalBase.sequenceCode;
            }
        }
    }
    if (gFldStart == 1) {
        return;
    }
    if (fldSeLastBgm != track) {
        p2sdStopBGM(fldSeLastBgm);
    }
    fldSeLastBgm = track;
    p2sdPlayBGM(track);
}

void fldPlayFieldBgm(void) {
    if (fldGlobalBase.sequenceCode == 0x80) {
        p2sdPlayBGM(fldBgmTrackTable[0] + 1);
    }
}

/* Bank snapshots are refreshed at the native callback boundaries. */
void fldBgmContinue(void)
{
    s32 track = fldGetBgmTrack(fldGlobalBase.sequenceCode);
    s32 base;
    s32 floor;

    if (track == -1) {
        if (fldGlobalBase.area >= 50) {
            return;
        }
        {
            s32 initialBase = gFldBgmTop[fldGlobalBase.area].smgId;
            track = (s32)((u32)initialBase + (u32)fldGlobalBase.sequenceCode);
            base = initialBase;
        }
        if (fldGlobalBase.area == 14) {
            fldSeSmgId = base;
            if (evtBitCheck(5) != 0) {
                if (fldGlobalBase.sequenceCode != 1) {
                    fldBgmStop();
                }
                fldGlobalBase.sequenceCode = 1;
            }
            {
                s32 refreshedBase = gFldBgmTop[fldGlobalBase.area].smgId;
                track = (s32)((u32)refreshedBase + (u32)fldGlobalBase.sequenceCode);
                base = refreshedBase;
            }
        }
        if (fldGlobalBase.area == 20) {
            floor = fldGlobalBase.floor;
            if (floor != 5 && floor != 8 && floor != 13 && floor != 14 &&
                floor != 15 && floor != 24) {
                fldSeSmgId = base;
                if (floor != 12 || evtBitCheck(0x16) == 0) {
                    goto specialBankDone;
                }
            }
            fldSeSmgId = gFldBgmTop[55].smgId;
            track = (s32)((u32)fldSeSmgId + (u32)fldGlobalBase.sequenceCode);
        } else {
            fldSeSmgId = base;
        }
specialBankDone:
        if (fldGlobalBase.area == 0x22 || fldGlobalBase.area == 0x20 ||
            fldGlobalBase.area == 0x25 || fldGlobalBase.area == 0x24 ||
            fldGlobalBase.area == 0x29 || fldGlobalBase.area == 0x2A ||
            fldGlobalBase.area == 0x2B || fldGlobalBase.area == 0x2C ||
            fldGlobalBase.area == 0x2D) {
            if (evtBitCheck(fldBgmParamTable[fldGlobalBase.area].bit) != 0) {
                fldGlobalBase.sequenceCode = fldBgmParamTable[fldGlobalBase.area].alternate[fldGlobalBase.floor + 1];
            } else {
                fldGlobalBase.sequenceCode = fldBgmParamTable[fldGlobalBase.area].normal[fldGlobalBase.floor + 1];
            }
            {
                s32 selectedBase = gFldBgmTop[fldGlobalBase.area].smgId;
                fldSeSmgId = selectedBase;
                track = (s32)((u32)selectedBase + (u32)fldGlobalBase.sequenceCode);
            }
        }
    }
    if (gFldStart == 1) {
        return;
    }
    fldSeLastBgm = track;
    p2sdResumeBgm(track);
}

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
    s8 first = gFldBgmTop[fldGlobalBase.area].trans[id1];
    s8 second = gFldBgmTop[fldGlobalBase.area].trans[id2];

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
        if (fldSeBgmId != mode) {
            p2sdStopSe(fldSeBgmId);
            fldSeBgmId = mode;
        }
        break;
    case 0:
        mode = -1;
        if (fldSeBgmId != mode) {
            p2sdStopBGM(fldSeBgmId);
            fldSeBgmId = mode;
        }
        break;
    case 1:
        mode = fldBgmTrackTable[0] + 1;
        p2sdPlayBGM(mode);
        fldSeBgmId = mode;
        break;
    case 2:
        mode = fldSceneBgmTrackTable[0] + 1;
        p2sdPlayBGM(mode);
        fldSeBgmId = mode;
        break;
    }
}

/* Bank snapshots are refreshed at the native callback boundaries before
 * the selected track is published for the staged reload process. */
void fldBgmLoad(void)
{
    s32 track;
    s32 base;
    s32 floor;

    if (fldGlobalBase.area == 20 && fldGlobalBase.floor == 2) {
        return;
    }
    track = fldGetBgmTrack(fldGlobalBase.sequenceCode);
    if (track == -1) {
        if (fldGlobalBase.area >= 50) {
            return;
        }
        {
            s32 initialBase = gFldBgmTop[fldGlobalBase.area].smgId;
            track = (s32)((u32)initialBase + (u32)fldGlobalBase.sequenceCode);
            base = initialBase;
        }
        if (fldGlobalBase.area == 14) {
            fldSeSmgId = base;
            if (evtBitCheck(5) != 0) {
                if (fldGlobalBase.sequenceCode != 1) {
                    fldBgmStop();
                }
                fldGlobalBase.sequenceCode = 1;
            }
            {
                s32 refreshedBase = gFldBgmTop[fldGlobalBase.area].smgId;
                track = (s32)((u32)refreshedBase + (u32)fldGlobalBase.sequenceCode);
                base = refreshedBase;
            }
        }
        if (fldGlobalBase.area == 20) {
            floor = fldGlobalBase.floor;
            if (floor != 5 && floor != 8 && floor != 13 && floor != 14 &&
                floor != 15 && floor != 24) {
                fldSeSmgId = base;
                if (floor != 12 || evtBitCheck(0x16) == 0) {
                    goto specialBankDone;
                }
            }
            fldSeSmgId = gFldBgmTop[55].smgId;
            track = (s32)((u32)fldSeSmgId + (u32)fldGlobalBase.sequenceCode);
        } else {
            fldSeSmgId = base;
        }
specialBankDone:
        if (fldGlobalBase.area == 0x22 || fldGlobalBase.area == 0x20 ||
            fldGlobalBase.area == 0x25 || fldGlobalBase.area == 0x24 ||
            fldGlobalBase.area == 0x29 || fldGlobalBase.area == 0x2A ||
            fldGlobalBase.area == 0x2B || fldGlobalBase.area == 0x2C ||
            fldGlobalBase.area == 0x2D) {
            if (evtBitCheck(fldBgmParamTable[fldGlobalBase.area].bit) != 0) {
                gFldBgmBit = 1;
                fldGlobalBase.sequenceCode = fldBgmParamTable[fldGlobalBase.area].alternate[fldGlobalBase.floor + 1];
            } else {
                gFldBgmBit = 0;
                fldGlobalBase.sequenceCode = fldBgmParamTable[fldGlobalBase.area].normal[fldGlobalBase.floor + 1];
            }
            {
                s32 selectedBase = gFldBgmTop[fldGlobalBase.area].smgId;
                fldSeSmgId = selectedBase;
                track = (s32)((u32)selectedBase + (u32)fldGlobalBase.sequenceCode);
            }
        }
    }
    MidiNo = track;
    fldBgmReloadStep = 0;
}

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

/* Exact with the real fldBgmPlay C provider visible; see matching notes. */
void fldBgmBitChk(void)
{
    if (fldGlobalBase.area != 0x22) {
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

extern f32 fldCamEye[];
extern f32 fldCamTarget[];
extern void p2sdSetStandPos(f32 cameraX, f32 cameraY, f32 cameraZ,
                           f32 playerX, f32 playerY, f32 playerZ);
extern void p2sdPlaySe3DEx(s32 midiNo, s32 x, f32 y, f32 z, f32 limit);

void fld_145DC0(s32 soundId, f32 x, f32 y, f32 z, f32 limit)
{
    f32 listenerX = fldCamTarget[0] - fldCamEye[0];
    f32 listenerY = fldCamEye[1];
    f32 listenerZ = fldCamTarget[2] - fldCamEye[2];
    f32 distance = fsqrtf(listenerX * listenerX + listenerZ * listenerZ);

    if (distance > 200.0f) {
        listenerX = fldCamEye[0] + listenerX / distance * 200.0f;
        listenerZ = fldCamEye[2] + listenerZ / distance * 200.0f;
    } else {
        listenerX = fldCamTarget[0];
        listenerZ = fldCamTarget[2];
    }
    p2sdSetStandPos(listenerX, listenerY, listenerZ,
                    fldCamEye[0], fldCamEye[1], fldCamEye[2]);
    p2sdPlaySe3DEx(gFldBgmTop[fldGlobalBase.area].smgId + soundId, (s32)(x / 100.0f), y, z, limit);
}

u32 gFldBgmBit = 0;

u32 fldSeLastBgm = 0;

s32 fldSeSmgId = 0;

s32 gFldSndTransSrc = 0;

s32 gFldSe3DNum = 0;

u32 fldSe3DCalls = 0;

u32 fldSe3DTick = 0;

s32 fldSeBgmId = -1;

s32 MidiNo = -1;

s32 fldBgmReloadStep = -1;

s32 fldBgmSmgAddr = -1;

s32 fldBgmLoadStep = -1;

s32 fldAutoMapField = 0;

s32 fldAutoMapSeqStep = 0;

s32 fldAutoMapStartFloor = 0;

s32 fldAutoMapSeqState = 0;

u32 AutoMapSeq = 0;

fldAmAreaData_t *fldAutoMapHeader = NULL;

s32 fldAutoMapData = 0;

sdfMemHandle_t *fldAutoMapBuf = NULL;

f32 fldAutoMapPulsePhase = 0.0f;

s32 fldAutoMapBBox = 0;

s32 fldAutoMapFloorCur = 1;

s32 fldAutoMapFloorOld = 1;

f32 fldAutoMapFloorOfsY = 0.0f;

f32 fldAutoMapDrawX = 0.0f;

s32 fldAutoMapDrawY = 128;

s32 fldAutoMapDrawState = 0;

s32 fldAutoMapSlideCount = 0;

f32 fldAutoMapBlinkPhase = 1.0f;

f32 D_0054EE98 = 0.0f;

f32 fldAutoMapBobPhase = 0.0f;

f32 fldAutoMapBgScroll = 0.0f;

f32 fldAreaFontAlpha = 1.0f;

s32 fldAreaFontWork = 0;

s32 fldNpcCamTarget = 0;

s32 fldNpcCamSlot = 0;

s32 fldNpcModelTable = 0;

sdfTexHandle_t *fldUiTexture = NULL;

sdfTexHandle_t *fldEATexture = NULL;

sdfTexHandle_t *fldVLTexture = NULL;

sdfTexHandle_t *fldVLBlurHandle = NULL;

sdfTexHandle_t *fldSnowBlurHandle = NULL;

struct effNode_t *fldNpcLoadIdx = NULL;

void *fldDmgBlurEffect = NULL;

u32 fldDmgBlurHandle = 0;

u32 fldNpcBlurHandle = 0;

void *fldTDmgEffect = NULL;

dds3Basic_t *fldNpcFreeCount = NULL;

s32 fldNpcModelFreeIdx = -1;

s32 fldNpcDrawCount = 0;

sdfMemHandle_t *fldNpcModelCount = NULL;

u8 *fldGenNpcData = NULL;

u32 fldGenNpcSize = 0;

sdfMemHandle_t *fldTakara1File = NULL;

u8 *fldTakara1Data = NULL;

s32 fldTakara1Size = 0;

sdfMemHandle_t *fldTakara3File = NULL;

u8 *fldTakara3Data = NULL;

s32 fldTakara3Size = 0;

sdfMemHandle_t *fldTakara2File = NULL;

u8 *fldTakara2Data = NULL;

s32 fldTakara2Size = 0;

sdfMemHandle_t *fldTakara1Mem = NULL;

sdfMemHandle_t *fldTakara3Mem = NULL;

sdfMemHandle_t *fldTakara2Mem = NULL;

void *fldItemBoxEffectHandle = NULL;

void *fldItemBoxEffectData = NULL;

s32 D_0054EF30 = 0;

void *fldItemBoxCount = NULL;

sdfTexHandle_t *hEffPart = NULL;

void *fldItemBoxTexHandle = NULL;

void *fldItemBoxTexData = NULL;

struct effNode_t *fldTakaraSlotCount = NULL;

s32 fldTakaraCount = 0;

f32 fldHudSlide = 1.0f;

f32 fldEADrawState = 0.0f;

s32 fldEAPrevAlpha = 0;

s32 fldEAAlphaRising = 0;

s32 fldEADrawWork = 0;

f32 fldVLPulsePhase = 0.0f;

f32 fldWaveDistance = 0.0f;

s32 fldWaveFrames = 0;

f32 fldNpcCamPosX = 0.0f;

f32 fldNpcCamPosY = 0.0f;

f32 fldNpcCamPosZ = 0.0f;

f32 fldNpcCamAngle = 0.0f;

f32 fldCamNpcTarget = 0.0f;

f32 fldNpcCamTargetY = 0.0f;

f32 fldCamNpcTarget2 = 0.0f;

s32 fldTitleState = 0;

s32 fldTitlePhase = 99;

s32 fldTitleFld = 0;

s32 fldTitleWaitFrames = 0;

sdfTexHandle_t *fldTitleFldTex = NULL;

sdfTexHandle_t *fldTitleDun1Tex = NULL;

sdfTexHandle_t *fldTitleDun2Tex = NULL;

sdfTexHandle_t *fldTitleDun3Tex = NULL;

s32 fldTitleMiniState = 0;

s32 fldSnowActive = 99;

s32 fldTitleMiniFld = 0;

sdfTexHandle_t *fldTitleMiniTex = NULL;

sdfTexHandle_t *fldTitleMiniWork = NULL;

void *gFldSeedBuff = NULL;

sdfMemHandle_t *fldSnowSeedBuf = NULL;

s32 gFldSnowMode = 0;

f32 gFldSnowX = -640.0f;

f32 gFldSnowZ = -640.0f;

f32 gFldSnowY = -6900.0f;

f32 gFldSnowYD = 10000.0f;

f32 gFldSnowSpd = 4.0f;

s32 gFldSnowLife = 1500;

f32 gFldSnowW = 5.0f;

SDATA_PAD(4);

s32 slDDS3To31Flg = 0;

s32 slProcShutDownFlg = 0;

sdfTexHandle_t *slProcPtr = NULL;

sdfTexHandle_t *_saveUI = NULL;

s32 slExitWaitCnt = 0;

addr_t baseIconFileHandle = 0;

s32 gslCheckCardWait = 0;

u32 gslDrawFlagFile = 0;

u32 gslDrawFlagBg = 0;

s32 gslAnimCnt = 0;

s32 gSlSelCnt = 0;

u32 gSlNowCardCheck = 0;

u32 gSlExitMode = 0;

s32 gSlFileErroeFlg = 0;

s32 gSlSaveMode = 0;

u32 gSlEndingMode = 0;

s32 slChooseSaveIndex = 0;

s32 slQueryYesNoCursor = -1;

u32 slQueryYesNoMsg = 0;

s32 slQueryYesNoKeyFlag = 0;

s32 slFileSelError = 0;

s32 gslFlushCnt = 0;

s32 gslMode = 0;

s32 gslInfoLoadFlg = 0;

u32 gslExitType = 0;

s32 gslInfoCode = 0;

u32 gslErrorCode = 0;

u32 gslErrorKeyFlag = 0;

s32 version_convert_ver = 5;

f32 slNextPageDraw_fc = 0.0f;

s32 gSlMcAccessCnt = 0;

f32 slMcAccessDraw_fc = 0.0f;

s32 gSlWindowCnt = 0;

extern u8 slIconSysBuf[];
void *slIconSysSaveBuff = slIconSysBuf;

u32 slIconSysSaveSize = 0x3c4;

f32 slFileSelPulsePhase = 0.0f;

f32 slFileSelWork = 0.0f;

SDATA_PAD(4);

char slNum2dFmt[8] = "%2d";

char slNum3dFmt[8] = "%3d";

char slNum02dFmt[8] = "%02d";

char D_0054F098[8] = "NORMAL";

char D_0054F0A0[8] = "HARD";

char fldHojiNameKey[8] = "hoji";

