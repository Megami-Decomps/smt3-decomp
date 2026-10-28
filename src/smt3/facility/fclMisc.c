#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

void fclFreeStage(void);

extern s32 dds3GetProcessStat(void *id);

extern u32 dds3GetRandA(void *name);

extern s32 fclSetWorkValue(s8 index, s32 value);

s32 fclGetFlag(s8 index);

void fclClearFlag(s8 index);

extern s8 gSelMsgPos;

extern s8 gMsgInvFlag;

extern s8 D_00551F80[8];

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

extern s32 gMsgID;

extern s32 gSelMsgNo;

extern s8 fclEndMessageEx(s8 Mode);

extern void evtFreeStage(void);

extern s32 sdfDelayedResrcCheck(void);

extern void fclFilterOFF(void);

extern void sdfFileCmdReadFile(const char *aFileName, s32 aBuffer, s32 aLength, void *aCallback, void *aUserData);

extern void itfMesMngSetVarImmediate(s32 id, s8 index, s32 pstr);

extern s32 itfMesMngGetSelectNo(s32 msgid);

extern s32 itfMesMngGetSelPhase(s32 msgid);

extern s32 fclWorkValue[16];

extern s32 scrGetIntPara(s32 n);

extern void scrSetIntReturnValue(s32 i);

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

extern s32 datAddItem(s32 id, s32 num);

extern s32 dds3KillProcess(s32 id, s32 shutdowntimer);

extern s32 fclChkMessage(void);

extern void fclScriptForcedEnd(u8 *PID);

extern s32 fclBlendColor(s32 color, s32 mask, s32 alpha);

extern s8 gMsgFlag;

extern void itfMesMngRelease(s32 id);

extern void *dds3GetControlScene(void);

extern void *dds3SceneBasicSearchNameAndType(void *scene, s32 type, char *name);

void fclInitPanelPos(s32 X, s32 Y, s32 Z, s16 W, s16 H, s32 Work, fclPanel_t *pPanelPos);

extern void shuffleIndices(s8 *order, s32 count, s32 rounds);

extern s32 cmpItemOrder(u8 *a, u8 *b);

extern s32 fclScriptStart(s32 pScriptData, s32 StartNo, void *pPID);

extern s32 datGetItemSlotIndex(s32 id);

extern void datAddHearts(s32 id);

s32 fclAddItem(u16 ItemID, s32 Nums)
{
    s32 slot = datGetItemSlotIndex(ItemID);

    if (slot != 0) {
        datAddHearts(slot);
    } else {
        datAddItem(ItemID, Nums);
    }
    return 1;
}

typedef struct
{
    /* 0x00 */ u8 unk_00[0x18];
    /* 0x18 */ u8 *unk_18;
} fclSceneObj_t;

s32 fcl_213998(char *name, fclSceneObj_t *dst)
{
    fclSceneObj_t *obj = dds3SceneBasicSearchNameAndType(dds3GetControlScene(), 9, name);

    if (obj == NULL) {
        return 0;
    }
    *(s32 *)(dst->unk_18 + 0x80) = *(s32 *)(obj->unk_18 + 0x78);
    return 1;
}

extern s32 dds3GetMapNo(void *scene);

extern void evtLoadArea(s32 fld, s32 area);

void fclLoadStage(s32 MajorNo, s32 MinorNo)
{
    s32 id = (MajorNo << 16) + MinorNo;

    if (dds3GetMapNo(dds3GetControlScene()) == id) {
        return;
    }
    evtLoadArea(MajorNo, MinorNo);
}

extern void dds3SetBlur(s32 frame, s32 alpha, s32 scale);

extern void dds3MultiBlurOff(s32 frame);

extern void dds3DistBlurOff(s32 frame);

extern void Eve_BlurOFF(void);

extern void Eve_PartialMultiBlurOFF(void);

extern void Eve_PartialMultiFilterOFF(void);

extern void Eve_DistortionBlurOFF(void);

extern void Eve_FilterOFF(void);

void fclFilterOFF(void)
{
    dds3SetBlur(0, 0, 0);
    dds3MultiBlurOff(0);
    dds3DistBlurOff(0);
    Eve_BlurOFF();
    Eve_PartialMultiBlurOFF();
    Eve_PartialMultiFilterOFF();
    Eve_DistortionBlurOFF();
    Eve_FilterOFF();
}

void fclFreeStage(void)
{
    evtFreeStage();
    while (sdfDelayedResrcCheck() != 0) {
    }
    fclFilterOFF();
}

void fclInitPanelPos(s32 X, s32 Y, s32 Z, s16 W, s16 H, s32 Work, fclPanel_t *pPanelPos)
{
    pPanelPos->X = X;
    pPanelPos->Y = Y;
    pPanelPos->Z = Z;
    pPanelPos->W = W;
    pPanelPos->H = H;
    pPanelPos->Col = Work;
}

