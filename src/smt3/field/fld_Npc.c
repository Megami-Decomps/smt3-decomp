#include "common.h"
#include "basic.h"
#include "model.h"
#include "motion.h"
#include "newdata.h"
#include "scr.h"
#include "event.h"
#include "field.h"
#include "ps2asm.h"

typedef struct
{
    dds3Basic_t *object;
    s32 id;
    s32 activationState;
    s32 activationRequested;
    u8 unk10[0xC];
    f32 angle;                 /* 0x1C */
    s32 mode;                  /* 0x20 */
    void *effect;              /* 0x24 */
    u8 unk28[8];
} fldTakaraSlot_t;

typedef struct
{
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
    u8 unk10[0x10];
    dds3Basic_t *object;       /* 0x20 */
    u8 unk24[0xC];
    void *effect;              /* 0x30 */
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 room;                  /* 0x48 */
    s32 unk4C;
    s32 kind;                  /* 0x50 */
    u8 unk54[8];
    s32 modelIndex;            /* 0x5C */
    u8 unk60[0x10];
} fldNpcSlot_t;

typedef struct
{
    u8 data[0x100];
} fldNpcPalette_t;

typedef struct fldUnit_t fldUnit_t;

enum sl_slot_flag
{
    SL_SLOT_FLAG_CAN_READ_SAVE = 1,
    SL_SLOT_FLAG_DIRECTORY_READY = 2,
    SL_SLOT_FLAG_DELETE_BEFORE_WRITE = 8
};

extern void *memset(void *a, s32 b, u32 c);

extern s32 fldTakaraCount;

extern fldTakaraSlot_t fldTakaraSlotTable[32];

extern s32 fldGb[];

extern void rsetOtOfs(u32);

extern void rsetAlpha(s32);

extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);

extern sdfTexHandle_t *fldUiTexture;

extern void DrwSprite2D(s32, s32, s32, s32, s32, s32, s32, s32, s32, sdfTexHandle_t *);

extern s32 fldNpcModelTable;

extern fldNpcSlot_t fldModelSlotTable[];

extern s32 fldGbChkNpc(s32 fld, u32 area, s32 hojiID);

extern void dds3ObjectBaseOnFlag(dds3Basic_t *object, u32 flags);

extern u8 D_003129D0[0x100];

extern void sdfTexFreeDelayed(sdfTexHandle_t *aHandle);

extern void *memcpy(void *, const void *, unsigned int);

extern s32 (*proc_func_ans)(dds3ProcessID_t *process);

extern void sdfMemFreeDelayedDirect(sdfMemHandle_t *aHandle);

extern s32 fldNpcModelFreeIdx;

extern dds3Basic_t *fldNpcFreeCount;

extern s32 fldNpcCamTarget;

extern void mdlUnRegist(s32 aMajor, s32 aMinor);

extern void dds3DeleteUnitObjectBasic(dds3Basic_t *aPtr);

extern s32 sprintf(char *dst, const char *format, ...);

extern void dds3EffectReset(void *hEffect);

extern void dds3EffectSetPosition(void *pData, const sdfVector4_t *pPos);

extern void *D_0054EEDC;

extern s32 evtBitCheck(s32 no);

extern void dds3EffectDelete(void *pData);

extern void fldMakeFilePath(char *pathstr, s32 field, s32 area);

extern char D_00532790[];

extern char D_005327A0[];

extern s32 sdfFileOpen(const char *aFileName);

extern void sdfFileRead(s32 aHandle, void *aPtr, s32 aLeng);

extern void sdfFileClose(s32 aHandle);

extern sdfMemHandle_t *fldNpcModelCount;

extern sdfMemHandle_t *D_0054EF1C;

extern sdfMemHandle_t *D_0054EF20;

extern sdfMemHandle_t *D_0054EF24;

extern sdfTexHandle_t *hEffPart;

extern void *fldItemBoxCount;

extern void dds3EffectPartDelete(void *hEffPart);

extern sdfDrawTag_t *sdfGrAllocPacketBuff(s32 aSize);

extern void sdfDrawTagInit(sdfDrawTag_t *a);

extern s32 sdf2DLinkTexPacket(sdfDrawTag_t *aTag, sdfTexHandle_t *aTexHndl, s32 aCtx);

extern void SetRotateMatrixY(f32 aAngle);

extern void sdfPrimLinkFt4(sdfDrawTag_t *aTag, u32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0,
                           s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aX2, s32 aY2, s32 aU2,
                           s32 aV2, s32 aX3, s32 aY3, s32 aU3, s32 aV3, s32 aZ, void *aAllocator);

extern sdfGrOt_t sdfGrOtTable[85];

extern s32 rsetFrameMaskOtOffset;

extern sdfTexHandle_t *fldVLTexture;

extern sdfTexHandle_t *fldVLBlurHandle;

extern f32 D_0054EF4C;

extern f32 D_0054EF60;

extern f32 EulerAngleAxisY(void);

extern void sdfModelSetFaceDivParm(sdfModelRootNode_t *aRoot, f32 aSoften, f32 aBias);

