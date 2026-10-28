#include "common.h"
#include "basic.h"
#include "model.h"
#include "newdata.h"
#include "scr.h"
#include "field.h"
#include "ps2asm.h"

typedef struct fileRequest fileRequest_t;

enum sl_mc_result
{
    SL_MC_RESULT_PENDING = 0,
    SL_MC_RESULT_SUCCESS = 1,
    SL_MC_RESULT_ERROR = -1,
    SL_MC_RESULT_SPECIAL_ERROR = -2
};

typedef enum sl_mc_result slMcResult_t;

enum sl_slot_flag
{
    SL_SLOT_FLAG_CAN_READ_SAVE = 1,
    SL_SLOT_FLAG_DIRECTORY_READY = 2,
    SL_SLOT_FLAG_DELETE_BEFORE_WRITE = 8
};

void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern void rsetOtOfs(u32);

extern void rsetAlpha(s32);

dds3ProcessID_t *dds3SearchProcessName(const char *namestr);

extern s32 slProcShutDownFlg;

extern s32 gSlWindowCnt;

extern s32 gslInfoCode;

extern s32 slChooseSaveIndex;

extern u32 mccb;

void sdfMcSetSlotIndex(s32 request, s32 selectedSlot);

s8 sdfMcGetSlotIndex(s32 request);

u32 sdfMcGetSlotFlags(s32 request, s32 slot);

extern s32 slChooseData_getCardInfoNext(void);

extern void mcMakeFileName(char *path, s32 num);

extern u32 strlen(const char *s);

extern void *memcpy(void *, const void *, unsigned int);

void sdfMcFuncOpen(u32 context, const char *path, s32 openFlags);

s32 slHeadLoadProc_openFile(void);

extern s32 D_00551D50;

slMcResult_t sdfMcFuncOpen1(s32 *resultOut);

sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

void *sdfMemLock(sdfMemHandle_t *aHandle);

extern sdfMemHandle_t *slLoadFileFd;

extern void sdfMcFuncRead(s32, u32, s32);

extern void *slLoadFileHandle;

s32 slHeadLoadProc_readFile(void);

extern s32 slNoCard(void);

slMcResult_t sdfMcFuncRead1(void);

extern void sdfMcFuncClose(s32);

extern s32 slHeadLoadProc_closeFile(void);

void dds3FadeOut(s32 r, s32 g, s32 b, s32 frame);

extern void *slExitWaitNextFunc;

s32 slExitWaitProc(void);

extern s32 slExitWaitCnt;

extern s32 gslMode;

extern s32 slTop(void);

s32 slEndingExit(void);

extern u32 gslErrorCode;

extern u32 slQueryYesNoMsg;

extern u32 gslDrawFlagFile;

s32 slQueryYesNo(void *yesFunc, void *noFunc, s32 yesNoDefault);

s32 slSaveData(void);

extern s32 slFileSel(void);

s32 slExitToTitle(void);

s32 slBootProc_CheckCard(void);

void sdfMcRegetInfo(s32 request);

extern s32 gslCheckCardWait;

extern u32 gSlNowCardCheck;

s32 slBootProc_CheckCardR(void);

extern u32 gslDrawFlagBg;

s32 slExitToTitleSub(void);

extern char D_005328C0[];

s32 slExitToTitle2(void);

extern char D_005328D0[];

fileRequest_t *fileLoad(const char *filename);

extern sdfMemHandle_t *D_00551D44;

extern u32 baseIconFileHandle;

s32 sdfMcGetFreeBytes(s32 request);

s32 sdfMcFuncGetInfo1(void);

u8 sdfMcGetFileInfoReady(s32 request);

extern s32 slChooseData_B(void);

extern u32 gslErrorKeyFlag;

extern s8 dds3PadButtonState[];

void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);

extern u32 slHitAnyKeyNextFunc;

extern s32 slHitAnyKeyNoChkProc(void);

extern s32 slHitAnyKeyProc(void);

void sdfMcFuncChdir(u32 context, const char *path);

s32 slChooseData_getCardInfo2(void);

slMcResult_t sdfMcFuncChdir1(void);

void sdfMcSetSlotFlags(s32 request, s32 slot, s32 mask);

extern char D_005328E0[];

extern u8 D_005696C0[];

void sdfMcFuncGetDir(u32 context, const char *path, void *directoryEntries, s32 entryLimit);

s32 slChooseData_getCardInfo3(void);

slMcResult_t sdfMcFuncGetDir1(s32 *resultOut);

s32 slChooseData_getCardInfo2_B(void);

s32 slChooseData_getCardInfo3_B(void);

s32 slChooseData_getCardInfoNext_B(void);

void sdfMcSetCardStat(s32 request);

s32 slSaveData_chdir(void);

void sdfMcFuncMkdir(u32 mccb, const char *cb);

s32 slSaveProc_Write_mkdir(void);

s32 slSaveProc_Write_chdir(void);

slMcResult_t sdfMcFuncMkdir1(void);

extern s32 slFileSelError;

s32 slSaveData_deleteIconSys(void);

slMcResult_t sdfMcFuncDelete1(void);

extern s32 slSaveData_writeFile(void);

void slKillLoadProcess(void);

extern char D_005328F0[];

extern s32 slSaveWriteEnd_baseIcon(void);

extern u32 D_00551D48;

extern u32 D_00551D4C;

s32 slSaveWriteFile(const char *filename, u32 *buff, u32 *size, void *endFunc, u32 *srcPtr);

void sdfMcFuncDelete(u32 context, const char *path);

extern u32 *slSaveWriteFile_buff;

extern u32 *slSaveWriteFile_size;

extern void *slSaveWriteFile_endFunc;

extern u32 *slSaveWriteFile_dataWaitPtr;

s32 slSaveProc_openFile(void);

s32 slSaveProc_waitDataReady(void);

extern void sdfMcFuncWrite(s32 request, u32 first, u32 second);

s32 slSaveProc_writeFile(void);

slMcResult_t sdfMcFuncWrite1(void);

s32 slSaveProc_closeFile(void);

slMcResult_t sdfMcFuncClose1(void);

slMcResult_t sdfMcFuncFormat1(void);

extern void dds3SoftResetOn(void);

s32 slCardChk(void);

extern void dds3SoftResetOff(void);

void sdfMcFuncFormat(u32 port);

extern u32 gSlEndingMode;

void slVersionConvert_Step1(void);

s32 slLoadProc_openFile(void);

s32 slLoadProc_readFile(void);

s32 slLoadProc_closeFile(void);

void slVersionConvert_Step3(void);

extern s8 D_005516C1;

extern s32 dds3ConfigChanged(void);

extern void dds3ConfigSaveBackup(void);

extern void dds3ConfigRestoreBackup(void);

extern s32 gslAnimCnt;

extern s32 slDraw(s32);

s32 pacFileCheckEnd(const fileRequest_t *pacFileHandle);

sdfMemHandle_t *fileGetMemHandle(const fileRequest_t *fileHandle);

u8 *fileGetLoadPtr(const fileRequest_t *fileHandle);

u32 fileGetLoadSize(const fileRequest_t *fileHandle);

s32 fileHandleFree(fileRequest_t *fileHandle);

extern char D_00532948[];

extern u32 gslExitType;

extern void *slQueryYesFunc;

extern void *slQueryNoFunc;

extern s32 slQueryYesNoCursor;

extern s32 slQueryYesNoKeyFlag;

extern s32 slQueryYesNoProc(void);

extern u32 gSlExitMode;

extern s32 (*proc_func_ans)(dds3ProcessID_t *process);

extern void *frwGetSmallFontFRQ(s32 X, s32 Y, s32 Pri, u32 Color, const char *pStr, s32 pFRQ);

extern void *frwGetSmallNumFontFRQ(s32 X, s32 Y, s32 Pri, u32 Color, const char *pStr, s32 pFRQ);

extern void frDrawFont(void *pFRQ);

extern void frDrawFontEx(void *pFRQ, s32 Mode);

extern void frDestroyFontRequestAfter(void *pFRQ);

extern void *slKanjiDraw_handle;

extern s32 slDDS3To31Flg;

extern s32 sprintf(char *dst, const char *format, ...);

extern char D_00532860[];

extern s8 D_00569743[];

extern s32 version_convert_ver;

extern u8 D_00316B00[];

extern u32 D_0054F06C;

extern u32 D_0054F070;

extern void dds3AdminiCallSequence(s32 sequence, s32 *data, s32 datasize, s32 restore);

extern void frSetFontDrawOT(s32 OtType);

extern void sdfMcClearSlotFlags(s32 request, s32 slot);

extern s32 slChooseData_getCardInfo_B(void);

extern s32 slTopMenuSel[];

extern f32 slNextPageDraw_fc;

extern f32 sdfSin(f32 aRadius);

extern void DrwSprite2DF(f32 x, f32 y, f32 w, f32 h, s32 u, s32 v, s32 uw, s32 vh,
                         s32 rgba, sdfTexHandle_t *th);

extern sdfTexHandle_t *slProcPtr;

extern s32 slErrorWaitMc(void);

extern s32 gSlSelCnt;

extern s32 slSaveProc_CheckCard(void);

extern s32 slChooseData(void);

extern s32 slFormatStart(void);

extern s32 slFormatCancel(void);

extern u8 frEnableMode(s32 Flag);

extern void frDisableMode(s32 Flag);

