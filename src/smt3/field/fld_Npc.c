#include "common.h"
#include "basic.h"
#include "model.h"
#include "newdata.h"
#include "scr.h"
#include "field.h"
#include "ps2asm.h"

typedef struct
{
    s32 unk00;
    s32 id;
    s32 activationState;
    s32 activationRequested;
    u8 unk10[0x14];
    void *effect;              /* 0x24 */
    u8 unk28[8];
} fldTakaraSlot_t;

typedef struct
{
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
    u8 unk10[0x10];
    dds3Basic_t *object;       /* 0x20 */
    u8 unk24[0xC];
    void *effect;              /* 0x30 */
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 room;                  /* 0x48 */
    s32 unk4C;
    u8 unk50[0x20];
} fldNpcSlot_t;

typedef struct
{
    u8 data[0x100];
} fldNpcPalette_t;

typedef struct fldUnit_t fldUnit_t;

enum sl_slot_flag
{
    SL_SLOT_FLAG_CAN_READ_SAVE = 1,
    SL_SLOT_FLAG_DIRECTORY_READY = 2,
    SL_SLOT_FLAG_DELETE_BEFORE_WRITE = 8
};

extern void *memset(void *dst, s32 value, u32 size);

extern s32 fldTakaraCount;

extern fldTakaraSlot_t fldTakaraSlotTable[32];

extern s32 fldGb[];

extern void rsetOtOfs(u32);

extern void rsetAlpha(s32);

extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);

extern sdfTexHandle_t *fldUiTexture;

extern void DrwSprite2D(s32, s32, s32, s32, s32, s32, s32, s32, s32, sdfTexHandle_t *);

extern s32 fldNpcModelTable;

extern fldNpcSlot_t fldModelSlotTable[];

extern s32 fldGbChkNpc(s32 mapId, u32 slotIndex, s32 bit);

extern void dds3ObjectBaseOnFlag(dds3Basic_t *object, u32 flags);

extern u8 D_003129D0[0x100];

extern void sdfTexFreeDelayed(sdfTexHandle_t *texture);

extern void *memcpy(void *, const void *, unsigned int);

extern void fldNpcLightSet(fldUnit_t *unit);

extern s32 (*proc_func_ans)(dds3ProcessID_t *process);

extern void sdfMemFreeDelayedDirect(sdfMemHandle_t *allocation);

extern s32 fldNpcModelFreeIdx;

extern dds3Basic_t *fldNpcFreeCount;

extern s32 fldNpcCamTarget;

extern void mdlUnRegist(s32 kind, s32 slot);

extern void dds3DeleteUnitObjectBasic(dds3Basic_t *object);

extern s32 sprintf(char *dst, const char *format, ...);

extern void dds3EffectReset(void *effect);

extern void dds3EffectSetPosition(void *effect, const sdfVector4_t *position);

extern void *D_0054EEDC;

extern s32 evtBitCheck(s32 bit);

extern void dds3EffectDelete(void *effect);

extern void fldMakeFilePath(char *out, s32 field, s32 area);

extern char D_00532790[];

extern char D_005327A0[];

extern s32 sdfFileOpen(const char *path);

extern void sdfFileRead(s32 fd, void *buffer, s32 size);

extern void sdfFileClose(s32 fd);

extern sdfMemHandle_t *fldNpcModelCount;

extern sdfMemHandle_t *D_0054EF1C;

extern sdfMemHandle_t *D_0054EF20;

extern sdfMemHandle_t *D_0054EF24;

extern sdfTexHandle_t *D_0054EF38;

extern void *fldItemBoxCount;

extern void dds3EffectPartDelete(void *effect);


INCLUDE_ASM(const s32, "field/fld_Npc", fldNpcLightSet);

INCLUDE_ASM(const s32, "field/fld_Npc", fldNpcLoad);

INCLUDE_RODATA(const s32, "field/fld_Npc", D_005326F0);

INCLUDE_RODATA(const s32, "field/fld_Npc", D_00532700);

INCLUDE_ASM(const s32, "field/fld_Npc", fldItemBoxAdd);

void fldFindTakaraSlot(s32 id) {
    s32 i;

    for (i = 0; i < fldTakaraCount; i++) {
        if (fldTakaraSlotTable[i].id == id && fldTakaraSlotTable[i].activationState == 0) {
            fldTakaraSlotTable[i].activationRequested = 1;
        }
    }
}

void fldFreeModelTable(void) {
    s32 i;

    if (fldNpcModelCount != NULL) {
        mdlUnRegist(2, 0x1F);
        sdfMemFreeDelayedDirect(fldNpcModelCount);
        fldNpcModelCount = NULL;
    }
    if (D_0054EF1C != NULL) {
        mdlUnRegist(2, 0x15);
        sdfMemFreeDelayedDirect(D_0054EF1C);
        D_0054EF1C = NULL;
    }
    if (D_0054EF20 != NULL) {
        mdlUnRegist(2, 0x16);
        sdfMemFreeDelayedDirect(D_0054EF20);
        D_0054EF20 = NULL;
    }
    if (D_0054EF24 != NULL) {
        mdlUnRegist(2, 0x17);
        sdfMemFreeDelayedDirect(D_0054EF24);
        D_0054EF24 = NULL;
    }
    for (i = 0; i < 32; i++) {
        if (fldTakaraSlotTable[i].effect != NULL) {
            dds3EffectDelete(fldTakaraSlotTable[i].effect);
            fldTakaraSlotTable[i].effect = NULL;
        }
    }
    if (fldItemBoxCount != NULL) {
        dds3EffectPartDelete(fldItemBoxCount);
        fldItemBoxCount = NULL;
    }
    if (D_0054EF38 != NULL) {
        sdfTexFreeDelayed(D_0054EF38);
        D_0054EF38 = NULL;
    }
    fldTakaraCount = 0;
    for (i = 0; i < 32; i++) {
        fldTakaraSlotTable[i].id = -1;
        fldTakaraSlotTable[i].unk00 = 0;
        fldTakaraSlotTable[i].activationState = 0;
        fldTakaraSlotTable[i].activationRequested = 0;
    }
}