extern f32 sdfSin(f32 aRadius);

extern f32 sdfCos(f32 aRadius);

extern s32 fldArea[];

extern void dds3SetRotPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *rot);

extern void mdlSingleMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber);

extern s32 fldEncountTestFlag(s32 flag);

extern void *dds3EffectPartCreate(s32 type, void *pData);

extern void *D_0054EF2C;

extern const sdfVector4_t D_00532720;

extern mdlObj_t *dds3ObjectBaseGetModelHandle(dds3Basic_t *pObjectBasic);

extern void FromRotation(f32 aAngle);

extern void Mult(void);

extern void mdlLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber);

extern void mdlPackDecode(s32 major, s32 minor, s32 regFlags, const u8 *buff, s32 leng);

extern void mdlSetColor(mdlObj_t *aHandle, u32 aColor);

extern u32 dds3AllocObjectId(void);

extern dds3Basic_t *dds3AddUnitObjectBasic(s32 serialID, const sdfVector4_t *aPos, const sdfVector4_t *aRot);

extern void dds3UnitObjectBasicSetOt(dds3Basic_t *aPtr, s32 idx);

extern void dds3SetUnitObjectId(dds3Basic_t *basic, const char *name);

extern void dds3ObjectBaseMakeUnit(dds3Basic_t *pObjectBasic, s32 major, s32 minor);

extern evtUnit_t *evtGetUnitHandle2(void *unit);

extern void evtUnitLightOn(evtUnit_t *h);

extern void evtSetUnitLightDir(evtUnit_t *h, s32 frame);

extern void evtSetUnitLightAndAmbient(evtUnit_t *h, s32 frame, u32 lrgb, u32 argb);

extern s32 fldNpcDrawCount;

extern const u8 *D_00568E90[];

extern s32 D_00568F90[];

extern const char D_00532780[];

extern const sdfVector4_t D_00532770;

extern void *fldBlurEffectId[];

extern void dds3EffectSetLoop(void *hEffect, u8 loop);

extern u32 fldNpcBlurHandle;

extern void *D_0054EED0;

extern u32 fldDmgBlurHandle;


/* NON_MATCHING: 38 of 163 words differ (first at +0x4). */
#ifdef NON_MATCHING
void fldNpcLightSet(evtUnit_t *hEveUnit) {
    dds3Basic_t *object;
    mdlObj_t *model;
    u8 *row;
    f32 dir;
    f32 power;
    f32 rad;
    u32 light;
    u32 ambient;
    sdfVector4_t vec;

    object = hEveUnit->effObj;
    model = dds3ObjectBaseGetModelHandle(object);
    SDF_VU0_LOADVECTOR(&object->poseset->rot);
    row = &D_003129D0[(fldArea[0] + 1) * 8];
    dir = -EulerAngleAxisY();
    if (row[7] == 0) {
        dir = dir * 180.0f / 3.14f;
        dir = dir + 70.0f;
        power = 0.6f;
    } else if (row[7] == 1) {
        dir = dir * 180.0f / 3.14f;
        dir = dir - 70.0f;
        power = 0.6f;
    } else if (row[7] == 2) {
        dir = dir * 180.0f / 3.14f;
        dir = dir - 70.0f;
        power = -0.6f;
    } else if (row[7] == 3) {
        dir = dir * 180.0f / 3.14f;
        dir = dir + 70.0f;
        power = -0.6f;
    } else {
        dir = dir * 180.0f / 3.14f;
        dir = dir + 70.0f;
        power = 0.6f;
    }
    evtUnitLightOn(hEveUnit);
    dds3ObjectBaseOnFlag(object, 0x100);
    sdfModelSetFaceDivParm(model->inner, (f32)(row[6] >> 4), (f32)(row[6] & 0xF));
    rad = dir * 3.14f / 180.0f;
    vec.x = sdfSin(rad);
    vec.y = power;
    vec.z = sdfCos(rad);
    vec.w = 0.0f;
    SDF_VU0_LOADVECTOR(&vec);
    SDF_VU0_NORMALIZE();
    evtSetUnitLightDir(hEveUnit, 0);
    light = row[0] | (row[1] << 8) | (row[2] << 16) | 0x80000000;
    ambient = row[3] | (row[4] << 8) | (row[5] << 16) | 0x80000000;
    evtSetUnitLightAndAmbient(hEveUnit, 0, light, ambient);
    SDF_VU0_LOADVECTOR(&vec);
    SDF_VU0_NORMALIZE();
    evtSetUnitLightDir(hEveUnit, 0);
    evtSetUnitLightAndAmbient(hEveUnit, 0, light, ambient);
}
#else
INCLUDE_ASM(const s32, "field/fld_Npc", fldNpcLightSet);
#endif

INCLUDE_ASM(const s32, "field/fld_Npc", fldNpcLoad);

INCLUDE_RODATA(const s32, "field/fld_Npc", D_005326F0);

INCLUDE_RODATA(const s32, "field/fld_Npc", D_00532700);

