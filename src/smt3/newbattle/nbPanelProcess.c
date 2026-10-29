#include "common.h"
#include "newbattle.h"
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
    s8 order[12];
    s8 orderindex;
    u8 orderPadding;
    s16 previndex;
    u16 prevcommand;
    u16 prevcommind;
    u16 prevflag;
    u16 debug;
    u32 stat;
    u8 unk_64[0x4];
    u8 turn;
    s8 phase;
    s8 nextphase;
    u8 esccnt;
    u16 phasetime;
    u16 framePadding;
    u32 battletime;
    f32 press3;
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
    dds3ProcessID_t *MainProcessID;
    u8 unk_C0[0x14];
    dds3ProcessID_t *ActionProcessID[4];
    s32 NowActionProcess;
    u8 unk_E8[0xC];
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

/* 0x38 bytes of flash state: three unsigned phases, RGB colours and pixel offsets. */
typedef struct
{
    s32 PT_1_Frame;
    u16 PT_1_Process;
    u8 pad_06[0x2];
    s32 PT_2_Frame;
    u16 PT_2_Process;
    u8 pad_0E[0x2];
    f32 PT_2_Rate;
    s32 PT_3_Frame;
    u16 PT_3_Process;
    u8 pad_1A[0x2];
    f32 PT_3_Rate;
    u32 color0;
    u32 color1;
    f32 scaleX;
    f32 scaleY;
    s32 x;
    s32 y;
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
    s32 massuruid;
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

extern s32 frDestroyFontRequestAfter(_FRQ *pFRQ);

extern _FRQ *frwGetSmallNumFontFRQ(s32 X, s32 Y, s32 Pri, u32 Color, char **pStr, s32 pFRQ);

extern s32 func_001C7E38(_FRQ *frq, s8 mode, u32 kind);

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
    s32 u;
    s32 v;
    u8 pad08[8];
    s32 x;
    s32 y;
    s32 mask;
    s16 flag;
    u8 pad1E[2];
} nbSpriteCorner_t;

typedef struct {
    s32 r;
    s32 g;
    s32 b;
    s32 a;
    nbSpriteCorner_t corner[2];
} nbSpriteVertex_t;

typedef struct {
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;
} nbPanelProcess_nbPanelTMXuv_t;

extern void nbPnlSprite(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, s32 rgba,
                        sdfTexHandle_t *th);

extern void nbPnl_rsetTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte,
                           s32 ztst);

extern void nbPnl_rsetAlpha(s32 mode);

extern char D_00545488[];

extern char D_00545498[];

extern s32 nbPanelLayoutTable2[4][6];

/* The same 0x18-byte rows as nbPanelLayoutTable2, under the HD member names. */
typedef struct
{
    s32 ActiveMode;
    s32 OfsX;
    s32 OldHp;
    s32 OldMp;
    s32 HpAnmCnt;
    s32 MpAnmCnt;
} nbPnlWork_t;

extern nbPnlWork_t nbPnlWork[4] asm("nbPanelLayoutTable2");

extern s32 nbPanelDrawProcess(void);

extern s32 nbGetActionSeq(dds3ProcessID_t *id);

extern void nbPanelPressTurn(nbMainProcessData_t *data);

extern void nbPanelAnalyzeRun(void);

extern void nbPanelPressGauge(nbMainProcessData_t *data);

extern void nbPanelPartySelect(nbMainProcessData_t *data);

extern void nbPanelPartyDraw(void);

extern u32 dds3GetRandIntA(void *state, u32 limit);

extern void nbPanelDrawPP1(s32 x, s32 y, s32 i, s32 j, s32 jun, s32 act, nbMainProcessData_t *p);

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

extern sdfDrawTag_t *dds3FontReqAllocDrawTag(void);

extern void sdfPrimLinkG4(sdfDrawTag_t *aTag, s32 aPrim, s32 aX0, s32 aY0, u32 aColor0, s32 aX1,
                          s32 aY1, u32 aColor1, s32 aX2, s32 aY2, u32 aColor2, s32 aX3, s32 aY3,
                          u32 aColor3, s32 aZ, void *aAllocator);

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

void nbPnl_rsetAlpha(s32 mode)
{
    sdfDrawTag_t *list;
    u64 *buff;
    u64 *packet;
    sdfGrOt_t *ot;

    list = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    buff = sdfGrAllocPacketBuff(0x30);
    packet = sdf2DMakeStdPacket(buff, 0x30);
    switch (mode) {
    case 1:
        packet[4] = 0x48;
        break;
    case 2:
        packet[4] = 0x42;
        break;
    case 3:
        packet[4] = 0x84;
        break;
    case 4:
        packet[4] = 0x06;
        break;
    case 5:
        packet[4] = 0x89;
        break;
    case 6:
        packet[4] = 0x42;
        break;
    case 10:
        packet[4] = 0x2A;
        break;
    case 20:
        packet[4] = 0x4A;
        break;
    default:
        packet[4] = 0x44;
        break;
    }
    packet[5] = 0x42;
    sdfDrawTagLink(list, buff);
    ot = &sdfGrOtTable[gnbPnlOtOfs];
    ot->regFunc(ot, list);
}

#ifdef NON_MATCHING
extern s32 sdf2DLinkTexPacket(sdfDrawTag_t *aTag, sdfTexHandle_t *aTexHndl, s32 aCtx);
extern sdfGrPacketHeader_t *sdf2DAllocSprite(s32 aNumOfSpr);
extern void *sdf2DGetPrimPtr(sdfGrPacketHeader_t *aPacketPtr);
#endif

void nbPnlSprite(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, s32 rgba,
                 sdfTexHandle_t *th)
{
    void *handle = sdf2DAllocSprite(1);
    nbSpriteVertex_t *vtx = (nbSpriteVertex_t *)sdf2DGetPrimPtr((s32)handle);
    s32 ubase = u * 16;
    s32 xl = x * 16 + 0x7000;
    s32 vbase = v * 16;
    s32 yt = y * 8 + 0x7900;
    sdfDrawTag_t *command;
    sdfGrOt_t *ot;

    vtx->r = rgba & 0xFF;
    vtx->g = (rgba >> 8) & 0xFF;
    vtx->b = (rgba >> 16) & 0xFF;
    vtx->a = (rgba >> 24) & 0xFF;
    vtx->corner[0].u = ubase;
    vtx->corner[0].v = vbase;
    vtx->corner[0].x = xl;
    vtx->corner[0].y = yt;
    vtx->corner[0].mask = -1;
    vtx->corner[0].flag = 0;
    vtx->corner[1].u = ubase + uw * 16;
    vtx->corner[1].v = vbase + vh * 16;
    vtx->corner[1].x = xl + w * 16;
    vtx->corner[1].y = yt + h * 8;
    vtx->corner[1].mask = -1;
    vtx->corner[1].flag = 0;
    command = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(command);
    sdf2DLinkTexPacket(command, th, 0);
    sdfDrawTagLink(command, (u32)handle);
    ot = &sdfGrOtTable[gnbPnlOtOfs];
    ot->regFunc(ot, command);
}

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

extern char *text2[];
extern _FRQ *frGetCNameFRQ(s8 TargetNo, s8 Style, _FRQ *pFRQ, s8 FirstOption);
extern _FRQ *frwGetSmallFontFRQ2(s32 X, s32 Y, s32 Pri, u32 Color, char *pStr, _FRQ *pFRQ);
extern void frSetDrawPosition(_FRQ *pFRQ, s32 X, s32 Y);
extern void frSetFontColor(_FRQ *pFRQ, u32 Color);

void nbPanelSmallNameStringDraw(s32 x, s32 y, nbPanelProcess_nbPanelXYZ_t *pXYZ, u32 abgr, u32 statindex)
{
    _FRQ *frq;
    u32 col;

    if (DDS3_GBWK->akuma[statindex].flag & 4) {
        frq = frGetCNameFRQ(0, 1, NULL, 0);
        frSetDrawPosition(frq, (pXYZ->x + x) << 4, (pXYZ->y + y) << 3);
        col = effMagaCylinderCalcColor(abgr, nbPanelBaseRGBA);
        frSetFontColor(frq, col);
        func_001C7E38(frq, 1, 0x47);
        frDestroyFontRequestAfter(frq);
        return;
    }

    col = effMagaCylinderCalcColor(abgr, nbPanelBaseRGBA);
    frq = frwGetSmallFontFRQ2((pXYZ->x + x) << 4, (pXYZ->y + y) << 3, 0,
                              col, text2[DDS3_GBWK->akuma[statindex].devilId], NULL);
    func_001C7E38(frq, 1, 0x47);
    frDestroyFontRequestAfter(frq);
}

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

#ifdef NON_MATCHING
void nbPanelCursorDraw(s32 x, s32 y)
{
    sdfDrawTag_t *tag;
    sdfGrOt_t *ot;
    s32 x0, x1, x2, y0, y1;
    u32 c0, c1, c2;

    x0 = ((u32)x + 0x15) * 16 + 0x7000;
    x1 = ((u32)x + 0x15) * 16 + 0x7828;
    x2 = ((u32)x + 0x15) * 16 + 0x8050;
    y0 = (u32)y * 8 + 0x7900;
    y1 = (u32)y * 8 + 0x79A0;
    tag = dds3FontReqAllocDrawTag();
    c2 = effMagaCylinderCalcColor(0x468CBE1B, nbPanelBaseABGR);
    c1 = effMagaCylinderCalcColor(0x469B4400, nbPanelBaseABGR);
    c0 = effMagaCylinderCalcColor(0x00BF0013, nbPanelBaseABGR);
    sdfPrimLinkG4(tag, 0x14C, x0, y0, c0, x1, y0, c1,
                  x0, y1, c0, x1, y1, c1, -1, NULL);
    sdfPrimLinkG4(tag, 0x14C, x1, y0, c1, x2, y0, c2,
                  x1, y1, c1, x2, y1, c2, -1, NULL);
    ot = &sdfGrOtTable[71];
    ot->regFunc(ot, tag);
}
#else
INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelCursorDraw);
#endif

