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

typedef struct fldInfHit_t fldInfHit_t;

typedef struct fldInfPack_t fldInfPack_t;

typedef struct fldInfView_t fldInfView_t;

typedef struct fldInfExtraAction_t fldInfExtraAction_t;

typedef struct fldInfFlagSelector_t fldInfFlagSelector_t;

typedef struct fldInfMessageRow_t fldInfMessageRow_t;

typedef struct fldInfSet_t fldInfSet_t;

typedef struct fldInfTable_t fldInfTable_t;

typedef struct fldActorEntry_t fldActorEntry_t;

struct fldInfHit_t {
    s8 area;
    char eventName[15];
};

struct fldInfPack_t {
    s32 setIndex;
    fldInfHit_t hits[5];
};

struct fldInfView_t {
    s32 player;
    s32 motion;
    char positionName[12];
    char cameraName[12];
};

struct fldInfExtraAction_t {
    s32 action;
    s32 parameter0;
    s32 parameter1;
    s32 parameter2;
    s32 parameter3;
};

struct fldInfFlagSelector_t {
    s16 flag;
    s8 offTarget;
    s8 onTarget;
};

struct fldInfMessageRow_t {
    s16 kind;
    s16 message;
    s8 targets[4];
    s16 flags[2];
    u8 view;
    u8 extraAction;
};

struct fldInfSet_t {
    union {
        s16 packed;
        struct {
            u8 kind;
            u8 area;
        } bytes;
    } kindArea;
    s8 action;
    s8 eventHit;
    char eventName[12];
    fldInfFlagSelector_t flags[4];
    fldInfMessageRow_t messages[20];
};

struct fldInfTable_t {
    fldInfPack_t packs[8];
    fldInfView_t views[40];
    fldInfExtraAction_t extraActions[40];
    fldInfSet_t sets[40];
};

typedef struct devState_t devState_t;

struct fldActorEntry_t {
    s8 kind;
    u8 pad01;
    s16 requiredFlag;
    s16 floor;
    char name[0xC];
    s16 motion;
    s16 secondaryMotion;
    s16 sound;
    char motionName[0xC];
    char otherName[0xC];
    s8 variantMode;
    s8 flags31;
    s16 variant;
    s16 sequenceKind;
    s16 sequenceValue;
    char sequenceName[0xC];
    s8 linkKind;
    s8 rowIndex;
    char linkName[0xC];
    s8 sequenceCode;
    s8 selectedRoom;
    s8 flags54;
    char taskName[0xF];
};

dds3Basic_t *dds3GetActiveScene(void);

extern u32 gEvnRngPid[];

extern scrProcessWork_s *scrSearchScriptName(char *scriptname);

extern fldFileResource_t *gEvnRng[];

s32 dds3SceneBasicStartScript(dds3Basic_t *scene, const char *procname);

extern s32 strcmp(const char *left, const char *right);

extern fldInfTable_t fldInfTable;

extern s32 gFldInfIdx;

extern const char D_002FF3E0[];

void fldMakeFilePath(char *pathstr, s32 field, s32 area);

devState_t *sdfFileOpen(const char *aFileName);

void sdfFileRead(devState_t *aHandle, void *aPtr, s32 aLeng);

void sdfFileClose(devState_t *aHandle);

extern s32 sprintf(char *, const char *, ...);

extern const char D_00531E98[];

extern void *memcpy(void *, const void *, unsigned int);

extern s32 gFldInf_HitIdx;

extern s32 gFldInf_WpExit;

extern u8 *fldDoor_Find2(const char *);

s32 evtBitCheck(s32 no);

/* fldDoorTable is fldDoorParamTable.actors: the same 100-byte waypoint record,
   reached through its own symbol at fldDoorParamTable + 0xA0. */
extern fldActorEntry_t fldDoorTable[256];

extern s32 gFldWapPanel;

extern u8 D_00303020[];

const char *fldInf_FindTypeEveNpc(const char *evename) {
    s32 packOrSetIndex;
    s32 eventHitIndex;
    if (evename == 0) return 0;
    for (packOrSetIndex = 0; packOrSetIndex < 8; packOrSetIndex++) {
        if (fldInfTable.packs[packOrSetIndex].setIndex != 0) {
            for (eventHitIndex = 0; eventHitIndex < 5; eventHitIndex++) {
                if (fldInfTable.packs[packOrSetIndex].hits[eventHitIndex].area == fldGlobalBase.floor + 1 &&
                    strcmp(evename, fldInfTable.packs[packOrSetIndex].hits[eventHitIndex].eventName) == 0) {
                    gFldInfIdx = fldInfTable.packs[packOrSetIndex].setIndex;
                    return D_002FF3E0;
                }
            }
        }
    }
    for (packOrSetIndex = 0; packOrSetIndex < 40; packOrSetIndex++) {
        if ((u16)fldInfTable.sets[packOrSetIndex].kindArea.packed < 2 &&
            fldInfTable.sets[packOrSetIndex].action == fldGlobalBase.floor + 1 &&
            strcmp(evename, fldInfTable.sets[packOrSetIndex].eventName) == 0) {
            gFldInfIdx = packOrSetIndex;
            return D_002FF3E0;
        }
    }
    return 0;
}

