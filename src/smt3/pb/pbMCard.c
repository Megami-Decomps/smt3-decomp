#include "common.h"
#include "pbRecord.h"
#include "kernel.h"
#include "libsdf.h"
#include "scr.h"
#include "model.h"
#include "draw3d.h"
#include "ps2asm.h"

typedef struct pbProcHandle_t pbProcHandle_t;

typedef struct { s32 value; } pbPanelValue_t;

typedef struct { s32 code, unk04, timer; } pbErrorWork_t;

typedef struct pbGroupDB_t pbGroupDB_t;

typedef struct
{
    s32 flg;
    s16 x, y;
    s32 dir;
    u8 dmy[4];
    sdfVector4_t pos;
    union { s32 field; mdlObj_t *model; };
} pbMyChar_t;

typedef struct { s32 format, type, free, result; } pbMCardInfo_t;

typedef struct { u8 unk00[4]; u16 group; } pbGroupEntry_t;

struct pbGroupDB_t { u32 count; pbGroupEntry_t *entry[1]; };

typedef struct
{
    s32 flg;
    union { s32 value; struct { s16 unk04, unk06; }; };
    sdfMat_t *mat;
} pbExit_t;

typedef struct { f32 scale; u8 unk04[0xC]; sdfVector4_t eye, target; } pbViewCamWork_t;

typedef struct { u8 unk00[8]; s32 modelIndex; } pbPanelModel_t;

typedef struct { s32 value; u8 unk04[0xC0]; f32 x, y, z; } pbGroupData_t;

typedef struct { s16 index; u16 step; u8 unk04[4]; pbGroupData_t *block; } pbRoundBlock_t;

typedef struct { u32 count; pbGroupEntry_t *entry[0x40]; sdfMat_t *mat; } pbBlockDB_t;


/* Common panel work header: its constructor installs process and data links. */
struct pbProcHandle_t
{
    dds3ProcessID_t *id;
    union { u32 flags; struct { u16 unk04, state; }; };
    s16 x, y;
    union { pbPanelValue_t *value; pbGroupDB_t *groupDB; pbMyChar_t *chara;
            pbRecord_t *record; pbExit_t *mat; pbBlockDB_t *blockDB;
            pbPanelModel_t *model; pbRoundBlock_t *round; pbErrorWork_t *error;
            pbViewCamWork_t *viewCam; } data;
};

extern void sdfChipFreeDelayedDirect(void *aPtr);

extern s32 sceMcGetInfo(s32 a, s32 b, s32 *c, s32 *d, s32 *e);
extern s32 sceSifLoadModule(const char *path, s32 argSize, const char *args);
extern s32 sceMcInit(void);
extern s32 printf(const char *format, ...);
extern const char D_0054F600[];

extern pbProcHandle_t *record;

extern pbProcHandle_t *blockDB;

extern s32 pb_draw_error_Process(dds3ProcessID_t *id);

extern void pb_draw_error_Shutdown(dds3ProcessID_t *id);

extern void sceMcMkdir(s32 a, s32 b, const char *c);

extern void sceMcChdir(s32 a, s32 b, const char *c, char *d);

extern void sceMcGetDir(s32 a, s32 b, const char *c, u32 d, s32 e, void *f);

extern void sceMcSync(s32 a, s32 *b, s32 *c);

void pbInitMCard(void)
{
    while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/sio2man.irx", 0, NULL) < 0) {
        printf("Can't load module sio2man\n");
    }
    while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/mcman.irx", 0, NULL) < 0) {
        printf("Can't load module mcman\n");
    }
    while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/mcserv.irx", 0, NULL) < 0) {
        printf("Can't load module mcserv\n");
    }
    printf(D_0054F600, sceMcInit());
}

s32 pbGetFormatStatusMCard(s32 port, s32 slot, pbMCardInfo_t *stat) {
    return sceMcGetInfo(port, slot, NULL, NULL, &stat->format);
}

s32 pbGetTypeStatusMCard(s32 port, s32 slot, pbMCardInfo_t *stat) {
    return sceMcGetInfo(port, slot, &stat->type, NULL, NULL);
}

s32 pbGetFreeStatusMCard(s32 port, s32 slot, pbMCardInfo_t *stat) {
    return sceMcGetInfo(port, slot, NULL, &stat->free, NULL);
}

