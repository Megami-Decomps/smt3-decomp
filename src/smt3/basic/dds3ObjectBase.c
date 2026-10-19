#include "common.h"
#include "basic.h"
#include "model.h"
#include "motion.h"
#include "event.h"
#include "field.h"
#include "ps2asm.h"

extern dds3ObjectBaseControlWorkData_t *dds3GetCameraObjectBaseControlWorkData(dds3Basic_t *basic);
extern dds3ObjectBaseControlWorkData_t *dds3EffectObjectBasicGetControlWorkData(dds3Basic_t *basic);
extern dds3ObjectBaseControlWorkData_t *dds3EventObjectBasicGetControlWorkData(dds3Basic_t *basic);
extern dds3ObjectBaseControlWorkData_t *dds3LightObjectBasicGetControlWorkData(dds3Basic_t *basic);
extern void dds3AreaBasicGetItemArray(dds3Basic_t *area, dds3Basic_t *basic);
extern void dds3DeleteSceneBasicData(dds3Basic_t *basic);
extern void sdfModelRootFree(sdfModelRootNode_t *model, s32 freeTextures, s32 freeMaterials);
extern void dds3FreeBasicArrayHandle(dds3BasicArrayHandle_t *array);
extern void sdfChipFreeDirect(void *ptr);
extern sdfModelRootNode_t *dds3ModelBasicMakeModel(dds3Basic_t *basic);
extern sdfMotionCtrlr_t *dds3ModelBasicSetMotion(dds3Basic_t *basic, sdfModelRootNode_t *model);
extern void dds3GetTMPoseSet(dds3Basic_t *basic);
extern void sdfModelEvalTm2(sdfModelRootNode_t *model, s32 mode);
extern void *sdfChipAllocDirect(u32 size);
extern dds3BasicArrayHandle_t *dds3GetBasicArrayHandle(s32 count);
extern s32 evtBitCheck(s32 flag);
extern void rsetOtOfs(s32 ofs);
extern void rsetAlpha(s32 mode);
extern void rsetFrameMaskRgbOn(void);
extern void rsetFrameMaskRgbOff(void);
extern void rsetTest(s32 bit0, s32 bit1, s32 bit4, s32 bit12, s32 bit14, s32 bit15, s32 unused,
                     s32 bit17);
extern void aDrawBoxFZ(s32 x, s32 y, s32 w, s32 h, u32 vertexTag, u32 gsWord0, u32 gsWord1,
                       u32 gsWord2, u32 gsWord3);
extern void mdlLink(void *model, sdfModelOt_t *ot);
extern sdfModelOt_t D_002F6A38;
extern s32 dds3ObjectBaseAddModelOt(s32 area, s32 floor, s32 bit);
extern void sdfMotionSeek(sdfMotionCtrlr_t *motion, f32 frame);
extern void sdfMotionNext(sdfMotionCtrlr_t *motion);
extern void sdfModelEvalTm(sdfModelRootNode_t *model);
extern void sdfModelLinkOt(sdfModelOt_t *ot, sdfModelRootNode_t *model);

typedef struct
{
    char name[20];
    char parts[17][20];
} dds3UnitPartTable_t;

extern const dds3UnitPartTable_t D_0052F850;
extern const dds3UnitPartTable_t D_0052F9B8;
extern const dds3UnitPartTable_t D_0052FB20;
extern const dds3UnitPartTable_t D_0052FC88;
extern const dds3UnitPartTable_t D_0052FDF0;
extern const dds3UnitPartTable_t D_0052FF58;
extern const dds3UnitPartTable_t D_005300C0;
extern const dds3UnitPartTable_t D_00530228;
extern const dds3UnitPartTable_t D_00530390;
extern const dds3UnitPartTable_t D_005304F8;
extern const dds3UnitPartTable_t D_00530660;
extern const dds3UnitPartTable_t D_005307C8;
extern const dds3UnitPartTable_t D_00530930;
extern const dds3UnitPartTable_t D_00530A98;
extern char D_0054E9D0[];
extern char D_0054E9D8[];
extern char D_0054E9E0[];
extern char D_0054E9E8[];
extern char D_0054E9F0[];
extern char D_0054E9F8[];
extern mdlObj_t *mdlLoad(s32 major, s32 minor);
extern void mdlGetScale(mdlObj_t *obj);
extern void mdlSetRotate(mdlObj_t *obj);
extern void mdlSetPosition(mdlObj_t *obj);
extern void dds3SetSclPoseSet(dds3Basic_t *basic, const sdfVector4_t *scale);
extern void Normalize(void);
extern struct evtUnit_t *evtCreateUnitHandle(dds3Basic_t *basic, mdlObj_t *obj);
extern s32 sdfModelGetNodeIndexByName(sdfModelRootNode_t *model, const char *name);
extern char *sdfModelGetNodeName(sdfModelRootNode_t *model, s32 index);
extern s32 strcmp(const char *a, const char *b);
extern s32 printf(const char *fmt, ...);

