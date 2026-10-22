#include "common.h"
#include "kernel.h"
#include "libsdf.h"

/* 0x68-byte picture work block. InitPictureProcessData sets the defaults and
 * evtPictureProcess1 steps the three interpolators. */
typedef struct
{
    u32 flag;
    void *tex;
    s32 ot;
    s32 x;
    s32 y;
    s32 alpha;
    f32 zoom;
    s32 size_sw;
    s32 size_sh;
    s32 size_tw;
    s32 size_th;
    s32 pri;
    s32 pos_x0;
    s32 pos_y0;
    s32 pos_x1;
    s32 pos_y1;
    s32 pos_time;
    s32 pos_cnt;
    s32 alp_a0;
    s32 alp_a1;
    s32 alp_time;
    s32 alp_cnt;
    s32 zoom_p0;
    s32 zoom_p1;
    s32 zoom_time;
    s32 zoom_cnt;
} evtPictureWork_t;

/* One 0x30-byte row of the eight-entry priority table picpri_Process draws. */
typedef struct
{
    void *tex;
    s32 ot;
    f32 x0;
    f32 y0;
    f32 x1;
    f32 y1;
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;
    s32 alpha;
    s32 pri;
} evtPicturePriTbl_t;

extern char D_0054F8F0[];

/* The four GS colour components itfMiscDrawSpritePrim packs; only alpha is set
 * per sprite. */
typedef struct
{
    u32 components[4];
} evtPictureColor_t;

extern s32 D_0032C950[];
extern s32 D_0032C960[];
extern evtPictureColor_t D_0032C970;
extern sdfGrOt_t sdfGrOtTable[];
extern void eglSetOt(s32 ot);
extern void eglLinkTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst);
extern void eglLinkAlpha(s32 mode);
extern void *sdfGrAllocPacketBuff(s32 aSize);
extern void sdfDrawTagInit(sdfDrawTag_t *a);
extern void itfMiscDrawAlphaPrim(sdfDrawTag_t *tag, s32 index, s32 flag);
extern void itfMiscDrawSpritePrim(void *vertexData, void *uvData, void *colorData, s32 tail,
                                  void *texture, s32 flag, sdfDrawTag_t *tag);

extern void *sdfChipAllocDirect(u32 aSize);
extern void sdfChipFreeDirect(void *aPtr);
extern void sdfTexFreeDelayed(void *aHandle);
extern void *itfMiscLoadTexture(const char *filename);
extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2, s32 v3,
                                         dds3ProcessFunc_t drawExit, dds3ProcessFunc_t deleteExit,
                                         void *p);
extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern evtPictureWork_t *InitPictureProcessData(void);
extern s32 evtPictureProcess1(dds3ProcessID_t *id);
extern void *evtPicturePriList;
extern void evtPicturePriClear(void);
extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);
extern void dds3SetProcessData(dds3ProcessID_t *id, void *data);
extern char D_0054F8F8[];

void evtPicturePriClear(void)
{
    evtPicturePriTbl_t *tbl = dds3GetProcessData(evtPicturePriList);
    s32 i;

    for (i = 0; i < 8; i++) {
        tbl->tex = NULL;
        tbl->pri = 0;
        tbl->ot = 0;
        tbl++;
    }
}

void evtPicturePriAdd(evtPicturePriTbl_t *indata)
{
    evtPicturePriTbl_t *tbl = dds3GetProcessData(evtPicturePriList);
    s32 i;

    for (i = 0; i < 8; i++) {
        if (tbl->tex == NULL) {
            *tbl = *indata;
            return;
        }
        tbl++;
    }
}

void evtPicturePriDraw(void)
{
    evtPicturePriTbl_t *tbl;
    sdfDrawTag_t *tag;
    s32 pri;
    s32 i;
    s32 setup = 0;

    for (pri = 3; pri >= 0; pri--) {
        tbl = dds3GetProcessData(evtPicturePriList);
        for (i = 0; i < 8; i++) {
            if (tbl->tex != NULL && tbl->pri == pri) {
                if (setup == 0) {
                    setup = 1;
                    eglSetOt(0x47);
                    eglLinkTest(1, 0, 0x80, 1, 0, 0, 1, 1);
                    eglLinkAlpha(0);
                    eglSetOt(0x33);
                    eglLinkTest(1, 0, 0x80, 1, 0, 0, 1, 1);
                    eglLinkAlpha(0);
                }
                D_0032C950[0] = tbl->x0 * 16.0f;
                D_0032C950[1] = tbl->y0 * 8.0f;
                D_0032C950[2] = tbl->x1 * 16.0f;
                D_0032C950[3] = tbl->y1 * 8.0f;
                D_0032C960[0] = tbl->u0 << 4;
                D_0032C960[1] = tbl->v0 << 4;
                D_0032C960[2] = tbl->u1 << 4;
                D_0032C960[3] = tbl->v1 << 4;
                D_0032C970.components[3] = tbl->alpha;
                tag = sdfGrAllocPacketBuff(0x20);
                sdfDrawTagInit(tag);
                itfMiscDrawAlphaPrim(tag, 0, 0);
                itfMiscDrawSpritePrim(D_0032C950, D_0032C960, &D_0032C970, 0xFFF, tbl->tex, 0, tag);
                if (tbl->ot == 0) {
                    sdfGrOt_t *ot = &sdfGrOtTable[0x47];

                    ot->regFunc(ot, tag);
                } else {
                    sdfGrOt_t *ot = &sdfGrOtTable[0x33];

                    ot->regFunc(ot, tag);
                }
            }
            tbl++;
        }
    }
}

