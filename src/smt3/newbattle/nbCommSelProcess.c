#include "common.h"
#include "kernel.h"
#include "nb.h"
#include "ps2asm.h"
#include "model.h"
#include "event.h"
#include "motion.h"
#include "field.h"

extern char *text1[];

extern char *text2[];

extern char *pStr[];

extern char D_00551130[];

extern void cmpSetSkillListRows(s32 rows);

extern void *frwGetSmallFontFRQ(s32 X, s32 Y, u32 Pri, u32 Color, char *pStr, void *pFRQ);

extern void *frwGetSmallNumFontFRQ(s32 X, s32 Y, u32 Pri, u32 Color, char *pStr, void *pFRQ);

extern s32 frDrawFontEx(void *pFRQ, s8 Mode);

extern s32 frDestroyFontRequestAfter(void *pFRQ);

extern s32 sprintf(char *buf, const char *fmt, ...);

extern s32 datGetSkillCost(datUnitWork_t *w, s32 nskill);

extern void nbPnl_setOt(s32 ot);
extern void nbPnl_rsetTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte,
                           s32 ztst);
extern void nbPnl_rsetAlpha(s32 mode);

extern void DispCommCursor(s32 ox, s32 oy, s32 a);

extern void DispSummonCursor(s32 ox, s32 oy, s32 a);

extern void nbPnlSprite(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, s32 rgba,
                        sdfTexHandle_t *th);

extern f32 fabsf(f32 value);

/* The quadword copy of a formation position: 16-byte aligned, so ee-gcc moves
 * it with one lq/sq pair. */
typedef struct nbQuadVector_t {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} __attribute__((aligned(16))) nbQuadVector_t;

/* 8-byte rows at +0x88 indexed by formation slot. */
typedef struct nbTimeList_t {
    u16 lockframe;
    u16 end;
    u16 hp;
    u16 pad06;
} nbTimeList_t;

typedef struct nbActionProcessData_t {
    nbMainProcessData_t *data;
    u32 stat;
    s8 seq;
    s8 nextseq;
    s16 seqtime;
    s32 seqcnt;
    s32 skilltime;
    f32 press;
    s32 newpresstype;
    s16 newpress_p;
    s16 newpress_ten;
    s32 newaddpresstype;
    struct nbParty_t *party;
    nbFormation_t *form;
    datUnitWork_t *work;
    s32 partyindex;
    s32 type;
    s32 target;
    u32 select;
    u32 timer;
    s32 runerr;
    s32 jyokyomes;
    s32 jyo_varid;
    s32 jyo_varind;
    s32 adata;
    s32 autoskill;
    s32 allhpdamage;
    s32 allmpdamage;
    s32 massuruid;
    u32 negotype;
    s32 motion;
    s32 motse;
    void *hEffBTL;
    u8 unk78[0x10];
    nbTimeList_t timelist[32];
    u32 packettime;
    s32 hitstopframe;
    u8 unk190[0x10];
    u32 aisummonid;
} nbActionProcessData_t;

/* 128-byte party records. */
struct nbParty_t {
    u16 flag;
    s16 formindex;
    s16 statindex;
    u16 partyindex;
    s16 count[20];
    u32 aisyo[16];
    u16 hojoskill[8];
};

typedef struct nbCommSelProcessData_t {
    void *handle;
    nbMainProcessData_t *data;
    nbActionProcessData_t *act;
    s8 stat;
    u8 nowtype;
    s16 pad0E;
    s32 drawtype;
    s32 drawcursor;
    s32 drawlisty;
    s32 nowcursor[6];
    s32 nowlisty[6];
    struct nbParty_t *my;
    struct nbParty_t *partytarget;
    u16 commlist[6][288];
    s32 commcnt[6];
    s8 commdisable[6][288];
    s32 cnt;
    f32 smritu;
    u32 animeflag;
    s32 ox;
    s32 oy;
    s32 oz;
    s32 alpha;
    s32 helpa;
    f32 ritu;
    s32 openwait;
} nbCommSelProcessData_t;

extern char D_005441A8[];

extern sdfGrOt_t sdfGrOtTable[];

extern sdfDrawTag_t *dds3FontReqAllocDrawTag(void);

extern void sdfPrimLinkG4(sdfDrawTag_t *aTag, s32 aPrim, s32 aX0, s32 aY0, u32 aColor0, s32 aX1,
                          s32 aY1, u32 aColor1, s32 aX2, s32 aY2, u32 aColor2, s32 aX3, s32 aY3,
                          u32 aColor3, s32 aZ, void *aAllocator);

extern void *memset(void *a, s32 b, u32 c);

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern dds3ProcessID_t *dds3SearchProcessName(const char *namestr);

extern void sdfMemFreeDirect(void *aHandle);

extern void SetCommCursor2(nbCommSelProcessData_t *s);

extern void *memset(void *a, s32 b, u32 c);

extern char D_00551118[];

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, void *drawExit,
                                         void *deleteExit, void *p);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

nbCommSelProcessData_t *InitCommSelProcessData(nbMainProcessData_t *data, struct nbParty_t *my,
                                               nbActionProcessData_t *act);

void *nbCommSelProcess1(dds3ProcessID_t *id);

void nbCommSelShutdown(dds3ProcessID_t *id);

extern void SetCommList(nbCommSelProcessData_t *s, struct nbParty_t *my);

extern char *D_00551110;

extern char *D_00551114;

extern s32 StkSelId;

extern void nbInitCommSelProcess(nbMainProcessData_t *data, struct nbParty_t *my,
                                 nbActionProcessData_t *act);

void MemoryCommand(nbCommSelProcessData_t *s);

s32 GetCommListCnt(nbCommSelProcessData_t *s, s32 type);

extern datUnitWork_t *nbGetUnitWorkPointer(nbMainProcessData_t *data, struct nbParty_t *party);

nbCommSelProcessData_t *GetCommSelProcessData(void);

#ifdef NON_MATCHING

#else

#endif

#ifdef NON_MATCHING

#else

#endif

#ifdef NON_MATCHING

#else

#endif

#ifdef NON_MATCHING

#else

#endif

#ifdef NON_MATCHING

#else

#endif

#ifdef NON_MATCHING

#else

#endif

#ifdef NON_MATCHING

#else

#endif

#ifdef NON_MATCHING

#else

#endif

#ifdef NON_MATCHING

#else

#endif

#ifdef NON_MATCHING

#else

#endif

s32 GetCommListCnt(nbCommSelProcessData_t *s, s32 type)
{
    s32 cnt[6] = {4, 4, 4, 3, 4, 4};

    if (type == 3) {
        return s->cnt;
    }
    return cnt[type];
}