void nbPanelPolygonDraw(s32 x, s32 y, nbPanelProcess_nbPanelXYZ_t *pXYZ, u32 abgr0, u32 abgr1,
                        u32 abgr2, u32 abgr3)
{
    sdfDrawTag_t *tag;
    sdfGrOt_t *ot;
    s32 x0, y0, x1, y1, w, h;
    u32 c0, c1, c2, c3;

    tag = dds3FontReqAllocDrawTag();
    x0 = ((pXYZ->x + x) << 4) + 0x7000;
    y0 = ((pXYZ->y + y) << 3) + 0x7900;
    w = pXYZ->w << 4;
    h = pXYZ->h << 3;
    c0 = effMagaCylinderCalcColor(abgr0, nbPanelBaseABGR);
    c1 = effMagaCylinderCalcColor(abgr1, nbPanelBaseABGR);
    c2 = effMagaCylinderCalcColor(abgr2, nbPanelBaseABGR);
    c3 = effMagaCylinderCalcColor(abgr3, nbPanelBaseABGR);
    x1 = x0 + w;
    y1 = y0 + h;
    sdfPrimLinkG4(tag, 0x14C, x0, y0, c0, x1, y0, c1, x0, y1, c2, x1, y1, c3, -1, NULL);
    ot = &sdfGrOtTable[71];
    ot->regFunc(ot, tag);
}

void nbPanelPolygonScaleDraw(s32 x, s32 y, nbPanelProcess_nbPanelXYZ_t *pXYZ, f32 scale_w,
                             f32 scale_h, u32 abgr0, u32 abgr1, u32 abgr2, u32 abgr3)
{
    sdfDrawTag_t *tag;
    sdfGrOt_t *ot;
    s32 x0, y0, x1, y1;
    s32 w, h;
    u32 c0, c1, c2, c3;

    tag = dds3FontReqAllocDrawTag();
    w = pXYZ->w << 4;
    h = pXYZ->h << 3;
    w = w * scale_w;
    h = h * scale_h;
    x0 = ((pXYZ->x + x) << 4) + 0x7000;
    y0 = ((pXYZ->y + y) << 3) + 0x7900;
    c0 = effMagaCylinderCalcColor(abgr0, nbPanelBaseABGR);
    c1 = effMagaCylinderCalcColor(abgr1, nbPanelBaseABGR);
    c2 = effMagaCylinderCalcColor(abgr2, nbPanelBaseABGR);
    c3 = effMagaCylinderCalcColor(abgr3, nbPanelBaseABGR);
    x1 = x0 + w;
    y1 = y0 + h;
    sdfPrimLinkG4(tag, 0x14C, x0, y0, c0, x1, y0, c1, x0, y1, c2, x1, y1, c3, -1, NULL);
    ot = &sdfGrOtTable[71];
    ot->regFunc(ot, tag);
}

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelPolygonFlashDraw);

void nbPanelDrawPP0(s32 x, s32 y, nbMainProcessData_t *p)
{
    nbPnl_rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    nbPnl_rsetAlpha(0);
    nbPnlSprite(x, y, 0x36, 0x17, 0x34, 1, 0x36, 0x17, nbPanelBaseABGR, p->pnltex[0]);
    nbPnlSprite(x + 0x36, y, 0xE4, 0x17, 0x69, 1, 1, 0x17, nbPanelBaseABGR, p->pnltex[0]);
}

extern f32 sdfSin(f32 angle);
extern s32 sprintf(char *dst, const char *format, ...);
extern void cmpSetSkillListRows(s32 rows);
extern char D_00551188[];
extern _FRQ *gnbPnlWork;

/* NON_MATCHING: 798 of 806 instruction words emitted against the native
 * 3224-byte size; the body is structurally complete and the tail aligns at a
 * constant eight-word offset. See docs/nonmatching/newbattle/nbPanelProcess.md. */

#ifdef NON_MATCHING

