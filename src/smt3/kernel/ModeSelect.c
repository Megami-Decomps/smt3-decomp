#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"

extern void dds3KillProcessName(char *name, s32 shutdowntimer);

extern s32 fldWmapEnabled;

typedef struct
{
    /* 0x0000 */ u8 unk_00[0x20];
    /* 0x0020 */ f32 screenOfsX;
    /* 0x0024 */ f32 screenOfsY;
    /* 0x0028 */ u8 unk_28[0x10];
    /* 0x0038 */ s32 texHandle[7];
    /* 0x0054 */ s32 stat;
    /* 0x0058 */ u16 unk_58;
    /* 0x005A */ u16 unk_5A;
    /* 0x005C */ u8 unk_5C[8];
    /* 0x0064 */ s16 lw_cur[4];
    /* 0x006C */ u8 unk_6C;
    /* 0x006D */ u8 unk_6D;
    /* 0x006E */ u16 modelId;
    /* 0x0070 */ s32 unk_70;
    /* 0x0074 */ u8 unk_74[0x10];
    /* 0x0084 */ u8 unk_84[0x2DC];
    /* 0x0360 */ void *explainData;
    /* 0x0364 */ s32 unk_364;
    /* 0x0368 */ void *allocation;
    /* 0x036C */ s32 pwork;
} fclEncyc_instance_tag;

extern dds3ProcessID_t *mselShutDown(u32 mode);

extern char D_0053DF08[];

extern void mselFreeResources(s32 arg);

typedef struct
{
    /* 0x00 */ void *unk_00;
    /* 0x04 */ void *unk_04;
    /* 0x08 */ void *unk_08;
    /* 0x0C */ u8 unk_0C[0x34];
    /* 0x40 */ s32 unk_40;
    /* 0x44 */ s32 unk_44;
    /* 0x48 */ s32 unk_48;
    /* 0x4C */ s32 unk_4C;
    /* 0x50 */ s32 unk_50;
    /* 0x54 */ s32 cursor;
    /* 0x58 */ void *unk_58;
    /* 0x5C */ void *unk_5C;
    /* 0x60 */ s32 unk_60;
    /* 0x64 */ s32 tover[5];
    /* 0x78 */ u8 unk_78[0x10];
    /* 0x88 */ s8 unk_88;
    /* 0x89 */ u8 unk_89[3];
    /* 0x8C */ s32 unk_8C;
    /* 0x90 */ s32 unk_90;
    /* 0x94 */ s32 unk_94[5];
    /* 0xA8 */ s32 unk_A8;
} mselGlobalWork_t;

extern mselGlobalWork_t *gGlobal;

s32 fclDestroyMessage(void);
s8 fclInitMessage(void *data);

s32 fclLoadTexture(char *pFileName);

extern s32 evtBitOn(s32 no);

extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);

INCLUDE_ASM(const s32, "kernel/ModeSelect", mselDevilChoice);

void Msel_TOverData_Clear(void)
{
    s32 i;

    for (i = 0; i < 5; i++) {
        gGlobal->tover[i] = -1;
    }
    gGlobal->unk_60 = 0;
}

void mselCountSaves(void)
{
    mselGlobalWork_t *work;
    s32 i;

    for (i = 0; i < 5; i++)
    {
        gGlobal->unk_94[i] = gGlobal->tover[i];
    }
    work = gGlobal;
    work->unk_90 = work->unk_60;
    if (work->unk_60 != 0)
    {
        work->unk_88 = 0;
        work->unk_8C = 0x64618780;
    }
}

void mselFindSaveSlot(s32 *first, s32 *second, u32 start, u32 end, s8 reverse)
{
    u32 i;

    if (reverse != 0) {
        i = start;
        if (start < end) {
            for (; i < end; i++) {
                if (first != NULL) {
                    first[i - 1] = first[i];
                }
                if (second != NULL) {
                    second[i - 1] = second[i];
                }
            }
        }
    } else {
        i = end - 1;
        if (i >= start) {
            do {
                if (first != NULL) {
                    first[i] = first[i - 1];
                }
                if (second != NULL) {
                    second[i] = second[i - 1];
                }
                i--;
            } while (i >= start);
        }
    }
}

