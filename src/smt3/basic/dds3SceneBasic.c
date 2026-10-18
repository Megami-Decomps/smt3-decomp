#include "common.h"
#include "basic.h"
dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type, const char *name);


INCLUDE_ASM(const s32, "basic/dds3SceneBasic", sceneconstructor);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", scenedestructor);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", scenemainprocuder);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", scenedrawprocuder);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3VisibleSceneBasic);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3AddSceneBasicData);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3DeleteSceneBasicData);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3SetActiveCameraSceneBasic);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3GetActiveCameraSceneBasic);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3SetActiveUnitSceneBasic);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3GetActiveUnitSceneBasic);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3SearchSceneBasic);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3SceneBasicGetList);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3SceneBasicMakeScene);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3SceneBasicMakeScene_FromMemory);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3SceneBasicFreeScene);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3SceneBasicLoadScriptFile);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3SceneBasicLoadScriptFile_FromMem);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3SceneBasicReleaseScriptFile);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3SceneBasicStartScript);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3SceneBasicEndScript);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3SceneBasicExistBasicFromType);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3SceneBasicExistBasic);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3SetMapNo);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3GetMapNo);

INCLUDE_ASM(const s32, "basic/dds3SceneBasic", dds3SceneBasicSearchNameAndType);

dds3Basic_t *dds3SceneBasicSearchName(dds3Basic_t *scene, const char *name)
{
    dds3Basic_t *basic;
    s32 type;

    if (scene == NULL || name == NULL) {
        return NULL;
    }
    for (type = 0; type < BASIC_TYPE_MAX; type++) {
        basic = dds3SceneBasicSearchNameAndType(scene, type, name);
        if (basic != NULL) {
            return basic;
        }
    }
    return NULL;
}
