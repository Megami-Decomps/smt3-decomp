#include "common.h"
#include "kernel.h"

extern char D_0054F908[];

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
            skyamax = frame;
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

    id = dds3SearchProcessName(D_0054F908);
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
    return dds3StartProcess(D_0054F908, 0x2B0E, 1, 1, evtSkyProcess1, (dds3ProcessFunc_t)evtSkyShutdown, NULL);
}
INCLUDE_SDATA(const s32, "event/evtSky", skyanime);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F902);

INCLUDE_SDATA(const s32, "event/evtSky", fldbokasi);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F908);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F910);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F918);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F920);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F928);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F930);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F938);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F940);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F948);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F950);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F958);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F960);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F968);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F970);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F978);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F980);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F988);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F990);

INCLUDE_SDATA(const s32, "event/evtSky", evtMenuDisp_cnt);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F99C);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F9A0);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F9A8);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F9B0);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F9B8);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F9C0);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F9C8);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F9D0);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F9D8);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F9E0);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F9E8);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F9F0);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054F9F8);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054FA00);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054FA08);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054FA10);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054FA18);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054FA20);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054FA28);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054FA30);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054FA38);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054FA40);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054FA48);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054FA50);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054FA58);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054FA60);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054FA68);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054FA70);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054FA78);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054FA80);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054FA88);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054FA90);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054FA98);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054FAA0);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054FAA8);

INCLUDE_SDATA(const s32, "event/evtSky", D_0054FAB0);