extern void *frSetFontString(const char *text, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

extern void frSetFontSpace(void *pFRQ, s32 Space);

extern void frSetDrawPosition(void *pFRQ, s32 X, s32 Y);

extern void frSetFontColor(void *pFRQ, u32 Color);

extern void func_001C7E38(void *request, s32 a1, s32 a2);

extern void *slKanjiDrawH_handle;


extern s32 sdfMcGetStatus(u32 request);

extern void sdfMcClearCardStat(u32 request);

extern s32 gSlFileErroeFlg;

extern s32 slFileSelCalc(void);

extern void *memset(void *a, s32 b, u32 c);

typedef struct slFileName_t {
    s8 ch[20];
} slFileName_t;

typedef struct slFileWork_t {
    s32 w[5];
} slFileWork_t;

typedef struct slFileRec_t {
    s8 verstr[3];
    s8 ver;
    s8 tmnlMode;
    s8 tmnlId;
    s8 level;
    s8 mode;
    s8 playTime[4];
    s8 endcnt[2];
    s8 newfile[2];
    slFileName_t name;
    s8 mapOpen;
    s8 unk25[15];
} slFileRec_t;

typedef struct GameFileInfo_t {
    u8 verstr[3];
    u8 ver;
    u8 TmnlMode;
    u8 TmnlId;
    u8 Lv;
    u8 mode;
    s32 PlayTime;
    u16 endcnt;
    u16 newfile;
    slFileWork_t work;
    u8 autoMapOpen;
    u8 autoMapOpenEX[15];
} GameFileInfo_t;

#define SL_REC ((u8 (*)[52])D_00569740)
#define SL_FILE_HEAD ((GameFileInfo_t *)DDS3_GBWK)

extern slFileRec_t D_00569740[];


extern sdfTexHandle_t *_saveUI;

extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);

extern void DrwSprite2D(s32, s32, s32, s32, s32, s32, s32, s32, u32, sdfTexHandle_t *);

extern void aDrawBoxG(s32 x, s32 y, s32 w, s32 h, u32 r0, u32 g0, u32 b0, u32 a0,
                      u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2,
                      u32 r3, u32 g3, u32 b3, u32 a3);

extern void slFileSelDraw(s32 id);

extern void slDrawInfo(void);

extern void slQueryYesNoDraw(void);

extern void slDrawError(void);


extern s32 datGetPlayerLevel(void);

extern s32 evtBitCheck(s32 no);

extern s32 gSlSaveMode;

extern u32 D_00551D30;

extern u32 D_00551D34;


extern char D_00528510[];

extern char D_00528548[];

extern char D_005285A8[];

extern char D_005285F8[];

extern char D_00528608[];

extern char D_00528598[];

extern char D_00528738[];

extern char D_005285B8[];

extern char D_00528748[];

extern char D_005287D8[];

extern char D_005287C0[];

extern char D_00528728[];

extern s32 slDrawWindow(void);

extern void slMcAccessDraw(void);


extern s32 slExitToTitle2(void);


extern char D_00528280[];

extern char D_005282B8[];

extern char D_005282D0[];

extern char D_00528308[];

extern char D_00528330[];

extern char D_00528340[];

extern char D_005281D8[];

extern char D_00528210[];

extern char D_00528228[];

extern char D_00528260[];

extern char D_00528448[];

extern char D_00528480[];

extern char D_005284B8[];

extern char D_005284E0[];

extern char D_005284F0[];

extern char D_005283B8[];

extern char D_005283F0[];

extern char D_00528428[];

extern char D_005285C8[];

extern char D_005285D8[];

extern char D_00528638[];

extern char D_00528658[];

extern char D_00528678[];

extern char D_00528690[];

extern char D_005285E8[];

extern char D_005288A0[];

extern char D_005288C0[];

extern char D_00528820[];

extern char D_00528850[];

extern char D_00528888[];

extern s32 gslInfoLoadFlg;

extern s32 gslFlushCnt;

extern s32 gSlMcAccessCnt;

extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frame);

extern sdfTexHandle_t *sdfTexSet(void *aTex);

extern sdfMemHandle_t *sdfDevLoadFileEx(const char *path, void **dataOut, s32 flag);

extern void evtBitOn(s32 no);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2, s32 v3,
                             s32 (*drawExit)(dds3ProcessID_t *),
                             s32 (*deleteExit)(dds3ProcessID_t *), void *p);

void slShutDownProcess(dds3ProcessID_t *id);
void func_00157058(void);

extern void *dds3GetActiveScene(void);

extern void dds3VisibleSceneBasic(void *basic, s32 visible);

void mcMakeFileName(char *path, s32 num) {
    if (slDDS3To31Flg != 0) {
        sprintf(path, D_00532860, 0xFEDA, num);
    } else {
        sprintf(path, D_00532860, 0x51AF, num);
    }
}

void slVersionConvert_Step1(void) {
    version_convert_ver = *(D_00569743 + sdfMcGetSlotIndex(mccb) * 52);
}

/* Save revisions have different persistent block extents. Keeping this
 * earlier provider in C also exposes the native same-TU call visibility to
 * slLoadProc_openFile (DDS idioms: earlier definitions and branch-likely). */
u32 slVersionConvert_GetSize(void) {
    if (version_convert_ver == 1) {
        return 0xF990;
    }
    if (version_convert_ver == 2) {
        return 0xFA30;
    }
    return (u32)(version_convert_ver - 3) < 2 ? 0x16DC0 : 0x1A510;
}

INCLUDE_RODATA(const s32, "mc/slMain", D_00532860);

INCLUDE_ASM(const s32, "mc/slMain", slVersionConvert_Step3);

u8 slGetPadTrig(s32 map) {
    return map != 0 && slProcShutDownFlg == 1;
}

u8 slGetPadRep(s32 map) {
    return map != 0 && slProcShutDownFlg == 1;
}


/* NON_MATCHING: 14 of 16 words, exact size; the trailing store and src++ are swapped. */
#ifdef NON_MATCHING
void slCopyNonZero(u8 *dst, u8 *src, s32 count) {
    s32 i;

    for (i = 0; i < count; i++) {
        if (*(s8 *)src == 0) {
            src++;
        } else {
            *dst = *src;
            dst++;
            src++;
        }
        *dst = *src;
        dst++;
        src++;
    }
    *dst = 0;
}
#else
INCLUDE_ASM(const s32, "mc/slMain", slCopyNonZero);
#endif


void slKanjiDraw(s32 x, s32 y, u32 rgba, const char *moji) {
    slKanjiDraw_handle = frwGetSmallFontFRQ(x << 4, y << 4, 0, rgba, moji, 0);
    frDrawFont(slKanjiDraw_handle);
    frDestroyFontRequestAfter(slKanjiDraw_handle);
}

void slKanjiDrawH(s32 x, s32 y, u32 rgba, const char *moji) {
    frEnableMode(1);
    slKanjiDrawH_handle = frSetFontString(moji, 0, 0, 0, 0, 0);
    frDisableMode(1);
    frSetFontSpace(slKanjiDrawH_handle, 3);
    frSetDrawPosition(slKanjiDrawH_handle, x << 4, y << 4);
    frSetFontColor(slKanjiDrawH_handle, rgba);
    func_001C7E38(slKanjiDrawH_handle, 0, 0x4A);
    frDestroyFontRequestAfter(slKanjiDrawH_handle);
    frSetFontDrawOT(0x48);
}

void slKanjiDrawN(s32 x, s32 y, u32 rgba, const char *moji) {
    void *request = frwGetSmallNumFontFRQ(x << 4, y << 4, 0, rgba, moji, 0);

    frDrawFontEx(request, 1);
    frDestroyFontRequestAfter(request);
}

void slNextPageDraw(void) {
    f32 phase;

    rsetOtOfs(0x4A);
    rsetAlpha(0);
    phase = slNextPageDraw_fc + 0.4f;
    slNextPageDraw_fc = phase;
    DrwSprite2DF(448.0f, sdfSin(phase) + 308.0f, 25.0f, 18.0f, 0xDB, 0x2D, 0x19, 0x12,
                 0x80808080, slProcPtr);
}

extern f32 slMcAccessDraw_fc;
extern void aDrawBoxF(s32 x, s32 y, s32 w, s32 h, u32 r, u32 g, u32 b, u32 a);
extern void aDrawBoxGF(f32 x, f32 y, f32 w, f32 h,
                       u32 r0, u32 g0, u32 b0, u32 a0,
                       u32 r1, u32 g1, u32 b1, u32 a1,
                       u32 r2, u32 g2, u32 b2, u32 a2,
                       u32 r3, u32 g3, u32 b3, u32 a3);

void slMcAccessDraw(void)
{
    f32 offset = 0.0f;
    s32 alpha;
    s32 color;
    rsetOtOfs(0x4A);
    rsetAlpha(0);
    if (gSlMcAccessCnt < 10) {
        alpha = (gSlMcAccessCnt * 128) / 10;
        aDrawBoxF(0, 0x89, 0x200, 0xC, 0, 0, 0, alpha / 5);
        color = ((u32)alpha << 24) | 0x808080;
        DrwSprite2D(0x47, 0x113, 0xBB, 0x13, 0x41, 0x6C, 0xBB, 0x13, color, slProcPtr);
        DrwSprite2D(0x102, 0x113, 0xBB, 0x13, 0xFB, 0x6C, -0xBB, 0x13, color, slProcPtr);
        slMcAccessDraw_fc = 0.0f;
    } else {
        slMcAccessDraw_fc += 0.4f;
        sdfSin(slMcAccessDraw_fc);
        aDrawBoxF(0, 0x89, 0x200, 0xC, 0, 0, 0, 0x19);
        DrwSprite2D(0x47, 0x113, 0xBB, 0x13, 0x41, 0x6C, 0xBB, 0x13, 0x80808080, slProcPtr);
        DrwSprite2D(0x102, 0x113, 0xBB, 0x13, 0xFB, 0x6C, -0xBB, 0x13, 0x80808080, slProcPtr);
    }
    if (gSlMcAccessCnt >= 10) {
        s32 phase = (gSlMcAccessCnt - 10) % 30;
        s32 i;
        offset = 0.0f;
        alpha = 128;
        if (phase < 10) {
            alpha = (phase * 128) / 10;
        } else {
            f32 speed = 2.0f;
            for (i = 0; i < phase - 10; i++) {
                offset += speed;
                speed += 3.0f;
            }
        }
        offset = -offset;
        aDrawBoxGF(offset + 338.0f, 282.0f, 144.0f, 2.0f,
                   100, 118, 255, (alpha * 70) / 100,
                   100, 118, 255, 0,
                   100, 118, 255, 0,
                   100, 118, 255, (alpha * 70) / 100);
        aDrawBoxGF(offset + 0.0f, 282.0f, 338.0f, 2.0f,
                   100, 118, 255, (alpha * 70) / 100,
                   100, 118, 255, (alpha * 70) / 100,
                   100, 118, 255, (alpha * 70) / 100,
                   100, 118, 255, (alpha * 70) / 100);
        offset = -offset;
        aDrawBoxGF(offset + 30.0f, 286.0f, 144.0f, 2.0f,
                   100, 118, 255, 0,
                   100, 118, 255, (alpha * 70) / 100,
                   100, 118, 255, (alpha * 70) / 100,
                   100, 118, 255, 0);
        aDrawBoxGF(offset + 174.0f, 286.0f, 338.0f, 2.0f,
                   100, 118, 255, (alpha * 70) / 100,
                   100, 118, 255, (alpha * 70) / 100,
                   100, 118, 255, (alpha * 70) / 100,
                   100, 118, 255, (alpha * 70) / 100);
    }
    gSlMcAccessCnt++;
}

