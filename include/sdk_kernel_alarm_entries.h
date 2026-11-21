#ifndef SDK_KERNEL_ALARM_ENTRIES_H
#define SDK_KERNEL_ALARM_ENTRIES_H
#include "common.h"
typedef s32 (*kernelAlarmCopyEntry_t)(u32 *, const u32 *, u32);
typedef struct {
    u32 index;
    union {
        kernelAlarmCopyEntry_t copy;
        void *resident;
    } entry;
} kernelAlarmEntryPair_t;
extern kernelAlarmEntryPair_t alarmEntryPairs[8];
#endif
