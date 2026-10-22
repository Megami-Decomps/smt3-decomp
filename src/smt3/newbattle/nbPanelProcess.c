#include "common.h"
#include "libsdf.h"
#include "basic.h"
#include "scr.h"
#include "ps2asm.h"
#include "newdata.h"

typedef struct evtUnitHandle_t
{
    u8 unk_00[0x8C];
    dds3Basic_t *basic;
    u8 unk_90[0x14];
    u32 flags;
} evtUnitHandle_t;

typedef struct effBTLSysHandle_t effBTLSysHandle_t;

typedef struct NB_FILEHANDLE_T NB_FILEHANDLE_T;

typedef struct
{
    sdfVector4_t orgpos;
    sdfVector4_t orgrot;
    sdfVector4_t pos;
    sdfVector4_t rot;
    sdfVector4_t center;
    f32 r;
    f32 h;
    f32 cr;
    f32 scl;
    f32 oneshotr;
    f32 allshotr;
    f32 movey;
    f32 movez;
    s32 type;
    u32 time;
    u32 stat;
    u32 flag;
    u32 anime;
    dds3Basic_t *basic;
    s32 serial;
    evtUnitHandle_t *hunit;
    s16 partyindex;
    s16 formindex;
    u32 autotime;
    effBTLSysHandle_t *badeff;
    u32 modelcol;
    f32 motspeed;
    s16 motendtype;
    s16 motenddata;
    s16 nexthokan;
    s16 nexthokanb;
    s16 waitmot;
    s16 motcate;
    s32 reserve4;
    NB_FILEHANDLE_T *packh;
    char *akey;
    s32 read_minor;
    u32 tartime;
    f32 range;
    f32 addmovet;
    f32 movet;
    sdfVector4_t tarpos;
    sdfVector4_t startpos;
} nbFormation_t;

typedef struct nbParty_t
{
    u16 flag;
    s16 formindex;
    s16 statindex;
    u16 partyindex;
    s16 count[20];
    u32 aisyo[16];
    u16 hojoskill[8];
} nbParty_t;

/* A loaded pac file; only the chain down to the decoded buffer is named. */
typedef struct { u8 unk_00[0x8]; void *buf; } nbPacEntry_t;

typedef struct { u8 unk_00[0x60]; nbPacEntry_t *entry; } nbPacHandle_t;

/* 0xE0 bytes; only the field this unit reads is named. */
typedef struct
{
    s16 id;
    u16 unk_02;
    s32 refcnt;
    void *file[26];
    void *buf[26];
    void *motfile;
    void *motbuf;
} nbStatWork_t;

/* Only the fields this unit touches are named; the rest of the layout is unconfirmed. */
typedef struct nbMainProcessData_t
{
    nbFormation_t *form;
    u8 unk_04[0xC];
    nbParty_t *party;
    u8 unk_14[0x4];
    s16 playerpcnt;
    s16 enemypcnt;
    u8 unk_1C[0x4];
    nbStatWork_t *statwork;
    void *statbuf;
    s32 unk_28;
    u8 unk_2C[0x8];
    s32 encno;
    u8 unk_38[0x8];
    s32 actionstackp;
    u32 endtype;
    u8 unk_48[0x18];
    u32 stat;
    u8 unk_64[0x4];
    u8 turn;
    u8 unk_69[0xF];
    s16 press4_p;
    s16 pressturn;
    u16 stagemajor;
    u8 unk_7E[0x6];
    s32 unk_84;
    s32 eventno;
    s32 eventrid;
    s16 negopformindex;
    s16 negoeformindex;
    s32 negoresult;
    u8 item[4];
    u8 itemcnt[4];
    u32 maka;
    u32 exp;
    u8 unk_A8[0xC];
    nbPacHandle_t *formpack;
    u8 unk_B8[0x4];
    dds3ProcessID_t *mainproc;
    u8 unk_C0[0x1C];
    void *packdata;
    u8 unk_E0[0x14];
    dds3ProcessID_t *koukaproc;
    dds3ProcessID_t *negoproc;
    u8 unk_FC[0x4];
    dds3ProcessID_t *debugproc;
    dds3ProcessID_t *aiproc;
    dds3ProcessID_t *pnlproc;
    dds3ProcessID_t *helpproc;
    dds3ProcessID_t *autocommproc;
    dds3ProcessID_t *encproc;
    dds3ProcessID_t *eventproc;
    u8 unk_11C[0x4];
    void *pnltex[4];
    s32 unk_130;
    s32 unk_134;
    nbPacHandle_t *modelpack;
    void *modelpackbuf;
} nbMainProcessData_t;

