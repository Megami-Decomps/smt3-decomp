#ifndef ITF_MES_MANAGER_H
#define ITF_MES_MANAGER_H

#include "common.h"
#include "font.h"
#include "interface.h"
#include "sdf_resource.h"

/* A message window as the manager keeps it. stat's bits 3-5 follow its
 * selection: 0x10 requested, 0x18 drawing, 0x20 taking the pad. */
typedef struct itfMesBinMesHeader_t itfMesBinMesHeader_t;
typedef struct itfMesResource_t itfMesResource_t;
typedef struct Speaker_tag Speaker_tag;
typedef struct message_tag message_tag;
typedef struct defkey_tag defkey_tag;
typedef struct select_tag select_tag;
typedef struct window_tag window_tag;
typedef struct variable_tag variable_tag;
typedef struct curctr_tag curctr_tag;
typedef struct instanceMes_tag instanceMes_tag;

struct Speaker_tag {
    u32 x;
    u32 y;
    frFontHandle_t *pfont;
    u16 num;
    u8 pad0E[2];
};

struct message_tag {
    u32 x;
    s32 y;
    itfMesBinMesHeader_t *table;
    frFontHandle_t *pfont;
    s8 phase;
    u8 textInterfaceMask; /* 0x11: result of itfTestTextInterfaceMask(3). */
    u8 color[4];
    s16 lines;
    s16 page;
    s16 maxpage;
};

struct defkey_tag {
    s16 key;
    s16 index;
};

struct select_tag {
    u32 x;
    u32 y;
    frFontHandle_t *pfont;
    u32 mask;
    s16 phase;
    s16 select;
    s16 select_imm;
    s16 lines;
    s16 glyphColorIntensity;
    s16 numdef;
    defkey_tag defkey[15];
};

/* The speaker frame and message panel occupy +0x9C/+0xA0. */
struct window_tag {
    itfpanel_t *spk;
    itfpanel_t *mes;
    s32 mes_offset[4];
    s32 red;
    s32 green;
    s32 blue;
    s32 fadeLimit;
};

struct variable_tag {
    u32 pptr[0x20];
    sdfMemHandle_t *pmh[0x20];
};

/* Native 0xC-byte fade record shared by message windows and sound UI state. */
struct curctr_tag {
    u8 isdraw;
    u8 pad1;
    s16 phase;
    s16 ctr0;
    s16 ctr1;
    u32 ctr_key; /* 0x08: entry advances count ctr_key before triangle skip. */
};

struct instanceMes_tag {
    u32 stat;
    itfMesResource_t *pdata;
    u8 *pext; /* 0x08: relocated font resource bytes from the MSG1 trailing entry. */
    s32 z;
    s16 otindex;
    s16 type;
    Speaker_tag speaker;
    message_tag message;
    select_tag select;
    u8 unk98[4];
    window_tag window;
    variable_tag variable;
    curctr_tag curctr;
    u32 func_kuti;
};

/* pinst[window].mes: NULL while the window is free. */
typedef struct { instanceMes_tag *mes; u8 unk04[0x10]; } itfMesInstance_t;

#endif