s32 slChkWindowOpenDone(void) {
    if (gSlWindowCnt < 7) {
        return 0;
    }
    return 1;
}

s32 slDrawWindow(void) {
    s32 y;
    s32 h;
    s32 cnt;
    s32 val;

    rsetOtOfs(0x4A);
    rsetAlpha(0);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    gSlWindowCnt++;
    if (gSlWindowCnt < 4) {
        cnt = gSlWindowCnt;
        y = 0xE9 - cnt * 0xB5 / 4;
        h = cnt * 0x16B / 4;
    } else {
        y = 0x34;
        h = 0x16B;
    }
    aDrawBoxG(0, y / 2, 0x200, h / 2, 8, 5, 0x23, 0x66, 8, 5, 0x23, 0x66,
              8, 5, 0x23, 0x66, 8, 5, 0x23, 0x66);
    if (gSlWindowCnt >= 2) {
        if (gSlWindowCnt < 7) {
            cnt = gSlWindowCnt - 2;
            y = 0xE4 - cnt * 0x74 / 5;
            h = cnt * 0xE9 / 5;
        } else {
            y = 0x70;
            h = 0xE9;
        }
        aDrawBoxG(0, y / 2, 0x200, h / 2, 0, 0, 0, 0x26, 0, 0, 0, 0x26,
                  0, 0, 0, 0x26, 0, 0, 0, 0x26);
        if (gSlWindowCnt < 7) {
            cnt = gSlWindowCnt - 2;
            val = cnt * 4;
        } else {
            val = 0x14;
        }
        aDrawBoxG(0, 0x37, 0x100, 0x75, 0x61, 0x72, 0xF7, val * 0x80 / 0x64, 0x61, 0x72, 0xF7, 0,
                  0x61, 0x72, 0xF7, 0, 0x61, 0x72, 0xF7, 0);
        aDrawBoxG(0x100, 0x37, 0x100, 0x75, 0x61, 0x72, 0xF7, 0, 0x61, 0x72, 0xF7, 0,
                  0x61, 0x72, 0xF7, val * 0x80 / 0x64, 0x61, 0x72, 0xF7, 0);
        if (gSlWindowCnt < 7) {
            cnt = gSlWindowCnt - 2;
            val = cnt * 0x200 / 5 - 0x200;
        } else {
            val = 0;
        }
        aDrawBoxG(val + 0x152, 0x36, 0x90, 1, 0x64, 0x76, 0xFF, 0x59,
                  0x64, 0x76, 0xFF, 0, 0x64, 0x76, 0xFF, 0, 0x64, 0x76, 0xFF, 0x59);
        aDrawBoxG(val, 0x36, 0x152, 1, 0x64, 0x76, 0xFF, 0x59,
                  0x64, 0x76, 0xFF, 0x59, 0x64, 0x76, 0xFF, 0x59, 0x64, 0x76, 0xFF, 0x59);
        if (gSlWindowCnt < 7) {
            cnt = gSlWindowCnt - 2;
            val = 0x200 - cnt * 0x200 / 5;
        } else {
            val = 0;
        }
        aDrawBoxG(val + 0x1E, 0xAC, 0x90, 1, 0x64, 0x76, 0xFF, 0,
                  0x64, 0x76, 0xFF, 0x59, 0x64, 0x76, 0xFF, 0x59, 0x64, 0x76, 0xFF, 0);
        aDrawBoxG(val + 0xAE, 0xAC, 0x152, 1, 0x64, 0x76, 0xFF, 0x59,
                  0x64, 0x76, 0xFF, 0x59, 0x64, 0x76, 0xFF, 0x59, 0x64, 0x76, 0xFF, 0x59);
    }
    rsetOtOfs(0x47);
    if (gSlWindowCnt < 7) {
        return 0;
    }
    return 1;
}


void slDrawError(void) {
    if (gslErrorCode == 0) {
        return;
    }
    if (gslErrorCode == 1) {
        slFileSelError = 0;
        if (slDrawWindow() != 0) {
            if (slDDS3To31Flg != 0) {
                slKanjiDrawH(0xA, 0x4C, 0x64618780, D_00528280);
                slKanjiDrawH(0xA, 0x58, 0x64618780, D_005282B8);
                slKanjiDrawH(0xA, 0x64, 0x64618780, D_005282D0);
                slKanjiDrawH(0xA, 0x70, 0x64618780, D_00528308);
                slKanjiDrawH(0xA, 0x7C, 0x64618780, D_00528330);
                slKanjiDrawH(0xA, 0x88, 0x64618780, D_00528340);
            } else {
                slKanjiDrawH(0xA, 0x4C, 0x64618780, D_005281D8);
                slKanjiDrawH(0xA, 0x58, 0x64618780, D_00528210);
                slKanjiDrawH(0xA, 0x64, 0x64618780, D_00528228);
                slKanjiDrawH(0xA, 0x70, 0x64618780, D_00528260);
            }
            slNextPageDraw();
            gslDrawFlagFile = 0;
        }
    }
    if (gslErrorCode == 2) {
        if (slDrawWindow() != 0) {
            if (slDDS3To31Flg != 0) {
                slKanjiDrawH(0x28, 0x4C, 0x64618780, D_00528448);
                slKanjiDrawH(0x28, 0x58, 0x64618780, D_00528480);
                slKanjiDrawH(0x28, 0x64, 0x64618780, D_005284B8);
                slKanjiDrawH(0x28, 0x70, 0x64618780, D_005284E0);
                slKanjiDrawH(0x28, 0x7C, 0x64618780, D_005284F0);
            } else {
                slKanjiDrawH(0x1D, 0x4C, 0x64618780, D_005283B8);
                slKanjiDrawH(0x1D, 0x58, 0x64618780, D_005283F0);
                slKanjiDrawH(0x1D, 0x64, 0x64618780, D_00528428);
            }
            slNextPageDraw();
            gslDrawFlagFile = 0;
        }
    }
    if (gslErrorCode == 3) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(0x28, 0x4C, 0x64618780, D_005285C8);
            slNextPageDraw();
            gslDrawFlagFile = 0;
        }
    }
    if (gslErrorCode == 4) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(0x28, 0x4C, 0x64618780, D_005285D8);
            slNextPageDraw();
            gslDrawFlagFile = 0;
        }
    }
    if (gslErrorCode == 5) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(0x28, 0x4C, 0x64618780, D_00528638);
            slKanjiDrawH(0x28, 0x58, 0x64618780, D_00528658);
            slKanjiDrawH(0x28, 0x64, 0x64618780, D_00528678);
            slKanjiDrawH(0x28, 0x70, 0x64618780, D_00528690);
            slNextPageDraw();
            gslDrawFlagFile = 0;
        }
    }
    if (gslErrorCode == 6) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(0x28, 0x4C, 0x64618780, D_005285E8);
            slNextPageDraw();
            gslDrawFlagFile = 0;
        }
    }
    if (gslErrorCode == 7) {
        if (slDrawWindow() != 0) {
            if (slDDS3To31Flg != 0) {
                slKanjiDrawH(0x28, 0x4C, 0x64618780, D_005288A0);
                slKanjiDrawH(0x28, 0x58, 0x64618780, D_005288C0);
            } else {
                slKanjiDrawH(0x28, 0x4C, 0x64618780, D_00528820);
                slKanjiDrawH(0x28, 0x58, 0x64618780, D_00528850);
                slKanjiDrawH(0x28, 0x64, 0x64618780, D_00528888);
            }
            slNextPageDraw();
            gslDrawFlagFile = 0;
        }
    }
}

void slSetInfo(u32 id) {
    s32 previous;

    previous = gslInfoCode;
    gslInfoCode = id;
    if (previous == 0) {
        gSlWindowCnt = 0;
    }
}

