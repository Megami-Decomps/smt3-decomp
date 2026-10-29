#ifndef NEWDATA_H
#define NEWDATA_H

#include "common.h"
#include "libsdf.h"

/* Field-flag rows and the persistent digging-party records used by the
 * upstream field helpers. Their strides are 0x1E and 0xEC respectively. */
typedef struct
{
    u16 modelFlag;
    u16 animeFlag;
    u16 atariFlag;
    u16 modelFlag2;
    u16 animeFlag2;
    u8 animeId2[0x10];
    u16 npcFlag;
    u16 hojiFlag;
} fldGbFlags_t;

typedef struct
{
    fldGbFlags_t flags[32];
} fldGbTypeBlock_t;

typedef struct
{
    s32 unk00;
    u32 flag;
    u16 devilId;
    u16 hp;
    union {
        struct {
            u8 unk0C[6];
            u16 badstatus;
            u32 exp;
        };
        struct {
            u8 statePrefix[4];
            u64 packedState;
        } __attribute__((packed));
    };
    u8 pad18[2];
    s8 level;
    u8 unk1B[0x21];
    u16 skillcnt;
    u16 skill[24];
    u8 unk6E[0x7E];
} fldAnahoriAkuma_t;

/* The IDB's field order at PS2 widths: narrowing its 0x120-byte record, where
 * skillcnt is s32 and skill is s32[24], gives this 0xEC record with no gap.
 * Three HD bodies pin the names against retail: rstChkAddSkill tests
 * skillcnt at +0x38, SetAction_AI stores nowcommand and nowindex at +0x6A and
 * +0x6C, and datGetBaseParam sums param, skillparam and mitamaparam.
 * See docs/idb-corrections.md. */
typedef struct datUnitWork_t
{
    /* 0x00 */ u32 flag;
    /* 0x04 */ u16 id;
    /* 0x06 */ u16 hp;
    /* 0x08 */ u16 maxhp;
    /* 0x0A */ u16 mp;
    /* 0x0C */ u16 maxmp;
    /* 0x0E */ u16 badstatus;
    /* 0x10 */ u32 exp;
    /* 0x14 */ u16 level;
    /* 0x16 */ s8 param[6];
    /* 0x1C */ s8 skillparam[6];
    /* 0x22 */ u16 uniqueid;
    /* 0x24 */ u16 reserve1;
    /* 0x26 */ u16 reserve2;
    /* 0x28 */ u16 namecode[8];
    /* 0x38 */ u16 skillcnt;
    /* 0x3A */ u16 skill[24];
    /* 0x6A */ s16 nowcommand;
    /* 0x6C */ u16 nowindex;
    /* 0x6E */ s16 nowtarea;
    /* 0x70 */ s16 nowtform;
    /* 0x72 */ s16 prevcommand;
    /* 0x74 */ u16 previndex;
    /* 0x76 */ s16 prevtarea;
    /* 0x78 */ s16 prevtform;
    /* 0x7A */ s8 mitamaparam[6];
    /* 0x80 */ s8 levelupparam[6];
    /* 0x86 */ u16 keisyoskill[24];
    /* 0xB6 */ u8 keiattr[24];
    /* 0xCE */ u16 hensinmae;
    /* 0xD0 */ u16 getdevilhearts[4];
    /* 0xD8 */ u32 hensinmaeexp;
    /* 0xDC */ u16 fullnamecode[8];
} datUnitWork_t;

/* Shared persistent work, recovered through its consumers. These fields form
 * a partial view, not a claim about the complete save-work allocation. */
