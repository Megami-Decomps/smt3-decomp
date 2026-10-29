#ifndef NEWBATTLE_H
#define NEWBATTLE_H

#include "common.h"
#include "libsdf.h"

typedef struct nbFormation_t
{
    sdfVector4_t orgpos;
    sdfVector4_t orgrot;
    sdfVector4_t pos;
    sdfVector4_t rot;
    sdfVector4_t center;
    f32 r;
    f32 h;
    f32 cr;
    f32 scl;
    f32 oneshotr;
    f32 allshotr;
    f32 movey;
    f32 movez;
    s32 type;
    u32 time;
    u32 stat;
    u32 flag;
    u32 anime;
    struct dds3Basic_t *basic;
    s32 serial;
    struct evtUnit_t *hunit;
    s16 partyindex;
    s16 formindex;
    u32 autotime;
    struct effBTLSysHandle_t *badeff;
    u32 modelcol;
    f32 motspeed;
    s16 motendtype;
    s16 motenddata;
    s16 nexthokan;
    s16 nexthokanb;
    s16 waitmot;
    s16 motcate;
    s32 reserve4;
    struct NB_FILEHANDLE_T *packh;
    /* Motion modes reuse the loading/target words for damage and shake state. */
    union {
        char *akey;
        s32 damageFrames;
    };
    union {
        s32 read_minor;
        f32 damageRange;
    };
    union {
        u32 tartime;
        f32 shakePower;
    };
    union {
        f32 range;
        s32 shakePhase;
    };
    union {
        f32 addmovet;
        s32 badframe;
    };
    f32 movet;
    sdfVector4_t tarpos;
    sdfVector4_t startpos;
} nbFormation_t;

/* The native 0x80-byte battle party record. */
typedef struct nbParty_t {
    u16 flag;
    s16 formindex;
    s16 statindex;
    u16 partyindex;
    s16 count[20];
    u32 aisyo[16];
    u16 hojoskill[8];
} nbParty_t;

#endif /* NEWBATTLE_H */