void slDrawInfo(void) {
    if (gslInfoCode == 0) {
        return;
    }
    if (gslInfoCode == 1) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(0x28, 0x4C, 0x64618780, D_00528510);
            slKanjiDrawH(0x28, 0x58, 0x64618780, D_00528548);
        }
    }
    if (gslInfoCode == 2) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(0x28, 0x4C, 0x64618780, D_005285A8);
            slKanjiDrawH(0x28, 0x58, 0x64618780, D_005285F8);
            slKanjiDrawH(0x28, 0x64, 0x64618780, D_00528608);
            slMcAccessDraw();
        }
    }
    if (gslInfoCode == 3) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(0x28, 0x4C, 0x64618780, D_00528598);
            slKanjiDrawH(0x28, 0x58, 0x64618780, D_005285F8);
            slKanjiDrawH(0x28, 0x64, 0x64618780, D_00528608);
        }
    }
    if (gslInfoCode == 4) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(0x28, 0x4C, 0x64618780, D_00528738);
            slNextPageDraw();
        }
        gSlExitMode = 1;
    }
    if (gslInfoCode == 5) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(0x28, 0x4C, 0x64618780, D_005285B8);
            slKanjiDrawH(0x28, 0x58, 0x64618780, D_005285F8);
            slKanjiDrawH(0x28, 0x64, 0x64618780, D_00528608);
            slMcAccessDraw();
        }
    }
    if (gslInfoCode == 6) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(0x28, 0x4C, 0x64618780, D_00528748);
            slNextPageDraw();
        }
    }
    if (gslInfoCode == 0xB) {
        slKanjiDraw(0x14, 0x1C, 0x64618780, D_005287D8);
    }
    if (gslInfoCode == 0xC) {
        slKanjiDraw(0x14, 0x1C, 0x64618780, D_005287C0);
    }
    if (gslInfoCode == 0xD) {
        if (slDrawWindow() != 0) {
            slKanjiDrawH(0x28, 0x4C, 0x64618780, D_00528728);
            slNextPageDraw();
        }
        gSlExitMode = 2;
    }
}

s32 slHeadLoad(void) {
    char path[0x50];
    u32 ctx;
    s32 slot;
    s32 len;

    sdfMcSetSlotIndex(mccb, slChooseSaveIndex);
    ctx = mccb;
    slot = sdfMcGetSlotIndex(ctx);
    if ((sdfMcGetSlotFlags(ctx, slot) & SL_SLOT_FLAG_CAN_READ_SAVE) == 0) {
        return slChooseData_getCardInfoNext();
    }
    path[0] = 0x2F;
    mcMakeFileName(&path[1], slot);
    len = strlen(&path[1]);
    path[len + 1] = 0x2F;
    memcpy(&path[len + 2], &path[1], len);
    path[len * 2 + 2] = 0;
    sdfMcFuncOpen(ctx, path, 1);
    return (s32)slHeadLoadProc_openFile;
}

s32 slHeadLoadProc_openFile(void) {
    slMcResult_t status = sdfMcFuncOpen1(&D_00551D50);

    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        slLoadFileFd = sdfMemAllocDirect(0x34);
        slLoadFileHandle = sdfMemLock(slLoadFileFd);
        sdfMcFuncRead(D_00551D50, (u32)slLoadFileHandle, 0x34);
        return (s32)slHeadLoadProc_readFile;
    }
    return slNoCard();
}

s32 slHeadLoadProc_readFile(void) {
    slMcResult_t status = sdfMcFuncRead1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        sdfMcFuncClose(D_00551D50);
        return (s32)slHeadLoadProc_closeFile;
    }
    sdfMemFreeDirect(slLoadFileFd);
    return slNoCard();
}

s32 slHeadLoadProc_closeFile(void) {
    slMcResult_t status = sdfMcFuncClose1();
    s32 index;
    slFileRec_t *head;

    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        index = slChooseSaveIndex;
        head = &D_00569740[index];
        *head = *(slFileRec_t *)slLoadFileHandle;
        if (head->ver == 1) {
            memset(&D_00569740[index].name, 0, 0x14);
            D_00569740[index].mapOpen = 0;
        }
        sdfMemFreeDirect(slLoadFileFd);
        return slChooseData_getCardInfoNext();
    }
    sdfMemFreeDirect(slLoadFileFd);
    return slNoCard();
}

s32 slExitWait(void *nextFunc) {
    dds3FadeOut(0, 0, 0, 15);
    slExitWaitNextFunc = nextFunc;
    slExitWaitCnt = 20;
    return (s32)slExitWaitProc;
}

s32 slEndingTop(void) {
    gslMode = 0;
    slSetInfo(0);
    gslErrorCode = 0;
    slQueryYesNoMsg = 4;
    gslDrawFlagFile = 0;
    return slQueryYesNo(&slTop, &slEndingExit, 0);
}

s32 slEndingUpd2(void) {
    slSetInfo(0);
    gslErrorCode = 0;
    slQueryYesNoMsg = 9;
    return slQueryYesNo(&slSaveData, &slFileSel, 1);
}

s32 slEndingExit(void) {
    slSetInfo(0);
    gslErrorCode = 0;
    slQueryYesNoMsg = 8;
    gslDrawFlagFile = 0;
    return slQueryYesNo(&slExitToTitle, &slEndingTop, 1);
}

s32 slBootTop(void) {
    slSetInfo(0);
    gslDrawFlagFile = 0;
    return (s32)slBootProc_CheckCard;
}

s32 slBootRetry(void) {
    sdfMcRegetInfo(0);
    gslCheckCardWait = 30;
    gSlNowCardCheck = 1;
    slSetInfo(1);
    gslDrawFlagFile = 1;
    return (s32)slBootProc_CheckCardR;
}

s32 slBootError(void) {
    gslDrawFlagBg = 1;
    slSetInfo(0);
    slQueryYesNoMsg = 5;
    return slQueryYesNo(&slExitToTitle, &slBootRetry, 1);
}

s32 slBootError2(void) {
    gslDrawFlagBg = 1;
    slSetInfo(0);
    slQueryYesNoMsg = 6;
    return slQueryYesNo(&slExitToTitle, &slBootRetry, 1);
}

s32 slNoCard(void) {
    sdfMcSetCardStat(mccb);
    sdfMcClearSlotFlags(mccb, 0);
    sdfMcClearSlotFlags(mccb, 1);
    sdfMcClearSlotFlags(mccb, 2);
    sdfMcClearSlotFlags(mccb, 3);
    sdfMcClearSlotFlags(mccb, 4);
    slSetInfo(0);
    gslErrorCode = 1;
    gslErrorKeyFlag = 0;
    sdfMcRegetInfo(mccb);
    return (s32)slErrorWaitMc;
}

s32 slTop(void) {
    gSlSelCnt = 0;
    sdfMcSetCardStat(mccb);
    sdfMcClearSlotFlags(mccb, 0);
    sdfMcClearSlotFlags(mccb, 1);
    sdfMcClearSlotFlags(mccb, 2);
    sdfMcClearSlotFlags(mccb, 3);
    sdfMcClearSlotFlags(mccb, 4);
    slTopMenuSel[0] = 0;
    slTopMenuSel[1] = 0;
    slTopMenuSel[2] = 0;
    slTopMenuSel[3] = 0;
    slTopMenuSel[4] = 0;
    sdfMcRegetInfo(0);
    gslCheckCardWait = 15;
    gSlNowCardCheck = 1;
    slSetInfo(1);
    gslDrawFlagFile = 1;
    return (s32)slSaveProc_CheckCard;
}

s32 slExit(void) {
    slSetInfo(0);
    gslDrawFlagFile = 0;
    gslDrawFlagBg = 0;
    return 0xffffffff;
}

s32 slExitToTitleSub(void) {
    s32 argument = 0;

    if (dds3SearchProcessName(D_005328C0) != NULL) {
        return -1;
    }
    if (slDDS3To31Flg != 0) {
        return -1;
    }
    dds3AdminiCallSequence(2, &argument, 4, 0);
    return 0;
}

s32 slExitToTitle(void) {
    return slExitToTitleSub();
}

s32 slExitToTitle2(void) {
    s32 argument = 2;

    gslExitType = 1;
    if (dds3SearchProcessName(D_005328C0) != NULL) {
        return -1;
    }
    if (slDDS3To31Flg != 0) {
        return -1;
    }
    dds3AdminiCallSequence(2, &argument, 4, 0);
    return 0;
}

s32 slCardChk(void) {
    sdfMcSetCardStat(mccb);
    sdfMcClearSlotFlags(mccb, 0);
    sdfMcClearSlotFlags(mccb, 1);
    sdfMcClearSlotFlags(mccb, 2);
    sdfMcClearSlotFlags(mccb, 3);
    sdfMcClearSlotFlags(mccb, 4);
    slSetInfo(1);
    gslErrorCode = 0;
    gslDrawFlagFile = 0;
    return slTop();
}

s32 slFileSel(void) {
    s32 i;

    if (gslMode == 0) {
        slSetInfo(0xB);
    } else {
        slSetInfo(0xC);
    }
    gSlFileErroeFlg = 0;
    gslDrawFlagFile = 1;
    for (i = 0; i < 5; i++) {
        u32 flags = sdfMcGetSlotFlags(mccb, i);

        slTopMenuSel[i] = 0;
        if (flags & SL_SLOT_FLAG_CAN_READ_SAVE) {
            if (flags & SL_SLOT_FLAG_DIRECTORY_READY) {
                if (flags & SL_SLOT_FLAG_DELETE_BEFORE_WRITE) {
                    slTopMenuSel[i] = 1;
                }
            }
        } else {
            if (flags & SL_SLOT_FLAG_DIRECTORY_READY) {
                if (!(flags & SL_SLOT_FLAG_DELETE_BEFORE_WRITE)) {
                    slTopMenuSel[i] = 2;
                }
            }
        }
    }
    sdfMcRegetInfo(mccb);
    return (s32)slFileSelCalc;
}

s32 slCancel(void) {
    if (gslMode == 1 && dds3SearchProcessName(D_005328C0) == NULL) {
        return slExitToTitle2();
    }
    slSetInfo(0);
    return slExitWait(&slExit);
}

s32 slSaveProc1st(void) {
    D_00551D44 = 0;
    baseIconFileHandle = (u32)fileLoad(D_005328D0);
    return slTop();
}

s32 slLoadProc1st(void) {
    return slTop();
}

s32 slEndingProc1st(void) {
    D_00551D44 = 0;
    baseIconFileHandle = (u32)fileLoad(D_005328D0);
    return slEndingTop();
}