void nbPanelDrawPP1(s32 x, s32 y, s32 i, s32 j, s32 jun, s32 act, nbMainProcessData_t *p)
{
    char buf[16];
    s32 pcnum[4][2] = {{1, 1}, {0xD, 1}, {0x19, 1}, {0x24, 1}};
    s32 bad[12][2] = {{0x55, 0x38}, {1, 0x22},    {0x16, 0x22}, {0x2B, 0x22},
                      {0x40, 0x22}, {0x55, 0x22}, {0x6A, 0x22}, {0x7F, 0x22},
                      {1, 0x38},    {0x16, 0x38}, {0x2B, 0x38}, {0x40, 0x38}};
    u32 col;
    u32 c;
    s32 a;
    s32 anm;
    s32 v;
    s32 w;
    s32 q;
    s32 yb;
    s32 bs;
    s32 idx;

    nbPnl_rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    nbPnl_rsetAlpha(0);
    x -= nbPnlWork[i].OfsX;
    if (act) {
        v = sdfSin(nbPnlCnt) * 48.0f;
        v += 0xB0;
        col = v + (v << 8) + (v << 16);
        if (nbPnlWork[i].OfsX >= 16) {
            col |= 0x80000000;
            nbPnlSprite(x, y, 0x36, 0x17, 0x2D, 1, 0x36, 0x17,
                        effMagaCylinderCalcColor(col, nbPanelBaseABGR), p->pnltex[3]);
            nbPnlSprite(x + 0x36, y, nbPnlWork[i].OfsX + 0xE4, 0x17, 0x62, 1, 1, 0x17,
                        effMagaCylinderCalcColor(col, nbPanelBaseABGR), p->pnltex[3]);
        } else {
            a = nbPnlWork[i].OfsX * 8;
            col |= a << 24;
            nbPnlSprite(x, y, 0x36, 0x17, 0x2D, 1, 0x36, 0x17,
                        effMagaCylinderCalcColor(col, nbPanelBaseABGR), p->pnltex[3]);
            nbPnlSprite(x + 0x36, y, nbPnlWork[i].OfsX + 0xE4, 0x17, 0x62, 1, 1, 0x17,
                        effMagaCylinderCalcColor(col, nbPanelBaseABGR), p->pnltex[3]);
            col = (col & 0xFFFFFF) | ((0x80 - a) << 24);
            nbPnlSprite(x, y, 0x36, 0x17, 0x34, 1, 0x36, 0x17,
                        effMagaCylinderCalcColor(col, nbPanelBaseABGR), p->pnltex[0]);
            nbPnlSprite(x + 0x36, y, nbPnlWork[i].OfsX + 0xE4, 0x17, 0x69, 1, 1, 0x17,
                        effMagaCylinderCalcColor(col, nbPanelBaseABGR), p->pnltex[0]);
        }
    } else {
        col = 0x808080;
        if (nbPnlWork[i].OfsX == 0) {
            col |= 0x80000000;
            nbPnlSprite(x, y, 0x36, 0x17, 0x34, 1, 0x36, 0x17,
                        effMagaCylinderCalcColor(col, nbPanelBaseABGR), p->pnltex[0]);
            nbPnlSprite(x + 0x36, y, nbPnlWork[i].OfsX + 0xE4, 0x17, 0x69, 1, 1, 0x17,
                        effMagaCylinderCalcColor(col, nbPanelBaseABGR), p->pnltex[0]);
        } else {
            a = nbPnlWork[i].OfsX * 8;
            col |= a << 24;
            nbPnlSprite(x, y, 0x36, 0x17, 0x2D, 1, 0x36, 0x17,
                        effMagaCylinderCalcColor(col, nbPanelBaseABGR), p->pnltex[3]);
            nbPnlSprite(x + 0x36, y, nbPnlWork[i].OfsX + 0xE4, 0x17, 0x62, 1, 1, 0x17,
                        effMagaCylinderCalcColor(col, nbPanelBaseABGR), p->pnltex[3]);
            col = (col & 0xFFFFFF) | ((0x80 - a) << 24);
            nbPnlSprite(x, y, 0x36, 0x17, 0x34, 1, 0x36, 0x17,
                        effMagaCylinderCalcColor(col, nbPanelBaseABGR), p->pnltex[0]);
            nbPnlSprite(x + 0x36, y, nbPnlWork[i].OfsX + 0xE4, 0x17, 0x69, 1, 1, 0x17,
                        effMagaCylinderCalcColor(col, nbPanelBaseABGR), p->pnltex[0]);
        }
    }
    nbPnlSprite(x + 0x15, y + 2, 0x1A, 0x13, 0x6C, 1, 0x19, 0x13, nbPanelBaseABGR, p->pnltex[0]);
    if (jun >= 0 && jun < 4) {
        nbPnlSprite(x + 0x1C, y + 8, 0xB, 0xB, pcnum[jun][0], pcnum[jun][1], 0xB, 0xB,
                    nbPanelBaseABGR, p->pnltex[0]);
    }
    nbPnl_rsetAlpha(0);
    w = 0;
    yb = y + 0xF;
    nbPnlSprite(x + 0xA4, yb, 0x32, 6, 1, 0x14, 0x32, 6, nbPanelBaseABGR, p->pnltex[0]);
    if (DDS3_GBWK->akuma[j].hp != 0) {
        w = 50 * DDS3_GBWK->akuma[j].hp /
            *(u16 *)&DDS3_GBWK->akuma[j].unk0C[0];
        if (w == 0) {
            w = 1;
        }
        nbPnlSprite(x + 0xA4, yb, w, 6, 1, 0xD, w, 6, nbPanelBaseABGR, p->pnltex[0]);
    }
    if (DDS3_GBWK->akuma[j].hp < nbPnlWork[i].OldHp) {
        q = 50 * nbPnlWork[i].OldHp /
            *(u16 *)&DDS3_GBWK->akuma[j].unk0C[0];
        anm = nbPnlWork[i].HpAnmCnt << 28;
        nbPnl_rsetAlpha(1);
        c = effMagaCylinderCalcColor(anm | 0xFFFFFF, nbPanelBaseABGR);
        nbPnlSprite(x + w + 0xA4, yb, q - w, 6, w + 1, 0xD, q - w, 6, c, p->pnltex[0]);
    }
    c = nbPanelHPColor((datUnitWork_t *)&DDS3_GBWK->akuma[j].flag);
    sprintf(buf, D_00551188, DDS3_GBWK->akuma[j].hp);
    c = effMagaCylinderCalcColor(c, nbPanelBaseRGBA);
    gnbPnlWork = frwGetSmallNumFontFRQ((x + 0xB9) << 4, (y + 2) << 3, 0, c, (char **)buf, 0);
    func_001C7E38(gnbPnlWork, 1, 0x47);
    frDestroyFontRequestAfter(gnbPnlWork);
    nbPnl_rsetAlpha(0);
    w = 0;
    nbPnlSprite(x + 0xDA, yb, 0x32, 6, 1, 0x14, 0x32, 6, nbPanelBaseABGR, p->pnltex[0]);
    if (*(u16 *)&DDS3_GBWK->akuma[j].unk0C[2] != 0) {
        w = 50 * *(u16 *)&DDS3_GBWK->akuma[j].unk0C[2] /
            *(u16 *)&DDS3_GBWK->akuma[j].unk0C[4];
        if (w == 0) {
            w = 1;
        }
        nbPnlSprite(x + 0xDA, yb, w, 6, 1, 0x1B, w, 6, nbPanelBaseABGR, p->pnltex[0]);
    }
    if (*(u16 *)&DDS3_GBWK->akuma[j].unk0C[2] < nbPnlWork[i].OldMp) {
        q = 50 * nbPnlWork[i].OldMp /
            *(u16 *)&DDS3_GBWK->akuma[j].unk0C[4];
        anm = nbPnlWork[i].MpAnmCnt << 28;
        nbPnl_rsetAlpha(1);
        c = effMagaCylinderCalcColor(anm | 0xFFFFFF, nbPanelBaseABGR);
        nbPnlSprite(x + w + 0xDA, yb, q - w, 6, w + 1, 0x1B, q - w, 6, c, p->pnltex[0]);
    }
    nbPnl_rsetAlpha(0);
    nbPnlSprite(x + 0xA8, y + 7, 0x10, 7, 0xE4, 1, 0x10, 7, nbPanelBaseABGR, p->pnltex[2]);
    nbPnlSprite(x + 0xDE, y + 7, 0x10, 7, 0xE4, 9, 0x10, 7, nbPanelBaseABGR, p->pnltex[2]);
    c = nbPanelMPColor((datUnitWork_t *)&DDS3_GBWK->akuma[j].flag);
    sprintf(buf, D_00551188, *(u16 *)&DDS3_GBWK->akuma[j].unk0C[2]);
    c = effMagaCylinderCalcColor(c, nbPanelBaseRGBA);
    gnbPnlWork = frwGetSmallNumFontFRQ((x + 0xEF) << 4, (y + 2) << 3, 0, c, (char **)buf, 0);
    func_001C7E38(gnbPnlWork, 1, 0x47);
    frDestroyFontRequestAfter(gnbPnlWork);
    cmpSetSkillListRows(0x13);
    if (DDS3_GBWK->akuma[j].flag & 4) {
        gnbPnlWork = frGetCNameFRQ(0, 1, NULL, 0);
        frSetDrawPosition(gnbPnlWork, (x + 0x2D) << 4, (y + 4) << 3);
        c = effMagaCylinderCalcColor(0xA09DC380, nbPanelBaseRGBA);
        frSetFontColor(gnbPnlWork, c);
        func_001C7E38(gnbPnlWork, 1, 0x47);
        frDestroyFontRequestAfter(gnbPnlWork);
    } else {
        c = effMagaCylinderCalcColor(0xA09DC380, nbPanelBaseRGBA);
        gnbPnlWork = frwGetSmallFontFRQ2((x + 0x2D) << 4, (y + 4) << 3, 0, c,
                                         text2[DDS3_GBWK->akuma[j].devilId],
                                         NULL);
        func_001C7E38(gnbPnlWork, 1, 0x47);
        frDestroyFontRequestAfter(gnbPnlWork);
    }
    cmpSetSkillListRows(-1);
    nbPnl_rsetAlpha(0);
    bs = DDS3_GBWK->akuma[j].badstatus;
    idx = -1;
    if (bs & 0x800) {
        idx = 0;
    }
    if (bs & 0x400) {
        idx = 1;
    }
    if (bs & 0x200) {
        idx = 2;
    }
    if (bs & 0x100) {
        idx = 3;
    }
    if (bs & 0x80) {
        idx = 4;
    }
    if (bs & 0x40) {
        idx = 5;
    }
    if (bs & 0x20) {
        idx = 6;
    }
    if (bs & 0x10) {
        idx = 7;
    }
    if (bs & 8) {
        idx = 8;
    }
    if (bs & 4) {
        idx = 9;
    }
    if (bs & 2) {
        idx = 10;
    }
    if (bs & 1) {
        idx = 11;
    }
    if (idx != -1) {
        nbPnlSprite(x + 8, y, 0x14, 0x15, bad[idx][0], bad[idx][1], 0x14, 0x15, nbPanelBaseABGR,
                    p->pnltex[0]);
    }
}

#else

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545300);

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelDrawPP1);

#endif

/* NON_MATCHING: 671 of 673 words match at the native 2692-byte size. The two
 * differing words read hp and maxhp out of their spill slots. */

#ifdef NON_MATCHING

extern sdfTexHandle_t *D_00345D88[];
extern nbPanelProcess_nbPanelXYZ_t D_004EFE90[];
extern nbPanelProcess_nbPanelXYZ_t D_004F01A0[];
extern nbPanelProcess_nbPanelTMXuv_t nbPanelParamTable[];

