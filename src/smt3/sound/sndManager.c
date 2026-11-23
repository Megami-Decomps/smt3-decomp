#include "common.h"
#include "sdfFrame.h"
#include "sndManager.h"
#include "facility.h"
#include "fpu.h"
#include "libsdf.h"
#include "titleGlobal.h"
#include "fileManager.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"



extern dds3TitleWork_t *titleWork;

extern u32 dds3GetKernelCount(void);

extern s32 p2sdStopAllSe(void);

extern void sdfMemFreeDirect(void *aHandle);

extern dds3ProcessID_t *titleAllocWork(s32 arg);

extern void movieStop(void);

extern s8 dds3PadButtonState[];

extern s32 scrGetIntPara(s32 n);

extern void p2sdPlayBGM(s32 MidiNo);

extern void sdfTexFree(sdfTexHandle_t *aHandle);

extern void p2sdPlaySE(s32 index, s32 volume, s32 pan);

extern s32 titleFreeWork(s32 arg);

extern s32 dds3KernelDebugSign;

extern void titleFreeTextures(void);

extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);

extern char sndPathPrefix[]; /* "/sound/" */

extern char *strcat(char *dst, const char *src);

extern void *memcpy(void *a, const void *b, size_t c);

char *sdfSndMakeFilename(char *aDstName, const char *aSrcName)
{
    memcpy(aDstName, sndPathPrefix, 8);
    return strcat(aDstName, aSrcName);
}

extern s8 sndInitDone;

extern s32 D_0054FD20;

extern s32 D_0054FD24;

extern s16 D_00551F94;

extern char sndVcInfo[];

extern u32 sndSqInfo[4];

/* Only the first packed MIDI ID word of this raw sound bank is read here. */
extern s32 D_0033A540 __attribute__((section(".data")));

extern void sdfSndInit(char *aVcInfo, s32 aNumVc, char *aSqInfo, s32 aNumSq);

extern void p2sdLoadSMGFile(s32 MidiNo);

void sndInit(void)
{
    if (sndInitDone != 0) {
        p2sdStopAllSe();
        return;
    }
    sndInitDone = 1;
    sdfSndInit(sndVcInfo, 4, (char *)sndSqInfo, 4);
    p2sdLoadSMGFile(D_0033A540);
    D_0054FD20 = 4;
    D_00551F94 = -1;
    D_0054FD24 = 0;
}

s32 snd_226388(void)
{
    return 0x7B0;
}

s32 snd_226390(void)
{
    p2sdPlayBGM(scrGetIntPara(0));
    return 1;
}

s32 dds3_2263B8(void)
{
    p2sdPlaySE(scrGetIntPara(0), 0x7F, 0x3F);
    return 1;
}

s32 dds3_2263E8(void)
{
    p2sdStopAllSe();
    return 1;
}

s8 sndInitDone = 0;

char sndPathPrefix[8] = "/sound/";

s32 D_0054FD20 = 0;

s32 D_0054FD24 = 0;