s32 slBootProc1st(void) {
    sdfMcRegetInfo(0);
    gSlNowCardCheck = 1;
    gslCheckCardWait = 0;
    slSetInfo(0);
    gslDrawFlagFile = 0;
    return slBootTop();
}

s32 slSaveProc_CheckCard(void) {
    if (sdfMcFuncGetInfo1() == 0) {
        return 0;
    }
    if (gslCheckCardWait > 0) {
        gslCheckCardWait--;
        sdfMcRegetInfo(mccb);
        return 0;
    }
    switch (sdfMcGetFileInfoReady(mccb)) {
    case 0:
        return slNoCard();
    case 2:
        if (gslMode == 0) {
            sdfMcSetCardStat(mccb);
            sdfMcClearSlotFlags(mccb, 0);
            sdfMcClearSlotFlags(mccb, 1);
            sdfMcClearSlotFlags(mccb, 2);
            sdfMcClearSlotFlags(mccb, 3);
            sdfMcClearSlotFlags(mccb, 4);
            slSetInfo(0);
            gslErrorCode = 0;
            slQueryYesNoMsg = 2;
            return slQueryYesNo(&slFormatStart, &slFormatCancel, 1);
        }
        slSetInfo(0);
        gslErrorCode = 7;
        gslErrorKeyFlag = 0;
        sdfMcRegetInfo(mccb);
        return (s32)slErrorWaitMc;
    case 3:
        sdfMcSetCardStat(mccb);
        sdfMcClearSlotFlags(mccb, 0);
        sdfMcClearSlotFlags(mccb, 1);
        sdfMcClearSlotFlags(mccb, 2);
        sdfMcClearSlotFlags(mccb, 3);
        sdfMcClearSlotFlags(mccb, 4);
        slSetInfo(0);
        gslErrorCode = 2;
        gslErrorKeyFlag = 0;
        sdfMcRegetInfo(mccb);
        return (s32)slErrorWaitMc;
    case 1:
        return slChooseData();
    }
    return 0;
}

s32 sl1Ken(void) {
    s32 i = 0;
    s32 count = 0;

    do {
        u32 flags = sdfMcGetSlotFlags(mccb, i);

        i++;
        if (flags & SL_SLOT_FLAG_CAN_READ_SAVE) {
            if (flags & SL_SLOT_FLAG_DIRECTORY_READY) {
                if (flags & SL_SLOT_FLAG_DELETE_BEFORE_WRITE) {
                    count++;
                }
            }
        }
    } while (i < 5);
    return count;
}

s32 slSizeChk(void) {
    return sdfMcGetFreeBytes(mccb) > 0x293FF;
}

s32 slBootProc_CheckCard(void) {
    s32 result = sdfMcFuncGetInfo1();
    if (result == 0) {
        return result;
    }
    if (gslCheckCardWait > 0) {
        gslCheckCardWait--;
        sdfMcRegetInfo(mccb);
        return 0;
    }
    switch (sdfMcGetFileInfoReady(mccb)) {
    case 0:
    case 3:
        return slBootError();
    case 2:
        return slExitToTitle();
    case 1:
        if (slSizeChk() != 0) {
            return slExitToTitle();
        }
        return (s32)slBootRetry();
    default:
        return 0;
    }
}

s32 slBootProc_CheckCardR(void) {
    if (sdfMcFuncGetInfo1() == 0) {
        return 0;
    }
    if (gslCheckCardWait > 0) {
        gslCheckCardWait--;
        sdfMcRegetInfo(mccb);
        return 0;
    }
    switch (sdfMcGetFileInfoReady(mccb)) {
    case 0:
    case 3:
        return slBootError();
    case 2:
        return slExitToTitle();
    case 1:
        if (slSizeChk() != 0) {
            return slExitToTitle();
        }
        return slChooseData_B();
    default:
        return 0;
    }
}

s32 slErrorWaitMc(void) {
    gslDrawFlagFile = 0;
    if (gslErrorKeyFlag == 0) {
        if (slGetPadTrig(dds3PadButtonState[0x21] < 0) ||
            slGetPadTrig(dds3PadButtonState[0x23] < 0)) {
            p2sdPlaySE(0xA, 0x7F, 0x3F);
            gslErrorKeyFlag = 1;
        }
    }
    if (sdfMcFuncGetInfo1() == 0) {
        goto done;
    }
    {
        if (gslErrorKeyFlag == 1) {
            gslErrorCode = 0;
            gslDrawFlagFile = 0;
            if (gSlEndingMode != 0) {
                return slEndingExit();
            }
            if (gslMode == 0) {
                return slExitWait(slExit);
            }
            return slExitToTitle2();
        }
        switch (sdfMcGetFileInfoReady(mccb)) {
        case 1:
            if (gslErrorCode == 1) {
            sdfMcSetCardStat(mccb);
            sdfMcClearSlotFlags(mccb, 0);
            sdfMcClearSlotFlags(mccb, 1);
            sdfMcClearSlotFlags(mccb, 2);
            sdfMcClearSlotFlags(mccb, 3);
            sdfMcClearSlotFlags(mccb, 4);
                slSetInfo(1);
                gslErrorCode = 0;
                gslDrawFlagFile = 0;
                return slTop();
            }
            sdfMcRegetInfo(mccb);
            goto done;
        case 0:
            sdfMcSetCardStat(mccb);
            sdfMcClearSlotFlags(mccb, 0);
            sdfMcClearSlotFlags(mccb, 1);
            sdfMcClearSlotFlags(mccb, 2);
            sdfMcClearSlotFlags(mccb, 3);
            sdfMcClearSlotFlags(mccb, 4);
            if (gslErrorCode != 1) {
                gslErrorCode = 1;
                gslErrorKeyFlag = 0;
            }
            sdfMcRegetInfo(mccb);
            goto done;
        case 2:
            if (gslMode == 0) {
            sdfMcSetCardStat(mccb);
            sdfMcClearSlotFlags(mccb, 0);
            sdfMcClearSlotFlags(mccb, 1);
            sdfMcClearSlotFlags(mccb, 2);
            sdfMcClearSlotFlags(mccb, 3);
            sdfMcClearSlotFlags(mccb, 4);
                gslErrorCode = 0;
                slQueryYesNoMsg = 2;
                return slQueryYesNo(slFormatStart, slFormatCancel, 1);
            }
            if (gslErrorCode != 7) {
            sdfMcSetCardStat(mccb);
            sdfMcClearSlotFlags(mccb, 0);
            sdfMcClearSlotFlags(mccb, 1);
            sdfMcClearSlotFlags(mccb, 2);
            sdfMcClearSlotFlags(mccb, 3);
            sdfMcClearSlotFlags(mccb, 4);
                gslErrorCode = 7;
                gslErrorKeyFlag = 0;
            }
            sdfMcRegetInfo(mccb);
            goto done;
        case 3:
            if (gslErrorCode != 2) {
            sdfMcSetCardStat(mccb);
            sdfMcClearSlotFlags(mccb, 0);
            sdfMcClearSlotFlags(mccb, 1);
            sdfMcClearSlotFlags(mccb, 2);
            sdfMcClearSlotFlags(mccb, 3);
            sdfMcClearSlotFlags(mccb, 4);
                gslErrorCode = 2;
                gslErrorKeyFlag = 0;
            }
            sdfMcRegetInfo(mccb);
            goto done;
        }
    }
done:
    return 0;
}

s32 slErrorWaitToFileMc(void) {
    if (gslErrorKeyFlag == 0) {
        if (slGetPadTrig(dds3PadButtonState[0x21] < 0) ||
            slGetPadTrig(dds3PadButtonState[0x23] < 0)) {
            p2sdPlaySE(8, 0x7F, 0x3F);
            gslErrorKeyFlag = 1;
        }
    }
    if (sdfMcFuncGetInfo1() != 0) {
        if (gslErrorKeyFlag == 1) {
            gslErrorCode = 0;
            return slFileSel();
        }
        if (sdfMcGetFileInfoReady(mccb) != 1) {
            gslErrorCode = 0;
            gslDrawFlagFile = 0;
            return slTop();
        }
        sdfMcRegetInfo(mccb);
    }
    return 0;
}

s32 slErrorWait(void) {
    if (slGetPadTrig(dds3PadButtonState[0x21] < 0) ||
        slGetPadTrig(dds3PadButtonState[0x23] < 0)) {
        p2sdPlaySE(8, 0x7F, 0x3F);
        gslErrorCode = 0;
        gslDrawFlagFile = 0;
        return slTop();
    }
    return 0;
}

s32 slExitWaitProc(void) {
    s32 remaining = slExitWaitCnt - 1;
    slExitWaitCnt = remaining;
    if (remaining <= 0) {
        if (slHitAnyKeyNextFunc == -1) {
            return -1;
        }
        return ((s32 (*)())slExitWaitNextFunc)();
    }
    return 0;
}

s32 slHitAnyKeyNoChk(u32 nextFunc) {
    slHitAnyKeyNextFunc = nextFunc;
    gslErrorKeyFlag = 0;
    return (s32)slHitAnyKeyNoChkProc;
}

s32 slHitAnyKeyNoChkProc(void) {
    if (slGetPadTrig(dds3PadButtonState[0x21] < 0) ||
        slGetPadTrig(dds3PadButtonState[0x23] < 0)) {
        slSetInfo(0);
        gslErrorCode = 0;
        gslDrawFlagFile = 0;
        if (slHitAnyKeyNextFunc == -1) {
            return slExitWait((void *)-1);
        }
        return ((s32 (*)())slHitAnyKeyNextFunc)();
    }
    return 0;
}

s32 slHitAnyKey(u32 nextFunc) {
    slHitAnyKeyNextFunc = nextFunc;
    gslErrorKeyFlag = 0;
    sdfMcRegetInfo(mccb);
    return (s32)slHitAnyKeyProc;
}