void fclScriptForcedEnd(u8 *PID)
{
    if (PID != NULL && dds3GetProcessStat(PID) != 0) {
        dds3KillProcess(PID, 0);
        PID = NULL;
    }
}

extern s32 scrStartScript2(s32 priority, s32 scrdata, s32 startproc);

s32 fclScriptStart(s32 pScriptData, s32 StartNo, void *pPID)
{
    s32 handle;

    if (pPID != NULL)
    {
        fclScriptForcedEnd(*(u8 **)pPID);
    }
    handle = scrStartScript2(0x7D0, pScriptData, StartNo);
    fclClearFlag(0);
    if (pPID != NULL)
    {
        *(s32 *)pPID = handle;
    }
    return handle;
}

typedef struct
{
    /* 0x00 */ u8 count;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u16 item[0xA0];
} fclItemList_t;

void func_00213BB8(fclItemList_t *list)
{
    s32 item;

    list->count = 0;
    for (item = 1; item < 0xA0; item++) {
        if (((u8 *)item + (u32)cmbGlobalWork)[0x1928] != 0) {
            list->item[list->count++] = item;
        }
    }
}

INCLUDE_ASM(const s32, "facility/fclMisc", fclCreateItemListEx);

s32 cmpItemOrder(u8 *a, u8 *b)
{
    u8 va = *a;
    u8 vb = *b;

    if (va > vb) {
        return 1;
    }
    if (va < vb) {
        return -1;
    }
    return 0;
}

s32 func_00213CD8(u8 *list, s32 count, u8 value)
{
    s32 kept = 0;
    s32 i;

    for (i = 0; i < count; i++) {
        if (list[i] != value) {
            u8 entry = list[i];

            list[i] = 0;
            list[kept] = entry;
            kept++;
        }
    }
    return kept;
}

void shuffleIndices(s8 *order, s32 count, s32 rounds)
{
    s32 n;

    if (rounds <= 0) {
        return;
    }
    n = rounds;
    while (n != 0) {
        s32 first = (u32)dds3GetRandA(0) % (u32)count;
        s32 second = (u32)dds3GetRandA(0) % (u32)count;
        u8 value = order[first];

        order[first] = order[second];
        order[second] = value;
        n--;
    }
}

extern char *text2[];

char *func_00213DC0(datUnitWork_t *unit)
{
    char *name = unit->namecode;

    if (name[0] != 0) {
        return name;
    }
    return text2[unit->id];
}

INCLUDE_ASM(const s32, "facility/fclMisc", func_00213DF0);

INCLUDE_ASM(const s32, "facility/fclMisc", fclBlendColor);

extern s32 itfMesMngInitialize(void);

extern void itfMesMngChangeWindowType(s32 id, s32 type, s32 alpha);