typedef struct
{
    dds3ObjectBaseControlWorkData_t *control;
    s32 hojiID;
} dds3ObjectBaseMatterWork_t;

void dds3ObjectBaseFreeModelHandle(dds3Basic_t *basic);
void *dds3ObjectBaseGetUserValue(dds3Basic_t *basic);
s32 dds3ObjectBaseSetPrimaryControl(dds3Basic_t *basic, dds3BasicControlProc_t func);
extern void mdlFree(void *model);
extern void evtDeleteUnitHandle(void *handle);
extern void sdfMotionCtrlrFree(sdfMotionCtrlr_t *motion);

u32 BasicTypeToItemType(u32 type);
void dds3AreaBasicRegistItem(dds3Basic_t *area, dds3Basic_t *basic);
dds3Basic_t *dds3AddMoverBasic(dds3Basic_t *basic);
void dds3MoverBasicSetCallBackPrimaryControl(dds3Basic_t *mover, dds3BasicControlProc_t func);
void dds3MoverBasicSetPath(dds3Basic_t *mover, dds3Basic_t *path);
void dds3MoverBasicMakePath(dds3Basic_t *basic);
void dds3MoverBasicFreePath(dds3Basic_t *mover);
dds3PathBasicControl_t *dds3MoverBasicGetPathControl(dds3Basic_t *mover);

dds3Basic_t *dds3ObjectBaseGetControlBasic(dds3Basic_t *basic, s32 type);
dds3Basic_t *dds3ObjectBaseSetControlBasic(dds3Basic_t *basic, dds3Basic_t *control);
dds3Basic_t *dds3ObjectBaseSetControlBasicType(dds3Basic_t *basic, dds3Basic_t *control, s32 type);


dds3ObjectBaseControlWorkData_t *dds3ObjectBaseInitControlWorkData(dds3Basic_t *basic)
{
    dds3ObjectBaseControlWorkData_t *control;
    s32 index;

    control = sdfChipAllocDirect(0xAC);
    memset(control, 0, 0xAC);
    control->modelType = 3;
    control->eventArray = dds3GetBasicArrayHandle(0);
    control->control[0] = basic;
    for (index = 0; index < 24; index++) {
        control->modelParts[index] = -1;
    }
    return control;
}

void dds3ObjectBaseFreeControlWorkData(dds3ObjectBaseControlWorkData_t *control)
{
    dds3Basic_t *owner;
    dds3Basic_t *area;
    s32 index;

    owner = control->control[0];
    area = dds3ObjectBaseGetControlBasic(owner, 5);
    if (area != NULL) {
        dds3AreaBasicGetItemArray(area, owner);
    }
    for (index = 0; index < 8; index++) {
        if (index < 3) {
            if (index > 0) {
                if (control->control[index] != NULL) {
                    dds3DeleteSceneBasicData(control->control[index]);
                }
            }
        }
    }
    dds3ObjectBaseFreeModelHandle(owner);
    if (control->modelRoot != NULL) {
        sdfModelRootFree(control->modelRoot, 1, 1);
    }
    dds3FreeBasicArrayHandle(control->eventArray);
    sdfChipFreeDirect(control);
}

void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 mask)
{
    dds3ObjectBaseControlWorkData_t *control;

    control = dds3GetBaseControlWorkData(basic);
    control->flag |= mask;
}

void dds3ObjectBaseOffFlag(dds3Basic_t *basic, s32 mask)
{
    dds3ObjectBaseControlWorkData_t *control;

    control = dds3GetBaseControlWorkData(basic);
    control->flag &= ~mask;
}

u8 dds3ObjectBaseCheckFlag(dds3Basic_t *basic, u32 mask)
{
    dds3ObjectBaseControlWorkData_t *control;

    control = dds3GetBaseControlWorkData(basic);
    return (control->flag & mask) != 0;
}

