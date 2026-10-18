#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "ps2asm.h"

/* fldDoorTable stride, 100 bytes; only the warp effect id is reached here. */
typedef struct {
    u8 unk00[0x12];
    s16 warpEff;
    u8 unk14[0x50];
} fldDoorData_t;

extern fldDoorData_t fldDoorTable[256];
extern s32 scriptlabel;
extern void *fldDrwZBuf;
extern f32 fldWmFastMove;
extern void *D_0054EC68;
extern s32 gfldPlayerHasiCnt;
extern s32 gfldPlayerDamegeCnt;
extern s32 fldPlayerStepDist;
extern s32 gSkyTblAnime;
extern s32 gSkyTblAnimeCntI;
extern s32 gSkyTblBaseIdx;
extern s32 D_002FC0EC __attribute__((section(".data")));
extern char D_0054EC80[];

extern void *sdfAllocAuto(u32 size);
extern void sdfFreeAutoDelayed(void *p);
extern void dds3EffectPartialMultiBlurDelete(void *p);
extern void fldSkySetTbl(s32 idx, s32 flag);
extern s32 encDrawProc(dds3ProcessID_t *id);
extern dds3ProcessID_t *dds3StartProcess(char *namestr, u32 priority, s32 starttimer,
                                         s32 flag, dds3ProcessFunc_t func, void *arg,
                                         void *parent);

typedef struct {
    s32 px;
    s32 py;
    s32 sx;
    s32 sy;
    u32 pz;
    u32 panelCol;
    u32 frameCol;
    s32 cx;
    s32 cy;
    s32 cz;
    sdfDrawTag_t *tag;
} dbgConsCtl_t;

extern sdfGrOt_t sdfGrOtTable[85];
extern s32 gencOn;
extern s32 gencMode;
extern s32 rsetFrameMaskOtOffset;
extern s32 gFilterAlpha;
extern s32 gFilterAnime;
extern s32 gFilterAnimeCntF;
extern s32 gFilterAnimeOfsA;
extern s32 fldCamResetOn;
extern s32 fldCamMiwatasi;
extern s32 fldElvCameraScene;
extern s32 fldWmWarpMode;
extern s32 gFldWapPanel;
extern s32 fldSkyAlphaOfs __attribute__((section(".data")));

extern void fldCamSetChaseCore(f32 height);

INCLUDE_ASM(const s32, "code_00128848", fldDrawInit);

sdfGrOt_t *fldDrawGetOt(void)
{
    return &sdfGrOtTable[rsetFrameMaskOtOffset];
}


INCLUDE_ASM(const s32, "code_00128848", DrwSprite2D);

INCLUDE_ASM(const s32, "code_00128848", DrwSprite2DC);

INCLUDE_ASM(const s32, "code_00128848", DrwSprite2DF);

INCLUDE_ASM(const s32, "code_00128848", fldPoint3To2);

INCLUDE_ASM(const s32, "code_00128848", fldPoint3To2H);

INCLUDE_ASM(const s32, "code_00128848", fldPoint3To2_A);

INCLUDE_ASM(const s32, "code_00128848", fldPoint3To2_B);

void rsetOtOfs(s32 ofs)
{
    rsetFrameMaskOtOffset = ofs;
}


INCLUDE_ASM(const s32, "code_00128848", rsetTexa);

INCLUDE_ASM(const s32, "code_00128848", rsetTest);

INCLUDE_ASM(const s32, "code_00128848", rsetAlpha);

INCLUDE_ASM(const s32, "code_00128848", aDrawLineStripPrim);

INCLUDE_ASM(const s32, "code_00128848", aDrawLineDirect);

INCLUDE_ASM(const s32, "code_00128848", aDrawBoxF);

INCLUDE_ASM(const s32, "code_00128848", aDrawBoxFZ);

INCLUDE_ASM(const s32, "code_00128848", aDrawBoxF_Direct);

INCLUDE_ASM(const s32, "code_00128848", aDrawTriG);

INCLUDE_ASM(const s32, "code_00128848", aDrawBoxG);

