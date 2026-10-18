#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "scr.h"

#include "model.h"
#include "draw3d.h"
#include "ps2asm.h"

/* EE GCC promotes %f arguments through SMT3's software conversion helper. */
__asm__("fptodp = sdfFloatToULong");

typedef struct { s32 x, y, z; u32 style; } pbPrintState_t;
typedef struct { u8 unk00[8]; f32 degrees; u8 unk0C[4]; } pbFovWork_t;
typedef struct
{
    s16 editing, selection;
    u32 blink;
    u8 unk08[8];
} pbPanelEditor_t;
typedef struct { const char *title; s32 highlight; } pbColorRow_t;
typedef struct { u8 value; u8 unk01[3]; } pbColorByte_t;
typedef union
{
    struct { s32 red, green, blue, alpha; };
    s32 channels[4];
    pbColorByte_t channelBytes[4];
} pbColor_t;
typedef struct pbPanel_t pbPanel_t;
typedef struct { s32 value; } pbPanelValue_t;
typedef struct { s32 code, unk04, timer; } pbErrorWork_t;
typedef struct pbGroupDB_t pbGroupDB_t;
typedef struct
{
    s32 dispStat;
    s16 cellX, cellY;
    s32 rot;
    u8 unk0C[4];
    sdfVector4_t pos;
    union { s32 field; mdlObj_t *model; };
} pbMyCharacter_t;
typedef struct { s32 format, type, free, result; } pbMCardInfo_t;
typedef struct { s16 number, label; } pbStageEntry_t;
typedef struct
{
    s32 pcharNum;
    u8 unk004[0x14];
    s16 exitX, exitY;
    u8 unk01C[0x320];
    s32 charIndex;
    void *baseTex;
    void *tex344, *tex348, *tex34C;
    sdfMemHandle_t *mapData;
    u8 *giveupScript;
    s16 level, stage;
    u8 unk35C[4];
    s32 menuState;
    struct pbPanel_t *helpProc;
} pbMapWork_t;
typedef struct { u8 unk00[4]; u16 group; } pbGroupEntry_t;
struct pbGroupDB_t { u32 count; pbGroupEntry_t *entry[1]; };
typedef struct
{
    s32 unk00;
    union { s32 value; struct { s16 unk04, unk06; }; };
    sdfMat_t *mat;
} pbMatWork_t;
typedef struct { f32 scale; u8 unk04[0xC]; sdfVector4_t eye, target; } pbViewCamWork_t;
typedef struct { u8 unk00[8]; s32 modelIndex; } pbPanelModel_t;
typedef struct { s32 value; u8 unk04[0xC0]; f32 x, y, z; } pbBlockObj_t;
typedef struct { s16 index; u16 step; u8 unk04[4]; pbBlockObj_t *block; } pbRoundBlock_t;
typedef struct { sdfVector4_t v[4]; } pbMatrix_t;
typedef struct { u32 count; pbGroupEntry_t *entry[0x40]; sdfMat_t *mat; } pbBlockDB_t;
typedef struct { s16 x, y; } pbGoalPos_t;
typedef struct { u8 unk00[8]; pbGoalPos_t pos; } pbGoal_t;
typedef struct { u8 flag[4]; void *block, *floor, *chara; } pbRecordSlot_t;
typedef struct { s32 pos; pbRecordSlot_t *slot; } pbRecord_t;
typedef struct
{
    s16 clearBit;
    s16 unk02;
    pbStageEntry_t stage[1];
} pbLevel_t;
/* Common panel work header: its constructor installs process and data links. */
struct pbPanel_t
{
    dds3ProcessID_t *process;
    union { u32 flags; struct { u16 unk04, state; }; };
    s16 x, y;
    union { pbPanelValue_t *value; pbGroupDB_t *groupDB; pbMyCharacter_t *chara;
            pbRecord_t *record; pbMatWork_t *mat; pbBlockDB_t *blockDB;
            pbPanelModel_t *model; pbRoundBlock_t *round; pbErrorWork_t *error;
            pbViewCamWork_t *viewCam; } data;
};
typedef struct { sdfMemHandle_t *allocation; u16 unk04, unk06, unk08, level, stage; } pbDebugWork_t;

/* The native FOV workspace is an unnamed view at pinst + 0xA34. Bind its
 * assembler address without redefining pinst's message-instance array type
 * or inventing an overlapping allocation in the generated BSS split. */
PS2_SYMBOL_AT(pbFovWork_t, pbDebugFovWork, pinst, 0xA34);
extern pbPanelEditor_t pbDebugPanel2Pos, pbDebugPanel3X;
extern pbColorRow_t D_00326C20[6];
extern pbColor_t D_00326C50;
extern sdf3DScreenParam_t dds3ScreenParam;
extern const char *D_00326C08[6];
extern u8 dds3ScreenBlurFlag;
extern s16 dds3ScreenBlurAlpha, dds3ScreenBlurScale;
extern s32 dds3ScreenBlurOfsX[2]; /* Native adjacent X/Y offset pair. */
extern s32 D_00551EA8, pbDebugPanel3State;
extern u8 D_0054F584;
extern s32 pbMoveChar;
extern f32 cameraAngle, cameraRadius, cameraDegree, cameraDegreeInit;
extern u8 keyActMode;
extern dds3ProcessID_t *scr_msg;
extern pbDebugWork_t *g_pdebug_0;
extern void *dds3FontReqAllocDrawTag(void);
extern void *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);
extern void sdfDrawTagLink(sdfDrawTag_t *, u32);
extern void sdfDevSetPrintCB(pbPrintState_t *, s32, s32, s32, s32);
extern void *sdfDevPrintfEx(void *, const char *, ...);
extern s32 printf(const char *, ...);
extern f32 sdfSin(f32 angle);
extern void mdlSetPosition(mdlObj_t *model);
extern void mdlSetRotate(mdlObj_t *model);
extern void FromEuler(void);
extern void sdfChipFreeDelayedDirect(void *ptr);
extern void sdfChipFreeDirect(void *ptr);
extern void sdfMemFreeDirect(sdfMemHandle_t *handle);
extern u32 pbMapIsValid(void);
extern s32 sceMcGetInfo(s32 port, s32 slot, s32 *type, s32 *free, s32 *format);
extern pbLevel_t *pbLevelTable[];
extern u8 keyAct[];
extern s8 pbLoadState;
extern pbMapWork_t *D_0054F5C4;
extern pbGoal_t *goal;
extern pbPanel_t *record;
extern void sdfMatFreeDelayed(sdfMat_t *mat);
extern sdfMat_t *sdfMatNew(void);
extern void sdfMatSetTex(sdfMat_t *mat, void *tex);
extern void dds3ResetProcessFlag(dds3ProcessID_t *id, u32 mask, s32 value);
extern void mdlFree(mdlObj_t *model);
extern void pbGotoNewStage(void);
extern void pbBuildMapBaseVerts(pbPanel_t *panel);
typedef void (*pbCharacterFunc_t)(void);
extern pbCharacterFunc_t pbCharacterTable[];
extern pbPanel_t *blockDB;
extern u32 current_char;
extern char pbMapPathBuf[];
extern f32 D_00327370[];
extern sdfVector4_t D_0056CB30[], D_0056C8F0[], D_00326C70[], D_00326CD0[];
extern u8 D_00326D50[];
extern char D_00535E40[], D_00535E68[];
extern s32 pbPopRecord(pbPanel_t *record, pbPanel_t **chars, pbPanel_t *db);
extern pbPanel_t *pbStartErrorDraw(pbPanel_t *parent, s32 code, s32 timer);
extern pbPanel_t *pbWaitLoadMapData(pbPanel_t *parent);
extern void *sdfChipAllocDirect(u32 size);
extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *child);
extern void dds3SetProcessData(dds3ProcessID_t *id, void *data);
extern s32 pb_draw_bg_Process(dds3ProcessID_t *id);
extern void pb_draw_bg_Shutdown(dds3ProcessID_t *id);
extern s32 pb_load_mapdata_Process(dds3ProcessID_t *id);
extern void pb_load_mapdata_Shutdown(dds3ProcessID_t *id);
extern s32 pb_proc_dummy_Process(dds3ProcessID_t *id);
extern void pb_proc_dummy_Shutdown(dds3ProcessID_t *id);
extern s32 ClearProc(dds3ProcessID_t *id);
extern void DeleteClearProc(dds3ProcessID_t *id);
extern void rsetOtOfs(s32 ofs);
extern void rsetAlpha(s32 mode);
extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void DrwSprite2D(s32, s32, s32, s32, s32, s32, s32, s32, u32, void *);
extern u8 pbMapPathIdx, pbMenuIndex, pbMenuState;
extern void *memset(void *dst, s32 c, u32 n);
extern s32 pb_draw_map_cursor_Process(dds3ProcessID_t *id);
extern void pb_draw_map_cursor_Shutdown(dds3ProcessID_t *id);
extern s32 pb_draw_error_Process(dds3ProcessID_t *id);
extern void pb_draw_error_Shutdown(dds3ProcessID_t *id);
extern s32 pb_proc_record_Process(dds3ProcessID_t *id);
extern void pb_proc_record_Shutdown(dds3ProcessID_t *id);
extern s32 pb_draw_help_Process(dds3ProcessID_t *id);
extern void pb_draw_help_Shutdown(dds3ProcessID_t *id);
extern s32 pb_proc_menu_Process(dds3ProcessID_t *id);
extern void pb_proc_menu_Shutdown(dds3ProcessID_t *id);
extern char D_005361B0[], D_005361C8[], D_005365D8[], D_00536398[], D_00535EB8[];
extern char D_00536040[], D_00535CE8[], D_00535CD8[], D_00535FA8[];
extern s32 pb_proc_block_DB_Process(dds3ProcessID_t *id);
extern void pb_proc_block_DB_Shutdown(dds3ProcessID_t *id);
extern s32 pb_draw_mapbase_Process(dds3ProcessID_t *id);
extern void pb_draw_mapbase_Shutdown(dds3ProcessID_t *id);
extern s32 pb_draw_exit_Process(dds3ProcessID_t *id);
extern void pb_draw_exit_Shutdown(dds3ProcessID_t *id);
extern s32 viewModeCamera(dds3ProcessID_t *id);
extern void deleteViewModeCamera(dds3ProcessID_t *id);
extern void sdfMatSetDiffuse(sdfMat_t *mat, u32 color);
extern void pbInitMapBaseMat(pbPanel_t *panel);
extern char D_00535E00[], D_00535E10[], D_00535E20[], D_00535EC8[], D_00535FC8[];
extern void updateMotion(void);
extern void dds3FadeOut(s32, s32, s32, s32);
extern s32 sprintf(char *buf, const char *fmt, ...);
extern void sdfFileCmdReadFile(char *path, void *dest, s32 size, void *done, s32 arg);
extern s32 pbLoadSystemDataDone(s32 handle, s32 status);
extern void pbFreeRecord(pbRecordSlot_t *slot);
extern void pbFreeGroupHandles(pbBlockDB_t *db);
extern void pbFreeGroupEntry(mdlResrc_t *entry);
extern s32 pbGetMoveChar(void);
extern void dds3AdminiCallSequence(s32, s32, s32, s32);
extern sdfMemHandle_t *sdfMemAllocDirect(u32 size);
extern void *sdfMemLock(sdfMemHandle_t *handle);
extern void fclClearFlag(s32 flag);
extern dds3ProcessID_t *dds3SearchProcessName(char *name);
extern void dds3SetProcessFlag(dds3ProcessID_t *id, u32 mask, s32 value);
extern void setRoundBlockData(pbBlockObj_t *block, s16 index);
extern s32 pbStartAutoRoundCamera(s32, s32);
extern void pbSetModelRotate(mdlObj_t *model, f32 x, f32 y, f32 z);
extern void SetRotateMatrixXYZ(sdfVector4_t *rot);
extern void StoreMatrix(pbMatrix_t *m);
extern void LoadMatrix(pbMatrix_t *m);
extern void sdf3DPrimLinkTm(void *prim, s32 mode);
extern void func_001D2D00(pbPanel_t *panel, s32 arg);
extern pbPanel_t *mychar[];
extern s32 moveB, roundCamera;
extern f32 D_00326C60[];
extern char D_00535EF8[];
extern char D_00535DE0[];
extern char D_00535E90[];
extern u32 pbPushRecord(pbPanel_t *panel);
extern u8 degree_mode;
extern sdfMemHandle_t *a_key;
extern void p2sdStopAllSe(void);
extern void fclEncycFreeEntries(char *name);
extern void fldProcNextJump(void);
extern u32 pbMyCharacterDispStat(pbPanel_t *panel);
extern dds3ProcessID_t *pbStartGiveupConfirm(void);
extern s8 pbCheckPopRecord(pbPanel_t *panel);
extern s8 pbCheckPreviousRecord(void);
extern u8 pbRotLKeyAct(void);
extern u8 pbRotRKeyAct(void);
extern void pbClearRecord(pbPanel_t *panel);
extern s32 moveViewCamera;
extern sdfVector4_t aViewEye, aViewTarget, pbGameCameraParam, pbCameraVec;
extern sdfVector4_t pbInitCameraParam, pbCameraVec2;
extern void pbSetGameCamera1(void);
extern void pbSetGameCamera2(void);
extern void sceMcMkdir(s32 port, s32 slot, const char *name);
extern void sceMcChdir(s32 port, s32 slot, const char *newdir, char *currentdir);
extern void sceMcGetDir(s32 port, s32 slot, const char *name, u32 mode, s32 maxent, void *table);
extern void sceMcSync(s32 mode, s32 *cmd, s32 *result);
extern void pbPreviousRecord(void);
extern void *pbFindGroupEntry(pbGroupDB_t *db, u32 group);
extern pbPanel_t *pbStartHelp(pbPanel_t *parent, s32 page);
void ReleaseDebug_pbDebugMode(void);
s32 pbCalcFovDegrees(f32 *, f32, f32, f32, f32);
void pbDrawRotatedPrim(s32 otIndex);

