#ifndef FACILITY_H
#define FACILITY_H

#include "common.h"
#include "libsdf.h"

typedef struct
{
    /* 0x00 */ u16 x;
    /* 0x02 */ u16 y;
    /* 0x04 */ u16 u0;
    /* 0x06 */ u16 v0;
    /* 0x08 */ u16 u1;
    /* 0x0A */ u16 v1;
} fclSpriteRect_t;

typedef struct fclSpriteEntry_t
{
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ u8 Flag;
    /* 0x06 */ u16 res;
} fclSpriteEntry_t;

typedef struct fclSpriteRes_t
{
    /* 0x00 */ s8 tex;
    /* 0x02 */ fclSpriteRect_t rect;
} fclSpriteRes_t;

typedef struct
{
    /* 0x00 */ fclSpriteRes_t *res;
    /* 0x04 */ fclSpriteEntry_t *entries;
} fclSpriteTbl_t;

typedef struct
{
    sdfIVector4_t pos_act;
    sdfIVector4_t pos_deact;
    sdfIVector4_t uv_act;
    sdfIVector4_t uv_deact;
    sdfTexHandle_t *tex[2];
} fclribbon_elem_t;

typedef struct
{
    /* 0x00 */ void *allocation;
    /* 0x04 */ sdfTexHandle_t *tex[2];
    /* 0x0C */ s32 stat;
    /* 0x10 */ s16 act;
    /* 0x12 */ s16 deact;
    /* 0x14 */ s32 ctr_act;
    /* 0x18 */ s32 ctr_deact;
    /* 0x1C */ s32 ctr;
    /* 0x20 */ s32 num;
    /* 0x24 */ fclribbon_elem_t pelem[0];
} fclRibbon_t;

typedef struct
{
    u8 reserved00[2];
    s8 partyCount;
    s8 reserveCount;
    u8 reserved04;
    s8 stock[16];
} rcvStockList_t;

typedef struct
{
    /* 0x0000 */ sdfVector4_t camera[2];
    /* 0x0020 */ f32 screenOfsX;
    /* 0x0024 */ f32 screenOfsY;
    /* 0x0028 */ sdfMemHandle_t *memhandle;
    /* 0x002C */ struct dds3ProcessID_t *procid[3];
    /* 0x0038 */ sdfTexHandle_t *texHandle[7];
    /* 0x0054 */ s32 stat;
    /* 0x0058 */ u16 unk_58;
    /* 0x005A */ u16 unk_5A;
    /* 0x005C */ s32 selectedIndex;
    /* 0x0060 */ u8 reserved0060[4];
    /* 0x0064 */ s16 lw_cur[4];
    /* 0x006C */ union {
        struct { u8 unk_6C; s8 unk_6D; };
        u16 cursorShift;
    };
    /* 0x006E */ u16 modelId;
    /* 0x0070 */ s32 unk_70;
    /* 0x0074 */ union {
        struct {
            u8 validUnit[0x10];
            u8 reserved0084[0x2DC];
        } regist;
        struct {
            u8 reserved0074[0x0C];
            s16 sorted[2][0xB8];
        } read;
    } entries;
    /* 0x0360 */ void *explainData;
    /* 0x0364 */ s32 unk_364;
    /* 0x0368 */ void *allocation;
    /* 0x036C */ void *pwork;
} fclEncyc_instance_tag;

typedef struct fclPanel
{
    /* 0x00 */ s32 X;
    /* 0x04 */ s32 Y;
    /* 0x08 */ s32 Z;
    /* 0x0C */ s16 W;
    /* 0x0E */ s16 H;
    /* 0x10 */ s32 Col;
    /* 0x14 */ u8 Spd;
    /* 0x15 */ u8 unk_15[3];
    /* 0x18 */ s32 Timer;
    /* 0x1C */ s32 Timer2;
    /* 0x20 */ s32 Flag;
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 TexHandle;
    /* 0x38 */ void (*draw)(struct fclPanel *panel, s32 id);
    /* 0x3C */ void (*update)(struct fclPanel *panel);
} fclPanel_t;

typedef struct
{
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ s16 unk_0E;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
} fclPanelPos_t;

typedef struct
{
    /* 0x00 */ u8 animState;
    /* 0x01 */ u8 reserved01[3];
    /* 0x04 */ s32 target;
    /* 0x08 */ u8 reserved08[0xC];
    /* 0x14 */ s32 phase;
    /* 0x18 */ s32 phaseStep;
    /* 0x1C */ s32 value;
} fclPanelAnimState_t;

typedef struct
{
    /* 0x00 */ fclPanelAnimState_t state;
    /* 0x20 */ void (*calcSizes)();
} fclPanelAnim_t;

typedef char fclPanelAnimStateSizeCheck[(sizeof(fclPanelAnimState_t) == 0x20) ? 1 : -1];

typedef struct fclencyceelem_t {
    u16 id;
    u16 reserved0;
    u16 maxhp;
    u16 maxmp;
    u32 exp;
    u16 level;
    s8 param[6];
    s8 skillparam[6];
    u16 skillcnt;
    u16 skill[24];
    s8 mitamaparam[6];
    s8 levelupparam[6];
    u16 keisyoskill[24];
    u8 keiattr[24];
    u16 hensinmae;
    u16 reserved1;
    u32 flag;
    u32 reserved2;
    u32 hensinmaeexp;
} fclencyceelem_t;

/* Title-mode handle data: a count followed by compendium records. */
typedef struct mselSaveData_t {
    s32 count;
    u8 reserved[12];
    fclencyceelem_t records[184];
} mselSaveData_t;
typedef char mselSaveDataSizeCheck[(sizeof(mselSaveData_t) == 0x7E90) ? 1 : -1];

typedef struct
{
    /* 0x00 */ u16 item;
    /* 0x02 */ s8 count;
    /* 0x03 */ u8 unk_03;
} ragItemReq_t;

typedef char ragItemReqSizeCheck[(sizeof(ragItemReq_t) == 4) ? 1 : -1];


struct datDevilTableEntry_t;

typedef struct
{
    /* 0x00 */ u16 index;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ struct datDevilTableEntry_t *info;
} cmbRaceSortEntry_t;

#endif
