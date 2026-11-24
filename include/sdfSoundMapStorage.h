#ifndef SMT3_SDF_SOUND_MAP_STORAGE_H
#define SMT3_SDF_SOUND_MAP_STORAGE_H

#include "common.h"

typedef struct
{
    u32 unk0;
    u8 unk4;
    u8 unk5;
    u16 unk6;
} p2sdLoadEntry_t;

typedef struct sndChannel_t {
    u8 index;
    u8 unk1;
    s16 unk2;
    u32 unk4;
    u32 iopWorkAddress;
    u32 unkC;
} sndChannel_t;

typedef struct sndIopBuffer_t {
    s32 size;
    u32 iopAddress;
} sndIopBuffer_t;

typedef struct {
    u8 values[24];
    u8 pad18[8];
} sndMidiStateGroup_t;

typedef struct sndMergedWork_t {
    s32 header;
    s32 bufferCount;
    u32 iopBufferAddress;
    u32 iopBufferBytes;
    sndChannel_t channels[16];
    sndIopBuffer_t buffers[16];
    p2sdLoadEntry_t slots[7];
    void *eeWorkAddress;
    u32 eeWorkByteCount;
    u32 opaque1D0;
    u32 opaque1D4;
    u32 opaque1D8;
    u32 opaque1DC;
    u8 controlData[7][0x80];
    sndMidiStateGroup_t stateGroups[2];
} sndMergedWork_t;

/* The shared sound work area has both named fields and word-indexed consumers. */
typedef union {
    sndMergedWork_t work;
    u32 words[0x5A0 / sizeof(u32)];
} sdfSoundMapStorage_t;

SIZE_CHECK(sdfSndMapNativeSize, sdfSoundMapStorage_t, 0x5A0);
SIZE_CHECK(sdfSndMapWorkNativeSize, sndMergedWork_t, 0x5A0);
extern sdfSoundMapStorage_t sdfSndMap;

#endif