INCLUDE_ASM(const s32, "code_00128848", aDrawBoxGF);

INCLUDE_ASM(const s32, "code_00128848", func_0012A140);

INCLUDE_ASM(const s32, "code_00128848", drawSprite3D);

INCLUDE_ASM(const s32, "code_00128848", rsetFrameMaskRgbOn);

INCLUDE_ASM(const s32, "code_00128848", rsetFrameMaskRgbOff);

INCLUDE_ASM(const s32, "code_00128848", ooyTestLine3D);

INCLUDE_ASM(const s32, "code_00128848", fldDrawPoly3);

INCLUDE_ASM(const s32, "code_00128848", fldDrawPoly4);

INCLUDE_ASM(const s32, "code_00128848", fldDrawTexPoly4);

INCLUDE_ASM(const s32, "code_00128848", fldDrawFrameSprite);

INCLUDE_ASM(const s32, "code_00128848", fldDrawBokasi);

INCLUDE_ASM(const s32, "code_00128848", fldDrawFrameSpriteAlpha);

INCLUDE_ASM(const s32, "code_00128848", fldDrawPos);

INCLUDE_ASM(const s32, "code_00128848", fldDrawPosCol);

INCLUDE_ASM(const s32, "code_00128848", fldDraw2DBar);

void fldDrwZAlloc(void)
{
    if (fldDrwZBuf == NULL) {
        fldDrwZBuf = sdfAllocAuto(0x70000);
    }
}


void fldDrwZFree(void)
{
    if (fldDrwZBuf != NULL) {
        sdfFreeAutoDelayed(fldDrwZBuf);
        fldDrwZBuf = NULL;
    }
}


INCLUDE_ASM(const s32, "code_00128848", fldDrwZSave);

INCLUDE_ASM(const s32, "code_00128848", fldDrwZLoad);

INCLUDE_ASM(const s32, "code_00128848", fldKanjiDraw);

void LF(dbgConsCtl_t *cons)
{
    cons->cy += 0x60;
}


INCLUDE_ASM(const s32, "code_00128848", initDrawTag);

INCLUDE_ASM(const s32, "code_00128848", fldDbgPrint3D);

INCLUDE_ASM(const s32, "code_00128848", fldDbgPrint2D);

INCLUDE_ASM(const s32, "code_00128848", fldDbgPrint2DC);

INCLUDE_ASM(const s32, "code_00128848", fldDbgPrintNum2D);

INCLUDE_ASM(const s32, "code_00128848", fldDbgPrintNum2DH);

INCLUDE_ASM(const s32, "code_00128848", fldDbgPrintStr2D);

INCLUDE_ASM(const s32, "code_00128848", fldDbgPrintStr2DC);

INCLUDE_ASM(const s32, "code_00128848", fldDbgPrintStr2DCF);

INCLUDE_ASM(const s32, "code_00128848", fldDrawMaruKage);

s32 nbGetEncountOn(void)
{
    return gencOn;
}


void nbSetEncountMode(s32 mode)
{
    gencMode = mode;
}


INCLUDE_ASM(const s32, "code_00128848", encDrawProc);

void encFreeBlur(void)
{
    gencOn = 0;
    if (D_0054EC68 != NULL) {
        dds3EffectPartialMultiBlurDelete(D_0054EC68);
        D_0054EC68 = NULL;
    }
}


INCLUDE_ASM(const s32, "code_00128848", encStart);

void encInitProcess(void)
{
    dds3StartProcess(D_0054EC80, 0x2B0F, 0, 1, encDrawProc, NULL, NULL);
}


INCLUDE_ASM(const s32, "code_00128848", fldCalcAngleBetween);

INCLUDE_ASM(const s32, "code_00128848", GetSphericalPosMove);

INCLUDE_ASM(const s32, "code_00128848", fldCamCalcSlopeOffset);

INCLUDE_ASM(const s32, "code_00128848", fldCamSetLeng);

void fldCamResetNpc(void)
{
    fldCamResetOn = 0;
}


INCLUDE_ASM(const s32, "code_00128848", fldCamCalcSaka);

