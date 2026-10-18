#ifndef NEWDATA_H
#define NEWDATA_H

#include "common.h"
#include "libsdf.h"

/* Field-flag rows and the persistent digging-party records used by the
 * upstream field helpers. Their strides are 0x1E and 0xEC respectively. */
typedef struct
{
    u16 modelFlag;
    u8 unk02[0x1A];
    u16 hojiFlag;
} fldGbFlags_t;

typedef struct
{
    fldGbFlags_t flags[32];
} fldGbTypeBlock_t;

typedef struct
{
    u8 unk00[4];
    u32 flag;
    u8 unk08[2];
    u16 hp;
    u8 unk0C[0xE0];
} fldAnahoriAkuma_t;

/* Shared persistent work, recovered through its consumers. These fields form
 * a partial view, not a claim about the complete save-work allocation. */
typedef struct
{
    u8 unk00[0xE];
    s16 transition; /* 0xE: fldMainLoadStart */
    u8 unk10[0x30];
    s32 integers[256];
    f32 floats[256];
    u32 eventFlags[128]; /* 0x840: 4096 event bits */
    u8 moonFlags;       /* 0xA40 */
    u8 moonPhase;       /* 0xA41 */
    u8 unkA42[2];
    f32 moonTimer;      /* 0xA44 */
    s32 maka;
    u32 maFlags;                  /* 0xA4C */
    fldAnahoriAkuma_t akuma[16];   /* 0xA50 */
    u8 unk1910[0x3E4];
    f32 encounterRemainder;        /* 0x1CF4 */
    u16 encounterGauge;            /* 0x1CF8 */
    u8 unk1CFA[0x495E];
    fldGbTypeBlock_t gb[48];       /* 0x6658 */
    u8 takaraOpen[0x20];           /* 0x11A58 */
    s16 previewX;                 /* 0x11A78: slSetTmnlID */
    s16 previewY;
    u8 unk11A7C[4];
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

/* datCalc pins the parameters through levelupparam. cmbCountSkills,
 * rstClearSkillSlot and cmbClearSkillSlots pin 24 halfword inherited-skill
 * slots at +0x86. cmbClearStockEntry clears the complete native 0xEC record.
 * The IDB's field order differs: it has no field at +0x80 and widens namecode,
 * skillcnt and skill, which retail contradicts here. See docs/idb-corrections.md. */
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
    /* 0x28 */ char namecode[8];
    /* 0x30 */ s32 skillcnt;
    /* 0x34 */ u16 skill[24];
    /* 0x64 */ s16 nowcommand;
    /* 0x66 */ u16 nowindex;
    /* 0x68 */ s16 nowtarea;
    /* 0x6A */ s16 nowtform;
    /* 0x6C */ s16 prevcommand;
    /* 0x6E */ u16 previndex;
    /* 0x70 */ s16 prevtarea;
    /* 0x72 */ s16 prevtform;
    /* 0x74 */ s8 mitamaparam[6];
    /* 0x7A */ s8 levelupparam[6];
    /* 0x80 */ u8 unk80[6];
    /* 0x86 */ u16 keisyoskill[24];
    /* 0xB6 */ u8 keiattr[24];
    /* 0xCE */ u16 hensinmae;
    /* 0xD0 */ u16 getdevilhearts[4];
    /* 0xD8 */ u32 hensinmaeexp;
    /* 0xDC */ u16 fullnamecode[8];
} datUnitWork_t;

/* Model-visual records have a 0x310-byte native stride. The upstream packet
 * helpers use the scalar prefix; the action helpers index eight 0x1C-byte
 * visual-part records starting at +0x32. */
typedef struct
{
    u8 unk00;
    s8 unk01;
    u8 unk02[0x16];
    u16 unk18;
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
    datUnitVisualPart_t part[8];
    u8 unk112[0x1FE];
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
    datNormalSkill_t normal[288];
    u8 reserve[8];
    datNormalSkillVisual_t visual[288];
} datSkillTable_t;

extern datSkillTable_t datSkillTable;


#endif /* NEWDATA_H */
