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

extern void DispCommCursor(s32 ox, s32 oy, s32 a);

extern void DispSummonCursor(s32 ox, s32 oy, s32 a);

extern void nbPnlSprite(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, u32 rgba,
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

INCLUDE_ASM(const s32, "newbattle/nbCommSelProcess", DispPanelCursor);

INCLUDE_RODATA(const s32, "newbattle/nbCommSelProcess", D_005441A8);

INCLUDE_ASM(const s32, "newbattle/nbCommSelProcess", DispPanelLabel);

INCLUDE_ASM(const s32, "newbattle/nbCommSelProcess", DispPanelSitaji);

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

void DispComm1PanelTalk(void) {
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

INCLUDE_ASM(const s32, "newbattle/nbCommSelProcess", DispComm1PanelSummon);

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

INCLUDE_ASM(const s32, "newbattle/nbCommSelProcess", DispSummonCursor);

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

INCLUDE_ASM(const s32, "newbattle/nbCommSelProcess", DispHelpPanel);

INCLUDE_ASM(const s32, "newbattle/nbCommSelProcess", DispCommandPanel);

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

void DispComm1Summon(nbCommSelProcessData_t *s, u16 d, s32 ox, s32 oy, s32 oz, u32 col,
                     s32 listidx, s32 col2)
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

void DispComm1Item(nbCommSelProcessData_t *s, u16 d, s32 ox, s32 oy, s32 oz, u32 col,
                   s32 listidx, s32 col2)
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

void DispComm1Talk(nbCommSelProcessData_t *s, u16 d, s32 ox, s32 oy, s32 oz, u32 col,
                   s32 listidx, s32 col2)
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

void DispComm1Comm(nbCommSelProcessData_t *s, u16 d, s32 ox, s32 oy, s32 oz, u32 col,
                   s32 listidx, s32 col2)
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

INCLUDE_ASM(const s32, "newbattle/nbCommSelProcess", DispHelp1);

INCLUDE_ASM(const s32, "newbattle/nbCommSelProcess", DispCommandList2);

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

INCLUDE_ASM(const s32, "newbattle/nbCommSelProcess", nbCommSelProcess1);

void nbCommSelShutdown(dds3ProcessID_t *id)
{
    nbCommSelProcessData_t *s = dds3GetProcessData(id);
    nbMainProcessData_t *data = s->data;

    if (data->CommSelProcessID == id) {
        data->CommSelProcessID = NULL;
    }
    sdfMemFreeDirect(s->handle);
}

INCLUDE_ASM(const s32, "newbattle/nbCommSelProcess", SetCommList);

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