INCLUDE_ASM(const s32, "code_00128848", resetCamNormal);

INCLUDE_ASM(const s32, "code_00128848", resetCamNormal2);

INCLUDE_ASM(const s32, "code_00128848", fldCamGetPhokanParam);

INCLUDE_ASM(const s32, "code_00128848", fldCamUpdateFollow);

INCLUDE_ASM(const s32, "code_00128848", calcCamNormal);

INCLUDE_ASM(const s32, "code_00128848", calcCamDirLock);

INCLUDE_ASM(const s32, "code_00128848", calcCamNormalNPC);

INCLUDE_ASM(const s32, "code_00128848", resetCamTgtPlayer);

INCLUDE_ASM(const s32, "code_00128848", fldEventUpdateEx_Unity_2);

INCLUDE_ASM(const s32, "code_00128848", fldCamSetChaseCore);

void fldCamSetChase(void)
{
    fldCamSetChaseCore(0.0f);
}


void fldCamUpdateDefault(void)
{
    fldCamSetChaseCore(8.0f);
}


INCLUDE_ASM(const s32, "code_00128848", fldCamUpdateChase);

INCLUDE_ASM(const s32, "code_00128848", fldCamNowEveCamera);

INCLUDE_ASM(const s32, "code_00128848", calcCamSyukan);

INCLUDE_ASM(const s32, "code_00128848", fldSetElvCameraScene);

INCLUDE_ASM(const s32, "code_00128848", fldCamElvTask);

void fldClearElvCameraScene(void)
{
    fldElvCameraScene = 0;
}

INCLUDE_RODATA(const s32, "code_00128848", D_00531BF0);

INCLUDE_ASM(const s32, "code_00128848", fldCamMain);

INCLUDE_ASM(const s32, "code_00128848", fldCamSyukanSuriawase);

s32 fldCamMiwatasiChk(void)
{
    return fldCamMiwatasi;
}


INCLUDE_ASM(const s32, "code_00128848", fldCamNpcCamChk);

INCLUDE_ASM(const s32, "code_00128848", R360_LR);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerCalc360_8);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerCalc360_4);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerMirChk);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerMotion);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerMotionSe);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerCalc_Hit);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerCalc_WarpRun);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerCalcHasigo);

INCLUDE_RODATA(const s32, "code_00128848", D_00531C50);

INCLUDE_RODATA(const s32, "code_00128848", D_00531C60);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerCalc_Nml);

void fldPlayerCalc_WarpStart(void)
{
    fldWmWarpMode = 1;
}


void fldPlayerCalc_FastStart(void)
{
    if (fldWmWarpMode == 0) {
        fldWmWarpMode = 2;
        fldWmFastMove = 60.0f;
    }
}


INCLUDE_ASM(const s32, "code_00128848", fldPlayerCalc_Wm3);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerCalc_Wm4);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerCalc_Wm2);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerCalc_Nml_syukan);

INCLUDE_RODATA(const s32, "code_00128848", D_00531D08);

INCLUDE_RODATA(const s32, "code_00128848", D_00531D18);

INCLUDE_RODATA(const s32, "code_00128848", D_00531D28);

INCLUDE_RODATA(const s32, "code_00128848", D_00531D38);

INCLUDE_RODATA(const s32, "code_00128848", D_00531D48);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerCalc);

s32 fldPlayerChkAna(void)
{
    if (gfldPlayerHasiCnt == 0 && fldPlayerStepDist == 0 && gfldPlayerDamegeCnt == 0) {
        return 0;
    }
    return 1;
}


INCLUDE_ASM(const s32, "code_00128848", fldPlayerOff);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerOn);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerSvsOff);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerSvsOn);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerAlpOff);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerAlpOn);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerSdwOn);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerSdwOff);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerFastSet);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerMotReset);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerMotReset2);

INCLUDE_ASM(const s32, "code_00128848", fldPlayerPosRot);

INCLUDE_ASM(const s32, "code_00128848", fldSkyDefaultLoad);

INCLUDE_ASM(const s32, "code_00128848", fldSkyFieldLoad);

