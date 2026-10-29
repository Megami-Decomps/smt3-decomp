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
    u16 maxhp;
    u16 mp;
    u16 maxmp;
    u16 badstatus;
    u32 exp;
    u8 pad18[2];
    s8 level;
    u8 unk1B[0xD1];
} fldAnahoriAkuma_t;

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
    fldAnahoriAkuma_t akuma[16];   /* 0xA50 */
    /* 0x1910..0x1CF4, split on the IDB's names for the span. */
    u8 unk1910[4];
    u8 unk1914[0x10];
    u8 stocklist[65];              /* 0x1924 */
    u8 stockcnt[80];
    u8 item[19];
    u8 boss_press[160];            /* 0x19C8 */
    u8 syakkincnt[640];
    u8 negoques[4];                /* 0x1CE8 */
    u8 kotowari[4];                /* 0x1CEC */
    u16 wakimi_id;                 /* 0x1CF0 */
    u16 bouga_cnt;                 /* 0x1CF2 */
    f32 encountkyori;              /* 0x1CF4 */
    u16 encountCount;              /* 0x1CF8 */
    u8 heartsequip;
    u8 hearts[0x80];               /* 0x1CFB */
    u8 heartscnt;                  /* 0x1D7B */
    u8 nameData[0x4800];           /* 0x1D7C */
    u8 unk657C[0xDC];
    fldGbTypeBlock_t gb[48];       /* 0x6658 */
    u8 takaraOpen[0x20];           /* 0x11A58 */
    s16 previewX;                 /* 0x11A78: slSetTmnlID */
    s16 previewY;
    s32 moonAnahoriCount;
    u8 takaraOpenEx[0x20];         /* 0x11A80 */
} dds3GlobalWork_t;

extern dds3GlobalWork_t *DDS3_GBWK;

typedef struct
{
    /* 0x00 */ s32 flag;
    /* 0x04 */ s32 flag2;
    /* 0x08 */ s32 encid;
    /* 0x0C */ s32 dbest;
} datBossPress_t;

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