INCLUDE_ASM(const s32, "field/fld_Npc", fldItemBoxAdd);

void fldFindTakaraSlot(s32 id) {
    s32 i;

    for (i = 0; i < fldTakaraCount; i++) {
        if (fldTakaraSlotTable[i].id == id && fldTakaraSlotTable[i].activationState == 0) {
            fldTakaraSlotTable[i].activationRequested = 1;
        }
    }
}

void fldFreeModelTable(void) {
    s32 i;

    if (fldNpcModelCount != NULL) {
        mdlUnRegist(2, 0x1F);
        sdfMemFreeDelayedDirect(fldNpcModelCount);
        fldNpcModelCount = NULL;
    }
    if (D_0054EF1C != NULL) {
        mdlUnRegist(2, 0x15);
        sdfMemFreeDelayedDirect(D_0054EF1C);
        D_0054EF1C = NULL;
    }
    if (D_0054EF20 != NULL) {
        mdlUnRegist(2, 0x16);
        sdfMemFreeDelayedDirect(D_0054EF20);
        D_0054EF20 = NULL;
    }
    if (D_0054EF24 != NULL) {
        mdlUnRegist(2, 0x17);
        sdfMemFreeDelayedDirect(D_0054EF24);
        D_0054EF24 = NULL;
    }
    for (i = 0; i < 32; i++) {
        if (fldTakaraSlotTable[i].effect != NULL) {
            dds3EffectDelete(fldTakaraSlotTable[i].effect);
            fldTakaraSlotTable[i].effect = NULL;
        }
    }
    if (fldItemBoxCount != NULL) {
        dds3EffectPartDelete(fldItemBoxCount);
        fldItemBoxCount = NULL;
    }
    if (hEffPart != NULL) {
        sdfTexFreeDelayed(hEffPart);
        hEffPart = NULL;
    }
    fldTakaraCount = 0;
    for (i = 0; i < 32; i++) {
        fldTakaraSlotTable[i].id = -1;
        fldTakaraSlotTable[i].object = NULL;
        fldTakaraSlotTable[i].activationState = 0;
        fldTakaraSlotTable[i].activationRequested = 0;
    }
}

void fldItemBoxCalcTask(void) {
    sdfVector4_t axis;
    sdfVector4_t rot;
    s32 i;
    mdlObj_t *model;

    axis = D_00532720;
    for (i = 0; i < fldTakaraCount; i++) {
        switch (fldTakaraSlotTable[i].mode) {
        case 0:
        case 2:
        case 3:
            SDF_VU0_LOADVECTOR(&axis);
            FromRotation(fldTakaraSlotTable[i].angle * 3.14f / 180.0f);
            SDF_VU0_STOREVECTOR_NOMEM(&rot);
            dds3SetRotPoseSet(fldTakaraSlotTable[i].object, &rot);
            fldTakaraSlotTable[i].angle = fldTakaraSlotTable[i].angle + 1.5f;
            break;
        case 1:
            break;
        }
        switch (fldTakaraSlotTable[i].activationState) {
        case 0:
            if (fldTakaraSlotTable[i].activationRequested == 1) {
                mdlSingleMotion(dds3ObjectBaseGetModelHandle(fldTakaraSlotTable[i].object), 0, 1);
                fldTakaraSlotTable[i].activationState = fldTakaraSlotTable[i].activationRequested;
                if (fldItemBoxCount == NULL) {
                    fldItemBoxCount = dds3EffectPartCreate(6, D_0054EF2C);
                }
                dds3EffectReset(D_0054EEDC);
            } else {
                model = dds3ObjectBaseGetModelHandle(fldTakaraSlotTable[i].object);
                if (model->first->stat == 5) {
                    mdlSingleMotion(model, 0, 0);
                }
            }
            break;
        case 1:
            model = dds3ObjectBaseGetModelHandle(fldTakaraSlotTable[i].object);
            if (model->first->stat == 5) {
                mdlLoopMotion(model, 0, 2);
                fldTakaraSlotTable[i].activationRequested = 2;
                fldTakaraSlotTable[i].activationState = 2;
            }
            break;
        case 2:
            if (fldEncountTestFlag(0x40) != 0) {
                mdlLoopMotion(dds3ObjectBaseGetModelHandle(fldTakaraSlotTable[i].object), 0, 3);
                fldTakaraSlotTable[i].activationRequested = 3;
                fldTakaraSlotTable[i].activationState = 3;
                if (fldItemBoxCount != NULL) {
                    dds3EffectPartDelete(fldItemBoxCount);
                    fldItemBoxCount = NULL;
                }
            }
            break;
        }
    }
}

void fldTDmgStart(f32 x, f32 y, f32 z) {
    sdfVector4_t position;

    memset(&position, 0, sizeof(position));
    position.x = x;
    position.y = y;
    position.z = z;
    position.w = 1.0f;
    dds3EffectReset(D_0054EEDC);
    dds3EffectSetPosition(D_0054EEDC, &position);
}

