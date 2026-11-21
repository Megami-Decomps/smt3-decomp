#include "sdk_kernel_alarm_entries.h"

extern s32 kCopy_2(u32 *, const u32 *, u32);

/* InitAlarm first installs the copy helper and the copied resident code.
 * It discovers the remaining handlers from the kernel entry table. */
kernelAlarmEntryPair_t alarmEntryPairs[8] = {
    {0x5A, {.copy = kCopy_2}},
    {0x5B, {.resident = (void *)0x80076000}},
    {0xFC, {.resident = NULL}},
    {0xFE, {.resident = NULL}},
    {0xFD, {.resident = NULL}},
    {0xFF, {.resident = NULL}},
    {0x12C, {.resident = NULL}},
    {0x08, {.resident = NULL}},
};