extern char D_0054F530[];
extern char D_0054F538[];
extern char D_0054F540[];
extern char D_0054F548[];
extern char D_0054F550[];
extern char D_0054F558[];
extern char D_0054F588[];
extern char D_0054F590[];
extern char D_0054F598[];
extern char D_0054F5A0[];
extern char D_0054F5A8[];

typedef void (*sdfGrOtRegFunc_t)(sdfGrOt_t *ot, sdfDrawTag_t *tag);

typedef struct
{
    sdfGifTag_t giftag;
    sdfGrTagStdReg_t reg[2];
} pbRegPacket_t;

extern char D_00535BB0[];
extern char D_00535BC0[];
extern char *D_00326BC8[];
extern s32 D_00326BD8[4];
extern s32 D_00326BE8[4];
extern s32 D_00326BF8[4];
extern u8 scrEmbedded_3267A8[];
extern sdfGrOt_t sdfGrOtTable[];
extern u8 dds3PadButtonState[];

extern u32 dds3BackColor;
extern s32 D_0054F510;
extern void *D_0054F514;

extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern dds3ProcessID_t *dds3StartProcess(char *namestr, u32 priority, s32 starttimer,
                                         s32 shutdowntimer, dds3ProcessFunc_t processfunc,
                                         dds3ProcessFunc_t shutdownfunc, void *data);
