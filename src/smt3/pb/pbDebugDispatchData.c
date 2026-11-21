#include "common.h"
#include "libsdf.h"

typedef struct pbDebugWork_t pbDebugWork_t;
typedef s32 (*pbDebugProcStep_t)(pbDebugWork_t *work, sdfDrawTag_t *tag);

extern s32 DebugProcMenu_pbDebugMode(pbDebugWork_t *work, sdfDrawTag_t *tag);
extern s32 DebugProcRadius(pbDebugWork_t *work, sdfDrawTag_t *tag);
extern s32 DebugProcDegree(pbDebugWork_t *work, sdfDrawTag_t *tag);
extern s32 DebugProcStage(pbDebugWork_t *work, sdfDrawTag_t *tag);
extern s32 DebugProcFree(pbDebugWork_t *work, sdfDrawTag_t *tag);

/* Native phase dispatch used by pbDebugProc. */
pbDebugProcStep_t DebugProcTable[5] = {
    DebugProcMenu_pbDebugMode,
    DebugProcRadius,
    DebugProcDegree,
    DebugProcStage,
    DebugProcFree,
};
