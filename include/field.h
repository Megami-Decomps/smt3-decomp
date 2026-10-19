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
    s32 word10;                 /* 0x010 */
    u32 consumedFlags;          /* 0x014 */
    s32 mode;                   /* 0x018 */
    s32 word1C;                 /* 0x01C */
    char requestedSceneName[0x10]; /* 0x020 */
    sdfVector4_t campos;        /* 0x030 */
    s32 playerSeTable;          /* 0x040 */
    s32 word44;                 /* 0x044 */
    s32 hitMatterIndex;         /* 0x048 */
    s32 word4C;                 /* 0x04C */
    char *retryScript;          /* 0x050 */
    u8 unk54[8];
    s32 word5C;                 /* 0x05C */
    s32 amSBlock;               /* 0x060 */
    s32 amFloor;                /* 0x064 */
    s32 nameU;                  /* 0x068 */
    s32 nameD;                  /* 0x06C */
    s32 menuOpenFlag;           /* 0x070 */
    s32 encountState;           /* 0x074 */
    s32 word78;                 /* 0x078 */
    u8 unk7C[4];
    s32 word80;                 /* 0x080 */
    s32 resourceFlag;           /* 0x084 */
    s32 resourceArea;           /* 0x088 */
    s32 resourceFloor;          /* 0x08C */
    s32 word90;                 /* 0x090 */
    s32 word94;                 /* 0x094 */
    s32 word98;                 /* 0x098 */
    s32 cameraParamIndex;       /* 0x09C */
    s32 sequenceCode;           /* 0x0A0 */
    s32 wordA4;                 /* 0x0A4 */
    s32 reuseAreaResources;     /* 0x0A8 */
    s32 wordAC;                 /* 0x0AC */
    s32 wordB0;                 /* 0x0B0 */
    f32 floatB4;                /* 0x0B4 */
    s32 lightMaIdx;             /* 0x0B8 */
    s32 wordBC;                 /* 0x0BC */
    s32 wordC0;                 /* 0x0C0 */
    u8 unkC4[4];
    s32 wordC8;                 /* 0x0C8 */
    s32 wordCC;                 /* 0x0CC */
    void *pacRes[10];           /* 0x0D0: handle and mapped pointer per pac entry */
    s32 wordF8;                 /* 0x0F8 */
    u8 unkFC[4];
    f32 x;                      /* 0x100 */
    f32 y;                      /* 0x104 */
    f32 z;                      /* 0x108 */
    f32 float10C;               /* 0x10C */
    f32 float110;               /* 0x110 */
    f32 float114;               /* 0x114 */
    f32 hitX;                   /* 0x118 */
    f32 hitY;
    f32 hitZ;
    f32 camAngle;               /* 0x124 */
    f32 camAngle2;              /* 0x128 */
    f32 float12C;               /* 0x12C */
    f32 float130;               /* 0x130 */
    s32 hitValid;               /* 0x134 */
} fldAreaWork_t;

extern fldAreaWork_t fldGlobalBase;

#endif /* FIELD_H */