typedef struct _FRQ _FRQ;

/* Only the fields this unit reads are named; x and y are in 1/16 pixels. */
struct _FRQ
{
    u8 unk_00[0x4];
    s32 x;
    s32 y;
};

/* The box nbCenterText centres a string inside; only the width is named. */
typedef struct { u8 unk_00[0xC]; s32 width; } nbTextBox_t;

/* 0x38 bytes of panel flash state; retail never names the fields. */
typedef struct
{
    s32 PT_1_Frame;
    s16 PT_1_Process;
    u8 pad_06[0x2];
    s32 PT_2_Frame;
    s16 PT_2_Process;
    u8 pad_0E[0x2];
    f32 PT_2_Rate;
    s32 PT_3_Frame;
    s16 PT_3_Process;
    u8 pad_1A[0x2];
    f32 PT_3_Rate;
    s32 unk_20;
    s32 unk_24;
    f32 unk_28;
    f32 unk_2C;
    s32 abgr;
    s32 addY;
} nbPanelFlashData_t;

/* 0x1A4 bytes; only the fields this unit touches are named. */
typedef struct nbActionProcessData_t
{
    nbMainProcessData_t *data;
    u32 stat;
    u8 unk_08[0x1C];
    nbParty_t *party;
    nbFormation_t *form;
    datUnitWork_t *work;
    s32 partyindex;
    s32 type;
    s32 target;
    s32 select;
    u8 unk_40[0x28];
    s32 unk_68;
    u8 unk_6C[0x124];
    u32 aiselect;
    s32 aijokentbl;
    u8 aichktbl[0x8];
    u32 aisummonid;
} nbActionProcessData_t;

typedef struct nbNegoProcessData_t
{
    nbMainProcessData_t *data;
    nbActionProcessData_t *actdata;
    u32 stat;
    s32 negotype;
    s32 waitframe;
    u32 nownegoid;
    s32 nownegophase;
    u32 nextnegoid;
    s32 nextkouhocnt;
    u32 nextkouho[8];
    s32 nowmsgid;
    s32 nowmsgno;
    s32 nowmes;
    s32 msgwndid;
    u32 msgwndcnt;
    s32 nowselect;
    u32 workid[8];
    s32 requesttype;
    s32 requestdata;
    s32 requesthpmp;
    s32 prevreqtype;
    s32 cnt6_36;
    s32 cnt6_37;
    s32 cnt6_58;
    s32 cnt6_66;
    s32 mes6_2;
    s32 mes6_25;
    s32 mes6_67;
    s32 flag1_6;
    s32 flag1_39;
    s32 historyindex;
    u32 history[14];
    s32 historymes[14];
    s32 endid;
    s32 endtype;
    s32 negoreqtype;
    u32 negoskillid;
    s32 aisyo;
    s32 aisyomes;
    s32 hojopartyindex;
    s32 hojoskillid;
    s32 hojoseikou;
    s32 hojoaisyo;
    s32 hojoaisyomes;
    s32 qtype;
    s32 qindex;
    datUnitWork_t *pwork;
    datUnitWork_t *ework;
    s32 pformindex;
    s32 eformindex;
    nbFormation_t *pform;
    nbFormation_t *eform;
    s32 ppartyindex;
    s32 epartyindex;
    nbParty_t *pparty;
    nbParty_t *eparty;
    s32 pkucyo;
    s32 ekucyo;
    s32 innen_id;
    s32 temp_param[4];
    s32 boupartyindex;
    s32 getmakka;
    s32 getitem;
    s32 getinfotype;
    s32 getinfomsg;
    s32 flowmsg[31];
    s32 questionmsg;
    s32 aisyomsg[31];
    s32 infomsg[31];
    s32 helpmsg;
    s32 helpmsg2;
    s32 innenmsg;
    s32 bossmsg;
    s32 selectcur;
} nbNegoProcessData_t;

extern s32 printf(const char *format, ...);

extern nbMainProcessData_t *nbGetMainProcessData(void);

extern void *sdfTexSet(void *aTex);

extern void sdfTexFreeDelayed(void *aHandle);

extern void *pnbPnlFile1;

extern void *pnbPnlFile2;

extern void *pnbPnlFile3;

extern void *pnbPnlFile4;

extern s32 gnbPnlOtOfs;

extern s32 nbPnlDrawFlag;

extern f32 nbPnlCnt;

extern u32 nbPanelBaseABGR;

extern u32 nbPanelBaseRGBA;

extern s32 nbPanelPGPress;

