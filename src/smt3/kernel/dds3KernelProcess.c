#include "common.h"
#include "basic.h"
#include "kernel.h"
#include "kernel_draw.h"

extern dds3Basic_t *dds3TheWorld;

extern void dds3CallBasicMainProc(dds3Basic_t *pBasic);

extern void dds3CallBasicDrawProc(dds3Basic_t *basic);

typedef struct sdfPreDrawChain_t sdfPreDrawChain_t;
extern dds3DrawBank_t dds3DrawEnv[2];
extern sdfPreDrawChain_t D_002F6A70;
extern sdfGrOt_t sdfGrOtTable[85];
extern u32 dds3GetFrameID(void);
extern void sdfGrInitNewFrame(void);
extern void sdfGrOtInit(sdfGrOt_t *ot, s32 count);
extern void sdfPreDrawChainInit(sdfPreDrawChain_t *chain);
extern void dds3InitDrawEnvSub(s32 frame);
extern void sdfModelSetDrawEnv(sdfModelDrawEnv_t *env);

s32 dds3FirstProcess(dds3ProcessID_t *id)
{
    u32 frame = dds3GetFrameID();
    sdfModelDrawEnv_t *env;
    s32 count;

    sdfGrInitNewFrame();
    sdfGrOtInit(sdfGrOtTable, 85);
    sdfPreDrawChainInit(&D_002F6A70);
    dds3InitDrawEnvSub(frame);
    env = dds3DrawEnv[frame].modelEnv;
    for (count = 10; count >= 0; --count, ++env) {
        sdfModelSetDrawEnv(env);
    }
    sdfGrOtTable[0].regFunc(&sdfGrOtTable[0], &dds3DrawEnv[frame].initTag);
    return 0;
}

INCLUDE_ASM(const s32, "kernel/dds3KernelProcess", dds3DrawFirstProcess);

INCLUDE_ASM(const s32, "kernel/dds3KernelProcess", dds3DrawProcess);

s32 dds3BasicMainProcess(dds3ProcessID_t *id) {
    dds3CallBasicMainProc(dds3TheWorld);
    return 0;
}

s32 dds3BasicDrawProcess(dds3ProcessID_t *id) {
    dds3CallBasicDrawProc(dds3TheWorld);
    return 0;
}

