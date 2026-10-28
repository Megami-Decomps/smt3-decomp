#ifndef EFF_BATTLE_H
#define EFF_BATTLE_H

#include "common.h"
#include "libsdf.h"
#include "effect.h"

typedef struct effBTLCut_t effBTLCut_t;
typedef struct dds3EffectPart_t dds3EffectPart_t;
typedef void (*effBTLCallback_t)(effBTLCut_t *cut);

/* Native eight-byte position/rotation selector shared by BED entries. */
typedef struct effBTLAimParams_t {
    u8 scaleKind;
    u8 aimMode;
    u8 directionMode;
    u8 unk03;
    s32 rangeOverride;
} effBTLAimParams_t;

typedef union effBTLPartValue_t {
    u32 sourceOffset;
    dds3EffectPart_t *work;
} effBTLPartValue_t;

typedef struct effBTL_SkillPartsList_t {
    u16 type;
    u8 pHandle[2];
    effBTLPartValue_t value;
    effBTLAimParams_t pos_ptr;
    u32 timing;
    u8 reverberations;
    u8 unk15[3];
} effBTL_SkillPartsList_t;

typedef struct effBTL_SkillCutTable_t {
    s32 frame;              /* 0x00 */
    u8 pad04[2];
    u16 shake;                     /* 0x06: screen-shake amount; 31 or more logs and caps at 300 frames */
    u16 shakeFrame;                /* 0x08: frame at which the shake starts */
    u16 shakeFade;                 /* 0x0A: fade applied with the shake */
    u8 colorKeys[0x24];            /* 0x0C: colour keyframes read by the unit colour keyframe routine */
    u8 rgbKeys[0x18];              /* 0x30: RGB keyframes read by the unit RGB keyframe routine */
    s32 callbackFrame;             /* 0x48 */
    u8 pad4C[0x20];
    u16 count;                     /* 0x6C */
    u8 pad6E[2];
    s32 colorRampEndFrame;         /* 0x70 */
    u8 actorScaleEnabled;          /* 0x74 */
    u8 pad75[3];
    effBTL_SkillPartsList_t entries[1];     /* 0x78 */
} effBTL_SkillCutTable_t;

/* Each BED bank reserves sixty entries, for a native stride of 0x618. */
typedef struct effBTLBank_t {
    u8 pad00[8];
    effBTL_SkillCutTable_t list;       /* 0x08 */
    u8 pad98[0x580];
} effBTLBank_t;

/* effBTLCreate allocates 0xC44 bytes; its two banks occupy 0xC30. */
typedef struct effBTL_t {
    effBTLBank_t banks[2];
    u8 unkC30[8];
    sdfMemHandle_t *resource;
    u32 CutCount;
    effBTLCut_t *hCutListTop;
} effBTL_t;

/* effBTLCutCreate allocates/clears 0x120 bytes. SMT3 has no DDS color/flags
 * words before currentFrame, so all subsequent fields are eight bytes earlier. */
struct effBTLCut_t {
    void *formation;
    effBTL_t *state;
    u32 currentFrame; /* 0x08 */
    u32 triggerFrame;
    u32 colorRampEndFrame;
    u16 kind;
    u8 unk16[2];
    s32 stat;
    effBTL_SkillCutTable_t *list;
    dds3EffectPart_t *parameterWorks[60]; /* 0x20 */
    effBTLCallback_t sourceFrameCallback; /* 0x110 */
    effBTLCallback_t triggerFrameCallback;
    void *userData;
    effBTLCut_t *next;
};


extern effBTL_t *effBTLCreate(void *pData);
extern void effBTLDelete(effBTL_t *hBTLEffect);
extern effBTLCut_t *effBTLCutCreate(effBTL_t *hBTLEffect, u16 CutNum, void *pFormation, s32 stat);
extern void effBTLCutDelete(effBTLCut_t *hBTLCutEffect);
extern void effBTLCutSetScale(effBTLCut_t *hBTLCutEffect, f32 scale);

#endif
