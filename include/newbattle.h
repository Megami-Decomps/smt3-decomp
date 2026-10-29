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

/* Decoded entry in a battle pack. */
typedef struct nbPacEntry_t {
    u8 unk_00[0x8];
    void *buf;
} nbPacEntry_t;

/* Battle pack handle and its decoded entry. */
typedef struct NB_FILEHANDLE_T {
    u8 unk_00[0x60];
    nbPacEntry_t *entry;
} nbPacHandle_t;

struct nbMainProcessData_t;
struct dds3ProcessID_t;

/* Battle event script process work. */
typedef struct nbEventProcessData_t {
    struct nbMainProcessData_t *data;
    s32 eventrid;
    s32 time;
    s32 seq;
    s32 seqtime;
    void *userdata;
    void (*unk_18)(struct dds3ProcessID_t *id);
    void *file;
    nbPacHandle_t *pfile;
    void *mem;
} nbEventProcessData_t;

/* Texture coordinates used by the battle panel tables. */
typedef struct nbPanelProcess_nbPanelTMXuv_t {
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;
} nbPanelProcess_nbPanelTMXuv_t;

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