extern dds3ProcessID_t *scrStartScript2(s32 handle, u8 *script, s32 arg);
extern void itfMesMngSetKutiCallback(s32 meshandle, void *func);
extern void itfDebugPrintMark(void);
extern void sdfTexFreeDelayed(void *tex);
extern void *itfMiscLoadTexture(char *name);
extern void *sdfGrAllocPacketBuff(u32 size);
extern void *sdf2DMakeStdPacket(void *buff, u32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern void itfMiscDrawAlphaPrim(sdfDrawTag_t *tag, s32 mode, s32 value);
extern void itfMiscDrawSpritePrim(s32 *rect, s32 *uv, s32 *color, s32 z, void *tex, s32 flag,
                                  sdfDrawTag_t *tag);

void TestMsgReloadTexture(void);
s32 TestMsgMngC_Process(dds3ProcessID_t *id);
s32 TestMsgMngD_Process(dds3ProcessID_t *id);

void itfDebugMesMngInitialize(void)
{
    dds3BackColor = 0x80FFFFFF;
    TestMsgReloadTexture();
    itfMesMngSetKutiCallback(
        ((scrProcessWork_s *)dds3GetProcessData(scrStartScript2(0x3E8, scrEmbedded_3267A8, 0)))
            ->MesHandle,
        itfDebugPrintMark);
    dds3StartProcess(D_00535BB0, 0x3EF, 0, 0, TestMsgMngC_Process, NULL, NULL);
    dds3StartProcess(D_00535BC0, 0x2AFE, 0, 0, TestMsgMngD_Process, NULL, NULL);
}

void TestMsgReloadTexture(void)
{
    if (D_0054F514 != NULL) {
        sdfTexFreeDelayed(D_0054F514);
        D_0054F514 = NULL;
    }
    D_0054F510 = (D_0054F510 + 1) & 3;
    if (D_0054F510 != 3) {
        D_0054F514 = itfMiscLoadTexture(D_00326BC8[D_0054F510]);
    }
}

s32 TestMsgMngC_Process(dds3ProcessID_t *id)
{
    if ((s8)dds3PadButtonState[0x20] < 0) {
        TestMsgReloadTexture();
    }
    return 0;
}

s32 TestMsgMngD_Process(dds3ProcessID_t *id)
{
    sdfDrawTag_t *tag;
    sdfGrOt_t *ot;

    if ((D_0054F514 != NULL) && (D_0054F510 != 3)) {
        tag = sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(tag);
        itfMiscDrawAlphaPrim(tag, 0, 0);
        itfMiscDrawSpritePrim(D_00326BD8, D_00326BE8, D_00326BF8, 0xFFF, D_0054F514, 0, tag);
        ot = &sdfGrOtTable[71];
        ((sdfGrOtRegFunc_t)ot->regFunc)(ot, tag);
        return 0;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "code_001D1470", D_00535BB0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535BC0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535BD0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535BE8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535C00);

void itfDebugPrintMark(void) {
    printf("********* AAAA ********\n");
}

s32 pbDebugPrintPanel(void) {
    pbPrintState_t packet;
    sdfDrawTag_t *list;
    f32 radiansToDegrees = 57.2957795f;

    list = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(list,
                    (u32)dds3PrimFrameRect(0x8500, 0x79C0, 0xFEFFFF,
                                  0xA80, 0x120, 0x60000000, 0x40806020));
    sdfDevSetPrintCB(&packet, 0x85C0, 0x7A20, 0xFF0000, 0);
    sdfDrawTagLink(list,
                    (u32)sdfDevPrintfEx(&packet, "FOVY: %6.2f",
                        dds3ScreenParam.fovy * radiansToDegrees));
    { sdfGrOt_t *ot = &sdfGrOtTable[81]; ((sdfGrOtRegFunc_t)ot->regFunc)(ot, list); }
    if (((s8)dds3PadButtonState[0x33]) < 0) {
        return -1;
    }
    pbDebugFovWork.degrees = dds3ScreenParam.fovy * radiansToDegrees;
    if (pbCalcFovDegrees(&pbDebugFovWork.degrees, 1.0f, 89.0f, 0.1f, 1.0f) != 0) {
        dds3ScreenParam.fovy = pbDebugFovWork.degrees * 0.0174532925f;
    }
    return 0;
}

s32 pbCalcFovDegrees(f32 *value, f32 minimum, f32 maximum, f32 coarseStep, f32 fineStep)
{
    f32 current = *value;
    s32 changed = 0;

    if ((s8)dds3PadButtonState[0x35] < 0) {
        if (current < maximum) {
            current += coarseStep;
        } else {
            current = minimum;
        }
        changed = 1;
    } else if (dds3PadButtonState[0x35] & 2) {
        if (current < maximum) {
            current += coarseStep;
            changed = 1;
        }
    } else if ((s8)dds3PadButtonState[0x34] < 0) {
        if (minimum < current) {
            current -= coarseStep;
        } else {
            current = maximum;
        }
        changed = 1;
    } else if (dds3PadButtonState[0x34] & 2) {
        if (minimum < current) {
            current -= coarseStep;
            changed = 1;
        }
    } else if (fineStep != 0.0f) {
        if ((s8)dds3PadButtonState[0x3A] < 0) {
            if (current < maximum) {
                current += fineStep;
                if (maximum < current) {
                    current = maximum;
                }
            } else {
                current = minimum;
            }
            changed = 1;
        } else if (dds3PadButtonState[0x3A] & 2) {
            if (current < maximum) {
                current += fineStep;
                if (maximum < current) {
                    current = maximum;
                }
                changed = 1;
            }
        } else if ((s8)dds3PadButtonState[0x38] < 0) {
            if (minimum < current) {
                current -= fineStep;
                if (current < minimum) {
                    current = minimum;
                }
            } else {
                current = maximum;
            }
            changed = 1;
        } else if (dds3PadButtonState[0x38] & 2) {
            if (minimum < current) {
                current -= fineStep;
                if (current < minimum) {
                    current = minimum;
                }
                changed = 1;
            }
        }
    }
    if (changed != 0) {
        *value = current;
        return 1;
    }
    return 0;
}

/* Draw the blur settings and handle selection, editing and cancellation. */
s32 pbDebugPrintPanel2(void) {
    pbPrintState_t packet;
    sdfDrawTag_t *list;
    s32 row;
    s32 y;
    s16 previous;
    s32 highlight;

    list = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(list,
        (u32)dds3PrimFrameRect(0x8290, 0x79A8, 0xFEFFFF, 0xC60, 0x1B0, 0x60000000, 0x40806020));
    sdfDevSetPrintCB(&packet, 0x82C0, 0x79C0, 0xFF0000, 0);
    sdfDrawTagLink(list, (u32)sdfDevPrintfEx(&packet,
        "BLUR:        %s", dds3ScreenBlurFlag ? D_0054F530 : D_0054F538));

    y = 0x7A20;
    if (pbDebugPanel2Pos.editing == 0) {
        sdfDevSetPrintCB(&packet, 0x82C0, pbDebugPanel2Pos.selection * 0x60 + 0x7A20, 0xFF0000, 0);
        pbDebugPanel2Pos.blink++;
        if (pbDebugPanel2Pos.blink < 32 || (pbDebugPanel2Pos.blink & 31) < 18) {
            sdfDrawTagLink(list, (u32)sdfDevPrintfEx(&packet, D_0054F540));
        }
    }

    for (row = 0; row != 3; row++) {
        sdfDevSetPrintCB(&packet, 0x82C0, y, 0xFF0000, 0);
        sdfDrawTagLink(list,
            (u32)sdfDevPrintfEx(&packet, D_0054F548, D_00326C08[row]));
        highlight = 0;
        if (pbDebugPanel2Pos.editing != 0 && row == pbDebugPanel2Pos.selection) {
            highlight = 6;
        }
        sdfDevSetPrintCB(&packet, 0x8800, y, 0xFF0000, highlight);
        switch (row) {
        case 0:
            sdfDrawTagLink(list,
                (u32)sdfDevPrintfEx(&packet, D_0054F550, dds3ScreenBlurAlpha));
            break;
        case 1:
            sdfDrawTagLink(list,
                (u32)sdfDevPrintfEx(&packet, D_0054F550, dds3ScreenBlurScale));
            break;
        case 2:
            sdfDrawTagLink(list,
                (u32)sdfDevPrintfEx(&packet, D_0054F558, dds3ScreenBlurOfsX[0], dds3ScreenBlurOfsX[1]));
            break;
        }
        y += 0x60;
    }
    sdfDrawTagLink(list,
        (u32)dds3PrimFrameRect((dds3ScreenBlurOfsX[0] * 16) + 0x7FC0, (dds3ScreenBlurOfsX[1] * 8) + 0x7FE0,
            0xFF0000, 0x80, 0x40, 0x80008080, 0x80000000));
    { sdfGrOt_t *ot = &sdfGrOtTable[83]; ((sdfGrOtRegFunc_t)ot->regFunc)(ot, list); }

    if ((s8)((s8)dds3PadButtonState[0x32]) < 0) {
        dds3ScreenBlurFlag ^= 1;
    }
    if (pbDebugPanel2Pos.editing == 0) {
        previous = pbDebugPanel2Pos.selection;
        if ((u8)((s8)dds3PadButtonState[0x36]) & 2) {
            pbDebugPanel2Pos.selection--;
            if (pbDebugPanel2Pos.selection < 0) {
                pbDebugPanel2Pos.selection = 2;
            }
        }
        if ((u8)((s8)dds3PadButtonState[0x37]) & 2) {
            pbDebugPanel2Pos.selection++;
            if (pbDebugPanel2Pos.selection >= 3) {
                pbDebugPanel2Pos.selection = 0;
            }
        }
        if (previous != pbDebugPanel2Pos.selection) {
            pbDebugPanel2Pos.blink = 0;
        }
        if (((s8)dds3PadButtonState[0x31]) < 0) {
            pbDebugPanel2Pos.editing ^= 1;
            pbDebugPanel2Pos.blink = 0;
        }
        if ((u8)((s8)dds3PadButtonState[0x33]) & 2) {
            return -1;
        }
    } else {
        if (pbDebugPanel2Pos.selection == 2) {
            if (((s8)dds3PadButtonState[0x36]) != 0) {
                dds3ScreenBlurOfsX[1] -= 16;
                if (dds3ScreenBlurOfsX[1] < -224) {
                    dds3ScreenBlurOfsX[1] = -224;
                }
            }
            if (((s8)dds3PadButtonState[0x37]) != 0) {
                dds3ScreenBlurOfsX[1] += 16;
                if (dds3ScreenBlurOfsX[1] > 224) {
                    dds3ScreenBlurOfsX[1] = 224;
                }
            }
            if (((s8)dds3PadButtonState[0x34]) != 0) {
                dds3ScreenBlurOfsX[0] -= 16;
                if (dds3ScreenBlurOfsX[0] < -256) {
                    dds3ScreenBlurOfsX[0] = -256;
                }
            }
            if (((s8)dds3PadButtonState[0x35]) != 0) {
                dds3ScreenBlurOfsX[0] += 16;
                if (dds3ScreenBlurOfsX[0] > 256) {
                    dds3ScreenBlurOfsX[0] = 256;
                }
            }
        } else if (pbDebugPanel2Pos.selection == 0) {
            if ((u8)((s8)dds3PadButtonState[0x34]) & 2) {
                dds3ScreenBlurAlpha -= 10;
                if (dds3ScreenBlurAlpha < 0) {
                    dds3ScreenBlurAlpha = 0;
                }
            }
            if ((u8)((s8)dds3PadButtonState[0x35]) & 2) {
                dds3ScreenBlurAlpha += 10;
                if (dds3ScreenBlurAlpha > 255) {
                    dds3ScreenBlurAlpha = 255;
                }
            }
        } else {
            if ((u8)((s8)dds3PadButtonState[0x34]) & 2) {
                dds3ScreenBlurScale -= 10;
                if (dds3ScreenBlurScale < -255) {
                    dds3ScreenBlurScale = -255;
                }
            }
            if ((u8)((s8)dds3PadButtonState[0x35]) & 2) {
                dds3ScreenBlurScale += 10;
                if (dds3ScreenBlurScale > 255) {
                    dds3ScreenBlurScale = 255;
                }
            }
        }
        if (((s8)dds3PadButtonState[0x33]) < 0) {
            pbDebugPanel2Pos.editing ^= 1;
            pbDebugPanel2Pos.blink = 0;
        }
    }
    return 0;
}

/* Draw the four color channels and handle selection and value editing. */
s32 pbDebugPrintPanel3(void) {
    pbPrintState_t packet;
    sdfDrawTag_t *list;
    s32 row;
    s32 y = 0x7A20;
    s16 previousSelection;
    s32 previousRate;
    s32 highlight;

    list = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(list,
        (u32)dds3PrimFrameRect(0x8950, 0x79A8, 0xFEFFFF, 0x5A0, 0x210, 0x60000000, 0x40806020));
    sdfDevSetPrintCB(&packet, 0x8980, 0x79C0, 0xFF0000, 0);
    sdfDrawTagLink(list, (u32)sdfDevPrintfEx(&packet, D_0054F588));
    if (pbDebugPanel3X.editing == 0) {
        sdfDevSetPrintCB(&packet, 0x8980, pbDebugPanel3X.selection * 0x60 + 0x7A20, 0xFF0000, 0);
        pbDebugPanel3X.blink++;
        if (pbDebugPanel3X.blink < 32 || (pbDebugPanel3X.blink & 31) < 18) {
            sdfDrawTagLink(list, (u32)sdfDevPrintfEx(&packet, D_0054F590));
        }
    }
    for (row = 0; row != 4; row++) {
        sdfDevSetPrintCB(&packet, 0x8980, y, 0xFF0000, D_00326C20[row].highlight);
        sdfDrawTagLink(list,
            (u32)sdfDevPrintfEx(&packet, D_0054F598, D_00326C20[row].title));
        highlight = 0;
        if (pbDebugPanel3X.editing != 0 && row == pbDebugPanel3X.selection) {
            highlight = 6;
        }
        sdfDevSetPrintCB(&packet, 0x8C80, y, 0xFF0000, highlight);
        if (row < 4) {
            sdfDrawTagLink(list,
                (u32)sdfDevPrintfEx(&packet, D_0054F5A0, D_00326C50.channels[row]));
        } else {
            sdfDrawTagLink(list,
                (u32)sdfDevPrintfEx(&packet, D_0054F5A8, D_00551EA8));
        }
        y += 0x60;
    }
    { sdfGrOt_t *ot = &sdfGrOtTable[83]; ((sdfGrOtRegFunc_t)ot->regFunc)(ot, list); }
    pbDebugPanel3State += D_00551EA8;
    pbDrawRotatedPrim(0x46);
    if ((s8)((s8)dds3PadButtonState[0x32]) < 0) {
        D_0054F584 ^= 1;
    }
    if (pbDebugPanel3X.editing == 0) {
        previousSelection = pbDebugPanel3X.selection;
        if ((u8)((s8)dds3PadButtonState[0x36]) & 2) {
            pbDebugPanel3X.selection--;
            if (pbDebugPanel3X.selection < 0) {
                pbDebugPanel3X.selection = 3;
            }
        }
        if ((u8)((s8)dds3PadButtonState[0x37]) & 2) {
            pbDebugPanel3X.selection++;
            if (pbDebugPanel3X.selection >= 4) {
                pbDebugPanel3X.selection = 0;
            }
        }
        if (previousSelection != pbDebugPanel3X.selection) {
            pbDebugPanel3X.blink = 0;
        }
        if (((s8)dds3PadButtonState[0x31]) < 0) {
            pbDebugPanel3X.editing ^= 1;
            pbDebugPanel3X.blink = 0;
        }
        if ((u8)((s8)dds3PadButtonState[0x33]) & 2) {
            return -1;
        }
    } else {
        if (pbDebugPanel3X.selection < 4) {
            if ((u8)((s8)dds3PadButtonState[0x34]) & 2) {
                D_00326C50.channels[pbDebugPanel3X.selection] -= 10;
                if (D_00326C50.channels[pbDebugPanel3X.selection] < 0) {
                    D_00326C50.channels[pbDebugPanel3X.selection] = 0;
                }
            }
            if ((u8)((s8)dds3PadButtonState[0x35]) & 2) {
                D_00326C50.channels[pbDebugPanel3X.selection] += 10;
                if (D_00326C50.channels[pbDebugPanel3X.selection] > 255) {
                    D_00326C50.channels[pbDebugPanel3X.selection] = 255;
                }
            }
        } else {
            previousRate = D_00551EA8;
            if ((u8)((s8)dds3PadButtonState[0x34]) & 2) {
                D_00551EA8 -= 0x100;
                if (D_00551EA8 < 0) {
                    D_00551EA8 = 0;
                }
            }
            if ((u8)((s8)dds3PadButtonState[0x35]) & 2) {
                D_00551EA8 += 0x100;
                if (D_00551EA8 > 0x800) {
                    D_00551EA8 = 0x800;
                }
            }
            if (previousRate != D_00551EA8) {
                pbDebugPanel3State = 0;
            }
        }
        if (((s8)dds3PadButtonState[0x33]) < 0) {
            pbDebugPanel3X.editing ^= 1;
            pbDebugPanel3X.blink = 0;
        }
    }
    return 0;
}

u64 *pbAllocPrimPacket(u32 x, u32 y, u32 depth, u32 width, u32 height, u32 color0, u32 color1) {
    u64 *packet = (u64 *)sdfGrAllocPacketBuff(0x80);

    packet[0] = 7;
    packet[1] = 0x5000000700000000ULL;
    packet[2] = 0xB400000000008001ULL;
    packet[3] = 0xFF515151510ULL;
    packet[4] = 0x14D;
    packet[5] = color0 | ((u64)0xFE00 << 46);
    packet[7] = color0 | ((u64)0xFE00 << 46);
    packet[9] = color1 | ((u64)0xFE00 << 46);
    packet[11] = color1 | ((u64)0xFE00 << 46);
    packet[6] = (u64)((x & 0xFFFF) | (y << 16)) | ((u64)depth << 32);
    packet[8] = (u64)(((x + width) & 0xFFFF) | (y << 16)) | ((u64)depth << 32);
    packet[10] = (u64)(((x + width) & 0xFFFF) | ((y + height) << 16)) | ((u64)depth << 32);
    packet[12] = (u64)((x & 0xFFFF) | ((y + height) << 16)) | ((u64)depth << 32);
    packet[13] = packet[14] = packet[15] = 0;
    return packet;
}

void *pbMakeSpritePacket(u64 data, s32 context)
{
    pbRegPacket_t *packet;

    packet = sdf2DMakeStdPacket(sdfGrAllocPacketBuff(0x30), 0x30);
    packet->reg[1].data = data;
    packet->reg[1].reg = context ? 0x48 : 0x47;
    return packet;
}

void *pbMakePrimPacket(u64 data, s32 context)
{
    pbRegPacket_t *packet;

    packet = sdf2DMakeStdPacket(sdfGrAllocPacketBuff(0x30), 0x30);
    packet->reg[1].data = data;
    packet->reg[1].reg = context ? 0x43 : 0x42;
    return packet;
}

void pbDrawRotatedPrim(s32 surfaceIndex) {
    u32 color = 0;
    s32 alpha;
    s32 i;
    pbColorByte_t *component;
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;
    f32 phase;

    phase = (f32)(pbDebugPanel3State % 4096) * (1.0f / 4096.0f);
    phase = phase * 6.2831852f + 1.5707963f + 0.78539815f;
    alpha = (s32)((sdfSin(phase) + 1.0f) * 0.5f * D_00326C50.alpha);
    component = D_00326C50.channelBytes;
    for (i = 0; i != 3; i++, component++) {
        color |= component->value << (i * 8);
    }
    color |= (u32)alpha << 24;
    list = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(list, (u32)pbMakeSpritePacket(0x33001, 0));
    sdfDrawTagLink(list, (u32)pbMakePrimPacket(6, 0));
    sdfDrawTagLink(list, (u32)pbAllocPrimPacket(0x7000, 0x7900, 0xFEFFFF, 0x2000, 0xE00, color, color));
    surface = &sdfGrOtTable[surfaceIndex];
    ((sdfGrOtRegFunc_t)surface->regFunc)(surface, list);
}

void func_001D2638(f32 *delta) {
    f32 *eye = (f32 *)&aViewEye;
    u32 i;
    for (i = 0; i < 3; i++) {
        eye[i] += delta[i];
    }
}

INCLUDE_ASM(const s32, "code_001D1470", pbRotateXCamera);

INCLUDE_ASM(const s32, "code_001D1470", pbRotateYCamera);

INCLUDE_ASM(const s32, "code_001D1470", pb_1D2838);

INCLUDE_ASM(const s32, "code_001D1470", pbSetViewRotation);

INCLUDE_ASM(const s32, "code_001D1470", pb_draw_mycharacter_Process);

void pb_draw_mycharacter_Shutdown(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    pbMyCharacter_t *chara = panel->data.chara;
    mdlFree(chara->model);
    sdfChipFreeDelayedDirect(chara);
    sdfChipFreeDelayedDirect(panel);
}

void pb_1D2AD8(mdlObj_t *model, sdfVector4_t *pos) {
    mdlGetPosition(model);
    SDF_VU0_STOREVECTOR(pos);
}

void pbSetModelPos(mdlObj_t *model, const f32 *position) {
    SDF_VU0_LOADVECTOR(position);
    mdlSetPosition(model);
}

void pb_1D2B18(mdlObj_t *model, sdfVector4_t *rot) {
    mdlGetRotate(model);
    SDF_VU0_STOREVECTOR(rot);
}

void pbSetModelRotate(mdlObj_t *model, f32 x, f32 y, f32 z) {
    FromEuler();
    mdlSetRotate(model);
}

void func_001D2B68(void) {
}

void func_001D2B70(void) {
}

INCLUDE_RODATA(const s32, "code_001D1470", D_00535C58);

INCLUDE_ASM(const s32, "code_001D1470", pbInitMyCharacterDraw);

s32 pbGetPanelField(pbPanel_t *panel) {
    pbMyCharacter_t *chara = panel->data.chara;
    s32 ret = 0;
    if (panel != NULL) {
        ret = chara->field;
    }
    return ret;
}

u32 pbMyCharacterDispStat(pbPanel_t *panel) {
    pbMyCharacter_t *chara = panel->data.chara;
    u32 ret = 0;
    if (panel != NULL) {
        ret = chara->dispStat;
    }
    return ret;
}

void pbSetPanelValue(pbPanel_t *panel, s8 value) {
    pbPanelValue_t *v = panel->data.value;
    if (panel != NULL) {
        v->value = value;
    }
}

void func_001D2D00(pbPanel_t *panel, s32 arg) {
}

void pbChangeMotion(void) {
}

s32 pb_1D2D10(void) {
    return 0;
}

void pbMoveMyCharcter(pbPanel_t *panel, f32 dx, f32 dz) {
    pbMyCharacter_t *chara = panel->data.chara;
    if (panel != NULL) {
        chara->pos.x += dx;
        chara->pos.z += dz;
    }
}

void pbMoveMyCharcter2(pbPanel_t *panel, s16 dx, s16 dy) {
    pbMyCharacter_t *chara = panel->data.chara;
    if (panel != NULL) {
        chara->cellX = dx + chara->cellX;
        chara->cellY = dy + chara->cellY;
    }
}

void pbGetCellY(pbPanel_t *panel, s16 cx, s16 cy) {
    pbMyCharacter_t *chara = panel->data.chara;
    if (panel != NULL) {
        chara->cellX = cx;
        chara->cellY = cy;
        chara->pos.x = cx * 40 - 380;
        chara->pos.z = cy * 40 - 380;
    }
}

void pbGetMyCharcterPos(pbPanel_t *panel, sdfVector4_t *pos) {
    pbMyCharacter_t *chara = panel->data.chara;
    if (panel != NULL) {
        SDF_VU0_COPYVECTOR(pos, &chara->pos);
    }
}

void pbGetMyCharcterPos2(pbPanel_t *panel, s16 *x, s16 *y) {
    pbMyCharacter_t *chara = panel->data.chara;
    if (panel != NULL) {
        *x = chara->cellX;
        *y = chara->cellY;
    }
}

s32 pbGetMyCharcterRot(pbPanel_t *panel) {
    pbMyCharacter_t *chara = panel->data.chara;
    s32 ret = -1;
    if (panel != NULL) {
        ret = chara->rot;
    }
    return ret;
}

/* NON_MATCHING: 2 of 23 words; the index and work pseudos swap saved registers. */
#ifdef NON_MATCHING
void pbSetPanelModel(pbPanel_t *panel, s32 index) {
    pbPanelModel_t *work;
    if (panel != NULL) {
        work = panel->data.model;
        pbSetModelRotate((mdlObj_t *)pbGetPanelField(panel), 0.0f, D_00326C60[index], 0.0f);
        work->modelIndex = index;
    }
}
#else
INCLUDE_ASM(const s32, "code_001D1470", pbSetPanelModel);
#endif

INCLUDE_ASM(const s32, "code_001D1470", pb_draw_exit_Process);

void pb_draw_exit_Shutdown(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    pbMatWork_t *work = panel->data.mat;
    sdfMatFreeDelayed(work->mat);
    sdfChipFreeDelayedDirect(work);
    sdfChipFreeDelayedDirect(panel);
}

INCLUDE_ASM(const s32, "code_001D1470", pb_draw_mapbase_Process);

void pb_draw_mapbase_Shutdown(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    pbMatWork_t *work = panel->data.mat;
    sdfMatFreeDelayed(work->mat);
    sdfChipFreeDelayedDirect(work);
    sdfChipFreeDelayedDirect(panel);
}

void pbInitMapBaseMat(pbPanel_t *panel) {
    pbMatWork_t *work = panel->data.mat;
    pbBuildMapBaseVerts(panel);
    work->mat = sdfMatNew();
    sdfMatSetTex(work->mat, D_0054F5C4->baseTex);
}

void pbBuildMapBaseVerts(pbPanel_t *panel) {
    u32 row, col;
    for (row = 0; row < 6; row++) {
        for (col = 0; col < 6; col++) {
            u32 i = col + row * 6;
            SDF_VU0_COPYVECTOR(&D_0056CB30[i], &D_00326C70[row]);
            SDF_VU0_COPYVECTOR(&D_0056C8F0[i], &D_00326CD0[D_00326D50[i]]);
        }
    }
}

void pbSetPrimMatrix(sdfVector4_t *rot, f32 *pos, void *prim) {
    pbMatrix_t m;
    SetRotateMatrixXYZ(rot);
    StoreMatrix(&m);
    m.v[3].x = pos[0];
    m.v[3].y = pos[1];
    m.v[3].z = pos[2];
    LoadMatrix(&m);
    sdf3DPrimLinkTm(prim, 0);
}

INCLUDE_ASM(const s32, "code_001D1470", pbFindBlockSlot);

INCLUDE_ASM(const s32, "code_001D1470", func_001D35F0);

INCLUDE_ASM(const s32, "code_001D1470", pbDrawMapPoly);

INCLUDE_ASM(const s32, "code_001D1470", pb_1D3940);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535C80);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535C88);