typedef struct
{
    u8 ver[8];      /* 0x0: "VER" plus a version byte, then four zeroed bytes */
    s32 word08;
    s16 half0C;
    s16 transition; /* 0xE: fldMainLoadStart */
    u8 unk10[0x24];
    sdfMemHandle_t *memHandle;
    s32 unk38;
    s32 playtime;
    s32 integers[256];
    f32 floats[256];
    u32 EventBit[128]; /* 0x840: 4096 event bits */
    u8 moonFlags;       /* 0xA40 */
    u8 moonPhase;       /* 0xA41 */
    u8 unkA42[2];
    f32 moonTimer;      /* 0xA44 */
    s32 maka;
    u32 maFlags;                  /* 0xA4C */
    union {
        struct { fldAnahoriAkuma_t akuma[16]; u8 unk1910[4]; };
        struct { u32 unitPrefix; datUnitWork_t unitwork[16]; };
    };
    /* 0x1914..0x1CF4, split on the IDB's names for the span. */
    union {
        u8 unk1914[0x10];
        u8 stockOrder[16];
    };
    union {
        struct {
            u8 stocklist[65];      /* 0x1924 */
            u8 stockcnt[80];
            u8 item[19];
        };
        struct {
            u32 stockCount;
            u8 stockRemainder[160];
        };
    };
    s16 boss_press[40][2];            /* 0x19C8 */
    u8 syakkincnt[640];
    u8 negoques[4];                /* 0x1CE8 */
    u8 kotowari[4];                /* 0x1CEC */
    u16 wakimi_id;                 /* 0x1CF0 */
    u16 bouga_cnt;                 /* 0x1CF2 */
    f32 encountkyori;              /* 0x1CF4 */
    u16 encountCount;              /* 0x1CF8 */
    u8 heartsequip;
    union {
        struct {
            u8 hearts[0x80];       /* 0x1CFB */
            u8 heartscnt;          /* 0x1D7B */
            u8 nameData[0x4800];   /* 0x1D7C */
        };
        struct {
            u8 heartsPrefix[0x40];
            u8 devilSlot[0x100];   /* 0x1D3B */
        };
    };
    u8 unk657C[0xDC];
    fldGbTypeBlock_t gb[48];       /* 0x6658 */
    u8 takaraOpen[0x20];           /* 0x11A58 */
    s16 previewX;                 /* 0x11A78: slSetTmnlID */
    s16 previewY;
    s32 moonAnahoriCount;
    u8 takaraOpenEx[0x20];         /* 0x11A80 */
    u8 reserved11AA0[0x20];
    u16 heartsSkills[32][12];    /* 0x11AC0 */
} dds3GlobalWork_t;

extern dds3GlobalWork_t *DDS3_GBWK;

typedef struct
{
    /* 0x00 */ s32 flag;
    /* 0x04 */ s32 flag2;
    /* 0x08 */ s32 encid;
    /* 0x0C */ s32 dbest;
} datBossPress_t;

typedef struct datDevilTableEntry_t
{
    /* 0x00 */ union { u32 flag; struct { u8 flagByte; u8 flagReserved[3]; }; };
    /* 0x04 */ u8 race;
    /* 0x05 */ u8 level;
    /* 0x06 */ u16 hp;
    /* 0x08 */ u16 maxhp;
    /* 0x0A */ u16 mp;
    /* 0x0C */ u16 maxmp;
    /* 0x0E */ s16 aisyoid;
    /* 0x10 */ s8 param[6];
    /* 0x16 */ u16 skill[8];
    /* 0x26 */ s16 keisyotype;
    /* 0x28 */ u16 keisyoform;
    /* 0x2A */ u16 dropmakka;
    /* 0x2C */ u16 dropexp;
    /* 0x2E */ u8 dropitem[2];
    /* 0x30 */ u8 droppoint[2];
    /* 0x32 */ u16 specialbit;
    /* 0x34 */ u8 specialitem;
    /* 0x35 */ u8 specialpoint;
    /* 0x36 */ u8 hougyokupoint;
    /* 0x37 */ u8 masekipoint;
    /* 0x38 */ s8 attackattr;
    /* 0x39 */ u8 attackcnt;
    /* 0x3A */ u8 attackinterval;
    /* 0x3B */ u8 reserve1;
} datDevilTableEntry_t;



/* The 0x1C-byte motion records at datUnitVisual_t +0x2C; the shared view splits
 * the same bytes as part[], six bytes later. */
typedef struct nbUnitMotion_t {
    s16 movetype;
    u16 actframe;
    union {
        s32 se_no;
        struct { u8 seBytes[2]; u8 type; s8 data; };
    };
    f32 motionsp;
    f32 atarisize;
    f32 movey;
    f32 movez;
    s16 bframe;
    s16 hframe;
} nbUnitMotion_t;


/* Complete motion-bank view of a native 0x310-byte model-visual row. */
typedef struct {
    sdfVector4_t center;
    f32 formsize;
    f32 formheight;
    f32 formscale;
    f32 oneshotr;
    f32 allshotr;
    u16 modelsize;
    u16 shadowsize;
    u16 badframe;
    u16 autowait;
    nbUnitMotion_t motions[26];
    u32 reserved[3];
} nbUnitVisualMotions_t;

typedef char nbUnitVisualMotions_size_must_be_0x310[
    sizeof(nbUnitVisualMotions_t) == 0x310 ? 1 : -1];