extern char D_0053DEC0[];

extern char D_0053DED8[];

void mselLoadBackground(void)
{
    gGlobal->unk_40 = fclLoadTexture(D_0053DEC0);
    gGlobal->unk_44 = 0;
    gGlobal->unk_48 = fclLoadTexture(D_0053DED8);
}

extern void sdfTexFreeDelayed(s32 aHandle);

void mselFreeTextures(void)
{
    if (gGlobal->unk_40 != 0) {
        sdfTexFreeDelayed(gGlobal->unk_40);
        gGlobal->unk_40 = 0;
    }
    if (gGlobal->unk_44 != 0) {
        sdfTexFreeDelayed(gGlobal->unk_44);
        gGlobal->unk_44 = 0;
    }
    if (gGlobal->unk_48 != 0) {
        sdfTexFreeDelayed(gGlobal->unk_48);
        gGlobal->unk_48 = 0;
    }
}

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053DEC0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053DED8);

const char D_0053DEF0[] = "/title/mseldata.bmd";

extern sdfMemHandle_t *sdfMemAllocDirect(s32 size);
extern void *sdfMemLock(sdfMemHandle_t *handle);
extern void *sdfDevLoadFileEx(const char *path, u32 *address, u32 *size);
extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 startTimer, s32 shutdownTimer,
                                         dds3ProcessFunc_t processFunc, dds3ProcessFunc_t shutdownFunc, void *work);
extern s32 mselMain(dds3ProcessID_t *id);
extern u32 dds3BackColor;

dds3ProcessID_t *mselShutDown(u32 mode)
{
    sdfMemHandle_t *handle;
    u32 *work;
    s32 i;
    void *fileHandle;

    handle = sdfMemAllocDirect(0xB4);
    work = sdfMemLock(handle);
    gGlobal = (mselGlobalWork_t *)work;
    gGlobal->unk_00 = handle;
    gGlobal->unk_4C = 0;
    gGlobal->unk_50 = 0;
    gGlobal->cursor = 0;
    for (i = 0; i < 5; i++) {
        gGlobal->tover[i] = -1;
    }
    gGlobal->unk_60 = 0;
    mselLoadBackground();
    gGlobal->unk_A8 = mode;
    gGlobal->unk_04 = sdfMemAllocDirect(0x1A510);
    gGlobal->unk_08 = sdfMemAllocDirect(0x7E90);
    fileHandle = sdfDevLoadFileEx(D_0053DEF0, (u32 *)&gGlobal->unk_5C, NULL);
    gGlobal->unk_58 = fileHandle;
    fclInitMessage(gGlobal->unk_5C);
    dds3BackColor = 0x80000000;
    return dds3StartProcess(D_0053DF08, 0x2B19, 0, 0, mselMain, NULL, NULL);
}

extern void mselFreeTextures(void);

extern void sdfMemFreeDelayedDirect(void *aHandle);

void mselFreeResources(s32 arg)
{
    mselFreeTextures();
    fclDestroyMessage();
    sdfMemFreeDelayedDirect(gGlobal->unk_04);
    sdfMemFreeDelayedDirect(gGlobal->unk_08);
    sdfMemFreeDelayedDirect(gGlobal->unk_58);
    sdfMemFreeDelayedDirect(gGlobal->unk_00);
    gGlobal = NULL;
}

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053DF08);

INCLUDE_ASM(const s32, "kernel/ModeSelect", mselMain);

s32 mselExit(u32 mode)
{
    mselShutDown(mode);
    return -1;
}

s32 mselInit(void)
{
    mselFreeResources(0);
    dds3KillProcessName(D_0053DF08, 1);
    return 0;
}

s32 fldWmapIsEnabled(void)
{
    return fldWmapEnabled;
}

extern void dds3GlobalWorkUserInit2(s32 mode);