INCLUDE_ASM(const s32, "code_001D1470", pbDrawMapPolyTransformed);

INCLUDE_ASM(const s32, "code_001D1470", pbDrawMapBlock);

pbPanel_t *pbInitExitDraw(pbPanel_t *parent) {
    pbPanel_t *panel = sdfChipAllocDirect(0x10);
    pbMatWork_t *work;
    panel->process = dds3StartProcess(D_00535CD8, 0x2AFD, 1, 1, pb_draw_exit_Process,
                                      (dds3ProcessFunc_t)pb_draw_exit_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->process, panel->process);
    }
    panel->x = 10;
    panel->y = 10;
    panel->flags = 0x10000;
    work = sdfChipAllocDirect(0xC);
    panel->data.mat = work;
    memset(work, 0, 0xC);
    dds3SetProcessData(panel->process, panel);
    work->unk00 = 0;
    work->unk06 = 0;
    work->unk04 = 0;
    work->mat = sdfMatNew();
    sdfMatSetDiffuse(work->mat, 0x70FFFFFF);
    return panel;
}

void pbSetExitDisp(pbPanel_t *panel, s8 disp) {
    panel->data.value->value = disp;
}

s32 pbGetExitDisp(pbPanel_t *panel) {
    return panel->data.value->value;
}

void pbSetExitPos(pbPanel_t *panel, s32 x, s32 y) {
    panel->x = x;
    panel->y = y;
}

/* NON_MATCHING: 3 of 49 words; the work store and the process load swap delay slots. */
#ifdef NON_MATCHING
pbPanel_t *pbInitMapBaseDraw(pbPanel_t *parent, s32 arg) {
    pbPanel_t *panel = sdfChipAllocDirect(0x10);
    void *work;
    panel->process = dds3StartProcess(D_00535CE8, 0x2AFD, 1, 1, pb_draw_mapbase_Process,
                                      (dds3ProcessFunc_t)pb_draw_mapbase_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->process, panel->process);
    }
    panel->flags = 0x10000;
    work = sdfChipAllocDirect(0xC);
    panel->data.mat = work;
    memset(work, 0, 0xC);
    panel->data.mat->value = arg;
    dds3SetProcessData(panel->process, panel);
    pbInitMapBaseMat(panel);
    return panel;
}
#else
INCLUDE_RODATA(const s32, "code_001D1470", D_00535CB8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535CC0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535CC8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535CD0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535CD8);

INCLUDE_ASM(const s32, "code_001D1470", pbInitMapBaseDraw);
#endif

void func_001D4140(pbPanel_t *panel, s32 value) {
    panel->data.value->value = value;
}

INCLUDE_ASM(const s32, "code_001D1470", pbLoadTextures);

INCLUDE_ASM(const s32, "code_001D1470", pbEditInitProc);