typedef struct
{
    datUnitWork_t unit;
} cmbUnit_t;

/* Model-visual records have a 0x310-byte native stride. The upstream packet
 * helpers use the scalar prefix; the action helpers index 0x1C-byte
 * visual-part records starting at +0x32, up to index 11. */
typedef struct
{
    u8 unk00;
    s8 unk01;
    u8 unk02[0x16];
    u16 actframe;
    u8 unk1A[2];
} datUnitVisualPart_t;

typedef struct datUnitVisual_t
{
    sdfVector4_t center;
    f32 formsize;
    f32 formheight;
    f32 formscale;
    f32 oneshotr;
    f32 allshotr;
    u16 modelsize;
    u16 shadowsize;
    u16 badframe;
    u16 autowait;
    void *motion;
    u16 unk30;
    datUnitVisualPart_t part[12];
    u8 unk182[0x18E];
} datUnitVisual_t;

typedef struct { u16 ritu, skill; u32 target; } datDevilAITbl_t;

typedef struct {
    datDevilAITbl_t table[4][5];
    u32 reserved_A0;
} datDevilAIData_t;

typedef struct { u8 ritu, item; } datNegoPresent_t;

typedef struct {
    s32 kucyo;
    u32 tokucyo;
    u8 tokuboutype, honboutype, fighttype, reqtype, tuikareqtype;
    u8 privatesel, privateselindex, senseitype, inotitype, binjotype;
    u16 mukantype, mukanindex;
    datNegoPresent_t itema[4], itemb[4], stone[4];
} datDevilNegoFormat_t;

extern datDevilNegoFormat_t master_text[];

typedef struct datSkill_t
{
    s8 skillattr;
    s8 type;
    s16 index;
    u16 keisyoform;
    s8 capacity;
    u8 flag;
} datSkill_t;

typedef struct
{
    u8 flag;
    u8 use;
    u8 koukatype;
    u8 costtype;
    u16 cost;
    u16 costbase;
    u8 targettype;
    u8 targetarea;
    u8 targetrule;
    u8 targetrandom;
    u16 untargetbadstat;
    u16 targetprog;
    u8 hittype;
    u8 hitlevel;
    u16 hitprog;
    u8 targetcntmin;
    u8 targetcntmax;
    u16 hptype;
    s16 hpn;
    u16 mptype;
    s16 mpn;
    s16 hpbase;
    s16 mpbase;
    u16 minus;
    u8 badtype;
    u8 badlevel;
    u16 basstatus;
    u32 hojotype;
    s8 hojopoint;
    u8 deadtype;
    u32 program;
    s16 criticalpoint;
    s16 failpoint;
    s16 magicbase;
    s16 magiclimit;
} datNormalSkill_t;

typedef struct
{
    u8 motion;
    u8 animetype;
    u8 hatudo;
    u8 bedno;
} datNormalSkillVisual_t;

typedef struct
{
    s16 n;
    s16 m;
    s16 a;
    s16 b;
} datSpecialSkill_t;

typedef struct
{
    datNormalSkill_t normal[288];
    u8 reserve[8];
    datNormalSkillVisual_t visual[288];
    datSpecialSkill_t special[95];
} datSkillTable_t;

extern datSkillTable_t datSkillTable;

typedef struct nbNegoSkillCondition_t {
    s32 ptype;
    s32 etype;
    s32 message;
} nbNegoSkillCondition_t;

typedef struct nbNegoSkillEntry_t {
    s32 requestmode;
    u8 rate[4];
    nbNegoSkillCondition_t condition[8];
} nbNegoSkillEntry_t;




/* Target codes for the AI's group and single target helpers. SMT3HD recovers this
 * enum in its own newdata.h and never cites it from a body, so the names come from
 * the definition alone; SetGroupTarget2 switches on all eleven values in order. */
typedef enum TAR_CODE {
    TAR_CODE_AI = 0,
    TAR_CODE_LIFEMIN = 1,
    TAR_CODE_BAD = 2,
    TAR_CODE_ID = 3,
    TAR_CODE_LIGHT = 4,
    TAR_CODE_DARK = 5,
    TAR_CODE_PLAYER = 6,
    TAR_CODE_JOKEN = 7,
    TAR_CODE_RND = 8,
    TAR_CODE_BAARLV = 9,
    TAR_CODE_LVMIN = 10,
} TAR_CODE;

#endif /* NEWDATA_H */