void nbPanelDrawPP(s32 x, s32 y, s32 select, u32 statindex, u32 flag)
{
    char buf[112];
    nbMainProcessData_t *p;
    s32 hp;
    s32 maxhp;
    s32 mp;
    s32 maxmp;
    f32 rate;
    u32 col;

    p = nbGetMainProcessData();
    nbPnl_rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    nbPnl_rsetAlpha(0);
    hp = 0;
    maxhp = 0;
    if (!(flag & 8)) {
        if (select == 0) {
            nbPanelProcess_nbPanelXYZ_t *xyz0 = &D_004EFE90[0];
            nbPanelProcess_nbPanelTMXuv_t *uv0 = &nbPanelParamTable[3];
            nbPanelProcess_nbPanelXYZ_t *xyz1;
            nbPanelProcess_nbPanelTMXuv_t *uv1;
            sdfTexHandle_t *hTex0 = p->pnltex[0];
            sdfTexHandle_t *hTex1;

            nbPnlSprite(xyz0->x + x, xyz0->y + y, xyz0->w, xyz0->h, uv0->u0, uv0->v0, uv0->u1,
                        uv0->v1, effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR), hTex0);
            xyz1 = &D_004EFE90[1];
            uv1 = &nbPanelParamTable[4];
            hTex1 = p->pnltex[0];
            nbPnlSprite(xyz1->x + x, xyz1->y + y, xyz1->w, xyz1->h, uv1->u0, uv1->v0, uv1->u1,
                        uv1->v1, effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR), hTex1);
        } else {
            nbPanelProcess_nbPanelXYZ_t *xyz0 = &D_004EFE90[12];
            nbPanelProcess_nbPanelTMXuv_t *uv0 = &nbPanelParamTable[3];
            nbPanelProcess_nbPanelXYZ_t *xyz1;
            nbPanelProcess_nbPanelTMXuv_t *uv1;
            nbPanelProcess_nbPanelXYZ_t *xyz2;
            nbPanelProcess_nbPanelTMXuv_t *uv2;
            nbPanelProcess_nbPanelXYZ_t *xyz3;
            nbPanelProcess_nbPanelTMXuv_t *uv3;
            sdfTexHandle_t *hTex0 = p->pnltex[0];
            sdfTexHandle_t *hTex1;
            sdfTexHandle_t *hTex2;
            sdfTexHandle_t *hTex3;

            nbPnlSprite(xyz0->x + x, xyz0->y + y, xyz0->w, xyz0->h, uv0->u0, uv0->v0, uv0->u1,
                        uv0->v1, effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR), hTex0);
            xyz1 = &D_004EFE90[13];
            uv1 = &nbPanelParamTable[4];
            hTex1 = p->pnltex[0];
            nbPnlSprite(xyz1->x + x, xyz1->y + y, xyz1->w, xyz1->h, uv1->u0, uv1->v0, uv1->u1,
                        uv1->v1, effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR), hTex1);
            col = (s32)(sdfSin((f32)p->battletime * 0.2f) * 48.0f) + 0xB0;
            xyz2 = &D_004EFE90[15];
            uv2 = &nbPanelParamTable[17];
            col = col + ((col << 16) + (col << 8)) + 0x80000000;
            hTex2 = p->pnltex[3];
            nbPnlSprite(xyz2->x + x, xyz2->y + y, xyz2->w, xyz2->h, uv2->u0, uv2->v0, uv2->u1,
                        uv2->v1, effMagaCylinderCalcColor(col, nbPanelBaseABGR), hTex2);
            xyz3 = &D_004EFE90[16];
            uv3 = &nbPanelParamTable[18];
            hTex3 = p->pnltex[3];
            nbPnlSprite(xyz3->x + x, xyz3->y + y, xyz3->w, xyz3->h, uv3->u0, uv3->v0, uv3->u1,
                        uv3->v1, effMagaCylinderCalcColor(col, nbPanelBaseABGR), hTex3);
        }
    }
    if (select != 0) {
        x -= 0x10;
    }
    if (!(flag & 0x10)) {
        nbPanelProcess_nbPanelXYZ_t *xyz = &D_004EFE90[14];
        nbPanelProcess_nbPanelTMXuv_t *uv = &nbPanelParamTable[16];
        sdfTexHandle_t *hTex = p->pnltex[0];

        nbPnlSprite(xyz->x + x, xyz->y + y, xyz->w, xyz->h, uv->u0, uv->v0, uv->u1, uv->v1,
                    effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR), hTex);
    }
    if (!(flag & 2)) {
        nbPanelProcess_nbPanelXYZ_t *xyz0 = &D_004EFE90[6];
        nbPanelProcess_nbPanelTMXuv_t *uv0 = &nbPanelParamTable[5];
        nbPanelProcess_nbPanelXYZ_t *xyz1 = &D_004EFE90[5];
        nbPanelProcess_nbPanelTMXuv_t *uv1 = &nbPanelParamTable[2];
        sdfTexHandle_t *hTex0 = p->pnltex[2];
        sdfTexHandle_t *hTex1;
        fldAnahoriAkuma_t *ak;
        s32 ofs;
        u32 col;

        nbPnlSprite(xyz0->x + x, xyz0->y + y, xyz0->w, xyz0->h, uv0->u0, uv0->v0, uv0->u1,
                    uv0->v1, effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR), hTex0);
        hTex1 = p->pnltex[0];
        nbPnlSprite(xyz1->x + x, xyz1->y + y, xyz1->w, xyz1->h, uv1->u0, uv1->v0, uv1->u1,
                    uv1->v1, effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR), hTex1);
        ofs = statindex * sizeof(fldAnahoriAkuma_t) + 0xA50;
        ak = (fldAnahoriAkuma_t *)((u8 *)DDS3_GBWK + ofs);
        hp = ak->hp;
        maxhp = *(u16 *)&ak->unk0C[0];
        if (hp != 0 && maxhp != 0) {
            nbPanelProcess_nbPanelXYZ_t *xyz2 = &D_004EFE90[4];
            nbPanelProcess_nbPanelTMXuv_t *uv2 = &nbPanelParamTable[0];
            sdfTexHandle_t *hTex2;

            rate = (f32)hp;
            rate = rate / (f32)maxhp;
            hTex2 = p->pnltex[0];
            nbPnlSprite(xyz2->x + x, xyz2->y + y, xyz2->w * rate, xyz2->h, uv2->u0, uv2->v0,
                        uv2->u1 * rate, uv2->v1,
                        effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR), hTex2);
        }
        sprintf(buf, D_00551188, hp);
        col = nbPanelHPColor((datUnitWork_t *)&DDS3_GBWK->akuma[statindex].flag);
        nbPanelSmallStringDraw(x, y, &D_004EFE90[3], col, (char **)buf);
    }
    if (!(flag & 4)) {
        nbPanelProcess_nbPanelXYZ_t *xyz0 = &D_004EFE90[10];
        nbPanelProcess_nbPanelTMXuv_t *uv0 = &nbPanelParamTable[6];
        nbPanelProcess_nbPanelXYZ_t *xyz1 = &D_004EFE90[9];
        nbPanelProcess_nbPanelTMXuv_t *uv1 = &nbPanelParamTable[2];
        sdfTexHandle_t *hTex0 = p->pnltex[2];
        sdfTexHandle_t *hTex1;
        fldAnahoriAkuma_t *ak;
        s32 ofs;
        u32 col;

        nbPnlSprite(xyz0->x + x, xyz0->y + y, xyz0->w, xyz0->h, uv0->u0, uv0->v0, uv0->u1,
                    uv0->v1, effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR), hTex0);
        hTex1 = p->pnltex[0];
        nbPnlSprite(xyz1->x + x, xyz1->y + y, xyz1->w, xyz1->h, uv1->u0, uv1->v0, uv1->u1,
                    uv1->v1, effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR), hTex1);
        ofs = statindex * sizeof(fldAnahoriAkuma_t) + 0xA50;
        ak = (fldAnahoriAkuma_t *)((u8 *)DDS3_GBWK + ofs);
        mp = *(u16 *)&ak->unk0C[2];
        maxmp = *(u16 *)&ak->unk0C[4];
        if (mp != 0 && maxmp != 0) {
            rate = (f32)mp / (f32)maxmp;
            if (hp == 0 || maxhp == 0) {
                nbPanelProcess_nbPanelXYZ_t *xyz2 = &D_004EFE90[8];
                nbPanelProcess_nbPanelTMXuv_t *uv2 = &nbPanelParamTable[56];
                sdfTexHandle_t *hTex2 = D_00345D88[0];

                nbPnlSprite(xyz2->x + x, xyz2->y + y, xyz2->w * rate, xyz2->h, uv2->u0, uv2->v0,
                            uv2->u1 * rate, uv2->v1,
                            effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR), hTex2);
            } else {
                nbPanelProcess_nbPanelXYZ_t *xyz2 = &D_004EFE90[8];
                nbPanelProcess_nbPanelTMXuv_t *uv2 = &nbPanelParamTable[1];
                sdfTexHandle_t *hTex2 = p->pnltex[0];

                nbPnlSprite(xyz2->x + x, xyz2->y + y, xyz2->w * rate, xyz2->h, uv2->u0, uv2->v0,
                            uv2->u1 * rate, uv2->v1,
                            effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR), hTex2);
            }
        }
        sprintf(buf, D_00551188, mp);
        col = nbPanelMPColor((datUnitWork_t *)&DDS3_GBWK->akuma[statindex].flag);
        nbPanelSmallStringDraw(x, y, &D_004EFE90[7], col, (char **)buf);
    }
    if (flag & 0x200000) {
        nbPanelProcess_nbPanelXYZ_t *xyz;
        nbPanelProcess_nbPanelTMXuv_t *uv;
        sdfTexHandle_t *hTex;
        u32 c;

        col = (s32)(sdfSin((f32)p->battletime * 0.2f) * 96.0f) + 0xA0;
        xyz = &D_004F01A0[0];
        uv = &nbPanelParamTable[14];
        c = col + ((col << 16) + (col << 8)) + 0x80000000;
        hTex = p->pnltex[0];
        nbPnlSprite(xyz->x + x, xyz->y + y, xyz->w, xyz->h, uv->u0, uv->v0, uv->u1, uv->v1,
                    effMagaCylinderCalcColor(c, nbPanelBaseABGR), hTex);
    }
    if (!(flag & 1)) {
        if (flag & 0x100000) {
            nbPanelSmallNameStringDraw(x, y, &D_004EFE90[2], 0x89FEFF80, statindex);
        } else if (flag & 0x200000) {
            nbPanelSmallNameStringDraw(x, y, &D_004EFE90[2], 0x89FEFF80, statindex);
        } else {
            nbPanelSmallNameStringDraw(x, y, &D_004EFE90[2], 0xA09DC380, statindex);
        }
    }
    if (!(flag & 0x20)) {
        s32 bs = DDS3_GBWK->akuma[statindex].badstatus;
        s32 idx;

        if (bs & 0x800) {
            idx = 0xD;
        } else if (bs & 0x400) {
            idx = 0x13;
        } else if (bs & 0x200) {
            idx = 0x14;
        } else if (bs & 0x100) {
            idx = 0x15;
        } else if (bs & 0x80) {
            idx = 0x16;
        } else if (bs & 0x40) {
            idx = 0x17;
        } else if (bs & 0x10) {
            idx = 0x19;
        } else if (bs & 4) {
            idx = 0x1B;
        } else if (bs & 2) {
            idx = 0x1C;
        } else if (bs & 1) {
            idx = 0x1D;
        } else if (bs & 0x20) {
            idx = 0x18;
        } else if (bs & 8) {
            idx = 0x1A;
        } else {
            idx = -1;
        }
        if (idx != -1) {
            nbPanelProcess_nbPanelXYZ_t *xyz = &D_004EFE90[17];
            nbPanelProcess_nbPanelTMXuv_t *uv = &nbPanelParamTable[idx];
            sdfTexHandle_t *hTex = p->pnltex[0];

            nbPnlSprite(xyz->x + x, xyz->y + y, xyz->w, xyz->h, uv->u0, uv->v0, uv->u1, uv->v1,
                        effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR), hTex);
        }
    }
}

#else

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelDrawPP);

#endif

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

extern nbPanelProcess_nbPanelXYZ_t nbStockSelectTable[];
extern nbPanelProcess_nbPanelXYZ_t D_004F01A0[];
extern nbPanelProcess_nbPanelXYZ_t D_004EFE90[];
extern nbPanelProcess_nbPanelTMXuv_t nbPanelParamTable[];
extern s32 D_005511FC;
extern void nbPanelDrawPP(s32 x, s32 y, s32 select, u32 statindex, u32 flag);

