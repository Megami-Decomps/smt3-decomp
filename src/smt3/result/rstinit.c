#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

extern void dds3KillProcessName(char *name, s32 shutdowntimer);

void fclFreeStage(void);

extern void *dds3SearchProcessName(const char *namestr);

typedef struct
{
    /* 0x0000 */ u8 unk_00[0x20];
    /* 0x0020 */ f32 screenOfsX;
    /* 0x0024 */ f32 screenOfsY;
    /* 0x0028 */ u8 unk_28[0x10];
    /* 0x0038 */ s32 texHandle[7];
    /* 0x0054 */ s32 flags;
    /* 0x0058 */ u16 unk_58;
    /* 0x005A */ u16 unk_5A;
    /* 0x005C */ u8 unk_5C[8];
    /* 0x0064 */ s16 listCursor[4];
    /* 0x006C */ u8 unk_6C;
    /* 0x006D */ u8 unk_6D;
    /* 0x006E */ u16 modelId;
    /* 0x0070 */ s32 unk_70;
    /* 0x0074 */ u8 unk_74[0x10];
    /* 0x0084 */ u8 unk_84[0x2DC];
    /* 0x0360 */ void *explainData;
    /* 0x0364 */ s32 unk_364;
    /* 0x0368 */ void *allocation;
    /* 0x036C */ s32 unk_36C;
} fclEncyc_instance_tag;

extern s32 datCopyDevilWorkEntry(datUnitWork_t *work, s32 level);

typedef struct
{
    /* 0x0000 */ u8 unk_00[0x18];
    /* 0x0018 */ s8 unk_18;
    /* 0x0019 */ s8 unk_19;
    /* 0x001A */ u8 unk_1A[0x16];
    /* 0x0030 */ datUnitWork_t *levelUpUnit[16];
    /* 0x0070 */ s8 levelUpCount;
    /* 0x0071 */ s8 levelUpStep;
    /* 0x0072 */ u8 unk_72[0xFA];
    /* 0x016C */ datUnitWork_t *unit;
    /* 0x0170 */ u8 unk_170[0x2175];
    /* 0x22E5 */ s8 unk_22E5;
    /* 0x22E6 */ s8 unk_22E6;
    /* 0x22E7 */ u8 unk_22E7;
    /* 0x22E8 */ f32 unk_22E8;
    /* 0x22EC */ f32 unk_22EC;
    /* 0x22F0 */ u8 unk_22F0[0x18];
    /* 0x2308 */ s32 unk_2308;
} evtStartWork_t;

extern evtStartWork_t *EventStart;

typedef struct
{
    /* 0x00 */ datUnitWork_t unit;
} dds3PartyUnit_t;

typedef struct
{
    /* 0x00000 */ u8 unk_00000[0xA54];
    /* 0x00A54 */ dds3PartyUnit_t party[16];
    /* 0x01914 */ s8 stock[0x10];
    /* 0x01924 */ s32 stockCount;
    /* 0x01928 */ u8 unk_01928[0x3D];
    /* 0x01965 */ u8 unk_1965;
    /* 0x01966 */ u8 unk_01966[0x394];
    /* 0x01CFA */ u8 equipHearts;
    /* 0x01CFB */ u8 unk_01CFB[0x40];
    /* 0x01D3B */ u8 devilSlot[0x100];
    /* 0x01E3B */ u8 unk_01E3B[0x10045];
    /* 0x11E80 */ s32 encycNum;
} cmbGlobalWork_t;

/* newdata.h declares the same symbol dds3GlobalWork_t *DDS3_GBWK, and this
 * unit reads it through a different member set at the same offsets. Both are
 * partial views; the alias holds them apart until one covers the object. */
extern cmbGlobalWork_t *cmbGlobalWork asm("DDS3_GBWK");

extern u8 *GBWK_1;

extern s32 printf(const char *fmt, ...);

extern char D_0053DA28[];

extern s8 gProcessStat_4;

extern char D_0054FCF0[];

extern char D_0053DA90[];

extern char D_0053DAA0[];

extern void (*DrawPhaseTable[])(fclEncyc_instance_tag *work);

INCLUDE_ASM(const s32, "result/rstinit", rstInitEvoAct);

s32 rstChkEvoAct(void)
{
    char name[10] = "e809_main";

    return dds3SearchProcessName(name) != 0;
}