INCLUDE_ASM(const s32, "code_00128848", fldGetSkyEntry);

INCLUDE_ASM(const s32, "code_00128848", fldSkyCalcFilter);

INCLUDE_ASM(const s32, "code_00128848", fldSkyDrawFilter);

INCLUDE_ASM(const s32, "code_00128848", fldSkySetFilter);

void fldSetFilterAnime(s32 anime)
{
    gFilterAnime = anime;
    gFilterAnimeCntF = 0;
    gFilterAnimeOfsA = 0;
}


void fldSkySetAlpha(s32 alpha)
{
    gFilterAlpha = alpha;
}


s32 fldSkyGetAlpha(void)
{
    return gFilterAlpha;
}


void fldSkySetAlphaOfs(s32 ofs)
{
    fldSkyAlphaOfs = ofs;
}


s32 fldSkyGetAlphaOfs(void)
{
    return fldSkyAlphaOfs;
}


void fldSkySetTableAnime(s32 anime)
{
    s32 idx = D_002FC0EC;

    gSkyTblAnime = anime;
    gSkyTblAnimeCntI = 0;
    gSkyTblBaseIdx = idx;
    fldSkySetTbl(idx, 1);
}


INCLUDE_ASM(const s32, "code_00128848", fldSkyCalcTable);

INCLUDE_ASM(const s32, "code_00128848", fld_136800);

INCLUDE_ASM(const s32, "code_00128848", fldSkySetPlayer);

INCLUDE_ASM(const s32, "code_00128848", fldSkySetTbl);

INCLUDE_ASM(const s32, "code_00128848", fldSkySetTblModosu);

INCLUDE_ASM(const s32, "code_00128848", fldSkySetTblUpper);

INCLUDE_ASM(const s32, "code_00128848", fldSkySetTblLightMaDZ);

INCLUDE_ASM(const s32, "code_00128848", fldSkySetTblLightMa);

INCLUDE_ASM(const s32, "code_00128848", fldSetFileResolverIdx);

INCLUDE_ASM(const s32, "code_00128848", fld_1374F0);

INCLUDE_ASM(const s32, "code_00128848", fldSkyRefreshTbl);

INCLUDE_ASM(const s32, "code_00128848", fldSkyBattle);

INCLUDE_ASM(const s32, "code_00128848", fldAllocPlaneTable);

INCLUDE_ASM(const s32, "code_00128848", fldFreeMatterTable);

f32 fldVecDot(sdfVector4_t *a, sdfVector4_t *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}


f32 MATH_VectorLength(sdfVector4_t *v)
{
    f32 len;

    EE_SQRT_F32(len, v->x * v->x + v->y * v->y + v->z * v->z);
    return len;
}


INCLUDE_ASM(const s32, "code_00128848", fldMakeEveHitPlanes);

INCLUDE_ASM(const s32, "code_00128848", fldFindMatterIndex);

INCLUDE_ASM(const s32, "code_00128848", fldFindMatterIndexFrom);

INCLUDE_ASM(const s32, "code_00128848", fldClearMatterFlags);

INCLUDE_ASM(const s32, "code_00128848", fldMakePlane);

INCLUDE_ASM(const s32, "code_00128848", HitPositionInside2);

INCLUDE_ASM(const s32, "code_00128848", fldHitChkUnitElvOnly);

INCLUDE_ASM(const s32, "code_00128848", fldHitChkUnitElv);

INCLUDE_ASM(const s32, "code_00128848", fldHitChkUnitDamage);

INCLUDE_ASM(const s32, "code_00128848", fldHitChkUnit);

INCLUDE_ASM(const s32, "code_00128848", fldHitChkUnitVWM);

void func_0013B658(void) {
}

void func_0013B660(void) {
}

void func_0013B668(void) {
}

INCLUDE_ASM(const s32, "code_00128848", fldHitWM_Mask);

INCLUDE_ASM(const s32, "code_00128848", HitPositionInside3);

INCLUDE_ASM(const s32, "code_00128848", fldHitChkCamera);

