#include "common.h"
#include "newbattle.h"
#include "kernel.h"
#include "ps2asm.h"
#include "nb.h"
#include "model.h"
#include "motion.h"

/* 128-byte party records; this unit only reads the formation index. */


extern s8 nbCameraPressRunFlag;

extern u32 nbCameraPressThrough;

extern u32 nbCameraPressFrame;

void nbCameraPressStart(void);

extern u32 D_00325970[];

extern u32 D_00325998[];

extern nbMainProcessData_t *nbGetMainProcessData(void);

extern u32 effColorBlend(u32 abgr0, u32 abgr1, f32 rate);

/* NON_MATCHING: 2 of 61 words differ, at the exact 244-byte size. */
#ifdef NON_MATCHING
void nbCameraPressProcessRun(void)
{
    nbMainProcessData_t *data;
    u32 *target;
    s32 frame;
    f32 rate;

    if (nbCameraPressRunFlag != 0) {
        data = nbGetMainProcessData();
        if (data != NULL) {
            if (nbCameraPressFrame < 0xA) {
                frame = nbCameraPressFrame;
                if ((data->stat & 8) != 0) {
                    target = D_00325998;
                } else {
                    target = D_00325970;
                }
                if (frame < 3) {
                    rate = frame / 3.0f;
                } else if (0xA - frame < 4) {
                    rate = (0xA - frame) / 3.0f;
                } else {
                    rate = 1.0f;
                }
                *target = effColorBlend(0x808080, 0x60808080, rate);
            }
            frame = nbCameraPressFrame + 1;
            nbCameraPressFrame = frame;
            if (frame >= 0x15) {
                nbCameraPressRunFlag = FALSE;
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCameraPress", nbCameraPressProcessRun);
#endif

#ifdef NON_MATCHING
void nbCameraPressProcessInit(void)
{
    if (nbCameraPressThrough == 0) {
        nbCameraPressStart();
    } else {
        nbCameraPressRunFlag = FALSE;
        nbCameraPressThrough = FALSE;
        nbCameraPressFrame = 100;
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCameraPress", nbCameraPressProcessInit);
#endif

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