void fldDmgStart(f32 x, f32 y, f32 z) {
    sdfVector4_t position;

    memset(&position, 0, sizeof(position));
    position.w = 1.0f;
    if (fldGlobalBase.area == 0x2A) {
        if (fldGlobalBase.floor == 0xD) {
            return;
        }
        if (fldGlobalBase.floor == 0x19) {
            return;
        }
    }
    if (fldGlobalBase.area == 0x2C) {
        if (fldGlobalBase.floor == 2) {
            return;
        }
        if (fldGlobalBase.floor == 4) {
            return;
        }
        if (fldGlobalBase.floor == 0x18) {
            return;
        }
    }
    position.x = x;
    position.y = y;
    position.z = z;
    dds3EffectReset(D_0054EED0);
    dds3EffectSetPosition(D_0054EED0, &position);
    fldDmgBlurHandle = 1;
}

/* NON_MATCHING: 10 of 38 words differ (first at +0x8). */
#ifdef NON_MATCHING
void fldSibukiStart(f32 x, f32 y, f32 z) {
    sdfVector4_t position;

    memset(&position, 0, sizeof(position));
    position.w = 1.0f;
    position.x = x;
    position.y = y;
    position.z = z;
    dds3EffectReset(fldBlurEffectId[0]);
    dds3EffectSetPosition(fldBlurEffectId[0], &position);
    dds3EffectSetLoop(fldBlurEffectId[0], 0);
    fldNpcBlurHandle = 1;
}
#else
INCLUDE_ASM(const s32, "field/fld_Npc", fldSibukiStart);
#endif

/* NON_MATCHING: 65 of 208 words differ (first at +0x30). */
#ifdef NON_MATCHING
void fldDrawEATexQuad(s32 x, s32 y, s32 ax, s32 ay, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh,
                      f32 angle) {
    sdfVector4_t pos[4];
    sdfDrawTag_t *tag;
    sdfGrOt_t *descriptor;
    s32 left;
    s32 top;
    s32 su;
    s32 sv;
    s32 eu;
    s32 ev;
    f32 lf;
    f32 tf;
    f32 rf;
    f32 bf;
    f32 xf;
    f32 yf;

    tag = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(tag);
    sdf2DLinkTexPacket(tag, fldUiTexture, 0);
    left = -(ax * 16);
    top = -(ay * 16);
    su = u * 16;
    sv = v * 16;
    eu = su + uw * 16;
    ev = sv + vh * 16;
    lf = (f32)left;
    tf = (f32)top;
    rf = lf + (f32)(w * 16);
    bf = tf + (f32)(h * 16);
    pos[0].x = lf;
    pos[0].y = 0.0f;
    pos[0].z = tf;
    pos[1].x = rf;
    pos[1].y = 0.0f;
    pos[1].z = tf;
    pos[2].x = lf;
    pos[2].y = 0.0f;
    pos[2].z = bf;
    pos[3].x = rf;
    pos[3].y = 0.0f;
    pos[3].z = bf;
    SetRotateMatrixY((angle + 180.0f) * 3.14f / 180.0f);
    SDF_VU0_LOADVECTOR(&pos[0]);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR_NOMEM(&pos[0]);
    SDF_VU0_LOADVECTOR(&pos[1]);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR_NOMEM(&pos[1]);
    SDF_VU0_LOADVECTOR(&pos[2]);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR_NOMEM(&pos[2]);
    SDF_VU0_LOADVECTOR(&pos[3]);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR_NOMEM(&pos[3]);
    xf = (f32)x * 16.0f;
    yf = (f32)y * 8.0f;
    pos[0].x = pos[0].x + xf + 28672.0f;
    pos[0].z = pos[0].z * 0.5f + yf + 30976.0f;
    pos[1].x = pos[1].x + xf + 28672.0f;
    pos[1].z = pos[1].z * 0.5f + yf + 30976.0f;
    pos[2].x = pos[2].x + xf + 28672.0f;
    pos[2].z = pos[2].z * 0.5f + yf + 30976.0f;
    pos[3].x = pos[3].x + xf + 28672.0f;
    pos[3].z = pos[3].z * 0.5f + yf + 30976.0f;
    sdfPrimLinkFt4(tag, 0x80808080, 0x44,
                   (s32)pos[0].x, (s32)pos[0].z, su, sv,
                   (s32)pos[1].x, (s32)pos[1].z, eu, sv,
                   (s32)pos[2].x, (s32)pos[2].z, su, ev,
                   (s32)pos[3].x, (s32)pos[3].z, eu, ev,
                   -1, NULL);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, tag);
}
#else
INCLUDE_ASM(const s32, "field/fld_Npc", fldDrawEATexQuad);
#endif

void fldDrawAddressPanel(s32 width, s32 x, s32 y) {
    s32 ofs;
    s32 left;
    s32 right;
    s32 top;

    rsetOtOfs(0x47);
    rsetAlpha(0);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    ofs = 0x1BF - width;
    left = x + ofs;
    right = 0x210 - ofs;
    top = y + 0x12;
    DrwSprite2D(left, top, 0x5B, 0x1F, 1, 1, 0x5B, 0x1F, 0x80808080, fldUiTexture);
    DrwSprite2D(left + 0x5B, top, right, 0x1F, 0x5B, 1, 1, 0x1F, 0x80808080, fldUiTexture);
    rsetAlpha(0);
}