INCLUDE_ASM(const s32, "code_00128848", fldHitChkEnemy);

INCLUDE_ASM(const s32, "code_00128848", fldAddItemBoxObject);

INCLUDE_ASM(const s32, "code_00128848", fldKaifukuTest);

INCLUDE_ASM(const s32, "code_00128848", fldStatusSet);

INCLUDE_ASM(const s32, "code_00128848", fldClearEveRngTable);

INCLUDE_ASM(const s32, "code_00128848", fldEveHit_End);

INCLUDE_ASM(const s32, "code_00128848", fldEveHit_AddEveData);


INCLUDE_ASM(const s32, "code_00128848", fldEveHit_AddEveHit);

INCLUDE_ASM(const s32, "code_00128848", fldEveHit_ChkDir);

INCLUDE_ASM(const s32, "code_00128848", fldEveHit_ChkDir2);

INCLUDE_ASM(const s32, "code_00128848", fldEveHitTestRect);

INCLUDE_ASM(const s32, "code_00128848", fldEveHitTestPlanes);

INCLUDE_ASM(const s32, "code_00128848", fldEveHit_ScriptStart);

INCLUDE_ASM(const s32, "code_00128848", fldEveHit_ScriptEnd);

INCLUDE_ASM(const s32, "code_00128848", fldEveHit_ScriptEnd2);

INCLUDE_ASM(const s32, "code_00128848", fldEveHit_ChkEveHitAfter);

INCLUDE_ASM(const s32, "code_00128848", fldEveHit_ChkOn);

INCLUDE_RODATA(const s32, "code_00128848", D_00531E08);

INCLUDE_ASM(const s32, "code_00128848", fldEveHit_ChkEveHit);

INCLUDE_ASM(const s32, "code_00128848", fld_13F6B8);

INCLUDE_ASM(const s32, "code_00128848", fldPurgeEveRngProcesses);

INCLUDE_ASM(const s32, "code_00128848", fldEveHitResID_Find);

INCLUDE_ASM(const s32, "code_00128848", fldFindEveRngIndex);

INCLUDE_ASM(const s32, "code_00128848", fldEveHitResName_Find);

INCLUDE_ASM(const s32, "code_00128848", fldEveHit_AddEveHitItemBox);

INCLUDE_ASM(const s32, "code_00128848", fldEveHit_AddEveHitNpc);

INCLUDE_ASM(const s32, "code_00128848", fldCheckEveRngN);

INCLUDE_ASM(const s32, "code_00128848", fldCheckEveRngT);

INCLUDE_ASM(const s32, "code_00128848", fldCheckEveRngEncount);

INCLUDE_ASM(const s32, "code_00128848", fldCheckNpcTalk);

INCLUDE_ASM(const s32, "code_00128848", fldCheckNpcTalk2);

INCLUDE_ASM(const s32, "code_00128848", fldInf_FindTypeEveNpc);

INCLUDE_ASM(const s32, "code_00128848", fldInfChkFastPanel);

INCLUDE_ASM(const s32, "code_00128848", fldInfNext);

INCLUDE_ASM(const s32, "code_00128848", fldInfGet);

INCLUDE_ASM(const s32, "code_00128848", fldInfLoad);

INCLUDE_ASM(const s32, "code_00128848", fldInfLoadFromMem);

INCLUDE_ASM(const s32, "code_00128848", fldInfToWAP);

INCLUDE_ASM(const s32, "code_00128848", fldFindEventEntry);

INCLUDE_ASM(const s32, "code_00128848", fldDoor_Find2);

INCLUDE_ASM(const s32, "code_00128848", fldDoor_Find3);

s32 fldDoor_GetWarpEff(void)
{
    return fldDoorTable[scriptlabel].warpEff;
}


INCLUDE_ASM(const s32, "code_00128848", fldWapGetDoor);

INCLUDE_ASM(const s32, "code_00128848", fldDoorRun_FindStart);

INCLUDE_ASM(const s32, "code_00128848", fldDoor_ExitCamera);

s32 fldDoor_ChkWarp(void)
{
    return gFldWapPanel == 8;
}