#ifdef NON_MATCHING
void nbPanelPartySelect(nbMainProcessData_t *data)
{
    nbPanelProcess_nbPanelXYZ_t *rect;
    nbPanelProcess_nbPanelTMXuv_t *uv;
    nbFormation_t *form;
    nbParty_t *party;
    sdfTexHandle_t *texture;
    s32 active;
    s32 selected;
    s32 statindex;
    s32 x;
    s32 y = 0x153;
    s32 i;
    u32 color;
    u32 base;
    u32 stat;

    uv = &nbPanelParamTable[7];
    for (i = 0; i < 2; i++) {
        rect = &nbStockSelectTable[6 + i];
        texture = data->pnltex[1];
        color = effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR);
        nbPnlSprite(rect->x + 0xE6, rect->y + y, rect->w, rect->h,
                    uv->u0, uv->v0, uv->u1, uv->v1, color, texture);
    }
    party = data->party;
    form = data->form;
    active = party[data->order[data->orderindex]].statindex;
    for (i = 0; i < 4; i++, form++, y += 0x17) {
        stat = form->stat;
        base = *(volatile u32 *)&nbPanelBaseABGR;
        if ((stat & 0x101) == 0x101) {
            statindex = party[form->partyindex].statindex;
            selected = active == statindex;
            if (stat & 8) {
                nbPanelDrawPP(0xE6, y, selected, statindex, 0x200000);
                x = selected ? 214 : (s32)(f32)D_005511FC;
                texture = data->pnltex[0];
                color = effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR);
                rect = &D_004F01A0[1];
                uv = &nbPanelParamTable[15];
                nbPnlSprite(rect->x + x, rect->y + y, rect->w, rect->h,
                            uv->u0, uv->v0, uv->u1, uv->v1, color, texture);
            } else {
                nbPanelDrawPP(0xE6, y, selected, statindex, 0);
            }
        } else {
            rect = &D_004EFE90[0];
            uv = &nbPanelParamTable[3];
            texture = data->pnltex[0];
            color = effMagaCylinderCalcColor(0x80808080, base);
            nbPnlSprite(rect->x + 0xE6, rect->y + y, rect->w, rect->h,
                        uv->u0, uv->v0, uv->u1, uv->v1, color, texture);
            rect = &D_004EFE90[1];
            uv = &nbPanelParamTable[4];
            texture = data->pnltex[0];
            color = effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR);
            nbPnlSprite(rect->x + 0xE6, rect->y + y, rect->w, rect->h,
                        uv->u0, uv->v0, uv->u1, uv->v1, color, texture);
        }
    }
}
#else
INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelPartySelect);
#endif

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

extern s32 StockIDListCount;
extern s32 StockIDListCursor;
extern s32 StockIDListTop;
extern u32 StockIDList[];

#ifdef NON_MATCHING
void nbPanelStockSelectInit(void)
{
    volatile dds3GlobalWork_t *global;
    volatile fldAnahoriAkuma_t *unit;
    u32 i = 0;
    u32 index;
    u32 flags;
    u32 stockCount;
    s32 count;

    nbGetMainProcessData();
    global = DDS3_GBWK;
    nbPanelStockShiftX = 300;
    stockCount = global->stockCount;
    StockIDListCursor = 0;
    StockIDListTop = 0;
    nbPanelStockFrame = 0;
    StockIDListCount = 0;
    if (stockCount != 0) {
        count = 0;
        do {
            index = global->stockOrder[i++];
            unit = &global->akuma[index];
            flags = unit->flag;
            if ((flags & 1) && !(flags & 4) && !(flags & 2) &&
                (unit->hp == 0 || unit->badstatus == 0x800)) {
                StockIDList[count++] = index;
            }
        } while (i < global->stockCount);
        StockIDListCount = count;
    }
    nbPanelStockPad = 1;
    nbPanelStockABGR = 0x80808080;
    nbPanelStockFadeFlag = 0;
    nbPanelStockRun = 1;
}
#else
INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelStockSelectInit);
#endif

extern s8 dds3PadButtonState[];
extern s32 StockIDListCount;
extern s32 StockIDListCursor;
extern s32 StockIDListTop;
extern u32 StockIDList[];

s32 nbPanelStockSelectCtrl(void)
{
    if (nbPanelStockFrame < 8) {
        return -1;
    }

    if (((u8)dds3PadButtonState[0x26] & 2) != 0) {
        if (StockIDListTop > 0) {
            if (StockIDListCursor == 1) {
                StockIDListTop = StockIDListTop - 1;
            } else {
                StockIDListCursor = StockIDListCursor - 1;
            }
        } else if (StockIDListCursor > 0) {
            StockIDListCursor = StockIDListCursor - 1;
        } else if (dds3PadButtonState[0x26] < 0 && StockIDListCursor == 0) {
            if (StockIDListCount >= 4) {
                StockIDListTop = StockIDListCount - 4;
                StockIDListCursor = 3;
            } else {
                StockIDListTop = 0;
                StockIDListCursor = StockIDListCount - 1;
            }
        }
    } else if (((u8)dds3PadButtonState[0x27] & 2) != 0) {
        if (StockIDListTop < StockIDListCount - 4) {
            if (StockIDListCursor == 2) {
                StockIDListTop = StockIDListTop + 1;
            } else {
                StockIDListCursor = StockIDListCursor + 1;
            }
        } else if (StockIDListCursor < StockIDListCount - 1 && StockIDListCursor < 3) {
            StockIDListCursor = StockIDListCursor + 1;
        } else if (dds3PadButtonState[0x27] < 0) {
            StockIDListTop = 0;
            StockIDListCursor = 0;
        }
    } else if (dds3PadButtonState[0x23] < 0) {
        nbPanelStockPad = 0;
        return -2;
    } else if (dds3PadButtonState[0x21] < 0) {
        nbPanelStockPad = 0;
        return StockIDList[StockIDListTop + StockIDListCursor];
    }
    return -1;
}

extern void nbPanelCursorDraw(s32 x, s32 y);

extern void nbPanelDrawPP(s32 x, s32 y, s32 select, u32 statindex, u32 flag);

extern nbPanelProcess_nbPanelXYZ_t nbStockSelectTable[];

extern nbPanelProcess_nbPanelXYZ_t D_004EFE90[];

extern nbPanelProcess_nbPanelXYZ_t D_004EFF6C[];

extern nbPanelProcess_nbPanelTMXuv_t D_004EFB00[];