void fldDrawInputPanel(void) {
    if (fldGb[0] >= 0xC8) {
        return;
    }
    rsetOtOfs(0x47);
    rsetAlpha(0);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    DrwSprite2D(0x87, 0x124, 0x79, 0x18, 1, 0x5D, 0x79, 0x18, 0x80808080, fldUiTexture);
    DrwSprite2D(0x87, 0x13C, 0x79, 0x18, 1, 0x74, 0x79, -0x18, 0x80808080, fldUiTexture);
    DrwSprite2D(0x100, 0x124, 0x79, 0x18, 0x79, 0x5D, -0x79, 0x18, 0x80808080, fldUiTexture);
    DrwSprite2D(0x100, 0x13C, 0x79, 0x18, 0x79, 0x74, -0x79, -0x18, 0x80808080, fldUiTexture);
    rsetAlpha(0);
}

void fldDrawInputCursor(s32 x, s32 y) {
    if (fldGb[0] < 200) {
        rsetOtOfs(0x47);
        rsetAlpha(0);
        rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        DrwSprite2D(x, y, 0x14, 0x15, 0x2E, 0x47, 0x14, 0x15, 0x80808080, fldUiTexture);
        rsetAlpha(0);
    }
}

extern s32 gFldElvFloorNow;
extern s32 fldPlayerStatus[];
extern f32 fldEADrawState;
extern s32 fldEADrawWork;
extern s32 D_0054EF54;
extern s32 D_0054EF58;
extern sdfTexHandle_t *fldEATexture;
extern f32 fldCamPitch;
extern s32 fldChkVortex(void);
extern s32 nbGetEncountCnt(void);
extern s32 nbGetEncountMax(s32 id);
void fldDrawEATexQuad(s32 x, s32 y, s32 ax, s32 ay, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, f32 angle);

INCLUDE_RODATA(const s32, "field/fld_Npc", D_00532720);