void rstKillEvoAct(void)
{
    fclFreeStage();
    printf(D_0053DA28);
}

void rstSetTestParam(void)
{
    cmbGlobalWork->party[0].unit.exp += 100;
    cmbGlobalWork->party[4].unit.exp += cmbGlobalWork->party[4].unit.level * 100;
}

s32 rstChkLevelUpTarget(datUnitWork_t *pStock)
{
    if (pStock->flag & 1) {
        if (!(pStock->badstatus & 0x800)) {
            if (cmbGetLevelUpCount(pStock, 0) > 0) {
                return 1;
            }
        }
    }
    return 0;
}

extern s32 rstChkLevelUpTarget(datUnitWork_t *pStock);

s32 rstChkResultEntry(void)
{
    s32 i;

    for (i = 0; i < 0x10; i++)
    {
        if (rstChkLevelUpTarget(&cmbGlobalWork->party[i].unit) != 0)
        {
            return 1;
        }
    }
    return 0;
}

extern s32 cmbGetLevelUpCount(datUnitWork_t *unit, u32 exp);

u8 rstCalcLevelFromExp(u8 Index, u32 Exp)
{
    datUnitWork_t *unit = &cmbGlobalWork->party[Index].unit;

    return (u8)unit->level + cmbGetLevelUpCount(unit, Exp - unit->exp);
}

s32 rstCalcExpFromLevel(u8 Index, s32 Level)
{
    return datCopyDevilWorkEntry(&cmbGlobalWork->party[Index].unit, Level);
}

void rstChkResult2Process(u8 *work)
{
    memset(&work[0x129F], 0, 6);
}

INCLUDE_ASM(const s32, "result/rstinit", rstCreateTargetList);

INCLUDE_RODATA(const s32, "result/rstinit", D_0053DA28);

INCLUDE_ASM(const s32, "result/rstinit", rstInit);

INCLUDE_ASM(const s32, "result/rstinit", rstDestroyResult2);

INCLUDE_ASM(const s32, "result/rstinit", rstResult2ProcessStart);

void rstResult2ProcessEnd(void)
{
    dds3KillProcessName(D_0054FCF0, 1);
    dds3KillProcessName(D_0053DA90, 1);
    dds3KillProcessName(D_0053DAA0, 1);
}

s32 checkResultStat(void)
{
    if (gProcessStat_4 == 1) {
        return 1;
    }
    if (gProcessStat_4 < 2) {
        return 0;
    }
    if (gProcessStat_4 == 2) {
        gProcessStat_4 = 0;
    }
    return 0;
}

void rstSetLevelUpFlag(void)
{
    if (GBWK_1 != NULL) {
        *GBWK_1 |= 0x10;
    }
}

void rstAddExp(s32 amount, datUnitWork_t *work)
{
    if (work->level < 0xFF) {
        work->level += amount;
    }
}

void rstAdvanceLevelUpStep(void)
{
    evtStartWork_t *work = EventStart;
    s8 step;

    if (work->levelUpCount == 0) {
        return;
    }
    step = work->levelUpStep;
    if (step >= work->levelUpCount) {
        return;
    }
    work->levelUpStep++;
    rstPrepareStatusModel(6, work->levelUpUnit[step]->id, 0);
}

s32 func_0021FE18(void)
{
    return EventStart->unk_22E5 >= 0;
}

void func_0021FE30(s8 a, s8 b, f32 x, f32 y)
{
    EventStart->unk_22E5 = a;
    EventStart->unk_22E8 = x;
    EventStart->unk_22EC = y;
    EventStart->unk_22E6 = b;
}

INCLUDE_ASM(const s32, "result/rstinit", rstDrawEvoModel);

void rstSetEvoState(void)
{
    EventStart->unk_22E6 = 2;
}

INCLUDE_RODATA(const s32, "result/rstinit", D_0053DA90);

INCLUDE_RODATA(const s32, "result/rstinit", D_0053DAA0);

INCLUDE_SDATA(const s32, "result/rstinit", GBWK_1);

INCLUDE_SDATA(const s32, "result/rstinit", gProcessStat_4);

INCLUDE_SDATA(const s32, "result/rstinit", D_0054FCED);

INCLUDE_SDATA(const s32, "result/rstinit", D_0054FCF0);

INCLUDE_SDATA(const s32, "result/rstinit", D_0054FCF8);