void evtPictureDrawOn(dds3ProcessID_t *id)
{
    evtPictureWork_t *work = dds3GetProcessData(id);
    work->flag |= 1;
}

void evtPictureDrawOff(dds3ProcessID_t *id)
{
    evtPictureWork_t *work = dds3GetProcessData(id);
    work->flag &= ~1;
}

void evtKillPictureProcess(dds3ProcessID_t *id)
{
    dds3KillProcess(id, 1);
}

void evtPictureSetOt(dds3ProcessID_t *id, s32 ot)
{
    evtPictureWork_t *work = dds3GetProcessData(id);
    work->ot = ot;
}

void evtPictureSetPri(dds3ProcessID_t *id, s32 pri)
{
    evtPictureWork_t *work = dds3GetProcessData(id);
    work->pri = pri;
}

void evtPictureSetSize(dds3ProcessID_t *id, s32 sw, s32 sh, s32 tw, s32 th)
{
    evtPictureWork_t *work = dds3GetProcessData(id);

    work->size_sw = sw;
    work->size_sh = sh;
    work->size_tw = tw;
    work->size_th = th;
}

void evtPictureSetPos(dds3ProcessID_t *id, s32 x0, s32 y0, s32 x1, s32 y1, s32 time)
{
    evtPictureWork_t *work = dds3GetProcessData(id);

    work->x = x0;
    work->y = y0;
    work->pos_x0 = x0;
    work->pos_y0 = y0;
    work->pos_x1 = x1;
    work->pos_y1 = y1;
    work->pos_time = time;
    work->pos_cnt = time;
}

void evtPictureSetAlpha(dds3ProcessID_t *id, s32 alp0, s32 alp1, s32 time)
{
    evtPictureWork_t *work = dds3GetProcessData(id);

    work->alpha = alp0;
    work->alp_a0 = alp0;
    work->alp_a1 = alp1;
    work->alp_time = time;
    work->alp_cnt = time;
}

void evtPictureSetZoom(dds3ProcessID_t *id, s32 zoom0, s32 zoom1, s32 time)
{
    evtPictureWork_t *work = dds3GetProcessData(id);

    work->zoom = zoom0;
    work->zoom_p0 = zoom0;
    work->zoom_p1 = zoom1;
    work->zoom_time = time;
    work->zoom_cnt = time;
}

void evtPictureSwap(void) {
}

s32 evtPictureStat(dds3ProcessID_t *id)
{
    evtPictureWork_t *work = dds3GetProcessData(id);
    s32 stat = 0;

    if (work->pos_cnt != 0) {
        stat = 1;
    }
    if (work->alp_cnt != 0) {
        stat |= 2;
    }
    if (work->zoom_cnt != 0) {
        stat |= 4;
    }
    return stat;
}

evtPictureWork_t *InitPictureProcessData(void)
{
    evtPictureWork_t *work = sdfChipAllocDirect(sizeof(evtPictureWork_t));

    work->flag = 0;
    work->tex = NULL;
    work->ot = 0;
    work->x = 0;
    work->y = 0;
    work->alpha = 0x80;
    work->zoom = 100.0f;
    work->size_sw = 0x200;
    work->size_sh = 0x1C0;
    work->size_tw = 0x200;
    work->size_th = 0x1C0;
    work->pri = 0;
    work->pos_x0 = 0;
    work->pos_y0 = 0;
    work->pos_x1 = 0;
    work->pos_y1 = 0;
    work->pos_time = 0;
    work->pos_cnt = 0;
    work->alp_a0 = 0x80;
    work->alp_a1 = 0x80;
    work->alp_time = 0;
    work->alp_cnt = 0;
    work->zoom_p0 = 100;
    work->zoom_p1 = 100;
    work->zoom_time = 0;
    work->zoom_cnt = 0;
    return work;
}

