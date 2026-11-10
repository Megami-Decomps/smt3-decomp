#include "common.h"
#include "sdfFrame.h"
#include "kernel.h"

extern char evtSkyProcName[];

extern u16 skyanime;

extern u16 skyanow;

extern u16 skyamax;

extern s16 skyatarget;

extern s16 skyabase_par;

extern s32 fldbokasi;

extern s16 fldSkyGetAlphaOfs(void);

extern void fldSkySetAlphaOfs(s32 alp);

extern void fldSkySetAlpha(s32 alp);

extern void fldSetFilterAnime(s32 anime);

extern void fldSkySetFilter(s32 idx, s32 mode, s32 hokan);

extern void fldSkyDrawFilter(void);

extern void fldDrawFrameSprite(void);

extern void rsetOtOfs(s32 otofs);

extern dds3ProcessID_t *dds3SearchProcessName(char *namestr);

extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);

void evtUpdateSkyAlpha(void);

s32 evtSkyProcess1(dds3ProcessID_t *id);

void evtSkyShutdown(dds3ProcessID_t *id);

void evtSetBokasi(s32 sw)
{
    fldbokasi = sw;
}

void evtSetSkyAlpha(s32 frame, s32 a)
{
    s16 current;

    current = fldSkyGetAlphaOfs();
    if (current != a) {
        if (frame == 0) {
            fldSkySetAlphaOfs(a);
            skyanime = 0;
        } else {
            skyamax = SDF_FRAMES(frame);
            skyatarget = current;
            skyabase_par = a;
            skyanime = 1;
            skyanow = 0;
        }
    }
}

u16 evtCheckSkyAlphaAnime(void)
{
    return skyanime;
}

void evtUpdateSkyAlpha(void)
{
    if (skyanime != 0) {
        skyanow++;
        fldSkySetAlphaOfs(skyatarget +
                          (s32)((f32)(skyabase_par - skyatarget) * ((f32)skyanow / (f32)skyamax)));
        if (skyanow >= skyamax) {
            skyanime = 0;
        }
    }
}

s32 evtSkyProcess1(dds3ProcessID_t *id)
{
    evtUpdateSkyAlpha();
    fldSkyDrawFilter();
    if (fldbokasi != 0) {
        rsetOtOfs(0x47);
        fldDrawFrameSprite();
    }
    return 0;
}

void evtSkyShutdown(dds3ProcessID_t *id)
{
    skyanime = 0;
    fldbokasi = 0;
}

void evtExitSky(void)
{
    dds3ProcessID_t *id;

    id = dds3SearchProcessName(evtSkyProcName);
    if (id != NULL) {
        dds3KillProcess(id, 1);
    }
}

dds3ProcessID_t *evtInitSky(void)
{
    fldSetFilterAnime(0);
    fldSkySetAlpha(0x80);
    fldSkySetAlphaOfs(0);
    fldSkySetFilter(0, 1, 0);
    return dds3StartProcess(evtSkyProcName, 0x2B0E, 1, 1, evtSkyProcess1, (dds3ProcessFunc_t)evtSkyShutdown, NULL);
}
u16 skyanime = 0;

SDATA_PAD(2);

s32 fldbokasi = 0;

char evtSkyProcName[8] = "sky";

char evtFrameNumFmt[8] = "%3d";

char D_0054F918[8] = "PRESET";

char D_0054F920[8] = "STOP";

char D_0054F928[8] = "DATA";

char D_0054F930[8] = "MODE";

char D_0054F938[8] = "SW";

char D_0054F940[8] = "LINK";

char D_0054F948[8] = "END";

char D_0054F950[8] = "NO";

char D_0054F958[8] = "VOLUME";

char D_0054F960[8] = "FOVY";

char D_0054F968[8] = "HOKAN";

char D_0054F970[8] = "SCALE";

char D_0054F978[8] = "ALPHA";

char D_0054F980[8] = "RANGE";

char D_0054F988[8] = "LENGTH";

char D_0054F990[8] = "START";

s32 evtMenuDisp_cnt = 0;

SDATA_PAD(4);

char evtScrollUpMark[8] = "^";

char evtScrollDownMark[8] = "v";

char evtFmtIndentInt[8] = "     %d";

char evtFmtStr[8] = "%s";

char evtFmtSpaceStr[8] = " %s";

char evtFmtNameExt[8] = "[%s.%s]";

char evtFmtChar[8] = "%c";

char evtLabelEvent[8] = " EVENT";

char evtFmtEventNo[8] = "%3d";

char evtLabelCut[8] = "   CUT";

char evtFmtCutNo[8] = "%03d";

char evtLabelName[8] = "NAME:";

char D_0054FA00[8] = "ALLSTOP";

char D_0054FA08[8] = "VOLUP";

char D_0054FA10[8] = "VOLDOWN";

char D_0054FA18[8] = "TRANS";

char D_0054FA20[8] = "FADEOUT";

char D_0054FA28[8] = "FADEIN";

char D_0054FA30[8] = "PLAY";

char D_0054FA38[8] = "DISP ON";

char evtFmtInt4[8] = "%4d";

char evtStrEllipsis[8] = "...";

char evtStrOn[8] = "ON ";

char evtStrOff[8] = "OFF";

char evtBlendNameNormal[8] = "NORMAL";

char evtBlendNameAdd[8] = "ADD";

char evtBlendNameSub[8] = "SUB";

char evtFmtFloat1[8] = "%.1f";

char evtFmtInt[8] = "%d";

char evtFmtName11[8] = "%-.11s";

char evtFmtPlayerNo[8] = "P%d:";

char evtFmtIndentStr[8] = "   %s";

char evtMsgSaveOpenError[8] = "err\n";

char evtPfsDevice[8] = "pfs:";

char evtPackNameFormat[8] = "e%03d_p";

