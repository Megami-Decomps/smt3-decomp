#ifndef SMT3_NB_H
#define SMT3_NB_H

#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "newdata.h"

struct dds3Basic_t;
struct evtUnit_t;
struct effBTLSysHandle_t;
struct effBTL_t;
struct NB_FILEHANDLE_T;
struct nbParty_t;
struct nbKoukaPacket_t;

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
    f32 addmovet;
    f32 movet;
    sdfVector4_t tarpos;
    sdfVector4_t startpos;
} nbFormation_t;

/* The upstream packet prefix and the independently recovered process slots
 * describe one battle work area, not separately allocated partial views. */
typedef struct nbMainProcessData_t
{
    nbFormation_t *form;                    /* 0x000 */
    u8 unk_04[0xC];
    struct nbParty_t *party;                /* 0x010 */
    u8 unk_14[4];
    s16 playerpcnt;
    s16 enemypcnt;
    u8 unk_1C[0x18];
    s32 encno;                             /* 0x034 */
    u8 unk_38[8];
    s32 actionstackp;                      /* 0x040 */
    u32 endtype;
    u8 unk_48[0x18];
    u32 stat;                              /* 0x060 */
    u8 unk_64[0x14];
    s16 unk78;
    s16 unk7A;
    s16 unk7C;
    s16 unk7E;
    u8 unk80[8];
    s32 eventno;                           /* 0x088 */
    s32 eventrid;
    s16 negopformindex;
    s16 negoeformindex;
    s32 negoresult;
    u8 item[4];
    u8 itemcnt[4];
    u32 maka;
    u32 exp;
    u8 unkA8[0xC];
    s32 unkB4;
    u8 unkB8[4];
    dds3ProcessID_t *unkBC;
    dds3ProcessID_t *maeProcess;            /* 0x0C0 */
    dds3ProcessID_t *atoProcess;
    u8 unkC8[4];
    dds3ProcessID_t *stageProcess;          /* 0x0CC */
    dds3ProcessID_t *unitProcess;
    dds3ProcessID_t *unkD4[4];
    s32 unkE4;
    dds3ProcessID_t *unkE8;
    u8 unkEC[0x10];
    void *camera;                          /* 0x0FC */
    u8 unk100[0x14];
    dds3ProcessID_t *encounterProcess;      /* 0x114 */
    u8 unk118[4];
    struct effBTL_t *residentEffect;         /* 0x11C */
    u8 unk120[0x18];
    void *eventPack;                       /* 0x138 */
    sdfMemHandle_t *eventMemory;
} nbMainProcessData_t;

typedef struct nbKoukaProcessData_t
{
    nbMainProcessData_t *data;
    s32 packetcnt;
    struct nbKoukaPacket_t *first;
    struct nbKoukaPacket_t *last;
} nbKoukaProcessData_t;

nbMainProcessData_t *nbGetMainProcessData(void);
nbKoukaProcessData_t *nbGetKoukaProcessData(void);

#endif /* SMT3_NB_H */