void dds3ObjectBaseSetUserValue(dds3Basic_t *basic, void *userValue)
{
    dds3ObjectBaseControlWorkData_t *control;

    control = dds3GetBaseControlWorkData(basic);
    dds3ObjectBaseGetUserValue(basic);
    control->userValue = userValue;
}

void *dds3ObjectBaseGetUserValue(dds3Basic_t *basic)
{
    return dds3GetBaseControlWorkData(basic)->userValue;
}

dds3Basic_t *dds3ObjectBaseSetControlBasic(dds3Basic_t *basic, dds3Basic_t *control)
{
    if (control == NULL) {
        return NULL;
    }
    return dds3ObjectBaseSetControlBasicType(basic, control,
                                             BasicTypeToItemType(control->type >> 24));
}

dds3Basic_t *dds3ObjectBaseSetControlBasicType(dds3Basic_t *basic, dds3Basic_t *item, s32 type)
{
    dds3Basic_t *previous;

    previous = dds3ObjectBaseGetControlBasic(basic, type);
    dds3GetBaseControlWorkData(basic)->control[type] = item;
    return previous;
}

dds3Basic_t *dds3ObjectBaseGetControlBasic(dds3Basic_t *basic, s32 type)
{
    return dds3GetBaseControlWorkData(basic)->control[type];
}

dds3BasicArrayHandle_t *dds3ObjectBaseGetEventArray(dds3Basic_t *basic)
{
    return dds3GetBaseControlWorkData(basic)->eventArray;
}

void *dds3ObjectBaseGetModelHandle(dds3Basic_t *basic)
{
    return dds3GetBaseControlWorkData(basic)->modelHandle;
}

/* modelType selects the model-context or raw model/motion ownership path. */
void dds3ObjectBaseFreeModelHandle(dds3Basic_t *basic)
{
    dds3ObjectBaseControlWorkData_t *control;
    dds3UnitObjectBasicWorkData_t *work;

    control = dds3GetBaseControlWorkData(basic);
    if (control->modelHandle != NULL) {
        if (control->modelType != 1) {
            if (control->modelType == 0) {
                mdlFree(control->modelHandle);
                work = basic->workdata;
                if (work->unit != NULL) {
                    evtDeleteUnitHandle(work->unit);
                    work->unit = NULL;
                }
            }
        } else {
            sdfModelRootFree(control->modelHandle, 1, 1);
            sdfMotionCtrlrFree(control->motion);
        }
        control->modelHandle = NULL;
        control->modelType = 3;
    }
}

/* The loaded model is identified by its root node name, and the matching table
 * supplies the 17 part names whose node indices the control keeps. */
INCLUDE_RODATA(const s32, "basic/dds3ObjectBase", D_0052F850);

INCLUDE_RODATA(const s32, "basic/dds3ObjectBase", D_0052F9B8);

INCLUDE_RODATA(const s32, "basic/dds3ObjectBase", D_0052FB20);

INCLUDE_RODATA(const s32, "basic/dds3ObjectBase", D_0052FC88);

INCLUDE_RODATA(const s32, "basic/dds3ObjectBase", D_0052FDF0);

INCLUDE_RODATA(const s32, "basic/dds3ObjectBase", D_0052FF58);

INCLUDE_RODATA(const s32, "basic/dds3ObjectBase", D_00530000);

INCLUDE_RODATA(const s32, "basic/dds3ObjectBase", D_0053001C);

INCLUDE_RODATA(const s32, "basic/dds3ObjectBase", D_00530020);

INCLUDE_RODATA(const s32, "basic/dds3ObjectBase", D_00530040);

INCLUDE_RODATA(const s32, "basic/dds3ObjectBase", D_00530054);

INCLUDE_RODATA(const s32, "basic/dds3ObjectBase", D_00530068);

INCLUDE_RODATA(const s32, "basic/dds3ObjectBase", D_005300C0);

INCLUDE_RODATA(const s32, "basic/dds3ObjectBase", D_00530228);

INCLUDE_RODATA(const s32, "basic/dds3ObjectBase", D_00530390);

INCLUDE_RODATA(const s32, "basic/dds3ObjectBase", D_005304F8);

INCLUDE_RODATA(const s32, "basic/dds3ObjectBase", D_00530660);