void pbEditMainExitProc(void) {
    p2sdStopAllSe();
    sdfTexFreeDelayed(D_0054F5C4->baseTex);
    sdfTexFreeDelayed(D_0054F5C4->tex344);
    sdfTexFreeDelayed(D_0054F5C4->tex348);
    sdfTexFreeDelayed(D_0054F5C4->tex34C);
    if (D_0054F5C4->giveupScript != NULL) {
        sdfMemFreeDirect(D_0054F5C4->mapData);
        D_0054F5C4->giveupScript = NULL;
    }
    sdfMemFreeDirect(a_key);
    fclEncycFreeEntries(D_00535DE0);
    fldProcNextJump();
}

s32 pb_proc_dummy_Process(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    if (panel != NULL) {
        panel->flags = 0x20000;
    }
    return -1;
}

void pb_proc_dummy_Shutdown(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    sdfChipFreeDelayedDirect(panel->data.value);
    sdfChipFreeDelayedDirect(panel);
}

INCLUDE_ASM(const s32, "code_001D1470", pb_draw_bg_Process);

void pb_draw_bg_Shutdown(dds3ProcessID_t *id) {
    sdfChipFreeDelayedDirect(id->workdata);
}

INCLUDE_RODATA(const s32, "code_001D1470", D_00535DE0);

INCLUDE_ASM(const s32, "code_001D1470", pbStartMapEditor);

s32 pbGetMoveChar(void) {
    return pbMoveChar;
}

pbPanel_t *pbStartDummy(pbPanel_t *parent) {
    pbPanel_t *panel = sdfChipAllocDirect(0x10);
    pbPanelValue_t *work;
    panel->process = dds3StartProcess(D_00535E00, 0x3EE, 1, 1, pb_proc_dummy_Process,
                                      (dds3ProcessFunc_t)pb_proc_dummy_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->process, panel->process);
    }
    work = sdfChipAllocDirect(4);
    work->value = 0;
    panel->data.value = work;
    panel->flags = 0;
    dds3SetProcessData(panel->process, panel);
    return panel;
}

pbPanel_t *pbStartBackGround(pbPanel_t *parent) {
    pbPanel_t *panel = sdfChipAllocDirect(0x10);
    panel->process = dds3StartProcess(D_00535E10, 0x2AFD, 1, 1, pb_draw_bg_Process,
                                      (dds3ProcessFunc_t)pb_draw_bg_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->process, panel->process);
    }
    panel->flags = 0;
    dds3SetProcessData(panel->process, panel);
    return panel;
}

s32 func_001D4B28(void) {
    return 0;
}

u32 pbMapIsValid(void) {
    u32 ret = (D_0054F5C4->pcharNum == 0);
    if ((D_0054F5C4->exitX < 0) || (D_0054F5C4->exitY < 0)) {
        ret |= 2;
    }
    return ret;
}

s32 pb_1D4B60(void) {
    return 0;
}

void func_001D4B68(void) {
}

s32 pb_1D4B70(void) {
    return 0;
}

void func_001D4B78(void) {
}

s32 pb_1D4B80(void) {
    return 0;
}

s32 pb_1D4B88(void) {
    return 0;
}

void func_001D4B90(void) {
}

s32 pbLoadSystemDataDone(s32 handle, s32 status) {
    if (status == 8) {
        pbLoadState = 1;
    } else if (status == 0) {
        pbLoadState = -1;
    }
    return 0;
}

s32 func_001D4BC0(void) {
    return 1;
}

s32 func_001D4BC8(void) {
    return pbMapIsValid() == 0;
}

s8 func_001D4BE8(void) {
    s8 ret = pbCheckPreviousRecord();
    return ret;
}

/* NON_MATCHING: 1 of 37 words; retail annuls the epilogue restore in a beql delay slot. */
#ifdef NON_MATCHING
void func_001D4C08(pbPanel_t *panel) {
    if (pbMapIsValid() == 0) {
        dds3FadeOut(0, 0, 0, 1);
        dds3ResetProcessFlag(panel->process, 0x10, 1);
        if (pbGetMoveChar() != 0) {
            if (pbGetMoveChar() != 0) {
                ((pbPanel_t *)pbGetMoveChar())->flags = 0x40000;
            }
            ((pbPanel_t *)pbGetMoveChar())->flags |= 1;
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_001D1470", func_001D4C08);
#endif

void pbMakeMapPathHdd(pbPanel_t *parent) {
    if (pbMapPathIdx == 1) {
        dds3FadeOut(0, 0, 0, 1);
        sprintf(pbMapPathBuf, D_00535E20, pbMenuIndex);
        sdfFileCmdReadFile(pbMapPathBuf, D_0054F5C4, -1, pbLoadSystemDataDone, 0);
        pbLoadState = 0;
        pbWaitLoadMapData(parent);
    } else {
        pbMapPathIdx = 1;
    }
}

INCLUDE_ASM(const s32, "code_001D1470", pbMakeMapPath);

void func_001D4E08(void) {
}

void func_001D4E10(void) {
}

void exitPB(pbPanel_t *panel) {
    dds3ResetProcessFlag(panel->process, 0x10, 1);
    if (pbGetMoveChar() != 0) {
        ((pbPanel_t *)pbGetMoveChar())->flags = 0x60000;
    }
    dds3AdminiCallSequence(0, 0, 0, 0);
}

void func_001D4E70(pbPanel_t *panel) {
    dds3ResetProcessFlag(panel->process, 0x10, 1);
    pbStartGiveupConfirm();
    panel->flags = 0x50000;
}

/* NON_MATCHING: 21 of 22 words; retail annuls the trailing lui in a beql delay slot. */
#ifdef NON_MATCHING
void func_001D4EB0(pbPanel_t *panel) {
    dds3ResetProcessFlag(panel->process, 0x10, 1);
    if (pbGetMoveChar() != 0) {
        ((pbPanel_t *)pbGetMoveChar())->flags = 0x10000;
    }
    panel->flags = 0x50000;
}
#else
INCLUDE_ASM(const s32, "code_001D1470", func_001D4EB0);
#endif

void func_001D4F08(void) {
    pbPreviousRecord();
}

void pbSetDebugOwner(pbPanel_t *panel) {
    pbPanelValue_t *work = panel->data.value;
    pbPanel_t *help = pbStartHelp((pbPanel_t *)pbGetMoveChar(), work->value);
    dds3ProcessID_t *id;
    D_0054F5C4->helpProc = help;
    dds3SetProcessFlag(help->process, 0x10, 1);
    id = dds3SearchProcessName(D_00535E90);
    if (id != NULL) {
        dds3ResetProcessFlag(id, 0x10, 2);
    }
    if (panel != NULL) {
        panel->flags = 0x50000;
    }
}

INCLUDE_RODATA(const s32, "code_001D1470", D_00535E00);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535E10);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535E20);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535E40);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535E68);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535E90);

INCLUDE_ASM(const s32, "code_001D1470", pb_proc_menu_Process);

void pb_proc_menu_Shutdown(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    pbPanelValue_t *work = panel->data.value;
    dds3ResetProcessFlag(id, 0x10, 1);
    sdfChipFreeDelayedDirect(work);
    sdfChipFreeDelayedDirect(panel);
    D_0054F5C4->menuState = 0;
}

INCLUDE_ASM(const s32, "code_001D1470", pb_load_mapdata_Process);

void pb_load_mapdata_Shutdown(dds3ProcessID_t *id) {
    sdfChipFreeDirect(id->workdata);
}

pbPanel_t *pbStartMenu(pbPanel_t *parent, s32 mode) {
    pbPanel_t *panel = sdfChipAllocDirect(0x10);
    void *work;
    panel->process = dds3StartProcess(D_00535EB8, 0x3F8, 1, 1, pb_proc_menu_Process,
                                      (dds3ProcessFunc_t)pb_proc_menu_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->process, panel->process);
    }
    panel->flags = 0x10000;
    work = sdfChipAllocDirect(4);
    panel->data.value = work;
    memset(work, 0, 4);
    panel->data.value->value = mode;
    dds3SetProcessData(panel->process, panel);
    pbMenuState = 0;
    pbMapPathIdx = 0;
    return panel;
}

pbPanel_t *pbWaitLoadMapData(pbPanel_t *parent) {
    pbPanel_t *panel = sdfChipAllocDirect(0x10);
    panel->process = dds3StartProcess(D_00535EC8, 0x3EE, 1, 1, pb_load_mapdata_Process,
                                      (dds3ProcessFunc_t)pb_load_mapdata_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->process, panel->process);
    }
    panel->flags = 0;
    dds3SetProcessData(panel->process, panel);
    return panel;
}

void pbLoadSystemData(s32 index) {
    dds3FadeOut(0, 0, 0, 1);
    if (index < 30) {
        sprintf(pbMapPathBuf, D_00535E40, index);
    } else {
        sprintf(pbMapPathBuf, D_00535E68, index);
    }
    sdfFileCmdReadFile(pbMapPathBuf, D_0054F5C4, -1, pbLoadSystemDataDone, 0);
    pbLoadState = 0;
    pbWaitLoadMapData(NULL);
}

s32 getCharacterNum(void) {
    s32 count = 0;
    u32 i;
    for (i = 0; i < D_0054F5C4->pcharNum; i++) {
        if (pbMyCharacterDispStat(mychar[i]) != 0) {
            count++;
        }
    }
    return count;
}

void pbNextCharacter(void) {
    do {
        current_char = (current_char + D_0054F5C4->pcharNum + 1) % D_0054F5C4->pcharNum;
    } while (pbMyCharacterDispStat(mychar[current_char]) == 0);
    updateMotion();
    if (pbCharacterTable[D_0054F5C4->charIndex] != NULL) {
        pbCharacterTable[D_0054F5C4->charIndex]();
    }
}

void updateMotion(void) {
    u32 i;
    for (i = 0; i < D_0054F5C4->pcharNum; i++) {
        func_001D2D00(mychar[i], 0);
    }
}

INCLUDE_RODATA(const s32, "code_001D1470", D_00535EB8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535EC8);

INCLUDE_ASM(const s32, "code_001D1470", hitCheck);

INCLUDE_ASM(const s32, "code_001D1470", pushCheck);

INCLUDE_ASM(const s32, "code_001D1470", roundCheck);

INCLUDE_ASM(const s32, "code_001D1470", roundDirCheck);

INCLUDE_ASM(const s32, "code_001D1470", hallCheck);

INCLUDE_ASM(const s32, "code_001D1470", pbFindProcHandle);

s8 goalHitCheck(s16 x, s16 y) {
    if ((x == goal->pos.x) && (y == goal->pos.y)) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_001D1470", gameKey0);

INCLUDE_ASM(const s32, "code_001D1470", gameKey1);

u32 RotateBit(u32 val, s32 loop) {
    s32 i;
    for (i = 0; i < loop; i++) {
        u32 bit = (val & 1) << 3;
        val >>= 1;
        val |= bit;
    }
    return val;
}

INCLUDE_ASM(const s32, "code_001D1470", gameKey2);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535EF8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535F08);

INCLUDE_ASM(const s32, "code_001D1470", game);

INCLUDE_ASM(const s32, "code_001D1470", deleteGame);

INCLUDE_ASM(const s32, "code_001D1470", pbInitGame);

INCLUDE_ASM(const s32, "code_001D1470", moveChar);

INCLUDE_ASM(const s32, "code_001D1470", deleteMoveChar);

INCLUDE_ASM(const s32, "code_001D1470", setMoveBlockData);

