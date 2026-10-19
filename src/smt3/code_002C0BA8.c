#include "fpu.h"
#include "common.h"
#include "libsdf.h"
#include "ps2asm.h"

u32 dds3GetRandA(void *aState);

enum sdf_dev_state {
    SDF_DEV_STATE_OPENING = 2,
    SDF_DEV_STATE_SEEKING = 3,
    SDF_DEV_STATE_READING = 4,
    SDF_DEV_STATE_WRITING = 5,
    SDF_DEV_STATE_CLOSING = 6,
    SDF_DEV_STATE_ACTIVE = 7,
    SDF_DEV_STATE_COMPLETE = 8,
    SDF_DEV_STATE_INACTIVE = 9
};

enum sdf_dev_operation {
    SDF_DEV_OPERATION_NONE = 0,
    SDF_DEV_OPERATION_OPEN_READ = 1,
    SDF_DEV_OPERATION_OPEN_WRITE = 2,
    SDF_DEV_OPERATION_SEEK = 3,
    SDF_DEV_OPERATION_GET_SIZE = 4,
    SDF_DEV_OPERATION_READ = 5,
    SDF_DEV_OPERATION_WRITE = 6,
    SDF_DEV_OPERATION_CLOSE = 7,
    SDF_DEV_OPERATION_READ_BUFFER = 8,
    SDF_DEV_OPERATION_WRITE_BUFFER = 9
};

typedef struct sdfRandCB_t {
    u32 words[4];
} sdfRandCB_t;

enum sdf_tex_resource {
    SDF_TEX_RESOURCE_FREE = 0,
    SDF_TEX_RESOURCE_FRAME = 1,
    SDF_TEX_RESOURCE_TEXTURE = 2,
    SDF_TEX_RESOURCE_CLUT = 3,
    SDF_TEX_RESOURCE_KNOWN_MODE_COUNT = 4
};

enum sdf_stream_output {
    SDF_STREAM_OUTPUT_RGBA32 = 0,
    SDF_STREAM_OUTPUT_RGBA16 = 1,
    SDF_STREAM_OUTPUT_RGBA16_DITHERED = 2
};

enum sdf_stream_playback {
    SDF_STREAM_PLAYBACK_INITIAL = 0,
    SDF_STREAM_PLAYBACK_FIRST_COMPLETION = 1,
    SDF_STREAM_PLAYBACK_CADENCED = 2
};

extern sdfRandCB_t sdfDefaultRandCB;

/* Four-word shift-register PRNG; NULL selects the shared effect RNG state. */
u32 dds3GetRandA(void *aState)
{
    sdfRandCB_t *state = aState;
    u32 x0;
    u32 x1;
    u32 x2;
    u32 x3;
    u32 rand;
    if (state == ((void*)0))
    {
        state = &sdfDefaultRandCB;
    }
    x0 = state->words[0];
    x1 = state->words[1];
    x2 = state->words[2];
    x3 = state->words[3];
    /* Mix the high two bits of x0 and high bit of x2 into the shifted taps. */
    rand = ((x1 << 0x02) | (((x0 >> 0x1e)) % 4)) ^ ((x3 << 0x01) | (((x2 >> 0x1f)) % 2));
    state->words[0] = rand;
    state->words[1] = x0;
    state->words[2] = x1;
    state->words[3] = x2;
    return rand;
}

/* Uniform float in [0, 1): 24 random bits scaled by 2^-24. */
f32 dds3GetRandFloatA(sdfRandCB_t *state) {
    u32 value = dds3GetRandA(state) & 0xFFFFFF;
    return (f32)value * 5.9604644775390625e-8f;
}

u32 dds3GetRandIntA(void *aState, u32 aRange)
{
    return dds3GetRandA(aState) % aRange;
}