INCLUDE_RODATA(const s32, "basic/dds3ObjectBase", D_005307C8);

INCLUDE_RODATA(const s32, "basic/dds3ObjectBase", D_00530930);

INCLUDE_RODATA(const s32, "basic/dds3ObjectBase", D_00530A98);

void dds3ObjectBaseMakeUnit(dds3Basic_t *basic, s32 major, s32 minor)
{
    sdfVector4_t scale;
    dds3UnitPartTable_t hijriParts;
    dds3UnitPartTable_t hijBParts;
    dds3UnitPartTable_t playerAParts;
    dds3UnitPartTable_t playerBParts;
    dds3UnitPartTable_t tiakiParts;
    dds3UnitPartTable_t yuukoParts;
    dds3UnitPartTable_t isamuParts;
    dds3UnitPartTable_t isamuFParts;
    dds3UnitPartTable_t isamuFzParts;
    dds3UnitPartTable_t hikawaParts;
    dds3UnitPartTable_t model113Parts;
    dds3UnitPartTable_t tiakiBParts;
    dds3UnitPartTable_t majyoParts;
    dds3UnitPartTable_t hutomimiParts;
    dds3ObjectBaseControlWorkData_t *control;
    dds3UnitObjectBasicWorkData_t *work;
    mdlObj_t *obj;
    char *nodeName;
    s32 index;
    s32 count;
    s32 i;

    control = dds3GetBaseControlWorkData(basic);
    if (control->modelHandle != NULL) {
        mdlFree(control->modelHandle);
    }
    obj = mdlLoad(major, minor);
    hijriParts = D_0052F850;
    hijBParts = D_0052F9B8;
    playerAParts = D_0052FB20;
    playerBParts = D_0052FC88;
    tiakiParts = D_0052FDF0;
    yuukoParts = D_0052FF58;
    isamuParts = D_005300C0;
    isamuFParts = D_00530228;
    isamuFzParts = D_00530390;
    hikawaParts = D_005304F8;
    model113Parts = D_00530660;
    tiakiBParts = D_005307C8;
    majyoParts = D_00530930;
    hutomimiParts = D_00530A98;
    nodeName = sdfModelGetNodeName(obj->inner, 0);
    if (sdfModelGetNodeIndexByName(obj->inner, D_0054E9D0) > 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, hijBParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 4;
                control->modelParts[i] = index;
            }
        }
    } else if (sdfModelGetNodeIndexByName(obj->inner, "isam_f_b") > 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, isamuFParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 4;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, hutomimiParts.name) == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, hutomimiParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 4;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, majyoParts.name) == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, majyoParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 4;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, model113Parts.name) == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, model113Parts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 4;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, hikawaParts.name) == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, hikawaParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 4;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, "hikawa_j") == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, hikawaParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 4;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, isamuParts.name) == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, isamuParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 4;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, D_0054E9D8) == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, isamuParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 4;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, isamuFzParts.name) == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, isamuFzParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 4;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, yuukoParts.name) == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, yuukoParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 4;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, D_0054E9E0) == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, yuukoParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 4;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, D_0054E9E8) == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, yuukoParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 4;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, tiakiParts.name) == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, tiakiParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 3;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, D_0054E9F0) == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, tiakiParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 3;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, D_0054E9F8) == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, tiakiParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 3;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, tiakiBParts.name) == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, tiakiBParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 3;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, hijriParts.name) == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, hijriParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 2;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, playerAParts.name) == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, playerAParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 0;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, "player_j") == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, playerAParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 0;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, "player_aj") == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, playerAParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 0;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, "player84") == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, playerAParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 0;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, "player86") == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, playerAParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 0;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, "player88") == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, playerAParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 0;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, playerBParts.name) == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, playerBParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 1;
                control->modelParts[i] = index;
            }
        }
    } else if (strcmp(nodeName, "player_z") == 0) {
        for (i = 0; i < 17; i++) {
            index = sdfModelGetNodeIndexByName(obj->inner, playerBParts.parts[i]);
            if (index > 0) {
                control->modelPartCount = 1;
                control->modelParts[i] = index;
            }
        }
    } else if (major == 3 && minor == 1) {
        count = obj->inner->nodeList->count;
        for (i = 0; i < count; i++) {
            if (i == 0) {
                printf("%02d:pName:[%s]\n", i, sdfModelGetNodeName(obj->inner, i));
            }
        }
    }
    mdlGetScale(obj);
    SDF_VU0_STOREVECTOR(&scale);
    dds3SetSclPoseSet(basic, &scale);
    SDF_VU0_LOADVECTOR(&basic->poseset->rot);
    Normalize();
    mdlSetRotate(obj);
    SDF_VU0_LOADVECTOR(&basic->poseset->pos);
    SDF_VU0_VECTOR_W_SET_ONE();
    mdlSetPosition(obj);
    basic->poseset->r = obj->objectRadius;
    control->modelHandle = obj;
    work = basic->workdata;
    control->modelType = 0;
    control->controlSvs = 0;
    if (work->unit == NULL) {
        work->unit = evtCreateUnitHandle(basic, obj);
    }
}