/* NON_MATCHING: 30 of 31 words, 124 bytes against retail's 140; retail holds the delta row in three pointer copies and materialises the -1 return late. */
#ifdef NON_MATCHING
s32 moveBlock(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    pbRoundBlock_t *work = panel->data.round;
    if (work->step >= 4) {
        return -1;
    }
    {
        f32 *delta = &D_00327370[work->index * 3];
        pbBlockObj_t *obj = work->block;
        obj->x = obj->x + delta[0];
        obj->y = obj->y + delta[1];
        obj->z = obj->z + delta[2];
    }
    work->step = work->step + 1;
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_001D1470", moveBlock);
#endif

INCLUDE_ASM(const s32, "code_001D1470", deleteMoveBlock);

void rotXY(s16 *x, s16 *y, s32 dir) {
    s16 ox = *x;
    if ((ox == 0) && (*y == 0)) {
        return;
    }
    *x = *y;
    *y = ox;
    if (dir == -1) {
        if (ox == 0) {
            s16 v = *x;
            *x = -v;
        }
    } else if (dir == 1) {
        if (*x == 0) {
            *y = -ox;
        }
    }
}

INCLUDE_ASM(const s32, "code_001D1470", setRoundBlockData);

INCLUDE_ASM(const s32, "code_001D1470", roundBlock);

void deleteRoundBlock(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    pbRoundBlock_t *work = panel->data.round;
    pbPushRecord(record);
    setRoundBlockData(work->block, work->index);
    work->block->value = 0;
    sdfChipFreeDelayedDirect(work);
    sdfChipFreeDelayedDirect(panel);
    moveB = 0;
}

INCLUDE_ASM(const s32, "code_001D1470", roundAutoCamera);

void deleteRoundAutoCamera(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    pbPanelValue_t *work = panel->data.value;
    if (cameraAngle > 6.28318501f) {
        cameraAngle = cameraAngle - 6.28318501f;
    } else if (cameraAngle < 0.0f) {
        cameraAngle = cameraAngle + 6.28318501f;
    }
    dds3ResetProcessFlag(panel->process, 0x10, 1);
    sdfChipFreeDelayedDirect(work);
    sdfChipFreeDelayedDirect(panel);
    roundCamera = 0;
}

INCLUDE_ASM(const s32, "code_001D1470", viewModeCamera);

void deleteViewModeCamera(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    sdfChipFreeDelayedDirect(panel->data.value);
    sdfChipFreeDelayedDirect(panel);
    moveViewCamera = 0;
}

s32 ClearProc(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    pbPanelValue_t *work = panel->data.value;
    rsetOtOfs(0x47);
    rsetAlpha(0);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    DrwSprite2D(0, 0xC3, 0x200, 0x3A, 0, 0x1C4, 0x200, 0x3A, 0x80808080,
                D_0054F5C4->tex34C);
    work->value = work->value - 1;
    if (work->value < 0) {
        return -1;
    }
    return 0;
}

void DeleteClearProc(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    sdfChipFreeDelayedDirect(panel->data.value);
    sdfChipFreeDelayedDirect(panel);
    D_0054F5C4->stage = (u16)D_0054F5C4->stage + 1;
    pbGotoNewStage();
}

INCLUDE_ASM(const s32, "code_001D1470", CalcDegreeMode);

s16 GetStageNumber(void) {
    return (pbLevelTable[D_0054F5C4->level]->stage + D_0054F5C4->stage)->number;
}

s16 GetStageScriptLabel(void) {
    return (pbLevelTable[D_0054F5C4->level]->stage + D_0054F5C4->stage)->label;
}

s16 GetAllStageClearBit(void) {
    return pbLevelTable[D_0054F5C4->level]->clearBit;
}

INCLUDE_RODATA(const s32, "code_001D1470", D_00535F40);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535F48);

INCLUDE_ASM(const s32, "code_001D1470", pbStartGame);

INCLUDE_ASM(const s32, "code_001D1470", pbStartMoveChar);

INCLUDE_ASM(const s32, "code_001D1470", pbStartMoveBlock);

INCLUDE_ASM(const s32, "code_001D1470", pbStartRoundBlock);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535F78);

INCLUDE_ASM(const s32, "code_001D1470", pbStartAutoRoundCamera);

pbPanel_t *pbStartViewModeCamera(pbPanel_t *parent) {
    pbPanel_t *panel = sdfChipAllocDirect(0x10);
    pbViewCamWork_t *work;
    panel->process = dds3StartProcess(D_00535FA8, 0x3EE, 0, 0, viewModeCamera,
                                      (dds3ProcessFunc_t)deleteViewModeCamera, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->process, panel->process);
    }
    panel->flags = 0;
    work = sdfChipAllocDirect(0x30);
    panel->data.viewCam = work;
    memset(work, 0, 0x30);
    work->scale = 1.0f;
    SDF_VU0_COPYVECTOR(&work->eye, &aViewEye);
    SDF_VU0_COPYVECTOR(&work->target, &aViewTarget);
    dds3SetProcessData(panel->process, panel);
    return panel;
}

void pbRoundCamera(s32 dir) {
    roundCamera = pbStartAutoRoundCamera(0, (dir != -1) ? -1 : 1);
    if (dir == -1) {
        pbRotLKeyAct();
    } else {
        pbRotRKeyAct();
    }
}

void pbSetGameCamera0(void) {
    SDF_VU0_COPYVECTOR(&aViewEye, &pbGameCameraParam);
    SDF_VU0_COPYVECTOR(&aViewTarget, &pbCameraVec);
}

INCLUDE_ASM(const s32, "code_001D1470", pbSetGameCamera1);

INCLUDE_ASM(const s32, "code_001D1470", pbSetGameCamera2);

INCLUDE_ASM(const s32, "code_001D1470", pbSetGameCamera3);

INCLUDE_ASM(const s32, "code_001D1470", pbSetGameCamera4);

/* NON_MATCHING: 26 of 38 words; gcc range-tests case 0 where retail compares it. */
#ifdef NON_MATCHING
void pbInitGameCamera(void) {
    switch (D_0054F5C4->charIndex) {
    case 0:
        pbSetGameCamera0();
        cameraAngle = 0.0f;
        break;
    case 1:
        SDF_VU0_COPYVECTOR(&aViewEye, &pbInitCameraParam);
        SDF_VU0_COPYVECTOR(&aViewTarget, &pbCameraVec2);
        pbSetGameCamera1();
        break;
    case 2:
        pbSetGameCamera2();
        break;
    }
}
#else
INCLUDE_ASM(const s32, "code_001D1470", pbInitGameCamera);
#endif

s8 pbCheckPreviousRecord(void) {
    s8 ret = pbCheckPopRecord(record);
    return ret;
}

void pbPreviousRecord(void) {
    s32 r = pbPopRecord(record, mychar, blockDB);
    if (r >= 0) {
        pbStartErrorDraw(0, 9, 30);
        if (blockDB != NULL) {
            blockDB->flags = 0x10000;
        }
        current_char = r;
        updateMotion();
        if (pbCharacterTable[D_0054F5C4->charIndex] != NULL) {
            pbCharacterTable[D_0054F5C4->charIndex]();
        }
    }
}

pbPanel_t *pbStartClear(pbPanel_t *parent) {
    pbPanel_t *panel = sdfChipAllocDirect(0x10);
    pbPanelValue_t *work;
    panel->process = dds3StartProcess(D_00535FC8, 0x3EE, 1, 1, ClearProc,
                                      (dds3ProcessFunc_t)DeleteClearProc, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->process, panel->process);
    }
    panel->flags = 0;
    work = sdfChipAllocDirect(4);
    work->value = 30;
    panel->data.value = work;
    dds3SetProcessData(panel->process, panel);
    return panel;
}

dds3ProcessID_t *pbStartGiveupConfirm(void) {
    fclClearFlag(0);
    scr_msg = scrStartScript2(0x3EE, D_0054F5C4->giveupScript, 1);
    dds3SetProcessFlag(dds3SearchProcessName(D_00535EF8), 0x10, 2);
    return scr_msg;
}

void pbGetCameraParam(f32 *param) {
    param[0] = cameraAngle;
    param[1] = cameraRadius;
    param[2] = cameraDegree;
}

void func_001D8EA0(f32 angle) {
    cameraAngle = angle;
}

void pbSetCameraRadius(f32 radius) {
    cameraRadius = radius;
}

void pbSetCameraDegree(f32 degree) {
    cameraDegree = degree;
}

void pbSetCameraDegreeInit(f32 degree) {
    cameraDegreeInit = degree;
}

dds3ProcessID_t *pbGetMessageScriptProc(void) {
    return scr_msg;
}

INCLUDE_RODATA(const s32, "code_001D1470", D_00535FA8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00535FC8);

INCLUDE_ASM(const s32, "code_001D1470", pbGotoNewStage);

void pbRoundStageNext(s16 *level, s16 *stage) {
    pbLevel_t *lv = pbLevelTable[*level];
    *stage = (u16)*stage + 1;
    if ((lv->stage + *stage)->number < 0) {
        *stage = 0;
        *level = (u16)*level + 1;
        if (*level >= 3) {
            *level = 0;
        }
    }
}

void pbRoundStagePrev(s16 *level, s16 *stage) {
    pbLevel_t *lv;
    s32 s = (u16)*stage - 1;
    *stage = s;
    if ((s16)s < 0) {
        s32 l = (u16)*level - 1;
        *level = l;
        if ((s16)l < 0) {
            *level = 2;
        }
        lv = pbLevelTable[*level];
        *stage = 0;
        while ((lv->stage + *stage + 1)->number >= 0) {
            *stage = (u16)*stage + 1;
        }
    }
}

void *pbFindGroupEntry(pbGroupDB_t *db, u32 group) {
    pbGroupEntry_t *found = NULL;
    u32 i;
    for (i = 0; i < db->count; i++) {
        pbGroupEntry_t *e = db->entry[i];
        if (group == e->group) {
            found = e;
            break;
        }
    }
    return found;
}

INCLUDE_ASM(const s32, "code_001D1470", pbAddGroupEntry);

void pbFreeGroupEntry(mdlResrc_t *entry) {
    sdfMemFreeDirect(entry->resourceHandle);
    sdfChipFreeDelayedDirect(entry);
}

void pbFreeGroupHandles(pbBlockDB_t *db) {
    u32 i;
    for (i = 0; i < db->count; i++) {
        pbFreeGroupEntry((mdlResrc_t *)db->entry[i]);
    }
    db->count = 0;
}

INCLUDE_ASM(const s32, "code_001D1470", pbGetBlockAt);

INCLUDE_ASM(const s32, "code_001D1470", pb_proc_block_DB_Process);

void pb_proc_block_DB_Shutdown(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    pbBlockDB_t *db = panel->data.blockDB;
    sdfMatFreeDelayed(db->mat);
    pbFreeGroupHandles(db);
    sdfChipFreeDelayedDirect(db);
    sdfChipFreeDelayedDirect(panel);
}

INCLUDE_ASM(const s32, "code_001D1470", pb_draw_map_cursor_Process);

void pb_draw_map_cursor_Shutdown(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    sdfChipFreeDelayedDirect(panel->data.value);
    sdfChipFreeDelayedDirect(panel);
}

