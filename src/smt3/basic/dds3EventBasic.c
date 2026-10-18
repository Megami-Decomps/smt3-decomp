#include "common.h"
#include "basic.h"

extern dds3Basic_t *dds3AddSceneBasicData(s32 type);

dds3Basic_t *dds3AddEventBasic(s32 id, void *resource, char *name) {
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_EVENT);

    basic->workdata = resource;
    basic->serialID = id;
    basic->name = name;
    return basic;
}

void *dds3EventBasicGetResource(dds3Basic_t *basic) {
    return basic->workdata;
}