void dds3ObjectBaseMakeMatter(dds3Basic_t *basic)
{
    dds3ObjectBaseControlWorkData_t *control;
    dds3Basic_t *modelBasic;
    sdfMotionCtrlr_t *motion;
    sdfModelRootNode_t *model;

    control = dds3GetBaseControlWorkData(basic);
    modelBasic = dds3ObjectBaseGetControlBasic(basic, 3);
    model = dds3ModelBasicMakeModel(modelBasic);
    motion = dds3ModelBasicSetMotion(modelBasic, model);
    dds3GetTMPoseSet(basic);
    SDF_VU0_STOREMATRIX(&model->tm);
    sdfModelEvalTm2(model, 0);
    sdfModelEvalTm2(model, 1);
    control->modelType = 1;
    control->modelHandle = model;
    control->motion = motion;
    control->animeSpeed = 1.0f;
    control->animeMode = 0;
    control->controlSvs = 0;
}

/* Modes 0, 4 and 5 play at unit speed; the remaining modes start stopped. */
void dds3ObjectBaseMatterAnimeSpeed(dds3Basic_t *basic, u32 mode)
{
    dds3ObjectBaseControlWorkData_t *control = dds3GetBaseControlWorkData(basic);

    switch (mode) {
    case 0:
        control->animeMode = 0;
        control->animeSpeed = 1.0f;
        break;
    case 1:
        control->animeMode = 1;
        control->animeSpeed = 0.0f;
        break;
    case 2:
        control->animeMode = 2;
        control->animeSpeed = 0.0f;
        break;
    case 3:
        control->animeMode = 3;
        control->animeSpeed = 0.0f;
        break;
    case 4:
        control->animeMode = 4;
        control->animeSpeed = 1.0f;
        break;
    case 5:
        control->animeMode = 5;
        control->animeSpeed = 1.0f;
        break;
    case 6:
        control->animeMode = 6;
        control->animeSpeed = 0.0f;
        break;
    }
}

s32 dds3ObjectBaseRegistArea(dds3Basic_t *basic)
{
    dds3Basic_t *area;

    area = dds3ObjectBaseGetControlBasic(basic, 5);
    if (area == NULL) {
        return 0;
    }
    dds3AreaBasicRegistItem(area, basic);
    return 1;
}

/* The save-screen variant: the model is drawn between two full-screen boxes, the
 * second only while event bit 0xAF0 is clear. */
void dds3ObjectBaseModelLinkOtSvs(sdfModelOt_t *ot, dds3Basic_t *basic)
{
    dds3ObjectBaseControlWorkData_t *control;
    s32 tag = 0xEFFFFFF;

    control = dds3GetBaseControlWorkData(basic);
    if (evtBitCheck(0xAF0) != 0) {
        tag = 0;
    }
    rsetOtOfs(0x3F);
    rsetAlpha(0);
    rsetFrameMaskRgbOn();
    rsetTest(1, 1, 0x80, 2, 0, 0, 1, 1);
    aDrawBoxFZ(0, 0, 0x200, 0xE0, 0xFFFFFFF, 0, 0, 0, 0x80);
    rsetTest(1, 1, 0x80, 2, 0, 0, 1, 1);
    rsetOtOfs(0x40);
    rsetTest(1, 1, 0x80, 2, 0, 0, 1, 1);
    rsetFrameMaskRgbOn();
    rsetOtOfs(0x41);
    rsetTest(1, 1, 0x80, 2, 0, 0, 1, 1);
    rsetFrameMaskRgbOn();
    mdlLink(control->modelHandle, &D_002F6A38);
    rsetOtOfs(0x43);
    rsetFrameMaskRgbOff();
    rsetAlpha(0);
    if (tag != 0) {
        rsetTest(1, 0, 0x80, 1, 0, 0, 1, 2);
        aDrawBoxFZ(0, 0, 0x200, 0xE0, 0xEFFFFFF, 0x70, 0x66, 0x58, 0x30);
    }
    rsetTest(1, 5, 0x80, 1, 0, 0, 1, 2);
}

