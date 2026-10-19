#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "scr.h"
#include "model.h"
#include "draw3d.h"
#include "ps2asm.h"

typedef struct pbPanel_t pbPanel_t;

typedef struct { s32 value; } pbPanelValue_t;

typedef struct { s32 code, unk04, timer; } pbErrorWork_t;

typedef struct pbGroupDB_t pbGroupDB_t;

typedef struct
{
    s32 dispStat;
    s16 cellX, cellY;
    s32 rot;
    u8 unk0C[4];
    sdfVector4_t pos;
    union { s32 field; mdlObj_t *model; };
} pbMyCharacter_t;

typedef struct { s32 format, type, free, result; } pbMCardInfo_t;

typedef struct { u8 unk00[4]; u16 group; } pbGroupEntry_t;

struct pbGroupDB_t { u32 count; pbGroupEntry_t *entry[1]; };

typedef struct
{
    s32 unk00;
    union { s32 value; struct { s16 unk04, unk06; }; };
    sdfMat_t *mat;
} pbMatWork_t;

typedef struct { f32 scale; u8 unk04[0xC]; sdfVector4_t eye, target; } pbViewCamWork_t;

typedef struct { u8 unk00[8]; s32 modelIndex; } pbPanelModel_t;

typedef struct { s32 value; u8 unk04[0xC0]; f32 x, y, z; } pbBlockObj_t;

typedef struct { s16 index; u16 step; u8 unk04[4]; pbBlockObj_t *block; } pbRoundBlock_t;

typedef struct { u32 count; pbGroupEntry_t *entry[0x40]; sdfMat_t *mat; } pbBlockDB_t;

typedef struct { u8 flag[4]; void *block, *floor, *chara; } pbRecordSlot_t;

typedef struct { s32 pos; pbRecordSlot_t *slot; } pbRecord_t;

/* Common panel work header: its constructor installs process and data links. */
struct pbPanel_t
{
    dds3ProcessID_t *process;
    union { u32 flags; struct { u16 unk04, state; }; };
    s16 x, y;
    union { pbPanelValue_t *value; pbGroupDB_t *groupDB; pbMyCharacter_t *chara;
            pbRecord_t *record; pbMatWork_t *mat; pbBlockDB_t *blockDB;
            pbPanelModel_t *model; pbRoundBlock_t *round; pbErrorWork_t *error;
            pbViewCamWork_t *viewCam; } data;
};

extern void sdfChipFreeDelayedDirect(void *ptr);

extern s32 sceMcGetInfo(s32 port, s32 slot, s32 *type, s32 *free, s32 *format);

extern pbPanel_t *record;

extern pbPanel_t *blockDB;

extern s32 pb_draw_error_Process(dds3ProcessID_t *id);

extern void pb_draw_error_Shutdown(dds3ProcessID_t *id);

extern void sceMcMkdir(s32 port, s32 slot, const char *name);

extern void sceMcChdir(s32 port, s32 slot, const char *newdir, char *currentdir);

extern void sceMcGetDir(s32 port, s32 slot, const char *name, u32 mode, s32 maxent, void *table);

extern void sceMcSync(s32 mode, s32 *cmd, s32 *result);

INCLUDE_ASM(const s32, "pb/pbMCard", pbInitMCard);

s32 pbGetFormatStatusMCard(s32 port, s32 slot, pbMCardInfo_t *info) {
    return sceMcGetInfo(port, slot, NULL, NULL, &info->format);
}

s32 pbGetTypeStatusMCard(s32 port, s32 slot, pbMCardInfo_t *info) {
    return sceMcGetInfo(port, slot, &info->type, NULL, NULL);
}

s32 pbGetFreeStatusMCard(s32 port, s32 slot, pbMCardInfo_t *info) {
    return sceMcGetInfo(port, slot, NULL, &info->free, NULL);
}

void pbGetStatusMCard(s32 port, s32 slot, pbMCardInfo_t *info) {
    sceMcGetInfo(port, slot, &info->type, &info->free, &info->format);
    sceMcSync(0, NULL, &info->result);
}

s32 pbGetDirMCard(s32 port, s32 slot, const char *fileName, void *tbl) {
    s32 result;
    sceMcGetDir(port, slot, fileName, 0, 18, tbl);
    sceMcSync(0, NULL, &result);
    return result;
}

s32 pbGetDirMCard2(s32 port, s32 slot, const char *fileName) {
    s32 result;
    sceMcGetDir(port, slot, fileName, 0, -1, NULL);
    sceMcSync(0, NULL, &result);
    return result;
}

s32 pbChangeDirectory(s32 port, s32 slot, const char *pathName) {
    s32 result;
    sceMcChdir(port, slot, pathName, NULL);
    sceMcSync(0, NULL, &result);
    return result;
}

s32 pbMkdirMCard(s32 port, s32 slot, const char *dirName) {
    s32 result;
    sceMcMkdir(port, slot, dirName);
    sceMcSync(0, NULL, &result);
    return result;
}

INCLUDE_RODATA(const s32, "pb/pbMCard", D_00536498);

INCLUDE_RODATA(const s32, "pb/pbMCard", D_005364C0);

INCLUDE_ASM(const s32, "pb/pbMCard", pbReadMCard);

INCLUDE_RODATA(const s32, "pb/pbMCard", D_00536538);

INCLUDE_RODATA(const s32, "pb/pbMCard", D_00536560);

INCLUDE_ASM(const s32, "pb/pbMCard", pbWriteMCard);

INCLUDE_ASM(const s32, "pb/pbMCard", pb_draw_error_Process);

void pb_draw_error_Shutdown(dds3ProcessID_t *id) {
    pbPanel_t *panel = id->workdata;
    sdfChipFreeDelayedDirect(panel->data.value);
    sdfChipFreeDelayedDirect(panel);
}