pbPanel_t *pbStartBlockDataBase(pbPanel_t *parent) {
    pbPanel_t *panel = sdfChipAllocDirect(0x10);
    void *work;
    sdfMat_t *mat;
    panel->process = dds3StartProcess(D_00536040, 0x2AFD, 1, 1, pb_proc_block_DB_Process,
                                      (dds3ProcessFunc_t)pb_proc_block_DB_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->process, panel->process);
    }
    panel->flags = 0x10000;
    work = sdfChipAllocDirect(0x108);
    panel->data.blockDB = work;
    memset(work, 0, 0x108);
    dds3SetProcessData(panel->process, panel);
    mat = sdfMatNew();
    panel->data.blockDB->mat = mat;
    sdfMatSetTex(mat, D_0054F5C4->baseTex);
    return panel;
}

void *pbSearchGroupDB(pbPanel_t *panel, u32 group) {
    return pbFindGroupEntry(panel->data.groupDB, group);
}

void pbDeleteGroupEntry(pbPanel_t *panel, u32 group) {
    pbGroupDB_t *db = panel->data.groupDB;
    s32 removed = 0;
    u32 i;
    for (i = 0; i < db->count; i++) {
        if (removed != 0) {
            db->entry[i - 1] = db->entry[i];
        }
        if (db->entry[i]->group == group) {
            pbFreeGroupEntry((mdlResrc_t *)db->entry[i]);
            removed = 1;
        }
    }
    db->count = db->count - 1;
}

INCLUDE_RODATA(const s32, "code_001D1470", D_00536000);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536040);

INCLUDE_ASM(const s32, "code_001D1470", pbDumpBlockData);

INCLUDE_ASM(const s32, "code_001D1470", pbRebuildOutline);

pbPanel_t *pbStartDrawMapCursor(pbPanel_t *parent) {
    pbPanel_t *panel = sdfChipAllocDirect(0x10);
    void *work;
    panel->process = dds3StartProcess(D_005361B0, 0x2AFD, 1, 1, pb_draw_map_cursor_Process,
                                      (dds3ProcessFunc_t)pb_draw_map_cursor_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->process, panel->process);
    }
    panel->flags = 0x10000;
    work = sdfChipAllocDirect(4);
    panel->data.value = work;
    memset(work, 0, 4);
    dds3SetProcessData(panel->process, panel);
    return panel;
}

void pbSetMapCursorFlag(pbPanel_t *panel, s32 flag) {
    pbPanelValue_t *v = panel->data.value;
    if (panel != NULL) {
        v->value = flag;
    }
}

s32 pb_proc_record_Process(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    u32 state = panel->state;
    switch (state) {
    case 4:
    case 5:
    case 6:
        return -1;
    case 1:
        if (panel != NULL) {
            panel->flags = 0;
        }
        break;
    case 0:
        break;
    }
    return 0;
}

void pb_proc_record_Shutdown(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    pbRecord_t *rec = panel->data.record;
    pbClearRecord(panel);
    sdfChipFreeDelayedDirect(rec->slot);
    sdfChipFreeDelayedDirect(rec);
    sdfChipFreeDelayedDirect(panel);
}

void pbFreeRecord(pbRecordSlot_t *slot) {
    if (slot->block != NULL) {
        slot->flag[0] = 0;
        slot->flag[1] = 0;
        sdfChipFreeDelayedDirect(slot->block);
        slot->block = NULL;
    }
    if (slot->floor != NULL) {
        slot->flag[2] = 0;
        sdfChipFreeDelayedDirect(slot->floor);
        slot->floor = NULL;
    }
    if (slot->chara != NULL) {
        slot->flag[3] = 0;
        sdfChipFreeDelayedDirect(slot->chara);
        slot->chara = NULL;
    }
}

pbPanel_t *pbStartRecord(pbPanel_t *parent) {
    pbPanel_t *panel = sdfChipAllocDirect(0x10);
    pbRecord_t *work;
    panel->process = dds3StartProcess(D_005361C8, 0x3EE, 1, 1, pb_proc_record_Process,
                                      (dds3ProcessFunc_t)pb_proc_record_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->process, panel->process);
    }
    panel->flags = 0x10000;
    work = sdfChipAllocDirect(8);
    panel->data.record = work;
    work->pos = 0;
    work->slot = sdfChipAllocDirect(0x1E0);
    memset(work->slot, 0, 0x1E0);
    dds3SetProcessData(panel->process, panel);
    return panel;
}

void pbClearRecord(pbPanel_t *panel) {
    pbRecord_t *rec = panel->data.record;
    u32 i;
    for (i = 0; i < 30; i++) {
        pbFreeRecord(&rec->slot[i]);
    }
}

s32 pb_1DA400(pbPanel_t *panel) {
    return panel->data.value->value;
}

u32 pbPushRecord(pbPanel_t *panel) {
    pbPanelValue_t *rec = panel->data.value;
    return rec->value = (rec->value + 31) % 30;
}

s8 pbCheckPopRecord(pbPanel_t *panel) {
    pbRecord_t *rec = panel->data.record;
    pbRecordSlot_t *slot = &rec->slot[(rec->pos + 29) % 30];
    return slot->block != NULL;
}

INCLUDE_ASM(const s32, "code_001D1470", pbPopRecord);

INCLUDE_ASM(const s32, "code_001D1470", pbAddRecord_blk);

INCLUDE_ASM(const s32, "code_001D1470", pbAddRecord_flor);

INCLUDE_ASM(const s32, "code_001D1470", pbAddRecord_char);

INCLUDE_RODATA(const s32, "code_001D1470", D_005361B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005361C8);

INCLUDE_ASM(const s32, "code_001D1470", pbDumpRecordData);

INCLUDE_ASM(const s32, "code_001D1470", pb_draw_help_Process);

void pb_draw_help_Shutdown(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    sdfChipFreeDelayedDirect(panel->data.value);
    sdfChipFreeDelayedDirect(panel);
    D_0054F5C4->helpProc = NULL;
}

pbPanel_t *pbStartHelp(pbPanel_t *parent, s32 page) {
    pbPanel_t *panel = sdfChipAllocDirect(0x10);
    pbPanelValue_t *work;
    panel->process = dds3StartProcess(D_00536398, 0x2AFD, 1, 1, pb_draw_help_Process,
                                      (dds3ProcessFunc_t)pb_draw_help_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->process, panel->process);
    }
    panel->x = 0;
    panel->y = 0;
    panel->flags = 0x10000;
    work = sdfChipAllocDirect(8);
    panel->data.value = work;
    memset(work, 0, 8);
    work->value = page;
    dds3SetProcessData(panel->process, panel);
    return panel;
}

pbPanel_t *func_001DB020(pbPanel_t *parent) {
    return pbStartHelp(parent, 0);
}

INCLUDE_ASM(const s32, "code_001D1470", pbResetKeyState);

u8 pbGetKeyAction(u8 action) {
    return keyAct[action + (s8)keyActMode * 5];
}

u8 pbGetKeyMode(void) {
    return keyActMode;
}

void func_001DB0D8(u8 mode) {
    keyActMode = mode;
}

u8 pbRotRKeyAct(void) {
    return keyActMode = ((s8)keyActMode + 5) % 4;
}

u8 pbRotLKeyAct(void) {
    return keyActMode = ((s8)keyActMode + 3) % 4;
}

INCLUDE_RODATA(const s32, "code_001D1470", D_00536398);

INCLUDE_ASM(const s32, "code_001D1470", pbInitMCard);

s32 pbGetFormatStatusMCard(s32 port, s32 slot, pbMCardInfo_t *info) {
    return sceMcGetInfo(port, slot, NULL, NULL, &info->format);
}

s32 pbGetTypeStatusMCard(s32 port, s32 slot, pbMCardInfo_t *info) {
    return sceMcGetInfo(port, slot, &info->type, NULL, NULL);
}

s32 pbGetFreeStatusMCard(s32 port, s32 slot, pbMCardInfo_t *info) {
    return sceMcGetInfo(port, slot, NULL, &info->free, NULL);
}

void pbGetStatusMCard(s32 port, s32 slot, pbMCardInfo_t *info) {
    sceMcGetInfo(port, slot, &info->type, &info->free, &info->format);
    sceMcSync(0, NULL, &info->result);
}

s32 pbGetDirMCard(s32 port, s32 slot, const char *fileName, void *tbl) {
    s32 result;
    sceMcGetDir(port, slot, fileName, 0, 18, tbl);
    sceMcSync(0, NULL, &result);
    return result;
}

s32 pbGetDirMCard2(s32 port, s32 slot, const char *fileName) {
    s32 result;
    sceMcGetDir(port, slot, fileName, 0, -1, NULL);
    sceMcSync(0, NULL, &result);
    return result;
}

s32 pbChangeDirectory(s32 port, s32 slot, const char *pathName) {
    s32 result;
    sceMcChdir(port, slot, pathName, NULL);
    sceMcSync(0, NULL, &result);
    return result;
}

s32 pbMkdirMCard(s32 port, s32 slot, const char *dirName) {
    s32 result;
    sceMcMkdir(port, slot, dirName);
    sceMcSync(0, NULL, &result);
    return result;
}

INCLUDE_RODATA(const s32, "code_001D1470", D_00536498);

INCLUDE_RODATA(const s32, "code_001D1470", D_005364C0);

INCLUDE_ASM(const s32, "code_001D1470", pbReadMCard);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536538);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536560);

INCLUDE_ASM(const s32, "code_001D1470", pbWriteMCard);

INCLUDE_ASM(const s32, "code_001D1470", pb_draw_error_Process);

void pb_draw_error_Shutdown(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    sdfChipFreeDelayedDirect(panel->data.value);
    sdfChipFreeDelayedDirect(panel);
}

/* NON_MATCHING: 2 of 47 words; the process load and the data store swap delay slots. */
#ifdef NON_MATCHING
pbPanel_t *pbStartErrorDraw(pbPanel_t *parent, s32 code, s32 timer) {
    pbPanel_t *panel = sdfChipAllocDirect(0x10);
    pbErrorWork_t *work;
    panel->process = dds3StartProcess(D_005365D8, 0x2AFD, 1, 1, pb_draw_error_Process,
                                      (dds3ProcessFunc_t)pb_draw_error_Shutdown, NULL);
    if (parent != NULL) {
        dds3LinkProcess(parent->process, panel->process);
    }
    work = sdfChipAllocDirect(0xC);
    work->code = code;
    work->timer = timer;
    work->unk04 = 0;
    panel->flags = 0x10000;
    panel->data.error = work;
    dds3SetProcessData(panel->process, panel);
    return panel;
}
#else
INCLUDE_ASM(const s32, "code_001D1470", pbStartErrorDraw);
#endif

INCLUDE_ASM(const s32, "code_001D1470", pbDebugProc);

void pbDebugShutdown(void) {
    ReleaseDebug_pbDebugMode();
}

void InitializeDebug_pbDebugMode(void) {
    sdfMemHandle_t *handle = sdfMemAllocDirect(0x10);
    pbDebugWork_t *work = sdfMemLock(handle);
    work->unk04 = 0;
    work->unk06 = 0;
    work->unk08 = 0;
    work->allocation = handle;
    work->level = D_0054F5C4->level;
    work->stage = D_0054F5C4->stage;
    g_pdebug_0 = work;
}

void ReleaseDebug_pbDebugMode(void) {
    if (g_pdebug_0 != NULL) {
        sdfMemFreeDirect(g_pdebug_0->allocation);
        g_pdebug_0 = NULL;
    }
}

INCLUDE_ASM(const s32, "code_001D1470", DebugProcMenu_pbDebugMode);