/* The matter path ramps animeSpeed up or down depending on whether the block's
 * anime flag is set, then seeks to the last frame in mode 6. */
void dds3ObjectBaseModelLinkOt(sdfModelOt_t *ot, dds3Basic_t *basic)
{
    dds3ObjectBaseControlWorkData_t *control;
    void *light;
    s32 hojiID;

    control = dds3GetBaseControlWorkData(basic);
    if (dds3ObjectBaseCheckFlag(basic, 1) != 0) {
        return;
    }
    if (control->modelHandle == NULL) {
        return;
    }
    if (control->modelType != 1) {
        if (control->modelType != 0) {
            return;
        }
        if (dds3ObjectBaseCheckFlag(basic, 0x800) != 0) {
            dds3ObjectBaseModelLinkOtSvs(ot, basic);
            return;
        }
        light = (void *)((dds3UnitObjectBasicWorkData_t *)basic->workdata)->unit->value;
        if (light != NULL) {
            *(void **)&((mdlObj_t *)control->modelHandle)->inner->light = light;
        } else {
            *(void **)&((mdlObj_t *)control->modelHandle)->inner->light = NULL;
        }
        mdlLink(control->modelHandle, ot);
        return;
    }
    if ((basic->color >> 24) == 0) {
        return;
    }
    if (control->motion != NULL) {
        hojiID = ((dds3ObjectBaseMatterWork_t *)basic->workdata)->hojiID;
        if (hojiID > 0) {
            if (dds3ObjectBaseAddModelOt(fldGlobalBase.area, fldGlobalBase.floor + 1, hojiID) ==
                0) {
                switch (control->animeMode) {
                case 0:
                    control->animeSpeed = 1.0f;
                    break;
                case 1:
                    control->animeSpeed += 0.05f;
                    if (1.0f < control->animeSpeed) {
                        control->animeSpeed = 1.0f;
                    }
                    break;
                case 2:
                    control->animeSpeed += 0.025f;
                    if (1.0f < control->animeSpeed) {
                        control->animeSpeed = 1.0f;
                    }
                    break;
                case 3:
                    hojiID = 0;
                    break;
                case 4:
                    hojiID = 0;
                    break;
                case 5:
                    hojiID = 0;
                    break;
                case 6:
                    hojiID = 0;
                    break;
                }
                if (control->animeMode == 6) {
                    sdfMotionSeek(control->motion, (f32)control->motion->frameLength);
                } else if (0.0f < control->animeSpeed) {
                    control->motion->speed = control->animeSpeed;
                    sdfMotionNext(control->motion);
                }
                sdfModelEvalTm(control->modelHandle);
            } else {
                switch (control->animeMode) {
                case 0:
                case 1:
                case 2:
                    control->animeSpeed = 0.0f;
                    control->animeMode = 3;
                    break;
                case 3:
                    control->animeSpeed = 0.0f;
                    break;
                case 4:
                    control->animeSpeed -= 0.05f;
                    if (control->animeSpeed < 0.0f) {
                        control->animeSpeed = 0.0f;
                    }
                    break;
                case 5:
                    control->animeSpeed -= 0.02f;
                    if (control->animeSpeed < 0.0f) {
                        control->animeSpeed = 0.0f;
                    }
                    break;
                case 6:
                    hojiID = 0;
                    break;
                }
                if (control->animeMode == 6) {
                    sdfMotionSeek(control->motion, (f32)control->motion->frameLength);
                } else if (0.0f < control->animeSpeed) {
                    control->motion->speed = control->animeSpeed;
                    sdfMotionNext(control->motion);
                }
                sdfModelEvalTm(control->modelHandle);
            }
        } else {
            u32 mode = control->animeMode;
            switch (mode) {
            case 0:
                control->animeSpeed = 1.0f;
                break;
            case 1:
                control->animeSpeed += 0.05f;
                if (1.0f < control->animeSpeed) {
                    control->animeSpeed = 1.0f;
                }
                break;
            case 2:
                control->animeSpeed += 0.025f;
                if (1.0f < control->animeSpeed) {
                    control->animeSpeed = 1.0f;
                }
                break;
            case 3:
                control->animeSpeed = 0.0f;
                break;
            case 4:
                control->animeSpeed -= 0.05f;
                if (control->animeSpeed < 0.0f) {
                    control->animeSpeed = 0.0f;
                }
                /* fallthrough */
            case 5:
                control->animeSpeed -= 0.02f;
                if (control->animeSpeed < 0.0f) {
                    control->animeSpeed = 0.0f;
                }
                break;
            }
            if (0.0f < control->animeSpeed) {
                control->motion->speed = control->animeSpeed;
                sdfMotionNext(control->motion);
            }
            sdfModelEvalTm(control->modelHandle);
        }
    }
    ((sdfModelRootNode_t *)control->modelHandle)->color = basic->color;
    sdfModelLinkOt(ot, control->modelHandle);
    ot = NULL;
}