extern s32 nbPanelPGNumber;

extern s32 nbPanelPGTurn;

extern s32 nbPanelPGTurnOld;

extern s32 nbPanelPGFrame;

extern s32 nbPanelPGBirthWait;

extern u32 nbPanelPGBaseColor;

extern s8 nbPanelPGShow;

extern s8 nbPanelPGStart;

extern s8 nbPanelPGEncount;

extern s32 nbPanelPT_1_Process;

extern s32 nbPanelPT_2_Process;

extern s32 nbPanelPT_3_Process;

extern s32 nbPanelPTP_1_Process;

extern s32 nbPanelPTAProcess;

extern s32 nbPanelPTAFrame;

extern f32 nbPanelPTARate_S;

extern f32 nbPanelPTARate_E;

extern s8 nbPanelPTAOnly;

extern s32 nbPanelPT_1_Frame;

extern s32 nbPanelPT_2_Frame;

extern f32 nbPanelPT_2_Rate;

extern s32 nbPanelPT_3_Frame;

extern f32 nbPanelPT_3_Rate;

extern s32 nbPanelPTP_0_Frame;

extern s32 nbPanelPTP_0_Process;

extern s32 nbPanelPTP_1_Frame;

extern s8 nbPanelPTOmit;

extern s32 nbPanelStockShiftX;

extern s32 nbPanelStockFrame;

extern u32 nbPanelStockABGR;

extern s8 nbPanelStockRun;

extern s8 nbPanelStockPad;

extern s8 nbPanelStockFadeFlag;

extern s32 nbPanelAnalyzeProcess;

extern s32 nbPanelAnalyzeFrame;

extern datUnitWork_t *pNbPanelAnalyzeUnitWork;

extern u32 PanelAutoABGR;

extern u32 PanelAutoBaseColor;

extern s8 PanelAutoShow;

extern s8 PanelPartyShow;

extern void nbPanelPressGaugeShow(void);

extern void nbPanelPressGaugeHide(void);

extern void nbPanelPartyShow(void);

extern void nbPanelPartyHide(void);

extern void nbPanelAutoShow(void);

extern void nbPanelAutoHide(void);

extern void nbPanelAllHide(void);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern s32 frGetStrWidth2(_FRQ *pFRQ);

extern _FRQ *frDestroyFontRequestAfter(_FRQ *pFRQ);

extern _FRQ *frwGetSmallNumFontFRQ(s32 X, s32 Y, s32 Pri, u32 Color, char **pStr, s32 pFRQ);

extern void func_001C7E38(_FRQ *frq, s32 mode, s32 kind);

extern u32 effMagaCylinderCalcColor(u32 color, u32 scale);

/* A panel rectangle: position, then width and height. */
typedef struct {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
    s32 h;
} nbPanelProcess_nbPanelXYZ_t;

typedef struct {
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;
} nbPanelProcess_nbPanelTMXuv_t;

extern void nbPnlSprite(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, u32 rgba,
                        sdfTexHandle_t *th);

extern void nbPnl_rsetTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte,
                           s32 ztst);

extern void nbPnl_rsetAlpha(s32 mode);

extern char D_00545488[];

extern char D_00545498[];

extern s32 nbPanelLayoutTable2[4][6];

extern s32 nbPanelDrawProcess(void);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, void *drawExit,
                                         void *deleteExit, void *p);

extern s32 nbPanelAutoCheckShow(void);

extern void nbPanelPressGaugeStart(void);

extern void nbPanelPressTurnGaugeAnimStart2(void);

extern void nbPanelPressTurnOmit(void);

/* The colour inlines read and write a 16-byte slot, one word of it used. */
typedef struct {
    u32 word;
    u8 pad04[12];
} nbColorWork_t;

/* The nb event script work; only the fields this unit touches are named. */
typedef struct nbEventProcessData_t {
    nbMainProcessData_t *data;
    s32 eventrid;
    s32 time;
    s32 seq;
    s32 seqtime;
    void *userdata;
    void (*unk_18)(dds3ProcessID_t *id);
    void *file;
    nbPacHandle_t *pfile;
    void *mem;
} nbEventProcessData_t;

extern u32 hnbPnlFile1;

extern u32 hnbPnlFile2;

extern u32 hnbPnlFile3;

extern u32 hnbPnlFile4;

extern char D_00545230[];

extern char D_00545250[];

extern char D_00545270[];

extern char D_00545290[];

extern u32 sdfDevLoadFileEx(char *path, void *dest, void *size);

extern s32 (*D_00527518[])(nbEventProcessData_t *e);

