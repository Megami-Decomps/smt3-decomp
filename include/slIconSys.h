#ifndef SL_ICON_SYS_H
#define SL_ICON_SYS_H

#include "common.h"

/* Serialized PS2 memory-card icon.sys record; title holds Shift-JIS bytes. */
typedef struct slIconSys_t {
    u8 magic[4];
    u16 type;
    u16 titleLineBreak;
    u32 reserved;
    u32 transparency;
    s32 background[4][4];
    f32 lightDirection[3][4];
    f32 lightColor[3][4];
    f32 ambient[4];
    u16 title[34];
    u8 viewIcon[64];
    u8 copyIcon[64];
    u8 deleteIcon[64];
    u8 reservedTrailing[512];
} slIconSys_t;

SIZE_CHECK(slIconSysSize, slIconSys_t, 0x3C4);
extern slIconSys_t slIconSysBuf;

#endif