INCLUDE_ASM(const s32, "code_00128848", fldWap_SetBgm);

INCLUDE_ASM(const s32, "code_00128848", fldDoor_Warp);

INCLUDE_RODATA(const s32, "code_00128848", D_00531F18);

INCLUDE_RODATA(const s32, "code_00128848", D_00531F28);

INCLUDE_RODATA(const s32, "code_00128848", jtbl_00531F40);

INCLUDE_ASM(const s32, "code_00128848", fldDoor_WarpEx);

INCLUDE_ASM(const s32, "code_00128848", fldSetTerminalState);

INCLUDE_ASM(const s32, "code_00128848", fldSetTerminalIndex);

INCLUDE_ASM(const s32, "code_00128848", fldDoor_SiseExit);

INCLUDE_ASM(const s32, "code_00128848", fldWap_Load);

INCLUDE_ASM(const s32, "code_00128848", fldDoor_EveBtlExit);

INCLUDE_ASM(const s32, "code_00128848", fldDoor_WarpCash);

INCLUDE_ASM(const s32, "code_00128848", fldDoor_GetHasigoTbl);

INCLUDE_ASM(const s32, "code_00128848", fldDoor_GetElvTbl);

INCLUDE_ASM(const s32, "code_00128848", fldDoor_ElvTakasaToFloor);

INCLUDE_ASM(const s32, "code_00128848", fldDoor_ElvExit);

INCLUDE_ASM(const s32, "code_00128848", fldClearDoorEvents);

INCLUDE_ASM(const s32, "code_00128848", fldDoorLoad);

INCLUDE_ASM(const s32, "code_00128848", fldGetDoorEntry);

INCLUDE_ASM(const s32, "code_00128848", fldSetupDoorScenes);

INCLUDE_ASM(const s32, "code_00128848", fldFindDoorEvent);

INCLUDE_ASM(const s32, "code_00128848", fldDoor_Step);

void func_00144288(void) {
}

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC44);

INCLUDE_SDATA(const s32, "code_00128848", fldDrwZBuf);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC4C);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC50);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC58);

INCLUDE_SDATA(const s32, "code_00128848", fldDrawEnv);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC60);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC68);

INCLUDE_SDATA(const s32, "code_00128848", gencMode);

INCLUDE_SDATA(const s32, "code_00128848", gencStageInnAlp);

INCLUDE_SDATA(const s32, "code_00128848", encDrawState);

INCLUDE_SDATA(const s32, "code_00128848", gencOn);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC7C);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC80);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC88);

INCLUDE_SDATA(const s32, "code_00128848", fldCamDist);

INCLUDE_SDATA(const s32, "code_00128848", fldCamMiwatasi);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC94);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC98);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EC9C);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ECA0);

INCLUDE_SDATA(const s32, "code_00128848", fldCamHeight);

INCLUDE_SDATA(const s32, "code_00128848", fldCamParamTable2);

INCLUDE_SDATA(const s32, "code_00128848", fldCamResetOn);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ECB0);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ECB4);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ECB8);

INCLUDE_SDATA(const s32, "code_00128848", fldElvCameraScene);

INCLUDE_SDATA(const s32, "code_00128848", fldCamElvState);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ECC4);

INCLUDE_SDATA(const s32, "code_00128848", fldHitChkMatter);

INCLUDE_SDATA(const s32, "code_00128848", fldHitChkSlope);

INCLUDE_SDATA(const s32, "code_00128848", fldCamPhokanState);

INCLUDE_SDATA(const s32, "code_00128848", fldCamPHokanMode);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ECD8);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ECDC);

INCLUDE_SDATA(const s32, "code_00128848", fldPlayerWarpState);

INCLUDE_SDATA(const s32, "code_00128848", fldPlayerFastMode);

INCLUDE_SDATA(const s32, "code_00128848", gfldPlayerHasiCnt);

INCLUDE_SDATA(const s32, "code_00128848", gfldPlayerDamegeCnt);

INCLUDE_SDATA(const s32, "code_00128848", fldPlayerStepDist);