nbCommSelProcessData_t *GetCommSelProcessData(void)
{
    return dds3GetProcessData(dds3SearchProcessName(D_005441A8));
}

void DispPanelCursor(nbCommSelProcessData_t *s, s32 type, s32 ox, s32 oy, u32 c)
{
    s32 margin = 0;
    s32 listCount;
    s32 blink;

    if (type == 3) {
        margin = 0x2B - (s32)(s->smritu * 43.0f);
    }
    listCount = GetCommListCnt(s, type);
    blink = s->data->battletime % 0x19 < 0xE;
    if (s->nowlisty[type] > 0 && blink) {
        nbPnlSprite(ox + 0xE, oy - margin + 0x166, 7, 9, 0x53, 0x4E, 7, 9, c,
                    s->data->pnltex[0]);
    } else {
        nbPnlSprite(ox + 0xE, oy - margin + 0x166, 7, 9, 0x4B, 0x4E, 7, 9, c,
                    s->data->pnltex[0]);
    }

    if (s->nowlisty[type] < s->commcnt[type] - listCount && blink) {
        nbPnlSprite(ox + 0xE, oy + 0x1A5, 7, -9, 0x53, 0x4E, 7, 9, c,
                    s->data->pnltex[0]);
    } else {
        nbPnlSprite(ox + 0xE, oy + 0x1A5, 7, -9, 0x4B, 0x4E, 7, 9, c,
                    s->data->pnltex[0]);
    }
}

INCLUDE_RODATA(const s32, "newbattle/nbCommSelProcess", D_005441A8);

void DispPanelLabel(nbCommSelProcessData_t *s, s32 type, s32 ox, s32 oy, u32 color)
{
    s32 margin = 0;
    s32 count;
    s32 rank;
    s32 i;

    nbGetUnitWorkPointer(s->data, s->my);
    if (type == 3) {
        margin = 0x2B - (s32)(s->smritu * 43.0f);
    }
    count = 0;
    rank = 0;
    for (i = 0; i < 6; i++) {
        if (s->commcnt[i] > 0) {
            if (i == type) {
                rank = count;
            }
            count++;
        }
    }
    for (i = 0; i < count; i++) {
        nbPnlSprite(ox + 0x6A + i * 0x10, oy - margin + 0x144, 0x1F, 0x10,
                    0x8F, 1, 0x1F, 0x10, color, s->data->pnltex[1]);
    }
    nbPnlSprite(ox + rank * 0x10 + 0x51, oy - margin + 0x13D, 0x38, 0x17,
                0x55, 1, 0x38, 0x17, color, s->data->pnltex[1]);
    if (rank > 0) {
        nbPnlSprite(ox + 0x51, oy - margin + 0x13D, rank * 0x10, 0x17,
                    0x55, 1, 1, 0x17, color, s->data->pnltex[1]);
    }
    if ((u32)type >= 6) {
        return;
    }
    switch (type) {
    case 0:
        nbPnlSprite(ox + 0x15, oy + 0x141, 0x38, 0x13, 0x96, 0x16, 0x38, 0x13,
                    color, s->data->pnltex[0]);
        break;
    case 1:
        nbPnlSprite(ox + 0x15, oy + 0x141, 0x31, 0x13, 0xC5, 2, 0x31, 0x13,
                    color, s->data->pnltex[0]);
        break;
    case 2:
        nbPnlSprite(ox + 0x15, oy + 0x141, 0x2E, 0x13, 0xD0, 0x16, 0x2E, 0x13,
                    color, s->data->pnltex[0]);
        break;
    case 4:
        nbPnlSprite(ox + 0x15, oy + 0x141, 0x4A, 0x13, 0x96, 0x2A, 0x4A, 0x13,
                    color, s->data->pnltex[0]);
        break;
    case 5:
        nbPnlSprite(ox + 0x15, oy + 0x141, 0x42, 0x13, 0x6A, 0x3E, 0x42, 0x13,
                    color, s->data->pnltex[0]);
        break;
    case 3:
        nbPnlSprite(ox + 0x15, oy - margin + 0x141, 0x50, 0x13, 0xAE, 0x3E, 0x50, 0x13,
                    color, s->data->pnltex[0]);
        break;
    }
}

