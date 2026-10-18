#include "common.h"
#include "basic.h"
#include "ps2asm.h"

typedef struct
{
    sdfVector4_t *sources[4];
    sdfVector4_t ambient;
} dds3SceneLightParam_t;

extern dds3SceneLightParam_t dds3LightParam;
extern void evtLinkLightObjectBasic(void *unit, dds3Basic_t *light, s32 frames);
extern void evtUnlinkLightObjectBasic(void *unit, s32 frames);


struct sdf3DLightParallelPacket_t;

typedef struct dds3LightObjectBasicWorkData_t
{
    sdf3DLight_t Light;
    sdfVector4_t Ambient;
    f32 InsideR;
    f32 OutsideR;
    f32 Soften;
    f32 Bias;
    u32 AnimeType;
    u32 Flag;
    u32 BaseLightRGB;
    u32 BaseAmbientRGB;
    u16 SwitchFrame;
    u16 Reserve1;
    dds3ObjectBaseControl_t *ObjectBaseControlWorkData;
    struct sdf3DLightParallelPacket_t *LightPacket;
} dds3LightObjectBasicWorkData_t;

typedef struct fldResourceLight_t
{
    u32 resv;
    u32 Flag;
    u32 AnimeType;
    f32 InsideR;
    f32 OutsideR;
    f32 Soften;
    f32 Bias;
    f32 DiffuseR;
    f32 DiffuseG;
    f32 DiffuseB;
    f32 AmbientR;
    f32 AmbientG;
    f32 AmbientB;
} fldResourceLight_t;


extern void *memset(void *s, s32 c, u32 n);
extern void *sdfChipAllocDirect(u32 size);
extern void sdfChipFreeDirect(void *ptr);
extern s32 dds3InitPoseSet(dds3Basic_t *basic);
extern void dds3FreePoseSet(dds3Basic_t *basic);
extern dds3Basic_t *dds3AddSceneBasicData(s32 type);
extern void dds3DeleteSceneBasicData(dds3Basic_t *basic);
extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 flag);
extern void dds3ObjectBaseCreateMover(dds3Basic_t *basic);

s32 lightconstructor(dds3Basic_t *basic)
{
    dds3LightObjectBasicWorkData_t *work;

    dds3InitPoseSet(basic);
    work = sdfChipAllocDirect(sizeof(dds3LightObjectBasicWorkData_t));
    basic->workdata = work;
    work->ObjectBaseControlWorkData = dds3ObjectBaseInitControlWorkData(basic);
    dds3ObjectBaseOnFlag(basic, 0x62);
    work->Flag = 0;

    work->Light.color.x = 0.7f;
    work->Light.color.y = 0.7f;
    work->Light.color.z = 0.7f;
    work->Light.color.w = 0.0f;
    work->Light.v.x = 0.0f;
    work->Light.v.y = 1.0f;
    work->Light.v.z = 0.5f;
    work->Light.v.w = 0.0f;
    work->Ambient.x = 0.2f;
    work->Ambient.y = 0.2f;
    work->Ambient.z = 0.2f;
    work->Ambient.w = 1.0f;
    work->InsideR = 100.0f;
    work->OutsideR = 800.0f;
    work->Soften = 7.0f;
    work->Bias = 0.0f;

    work->LightPacket = sdfChipAllocDirect(224);
    return 1;
}

void lightdestructor(dds3Basic_t *basic)
{
    dds3LightObjectBasicWorkData_t *work = basic->workdata;

    sdfChipFreeDirect(work->LightPacket);
    dds3ObjectBaseFreeControlWorkData(work->ObjectBaseControlWorkData);
    sdfChipFreeDirect(basic->workdata);
    basic->workdata = NULL;
    dds3FreePoseSet(basic);
}

/* The DDS C differs in the ambient subobject's global-address materialization. */
INCLUDE_ASM(const s32, "basic/dds3LightObjectBasic", lightmainprocedure);

s32 lightdrawprocedure(dds3Basic_t *basic)
{
    return 1;
}

dds3ObjectBaseControl_t *dds3LightObjectBasicGetControlWorkData(dds3Basic_t *basic)
{
    dds3LightObjectBasicWorkData_t *work = basic->workdata;

    return work->ObjectBaseControlWorkData;
}

dds3Basic_t *dds3AddLightObjectBasic(u32 serialID)
{
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_LIGHTOBJECT);

    basic->serialID = serialID;
    dds3ObjectBaseCreateMover(basic);
    return basic;
}

void dds3DeleteLightObjectBasic(dds3Basic_t *basic)
{
    dds3DeleteSceneBasicData(basic);
}

void dds3LinkSceneLight(dds3Basic_t *basic, s32 frames);

INCLUDE_ASM(const s32, "basic/dds3LightObjectBasic", dds3LinkSceneLight);

void dds3UnlinkSceneLight(dds3Basic_t *basic)
{
    dds3LightObjectBasicWorkData_t *work = basic->workdata;

    work->Flag &= ~3;
}

void dds3LinkLightObjectToUnitObject(dds3Basic_t *light, dds3Basic_t *unit, s32 frames)
{
    dds3UnitObjectWork_t *work = unit->workdata;

    evtLinkLightObjectBasic(work->unit, light, frames);
}

void dds3UnlinkLightObjectFromUnitObject(dds3Basic_t *unit, s32 frames)
{
    dds3UnitObjectWork_t *work = unit->workdata;

    evtUnlinkLightObjectBasic(work->unit, frames);
}

void dds3SetLightObjectResoruce(dds3Basic_t *basic, fldResourceLight_t *res)
{
    dds3LightObjectBasicWorkData_t *work = basic->workdata;

    work->Flag = 0;
    if (res->Flag & 1) {
        work->Flag = 1;
    }
    if (res->Flag & 2) {
        work->Flag |= 4;
    }

    work->InsideR = res->InsideR;
    work->AnimeType = res->AnimeType;
    work->OutsideR = res->OutsideR;
    work->Soften = res->Soften;
    work->Bias = res->Bias;
    work->Light.color.x = res->DiffuseR;
    work->Light.color.y = res->DiffuseG;
    work->Light.color.z = res->DiffuseB;
    work->Light.color.w = 0.0f;
    work->Ambient.x = res->AmbientR;
    work->Ambient.y = res->AmbientG;
    work->Ambient.z = res->AmbientB;
    work->Ambient.w = 1.0f;
}

void dds3SetLightObjectPathResoruce(dds3Basic_t *basic, fldResourceLightPath_t *res)
{
    dds3LightObjectBasicWorkData_t *work = basic->workdata;

    work->InsideR = res->InsideR;
    work->OutsideR = res->OutsideR;
    work->Soften = res->Soften;
    work->Bias = res->Bias;
    work->Light.color.x = res->DiffuseR;
    work->Light.color.y = res->DiffuseG;
    work->Light.color.z = res->DiffuseB;
    work->Light.color.w = 0.0f;
    work->Ambient.x = res->AmbientR;
    work->Ambient.y = res->AmbientG;
    work->Ambient.z = res->AmbientB;
    work->Ambient.w = 1.0f;
}

s32 dds3AllocEventBasicWorkData(dds3Basic_t *basic)
{
    void *work;

    work = sdfChipAllocDirect(28);
    basic->workdata = work;
    memset(work, 0, 28);
    return 1;
}