/* NON_MATCHING: 183 of 184 words differ (first at +0x0). */
#ifdef NON_MATCHING
s32 fldInfChkFastPanel(s32 aline) {
    s32 value;
    s32 slot;
    s32 index;

    if (aline < 0xA) {
        index = aline - 1;
        if (fldInfTable.sets[gFldInfIdx].flags[index].flag == 0) {
            value = fldInfTable.sets[gFldInfIdx].flags[index].offTarget;
        } else if (evtBitCheck(fldInfTable.sets[gFldInfIdx].flags[index].flag) != 0) {
            value = fldInfTable.sets[gFldInfIdx].flags[index].onTarget;
        } else {
            value = fldInfTable.sets[gFldInfIdx].flags[index].offTarget;
        }
    } else {
        index = aline - 0xA;
        value = fldInfTable.sets[gFldInfIdx].messages[index].targets[0];
    }
    switch (value) {
    case 0x64:
        slot = gFldInf_HitIdx;
        fldInf_FindTypeEveNpc(gEvnRng[slot]->name);
        if (fldInfTable.sets[gFldInfIdx].eventHit == 0) {
            return 0xC7;
        }
        slot = gFldInf_HitIdx;
        fldDoor_Find2(gEvnRng[slot]->name);
        switch (gFldWapPanel) {
        case 3:
            return 0x66;
        case 5:
            if (fldDoor_GetHasigoTbl(0) == 0) {
                return 0x68;
            }
            return 0x69;
        case 6:
            return 0x6A;
        case 7:
            return 0x6B;
        case 0:
        case 1:
        case 2:
        case 4:
        default:
            return 0x64;
        }
    case 0:
        return value;
    case 0x65:
        return value;
    case 0x66:
        return value;
    case 0x67:
        return value;
    case 0x68:
        return value;
    case 0x69:
        return value;
    }
    if (value < 0xA) {
        return fldInfChkFastPanel(value);
    }
    index = value - 0xA;
    if (fldInfTable.sets[gFldInfIdx].messages[index].kind == -1) {
        return fldInfChkFastPanel(aline);
    }
    if ((u16)fldInfTable.sets[gFldInfIdx].messages[index].kind >= 2) {
        return 0;
    }
    switch (fldInfTable.sets[gFldInfIdx].eventHit) {
    case 0:
        return 0xC7;
    case 1:
        return 2;
    case 2:
        return 0x64;
    case 3:
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "field/fldInf", fldInfChkFastPanel);
#endif

INCLUDE_ASM(const s32, "field/fldInf", fldInfNext);

s32 fldInfGet(s32 aline, s32 acol) {
    s32 message = aline - 10;
    s32 ret = 0;

    switch (acol) {
    case 0:
        ret = fldInfTable.sets[gFldInfIdx].messages[message].kind;
        break;
    case 1:
        ret = fldInfTable.sets[gFldInfIdx].messages[message].message;
        break;
    }
    return ret;
}

void fldInfLoad(s32 field) {
    char path[64];
    char directory[32];
    devState_t *command;
    if (field < 200) {
        fldMakeFilePath(directory, field, 1);
        sprintf(path, D_00531E98, directory, field);
        command = sdfFileOpen(path);
        sdfFileRead(command, &fldInfTable, sizeof(fldInfTable));
        sdfFileClose(command);
    }
}

void fldInfLoadFromMem(const void *data) {
    memcpy(&fldInfTable, data, sizeof(fldInfTable));
}

void fldInfToWAP(void) {
    s32 slot;
    char *actor;

    gFldInf_WpExit = 1;
    slot = gFldInf_HitIdx;
    actor = (char *)fldDoor_Find2(gEvnRng[slot]->name);
    if (scrSearchScriptName(actor) != NULL) {
        return;
    }
    gEvnRngPid[slot] = dds3SceneBasicStartScript(dds3GetActiveScene(), actor);
}

/* The native format slot includes twelve trailing zero bytes. */
const char D_00531E98[24] = "%sF%03d.INF";

extern u8 D_00302FA0[];

extern u8 D_00302FB0[];

extern u8 D_00302FC0[];

extern u8 D_00302FD0[];

extern u8 D_00302FF0[];

extern u8 D_00303000[];

extern u8 D_00303010[];

extern u8 D_00303030[];

u8 *fldFindEventEntry(const char *name) {
    s32 i;
    fldActorEntry_t *entry;

    if (name == NULL) return NULL;
    for (i = 0; i < 256; i++) {
        entry = &fldDoorTable[i];
        if (entry->requiredFlag != 0 && evtBitCheck(entry->requiredFlag) == 0) continue;
        if (entry->floor != fldGlobalBase.floor + 1) continue;
        if (strcmp(name, entry->name) != 0) continue;
        switch (entry->kind) {
        case 1:
            if (fldGlobalBase.area < 10) {
                if (entry->variantMode == 0 && entry->variant != 0 && entry->variant < 11) {
                    return D_00303010;
                }
                return D_00303010;
            }
            if (entry->variantMode == 0 && entry->variant != 0 && entry->variant < 10) {
                return D_00303000;
            }
            return D_00302FA0;
        case 2:
            return D_00302FB0;
        case 3:
            return D_00302FC0;
        case 4:
            if (fldGlobalBase.area >= 40) {
                return D_00303030;
            }
            return D_00302FD0;
        case 5:
            return D_00302FF0;
        case 6:
        case 7:
        case 8:
        case 9:
            break;
        case 10:
            return D_00303020;
        }
    }
    return NULL;
}
