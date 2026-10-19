#include "common.h"
#include "kernel.h"
#include "ps2asm.h"
#include "nb.h"
#include "model.h"
#include "motion.h"

/* 128-byte party records; this unit only reads the formation index. */
struct nbParty_t {
    u16 flag;
    s16 formindex;
    u8 unk04[0x7C];
};

extern s8 nbCameraPressRunFlag;

extern u32 nbCameraPressThrough;

extern u32 nbCameraPressFrame;

void nbCameraPressStart(void);

INCLUDE_ASM(const s32, "newbattle/nbCameraPress", nbCameraPressProcessRun);

INCLUDE_ASM(const s32, "newbattle/nbCameraPress", nbCameraPressProcessInit);

void nbCameraPressStart(void)
{
    nbCameraPressFrame = 0;
    nbCameraPressRunFlag = TRUE;
}

s32 nbCameraPressProcessCheckRun(void)
{
    return nbCameraPressRunFlag;
}

s32 nbCameraPressProcessGetFrame(void)
{
    return nbCameraPressFrame;
}

void nbCameraPressProcessThrough(void)
{
    nbCameraPressThrough = TRUE;
}