s32 fclInitMessage(void)
{
    s32 ret;

    if (gMsgID < 0) {
        gMsgID = itfMesMngInitialize();
        itfMesMngChangeWindowType(gMsgID, 2, 0);
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

extern void itfMesMngMarkBitStat(s32 id, s32 flag);

extern void itfMesMngRequestMessage(s32 msg, s32 id, s32 flag);

extern void itfMesMngSetSelPhase(s32 msgid, s32 phase);

s32 fclStartMessage(s32 MsgNo)
{
    if (gMsgID < 0)
    {
        return 0;
    }
    itfMesMngMarkBitStat(gMsgID, 0x200000);
    itfMesMngRequestMessage(gMsgID, MsgNo, 0);
    itfMesMngSetSelPhase(gMsgID, -1);
    gMsgFlag = 1;
    return 1;
}

s32 fclStartSelMessage(s32 SelMsgNo)
{
    if (gMsgID < 0) {
        return 0;
    }
    gSelMsgNo = SelMsgNo;
    return 1;
}

void fclSetSelMessagePos(s8 SelPos)
{
    if (gMsgID >= 0) {
        gSelMsgPos = SelPos;
    }
}

s8 fclGetCurrentSelMessagePos(void)
{
    return gSelMsgPos;
}

s8 fclGetSelMessagePos(void)
{
    if (gMsgID < 0) {
        return -1;
    }
    return itfMesMngGetSelectNo(gMsgID);
}

extern void itfMesMngSetPhase(s32 msgid, s32 phase);

extern void itfMesMngMsgWndCls(s32 msgid);

extern void itfMesMngRequestMessageRelease(s32 id, s32 cls_speaker);

extern void fclSetMessageInvalid(s8 Mode);

s8 fclEndMessageEx(s8 Mode)
{
    if (gMsgID < 0) {
        return 0;
    }
    itfMesMngSetPhase(gMsgID, 0);
    if (Mode != 0) {
        itfMesMngMsgWndCls(gMsgID);
    }
    itfMesMngRequestMessageRelease(gMsgID, 0);
    fclSetMessageInvalid(1);
    gMsgFlag = 0;
    return 1;
}

s8 fclEndMessage(void)
{
    return fclEndMessageEx(1);
}

s32 fclDestroyMessage(void)
{
    if (gMsgID < 0) {
        return 0;
    }
    itfMesMngRelease(gMsgID);
    gMsgID = -1;
    gMsgFlag = 0;
    gMsgInvFlag = 0;
    return 1;
}

s32 fclChkMessage(void)
{
    if (gMsgID < 0) {
        return 0;
    }
    if (gMsgInvFlag != 0 && gMsgFlag == 2) {
        return 0;
    }
    return (s8)(u8)gMsgFlag;
}

s32 fclChkSelMessage(void)
{
    if (gMsgID < 0) {
        return 0;
    }
    return itfMesMngGetSelPhase(gMsgID) >= 0;
}

INCLUDE_ASM(const s32, "facility/fclMisc", fclUpdateMessage);

void fclSetMessageVar_20743088(s8 Index, s32 pStr)
{
    itfMesMngSetVarImmediate(gMsgID, Index, pStr);
}

s8 fcl_214270(void)
{
    return gMsgInvFlag;
}

extern void itfMesMngClearBitStat(s32 id, s32 flag);

void fclSetMessageInvalid(s8 Mode)
{
    if (Mode != 0) {
        itfMesMngClearBitStat(gMsgID, 0x800000);
        itfMesMngClearBitStat(gMsgID, 0x100000);
        gMsgInvFlag = 0;
        itfMesMngSetPhase(gMsgID, 1);
        gMsgFlag = 1;
    } else {
        itfMesMngMarkBitStat(gMsgID, 0x800000);
        itfMesMngMarkBitStat(gMsgID, 0x100000);
        gMsgInvFlag = 1;
    }
}

extern void itfMesMngSetMesPosition(s32 id, s32 x, s32 y);

extern void itfMesMngSetOffsetY1(s32 msgid, s32 y1);

void fcl_2142F0(s32 x, s32 y)
{
    itfMesMngSetMesPosition(gMsgID, x * 16, y * 8);
    itfMesMngSetOffsetY1(gMsgID, -((0x15F - y) * 8));
}

s32 fclChkMessageDraw(void *PID)
{
    if (dds3GetProcessStat(PID) == 1)
    {
        return 1;
    }
    if (dds3GetProcessStat(PID) == 2)
    {
        return 1;
    }
    return dds3GetProcessStat(PID) == 3;
}

void fclClearFlag(s8 index)
{
    D_00551F80[index] = 0;
}

s32 fclGetFlag(s8 index)
{
    s8 flag = D_00551F80[index];

    if (flag != 0) {
        return 1;
    }
    return 0;
}

s32 fclSetWorkValue(s8 index, s32 value)
{
    if (index >= 0x10) {
        return 0;
    }
    fclWorkValue[index] = value;
    return 1;
}

s8 fclGetWorkValue(s8 index)
{
    if (index >= 0x10) {
        index = 0xF;
    }
    return fclWorkValue[index];
}

s32 fcl_214428(void)
{
    D_00551F80[scrGetIntPara(0)] = 1;
    return 1;
}

s32 fcl_214458(void)
{
    s32 index = scrGetIntPara(0);

    if (index >= 0x10) {
        index = 0xF;
    }
    scrSetIntReturnValue(fclWorkValue[index]);
    return 1;
}

s32 fclLoadFileDone(s32 unused, s32 cmd, s32 value, s32 unused2, s32 *out)
{
    if (cmd == 8) {
        *out = value;
    }
    return 0;
}

void fclLoadFileAsync(const char *name, void *buf)
{
    sdfFileCmdReadFile(name, 0, -1, fclLoadFileDone, buf);
}
INCLUDE_SDATA(const s32, "facility/fclMisc", gMsgID);

INCLUDE_SDATA(const s32, "facility/fclMisc", gMsgFlag);

INCLUDE_SDATA(const s32, "facility/fclMisc", gMsgInvFlag);

INCLUDE_SDATA(const s32, "facility/fclMisc", gSelMsgNo);

INCLUDE_SDATA(const s32, "facility/fclMisc", gSelMsgPos);

INCLUDE_SDATA(const s32, "facility/fclMisc", D_0054FC5D);

INCLUDE_SDATA(const s32, "facility/fclMisc", D_0054FC60);

INCLUDE_SDATA(const s32, "facility/fclMisc", D_0054FC68);

INCLUDE_SDATA(const s32, "facility/fclMisc", D_0054FC70);

INCLUDE_SDATA(const s32, "facility/fclMisc", D_0054FC78);

INCLUDE_SDATA(const s32, "facility/fclMisc", D_0054FC80);

INCLUDE_SDATA(const s32, "facility/fclMisc", D_0054FC88);

INCLUDE_SDATA(const s32, "facility/fclMisc", D_0054FC90);

INCLUDE_SDATA(const s32, "facility/fclMisc", D_0054FC98);

INCLUDE_SDATA(const s32, "facility/fclMisc", D_0054FCA0);