u32 nbPanelStockSelect(nbMainProcessData_t *p, u32 pad)
{
    s32 ret;
    s32 cx;
    s32 cy;
    s32 i;
    u32 *pid;
    s32 n;
    u32 blink;
    f32 rate;
    u32 col;

    if (pad != 0 && nbPanelStockPad != 0) {
        ret = nbPanelStockSelectCtrl();
    } else {
        ret = -1;
    }
    cx = nbPanelStockShiftX + 0xE6;
    if (nbPanelStockFrame > 0) {
        s32 frame;
        u32 col;

        frame = nbPanelStockFrame - 1;
        if (frame < 5) {
            rate = (f32)frame / 5.0f;
        } else {
            rate = 1.0f;
        }
        col = ((u32)(rate * 128.0f) << 24) | 0x808080;
        nbPanelBaseColorSet(col);
        nbPanelStockShiftX = nbPanelStockShiftX >> 1;
        if (nbPanelStockFadeFlag != 0) {
            if ((nbPanelStockABGR & 0xFF000000) != 0) {
                nbPanelStockABGR = nbPanelStockABGR + 0xE0000000;
            }
            effMagaCylinderCalcColor(col, nbPanelStockABGR);
            nbPanelBaseColorSet(nbPanelStockABGR);
        }
    }
    {
        nbPanelProcess_nbPanelXYZ_t *xyz0;
        nbPanelProcess_nbPanelXYZ_t *xyz1;
        nbPanelProcess_nbPanelTMXuv_t *uv0;
        nbPanelProcess_nbPanelTMXuv_t *uv1;

        xyz0 = &D_004EFE90[0];
        uv0 = &D_004EFB00[0];
        cy = 0x153;
        xyz1 = &D_004EFE90[1];
        uv1 = &D_004EFB00[1];
        for (i = 3; i >= 0; i--) {
            void *hTex0 = p->pnltex[0];
            void *hTex1;

            nbPnlSprite(xyz0->x + cx, xyz0->y + cy, xyz0->w, xyz0->h, uv0->u0, uv0->v0, uv0->u1,
                        uv0->v1, effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR), hTex0);
            hTex1 = p->pnltex[0];
            nbPnlSprite(xyz1->x + cx, xyz1->y + cy, xyz1->w, xyz1->h, uv1->u0, uv1->v0, uv1->u1,
                        uv1->v1, effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR), hTex1);
            cy += 0x17;
        }
    }
    cy = 0x153;
    {
        nbPanelProcess_nbPanelXYZ_t *xyz = &nbStockSelectTable[0];
        nbPanelProcess_nbPanelTMXuv_t *uv = &D_004EFB00[6];
        void *hTex = p->pnltex[1];

        nbPnlSprite(xyz->x + cx, xyz->y + cy, xyz->w, xyz->h, uv->u0, uv->v0, uv->u1,
                    uv->v1, effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR),
                    hTex);
    }
    {
        nbPanelProcess_nbPanelXYZ_t *xyz = &nbStockSelectTable[1];
        nbPanelProcess_nbPanelTMXuv_t *uv = &D_004EFB00[7];
        void *hTex = p->pnltex[1];

        nbPnlSprite(xyz->x + cx, xyz->y + cy, xyz->w, xyz->h, uv->u0, uv->v0, uv->u1,
                    uv->v1, effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR),
                    hTex);
    }
    {
        nbPanelProcess_nbPanelXYZ_t *xyz = &nbStockSelectTable[6];
        nbPanelProcess_nbPanelTMXuv_t *uv = &D_004EFB00[4];
        void *hTex = p->pnltex[1];

        nbPnlSprite(xyz->x + cx, xyz->y + cy, xyz->w, xyz->h, uv->u0, uv->v0, uv->u1,
                    uv->v1, effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR),
                    hTex);
    }
    {
        nbPanelProcess_nbPanelXYZ_t *xyz = &nbStockSelectTable[7];
        nbPanelProcess_nbPanelTMXuv_t *uv = &D_004EFB00[4];
        void *hTex = p->pnltex[1];

        nbPnlSprite(xyz->x + cx, xyz->y + cy, xyz->w, xyz->h, uv->u0, uv->v0, uv->u1,
                    uv->v1, effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR),
                    hTex);
    }
    {
        nbPanelProcess_nbPanelXYZ_t *xyz = &nbStockSelectTable[2];
        nbPanelProcess_nbPanelTMXuv_t *uv = &D_004EFB00[5];
        void *hTex = p->pnltex[0];

        nbPnlSprite(xyz->x + cx, xyz->y + cy, xyz->w, xyz->h, uv->u0, uv->v0, uv->u1,
                    uv->v1, effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR),
                    hTex);
    }
    {
        nbPanelProcess_nbPanelXYZ_t *xyz = &nbStockSelectTable[3];
        nbPanelProcess_nbPanelTMXuv_t *uv = &D_004EFB00[5];
        void *hTex = p->pnltex[0];

        nbPnlSprite(xyz->x + cx, xyz->y + cy, xyz->w, xyz->h, uv->u0, uv->v0, uv->u1,
                    uv->v1, effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR),
                    hTex);
    }
    blink = p->battletime % 25 < 14;
    if (blink == 0 || StockIDListTop == 0) {
        {
            nbPanelProcess_nbPanelXYZ_t *xyz = &nbStockSelectTable[4];
            nbPanelProcess_nbPanelTMXuv_t *uv = &D_004EFB00[8];
            void *hTex = p->pnltex[0];

            nbPnlSprite(xyz->x + cx, xyz->y + 0x153, xyz->w, xyz->h, uv->u0, uv->v0, uv->u1,
                        uv->v1, effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR),
                        hTex);
        }
    } else {
        {
            nbPanelProcess_nbPanelXYZ_t *xyz = &nbStockSelectTable[4];
            nbPanelProcess_nbPanelTMXuv_t *uv = &D_004EFB00[9];
            void *hTex = p->pnltex[0];

            nbPnlSprite(xyz->x + cx, xyz->y + 0x153, xyz->w, xyz->h, uv->u0, uv->v0, uv->u1,
                        uv->v1, effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR),
                        hTex);
        }
    }
    if (blink == 0 || StockIDListTop >= StockIDListCount - 4) {
        {
            nbPanelProcess_nbPanelXYZ_t *xyz = &nbStockSelectTable[5];
            nbPanelProcess_nbPanelTMXuv_t *uv = &D_004EFB00[8];
            void *hTex = p->pnltex[0];

            nbPnlSprite(xyz->x + cx, xyz->y + 0x153, xyz->w, xyz->h, uv->u0, uv->v0, uv->u1,
                        uv->v1, effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR),
                        hTex);
        }
    } else {
        {
            nbPanelProcess_nbPanelXYZ_t *xyz = &nbStockSelectTable[5];
            nbPanelProcess_nbPanelTMXuv_t *uv = &D_004EFB00[9];
            void *hTex = p->pnltex[0];

            nbPnlSprite(xyz->x + cx, xyz->y + 0x153, xyz->w, xyz->h, uv->u0, uv->v0, uv->u1,
                        uv->v1, effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR),
                        hTex);
        }
    }
    nbPanelCursorDraw(0xE6, StockIDListCursor * 23 + 0x153);
    n = StockIDListTop;
    if (StockIDListCount > 0) {
        nbPanelProcess_nbPanelXYZ_t *xyz = &D_004EFF6C[0];
        nbPanelProcess_nbPanelTMXuv_t *uv = &D_004EFB00[10];

        u32 ppSelect = 0x200000;

        pid = &StockIDList[n];
        i = 0;
        while (1) {
            if (i == StockIDListCursor) {
                nbPanelDrawPP(cx, cy, 0, *pid, ppSelect | 0x38);
            } else {
                nbPanelDrawPP(cx, cy, 0, *pid, 0x38);
            }
            if (DDS3_GBWK->akuma[*pid].badstatus & 0x800) {
                {
                    void *hTex = p->pnltex[0];

                    nbPnlSprite(xyz->x + cx, xyz->y + cy, xyz->w, xyz->h, uv->u0, uv->v0, uv->u1,
                                uv->v1, effMagaCylinderCalcColor(0x80808080, nbPanelBaseABGR),
                                hTex);
                }
            }
            pid++;
            n++;
            if (n >= StockIDListCount) {
                break;
            }
            i++;
            if (i >= 4) {
                break;
            }
            cy += 0x17;
        }
    }
    {
        if (nbPanelStockFrame < 5) {
            rate = 1.0f - (f32)nbPanelStockFrame / 5.0f;
        } else {
            rate = 0.0f;
        }
        nbPanelBaseColorSet(((u32)(rate * 128.0f) << 24) | 0x808080);
    }
    nbPanelPartyDraw();
    nbPanelBaseColorSet(0x80808080);
    nbPanelStockFrame = nbPanelStockFrame + 1;
    return ret;
}

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

/* NON_MATCHING: 226 of 232 words match at the native 928-byte size.
 * Six background-sprite argument loads remain reordered; see
 * docs/nonmatching/newbattle/nbPanelProcess.md. */
#ifdef NON_MATCHING
extern f32 sdfSin(f32 angle);
extern nbPanelProcess_nbPanelXYZ_t D_004F05D8[];
extern nbPanelProcess_nbPanelXYZ_t D_004F0600[];
extern nbPanelProcess_nbPanelTMXuv_t D_004EFE60[];
extern void nbPanelPolygonDraw(s32 x, s32 y, nbPanelProcess_nbPanelXYZ_t *geometry,
                               u32 color0, u32 color1, u32 color2, u32 color3);

void nbPanelAuto(nbMainProcessData_t *p)
{
    u32 color;
    u32 baseColor;

    if (p->phase < 3) {
        PanelAutoABGR = 0x40808080;
    } else if (PanelAutoABGR != 0x80808080) {
        PanelAutoABGR += 0x04000000;
    }

    baseColor = PanelAutoBaseColor;
    if (PanelAutoShow == 0) {
        if ((baseColor & 0xFF000000) == 0) {
            return;
        }
        baseColor -= 0x10000000;
        PanelAutoBaseColor = baseColor;
    } else if ((baseColor & 0xFF000000) != 0x80000000) {
        baseColor += 0x10000000;
        PanelAutoBaseColor = baseColor;
    }
    nbPanelBaseColorSet(baseColor);
    nbPnl_rsetAlpha(0);

    color = PanelAutoABGR;
    if ((p->stat & 0x40) != 0) {
        {
            nbPanelProcess_nbPanelXYZ_t *geometry = &D_004F05D8[1];
            nbPanelProcess_nbPanelTMXuv_t *uv = &D_004EFE60[2];
            sdfTexHandle_t *texture = p->pnltex[1];
            u32 drawColor = effMagaCylinderCalcColor(color, nbPanelBaseABGR);
            nbPnlSprite(geometry->x + 0x1C1, geometry->y + 0x12,
                        geometry->w, geometry->h,
                        uv->u0, uv->v0, uv->u1, uv->v1, drawColor, texture);
        }
        {
            f32 angle = (f32)p->battletime * 0.2f;
            nbPanelProcess_nbPanelXYZ_t *geometry = &D_004F0600[1];
            s32 pulse = (s32)(sdfSin(angle) * 64.0f) + 0x50;
            u32 pulseColor = effMagaCylinderCalcColor(pulse, color >> 24);
            pulseColor = (pulseColor << 24) | 0x00483902;
            nbPanelPolygonDraw(0x1C1, 0x12, geometry,
                               0x00483902, pulseColor, 0x00483902, pulseColor);
            nbPanelPolygonDraw(0x1C1, 0x12, geometry - 1,
                               0x00483902, 0x60483902, 0x00483902, 0x60483902);
        }
        {
            f32 angle = (f32)p->battletime * 0.2f;
            nbPanelProcess_nbPanelXYZ_t *geometry = &D_004F05D8[0];
            nbPanelProcess_nbPanelTMXuv_t *uv = &D_004EFE60[1];
            s32 pulse = (s32)(sdfSin(angle) * 48.0f) + 0xA0;
            u32 pulseColor = (u32)pulse + (color & 0xFF000000);
            sdfTexHandle_t *texture;
            u32 drawColor;
            pulseColor += ((u32)pulse << 16) + ((u32)pulse << 8);
            texture = p->pnltex[2];
            drawColor = effMagaCylinderCalcColor(pulseColor, nbPanelBaseABGR);
            nbPnlSprite(geometry->x + 0x1C1, geometry->y + 0x12,
                        geometry->w, geometry->h,
                        uv->u0, uv->v0, uv->u1, uv->v1, drawColor, texture);
        }
    } else {
        nbPanelProcess_nbPanelXYZ_t *geometry = &D_004F05D8[1];
        nbPanelProcess_nbPanelTMXuv_t *uv = &D_004EFE60[2];
        nbPanelProcess_nbPanelXYZ_t *lower;
        nbPanelProcess_nbPanelTMXuv_t *lowerUV;
        {
            sdfTexHandle_t *texture = p->pnltex[1];
            u32 drawColor = effMagaCylinderCalcColor(color, nbPanelBaseABGR);
            lower = geometry - 1;
            lowerUV = uv - 2;
            nbPnlSprite(geometry->x + 0x1C1, geometry->y + 0x12,
                        geometry->w, geometry->h,
                        uv->u0, uv->v0, uv->u1, uv->v1, drawColor, texture);
        }
        {
            sdfTexHandle_t *texture = p->pnltex[2];
            u32 drawColor = effMagaCylinderCalcColor(color, nbPanelBaseABGR);
            nbPnlSprite(lower->x + 0x1C1, lower->y + 0x12,
                        lower->w, lower->h,
                        lowerUV->u0, lowerUV->v0, lowerUV->u1, lowerUV->v1,
                        drawColor, texture);
        }
    }
    nbPanelBaseColorSet(0x80808080);
}
#else
INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelAuto);
#endif

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