INCLUDE_SDATA(const s32, "code_00128848", fldPlayerMoveState);

INCLUDE_SDATA(const s32, "code_00128848", fldPlayerSyukanState);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ECFC);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED00);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED04);

INCLUDE_SDATA(const s32, "code_00128848", fldWmWarpMode);

INCLUDE_SDATA(const s32, "code_00128848", fldWmapPlayerState);

INCLUDE_SDATA(const s32, "code_00128848", fldWmFastMove);

INCLUDE_SDATA(const s32, "code_00128848", fldSyukanState);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyEnable);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyDefault);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyDefaultData);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyDataTable);

INCLUDE_SDATA(const s32, "code_00128848", gFilterAlpha);

INCLUDE_SDATA(const s32, "code_00128848", gFilterAnime);

INCLUDE_SDATA(const s32, "code_00128848", gFilterAnimeCntF);

INCLUDE_SDATA(const s32, "code_00128848", gFilterAnimeOfsA);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyFilterPrev);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyFilter);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyFilterRate);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyFilterTime);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyTable);

INCLUDE_SDATA(const s32, "code_00128848", gSkyTblAnime);

INCLUDE_SDATA(const s32, "code_00128848", gSkyTblAnimeCntI);

INCLUDE_SDATA(const s32, "code_00128848", gSkyTblBaseIdx);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyTblUpperIdx);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED5C);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED60);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED64);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED68);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED6C);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED70);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED74);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyTableIdx);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED7C);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED80);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED84);

INCLUDE_SDATA(const s32, "code_00128848", fldSkyLightMaTbl);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED8C);

INCLUDE_SDATA(const s32, "code_00128848", D_0054ED90);

INCLUDE_SDATA(const s32, "code_00128848", fldItemBoxObjBuf);

INCLUDE_SDATA(const s32, "code_00128848", fldItemBoxObjCount);

INCLUDE_SDATA(const s32, "code_00128848", fldMatterTable);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EDA0);

INCLUDE_SDATA(const s32, "code_00128848", fldPlaneCount);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EDA8);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EDAC);

INCLUDE_SDATA(const s32, "code_00128848", fldEveRngPrevIdx);

INCLUDE_SDATA(const s32, "code_00128848", fldEveRngHitIdx);

INCLUDE_SDATA(const s32, "code_00128848", fldEveRngCount);

INCLUDE_SDATA(const s32, "code_00128848", gFldEveRngIdx);

INCLUDE_SDATA(const s32, "code_00128848", gFldEveRngIdx_N);

INCLUDE_SDATA(const s32, "code_00128848", gFldEveRngIdx_T);

INCLUDE_SDATA(const s32, "code_00128848", gEveNum);

INCLUDE_SDATA(const s32, "code_00128848", fldEveHitState);

INCLUDE_SDATA(const s32, "code_00128848", gFldKyouseiTalk);

INCLUDE_SDATA(const s32, "code_00128848", fldEveHitCount);

INCLUDE_SDATA(const s32, "code_00128848", gFldInfIdx);

INCLUDE_SDATA(const s32, "code_00128848", gFldInf_HitIdx);

INCLUDE_SDATA(const s32, "code_00128848", gFldInf_WpExit);

INCLUDE_SDATA(const s32, "code_00128848", fldDoorExitFlag);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EDE5);

INCLUDE_SDATA(const s32, "code_00128848", gFldWap_after_nextidx);

INCLUDE_SDATA(const s32, "code_00128848", gFld_ElvTakasa);

INCLUDE_SDATA(const s32, "code_00128848", fldDoorWarpTarget);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EDF4);

INCLUDE_SDATA(const s32, "code_00128848", fldDoorSiseState);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EDFC);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EE00);

INCLUDE_SDATA(const s32, "code_00128848", scriptlabel);

INCLUDE_SDATA(const s32, "code_00128848", gFldWapPanel);

INCLUDE_SDATA(const s32, "code_00128848", D_0054EE0C);

INCLUDE_RODATA(const s32, "code_00128848", D_00532030);