extern s32 dds3ConfigChanged(void);

extern void dds3ConfigSaveBackup(void);

extern void dds3ConfigRestoreBackup(void);

extern s32 evtBitOn(s32 no);

s32 mselStartDataSet(void)
{
    if (gGlobal->unk_A8 == 0) {
        dds3CreateGameData(0);
    }
    if (DDS3_GBWK->transition != 0) {
        dds3GlobalWorkUserInit2(1);
    } else {
        dds3GlobalWorkUserInit2(0);
    }
    if (dds3ConfigChanged() == 0) {
        dds3ConfigSaveBackup();
    } else {
        dds3ConfigRestoreBackup();
    }
    return evtBitOn(0xC0F);
}

typedef struct
{
    /* 0x00 */ u8 unk_00[0x44];
    /* 0x44 */ s32 enabled;
    /* 0x48 */ u32 unk_48;
    /* 0x4C */ s32 counter;
} fldDanteGunWork_t;

extern fldDanteGunWork_t fldRunDanteGunFlag;

extern void fldSetNextField(s32 a, s32 b);

extern void dds3AdminiCallSequence(s32 sequence, void *data, s32 datasize, s32 restore);

void mselStartGame(void)
{
    s32 field;

    fldRunDanteGunFlag.enabled = 1;
    field = 0x259;
    fldRunDanteGunFlag.counter = 0;
    fldSetNextField(0xB, 0x259);
    dds3AdminiCallSequence(7, &field, 4, 0);
}

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053DF90);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053DFA8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053DFB8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053DFC8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053DFD8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053DFF0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E008);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E020);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E030);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E040);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E050);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E060);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E078);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E088);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E0A0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E0B0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E0C0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E0D0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E0E0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E0F8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E108);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E118);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E130);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E148);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E158);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E168);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E178);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E190);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E1A8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E1C0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E1D0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E1E0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E1F0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E200);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E218);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E228);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E240);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E250);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E260);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E270);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E280);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E290);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E2A8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E2C0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E2D0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E2E0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E2F0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E308);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E318);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E328);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E338);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E348);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E360);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E370);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E380);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E390);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E3A8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E3B8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E3C8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E3D8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E3F0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E400);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E410);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E420);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E430);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E440);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E458);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E468);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E478);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E488);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E498);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E4A8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E4B8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E4C8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E4D8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E4E8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E500);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E510);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E520);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E530);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E540);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E550);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E560);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E570);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E580);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E590);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E5A0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E5B0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E5C0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E5D0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E5E8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E5F8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E608);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E618);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E630);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E648);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E660);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E678);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E690);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E6A0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E6B8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E6C8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E6E0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E6F0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E700);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E710);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E720);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E730);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E740);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E750);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E760);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E770);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E780);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E798);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E7B0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E7C0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E7D8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E7E8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E7F8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E808);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E818);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E828);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E838);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E848);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E878);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E890);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E8A8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E8B8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E8D0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E8E0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E8F8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E910);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E920);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E930);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E950);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E968);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E978);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E988);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E998);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E9A8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E9B8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E9D0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053E9E8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EA00);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EA10);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EA20);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EA30);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EA48);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EA60);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EA78);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EA88);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EA98);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EAA8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EAB8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EAD0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EAE0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EAF0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EB08);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EB18);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EB30);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EB40);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EB58);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EB68);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EB80);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EB90);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EBA0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EBB0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EBC0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EBD0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EBE0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EBF0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EC00);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EC10);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EC28);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EC38);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EC48);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EC58);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EC70);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EC80);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EC90);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ECA0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ECB0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ECC0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ECD0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ECE8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ED00);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ED18);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ED30);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ED48);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ED60);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ED78);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053ED90);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EDA8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EDC0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EDD8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EDF0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EE08);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EE20);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EE38);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EE50);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EE68);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EE80);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EE98);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EEB0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EEC8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EEE0);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EEF8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EF10);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EF28);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EF40);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EF58);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EF70);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EF88);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_0053EFA0);

