#ifndef FIELD_H
#define FIELD_H

#include "common.h"

/* Counted filename tables use a variable-length trailing array. The native
 * filename entry is 0x0C bytes and the file-resource row is 0x24 bytes. */
typedef struct fldFileNameEntry_t
{
    u32 word00;
    const char *name;
    u32 word08;
} fldFileNameEntry_t;

typedef struct fldFileNameTable_t
{
    u32 count;
    fldFileNameEntry_t entries[1];
} fldFileNameTable_t;

typedef struct fldFileResource_t
{
    u32 id;
    u32 word04;
    const char *name;
    u32 word0C;
    const f32 *transform; /* Serialized translation and rotation input. */
    u32 word14;
    fldFileNameTable_t *names;
    u32 word1C;
    void *data;
} fldFileResource_t;

/* Shared view of fldGlobalBase through the last recovered position component.
 * Unknown gaps remain opaque; this is not the complete global allocation. */
typedef struct fldAreaWork_t
{
    u32 flags;                  /* 0x000 */
    s32 area;                   /* 0x004 */
    s32 floor;                  /* 0x008: zero-based */
    s32 blockID;                /* 0x00C */
    u8 unk10[4];
    u32 consumedFlags;          /* 0x014 */
    s32 mode;                   /* 0x018 */
    u8 unk1C[4];
    char requestedSceneName[0x10]; /* 0x020 */
    sdfVector4_t campos;        /* 0x030 */
    u8 unk40[0x44];
    s32 resourceFlag;           /* 0x084 */
    s32 resourceArea;           /* 0x088 */
    s32 resourceFloor;          /* 0x08C */
    u8 unk90[0xC];
    s32 cameraParamIndex;       /* 0x09C */
    s32 sequenceCode;           /* 0x0A0 */
    u8 unkA4[4];
    s32 reuseAreaResources;     /* 0x0A8 */
    u8 unkAC[0xC];
    s32 lightMaIdx;             /* 0x0B8 */
    u8 unkBC[0x44];
    f32 x;                      /* 0x100 */
    f32 y;                      /* 0x104 */
    f32 z;                      /* 0x108 */
    u8 unk10C[0xC];
    f32 hitX;                   /* 0x118 */
    f32 hitY;
    f32 hitZ;
    u8 unk124[0x10];
    s32 hitValid;               /* 0x134 */
} fldAreaWork_t;

extern fldAreaWork_t fldGlobalBase;

#endif /* FIELD_H */