INCLUDE_RODATA(const s32, "code_001D1470", D_005365E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005365F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536608);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536620);

INCLUDE_ASM(const s32, "code_001D1470", DebugProcRadius);

INCLUDE_ASM(const s32, "code_001D1470", DebugProcDegree);

INCLUDE_ASM(const s32, "code_001D1470", DebugProcStage);

INCLUDE_ASM(const s32, "code_001D1470", DebugProcFree);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536690);

INCLUDE_RODATA(const s32, "code_001D1470", D_005366A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005366B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005366C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005366D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005366E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005366F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536708);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536720);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536738);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536750);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536768);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536780);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536798);

INCLUDE_RODATA(const s32, "code_001D1470", D_005367B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005367C8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005367E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005367F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536810);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536828);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536840);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536858);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536870);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536888);

INCLUDE_RODATA(const s32, "code_001D1470", D_005368A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005368B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005368D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005368E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536900);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536918);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536930);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536948);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536960);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536978);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536990);

INCLUDE_RODATA(const s32, "code_001D1470", D_005369A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005369C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005369D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005369F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536A08);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536A20);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536A38);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536A50);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536A68);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536A80);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536A98);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536AB0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536AC8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536AE0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536AF8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536B10);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536B28);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536B40);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536B58);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536B70);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536B88);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536BA0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536BB8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536BD0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536BE8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536C00);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536C18);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536C30);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536C48);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536C60);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536C78);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536C90);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536CA8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536CC0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536CD8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536CF0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536D08);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536D20);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536D38);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536D50);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536D68);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536D80);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536D98);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536DB0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536DC8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536DE0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536DF8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536E10);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536E28);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536E40);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536E58);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536E70);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536E88);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536EA0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536EB8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536ED0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536EE8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536F00);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536F18);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536F30);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536F48);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536F60);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536F78);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536F90);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536FA8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536FC0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536FD0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00536FE8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537000);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537018);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537030);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537048);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537060);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537078);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537090);

INCLUDE_RODATA(const s32, "code_001D1470", D_005370A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005370C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005370D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005370F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537108);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537120);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537138);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537150);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537168);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537180);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537198);

INCLUDE_RODATA(const s32, "code_001D1470", D_005371B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005371C8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005371E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005371F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537210);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537228);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537240);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537258);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537270);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537288);

INCLUDE_RODATA(const s32, "code_001D1470", D_005372A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005372B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005372D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005372E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537300);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537318);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537330);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537348);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537360);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537378);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537390);

INCLUDE_RODATA(const s32, "code_001D1470", D_005373A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005373C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005373D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005373F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537408);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537420);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537438);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537450);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537468);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537480);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537498);

INCLUDE_RODATA(const s32, "code_001D1470", D_005374B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005374C8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005374E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005374F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537510);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537528);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537540);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537558);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537570);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537588);

INCLUDE_RODATA(const s32, "code_001D1470", D_005375A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005375B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005375D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005375E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537600);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537618);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537630);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537648);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537660);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537678);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537690);

INCLUDE_RODATA(const s32, "code_001D1470", D_005376A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005376C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005376D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005376F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537708);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537720);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537738);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537750);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537768);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537780);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537798);

INCLUDE_RODATA(const s32, "code_001D1470", D_005377B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005377C8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005377E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005377F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537810);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537828);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537840);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537858);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537870);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537888);

INCLUDE_RODATA(const s32, "code_001D1470", D_005378A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005378B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005378D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005378E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537900);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537918);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537930);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537948);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537960);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537978);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537990);

INCLUDE_RODATA(const s32, "code_001D1470", D_005379A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005379C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005379D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005379F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537A08);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537A20);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537A38);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537A50);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537A68);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537A80);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537A98);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537AB0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537AC8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537AE0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537AF8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537B10);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537B28);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537B40);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537B58);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537B70);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537B88);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537BA0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537BB8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537BD0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537BE8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537C00);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537C18);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537C30);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537C48);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537C60);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537C78);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537C90);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537CA8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537CC0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537CD8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537CF0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537D08);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537D20);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537D38);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537D50);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537D68);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537D80);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537D98);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537DB0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537DC8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537DE0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537DF8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537E10);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537E28);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537E40);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537E58);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537E70);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537E88);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537EA0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537EB8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537ED0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537EE8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537F00);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537F18);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537F30);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537F48);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537F60);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537F78);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537F90);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537FA8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537FC0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537FD8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00537FF0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538008);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538020);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538038);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538050);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538068);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538080);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538098);

INCLUDE_RODATA(const s32, "code_001D1470", D_005380B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005380C8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005380E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005380F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538110);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538128);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538140);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538158);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538170);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538188);

INCLUDE_RODATA(const s32, "code_001D1470", D_005381A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005381B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005381D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005381E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538200);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538218);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538230);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538248);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538260);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538278);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538290);

INCLUDE_RODATA(const s32, "code_001D1470", D_005382A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005382C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005382D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005382F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538308);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538320);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538338);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538350);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538368);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538380);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538398);

INCLUDE_RODATA(const s32, "code_001D1470", D_005383B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005383C8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005383E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005383F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538410);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538428);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538440);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538458);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538470);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538488);

INCLUDE_RODATA(const s32, "code_001D1470", D_005384A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005384B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005384D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005384E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538500);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538518);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538530);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538548);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538560);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538578);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538590);

INCLUDE_RODATA(const s32, "code_001D1470", D_005385A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005385C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005385D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005385F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538608);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538620);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538638);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538650);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538668);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538680);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538698);

INCLUDE_RODATA(const s32, "code_001D1470", D_005386B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005386C8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005386E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005386F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538710);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538728);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538740);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538758);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538770);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538788);

INCLUDE_RODATA(const s32, "code_001D1470", D_005387A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005387B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005387D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005387E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538800);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538818);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538830);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538848);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538860);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538878);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538890);

INCLUDE_RODATA(const s32, "code_001D1470", D_005388A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005388C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005388D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005388F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538908);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538920);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538938);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538950);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538968);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538980);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538998);

INCLUDE_RODATA(const s32, "code_001D1470", D_005389B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005389C8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005389E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005389F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538A10);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538A28);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538A40);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538A58);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538A70);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538A88);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538AA0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538AB8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538AD0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538AE8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538B00);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538B18);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538B30);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538B48);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538B60);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538B78);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538B90);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538BA8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538BC0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538BD8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538BF0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538C08);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538C20);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538C38);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538C50);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538C68);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538C80);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538C98);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538CB0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538CC8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538CE0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538CF8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538D10);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538D28);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538D40);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538D58);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538D70);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538D88);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538DA0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538DB8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538DD0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538DE8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538E00);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538E18);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538E30);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538E48);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538E60);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538E78);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538E90);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538EA8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538EC0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538ED8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538EF0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538F08);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538F20);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538F38);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538F50);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538F68);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538F80);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538F98);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538FB0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538FC8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538FE0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00538FF8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539010);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539028);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539040);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539058);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539070);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539088);

INCLUDE_RODATA(const s32, "code_001D1470", D_005390A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005390B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005390D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005390E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539100);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539118);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539130);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539148);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539160);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539178);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539190);

INCLUDE_RODATA(const s32, "code_001D1470", D_005391A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005391C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005391D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005391F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539208);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539220);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539238);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539250);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539268);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539280);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539298);

INCLUDE_RODATA(const s32, "code_001D1470", D_005392B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005392C8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005392E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005392F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539310);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539328);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539340);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539358);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539370);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539388);

INCLUDE_RODATA(const s32, "code_001D1470", D_005393A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005393B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005393D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005393E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539400);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539418);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539430);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539448);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539460);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539478);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539490);

INCLUDE_RODATA(const s32, "code_001D1470", D_005394A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005394C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005394D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005394F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539508);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539520);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539538);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539550);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539560);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539570);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539580);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539590);

INCLUDE_RODATA(const s32, "code_001D1470", D_005395A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005395B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005395C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005395D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005395E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005395F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539600);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539610);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539620);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539630);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539648);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539660);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539678);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539688);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539698);

INCLUDE_RODATA(const s32, "code_001D1470", D_005396A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005396B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005396D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005396E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539700);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539718);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539730);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539748);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539760);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539778);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539790);

INCLUDE_RODATA(const s32, "code_001D1470", D_005397A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005397C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005397D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005397F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539808);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539820);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539838);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539850);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539868);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539880);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539898);

INCLUDE_RODATA(const s32, "code_001D1470", D_005398B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005398C8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005398E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005398F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539910);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539928);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539940);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539958);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539970);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539988);

INCLUDE_RODATA(const s32, "code_001D1470", D_005399A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005399B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_005399D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_005399E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539A00);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539A18);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539A30);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539A48);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539A60);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539A78);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539A90);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539AA8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539AC0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539AD8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539AF0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539B08);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539B20);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539B38);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539B50);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539B68);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539B80);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539B98);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539BB0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539BC8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539BE0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539BF8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539C10);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539C28);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539C40);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539C58);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539C70);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539C88);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539CA0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539CB8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539CD0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539CE8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539D00);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539D18);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539D30);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539D48);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539D60);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539D78);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539D90);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539DA8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539DC0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539DD8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539DF0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539E08);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539E20);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539E38);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539E50);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539E70);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539E90);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539EB0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539EC8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539EE0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539EF8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539F10);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539F28);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539F40);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539F58);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539F70);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539F88);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539FA0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539FB8);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539FD0);

INCLUDE_RODATA(const s32, "code_001D1470", D_00539FE8);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A000);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A018);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A030);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A048);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A060);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A078);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A090);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A0A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A0C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A0D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A0F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A108);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A120);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A138);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A150);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A168);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A180);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A198);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A1B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A1C8);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A1E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A1F8);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A210);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A228);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A240);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A258);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A270);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A288);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A298);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A2B8);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A2D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A2F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A308);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A320);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A338);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A350);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A368);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A380);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A398);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A3B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A3D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A3E8);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A400);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A418);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A430);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A448);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A460);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A478);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A490);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A4A8);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A4C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A4D8);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A4F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A508);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A520);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A538);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A550);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A568);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A580);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A590);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A5A0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A5B0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A5C0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A5D0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A5E0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A5F0);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A600);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A618);

INCLUDE_RODATA(const s32, "code_001D1470", D_0053A630);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F510);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F514);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F518);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F520);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F528);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F530);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F538);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F540);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F548);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F550);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F558);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F560);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F568);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F570);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F578);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F580);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F584);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F588);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F590);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F598);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F5A0);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F5A8);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F5B0);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F5B8);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F5C0);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F5C4);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F5D4);

INCLUDE_SDATA(const s32, "code_001D1470", pbMoveCharList);

INCLUDE_SDATA(const s32, "code_001D1470", blockDB);

INCLUDE_SDATA(const s32, "code_001D1470", bmap);

INCLUDE_SDATA(const s32, "code_001D1470", goal);

INCLUDE_SDATA(const s32, "code_001D1470", record);

INCLUDE_SDATA(const s32, "code_001D1470", mapCursor);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F5F0);

INCLUDE_SDATA(const s32, "code_001D1470", degree_mode);

INCLUDE_SDATA(const s32, "code_001D1470", pbGameKeyState);

INCLUDE_SDATA(const s32, "code_001D1470", pbMapCursorWork);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F5FC);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F600);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F608);

INCLUDE_SDATA(const s32, "code_001D1470", g_pdebug_0);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F614);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F618);

INCLUDE_SDATA(const s32, "code_001D1470", D_0054F620);