void fldEADraw(void) {
    s32 offset;
    s32 alpha;
    s32 level;
    s32 encounterId;
    if (fldGlobalBase.area >= 200) {
        return;
    }
    if (gFldElvFloorNow != -999) {
        return;
    }
    switch (fldGlobalBase.word90) {
    case 0:
    case 4:
        D_0054EF4C = 0.0f;
        break;
    case 2:
    case 6:
        if (D_0054EF4C > 0.0f) {
            D_0054EF4C -= 0.2f;
            if (D_0054EF4C < 0.0f) {
                D_0054EF4C = 0.0f;
            }
        }
        break;
    case 3:
    case 7:
        if (D_0054EF4C < 1.0f) {
            D_0054EF4C += 0.2f;
            if (D_0054EF4C > 1.0f) {
                D_0054EF4C = 1.0f;
            }
        }
        break;
    case 1:
    case 5:
        D_0054EF4C = 1.0f;
        break;
    }
    /* Same real 110-to-zero HUD interpolation as the matched fldVLDraw. */
    offset = (s32)(0.0f * D_0054EF4C + 110.0f * (1.0f - D_0054EF4C));
    rsetOtOfs(0x47);
    rsetAlpha(0);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    if (fldGlobalBase.area != 1) {
        DrwSprite2D(offset + 0x199, offset + 0x156, 0x57, 0x5A, 0x5D, 1, 0x57, 0x5A, 0x80808080, fldUiTexture);
    }
    if (fldGlobalBase.area != 1 && (fldPlayerStatus[0] & 0x1D)) {
        DrwSprite2D(offset + 0x1DA, offset + 0x197, 0x14, 0x15, 0x19, 0x47, 0x14, 0x15, 0x80808080, fldUiTexture);
    }
    if (fldGlobalBase.area == 11 || fldGlobalBase.area == 14) {
        alpha = (s32)(sdfSin(fldEADrawState) * 32.0f) + 96;
        DrwSprite2D(offset + 0x1A4, offset + 0x162, 0x3D, 0x21, 0x7B, 0x5D, 0x3D, 0x21, (alpha << 24) | 0x808080, fldUiTexture);
        DrwSprite2D(offset + 0x1A4, offset + 0x183, 0x3D, 0x21, 0x7B, 0x7D, 0x3D, -0x21, (alpha << 24) | 0x808080, fldUiTexture);
        fldEADrawState += 0.1f;
    }
    encounterId = fldGlobalBase.wordC0;
    if (encounterId == 0) {
        encounterId = fldGlobalBase.wordBC;
    }
    if (fldChkVortex()) {
        s32 count = nbGetEncountCnt();
        s32 maximum = nbGetEncountMax(encounterId);
        if (count > maximum) {
            count = maximum;
        }
        if (maximum > 0) {
            s32 percentage = count * 100 / maximum;
            if (percentage < 34) {
                level = 0;
            } else {
                level = 1;
            }
            if (percentage >= 67) {
                level = 2;
            }
        } else {
            level = 4;
        }
        alpha = (s32)(sdfSin(fldEADrawState) * 64.0f) + 64;
        if (D_0054EF58 == 0) {
            if (alpha < D_0054EF54) {
                D_0054EF58 = 1;
            }
        } else if (alpha > D_0054EF54) {
            D_0054EF58 = 0;
            fldEADrawWork ^= 1;
        }
        D_0054EF54 = alpha;
        if (level == 4) {
            alpha = (s32)(sdfSin(fldEADrawState) * 32.0f) + 96;
            DrwSprite2D(offset + 0x1A4, offset + 0x162, 0x3D, 0x21, 0x7B, 0x5D, 0x3D, 0x21, (alpha << 24) | 0x808080, fldUiTexture);
            DrwSprite2D(offset + 0x1A4, offset + 0x183, 0x3D, 0x21, 0x7B, 0x7D, 0x3D, -0x21, (alpha << 24) | 0x808080, fldUiTexture);
            fldEADrawState += 0.1f;
        }
        if (level == 0) {
            if (fldEADrawWork == 0) {
                DrwSprite2D(offset + 0x1A4, offset + 0x162, 0x3D, 0x21, 1, 1, 0x3D, 0x21, (alpha << 24) | 0x808080, fldEATexture);
                DrwSprite2D(offset + 0x1A4, offset + 0x183, 0x3D, 0x21, 1, 33, 0x3D, -0x21, (alpha << 24) | 0x808080, fldEATexture);
            } else {
                DrwSprite2D(offset + 0x1A4, offset + 0x162, 0x3D, 0x21, 1, 35, 0x3D, 0x21, (alpha << 24) | 0x808080, fldEATexture);
                DrwSprite2D(offset + 0x1A4, offset + 0x183, 0x3D, 0x21, 1, 67, 0x3D, -0x21, (alpha << 24) | 0x808080, fldEATexture);
            }
            fldEADrawState += 0.1f;
        }
        if (level == 1) {
            if (fldEADrawWork == 0) {
                DrwSprite2D(offset + 0x1A4, offset + 0x162, 0x3D, 0x21, 63, 1, 0x3D, 0x21, (alpha << 24) | 0x808080, fldEATexture);
                DrwSprite2D(offset + 0x1A4, offset + 0x183, 0x3D, 0x21, 63, 33, 0x3D, -0x21, (alpha << 24) | 0x808080, fldEATexture);
            } else {
                DrwSprite2D(offset + 0x1A4, offset + 0x162, 0x3D, 0x21, 63, 35, 0x3D, 0x21, (alpha << 24) | 0x808080, fldEATexture);
                DrwSprite2D(offset + 0x1A4, offset + 0x183, 0x3D, 0x21, 63, 67, 0x3D, -0x21, (alpha << 24) | 0x808080, fldEATexture);
            }
            fldEADrawState += 0.12f;
        }
        if (level == 2) {
            if (fldEADrawWork == 0) {
                DrwSprite2D(offset + 0x1A4, offset + 0x162, 0x3D, 0x21, 1, 69, 0x3D, 0x21, (alpha << 24) | 0x808080, fldEATexture);
                DrwSprite2D(offset + 0x1A4, offset + 0x183, 0x3D, 0x21, 1, 101, 0x3D, -0x21, (alpha << 24) | 0x808080, fldEATexture);
            } else {
                DrwSprite2D(offset + 0x1A4, offset + 0x162, 0x3D, 0x21, 63, 69, 0x3D, 0x21, (alpha << 24) | 0x808080, fldEATexture);
                DrwSprite2D(offset + 0x1A4, offset + 0x183, 0x3D, 0x21, 63, 101, 0x3D, -0x21, (alpha << 24) | 0x808080, fldEATexture);
            }
            fldEADrawState += 0.15f;
        }
    }
    if (fldGlobalBase.area >= 11) {
        rsetAlpha(1);
        fldDrawEATexQuad(offset + 0x1C3, offset + 0x182, 0xC, 0x14, 0x17, 0x12, 1, 0x3B, 0x17, 0x12, -fldCamPitch);
        rsetAlpha(0);
        fldDrawEATexQuad(offset + 0x1C2, offset + 0x182, 5, 8, 0xB, 0xB, 0x19, 0x3B, 0xB, 0xB, fldGlobalBase.camAngle);
        DrwSprite2D(offset + 0x1B7, offset + 0x15A, 0x17, 0x10, 1, 0x22, 0x17, 0x10, 0x80808080, fldUiTexture);
        DrwSprite2D(offset + 0x1B7, offset + 0x19B, 0x17, 0x10, 0x19, 0x22, 0x17, 0x10, 0x80808080, fldUiTexture);
        DrwSprite2D(offset + 0x19B, offset + 0x177, 0x15, 0x17, 0x31, 0x22, 0x15, 0x17, 0x80808080, fldUiTexture);
        DrwSprite2D(offset + 0x1D5, offset + 0x177, 0x15, 0x17, 0x47, 0x22, 0x15, 0x17, 0x80808080, fldUiTexture);
    }
    rsetAlpha(0);
}


