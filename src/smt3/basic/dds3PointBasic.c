#include "common.h"
#include "basic.h"
#include "kernel.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "newdata.h"
#include "field.h"

typedef struct fldResourcePoint_t fldResourcePoint_t;

extern dds3Basic_t *dds3AddSceneBasicData(u32 type);

dds3Basic_t *dds3AddPointBasic(u32 serialID, fldResourcePoint_t *pData, char *pName)
{
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_POINT);

    basic->workdata = pData;
    basic->serialID = serialID;
    basic->name = pName;
    return basic;
}

fldResourcePoint_t *dds3PointBasicGetResource(dds3Basic_t *pPointBasic)
{
    return pPointBasic->workdata;
}