void pbGetStatusMCard(s32 port, s32 slot, pbMCardInfo_t *stat) {
    sceMcGetInfo(port, slot, &stat->type, &stat->free, &stat->format);
    sceMcSync(0, NULL, &stat->result);
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

extern const char D_0054F608[];
extern const char D_00536498[];
extern const char D_005364C0[];
INCLUDE_RODATA(const s32, "pb/pbMCard", D_00536498);

INCLUDE_RODATA(const s32, "pb/pbMCard", D_005364C0);

const char D_005364E8[40] __attribute__((aligned(8))) = "\tregist read command. return = %d\n";
const char D_00536510[40] __attribute__((aligned(8))) = "\tcan't read a file < %s > err = %d\n";
extern const char D_00536538[];
extern const char D_00536560[];
extern s32 sceMcOpen(s32 port, s32 slot, char *name, s32 flags);
extern s32 sceMcClose(s32 fd);
extern s32 sceMcRead(s32 fd, void *buffer, s32 size);

s32 pbReadMCard(s32 port, s32 slot, char *fileName, void *buffer, s32 size)
{
    s32 fd;
    s32 readResult;
    s32 commandResult;

    printf(D_0054F608);
    commandResult = sceMcOpen(port, slot, fileName, 3);
    printf(D_00536498, commandResult);
    sceMcSync(0, NULL, &fd);
    if (fd < 0) {
        printf(D_005364C0, fileName, fd);
        return fd;
    }

    commandResult = sceMcRead(fd, buffer, size);
    printf(D_005364E8, commandResult);
    sceMcSync(0, NULL, &readResult);
    if (readResult < 0) {
        printf(D_00536510, fileName, readResult);
    }

    commandResult = sceMcClose(fd);
    printf(D_00536538, commandResult);
    sceMcSync(0, NULL, &commandResult);
    if (commandResult < 0) {
        printf(D_00536560, fileName, commandResult);
    }
    printf(D_0054F608);
    return readResult;
}

extern s32 sceMcWrite(s32 fd, void *buffer, s32 size);
INCLUDE_RODATA(const s32, "pb/pbMCard", D_00536538);

INCLUDE_RODATA(const s32, "pb/pbMCard", D_00536560);

const char D_00536588[40] __attribute__((aligned(8))) = "\tregist write command. return = %d\n";
const char D_005365B0[40] __attribute__((aligned(8))) = "\tcan't write a file < %s > err = %d\n";

s32 pbWriteMCard(s32 port, s32 slot, char *fileName, s32 flags,
                 void *buffer, s32 size)
{
    s32 fd;
    s32 writeSync;
    s32 commandResult;

    printf(D_0054F608);
    commandResult = sceMcOpen(port, slot, fileName, flags);
    printf(D_00536498, commandResult);
    sceMcSync(0, NULL, &fd);
    if (fd < 0) {
        printf(D_005364C0, fileName, fd);
        return fd;
    }

    commandResult = sceMcWrite(fd, buffer, size);
    printf(D_00536588, commandResult);
    sceMcSync(0, NULL, &writeSync);
    if (writeSync < 0) {
        printf(D_005365B0, fileName, writeSync);
    }

    commandResult = sceMcClose(fd);
    printf(D_00536538, commandResult);
    sceMcSync(0, NULL, &commandResult);
    if (commandResult < 0) {
        printf(D_00536560, fileName, commandResult);
    }
    printf(D_0054F608);
    return writeSync;
}

/* The error strings start at D_003278D4; each 0x84-byte row is preceded by its
 * own attribute word, which is why the row before supplies it. */
typedef struct { char text[0x80]; s32 attr; } pbErrorRow_t;

extern pbErrorRow_t D_003278D4[];

typedef struct
{
    s32 pcharNum;
    u8 unk004[0x14];
    s16 exitX, exitY;
    u8 unk01C[0x320];
    s32 charIndex;
    void *baseTex;
    sdfTexHandle_t *tex344;
} pbMapWork_t;

extern pbMapWork_t *D_0054F5C4;

extern sdfGrOt_t sdfGrOtTable[];

PS2_SYMBOL_AT(sdfGrOt_t, pbErrorOt39, sdfGrOtTable, 0x4E0);

extern void rsetOtOfs(u32 ofs);
extern void rsetAlpha(s32 alpha);
extern void rsetTest(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
extern void DrwSprite2D(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh,
                        u32 rgba, sdfTexHandle_t *th);
extern s32 strlen(const char *str);
extern sdfDrawTag_t *sdfGrAllocPacketBuff(s32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern void sdfDrawTagLink(sdfDrawTag_t *tag, u32 packet);
extern void sdfPrimSetOpaquePacket(sdfDrawTag_t *packet);
extern u32 dds3PrimFrameRect(s32 x, s32 y, s32 z, s32 w, s32 h, u32 bg, u32 frame);
extern u32 sdfDevPrintf(s32 x, s32 y, u32 z, s32 attr, const char *format, ...);

#ifdef NON_MATCHING
/* NON_MATCHING: 27 of 135 words match; 536 bytes emitted against retail 540. */
s32 pb_draw_error_Process(dds3ProcessID_t *id)
{
    pbProcHandle_t *panel = id->workdata;
    pbErrorWork_t *error = panel->data.error;
    sdfDrawTag_t *tag;
    sdfDrawTag_t *packet;
    s32 x;
    s32 len;
    s32 half;
    s32 pitch = 0xC0;

    switch (panel->state) {
    case 1:
        if (panel != NULL) {
            panel->flags = 0;
        }
        break;
    case 5:
        return -1;
    }
    if ((u32)error->unk04 >= (u32)error->timer) {
        return -1;
    }
    if (error->code == 9) {
        rsetOtOfs(0x47);
        rsetAlpha(0);
        rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        DrwSprite2D(0, 0xC3, 0x200, 0x3A, 0, 0x1C4, 0x200, 0x3A, 0x80808080,
                    D_0054F5C4->tex344);
    } else {
        len = strlen(D_003278D4[error->code].text);
        half = (0x200 - len * 12) >> 1;
        x = half << 4;
        tag = sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(tag);
        packet = sdfGrAllocPacketBuff(0x40);
        sdfPrimSetOpaquePacket(packet);
        sdfDrawTagLink(tag, (u32)packet);
        sdfDrawTagLink(tag, dds3PrimFrameRect(x + 0x6FD0, 0x7FB8, 0xFFFF7F,
                                              len * pitch + 0x60, 0x90,
                                              0x20000000, 0x40008000));
        sdfDrawTagLink(tag, sdfDevPrintf(x + 0x7000, 0x7FD0, 0xFFFF80,
                                         D_003278D4[error->code - 1].attr,
                                         D_003278D4[error->code].text));
        pbErrorOt39.regFunc(&pbErrorOt39, tag);
    }
    error->unk04++;
    return 0;
}
#else
INCLUDE_ASM(const s32, "pb/pbMCard", pb_draw_error_Process);
#endif


void pb_draw_error_Shutdown(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    sdfChipFreeDelayedDirect(panel->data.value);
    sdfChipFreeDelayedDirect(panel);
}