#ifdef NON_MATCHING
void DispPanelSitaji(nbCommSelProcessData_t *s, s32 type, s32 ox, s32 oy, u32 color)
{
    s32 margin = 0;
    s32 y;
    s32 lineX;

    if (type == 3) {
        margin = 43 - (s32)(s->ritu * 43.0f);
    }
    if ((u32)type < 6) {
        y = oy - margin;
        nbPnlSprite(ox, y + 0x13D, 0xE7, 0x2A, 1, 0x1C, 0xE7, 0x2A,
                    color, s->data->pnltex[1]);
        nbPnlSprite(ox, y + 0x167, 0xE7, margin + 0x48, 1, 0x45, 0xE7, 2,
                    color, s->data->pnltex[1]);
        nbPnlSprite(ox + 0x51, y + 0x153, 0x9A, 2, 1, 0x7A, 0x9A, 1,
                    color, s->data->pnltex[1]);
        nbPnlSprite(ox, oy + 0x1AF, 0xEB, 2, 1, 0x7D, 0xEB, 2,
                    color, s->data->pnltex[1]);
        nbPnlSprite(ox, y + 0x13D, 0x51, 0x18, 1, 1, 0x51, 0x19,
                    color, s->data->pnltex[1]);
        if (type != 3) {
            lineX = ox + 0x17;
            nbPnlSprite(lineX, y + 0x16C, 0xC5, 2, 1, 0x77, 0xC5, 2,
                        color, s->data->pnltex[1]);
            nbPnlSprite(lineX, y + 0x181, 0xC5, 2, 1, 0x77, 0xC5, 2,
                        color, s->data->pnltex[1]);
            nbPnlSprite(lineX, y + 0x196, 0xC5, 2, 1, 0x77, 0xC5, 2,
                        color, s->data->pnltex[1]);
        }
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCommSelProcess", DispPanelSitaji);
#endif

/* NON_MATCHING: 3 of 56 words differ (first at +0x28). */

#ifdef NON_MATCHING

void DispComm1PanelComm(nbCommSelProcessData_t *s, u16 d, s32 ox, s32 oy, s32 oz, u32 col)
{
    s32 no = d;

    if (no <= 0x8003) {
        if (no >= 0x8000) {
            return;
        }
    }
    switch (datSkillTable.normal[no].costtype) {
    case 1:
        nbPnlSprite(ox + 0x9B, oy + 5, 0x14, 0xB, 0xC, 0x4E, 0x14, 0xB, col,
                    s->data->pnltex[0]);
        break;
    case 2:
        nbPnlSprite(ox + 0x9B, oy + 5, 0x14, 0xB, 0x21, 0x4E, 0x14, 0xB, col,
                    s->data->pnltex[0]);
        break;
    }
}

#else

INCLUDE_ASM(const s32, "newbattle/nbCommSelProcess", DispComm1PanelComm);

#endif

void DispComm1PanelTalk(nbCommSelProcessData_t *s, u16 d, s32 ox, s32 oy, s32 oz, u32 col) {
}

void DispComm1PanelItem(nbCommSelProcessData_t *s, u16 d, s32 ox, s32 oy, s32 oz, u32 col)
{
    nbPnlSprite(ox + 0x8C, oy + 6, 9, 9, 1, 0x4E, 9, 9, col, s->data->pnltex[0]);
}

void DispHpPanel(sdfTexHandle_t *th, datUnitWork_t *w, s32 ox, s32 oy, s32 oz, u32 col)
{
    s32 bar;

    nbPnlSprite(ox, oy, 0x14, 0xB, 0xC, 0x4E, 0x14, 0xB, col, th);
    oy += 0xC;
    nbPnlSprite(ox, oy, 0x32, 6, 1, 0x14, 0x31, 5, col, th);
    if (w->maxhp == 0) {
        return;
    }
    bar = w->hp * 49 / w->maxhp;
    if (bar == 0) {
        return;
    }
    nbPnlSprite(ox, oy, bar, 6, 1, 0xD, bar, 5, col, th);
}

void DispMpPanel(sdfTexHandle_t *th, datUnitWork_t *w, s32 ox, s32 oy, s32 oz, u32 col)
{
    s32 bar;

    nbPnlSprite(ox, oy, 0x14, 0xB, 0x21, 0x4E, 0x14, 0xB, col, th);
    oy += 0xC;
    nbPnlSprite(ox, oy, 0x32, 6, 1, 0x14, 0x31, 5, col, th);
    if (w->maxmp == 0) {
        return;
    }
    bar = w->mp * 49 / w->maxmp;
    if (bar == 0) {
        return;
    }
    nbPnlSprite(ox, oy, bar, 6, 1, 0x1B, bar, 5, col, th);
}

void DispComm1PanelSummon(nbCommSelProcessData_t *s, u16 d, s32 ox, s32 oy,
                          s32 oz, u32 color)
{
    datUnitWork_t *work = (datUnitWork_t *)&DDS3_GBWK->akuma[d].flag;
    s32 u = 0;
    s32 v = 0;
    s32 status;

    nbPnlSprite(ox + 0x7F, oy + 5, 0x14, 0xB, 0x36, 0x4E, 0x14, 0xB,
                color, s->data->pnltex[0]);
    DispHpPanel(s->data->pnltex[0], work, ox + 2, oy + 0x14, oz, color);
    DispMpPanel(s->data->pnltex[0], work, ox + 0x3D, oy + 0x14, oz, color);
    status = work->badstatus & 0xFFF;
    if (status == 0) {
        return;
    }
    switch (status) {
    case 1:     u = 0x40; v = 0x38; break;
    case 2:     u = 0x2B; v = 0x38; break;
    case 4:     u = 0x16; v = 0x38; break;
    case 8:     u = 1;    v = 0x38; break;
    case 0x10:  u = 0x7F; v = 0x22; break;
    case 0x20:  u = 0x6A; v = 0x22; break;
    case 0x40:  u = 0x55; v = 0x22; break;
    case 0x80:  u = 0x40; v = 0x22; break;
    case 0x100: u = 0x2B; v = 0x22; break;
    case 0x200: u = 0x16; v = 0x22; break;
    case 0x400: u = 1;    v = 0x22; break;
    case 0x800: u = 0x55; v = 0x38; break;
    }
    nbPnlSprite(ox + 0x89, oy + 0x11, 0x14, 0x15, u, v, 0x14, 0x15,
                color, s->data->pnltex[0]);
}


/* NON_MATCHING: 29 of 97 words differ (first at +0x8). */

#ifdef NON_MATCHING

void DispCommCursor(s32 ox, s32 oy, s32 a)
{
    sdfDrawTag_t *list;
    sdfGrOt_t *ot;
    s32 xb;
    s32 x1;
    s32 alpha = 0x46;
    s32 y0, y1;
    u32 c0, c1;

    if (a != 0x80) {
        alpha = a * 0.0078125f * 70.0f;
    }
    xb = ox << 4;
    x1 = xb + 0x7630;
    ox = xb + 0x7000;
    list = dds3FontReqAllocDrawTag();
    y0 = (oy << 3) + 0x7900;
    y1 = (oy << 3) + 0x7998;
    c0 = (alpha << 24) | 0x8CBE1B;
    c1 = (alpha << 24) | 0x9B4400;
    sdfPrimLinkG4(list, 0x14C, ox, y0, c0, x1, y0, c1, ox, y1, c0, x1, y1, c1, -1, NULL);
    ox = x1;
    sdfPrimLinkG4(list, 0x14C, ox, y0, c1, xb + 0x7C60, y0, 0xBF0013, ox, y1, c1, xb + 0x7C60, y1,
                  0xBF0013, -1, NULL);
    ot = &sdfGrOtTable[71];
    ot->regFunc(ot, list);
}

#else

INCLUDE_ASM(const s32, "newbattle/nbCommSelProcess", DispCommCursor);

#endif

#ifdef NON_MATCHING
void DispSummonCursor(s32 ox, s32 oy, s32 a)
{
    sdfDrawTag_t *tag;
    sdfGrOt_t *ot;
    f32 rate;
    s32 alpha;
    s32 halfAlpha;
    s32 x0, x1, x2, y0, y1, y2;
    u32 left, middle, halfLeft, halfMiddle;

    if (a == 0x80) {
        alpha = 0x46;
        halfAlpha = 0x23;
    } else {
        rate = (f32)a * 0.0078125f;
        halfAlpha = (s32)(rate * 35.0f);
        alpha = (s32)(rate * 70.0f);
    }
    tag = dds3FontReqAllocDrawTag();
    left = ((u32)alpha << 24) | 0x8CBE1B;
    middle = ((u32)alpha << 24) | 0x9B4400;
    x0 = (u32)ox * 16 + 0x7000;
    x1 = (u32)ox * 16 + 0x7630;
    x2 = (u32)ox * 16 + 0x7C60;
    y0 = (u32)oy * 8 + 0x7900;
    y1 = (u32)oy * 8 + 0x7998;
    y2 = (u32)oy * 8 + 0x7A48;
    sdfPrimLinkG4(tag, 0x14C, x0, y0, left, x1, y0, middle,
                  x0, y1, left, x1, y1, middle, -1, NULL);
    sdfPrimLinkG4(tag, 0x14C, x1, y0, middle, x2, y0, 0xBF0013,
                  x1, y1, middle, x2, y1, 0xBF0013, -1, NULL);
    halfLeft = ((u32)halfAlpha << 24) | 0x8CBE1B;
    halfMiddle = ((u32)halfAlpha << 24) | 0x9B4400;
    sdfPrimLinkG4(tag, 0x14C, x0, y1, halfLeft, x1, y1, halfMiddle,
                  x0, y2, halfLeft, x1, y2, halfMiddle, -1, NULL);
    sdfPrimLinkG4(tag, 0x14C, x1, y1, halfMiddle, x2, y1, 0xBF0013,
                  x1, y2, halfMiddle, x2, y2, 0xBF0013, -1, NULL);
    ot = &sdfGrOtTable[71];
    ot->regFunc(ot, tag);
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCommSelProcess", DispSummonCursor);
#endif

void DispComm1PanelCursor(nbCommSelProcessData_t *s, s32 type, s32 ox, s32 oy, s32 oz, u32 col)
{
    s32 a = col >> 24;

    switch (type) {
    case 0:
    case 1:
    case 2:
    case 4:
    case 5:
        DispCommCursor(ox - 5, oy, a);
        break;
    case 3:
        DispSummonCursor(ox - 0xB, oy - 1, a);
        a = 0;
        break;
    }
}

extern s32 evtBitCheck(s32 no);

#ifdef NON_MATCHING
void DispHelpPanel(nbCommSelProcessData_t *s, u16 d, s32 x, s32 y, s32 oz,
                   u32 color, s32 helpa)
{
    sdfDrawTag_t *tag;
    sdfGrOt_t *ot;
    s32 alpha;
    s32 x0, x1, x2, y0, y1;
    u32 background;

    if (evtBitCheck(0xC0E) != 0) {
        return;
    }
    x = (u32)x + 0x43;
    y = (u32)y - 0x22;
    color = (color & 0xFFFFFF) | ((u32)helpa << 24);
    alpha = 0x46;
    if (helpa != 0x80) {
        alpha = (s32)((f32)helpa * 0.0078125f * 70.0f);
    }
    if (s->drawtype != 3) {
        nbPnlSprite(x, y, 0x6E, 0x19, 1, 0x48, 0x6E, 0x19,
                    color, s->data->pnltex[1]);
    }
    tag = dds3FontReqAllocDrawTag();
    x0 = (u32)x * 16 + 0x76E0;
    x1 = (u32)x * 16 + 0x8580;
    x2 = (u32)x * 16 + 0x8A00;
    y0 = (u32)y * 8 + 0x7900;
    y1 = (u32)y * 8 + 0x79C8;
    background = ((u32)alpha << 24) | 0x221810;
    sdfPrimLinkG4(tag, 0x14C, x0, y0, background, x1, y0, background,
                  x0, y1, background, x1, y1, background, -1, NULL);
    sdfPrimLinkG4(tag, 0x14C, x1, y0, background, x2, y0, 0x221810,
                  x1, y1, background, x2, y1, 0x221810, -1, NULL);
    ot = &sdfGrOtTable[71];
    ot->regFunc(ot, tag);
}
#else
INCLUDE_ASM(const s32, "newbattle/nbCommSelProcess", DispHelpPanel);
#endif

extern void DispPanelSitaji(nbCommSelProcessData_t *s, s32 type, s32 ox, s32 oy, u32 color);
extern void DispHelpPanel(nbCommSelProcessData_t *s, u16 d, s32 x, s32 y, s32 oz, u32 color,
                          s32 helpa);
extern void DispComm1PanelSummon(nbCommSelProcessData_t *s, u16 d, s32 ox, s32 oy, s32 oz,
                                 u32 color);

extern void DispComm1PanelComm(nbCommSelProcessData_t *s, u16 d, s32 ox, s32 oy,
                                s32 oz, u32 color);

/* NON_MATCHING: 203 of 207 words match with the command-row C draft visible. */
#ifdef NON_MATCHING
void DispCommandPanel(nbCommSelProcessData_t *s, s32 type, s32 ox, s32 oy, s32 alpha)
{
    s32 color = ((u32)alpha << 24) | 0x808080;
    s32 count;
    s32 rowStep = 0;
    s32 originalX = ox;
    s32 helpX;
    s32 helpY;
    s32 originalY = oy;
    s32 motion;
    s32 i;

    nbPnl_setOt(0x47);
    nbPnl_rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    nbPnl_rsetAlpha(0);
    DispPanelSitaji(s, type, originalX, originalY, color);
    DispPanelCursor(s, type, originalX, originalY, color);
    count = GetCommListCnt(s, type);
    motion = s->smritu * 43.0f;

    if ((u32)type < 6) {
        switch (type) {
        case 0:
        case 1:
        case 2:
        case 4:
        case 5:
            rowStep = 0x15;
            ox += 0x1B;
            oy += 0x159;
            break;
        case 3:
            rowStep = 0x2B;
            ox += 0x21;
            oy += motion + 0x12E;
            break;
        }
    }

    helpX = originalX + 0x1B;
    helpY = originalY + 0x159;
    for (i = s->drawlisty; i < s->drawlisty + count; i++) {
        if (i < s->commcnt[type]) {
            u16 entry = s->commlist[type][i];

            if (i == s->drawcursor) {
                if (s->stat == 2) {
                    DispComm1PanelCursor(s, type, ox, oy, s->oz, color);
                }
            }
            if (i == s->drawcursor) {
                DispHelpPanel(s, entry, helpX, helpY, s->oz - 1, color, s->helpa);
            }

            switch (type) {
            case 0:
                DispComm1PanelComm(s, entry, ox, oy, s->oz, color);
                break;
            case 1:
                DispComm1PanelTalk(s, entry, ox, oy, s->oz, color);
                break;
            case 2:
                DispComm1PanelItem(s, entry, ox, oy, s->oz, color);
                break;
            case 3:
                DispComm1PanelSummon(s, entry, ox, oy, s->oz, color);
                break;
            }
            oy += rowStep;
        }
    }
    DispPanelLabel(s, type, originalX, originalY, color);
}

#else
INCLUDE_ASM(const s32, "newbattle/nbCommSelProcess", DispCommandPanel);
#endif

/* NON_MATCHING: 2 of 99 words differ (first at +0x12C). */

#ifdef NON_MATCHING

void AnimeCommandList(nbCommSelProcessData_t *s)
{
    if (s->animeflag & 0x20) {
        if (s->helpa < 128) {
            s->helpa += 32;
            if (s->helpa > 128) {
                s->helpa = 128;
            }
        }
    } else {
        if (s->helpa > 0) {
            s->helpa -= 21;
            if (s->helpa < 0) {
                s->helpa = 0;
            }
        }
    }
    if (s->animeflag & 1) {
        if (s->alpha == 128) {
            s->animeflag &= ~1;
        } else {
            s->alpha += 20;
            if (s->alpha > 128) {
                s->alpha = 128;
            }
        }
    }
    if (s->animeflag & 2) {
        if (s->alpha == 0) {
            s->animeflag &= ~2;
        } else {
            s->alpha -= 12;
            if (s->alpha < 0) {
                s->alpha = 0;
            }
        }
    }
    if (s->animeflag & 8) {
        if (s->ox == 0) {
            s->animeflag &= ~8;
        } else {
            s32 d = (s32)(s->ox * 0.5f);

            if (d == 0) {
                d = -1;
            }
            s->ox -= d;
        }
    }
    if (s->animeflag & 4) {
        if (fabsf(s->smritu) < 0.01f) {
            s->smritu = 0.0f;
            s->animeflag &= ~4;
        } else {
            f32 d = s->smritu * 0.6f;

            if (d < 0.01f) {
                d = 0.01f;
            }
            s->smritu -= d;
            if (s->smritu < 0.0f) {
                s->smritu = 0.0f;
            }
        }
    }
}

#else

INCLUDE_ASM(const s32, "newbattle/nbCommSelProcess", AnimeCommandList);

#endif

void DispComm1Summon(nbCommSelProcessData_t *s, u16 d, s32 ox, s32 oy, s32 oz, u32 col)
{
    char buf[16];
    datUnitWork_t *w = (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54) + d;
    void *frq;
    void *frq2;
    u32 c;
    s32 per;
    u32 a;
    s32 id = w->id;

    cmpSetSkillListRows(0x13);
    frq = frwGetSmallFontFRQ(ox << 4, oy << 3, 0xFF0051, col, text2[id], NULL);
    if (w->level < 0x64) {
        sprintf(buf, D_00551130, w->level);
    } else {
        sprintf(buf, D_00551130, 0x63);
    }
    frq = frwGetSmallNumFontFRQ((ox + 0x8C) << 4, (oy + 4) << 3, 0xFF0051, col, buf, frq);
    c = 0x754B3D80;
    per = w->hp * 100 / w->maxhp;
    if (per >= 0x19) {
        if (per < 0x32) {
            c = 0xB4A06480;
        } else {
            c = 0xA09DC380;
        }
    }
    a = col & 0xFF;
    c = (c & 0xFFFFFF00) | a;
    sprintf(buf, D_00551130, w->hp);
    frq2 = frwGetSmallNumFontFRQ((ox + 0x17) << 4, (oy + 0x13) << 3, 0xFF0051, c, buf, NULL);
    frDrawFontEx(frq2, 1);
    frDestroyFontRequestAfter(frq2);
    c = a | (0xA09DC380 & 0xFFFFFF00);
    sprintf(buf, D_00551130, w->mp);
    frq2 = frwGetSmallNumFontFRQ((ox + 0x52) << 4, (oy + 0x13) << 3, 0xFF0051, c, buf, NULL);
    frDrawFontEx(frq2, 1);
    frDestroyFontRequestAfter(frq2);
    frDrawFontEx(frq, 1);
    frDestroyFontRequestAfter(frq);
    cmpSetSkillListRows(-1);
}

void DispComm1Item(nbCommSelProcessData_t *s, u16 d, s32 ox, s32 oy, s32 oz, u32 col)
{
    char buf[16];
    void *frq;

    cmpSetSkillListRows(0x13);
    frq = frwGetSmallFontFRQ(ox << 4, oy << 3, 0xFF0051, col, pStr[d], NULL);
    sprintf(buf, D_00551130, ((u8 *)(u32)d + (u32)DDS3_GBWK)[0x1928]);
    frq = frwGetSmallNumFontFRQ((ox + 0x8E) << 4, (oy + 4) << 3, 0xFF0051, col, buf, frq);
    frDrawFontEx(frq, 1);
    frDestroyFontRequestAfter(frq);
    cmpSetSkillListRows(-1);
}

void DispComm1Talk(nbCommSelProcessData_t *s, u16 d, s32 ox, s32 oy, s32 oz, u32 col)
{
    void *frq;

    cmpSetSkillListRows(0x13);
    if (d == 0x8004) {
        frq = frwGetSmallFontFRQ(ox << 4, oy << 3, 0xFF0051, col, text1[0x205], NULL);
    } else {
        frq = frwGetSmallFontFRQ(ox << 4, oy << 3, 0xFF0051, col, text1[d], NULL);
    }
    frDrawFontEx(frq, 1);
    frDestroyFontRequestAfter(frq);
    cmpSetSkillListRows(-1);
}

void DispComm1Comm(nbCommSelProcessData_t *s, u16 d, s32 ox, s32 oy, s32 oz, u32 col)
{
    char buf[16];
    datUnitWork_t *work = nbGetUnitWorkPointer(s->data, s->my);
    void *frq;
    s32 costtype;

    cmpSetSkillListRows(0x13);
    switch (d) {
    case 0x8000:
        frq = frwGetSmallFontFRQ(ox << 4, oy << 3, 0xFF0051, col, text1[0x200], NULL);
        break;
    case 0x8001:
        frq = frwGetSmallFontFRQ(ox << 4, oy << 3, 0xFF0051, col, text1[0x201], NULL);
        break;
    case 0x8002:
        frq = frwGetSmallFontFRQ(ox << 4, oy << 3, 0xFF0051, col, text1[0x202], NULL);
        break;
    case 0x8003:
        frq = frwGetSmallFontFRQ(ox << 4, oy << 3, 0xFF0051, col, text1[0x204], NULL);
        break;
    default:
        frq = frwGetSmallFontFRQ(ox << 4, oy << 3, 0xFF0051, col, text1[d], NULL);
        costtype = datSkillTable.normal[d].costtype;
        if (costtype < 3) {
            if (costtype != 0) {
                sprintf(buf, D_00551130, datGetSkillCost(work, d));
                frq = frwGetSmallNumFontFRQ((ox + 0x78) << 4, (oy + 4) << 3, 0xFF0051, col, buf,
                                            frq);
            }
        }
        break;
    }
    frDrawFontEx(frq, 1);
    frDestroyFontRequestAfter(frq);
    cmpSetSkillListRows(-1);
}

typedef struct frFontRequest_t frFontRequest_t;
typedef struct frTextBank_t frTextBank_t;
typedef struct frFontHandle_t frFontHandle_t;
extern frFontRequest_t *frGetHelpPos(s32 x, s32 y, s32 priority, s32 bank, s32 index);
extern u8 *MessageNumSearch(s32 index, frTextBank_t *bank, s32 size);
extern frFontHandle_t *MessageRequestNS(s32 x, s32 y, s32 depth, s32 style, s32 color0,
                                        s32 color1, s32 color2, u8 *text,
                                        frFontHandle_t *previous);
extern void frSetFontColor(frFontHandle_t *font, u32 color);
extern s16 D_00551134;
extern frTextBank_t itfMesEmbedded_3F7FC8;

void DispHelp1(nbCommSelProcessData_t *s, u16 d, s32 ox, s32 oy, s32 oz, s32 alpha)
{
    s32 x;
    s32 y;
    s32 index = d;
    void *font = NULL;

    if (evtBitCheck(0xC0E) != 0) {
        return;
    }

    x = (ox + 0xA9) << 4;
    y = (oy - 0x1F) << 3;
    cmpSetSkillListRows(0x13);
    switch (s->drawtype) {
    case 0:
        switch (index) {
        case 0x8000:
            font = MessageRequestNS(x, y, oz, 1, 0, 0, 0x80,
                                    MessageNumSearch(1, &itfMesEmbedded_3F7FC8, 0), NULL);
            D_00551134 = 0;
            break;
        case 0x8001:
            font = MessageRequestNS(x, y, oz, 1, 0, 0, 0x80,
                                    MessageNumSearch(4, &itfMesEmbedded_3F7FC8, 0), NULL);
            D_00551134 = 0;
            break;
        case 0x8002:
            font = MessageRequestNS(x, y, oz, 1, 0, 0, 0x80,
                                    MessageNumSearch(2, &itfMesEmbedded_3F7FC8, 0), NULL);
            D_00551134 = 0;
            break;
        case 0x8003:
            font = MessageRequestNS(x, y, oz, 1, 0, 0, 0x80,
                                    MessageNumSearch(5, &itfMesEmbedded_3F7FC8, 0), NULL);
            D_00551134 = 0;
            break;
        default:
            font = frGetHelpPos(x, y, oz, 0, index);
            break;
        }
        break;
    case 1:
        if (index != 0x8004) {
            cmpSetSkillListRows(0x12);
            font = frGetHelpPos(x, y, oz, 0, index);
        } else {
            font = MessageRequestNS(x, y, oz, 1, 0, 0, 0x80,
                                    MessageNumSearch(3, &itfMesEmbedded_3F7FC8, 0), NULL);
        }
        break;
    case 2:
        cmpSetSkillListRows(0x12);
        font = frGetHelpPos(x, y, oz, 1, index);
        break;
    case 4:
        font = MessageRequestNS(x, y, oz, 1, 0, 0, 0x80,
                                MessageNumSearch(5, &itfMesEmbedded_3F7FC8, 0), NULL);
        break;
    case 5:
        font = MessageRequestNS(x, y, oz, 1, 0, 0, 0x80,
                                MessageNumSearch(4, &itfMesEmbedded_3F7FC8, 0), NULL);
        break;
    }

    if (font != NULL) {
        frSetFontColor(font, 0x80808000 | alpha);
        frDrawFontEx(font, 1);
        frDestroyFontRequestAfter(font);
    }
    cmpSetSkillListRows(-1);
}

void DispCommandList2(nbCommSelProcessData_t *s, s32 type, s32 ox, s32 oy, s32 alpha)
{
    s32 rowStep;
    s32 count;
    s32 y = oy;
    s32 baseY;
    s32 index;
    s32 id;
    s32 color = 0;
    s32 oz = s->oz;
    s32 slide = (s32)(s->smritu * 43.0f);

    /* Keep the normalized X in its live coordinate; allocation evidence:
     * docs/levers/cross-block-locals.md. */
    if (type == 3) {
        rowStep = 0x2B;
        y += slide + 0x12E;
        ox += 0x21;
    } else {
        rowStep = 0x15;
        ox += 0x1B;
        y += 0x159;
    }
    count = GetCommListCnt(s, type);
    baseY = y;
    index = s->drawlisty;
    while (index < s->drawlisty + count) {
        if (index < s->commcnt[type]) {
            id = s->commlist[type][index];
            if (index == s->drawcursor) {
                color = 0x89FEFF80;
            } else if (s->commdisable[type][index] == 0) {
                color = 0xA09DC380;
            }
            if (s->commdisable[type][index] != 0) {
                color = 0x4D4B4B4D;
            }
            color = (color & ~0xFF) | alpha;
            if ((u32)type < 6) {
                switch (type) {
                case 0:
                case 4:
                case 5:
                    DispComm1Comm(s, id, ox, y, oz, color);
                    break;
                case 1:
                    DispComm1Talk(s, id, ox, y, oz, color);
                    break;
                case 2:
                    DispComm1Item(s, id, ox, y, oz, color);
                    break;
                case 3:
                    DispComm1Summon(s, id, ox, y, oz, color);
                    break;
                }
            }
            if (index == s->drawcursor) {
                DispHelp1(s, id, ox, baseY, oz, s->helpa);
            }
            y += rowStep;
        }
        index++;
    }
}

s32 CheckSelectError(nbCommSelProcessData_t *s)
{
    s32 type = s->nowtype;

    if (s->commcnt[type] == 0) {
        return 1;
    }
    return s->commdisable[type][s->nowcursor[type]] > 0;
}

void MemoryCommand(nbCommSelProcessData_t *s)
{
    datUnitWork_t *work = nbGetUnitWorkPointer(s->data, s->my);
    s32 type = s->nowtype;
    u16 d = s->commlist[type][s->nowcursor[type]];

    switch (type) {
    case 0:
    case 4:
    case 5:
        switch (d) {
        case 0x8000:
            work->nowcommand = 0;
            work->nowindex = 0;
            break;
        case 0x8001:
            work->nowindex = 0;
            work->nowcommand = 2;
            break;
        case 0x8002:
            work->nowindex = 0;
            work->nowcommand = 3;
            break;
        case 0x8003:
            work->nowindex = 0;
            work->nowcommand = 7;
            break;
        default:
            work->nowindex = d;
            work->nowcommand = 1;
            break;
        }
        break;
    case 1:
        if (d == 0x8004) {
            work->nowcommand = 4;
            work->nowindex = 0x180;
            break;
        }
        work->nowindex = d;
        work->nowcommand = 4;
        break;
    case 2:
        work->nowindex = d;
        work->nowcommand = 5;
        break;
    case 3:
        work->nowindex = d;
        work->nowcommand = 6;
        break;
    }
}

s32 GetNextCommType(nbCommSelProcessData_t *s, s32 dir)
{
    s32 type = s->nowtype;
    s32 i;

    i = 0;
    do {
        type = (dir != 0 ? type + 1 : type + 5) % 6;
        if (s->commcnt[type] > 0) {
            return type;
        }
        i++;
    } while (i < 6);
    return type;
}

INCLUDE_ASM(const s32, "newbattle/nbCommSelProcess", SelectCommandList);

void SelectCommandListAuto(nbCommSelProcessData_t *s)
{
    if (CheckSelectError(s) == 0) {
        s->stat = 3;
        MemoryCommand(s);
    }
}

s32 nbGetCommSelStat(void)
{
    return GetCommSelProcessData()->stat;
}

void nbSetCommSelStat(s32 stat)
{
    GetCommSelProcessData()->stat = stat;
}

void nbCloseCommSel(void)
{
    nbCommSelProcessData_t *s = GetCommSelProcessData();

    s->stat = 4;
    s->animeflag = (s->animeflag | 2) & ~1;
}

void *nbCommSelProcess1(dds3ProcessID_t *id)
{
    nbCommSelProcessData_t *s;
    nbActionProcessData_t *act;
    u32 actstat;
    u32 animeFlags;
    s32 resetStat3 = 0;
    s32 animation;
    s32 ox;
    s32 oy;
    s32 alpha;

    s = dds3GetProcessData(id);
    AnimeCommandList(s);

    switch (s->stat) {
    case 1:
        s->stat = 2;
        break;
    case 2:
        if ((s->act->stat & 0x80) != 0) {
            SelectCommandListAuto(s);
        } else {
            SelectCommandList(s);
        }
        if (s->stat == 3 && (s->act->stat & 0x80) == 0) {
            resetStat3 = 1;
            s->stat = 2;
        }
        break;
    case 3:
        break;
    case 4:
        if ((s->animeflag & 2) == 0) {
            return (void *)-1;
        }
        break;
    }

    act = s->act;
    animeFlags = s->animeflag;
    animation = animeFlags & 0x10;
    if (animation == 0) {
        s->drawtype = s->nowtype;
        s->drawcursor = s->nowcursor[s->drawtype];
        s->drawlisty = s->nowlisty[s->drawtype];
    }

    actstat = act->stat;
    if ((actstat & 0x80) != 0 && animation == 0) {
        return NULL;
    }
    ox = s->ox;
    oy = s->oy;
    alpha = s->alpha;
    s->animeflag = animeFlags & ~0x20;
    if ((animeFlags & 2) == 0 && s->stat == 2 && s->drawtype != 3) {
        s->animeflag |= 0x20;
    }
    DispCommandPanel(s, s->drawtype, ox, oy, alpha);
    nbPnl_setOt(0x47);
    nbPnl_rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    nbPnl_rsetAlpha(0);
    DispCommandList2(s, s->drawtype, ox, oy, alpha);

    if (resetStat3 != 0) {
        s->stat = 3;
    }
    return NULL;
}

void nbCommSelShutdown(dds3ProcessID_t *id)
{
    nbCommSelProcessData_t *s = dds3GetProcessData(id);
    nbMainProcessData_t *data = s->data;

    if (data->CommSelProcessID == id) {
        data->CommSelProcessID = NULL;
    }
    sdfMemFreeDirect(s->handle);
}

/* Native eight-byte skill attributes, also consumed by datCalc and nbCalc. */
typedef struct {
    s8 skillattr;
    s8 type;
    s16 index;
    u16 keisyoform;
    s8 capacity;
    u8 flag;
} nbCommSkillAttr_t;
extern nbCommSkillAttr_t attr[];

/* The native inventory scan uses eight-byte item rows; no table extent
 * is inferred from its 160 inventory slots. */
typedef struct {
    u8 use;
    u8 flag;
    u16 skill;
    u32 price;
} nbCommItem_t;
extern nbCommItem_t datItemTable[];

/* Negotiation rows are 0x68 bytes; NbNegoFlowOne reads the same first word. */
typedef struct {
    u32 type;
    u8 remainder[0x64];
} nbCommNegoSkill_t;
extern nbCommNegoSkill_t D_003E1338[];

/* Partial save-work view: datAddDevil corroborates the unit, stock-list
 * and count offsets; this routine reads the following inventory bytes. */
typedef struct {
    u8 prefix[0xA54];
    datUnitWork_t unit[16];
    u8 stocklist[16];
    s32 stockcnt;
    u8 item[160];
} nbCommGlobalWork_t;
extern nbCommGlobalWork_t *nbCommGlobalWork asm("DDS3_GBWK");

extern s32 nbCheckSkillCost(s32 formindex, s32 nskill);
extern s32 nbGetEscapeSwitch(nbMainProcessData_t *data);
extern s32 nbGetRandamStock(void);

void SetCommList(nbCommSelProcessData_t *s, struct nbParty_t *my)
{
    datUnitWork_t *work = nbGetUnitWorkPointer(s->data, my);
    s32 badstatus = work->badstatus & 0xFFF;
    s32 i;
    /* Skill IDs, then stock IDs, with independent definitions per phase.
     * Qualified stock-RTL allocation: docs/levers/cross-block-locals.md. */
    s32 entry;
    s32 skillcount;

    for (i = 5; i >= 0; i--) {
        s->commcnt[i] = 0;
    }
    s->commlist[0][s->commcnt[0]] = 0x8000;
    s->commdisable[0][s->commcnt[0]] = 0;
    s->commcnt[0]++;
    if (work->flag & 4) {
        s->commlist[1][s->commcnt[1]] = 0x8004;
        s->commdisable[1][s->commcnt[1]] = 0;
        if (badstatus == 0x200) {
            s->commdisable[1][s->commcnt[1]] = 5;
        }
        s->commcnt[1]++;
    }

    skillcount = 0x200;
    if (!(s->data->stat & 0x4000000)) {
        skillcount = work->skillcnt;
    }
    for (i = 0; i < skillcount; i++) {
        entry = i;
        if (!(s->data->stat & 0x4000000)) {
            entry = work->skill[i];
        }
        if (entry == 0) {
            continue;
        }
        switch (attr[entry].type) {
        case -1: /* Unused skill-table entries. */
            break;
        case 0:
            if (datSkillTable.normal[entry].use & 2) {
                s->commlist[0][s->commcnt[0]] = entry;
                s->commdisable[0][s->commcnt[0]] = 0;
                if (datSkillTable.normal[entry].flag & 1) {
                    if (s->data->stat & 1) {
                        s->commdisable[0][s->commcnt[0]] = 1;
                    }
                }
                if (datSkillTable.normal[entry].flag & 2) {
                    if (s->data->stat & 4) {
                        s->commdisable[0][s->commcnt[0]] = 2;
                    }
                }
                if (nbCheckSkillCost(s->my->formindex, entry)) {
                    s->commdisable[0][s->commcnt[0]] = 3;
                }
                if (datSkillTable.normal[entry].koukatype == 1 &&
                    (work->badstatus & 0xFFF) == 0x20) {
                    s->commdisable[0][s->commcnt[0]] = 5;
                }
                switch (entry) {
                case 0x48:
                    if (!nbGetEscapeSwitch(s->data)) {
                        s->commdisable[0][s->commcnt[0]] = 1;
                    }
                    break;
                case 0xDF:
                    if (s->data->playerpcnt >= 4 || nbGetRandamStock() < 0) {
                        s->commdisable[0][s->commcnt[0]] = 6;
                    }
                    break;
                }
                s->commcnt[0]++;
            }
            break;
        case 1:
            break;
        case 2:
            if (D_003E1338[entry - 0x180].type < 7) {
                s->commlist[1][s->commcnt[1]] = entry;
                s->commdisable[1][s->commcnt[1]] = 0;
                if (badstatus == 0x200) {
                    s->commdisable[1][s->commcnt[1]] = 5;
                }
                s->commcnt[1]++;
            }
            break;
        }
    }

    if (work->flag & 4) {
        s->commlist[5][s->commcnt[5]] = 0x8001;
        s->commdisable[5][s->commcnt[5]] = 0;
        if (!nbGetEscapeSwitch(s->data)) {
            s->commdisable[5][s->commcnt[5]] = 1;
        }
        s->commcnt[5]++;
    } else {
        s->commlist[4][s->commcnt[4]] = 0x8003;
        s->commdisable[4][s->commcnt[4]] = 0;
        s->commcnt[4]++;
        s->commlist[5][s->commcnt[5]] = 0x8001;
        s->commdisable[5][s->commcnt[5]] = 0;
        if (!nbGetEscapeSwitch(s->data)) {
            s->commdisable[5][s->commcnt[5]] = 1;
        }
        s->commcnt[5]++;
    }
    s->commlist[0][s->commcnt[0]] = 0x8002;
    s->commdisable[0][s->commcnt[0]] = 0;
    s->commcnt[0]++;

    if (work->flag & 4) {
        for (i = 0; i < 160; i++) {
            if (nbCommGlobalWork->item[i] != 0 && (datItemTable[i].use & 2)) {
                s->commlist[2][s->commcnt[2]] = i;
                s->commdisable[2][s->commcnt[2]] = 0;
                if (datItemTable[i].flag & 1) {
                    if (s->data->stat & 1) {
                        s->commdisable[2][s->commcnt[2]] = 1;
                    }
                }
                if (datItemTable[i].flag & 8) {
                    if (s->data->stat & 4) {
                        s->commdisable[2][s->commcnt[2]] = 2;
                    }
                }
                if (i == 0x38) {
                    if (!nbGetEscapeSwitch(s->data)) {
                        s->commdisable[2][s->commcnt[2]] = 1;
                    }
                }
                s->commcnt[2]++;
            }
        }
        for (i = 0; i < nbCommGlobalWork->stockcnt; i++) {
            entry = nbCommGlobalWork->stocklist[i];
            if ((nbCommGlobalWork->unit[entry].flag & 1) &&
                !(nbCommGlobalWork->unit[entry].flag & 4) &&
                !(nbCommGlobalWork->unit[entry].flag & 2)) {
                s->commlist[3][s->commcnt[3]] = entry;
                s->commdisable[3][s->commcnt[3]] = 0;
                if (nbCommGlobalWork->unit[entry].hp == 0 ||
                    nbCommGlobalWork->unit[entry].badstatus == 0x800) {
                    s->commdisable[3][s->commcnt[3]] = 4;
                }
                s->commcnt[3]++;
            }
        }
    }
}

void SetPrevCursor(nbCommSelProcessData_t *s, s32 type, s32 ind)
{
    s32 listcnt = GetCommListCnt(s, type);
    s32 y = ind;

    s->nowtype = type;
    s->nowcursor[type] = ind;
    s->nowlisty[type] = ind;
    if (y + listcnt >= s->commcnt[type]) {
        y -= y + listcnt - s->commcnt[type];
        s->nowlisty[type] = (y < 0) ? 0 : y;
    }
    if (s->commdisable[type][ind] != 0) {
        s->nowtype = 0;
        s->nowcursor[type] = 0;
        s->nowlisty[type] = 0;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbCommSelProcess", SetCommCursor2);

void nbSetAutoCommSel(void)
{
    nbCommSelProcessData_t *s = GetCommSelProcessData();

    SetCommCursor2(s);
    s->animeflag |= 0x10;
}

nbCommSelProcessData_t *InitCommSelProcessData(nbMainProcessData_t *data, struct nbParty_t *my,
                                               nbActionProcessData_t *act)
{
    void *h = sdfMemAllocDirect(0x14D0);
    nbCommSelProcessData_t *s = sdfMemLock(h);
    s32 i;

    memset(s, 0, 0x14D0);
    s->handle = h;
    s->data = data;
    s->act = act;
    if ((act->stat & 0x80) != 0) {
        s->stat = 2;
    } else {
        s->stat = 1;
    }
    s->nowtype = 0;
    for (i = 0; i < 6; i++) {
        s->nowcursor[i] = 0;
        s->nowlisty[i] = 0;
    }
    s->oz = 0xFF0050;
    s->animeflag = 9;
    s->ox = -0x1E;
    s->cnt = 2;
    s->smritu = 1.0f;
    s->my = my;
    s->partytarget = NULL;
    s->alpha = 0;
    s->ritu = 0.0f;
    SetCommList(s, my);
    SetCommCursor2(s);
    return s;
}

void nbInitCommSelProcess(nbMainProcessData_t *data, struct nbParty_t *my,
                          nbActionProcessData_t *act)
{
    nbCommSelProcessData_t *s = InitCommSelProcessData(data, my, act);
    dds3ProcessID_t *id = dds3StartProcess(D_005441A8, 0x2B0E, 1, 1, nbCommSelProcess1,
                                           nbCommSelShutdown, s);

    dds3LinkProcess(data->MainProcessID, id);
    data->CommSelProcessID = id;
}

INCLUDE_SDATA(const s32, "newbattle/nbCommSelProcess", D_00551130);

INCLUDE_SDATA(const s32, "newbattle/nbCommSelProcess", D_00551134);