void fldVLDraw(void) {
    s32 x;
    s32 y;
    s32 alpha;
    f32 t;

    if (fldGlobalBase.area == 1) {
        return;
    }
    if (fldGlobalBase.area >= 11) {
        return;
    }
    t = D_0054EF4C;
    x = (s32)(0.0f * t + -110.0f * (1.0f - t));
    y = (s32)(0.0f * t + 110.0f * (1.0f - t));
    x -= 2;
    y += 3;
    rsetOtOfs(0x47);
    rsetAlpha(0);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    DrwSprite2D(x + 9, y + 0x15C, 0x5B, 0x5C, 0xA4, 1, 0x5B, 0x5C, 0x80808080, fldVLTexture);
    D_0054EF60 = D_0054EF60 + 0.15f;
    alpha = (s32)(sdfSin(D_0054EF60) * 50.0f + 50.0f) * 80 / 100 + 0x30;
    if (alpha > 0x80) {
        alpha = 0x80;
    }
    switch (fldGlobalBase.area) {
    case 2: {
        s32 sx;
        s32 sy;

        rsetAlpha(1);
        sx = x + 0x29;
        sy = y + 0x168;
        DrwSprite2D(sx, sy, 0x26, 0x2D, 0x36, 0x24, 0x26, 0x2D, (alpha << 24) | 0x808080, fldVLTexture);
        rsetAlpha(0);
        DrwSprite2D(sx, sy, 0x26, 0x2D, 0x36, 0x24, 0x26, 0x2D, 0x80808080, fldVLBlurHandle);
        break;
    }
    case 3: {
        s32 sx;
        s32 sy;

        rsetAlpha(1);
        sx = x + 0x15;
        sy = y + 0x169;
        DrwSprite2D(sx, sy, 0x1A, 0x3C, 0x18, 0x3B, 0x1A, 0x3C, (alpha << 24) | 0x808080, fldVLTexture);
        rsetAlpha(0);
        DrwSprite2D(sx, sy, 0x1A, 0x3C, 0x18, 0x3B, 0x1A, 0x3C, 0x80808080, fldVLBlurHandle);
        break;
    }
    case 4: {
        s32 sx;
        s32 sy;

        rsetAlpha(1);
        DrwSprite2D(x + 0x24, y + 0x17A, 0x1A, 0x1F, 0xE5, 0x5F, 0x1A, 0x1F, 0x80808080, fldVLTexture);
        sx = x + 0x16;
        sy = y + 0x175;
        DrwSprite2D(sx, sy, 0x32, 0x37, 2, 2, 0x32, 0x37, (alpha << 24) | 0x808080, fldVLTexture);
        rsetAlpha(0);
        DrwSprite2D(sx, sy, 0x32, 0x37, 2, 2, 0x32, 0x37, 0x80808080, fldVLBlurHandle);
        break;
    }
    case 5: {
        s32 sx;
        s32 sy;

        rsetAlpha(1);
        sx = x + 0x42;
        sy = y + 0x16D;
        DrwSprite2D(sx, sy, 0x14, 0x38, 2, 0x3B, 0x14, 0x38, (alpha << 24) | 0x808080, fldVLTexture);
        rsetAlpha(0);
        DrwSprite2D(sx, sy, 0x14, 0x38, 2, 0x3B, 0x14, 0x38, 0x80808080, fldVLBlurHandle);
        break;
    }
    case 6:
    case 7: {
        s32 sx;
        s32 sy;

        rsetAlpha(1);
        sx = x + 0x1A;
        sy = y + 0x161;
        DrwSprite2D(sx, sy, 0x37, 0x1C, 0x36, 0x53, 0x37, 0x1C, (alpha << 24) | 0x808080, fldVLTexture);
        rsetAlpha(0);
        DrwSprite2D(sx, sy, 0x37, 0x1C, 0x36, 0x53, 0x37, 0x1C, 0x80808080, fldVLBlurHandle);
        break;
    }
    case 8: {
        s32 sx;
        s32 sy;

        rsetAlpha(1);
        sx = x + 0x20;
        sy = y + 0x18B;
        DrwSprite2D(sx, sy, 0x28, 0x20, 0x36, 2, 0x28, 0x20, (alpha << 24) | 0x808080, fldVLTexture);
        rsetAlpha(0);
        DrwSprite2D(sx, sy, 0x28, 0x20, 0x36, 2, 0x28, 0x20, 0x80808080, fldVLBlurHandle);
        break;
    }
    }
    if (fldGlobalBase.area != 4) {
        rsetAlpha(1);
        DrwSprite2D(x + 0x24, y + 0x17A, 0x1A, 0x1F, 0xE5, 0x5F, 0x1A, 0x1F, 0x80808080, fldVLTexture);
        rsetAlpha(0);
    }
    DrwSprite2D(x + 0x19, y + 0x16E, 0x39, 0x3A, 0x65, 1, 0x39, 0x3A, 0x80808080, fldVLTexture);
}