/* NON_MATCHING: 332 of 339 words match at the native 1356-byte size. The
 * branch form at +0x3FC needs nbPanelDrawPP1 as a visible C definition. */

#ifdef NON_MATCHING

void nbPanelPartyDraw(void)
{
    nbMainProcessData_t *data;
    nbFormation_t *form;
    s32 partyindex[4];
    s32 statindex[4];
    s32 type[4];
    s32 jun[4];
    s32 i, j;
    s32 oi, n;
    s32 x, yb, y, yy;
    s32 si;
    s32 ofs;

    if (PanelPartyShow == 0) {
        return;
    }
    data = nbGetMainProcessData();
    if (data == NULL) {
        return;
    }
    form = data->form;
    for (i = 0; i < 4; i++) {
        partyindex[i] = -1;
        statindex[i] = -1;
        jun[i] = -1;
        type[i] = -1;
        if ((form->stat & 0x41) == 0x41) {
            s32 pi = form->partyindex;
            s32 ty;

            partyindex[i] = pi;
            statindex[i] = data->party[pi].statindex;
            ty = form->type;
            if (ty == 1) {
                type[i] = ty;
            }
        }
        form++;
    }
    if ((data->stat & 8) == 0) {
        oi = data->orderindex;
        n = 0;
        if (oi != -1) {
        for (i = 0; i < 12; i++) {
            if (data->order[oi] != -1) {
                for (j = 0; j < 4; j++) {
                    if (data->order[oi] == partyindex[j]) {
                        jun[j] = n;
                        n++;
                    }
                }
            }
            oi++;
            if (oi >= 12) {
                oi = 0;
            }
        }
        }
    }
    y = 0;
    yb = 0x153;
    for (i = 0; i < 4; i++) {
        if (type[i] == -1) {
            x = 230;
            yy = y + 0x153;
        } else {
            x = dds3GetRandIntA(NULL, 6) + 227;
            yy = yb + dds3GetRandIntA(NULL, 6) - 3;
        }
        if (partyindex[i] != -1) {
            if (data->order[data->orderindex] == partyindex[i] && jun[i] != -1) {
                nbPnlWork[i].ActiveMode = 1;
                ofs = (16 - nbPnlWork[i].OfsX) / 3;
                if (ofs < 1) {
                    ofs = 1;
                }
                nbPnlWork[i].OfsX += ofs;
                if (nbPnlWork[i].OfsX >= 17) {
                    nbPnlWork[i].OfsX = 16;
                }
            } else {
                nbPnlWork[i].ActiveMode = 0;
                ofs = nbPnlWork[i].OfsX / 3;
                if (ofs < 1) {
                    ofs = 1;
                }
                nbPnlWork[i].OfsX -= ofs;
                if (nbPnlWork[i].OfsX < 0) {
                    nbPnlWork[i].OfsX = 0;
                }
            }
            si = statindex[i];
            if (nbPnlWork[i].OldHp == -1) {
                nbPnlWork[i].OldHp =
                    ((datUnitWork_t *)&DDS3_GBWK->akuma[si].flag)->hp;
            }
            if (type[i] == -1) {
                if (nbPnlWork[i].HpAnmCnt > 0) {
                    nbPnlWork[i].HpAnmCnt--;
                } else {
                    nbPnlWork[i].OldHp =
                        ((datUnitWork_t *)&DDS3_GBWK->akuma[si].flag)->hp;
                }
            } else {
                nbPnlWork[i].HpAnmCnt = 8;
            }
            if (nbPnlWork[i].OldMp == -1) {
                nbPnlWork[i].OldMp =
                    ((datUnitWork_t *)&DDS3_GBWK->akuma[si].flag)->mp;
            }
            if (nbPnlWork[i].MpAnmCnt > 0) {
                nbPnlWork[i].MpAnmCnt--;
                if (nbPnlWork[i].MpAnmCnt <= 0) {
                    nbPnlWork[i].OldMp =
                        ((datUnitWork_t *)&DDS3_GBWK->akuma[si].flag)->mp;
                }
            } else {
                if (((datUnitWork_t *)&DDS3_GBWK->akuma[si].flag)->mp <
                    nbPnlWork[i].OldMp) {
                    nbPnlWork[i].MpAnmCnt = 8;
                }
            }
            nbPanelDrawPP1(x, yy, i, statindex[i], jun[i], nbPnlWork[i].ActiveMode, data);
        } else {
            nbPanelDrawPP0(x, yy, data);
            nbPnlWork[i].ActiveMode = 0;
            nbPnlWork[i].OfsX = 0;
            nbPnlWork[i].OldHp = -1;
            nbPnlWork[i].OldMp = -1;
            nbPnlWork[i].HpAnmCnt = 0;
            nbPnlWork[i].MpAnmCnt = 0;
        }
        yb += 0x17;
        y += 0x17;
    }
    nbPnl_rsetAlpha(0);
    nbPnlSprite(0x103, 0x14F, 0xFD, 3, 1, 0x73, 0xFD, 3, nbPanelBaseABGR, data->pnltex[1]);
    nbPnlSprite(0x103, 0x1AE, 0xFD, 3, 1, 0x73, 0xFD, 3, nbPanelBaseABGR, data->pnltex[1]);
}

#else

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelPartyDraw);

#endif

/* NON_MATCHING: 65 of 66 words match; the one differing word is the branch form
 * after nbPanelStockCheckRun, which turns into bne once nbPanelPartyDraw is a
 * visible C definition in this unit. */

#ifdef NON_MATCHING

s32 nbPanelDrawProcess(void)
{
    nbMainProcessData_t *data;
    nbFormation_t *form;
    s32 i;

    data = nbGetMainProcessData();
    if (data == NULL) {
        return 0;
    }
    nbPanelPressTurn(data);
    nbPanelAnalyzeRun();
    if (nbPnlDrawFlag == 0) {
        return 0;
    }
    nbPanelPressGauge(data);
    nbPanelAuto(data);
    data = nbGetMainProcessData();
    if (data->ActionProcessID[data->NowActionProcess] != NULL && data->orderindex != -1 &&
        nbGetActionSeq(data->ActionProcessID[data->NowActionProcess]) == 5) {
        form = data->form;
        for (i = 0; i < 4; i++) {
            if ((form->stat & 0x109) == 0x109) {
                nbPanelPartySelect(data);
                return 0;
            }
            form++;
        }
    }
    if (nbPanelStockCheckRun() == 0) {
        nbPanelPartyDraw();
    }
    return 0;
}

#else

INCLUDE_ASM(const s32, "newbattle/nbPanelProcess", nbPanelDrawProcess);

#endif

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
    dds3LinkProcess(data->MainProcessID, id);
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

void nbPanelFlashInit(nbPanelFlashData_t *pFlash, u32 color0, u32 color1, s32 x, f32 scaleX, f32 scaleY, s32 y)
{
    pFlash->PT_1_Process = 1;
    pFlash->PT_1_Frame = 0;
    pFlash->PT_2_Process = 1;
    pFlash->PT_2_Frame = 0;
    pFlash->PT_2_Rate = 0.1f;
    pFlash->PT_3_Frame = 0;
    pFlash->PT_3_Process = 1;
    pFlash->PT_3_Rate = 0.1f;
    pFlash->scaleX = scaleX;
    pFlash->scaleY = scaleY;
    pFlash->color0 = color0;
    pFlash->color1 = color1;
    pFlash->x = x;
    pFlash->y = y;
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

extern nbPanelProcess_nbPanelXYZ_t nbPanelPTPartList[];
extern void nbPanelPolygonFlashDraw(s32 x, s32 y, s32 centerX, s32 centerY,
                                    s32 width, s32 height, u32 color, u32 edgeColor);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545488);

INCLUDE_RODATA(const s32, "newbattle/nbPanelProcess", D_00545498);