extern void nbPanelDraw(s32 flag);

extern s32 datCheckDying(datUnitWork_t *work);

extern s32 (*D_00527420[])(nbEventProcessData_t *e);

extern s32 (*D_004F0788[])(nbActionProcessData_t *a, s32 joken);

extern s32 (*D_00527688[])(nbEventProcessData_t *e);

extern s32 (*D_00527698[])(nbEventProcessData_t *e);

extern void *sdfGrAllocPacketBuff(s32 aSize);

extern void sdfDrawTagInit(sdfDrawTag_t *a);

extern u64 *sdf2DMakeStdPacket(void *aPacket, s32 aLeng);

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, void *aPacket);

extern sdfGrOt_t sdfGrOtTable[];

extern void frSetDrawPosition(_FRQ *pFRQ, s32 X, s32 Y);

extern s32 (*nbE902Table[])(nbEventProcessData_t *e);

extern s32 (*D_004EF4B0[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF550[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF698[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF6F8[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF780[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF7C8[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF7D8[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF810[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF868[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF8D8[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF928[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF950[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF960[])(nbNegoProcessData_t *n);

extern s32 (*D_004EFA28[])(nbNegoProcessData_t *n);

extern s32 (*D_004EFA60[])(nbNegoProcessData_t *n);

extern s32 (*D_004EFAC0[])(nbNegoProcessData_t *n);

void nbPnlLoad(void)
{
    hnbPnlFile1 = sdfDevLoadFileEx(D_00545230, &pnbPnlFile1, 0);
    hnbPnlFile2 = sdfDevLoadFileEx(D_00545250, &pnbPnlFile2, 0);
    hnbPnlFile3 = sdfDevLoadFileEx(D_00545270, &pnbPnlFile3, 0);
    hnbPnlFile4 = sdfDevLoadFileEx(D_00545290, &pnbPnlFile4, 0);
}

void nbPnlTexSet(nbMainProcessData_t *p)
{
    p->pnltex[0] = sdfTexSet(pnbPnlFile1);
    p->pnltex[1] = sdfTexSet(pnbPnlFile2);
    p->pnltex[2] = sdfTexSet(pnbPnlFile3);
    p->pnltex[3] = sdfTexSet(pnbPnlFile4);
}

void nbPnlTexDel(nbMainProcessData_t *p)
{
    sdfTexFreeDelayed(p->pnltex[0]);
    sdfTexFreeDelayed(p->pnltex[1]);
    sdfTexFreeDelayed(p->pnltex[2]);
    sdfTexFreeDelayed(p->pnltex[3]);
    p->pnltex[0] = NULL;
    p->pnltex[1] = NULL;
    p->pnltex[2] = NULL;
    p->pnltex[3] = NULL;
}

void nbPnl_setOt(s32 otofs)
{
    gnbPnlOtOfs = otofs;
}

void nbPnl_rsetTex1(s32 mmag, s32 mmin)
{
    sdfDrawTag_t *list;
    u64 *buff;
    u64 *packet;
    sdfGrOt_t *ot;

    list = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    buff = sdfGrAllocPacketBuff(0x30);
    packet = sdf2DMakeStdPacket(buff, 0x30);
    packet[4] = (mmin << 6) | (mmag << 5);
    packet[5] = 0x14;
    sdfDrawTagLink(list, buff);
    ot = &sdfGrOtTable[gnbPnlOtOfs];
    ot->regFunc(ot, list);
}

void nbPnl_rsetTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst)
{
    sdfDrawTag_t *list;
    u64 *buff;
    u64 *packet;
    sdfGrOt_t *ot;

    list = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    buff = sdfGrAllocPacketBuff(0x30);
    packet = sdf2DMakeStdPacket(buff, 0x30);
    packet[4] = (ztst << 17) | 0x10000 | (datm << 15) | (date << 14) | (afail << 12) |
                (aref << 4) | (atst << 1) | ate;
    packet[5] = 0x47;
    sdfDrawTagLink(list, buff);
    ot = &sdfGrOtTable[gnbPnlOtOfs];
    ot->regFunc(ot, list);
}

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545230);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545250);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545270);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545290);

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPnl_rsetAlpha);

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPnlSprite);

void nbPanelBaseColorSet(u32 abgr)
{
    nbPanelBaseABGR = abgr;
    nbPanelBaseRGBA = (abgr >> 24) | ((abgr >> 8) & 0xFF00) | ((abgr & 0xFF00) << 8) | (abgr << 24);
}

void nbPanelAllShow(void)
{
    nbPanelPressGaugeShow();
    nbPanelPartyShow();
    nbPanelAutoShow();
}

void nbPanelAllHide(void)
{
    nbPanelAutoHide();
    nbPanelPartyHide();
    nbPanelPressGaugeHide();
}

void nbPanelSpriteCenteringDraw(s32 x, s32 y, nbPanelProcess_nbPanelXYZ_t *pXYZ, nbPanelProcess_nbPanelXYZ_t *pArea,
                                nbPanelProcess_nbPanelTMXuv_t *pUV, u32 abgr, sdfTexHandle_t *hTex)
{
    s32 cx;
    u32 col;

    cx = pXYZ->x + ((pArea->w - pXYZ->w) >> 1);
    col = effMagaCylinderCalcColor(abgr, nbPanelBaseABGR);
    nbPnlSprite(cx + x, pXYZ->y + y, pXYZ->w, pXYZ->h, pUV->u0, pUV->v0, pUV->u1, pUV->v1, col, hTex);
}

void nbPanelSpriteScaleDraw(s32 x, s32 y, nbPanelProcess_nbPanelXYZ_t *pXYZ, nbPanelProcess_nbPanelTMXuv_t *pUV, u32 abgr, f32 scale,
                            sdfTexHandle_t *hTex)
{
    s32 sw;
    s32 sh;
    s32 px;
    s32 py;
    u32 col;

    sw = pXYZ->w * scale;
    sh = pXYZ->h * scale;
    px = pXYZ->x - ((sw - pXYZ->w) >> 1);
    py = pXYZ->y - ((sh - pXYZ->h) >> 1);
    col = effMagaCylinderCalcColor(abgr, nbPanelBaseABGR);
    nbPnlSprite(px + x, py + y, sw, sh, pUV->u0, pUV->v0, pUV->u1, pUV->v1, col, hTex);
}

void nbPanelSpriteScaleWHDraw(s32 x, s32 y, nbPanelProcess_nbPanelXYZ_t *pXYZ, nbPanelProcess_nbPanelTMXuv_t *pUV, u32 abgr, f32 scaleW,
                              f32 scaleH, sdfTexHandle_t *hTex)
{
    s32 sw;
    s32 sh;
    s32 px;
    s32 py;
    u32 col;

    sw = pXYZ->w * scaleW;
    sh = pXYZ->h * scaleH;
    px = pXYZ->x - ((sw - pXYZ->w) >> 1);
    py = pXYZ->y - ((sh - pXYZ->h) >> 1);
    col = effMagaCylinderCalcColor(abgr, nbPanelBaseABGR);
    nbPnlSprite(px + x, py + y, sw, sh, pUV->u0, pUV->v0, pUV->u1, pUV->v1, col, hTex);
}

void nbPanelSpriteBlendDraw(s32 x, s32 y, nbPanelProcess_nbPanelXYZ_t *pXYZ_0, nbPanelProcess_nbPanelTMXuv_t *pXYZ_1, u32 rate,
                            nbPanelProcess_nbPanelXYZ_t *pUV, f32 abgr, sdfTexHandle_t *hTex)
{
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 px;
    s32 py;
    s32 sw;
    s32 sh;
    u32 col;

    x0 = (pXYZ_0->x - pUV->x) * abgr;
    y0 = (pXYZ_0->y - pUV->y) * abgr;
    x1 = (pXYZ_0->x + pXYZ_0->w - pUV->x) * abgr;
    y1 = (pXYZ_0->y + pXYZ_0->h - pUV->y) * abgr;
    px = pUV->x + x0;
    py = pUV->y + y0;
    sw = x1 - x0;
    sh = y1 - y0;
    col = effMagaCylinderCalcColor(rate, nbPanelBaseABGR);
    nbPnlSprite(px + x, py + y, sw, sh, pXYZ_1->u0, pXYZ_1->v0, pXYZ_1->u1, pXYZ_1->v1, col, hTex);
}

void nbPanelSmallStringDraw(s32 x, s32 y, nbPanelProcess_nbPanelXYZ_t *pXYZ, u32 abgr, char **pString)
{
    _FRQ *frq;
    u32 col;

    col = effMagaCylinderCalcColor(abgr, nbPanelBaseRGBA);
    frq = frwGetSmallNumFontFRQ((pXYZ->x + x) << 4, (pXYZ->y + y) << 3, 0, col, pString, 0);
    func_001C7E38(frq, 1, 0x47);
    frDestroyFontRequestAfter(frq);
}

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelSmallNameStringDraw);

void nbCenterText(_FRQ *frq, nbTextBox_t *box)
{
    frSetDrawPosition(frq, frq->x + (((box->width - frGetStrWidth2(frq)) >> 1) << 4), frq->y);
}

u32 nbPanelHPColor(datUnitWork_t *pUnitWork)
{
    s32 rate;

    if (datCheckDying(pUnitWork)) {
        return 0x754B3D80;
    }
    rate = pUnitWork->hp * 100 / pUnitWork->maxhp;
    if (rate < 25) {
        return 0x754B3D80;
    }
    return rate < 50 ? 0xB4A06480 : 0xA09DC380;
}

u32 nbPanelMPColor(datUnitWork_t *pUnitWork)
{
    s32 pct;

    pct = pUnitWork->mp * 100 / pUnitWork->maxmp;
    if (pct < 0x19) {
        return 0x754B3D80;
    }
    return pct < 0x32 ? 0xB4A06480 : 0xA09DC380;
}

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelCursorDraw);

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelPolygonDraw);

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelPolygonScaleDraw);

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelPolygonFlashDraw);