INCLUDE_ASM(const s32, "field/fld_Npc", fldItemBoxCalcTask);

void fldTDmgStart(f32 x, f32 y, f32 z) {
    sdfVector4_t position;

    memset(&position, 0, sizeof(position));
    position.x = x;
    position.y = y;
    position.z = z;
    position.w = 1.0f;
    dds3EffectReset(D_0054EEDC);
    dds3EffectSetPosition(D_0054EEDC, &position);
}

INCLUDE_ASM(const s32, "field/fld_Npc", fldDmgStart);

INCLUDE_ASM(const s32, "field/fld_Npc", fldSibukiStart);

INCLUDE_ASM(const s32, "field/fld_Npc", fldDrawEATexQuad);

void fldDrawAddressPanel(s32 width, s32 x, s32 y) {
    s32 ofs;
    s32 left;
    s32 right;
    s32 top;

    rsetOtOfs(0x47);
    rsetAlpha(0);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    ofs = 0x1BF - width;
    left = x + ofs;
    right = 0x210 - ofs;
    top = y + 0x12;
    DrwSprite2D(left, top, 0x5B, 0x1F, 1, 1, 0x5B, 0x1F, 0x80808080, fldUiTexture);
    DrwSprite2D(left + 0x5B, top, right, 0x1F, 0x5B, 1, 1, 0x1F, 0x80808080, fldUiTexture);
    rsetAlpha(0);
}

void fldDrawInputPanel(void) {
    if (fldGb[0] >= 0xC8) {
        return;
    }
    rsetOtOfs(0x47);
    rsetAlpha(0);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    DrwSprite2D(0x87, 0x124, 0x79, 0x18, 1, 0x5D, 0x79, 0x18, 0x80808080, fldUiTexture);
    DrwSprite2D(0x87, 0x13C, 0x79, 0x18, 1, 0x74, 0x79, -0x18, 0x80808080, fldUiTexture);
    DrwSprite2D(0x100, 0x124, 0x79, 0x18, 0x79, 0x5D, -0x79, 0x18, 0x80808080, fldUiTexture);
    DrwSprite2D(0x100, 0x13C, 0x79, 0x18, 0x79, 0x74, -0x79, -0x18, 0x80808080, fldUiTexture);
    rsetAlpha(0);
}

void fldDrawInputCursor(s32 x, s32 y) {
    if (fldGb[0] < 200) {
        rsetOtOfs(0x47);
        rsetAlpha(0);
        rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        DrwSprite2D(x, y, 0x14, 0x15, 0x2E, 0x47, 0x14, 0x15, 0x80808080, fldUiTexture);
        rsetAlpha(0);
    }
}

INCLUDE_RODATA(const s32, "field/fld_Npc", D_00532720);

INCLUDE_ASM(const s32, "field/fld_Npc", fldEADraw);

INCLUDE_ASM(const s32, "field/fld_Npc", fldVLDraw);

INCLUDE_RODATA(const s32, "field/fld_Npc", D_00532770);

INCLUDE_ASM(const s32, "field/fld_Npc", fldNpcModelStart);

void fldDeleteNpcModel(s32 index) {
    if (fldNpcModelFreeIdx == index && fldNpcFreeCount != NULL) {
        dds3DeleteUnitObjectBasic(fldNpcFreeCount);
        mdlUnRegist(2, 10);
        fldNpcFreeCount = NULL;
        fldNpcCamTarget = 0;
        fldNpcModelFreeIdx = -1;
    }
}

void fldFreeNpcModel(void) {
    if (fldNpcModelFreeIdx != -1 && fldNpcFreeCount != NULL) {
        mdlUnRegist(2, 10);
        fldNpcFreeCount = NULL;
        fldNpcModelFreeIdx = -1;
        fldNpcCamTarget = 0;
    }
}

INCLUDE_ASM(const s32, "field/fld_Npc", fldNpcCalc);

INCLUDE_ASM(const s32, "field/fld_Npc", fldNpcDraw);

void fldNpcHojiRefresh(void) {
    s32 i;

    for (i = 0; i < fldNpcModelTable; i++) {
        s32 room = fldModelSlotTable[i].room;
        if (room != 0 && fldGbChkNpc(fldGlobalBase.area, fldGlobalBase.floor + 1, room) != 0) {
            if (fldModelSlotTable[i].object != 0) {
                dds3ObjectBaseOnFlag(fldModelSlotTable[i].object, 1);
            }
        }
    }
}

void fldNpcLightLoad(s32 index) {
    char path[0x40];
    char directory[0x20];
    s32 fd;

    if (index < 100) {
        fldMakeFilePath(directory, index, 1);
        if (index == 0x18 && evtBitCheck(0x4F) != 0) {
            sprintf(path, D_00532790, directory, 0x18);
        } else {
            sprintf(path, D_005327A0, directory, index);
        }
        fd = sdfFileOpen(path);
        sdfFileRead(fd, D_003129D0, 0x100);
        sdfFileClose(fd);
    }
}

void fldGetEAEntry(const fldNpcPalette_t *src) {
    memcpy(D_003129D0, src, sizeof(*src));
}

INCLUDE_RODATA(const s32, "field/fld_Npc", D_00532790);

INCLUDE_RODATA(const s32, "field/fld_Npc", D_005327A0);

