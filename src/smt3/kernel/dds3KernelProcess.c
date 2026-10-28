#include "common.h"
#include "basic.h"
#include "kernel.h"
#include "libsdf.h"
#include "draw.h"

/* One frame buffer's draw bank, stride 0x1BE0; only the eleven model draw
 * environments at 0x40 and the init tag at 0x0 are reached here. */
typedef struct
{
    u8 unk0000[0x40];
    sdfModelDrawEnv_t modelEnv[11];
    u8 unk12D0[0x910];
} dds3KernelProcess_dds3DrawBank_t;

typedef struct
{
    sdfDrawTag_t *tagHead;
    sdfPreDrawTag_t *tail;
} dds3KernelProcess_sdfPreDrawChain_t;

extern dds3KernelProcess_dds3DrawBank_t dds3DrawEnv[];

extern dds3KernelProcess_sdfPreDrawChain_t D_002F6A70[];

extern sdfGrOt_t sdfGrOtTable[];

extern s32 dds3GetFrameID(void);

extern void sdfGrInitNewFrame(void);

extern void sdfGrOtInit(sdfGrOt_t *aOt, s32 aNumOt);

extern void sdfPreDrawChainInit(dds3KernelProcess_sdfPreDrawChain_t *aChain);

extern void dds3InitDrawEnvSub(s32 i);

extern void sdfModelSetDrawEnv(sdfModelDrawEnv_t *aEnv);

extern dds3Basic_t *dds3TheWorld;

extern void dds3CallBasicMainProc(dds3Basic_t *pBasic);

extern void dds3CallBasicDrawProc(dds3Basic_t *basic);

s32 dds3FirstProcess(dds3ProcessID_t *id)
{
    sdfModelDrawEnv_t *env;
    s32 frame;
    s32 i;

    frame = dds3GetFrameID();
    sdfGrInitNewFrame();
    sdfGrOtInit(sdfGrOtTable, 0x55);
    sdfPreDrawChainInit(D_002F6A70);
    dds3InitDrawEnvSub(frame);
    env = dds3DrawEnv[frame].modelEnv;
    for (i = 0; i < 11; i++) {
        sdfModelSetDrawEnv(env);
        env++;
    }
    sdfGrOtTable[0].regFunc(&sdfGrOtTable[0], (sdfDrawTag_t *)&dds3DrawEnv[frame]);
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