void nbPanelDrawPP0(s32 x, s32 y, nbMainProcessData_t *p, s32 index)
{
    nbPnl_rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    nbPnl_rsetAlpha(0);
    nbPnlSprite(x, y, 0x36, 0x17, 0x34, 1, 0x36, 0x17, nbPanelBaseABGR, p->pnltex[0]);
    nbPnlSprite(x + 0x36, y, 0xE4, 0x17, 0x69, 1, 1, 0x17, nbPanelBaseABGR, p->pnltex[0]);
}

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545300);

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelDrawPP1);

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelDrawPP);

void nbPanelPressGaugeInit(void)
{
    nbPanelPGShow = 1;
    nbPanelPGNumber = 0;
    nbPanelPGFrame = 0;
    nbPanelPGBirthWait = 0;
    nbPanelPGStart = 0;
    nbPanelPGEncount = 0;
    nbPanelPGTurn = -1;
    nbPanelPGTurnOld = -1;
    nbPanelPGBaseColor = 0x80808080;
}

void nbPanelPressGaugeStart(void)
{
    nbPanelPGStart = 1;
    nbPanelPGNumber = 0;
    nbPanelPGFrame = 0;
    nbPanelPGBirthWait = 0;
    nbPanelPGTurn = -1;
    nbPanelPGTurnOld = -1;
}