void nbPanelFlashDraw(nbPanelFlashData_t *pFlash, u32 abgrBase)
{
    /* Colours/scales are frame snapshots; each layer refreshes its position. */
    f32 scaleX = pFlash->scaleX;
    f32 scaleY = pFlash->scaleY;
    s32 x = pFlash->x;
    s32 y = pFlash->y + 87;
    u32 color0 = pFlash->color0;
    u32 color1 = pFlash->color1;
    /* One live phase rate: qualified stock 00.rtl shares its pseudo across
     * cases; 13.life crosses six calls and 20.greg retains f20, including the
     * short alpha-only fade. Case-local rates instead select f0 there. */
    f32 rate;
    u32 color;

    switch (pFlash->PT_1_Process) {
    case 0:
        break;
    case 1:
        nbPnl_rsetAlpha(1);
        rate = (f32)pFlash->PT_1_Frame / 7.0f;
        color = nbPanelFlashRGBAMulti(color0 | ((u32)(rate * 32.0f) << 24), abgrBase);
        nbPanelPolygonFlashDraw(x, y, nbPanelPTPartList[2].x + (nbPanelPTPartList[2].w >> 1),
                                nbPanelPTPartList[2].y + (nbPanelPTPartList[2].h >> 1),
                                (s32)((nbPanelPTPartList[2].w +
                                       (s32)(((f32)nbPanelPTPartList[5].w - (f32)nbPanelPTPartList[2].w) * rate)) * scaleX),
                                (s32)((nbPanelPTPartList[2].h +
                                       (s32)(((f32)nbPanelPTPartList[5].h - (f32)nbPanelPTPartList[2].h) * rate)) * scaleY),
                                color, color0);
        if (++pFlash->PT_1_Frame >= 7) {
            pFlash->PT_1_Frame = 0;
            pFlash->PT_1_Process = 2;
        }
        break;
    case 2:
        nbPnl_rsetAlpha(1);
        color = nbPanelFlashRGBAMulti(color0 | 0x20000000, abgrBase);
        nbPanelPolygonFlashDraw(x, y, nbPanelPTPartList[2].x + (nbPanelPTPartList[2].w >> 1),
                                nbPanelPTPartList[2].y + (nbPanelPTPartList[2].h >> 1),
                                (s32)(nbPanelPTPartList[5].w * scaleX), (s32)(nbPanelPTPartList[5].h * scaleY),
                                color, color0);
        if (++pFlash->PT_1_Frame >= 10) {
            pFlash->PT_1_Frame = 0;
            pFlash->PT_1_Process = 3;
        }
        break;
    case 3:
        nbPnl_rsetAlpha(1);
        rate = 1.0f - (f32)pFlash->PT_1_Frame / 15.0f;
        color = nbPanelFlashRGBAMulti(color0 | ((u32)(rate * 32.0f) << 24), abgrBase);
        rate = 1.0f - rate;
        nbPanelPolygonFlashDraw(x, y, nbPanelPTPartList[2].x + (nbPanelPTPartList[2].w >> 1),
                                nbPanelPTPartList[2].y + (nbPanelPTPartList[2].h >> 1),
                                (s32)((nbPanelPTPartList[5].w +
                                       (s32)(((f32)nbPanelPTPartList[8].w - (f32)nbPanelPTPartList[5].w) * rate)) * scaleX),
                                (s32)((nbPanelPTPartList[5].h +
                                       (s32)(((f32)nbPanelPTPartList[8].h - (f32)nbPanelPTPartList[5].h) * rate)) * scaleY),
                                color, color0);
        if (++pFlash->PT_1_Frame >= 15) {
            pFlash->PT_1_Frame = 0;
            pFlash->PT_1_Process = 4;
        }
        break;
    case 4:
        pFlash->PT_1_Process = 0;
        break;
    }

    x = pFlash->x;
    y = pFlash->y + 87;

    switch (pFlash->PT_2_Process) {
    case 0:
        break;
    case 1:
        nbPnl_rsetAlpha(1);
        rate = (f32)pFlash->PT_2_Frame / 3.0f;
        color = nbPanelFlashRGBAMulti(color1 | ((u32)(rate * 128.0f) << 24), abgrBase);
        nbPanelPolygonFlashDraw(x, y, nbPanelPTPartList[3].x + (nbPanelPTPartList[3].w >> 1),
                                nbPanelPTPartList[3].y + (nbPanelPTPartList[3].h >> 1),
                                (s32)(nbPanelPTPartList[3].w * scaleX), (s32)(nbPanelPTPartList[3].h * scaleY),
                                color, color0);
        if (++pFlash->PT_2_Frame >= 3) {
            pFlash->PT_2_Frame = 0;
            pFlash->PT_2_Process = 2;
        }
        break;
    case 2:
        nbPnl_rsetAlpha(1);
        color = nbPanelFlashRGBAMulti(color1 | 0x80000000, abgrBase);
        nbPanelPolygonFlashDraw(x, y, nbPanelPTPartList[3].x + (nbPanelPTPartList[3].w >> 1),
                                nbPanelPTPartList[3].y + (nbPanelPTPartList[3].h >> 1),
                                (s32)(nbPanelPTPartList[3].w * scaleX), (s32)(nbPanelPTPartList[3].h * scaleY),
                                color, color0);
        if (++pFlash->PT_2_Frame >= 3) {
            pFlash->PT_2_Frame = 0;
            pFlash->PT_2_Process = 3;
            pFlash->PT_2_Rate = 0.1f;
        }
        break;
    case 3:
        nbPnl_rsetAlpha(1);
        rate = (f32)pFlash->PT_2_Frame / 5.0f;
        rate *= pFlash->PT_2_Rate;
        pFlash->PT_2_Rate *= 1.35f;
        if (pFlash->PT_2_Rate > 1.0f) {
            pFlash->PT_2_Rate = 1.0f;
        }
        color = nbPanelFlashRGBAMulti(color1 | 0x80000000, abgrBase);
        nbPanelPolygonFlashDraw(x, y, nbPanelPTPartList[3].x + (nbPanelPTPartList[3].w >> 1),
                                nbPanelPTPartList[3].y + (nbPanelPTPartList[3].h >> 1),
                                (s32)((nbPanelPTPartList[3].w +
                                       (s32)(((f32)nbPanelPTPartList[6].w - (f32)nbPanelPTPartList[3].w) * rate)) * scaleX),
                                (s32)((nbPanelPTPartList[3].h +
                                       (s32)(((f32)nbPanelPTPartList[6].h - (f32)nbPanelPTPartList[3].h) * rate)) * scaleY),
                                color, color0);
        if (++pFlash->PT_2_Frame >= 5) {
            pFlash->PT_2_Frame = 0;
            pFlash->PT_2_Process = 4;
        }
        break;
    case 4:
        nbPnl_rsetAlpha(1);
        rate = 1.0f - (f32)pFlash->PT_2_Frame / 23.0f;
        color = nbPanelFlashRGBAMulti(color1 | ((u32)(rate * 128.0f) << 24), abgrBase);
        rate = 1.0f - rate;
        nbPanelPolygonFlashDraw(x, y, nbPanelPTPartList[3].x + (nbPanelPTPartList[3].w >> 1),
                                nbPanelPTPartList[3].y + (nbPanelPTPartList[3].h >> 1),
                                (s32)((nbPanelPTPartList[6].w +
                                       (s32)(((f32)nbPanelPTPartList[9].w - (f32)nbPanelPTPartList[6].w) * rate)) * scaleX),
                                (s32)((nbPanelPTPartList[6].h +
                                       (s32)(((f32)nbPanelPTPartList[9].h - (f32)nbPanelPTPartList[6].h) * rate)) * scaleY),
                                color, color0);
        if (++pFlash->PT_2_Frame >= 23) {
            pFlash->PT_2_Frame = 0;
            pFlash->PT_2_Process = 5;
        }
        break;
    case 5:
        pFlash->PT_2_Process = 0;
        break;
    }

    x = pFlash->x;
    y = pFlash->y + 87;

    switch (pFlash->PT_3_Process) {
    case 0:
        break;
    case 1:
        nbPnl_rsetAlpha(1);
        rate = (f32)pFlash->PT_3_Frame / 10.0f;
        pFlash->PT_3_Rate *= 1.35f;
        if (pFlash->PT_3_Rate > 1.0f) {
            pFlash->PT_3_Rate = 1.0f;
        }
        color = nbPanelFlashRGBAMulti(color0 | ((u32)(rate * 96.0f) << 24), abgrBase);
        nbPanelPolygonFlashDraw(x, y, nbPanelPTPartList[4].x + (nbPanelPTPartList[4].w >> 1),
                                nbPanelPTPartList[4].y + (nbPanelPTPartList[4].h >> 1),
                                (s32)((nbPanelPTPartList[4].w +
                                       (s32)(((f32)nbPanelPTPartList[7].w - (f32)nbPanelPTPartList[4].w) * rate)) * scaleX),
                                (s32)((nbPanelPTPartList[4].h +
                                       (s32)(((f32)nbPanelPTPartList[7].h - (f32)nbPanelPTPartList[4].h) * rate)) * scaleY),
                                color, color0);
        if (++pFlash->PT_3_Frame >= 10) {
            pFlash->PT_3_Frame = 0;
            pFlash->PT_3_Process = 2;
        }
        break;
    case 2:
        nbPnl_rsetAlpha(1);
        rate = 1.0f - (f32)pFlash->PT_3_Frame / 15.0f;
        color = nbPanelFlashRGBAMulti(color0 | ((u32)(rate * 96.0f) << 24), abgrBase);
        rate = 1.0f - rate;
        nbPanelPolygonFlashDraw(x, y, nbPanelPTPartList[4].x + (nbPanelPTPartList[4].w >> 1),
                                nbPanelPTPartList[4].y + (nbPanelPTPartList[4].h >> 1),
                                (s32)((nbPanelPTPartList[7].w +
                                       (s32)(((f32)nbPanelPTPartList[10].w - (f32)nbPanelPTPartList[7].w) * rate)) * scaleX),
                                (s32)((nbPanelPTPartList[7].h +
                                       (s32)(((f32)nbPanelPTPartList[10].h - (f32)nbPanelPTPartList[7].h) * rate)) * scaleY),
                                color, color0);
        if (++pFlash->PT_3_Frame >= 15) {
            pFlash->PT_3_Frame = 0;
            pFlash->PT_3_Process = 3;
        }
        break;
    case 3:
        pFlash->PT_3_Process = 0;
        break;
    }
}

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