s32 slHitAnyKeyProc(void) {
    if (gslErrorKeyFlag == 0) {
        if (slGetPadTrig(dds3PadButtonState[0x21] < 0) ||
            slGetPadTrig(dds3PadButtonState[0x23] < 0)) {
            p2sdPlaySE(8, 0x7F, 0x3F);
            gslErrorKeyFlag = 1;
        }
    }
    if (sdfMcFuncGetInfo1() == 0) {
        goto done;
    }
    {
        if (gslErrorKeyFlag == 1) {
            slSetInfo(0);
            gslErrorCode = 0;
            gslDrawFlagFile = 0;
            if (slHitAnyKeyNextFunc == -1) {
                return slExitWait((void *)-1);
            }
            return ((s32 (*)())slHitAnyKeyNextFunc)();
        }
        switch (sdfMcGetFileInfoReady(mccb)) {
        case 1:
            if (gslErrorCode == 1) {
            sdfMcSetCardStat(mccb);
            sdfMcClearSlotFlags(mccb, 0);
            sdfMcClearSlotFlags(mccb, 1);
            sdfMcClearSlotFlags(mccb, 2);
            sdfMcClearSlotFlags(mccb, 3);
            sdfMcClearSlotFlags(mccb, 4);
                slSetInfo(1);
                gslErrorCode = 0;
                gslDrawFlagFile = 0;
                return slTop();
            }
            sdfMcRegetInfo(mccb);
            goto done;
        case 0:
            return slNoCard();
        case 2:
            if (gslMode == 0) {
            sdfMcSetCardStat(mccb);
            sdfMcClearSlotFlags(mccb, 0);
            sdfMcClearSlotFlags(mccb, 1);
            sdfMcClearSlotFlags(mccb, 2);
            sdfMcClearSlotFlags(mccb, 3);
            sdfMcClearSlotFlags(mccb, 4);
                slSetInfo(0);
                gslErrorCode = 0;
                slQueryYesNoMsg = 2;
                return slQueryYesNo(slFormatStart, slFormatCancel, 1);
            }
            if (gslErrorCode != 7) {
            sdfMcSetCardStat(mccb);
            sdfMcClearSlotFlags(mccb, 0);
            sdfMcClearSlotFlags(mccb, 1);
            sdfMcClearSlotFlags(mccb, 2);
            sdfMcClearSlotFlags(mccb, 3);
            sdfMcClearSlotFlags(mccb, 4);
                slSetInfo(0);
                gslErrorCode = 7;
                gslErrorKeyFlag = 0;
            }
            sdfMcRegetInfo(mccb);
            goto done;
        case 3:
            if (gslErrorCode != 2) {
            sdfMcSetCardStat(mccb);
            sdfMcClearSlotFlags(mccb, 0);
            sdfMcClearSlotFlags(mccb, 1);
            sdfMcClearSlotFlags(mccb, 2);
            sdfMcClearSlotFlags(mccb, 3);
            sdfMcClearSlotFlags(mccb, 4);
                slSetInfo(0);
                gslErrorCode = 2;
                gslErrorKeyFlag = 0;
            }
            sdfMcRegetInfo(mccb);
            goto done;
        }
    }
done:
    return 0;
}

s32 slChooseData_getCardInfo(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    mcMakeFileName(&buf[1], slChooseSaveIndex);
    sdfMcFuncChdir(mccb, (const char *)buf);
    return (s32)slChooseData_getCardInfo2;
}

s32 slChooseData_getCardInfo2(void) {
    slMcResult_t t = sdfMcFuncChdir1();

    if (t == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (t == SL_MC_RESULT_SUCCESS) {
        sdfMcSetSlotFlags(mccb, slChooseSaveIndex, SL_SLOT_FLAG_DIRECTORY_READY);
        sdfMcFuncGetDir(mccb, D_005328E0, D_005696C0, 1);
        return (s32)slChooseData_getCardInfo3;
    }
    if (t == SL_MC_RESULT_ERROR) {
        return slNoCard();
    }
    return slHeadLoad();
}

s32 slChooseData_getCardInfo3(void) {
    s32 value;
    slMcResult_t status = sdfMcFuncGetDir1(&value);
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        if (value == 1) {
            sdfMcSetSlotFlags(mccb, slChooseSaveIndex,
                                SL_SLOT_FLAG_CAN_READ_SAVE | SL_SLOT_FLAG_DELETE_BEFORE_WRITE);
        }
        return slHeadLoad();
    }
    if (status == SL_MC_RESULT_ERROR) {
        return slNoCard();
    }
    return slHeadLoad();
}

s32 slChooseData_getCardInfoNext(void) {
    s32 index = slChooseSaveIndex + 1;

    slChooseSaveIndex = index;
    if (index == 5) {
        sdfMcClearCardStat(mccb);
        slChooseSaveIndex = 0;
        gSlNowCardCheck = 0;
        if (sl1Ken() == 0) {
            if (gslMode != 0) {
                gslDrawFlagFile = 0;
                slSetInfo(0);
                gslErrorCode = 7;
                gslErrorKeyFlag = 0;
                sdfMcRegetInfo(mccb);
                return (s32)slErrorWaitMc;
            }
            if (slSizeChk() == 0) {
                slSetInfo(0);
                gslDrawFlagFile = 0;
                gslErrorCode = 5;
                gslErrorKeyFlag = 0;
                sdfMcRegetInfo(mccb);
                return (s32)slErrorWaitMc;
            }
        }
        slSetInfo(0);
        return slFileSel();
    }
    return slChooseData_getCardInfo();
}

s32 slChooseData(void) {
    s32 i;

    if (sdfMcGetStatus(mccb) != 0) {
        slSetInfo(1);
        slChooseSaveIndex = 0;
        for (i = 0; i < 5; i++) {
            slTopMenuSel[i] = 0;
            sdfMcClearSlotFlags(mccb, i);
        }
        return slChooseData_getCardInfo();
    }
    gSlNowCardCheck = 0;
    slSetInfo(0);
    return slFileSel();
}

s32 slChooseData_getCardInfo_B(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    mcMakeFileName(&buf[1], slChooseSaveIndex);
    sdfMcFuncChdir(mccb, (const char *)buf);
    return (s32)slChooseData_getCardInfo2_B;
}

s32 slChooseData_getCardInfo2_B(void) {
    slMcResult_t t = sdfMcFuncChdir1();

    if (t == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (t == SL_MC_RESULT_SUCCESS) {
        sdfMcSetSlotFlags(mccb, slChooseSaveIndex, SL_SLOT_FLAG_DIRECTORY_READY);
        sdfMcFuncGetDir(mccb, D_005328E0, D_005696C0, 1);
        return (s32)slChooseData_getCardInfo3_B;
    }
    if (t == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslDrawFlagFile = 0;
        gslErrorCode = 0;
        return slBootError();
    }
    return slChooseData_getCardInfoNext_B();
}

s32 slChooseData_getCardInfo3_B(void) {
    s32 value;
    slMcResult_t status = sdfMcFuncGetDir1(&value);
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        if (value == 1) {
            sdfMcSetSlotFlags(mccb, slChooseSaveIndex,
                                SL_SLOT_FLAG_CAN_READ_SAVE | SL_SLOT_FLAG_DELETE_BEFORE_WRITE);
        }
        return slChooseData_getCardInfoNext_B();
    }
    if (status == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslDrawFlagFile = 0;
        gslErrorCode = 0;
        return slBootError();
    }
    return slChooseData_getCardInfoNext_B();
}

s32 slChooseData_getCardInfoNext_B(void) {
    s32 slot = slChooseSaveIndex;
    s32 buttons = sdfMcGetSlotFlags(mccb, slot);

    slot++;
    if (buttons & SL_SLOT_FLAG_CAN_READ_SAVE) {
        if (buttons & SL_SLOT_FLAG_DIRECTORY_READY) {
            if (buttons & SL_SLOT_FLAG_DELETE_BEFORE_WRITE) {
                slSetInfo(0);
                gslDrawFlagFile = 0;
                gslErrorCode = 0;
                return slExitToTitle();
            }
        }
    }
    slChooseSaveIndex = slot;
    if (slot == 5) {
        slSetInfo(0);
        gslDrawFlagFile = 0;
        gslErrorCode = 0;
        return slBootError2();
    }
    return slChooseData_getCardInfo_B();
}

s32 slChooseData_B(void) {
    s32 i = 0;
    s32 *selection;

    slSetInfo(1);
    gslDrawFlagFile = 0;
    slChooseSaveIndex = 0;
    sdfMcSetCardStat(mccb);
    selection = slTopMenuSel;
    do {
        *selection = 0;
        selection++;
        sdfMcClearSlotFlags(mccb, i);
        i++;
    } while (i < 5);
    return slChooseData_getCardInfo_B();
}

s32 slSaveData(void) {
    u8 name[0x50];
    u32 entry = mccb;
    s32 slot = sdfMcGetSlotIndex(entry);
    sdfMcGetSlotFlags(entry, slot);
    slSetInfo(2);
    sdfMcSetCardStat(entry);
    if (sdfMcGetSlotFlags(entry, slot) & SL_SLOT_FLAG_DIRECTORY_READY) {
        return slSaveData_chdir();
    }
    name[0] = '/';
    mcMakeFileName(name + 1, slot);
    sdfMcFuncMkdir(entry, (const char *)name);
    return (s32)slSaveProc_Write_mkdir;
}

s32 slSaveData_chdir(void) {
    u8 buf[0x50];
    u32 entry = mccb;
    s32 slot = sdfMcGetSlotIndex(entry);

    buf[0] = 0x2F;
    mcMakeFileName(&buf[1], slot);
    sdfMcFuncChdir(entry, (const char *)buf);
    return (s32)slSaveProc_Write_chdir;
}