void nbPanelPressGaugeReset(void)
{
    printf("nbPanelPressGaugeReset\n");
    nbPanelPressGaugeStart();
    nbPanelPressTurnGaugeAnimStart2();
    nbPanelPressTurnOmit();
}

void nbPanelPressGaugeShow(void)
{
    nbMainProcessData_t *data;

    data = nbGetMainProcessData();
    if (!nbPanelPGShow && !nbPanelPGEncount) {
        printf("nbPanelPressGaugeShow : Reset\n");
        nbPanelPGPress = data->stat & 8;
        nbPanelPGFrame = 0;
        nbPanelPGNumber = data->pressturn;
    }
    nbPanelPGEncount = 0;
    nbPanelPGShow = 1;
    nbPanelPGBaseColor = 0x80808080;
}

void nbPanelPressGaugeHide(void)
{
    nbPanelPGShow = 0;
}

void nbPanelPressGaugeEncountHide(void)
{
    nbPanelPGShow = 0;
    nbPanelPGEncount = 1;
}

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelPressGauge);

void nbPanelPressTurnInit(void)
{
    nbPanelPT_1_Process = 0;
    nbPanelPT_2_Process = 0;
    nbPanelPT_3_Process = 0;
    nbPanelPTAProcess = 0;
    nbPanelPTP_1_Process = 0;
    nbPanelPTAOnly = 0;
    nbPanelPTOmit = 0;
}

s32 nbPanelPressTurnRun(void)
{
    s32 run;

    run = 0;
    if (nbPanelPT_1_Process != 0 || nbPanelPT_2_Process != 0 || nbPanelPT_3_Process != 0 ||
        nbPanelPTAProcess != 0 || nbPanelPTP_1_Process != 0) {
        run = 1;
    }
    return run;
}

s32 nbPanelPressTurnGaugeRun(void)
{
    return nbPanelPTAProcess != 0;
}

void nbPanelPressTurnOmit(void)
{
    nbPanelPTOmit = 1;
}

