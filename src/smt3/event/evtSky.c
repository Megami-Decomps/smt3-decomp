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

SDATA_HOLE(2);

s32 fldbokasi = 0;

INCLUDE_SDATA_C(const s32, "event/evtSky", evtSkyProcName);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtFrameNumFmt);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054F918);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054F920);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054F928);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054F930);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054F938);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054F940);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054F948);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054F950);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054F958);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054F960);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054F968);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054F970);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054F978);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054F980);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054F988);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054F990);

s32 evtMenuDisp_cnt = 0;

SDATA_HOLE(4);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtScrollUpMark);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtScrollDownMark);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtFmtIndentInt);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtFmtStr);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtFmtSpaceStr);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtFmtNameExt);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtFmtChar);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtLabelEvent);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtFmtEventNo);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtLabelCut);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtFmtCutNo);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtLabelName);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054FA00);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054FA08);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054FA10);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054FA18);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054FA20);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054FA28);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054FA30);

INCLUDE_SDATA_C(const s32, "event/evtSky", D_0054FA38);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtFmtInt4);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtStrEllipsis);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtStrOn);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtStrOff);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtBlendNameNormal);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtBlendNameAdd);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtBlendNameSub);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtFmtFloat1);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtFmtInt);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtFmtName11);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtFmtPlayerNo);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtFmtIndentStr);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtMsgSaveOpenError);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtPfsDevice);

INCLUDE_SDATA_C(const s32, "event/evtSky", evtPackNameFormat);