s32 slSaveProc_Write_mkdir(void) {
    slMcResult_t status = sdfMcFuncMkdir1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        sdfMcSetSlotFlags(mccb, slFileSelError, SL_SLOT_FLAG_DIRECTORY_READY);
        return slSaveData_chdir();
    }
    slSetInfo(0);
    gslErrorCode = 4;
    return (s32)slErrorWait;
}

s32 slSaveProc_Write_chdir(void) {
    slMcResult_t t = sdfMcFuncChdir1();

    if (t == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (t == SL_MC_RESULT_SUCCESS) {
        return slSaveData_deleteIconSys();
    }
    if (t == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return (s32)slErrorWait;
    }
    return 0;
}

s32 slSaveProc_Write_deleteIconSys(void) {
    slMcResult_t t = sdfMcFuncDelete1();

    if (t == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (t == SL_MC_RESULT_SUCCESS) {
        return slSaveData_writeFile();
    }
    if (t == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return (s32)slErrorWait;
    }
    return 0;
}

s32 slSaveProc_doNothing(void) {
    sdfMcSetSlotFlags(mccb, slFileSelError, SL_SLOT_FLAG_CAN_READ_SAVE);
    sdfMcSetSlotFlags(mccb, slFileSelError, SL_SLOT_FLAG_DELETE_BEFORE_WRITE);
    slSetInfo(4);
    return slHitAnyKey((u32)slFileSel);
}

s32 slSaveWriteEnd_iconSys(void) {
    slKillLoadProcess();
    return (s32)slSaveProc_doNothing;
}

s32 slSaveWriteEnd_baseIcon(void) {
    D_00316B00[0xE2] = -0x7E;
    D_00316B00[0xE3] = slFileSelError + 0x50;
    D_00316B00[0xE4] = 0;
    D_00316B00[0xE5] = 0;
    return slSaveWriteFile(D_005328E0, &D_0054F06C, &D_0054F070, slSaveWriteEnd_iconSys, 0);
}

s32 slSaveWriteEnd_saveData(void) {
    return slSaveWriteFile(D_005328F0, &D_00551D48, &D_00551D4C,
                          slSaveWriteEnd_baseIcon, &baseIconFileHandle);
}

/* NON_MATCHING: 106 of 142 words, exact size; one register renaming through the record copy. */
#ifdef NON_MATCHING
s32 slSaveData_writeFile(void) {
    char path[0x50];
    GameFileInfo_t *hdr;
    u8 *rec;
    s32 slot = sdfMcGetSlotIndex(mccb);

    mcMakeFileName(path, slot);
    SL_FILE_HEAD->verstr[0] = 'V';
    SL_FILE_HEAD->verstr[1] = 'E';
    SL_FILE_HEAD->verstr[2] = 'R';
    SL_FILE_HEAD->ver = 5;
    SL_FILE_HEAD->TmnlMode = DDS3_GBWK->previewX;
    SL_FILE_HEAD->TmnlId = DDS3_GBWK->previewY;
    SL_FILE_HEAD->Lv = datGetPlayerLevel();
    SL_FILE_HEAD->PlayTime = DDS3_GBWK->playtime;
    if (evtBitCheck(0xAF2) != 0) {
        SL_FILE_HEAD->mode = 1;
    } else {
        SL_FILE_HEAD->mode = 0;
    }
    SL_FILE_HEAD->work = *(slFileWork_t *)((u8 *)DDS3_GBWK + 0x6604);
    SL_FILE_HEAD->autoMapOpen = *((u8 *)DDS3_GBWK + 0x6654);
    hdr = SL_FILE_HEAD;
    rec = SL_REC[slot];
    rec[0] = hdr->verstr[0];
    SL_REC[slot][1] = hdr->verstr[1];
    SL_REC[slot][2] = hdr->verstr[2];
    rec[3] = hdr->ver;
    rec[4] = hdr->TmnlMode;
    rec[5] = hdr->TmnlId;
    rec[6] = hdr->Lv;
    *(s32 *)&SL_REC[slot][8] = hdr->PlayTime;
    *(u16 *)&rec[12] = hdr->endcnt;
    *(u16 *)&rec[14] = hdr->newfile;
    rec[7] = hdr->mode;
    *(slFileName_t *)&SL_REC[slot][0x10] = *(slFileName_t *)&hdr->work;
    SL_REC[slot][0x24] = hdr->autoMapOpen;
    D_00551D30 = (u32)DDS3_GBWK;
    D_00551D34 = 0x1A510;
    if (gSlSaveMode == 0 || gSlSaveMode == 2) {
        return slSaveWriteFile(path, &D_00551D30, &D_00551D34, slSaveWriteEnd_saveData, 0);
    }
    return slSaveWriteFile(path, &D_00551D30, &D_00551D34, slSaveWriteEnd_baseIcon, 0);
}
#else
INCLUDE_ASM(const s32, "mc/slMain", slSaveData_writeFile);
#endif

s32 slSaveData_deleteIconSys(void) {
    u32 entry = mccb;
    s32 slot = sdfMcGetSlotIndex(entry);
    u32 flags = sdfMcGetSlotFlags(entry, slot);
    if (!(flags & SL_SLOT_FLAG_DELETE_BEFORE_WRITE)) {
        return slSaveData_writeFile();
    }
    sdfMcFuncDelete(entry, D_005328E0);
    return (s32)slSaveProc_Write_deleteIconSys;
}

s32 slSaveWriteFile(const char *filename, u32 *buff, u32 *size, void *endFunc, u32 *srcPtr) {
    slSaveWriteFile_buff = buff;
    slSaveWriteFile_size = size;
    slSaveWriteFile_endFunc = endFunc;
    slSaveWriteFile_dataWaitPtr = srcPtr;
    sdfMcFuncOpen(mccb, filename, 0x203);
    return (s32)slSaveProc_openFile;
}

s32 slSaveProc_openFile(void) {
    slMcResult_t status = sdfMcFuncOpen1(&D_00551D50);

    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        if (slSaveWriteFile_dataWaitPtr != NULL && *slSaveWriteFile_dataWaitPtr != 0) {
            return (s32)slSaveProc_waitDataReady;
        }
        sdfMcFuncWrite(D_00551D50, *slSaveWriteFile_buff, *slSaveWriteFile_size);
        return (s32)slSaveProc_writeFile;
    }
    if (status == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return (s32)slErrorWait;
    }
    return 0;
}

s32 slSaveProc_writeFile(void) {
    slMcResult_t status = sdfMcFuncWrite1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        sdfMcFuncClose(D_00551D50);
        return (s32)slSaveProc_closeFile;
    }
    if (status == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return (s32)slErrorWait;
    }
    return 0;
}

s32 slSaveProc_closeFile(void) {
    slMcResult_t status = sdfMcFuncClose1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        return ((s32 (*)())slSaveWriteFile_endFunc)();
    }
    if (status == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return (s32)slErrorWait;
    }
    return 0;
}

s32 slSaveProc_waitDataReady(void) {
    if (baseIconFileHandle != 0) {
        return 0;
    }
    sdfMcFuncWrite(D_00551D50, *slSaveWriteFile_buff, *slSaveWriteFile_size);
    return (s32)slSaveProc_writeFile;
}

s32 slProc_Format(void) {
    slMcResult_t status = sdfMcFuncFormat1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    dds3SoftResetOn();
    if (status < 0) {
        slSetInfo(0);
        gslErrorCode = 6;
        return (s32)slErrorWait;
    }
    slSetInfo(6);
    return slHitAnyKey((u32)slCardChk);
}

s32 slFormatStart(void) {
    slSetInfo(5);
    dds3SoftResetOff();
    sdfMcFuncFormat(mccb);
    return (s32)slProc_Format;
}

s32 slFormatCancel(void) {
    if (gSlEndingMode != 0) {
        slQueryYesNoMsg = 7;
        return slQueryYesNo(slEndingExit, slTop, 1);
    }
    slQueryYesNoMsg = 7;
    return slQueryYesNo(slExit, slTop, 1);
}

s32 slLoadData(void) {
    char path[0x50];
    u32 ctx = mccb;
    s32 slot = sdfMcGetSlotIndex(ctx);
    u32 attr = sdfMcGetSlotFlags(ctx, slot);
    u32 len;

    slSetInfo(3);
    if ((attr & SL_SLOT_FLAG_CAN_READ_SAVE) == 0) {
        return slFileSel();
    }
    slVersionConvert_Step1();
    path[0] = '/';
    mcMakeFileName(&path[1], slot);
    len = strlen(&path[1]);
    path[len + 1] = '/';
    memcpy(&path[len + 2], &path[1], len);
    path[len * 2 + 2] = 0;
    sdfMcFuncOpen(ctx, path, 1);
    return (s32)slLoadProc_openFile;
}

s32 slLoadProc_openFile(void) {
    slMcResult_t status = sdfMcFuncOpen1(&D_00551D50);
    u32 size;

    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    size = slVersionConvert_GetSize();
    slLoadFileFd = sdfMemAllocDirect(size);
    slLoadFileHandle = sdfMemLock(slLoadFileFd);
    if (status == SL_MC_RESULT_SUCCESS) {
        sdfMcFuncRead(D_00551D50, (u32)slLoadFileHandle, size);
        return (s32)slLoadProc_readFile;
    }
    slSetInfo(0);
    gslErrorCode = 3;
    return (s32)slErrorWait;
}

s32 slLoadProc_readFile(void) {
    slMcResult_t status = sdfMcFuncRead1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        sdfMcFuncClose(D_00551D50);
        return (s32)slLoadProc_closeFile;
    }
    sdfMemFreeDirect(slLoadFileFd);
    slSetInfo(0);
    gslErrorCode = 3;
    return (s32)slErrorWait;
}