void dds3ObjectBaseCreateMover(dds3Basic_t *basic)
{
    if (dds3ObjectBaseGetControlBasic(basic, 1) == NULL) {
        dds3ObjectBaseSetControlBasic(basic, dds3AddMoverBasic(basic));
    }
}

void dds3ObjectBaseSetPadControl(dds3Basic_t *basic, dds3BasicControlProc_t func)
{
    dds3ObjectBaseSetPrimaryControl(basic, func);
    /* Retail dds3ObjectBaseSetPadControl and DDS1 001127A8 keep JAL plus epilogue.
     * DDS cc1probe: this consumed-callback reset is removed in 03.cse,
     * after the sibling-call decision (01.sibling / 02.jump). */
    func = NULL;
}

s32 dds3ObjectBaseSetPrimaryControl(dds3Basic_t *basic, dds3BasicControlProc_t func)
{
    dds3Basic_t *mover;

    mover = dds3ObjectBaseGetControlBasic(basic, 1);
    if (mover == NULL) {
        return 0;
    }
    dds3MoverBasicSetCallBackPrimaryControl(mover, func);
    return 1;
}

void dds3ObjectBaseMakePath(dds3Basic_t *basic, dds3Basic_t *path)
{
    dds3MoverBasicSetPath(dds3ObjectBaseGetControlBasic(basic, 1), path);
    dds3MoverBasicMakePath(basic);
}

void dds3ObjectBaseFreePath(dds3Basic_t *basic)
{
    dds3MoverBasicFreePath(dds3ObjectBaseGetControlBasic(basic, 1));
}

dds3PathBasicControl_t *dds3ObjectBaseGetPathControl(dds3Basic_t *basic)
{
    return dds3MoverBasicGetPathControl(dds3ObjectBaseGetControlBasic(basic, 1));
}

dds3ObjectBaseControlWorkData_t *dds3GetBaseControlWorkData(dds3Basic_t *basic)
{
    dds3ObjectBaseControlWorkData_t *control;

    control = NULL;
    switch ((basic->type >> 24) - BASIC_TYPE_CAMERAOBJECT) {
    case 0: control = dds3GetCameraObjectBaseControlWorkData(basic); break;
    case 1: control = dds3UnitObjectBasicGetControlWorkData(basic); break;
    case 2: control = dds3MatterObjectBasicGetControlWorkData(basic); break;
    case 3: control = dds3EffectObjectBasicGetControlWorkData(basic); break;
    case 4: control = dds3EventObjectBasicGetControlWorkData(basic); break;
    case 5: control = dds3LightObjectBasicGetControlWorkData(basic); break;
    }
    return control;
}

void dds3ObjectBaseSetControlBasicSvs(dds3Basic_t *basic, u32 value)
{
    dds3ObjectBaseControlWorkData_t *control;

    control = dds3GetBaseControlWorkData(basic);
    control->controlSvs = value;
}

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", D_0054E9D0);

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", D_0054E9D8);

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", D_0054E9E0);

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", D_0054E9E8);

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", D_0054E9F0);

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", D_0054E9F8);

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", frMessageCode);

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", D_0054EA02);