void fldNpcModelStart(s32 id, f32 x, f32 y, f32 z, const sdfVector4_t *rot) {
    sdfVector4_t position;
    sdfVector4_t rotation;
    sdfVector4_t axis;
    sdfVector4_t lightDir;
    mdlObj_t *model;
    evtUnit_t *unit;
    s32 major;
    s32 minor;

    major = 0;
    minor = 0;
    memset(&position, 0, sizeof(position));
    position.w = 1.0f;
    memset(&rotation, 0, sizeof(rotation));
    rotation.w = 1.0f;
    axis = D_00532770;
    if (fldNpcFreeCount != NULL) {
        return;
    }
    position.x = x;
    position.y = y;
    position.z = z;
    position.w = 1.0f;
    fldNpcModelFreeIdx = id;
    switch (fldModelSlotTable[id].kind) {
    case 1:
        return;
    case 0:
        major = 2;
        minor = 0x22;
        mdlPackDecode(major, minor, 0x101, D_00568E90[fldModelSlotTable[id].modelIndex],
                      D_00568F90[fldModelSlotTable[id].modelIndex]);
        SDF_VU0_LOADVECTOR(&axis);
        FromRotation(3.1415926f);
        SDF_VU0_LOADVECTOR_B(rot);
        Mult();
        SDF_VU0_STOREVECTOR(&rotation);
        break;
    }
    fldNpcFreeCount = dds3AddUnitObjectBasic(dds3AllocObjectId(), &position, &rotation);
    dds3UnitObjectBasicSetOt(fldNpcFreeCount, 8);
    dds3SetUnitObjectId(fldNpcFreeCount, D_00532780);
    dds3ObjectBaseOnFlag(fldNpcFreeCount, 0x100);
    dds3ObjectBaseMakeUnit(fldNpcFreeCount, major, minor);
    model = dds3ObjectBaseGetModelHandle(fldNpcFreeCount);
    model->first->speed = 1.0f;
    mdlLoopMotion(model, 0, 0);
    mdlSetColor(model, 0x808080);
    fldNpcDrawCount = 0;
    unit = evtGetUnitHandle2(fldNpcFreeCount);
    evtUnitLightOn(unit);
    evtSetUnitLightAndAmbient(unit, 0, 0x80666666, 0x80737373);
    lightDir.x = 0.0f;
    lightDir.y = 0.1f;
    lightDir.z = 0.1f;
    lightDir.w = 0.0f;
    SDF_VU0_LOADVECTOR(&lightDir);
    evtSetUnitLightDir(unit, 0);
}

void fldDeleteNpcModel(s32 index) {
    if (fldNpcModelFreeIdx == index && fldNpcFreeCount != NULL) {
        dds3DeleteUnitObjectBasic(fldNpcFreeCount);
        mdlUnRegist(2, 10);
        fldNpcFreeCount = NULL;
        fldNpcCamTarget = 0;
        fldNpcModelFreeIdx = -1;
    }
}

void fldFreeNpcModel(void) {
    if (fldNpcModelFreeIdx != -1 && fldNpcFreeCount != NULL) {
        mdlUnRegist(2, 10);
        fldNpcFreeCount = NULL;
        fldNpcModelFreeIdx = -1;
        fldNpcCamTarget = 0;
    }
}

INCLUDE_ASM(const s32, "field/fld_Npc", fldNpcCalc);

INCLUDE_ASM(const s32, "field/fld_Npc", fldNpcDraw);

void fldNpcHojiRefresh(void) {
    s32 i;

    for (i = 0; i < fldNpcModelTable; i++) {
        s32 room = fldModelSlotTable[i].room;
        if (room != 0 && fldGbChkNpc(fldGlobalBase.area, fldGlobalBase.floor + 1, room) != 0) {
            if (fldModelSlotTable[i].object != 0) {
                dds3ObjectBaseOnFlag(fldModelSlotTable[i].object, 1);
            }
        }
    }
}

void fldNpcLightLoad(s32 field) {
    char path[0x40];
    char directory[0x20];
    s32 fd;

    if (field < 100) {
        fldMakeFilePath(directory, field, 1);
        if (field == 0x18 && evtBitCheck(0x4F) != 0) {
            sprintf(path, D_00532790, directory, 0x18);
        } else {
            sprintf(path, D_005327A0, directory, field);
        }
        fd = sdfFileOpen(path);
        sdfFileRead(fd, D_003129D0, 0x100);
        sdfFileClose(fd);
    }
}

void fldGetEAEntry(const fldNpcPalette_t *src) {
    memcpy(D_003129D0, src, sizeof(*src));
}

INCLUDE_RODATA(const s32, "field/fld_Npc", D_00532770);

INCLUDE_RODATA(const s32, "field/fld_Npc", D_00532780);

INCLUDE_RODATA(const s32, "field/fld_Npc", D_00532790);

INCLUDE_RODATA(const s32, "field/fld_Npc", D_005327A0);

