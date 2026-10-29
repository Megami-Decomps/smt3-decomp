#ifndef FACILITY_H
#define FACILITY_H

#include "common.h"
#include "libsdf.h"

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
    /* 0x12 */ u16 deact;
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
    fclencyceelem_t records[1];
} mselSaveData_t;

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