void nbPanelPressTurnStart(void)
{
    printf("nbPanelPressTurnStart\n");
    nbGetMainProcessData();
    if (nbPanelPTOmit == 0) {
        nbPanelPT_1_Process = 1;
        nbPanelPT_1_Frame = 0;
        nbPanelPT_2_Process = 1;
        nbPanelPT_2_Frame = 0;
        nbPanelPT_2_Rate = 0.1f;
        nbPanelPT_3_Process = 1;
        nbPanelPT_3_Frame = 0;
        nbPanelPT_3_Rate = 0.1f;
        nbPanelPTP_0_Frame = 0;
        nbPanelPTP_0_Process = 0;
        nbPanelPTP_1_Frame = 0;
        nbPanelPTP_1_Process = 0;
        if (nbPanelPGTurn != nbPanelPGTurnOld) {
            nbPanelPTAProcess = 1;
            nbPanelPTAFrame = 0;
            nbPanelPTAOnly = 0;
        }
        nbPanelPGTurnOld = nbPanelPGTurn;
    } else {
        nbPanelPTOmit = 0;
    }
}

void nbPanelPressTurnGaugeAnimStart(void)
{
    nbPanelPT_1_Process = 0;
    nbPanelPT_2_Process = 0;
    nbPanelPT_3_Process = 0;
    nbPanelPTAProcess = 1;
    nbPanelPTAOnly = 1;
    nbPanelPTAFrame = 0;
}

void nbPanelPressTurnGaugeAnimStart2(void)
{
    nbPanelPTAFrame = 4;
    nbPanelPTAProcess = 3;
    nbPanelPT_1_Process = 0;
    nbPanelPT_2_Process = 0;
    nbPanelPT_3_Process = 0;
    nbPanelPTARate_S = 0.1f;
    nbPanelPTARate_E = 0.1f;
    nbPanelPTAOnly = 1;
}

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelPressTurn);

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelPartySelect);

void nbPanelStockInit(void)
{
    nbPanelStockShiftX = 300;
    nbPanelStockABGR = 0x80808080;
    nbPanelStockRun = 0;
    nbPanelStockPad = 0;
    nbPanelStockFadeFlag = 0;
    nbPanelStockFrame = 0;
}

s32 nbPanelStockCheckRun(void)
{
    return nbPanelStockRun;
}

void nbPanelStockEnd(void)
{
    nbPanelStockRun = 0;
    nbPanelStockPad = 0;
}

void nbPanelStockFadeOUT(void)
{
    nbPanelStockFadeFlag = 1;
}

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelStockSelectInit);

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelStockSelectCtrl);

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelStockSelect);

void nbPanelAnalyzeInit(void)
{
    nbPanelAnalyzeProcess = 0;
    nbPanelAnalyzeFrame = 0;
    pNbPanelAnalyzeUnitWork = NULL;
}

void nbPanelAnalyzeStart(void)
{
    nbPanelAnalyzeFrame = 0;
    nbPanelAnalyzeProcess = 1;
}

void nbPanelAnalyzeEnd(void)
{
    nbPanelAnalyzeFrame = 0;
    nbPanelAnalyzeProcess = 4;
}

void nbPanelAnalyzeDraw(datUnitWork_t *pUnitWork)
{
    pNbPanelAnalyzeUnitWork = pUnitWork;
}

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelAnalyzeRun);

void nbPanelAutoShow(void)
{
    PanelAutoShow = 1;
    PanelAutoBaseColor = 0x80808080;
}

void nbPanelAutoHide(void)
{
    PanelAutoShow = 0;
}

s32 nbPanelAutoCheckShow(void)
{
    return PanelAutoShow;
}

void nbPanelAutoInit(void)
{
    PanelAutoABGR = 0x40808080;
    PanelAutoBaseColor = 0x80808080;
    PanelAutoShow = 1;
}

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelAuto);

void nbPanelDraw(s32 flag)
{
    if (flag) {
        nbPanelAllShow();
    } else {
        nbPanelAllHide();
    }
    nbPnlDrawFlag = 1;
}

s32 nbPanelRunProcess(dds3ProcessID_t *id)
{
    nbPnlCnt += 0.2f;
    return 0;
}

void nbPanelPartyShow(void)
{
    PanelPartyShow = 1;
}

void nbPanelPartyHide(void)
{
    PanelPartyShow = 0;
}

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelPartyDraw);

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelDrawProcess);

void nbPanelShutdown(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data;

    data = nbGetMainProcessData();
    nbPnlTexDel(data);
    data->pnlproc = NULL;
}

