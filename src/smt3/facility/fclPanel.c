#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

typedef struct fclPanel
{
    /* 0x00 */ s32 X;
    /* 0x04 */ s32 Y;
    /* 0x08 */ s32 Z;
    /* 0x0C */ s16 W;
    /* 0x0E */ s16 H;
    /* 0x10 */ s32 Col;
    /* 0x14 */ u8 Spd;
    /* 0x15 */ u8 unk_15[3];
    /* 0x18 */ s32 Timer;
    /* 0x1C */ s32 Timer2;
    /* 0x20 */ s32 Flag;
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 TexHandle;
    /* 0x38 */ void (*draw)(struct fclPanel *panel, s32 id);
    /* 0x3C */ void (*update)(struct fclPanel *panel);
} fclPanel_t;

extern void sdfChipFreeDirect(void *aPtr);

void fclDrawPanelTypeDef(fclPanel_t *pPanel, s32 OtNo)
{
    s32 color = pPanel->Col;

    fclDrawPanelSprite2(0x44, OtNo);
    cmpDrawBoxF(pPanel->X, pPanel->Y, pPanel->Z, (u16)pPanel->W,
                (u16)pPanel->H, color, OtNo);
}

void fclUpdatePanelTypeDef(fclPanel_t *pPanel)
{
    pPanel->Timer += pPanel->Spd;
    if (pPanel->Timer > 0x100) {
        pPanel->Timer = 0x100;
    }
}

INCLUDE_ASM(const s32, "facility/fclPanel", fclDrawPanelType0Grad);

INCLUDE_ASM(const s32, "facility/fclPanel", fclDrawPanelType0);

void fclUpdatePanelType0(fclPanel_t *pPanel)
{
    pPanel->Timer += pPanel->Spd;
    if (pPanel->Timer > 0x100) {
        pPanel->Timer = 0x100;
    }
}

INCLUDE_ASM(const s32, "facility/fclPanel", fclDrawPanelType1);

void fclUpdatePanelType1(fclPanel_t *pPanel)
{
    pPanel->Timer += pPanel->Spd;
    if (pPanel->Timer > 0x200) {
        pPanel->Timer = 0;
    }
}

INCLUDE_ASM(const s32, "facility/fclPanel", fclDrawPanelType2);

void fclUpdatePanelType2(fclPanel_t *pPanel)
{
    pPanel->Timer += pPanel->Spd;
    if (pPanel->Timer > 0x100) {
        pPanel->Timer = 0x100;
    }
}

INCLUDE_ASM(const s32, "facility/fclPanel", fclDrawPanelCorners);

INCLUDE_ASM(const s32, "facility/fclPanel", fclDrawPanelType3BackGrad);

INCLUDE_ASM(const s32, "facility/fclPanel", fclDrawPanelEdges);

INCLUDE_ASM(const s32, "facility/fclPanel", fclDrawPanelType3);

void fclUpdatePanelType3(fclPanel_t *pPanel)
{
    pPanel->Timer += pPanel->Spd;
    if (pPanel->Timer >= 0x201) {
        pPanel->Timer = 0;
    }
    pPanel->Timer2 += 0x20;
    if (pPanel->Timer2 >= 0x201) {
        pPanel->Timer2 = 0x200;
    }
}

void fclSetPanelTex(fclPanel_t *pPanel, s32 UV1, s32 UV2, s32 UV3, s32 UV4, s32 TexHandle)
{
    pPanel->unk_24 = UV1;
    pPanel->unk_28 = UV2;
    pPanel->unk_2C = UV3;
    pPanel->unk_30 = UV4;
    pPanel->TexHandle = TexHandle;
}

INCLUDE_ASM(const s32, "facility/fclPanel", fclInitPanel);

INCLUDE_ASM(const s32, "facility/fclPanel", fclCreatePanel);

s32 fclDestroyPanel(void *pPanel)
{
    sdfChipFreeDirect(pPanel);
    return 1;
}

/* NON_MATCHING: 8 of 21 words differ (first at +0x10). */
#ifdef NON_MATCHING
s32 fclDrawPanel(fclPanel_t *pPanel, s32 OtNo)
{
    fclDrawPanelSprite(0x53001, OtNo);
    pPanel->draw(pPanel, OtNo);
    return fclDrawPanelSprite(0x5101B, OtNo);
}
#else
INCLUDE_ASM(const s32, "facility/fclPanel", fclDrawPanel);
#endif

void fclUpdatePanel(fclPanel_t *pPanel)
{
    pPanel->update(pPanel);
}