s32 slLoadProc_closeFile(void) {
    slMcResult_t status = sdfMcFuncClose1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        slVersionConvert_Step3();
        sdfMemFreeDirect(slLoadFileFd);
        D_005516C1 = 1;
        slKillLoadProcess();
        if (dds3ConfigChanged() == 0) {
            dds3ConfigSaveBackup();
        } else {
            dds3ConfigRestoreBackup();
        }
        slSetInfo(13);
        return slHitAnyKeyNoChk(-1);
    }
    sdfMemFreeDirect(slLoadFileFd);
    slSetInfo(0);
    gslErrorCode = 3;
    return (s32)slErrorWait;
}

INCLUDE_ASM(const s32, "mc/slMain", slFileSelCalc);

INCLUDE_RODATA(const s32, "mc/slMain", D_005328C0);

INCLUDE_RODATA(const s32, "mc/slMain", D_005328D0);

INCLUDE_RODATA(const s32, "mc/slMain", D_005328E0);

INCLUDE_RODATA(const s32, "mc/slMain", D_005328F0);

INCLUDE_ASM(const s32, "mc/slMain", slFileSelDraw);

s32 slCalc(dds3ProcessID_t *id) {
    s32 next;
    u32 job;
    s32 (*cur)();
    gslAnimCnt++;
    slDraw((s32)id);
    next = proc_func_ans(id);
    if (next == -1) {
        return next;
    }
    job = baseIconFileHandle;
    cur = proc_func_ans;
    if (next != 0) {
        cur = (s32 (*)())next;
    }
    proc_func_ans = cur;
    if (job != 0 && pacFileCheckEnd((fileRequest_t *)job) != 0) {
        baseIconFileHandle = 0;
        D_00551D44 = fileGetMemHandle((fileRequest_t *)(u32)job);
        D_00551D48 = (u32)fileGetLoadPtr((fileRequest_t *)(u32)job);
        D_00551D4C = fileGetLoadSize((fileRequest_t *)(u32)job);
        fileHandleFree((fileRequest_t *)(u32)job);
    }
    return 0;
}

s32 slDraw(s32 id) {
    rsetOtOfs(0x46);
    rsetAlpha(0);
    rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    if (gslDrawFlagBg != 0) {
        DrwSprite2D(0, 0, 0x200, 0x1C0, 0, 0, 0x100, 0x80, 0x80808080, _saveUI);
        aDrawBoxG(0, 0x2F, 0x89, 0x9D, 0x2B, 0x30, 0x40, 0x73, 0x2B, 0x30, 0x40, 0x73,
                  0x2B, 0x30, 0x40, 0x73, 0x2B, 0x30, 0x40, 0x73);
        aDrawBoxG(0x89, 0x2F, 0x82, 0x9D, 0x2B, 0x30, 0x40, 0x73, 0x2B, 0x30, 0x40, 0,
                  0x2B, 0x30, 0x40, 0, 0x2B, 0x30, 0x40, 0x73);
        aDrawBoxG(0x177, 0x2F, 0x89, 0x9D, 0x2B, 0x30, 0x40, 0x73, 0x2B, 0x30, 0x40, 0x73,
                  0x2B, 0x30, 0x40, 0x73, 0x2B, 0x30, 0x40, 0x73);
        aDrawBoxG(0xF5, 0x2F, 0x82, 0x9D, 0x2B, 0x30, 0x40, 0, 0x2B, 0x30, 0x40, 0x73,
                  0x2B, 0x30, 0x40, 0x73, 0x2B, 0x30, 0x40, 0);
    }
    if (gslDrawFlagFile != 0) {
        slFileSelDraw(id);
    }
    slDrawInfo();
    slQueryYesNoDraw();
    slDrawError();
    rsetAlpha(0);
    rsetTest(1, 5, 0x80, 3, 0, 0, 1, 2);
    return 0;
}

void fmProcTrigSequence(s32 mode) {
    sdfMemHandle_t *handle;
    void *data;
    GameFileInfo_t *hdr;
    void *scene;
    s32 on = 1;

    dds3SoftResetOff();
    slDDS3To31Flg = 0;
    handle = sdfDevLoadFileEx("/fld/f/pnl/save_01.tmx", &data, 0);
    slProcPtr = sdfTexSet(data);
    sdfMemFreeDirect(handle);
    handle = sdfDevLoadFileEx("/fld/f/pnl/save_02.tmx", &data, 0);
    _saveUI = sdfTexSet(data);
    sdfMemFreeDirect(handle);
    if (mode == 0) {
        gslDrawFlagBg = on;
        gSlEndingMode = 0;
        proc_func_ans = (s32 (*)(dds3ProcessID_t *))slSaveProc1st;
    } else if (mode == 1) {
        gSlEndingMode = 0;
        gslDrawFlagBg = on;
        proc_func_ans = (s32 (*)(dds3ProcessID_t *))slLoadProc1st;
    } else if (mode == 2) {
        gslDrawFlagBg = on;
        gSlEndingMode = on;
        evtBitOn(0x8A0);
        hdr = SL_FILE_HEAD;
        hdr->endcnt++;
        hdr->newfile = on;
        proc_func_ans = (s32 (*)(dds3ProcessID_t *))slEndingProc1st;
    } else {
        gSlEndingMode = 0;
        if (mode == 4) {
            gslDrawFlagBg = on;
            slDDS3To31Flg = 1;
            proc_func_ans = (s32 (*)(dds3ProcessID_t *))slLoadProc1st;
            mode = 1;
        } else {
            gslDrawFlagBg = 0;
            proc_func_ans = (s32 (*)(dds3ProcessID_t *))slBootProc1st;
        }
    }
    dds3StartProcess(D_00532948, 0x2B0A, 0, 0, slCalc,
                     (s32 (*)(dds3ProcessID_t *))slShutDownProcess, NULL);
    slProcShutDownFlg = 1;
    dds3FadeIn(0, 0, 0, 8);
    slSetInfo(0);
    gSlExitMode = 0;
    slQueryYesNoCursor = -1;
    gslMode = mode;
    gslInfoLoadFlg = 1;
    slExitWaitCnt = 0;
    gslFlushCnt = 0;
    gslExitType = 0;
    gslErrorCode = 0;
    gslErrorKeyFlag = 0;
    gslDrawFlagFile = 0;
    gSlSaveMode = 0;
    gslCheckCardWait = 0;
    gslAnimCnt = 0;
    gSlSelCnt = 0;
    gSlNowCardCheck = 0;
    gSlFileErroeFlg = 0;
    slQueryYesNoMsg = 0;
    slQueryYesNoKeyFlag = 0;
    gSlMcAccessCnt = 0;
    gSlWindowCnt = 0;
    sdfMcSetCardStat(mccb);
    sdfMcClearSlotFlags(mccb, 0);
    sdfMcClearSlotFlags(mccb, 1);
    sdfMcClearSlotFlags(mccb, 2);
    sdfMcClearSlotFlags(mccb, 3);
    sdfMcClearSlotFlags(mccb, 4);
    scene = dds3GetActiveScene();
    if (scene != NULL) {
        dds3VisibleSceneBasic(scene, 0);
    }
}

/* KillProcess passes this callback's process ID but discards its return. */
void slShutDownProcess(dds3ProcessID_t *id) {
    func_00157058();
}

/* ExitBootMc, ExitEndSave, and the neighboring exit path call this leaf. */
void func_00157050(void) {}

extern void sdfTexFreeDelayed(sdfTexHandle_t *handle);
extern void fileSync(fileRequest_t *fileHandle);
extern void sdfMemFreeHandleDirect(sdfMemHandle_t **aHandlePtr);
extern s32 dds3KillProcessName(char *name, s32 shutdowntimer);

/* Shared cleanup entry used by the callback wrapper and three exit paths. */
void func_00157058(void) {
    dds3Basic_t *scene;

    if (slProcShutDownFlg == 0) {
        return;
    }

    slProcShutDownFlg = 0;
    if (_saveUI != NULL) {
        sdfTexFreeDelayed(_saveUI);
        _saveUI = NULL;
    }
    if (slProcPtr != NULL) {
        sdfTexFreeDelayed(slProcPtr);
        slProcPtr = NULL;
    }
    if (dds3SearchProcessName(D_005328C0) == NULL) {
        scene = dds3GetActiveScene();
        if (scene != NULL) {
            dds3VisibleSceneBasic(scene, 1);
        }
    }
    if (baseIconFileHandle != 0) {
        fileSync((fileRequest_t *)baseIconFileHandle);
        D_00551D44 = fileGetMemHandle((fileRequest_t *)baseIconFileHandle);
        fileHandleFree((fileRequest_t *)baseIconFileHandle);
        baseIconFileHandle = 0;
    }
    sdfMemFreeHandleDirect(&D_00551D44);
    dds3KillProcessName(D_00532948, 1);
    dds3SoftResetOn();
}

u32 fmShutDown2(void) {
    return 1;
}

s32 fmProcessChk(void) {
    return dds3SearchProcessName(D_00532948) != 0;
}

u32 slGetExitType(void) {
    return gslExitType;
}

INCLUDE_ASM(const s32, "mc/slMain", slQueryYesNoProc);

s32 slQueryYesNo(void *yesFunc, void *noFunc, s32 yesNoDefault) {
    slQueryYesFunc = yesFunc;
    slQueryNoFunc = noFunc;
    slQueryYesNoCursor = yesNoDefault;
    slQueryYesNoKeyFlag = 0;
    if (slQueryYesNoMsg != 4 && slQueryYesNoMsg != 8) {
        sdfMcRegetInfo(mccb);
    }
    return (s32)slQueryYesNoProc;
}

INCLUDE_RODATA(const s32, "mc/slMain", D_00532948);

INCLUDE_ASM(const s32, "mc/slMain", slQueryYesNoDraw);

u32 slGetExitMode(void) {
    return gSlExitMode;
}

void slSetTmnlID(s16 type, s16 id) {
    dds3GlobalWork_t *state = DDS3_GBWK;

    state->previewX = type;
    state->previewY = id;
}
