#ifndef SDK_GS_RESET_DEFAULTS_H
#define SDK_GS_RESET_DEFAULTS_H

#include "common.h"

typedef struct {
    s16 interlace;
    u16 mode;
    s16 fieldMode;
    s16 version;
    s32 intcHandlerEnabled;
    s32 intcHandlerId;
} GParam;

typedef union {
    u32 vif[4];
    u128 quadword;
} sceGsVifCommands_t;

extern GParam sceGsGParam;
extern const sceGsVifCommands_t sceGsResetPathPackets[2];

#endif