void nbInitPanelProcess(nbMainProcessData_t *data)
{
    dds3ProcessID_t *id;
    s32 i;

    id = dds3StartProcess(D_00545488, 0x3F9, 1, 1, nbPanelRunProcess, nbPanelShutdown, NULL);
    dds3LinkProcess(data->mainproc, id);
    data->pnlproc = id;
    dds3LinkProcess(id, dds3StartProcess(D_00545498, 0x2B0E, 1, 1, nbPanelDrawProcess, NULL,
                                         NULL));
    nbPnlTexSet(data);
    nbPnlDrawFlag = 1;
    nbPnlCnt = 0.0f;
    for (i = 0; i < 4; i++) {
        nbPanelLayoutTable2[i][0] = 0;
        nbPanelLayoutTable2[i][1] = 0;
        nbPanelLayoutTable2[i][2] = -1;
        nbPanelLayoutTable2[i][3] = -1;
        nbPanelLayoutTable2[i][4] = 0;
        nbPanelLayoutTable2[i][5] = 0;
    }
    nbPanelPressTurnInit();
    nbPanelPressGaugeInit();
    nbPanelStockInit();
    nbPanelAnalyzeInit();
    nbPanelAutoInit();
    nbPanelAllShow();
    nbPanelBaseABGR = 0x80808080;
    nbPanelBaseRGBA = 0x80808080;
}

void nbPanelFlashInit(nbPanelFlashData_t *pFlash, s32 scale_x, s32 scale_y, s32 abgr, f32 abgr_c, f32 addX, s32 addY)
{
    pFlash->PT_1_Process = 1;
    pFlash->PT_1_Frame = 0;
    pFlash->PT_2_Process = 1;
    pFlash->PT_2_Frame = 0;
    pFlash->PT_2_Rate = 0.1f;
    pFlash->PT_3_Frame = 0;
    pFlash->PT_3_Process = 1;
    pFlash->PT_3_Rate = 0.1f;
    pFlash->unk_28 = abgr_c;
    pFlash->unk_2C = addX;
    pFlash->unk_20 = scale_x;
    pFlash->unk_24 = scale_y;
    pFlash->abgr = abgr;
    pFlash->addY = addY;
}

/* vu0 routine: unpack two colours, multiply and pack */
u32 nbPanelFlashRGBAMulti(u32 abgr0, u32 abgr1)
{
    nbColorWork_t work[3];
    u32 unit = 0x3C000000;
    u32 out;

    work[0].word = abgr0;
    MMI_RGBA_UNPACK(&work[0], unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    work[1].word = abgr1;
    MMI_RGBA_UNPACK(&work[1], unit);
    SDF_VU0_MULVECTOR();
    SDF_VU0_dds3ColorFtoI(out, 128.0f);
    return out;
}

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545488);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545498);

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelFlashDraw);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_005454F0);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545508);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545518);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545528);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545538);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545548);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545558);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545568);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545578);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545588);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545598);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_005455A8);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_005455B8);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", nbDebugUnitLabelTable);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545658);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545668);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545678);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545688);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545698);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", gnbPnlOtOfs);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPnlDrawFlag);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPnlCnt);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_00551174);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_00551178);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelBaseABGR);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelBaseRGBA);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_00551188);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPGPress);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPGNumber);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPGTurn);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPGTurnOld);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPGFrame);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPGBirthWait);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPGStart);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPGShow);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPGEncount);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPGBaseColor);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPTOmit);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_005511AD);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPT_1_Process);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPT_1_Frame);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPT_2_Process);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPT_2_Frame);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPT_2_Rate);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPT_3_Process);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPT_3_Frame);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPT_3_Rate);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPTP_0_Frame);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPTP_0_Process);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_005511D8);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPTP_1_Frame);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPTP_1_Process);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPTP_1_Rate);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPTAProcess);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPTAFrame);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPTARate_S);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPTARate_E);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelPTAOnly);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_005511F9);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_005511FC);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelStockRun);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelStockPad);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelStockABGR);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelStockFadeFlag);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_00551209);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelStockFrame);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelStockShiftX);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", StockIDListCount);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", StockIDListCursor);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", StockIDListTop);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelAnalyzeProcess);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelAnalyzeFrame);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", nbPanelAnalyzeRate);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", pNbPanelAnalyzeUnitWork);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_00551230);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", PanelAutoABGR);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", PanelAutoBaseColor);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", PanelAutoShow);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", PanelPartyShow);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_00551240);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_00551248);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_00551250);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_00551258);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_00551260);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_00551268);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_00551270);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_00551278);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_00551280);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_00551288);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_00551290);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_00551298);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_005512A0);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_005512A8);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_005512B0);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_005512B8);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_005512C0);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_005512C8);

INCLUDE_SDATA(const s32, "newbattle/nbPanelProcess", D_005512D0);