void LoadTexture(evtPictureWork_t *data, const char *texpath)
{
    data->tex = itfMiscLoadTexture(texpath);
}

/* NON_MATCHING: 33 of 138 words differ (first at +0x30). */
#ifdef NON_MATCHING
s32 evtPictureProcess1(dds3ProcessID_t *id)
{
    evtPictureWork_t *work = dds3GetProcessData(id);
    evtPicturePriTbl_t tbl;
    void *tex;
    s32 t;
    s32 a;
    f32 dw;

    if (!(work->flag & 1)) {
        return 0;
    }
    {
        tex = work->tex;
        if (tex == NULL) {
            return;
        }
        if (work->pos_cnt > 0) {
            s32 cnt = work->pos_cnt - 1;

            t = work->pos_time - cnt;
            work->x = work->pos_x0 + (work->pos_x1 - work->pos_x0) * t / work->pos_time;
            work->y = work->pos_y0 + (work->pos_y1 - work->pos_y0) * t / work->pos_time;
            work->pos_cnt = cnt;
        }
        if (work->alp_cnt > 0) {
            work->alp_cnt--;
            a = work->alp_a0 +
                (work->alp_a1 - work->alp_a0) * (work->alp_time - work->alp_cnt) / work->alp_time;
            if (a < 0) {
                a = 0;
            }
            work->alpha = a;
            if (a > 0x80) {
                work->alpha = 0x80;
            }
        }
        if (work->zoom_cnt > 0) {
            work->zoom_cnt--;
            work->zoom = work->zoom_p0 + (f32)(work->zoom_p1 - work->zoom_p0) *
                                             (work->zoom_time - work->zoom_cnt) / work->zoom_time;
        }
        dw = work->size_sw * work->zoom / 100.0f - work->size_sw;
        tbl.x0 = work->x - dw * 0.5f;
        tbl.x1 = tbl.x0 + work->size_sw + dw;
        dw = work->size_sh * work->zoom / 100.0f - work->size_sh;
        tbl.y0 = work->y - dw * 0.5f;
        tbl.y1 = tbl.y0 + work->size_sh + dw;
        tbl.u0 = 0;
        tbl.v0 = 0;
        tbl.u1 = work->size_tw;
        tbl.v1 = work->size_th;
        tbl.alpha = work->alpha;
        tbl.pri = work->pri;
        tbl.tex = tex;
        tbl.ot = work->ot;
        evtPicturePriAdd(&tbl);
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "event/evtPicture", evtPictureProcess1);
#endif

s32 evtPictureShutdown(dds3ProcessID_t *id)
{
    evtPictureWork_t *work = dds3GetProcessData(id);

    if (work->tex != NULL) {
        sdfTexFreeDelayed(work->tex);
        work->tex = NULL;
    }
    sdfChipFreeDirect(work);
}

dds3ProcessID_t *evtStartPictureProcess2(u32 priority, const char *texdata)
{
    evtPictureWork_t *work = InitPictureProcessData();

    LoadTexture(work, texdata);
    return dds3StartProcess(D_0054F8F0, priority, 1, 1, evtPictureProcess1, evtPictureShutdown, work);
}

dds3ProcessID_t *evtStartPictureProcess(u32 priority, void *texpath)
{
    evtPictureWork_t *work = InitPictureProcessData();

    work->tex = texpath;
    return dds3StartProcess(D_0054F8F0, priority, 1, 1, evtPictureProcess1, evtPictureShutdown, work);
}

s32 picpri_Process(dds3ProcessID_t *id)
{
    dds3GetProcessData(id);
    evtPicturePriDraw();
    evtPicturePriClear();
    return 0;
}

void picpri_Shutdown(dds3ProcessID_t *id)
{
    sdfChipFreeDirect(dds3GetProcessData(id));
    evtPicturePriList = NULL;
}

void evtPicturePriProcessInit(void)
{
    void *tbl = sdfChipAllocDirect(sizeof(evtPicturePriTbl_t) * 8);

    evtPicturePriList = dds3StartProcess(D_0054F8F8, 0x2AF8, 1, 1, picpri_Process, picpri_Shutdown, NULL);
    dds3SetProcessData(evtPicturePriList, tbl);
    evtPicturePriClear();
}
INCLUDE_SDATA(const s32, "event/evtPicture", evtPicturePriList);

INCLUDE_SDATA(const s32, "event/evtPicture", D_0054F8EC);

INCLUDE_SDATA(const s32, "event/evtPicture", D_0054F8F0);

INCLUDE_SDATA(const s32, "event/evtPicture", D_0054F8F8);

