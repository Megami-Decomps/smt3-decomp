#include "common.h"
#include "basic.h"

extern dds3Basic_t *dds3AddSceneBasicData(s32 type);

dds3Basic_t *dds3AddEventBasic(s32 serialID, void *pData, char *pName) {
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_EVENT);

    basic->workdata = pData;
    basic->serialID = serialID;
    basic->name = pName;
    return basic;
}

void *dds3EventBasicGetResource(dds3Basic_t *pBasic) {
    return pBasic->workdata;
}
