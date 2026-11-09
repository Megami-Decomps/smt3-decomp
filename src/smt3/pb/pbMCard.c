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
#include "sdk_libc.h"

extern const char pbMCardInitFormat[];

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
    printf(pbMCardInitFormat, sceMcInit());
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

extern const char pbMCardNewline[];
extern const char pbMCardOpenLog[];
extern const char pbMCardOpenErrLog[];
INCLUDE_RODATA(const s32, "pb/pbMCard", pbMCardOpenLog);

INCLUDE_RODATA(const s32, "pb/pbMCard", pbMCardOpenErrLog);

const char pbMCardReadLog[40] __attribute__((aligned(8))) = "\tregist read command. return = %d\n";
const char pbMCardReadErrLog[40] __attribute__((aligned(8))) = "\tcan't read a file < %s > err = %d\n";
extern const char pbMCardCloseLog[];
extern const char pbMCardCloseErrLog[];
extern s32 sceMcOpen(s32 port, s32 slot, char *name, s32 flags);
extern s32 sceMcClose(s32 fd);
extern s32 sceMcRead(s32 fd, void *buffer, s32 size);

s32 pbReadMCard(s32 port, s32 slot, char *fileName, void *buffer, s32 size)
{
    s32 fd;
    s32 readResult;
    s32 commandResult;

    printf(pbMCardNewline);
    commandResult = sceMcOpen(port, slot, fileName, 3);
    printf(pbMCardOpenLog, commandResult);
    sceMcSync(0, NULL, &fd);
    if (fd < 0) {
        printf(pbMCardOpenErrLog, fileName, fd);
        return fd;
    }

    commandResult = sceMcRead(fd, buffer, size);
    printf(pbMCardReadLog, commandResult);
    sceMcSync(0, NULL, &readResult);
    if (readResult < 0) {
        printf(pbMCardReadErrLog, fileName, readResult);
    }

    commandResult = sceMcClose(fd);
    printf(pbMCardCloseLog, commandResult);
    sceMcSync(0, NULL, &commandResult);
    if (commandResult < 0) {
        printf(pbMCardCloseErrLog, fileName, commandResult);
    }
    printf(pbMCardNewline);
    return readResult;
}

extern s32 sceMcWrite(s32 fd, const void *buffer, s32 size);
INCLUDE_RODATA(const s32, "pb/pbMCard", pbMCardCloseLog);

INCLUDE_RODATA(const s32, "pb/pbMCard", pbMCardCloseErrLog);

const char pbMCardWriteLog[40] __attribute__((aligned(8))) = "\tregist write command. return = %d\n";
const char pbMCardWriteErrLog[40] __attribute__((aligned(8))) = "\tcan't write a file < %s > err = %d\n";

s32 pbWriteMCard(s32 port, s32 slot, char *fileName, s32 flags,
                 void *buffer, s32 size)
{
    s32 fd;
    s32 writeSync;
    s32 commandResult;

    printf(pbMCardNewline);
    commandResult = sceMcOpen(port, slot, fileName, flags);
    printf(pbMCardOpenLog, commandResult);
    sceMcSync(0, NULL, &fd);
    if (fd < 0) {
        printf(pbMCardOpenErrLog, fileName, fd);
        return fd;
    }

    commandResult = sceMcWrite(fd, buffer, size);
    printf(pbMCardWriteLog, commandResult);
    sceMcSync(0, NULL, &writeSync);
    if (writeSync < 0) {
        printf(pbMCardWriteErrLog, fileName, writeSync);
    }

    commandResult = sceMcClose(fd);
    printf(pbMCardCloseLog, commandResult);
    sceMcSync(0, NULL, &commandResult);
    if (commandResult < 0) {
        printf(pbMCardCloseErrLog, fileName, commandResult);
    }
    printf(pbMCardNewline);
    return writeSync;
}

/* Ten 0x84-byte records start at 0x003278D0. The recovered symbol names
 * the first text byte, not the table base. Declare the complete table at its
 * real address so code 0 reads its own attribute without a negative index.
 * This alias emits no data and leaves the existing data owner unchanged. */
typedef struct { s32 attr; char text[0x80]; } pbErrorRow_t;
typedef pbErrorRow_t pbErrorTable_t[10];
SYMBOL_AT(pbErrorTable_t, pbErrorTable, pbMCardErrorRows, -4);

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

extern pbMapWork_t *pbMapWork;

extern sdfGrOt_t sdfGrOtTable[];

SYMBOL_AT(sdfGrOt_t, pbErrorOt39, sdfGrOtTable, 0x4E0);

extern void rsetOtOfs(s32 ofs);
extern void rsetAlpha(s32 alpha);
extern void rsetTest(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
extern void DrwSprite2D(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh,
                        s32 rgba, sdfTexHandle_t *th);
extern u32 strlen(const char *str);
extern s32 sdfGrAllocPacketBuff(s32 size);
extern void sdfDrawTagInit(sdfDrawTag_t *tag);
extern void sdfDrawTagLink(sdfDrawTag_t *tag, addr_t packet);
typedef struct sdfPrimTransPacket_t sdfPrimTransPacket_t;
extern void sdfPrimSetOpaquePacket(sdfPrimTransPacket_t *packet);
extern u64 *dds3PrimFrameRect(s32 x, s32 y, s32 z, s32 w, s32 h, u32 bg, u32 frame);
#include "sdfDevCons.h"

s32 pb_draw_error_Process(dds3ProcessID_t *id)
{
    pbProcHandle_t *panel = id->workdata;
    pbErrorWork_t *error = panel->data.error;
    sdfDrawTag_t *tag;
    sdfPrimTransPacket_t *packet;
    s32 x;
    s32 len;
    s32 pixelCenter;
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
    if ((u32)error->unk04 < (u32)error->timer) {
        if (error->code == 9) {
            rsetOtOfs(0x47);
            rsetAlpha(0);
            rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
            DrwSprite2D(0, 0xC3, 0x200, 0x3A, 0, 0x1C4, 0x200, 0x3A, 0x80808080,
                        pbMapWork->tex344);
        } else {
            len = (s32)strlen(pbErrorTable[error->code].text);
            pixelCenter = (0x200 - len * 12) >> 1;
            /* Keep the native pixel-to-fixed-point boundary. This empty
             * constraint is a target-specific aid, not original-source evidence. */
            FAKEMATCH_ASM_LIVE(pixelCenter);
            x = pixelCenter << 4;
            tag = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
            sdfDrawTagInit(tag);
            packet = EE_ADDR_AS(sdfPrimTransPacket_t *, sdfGrAllocPacketBuff(0x40));
            sdfPrimSetOpaquePacket(packet);
            sdfDrawTagLink(tag, EE_ADDR(packet));
            sdfDrawTagLink(tag, EE_ADDR(dds3PrimFrameRect(x + 0x6FD0, 0x7FB8, 0xFFFF7F,
                                                      len * pitch + 0x60, 0x90,
                                                      0x20000000, 0x40008000)));
            sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(x + 0x7000, 0x7FD0, 0xFFFF80,
                                                 pbErrorTable[error->code].attr,
                                                 pbErrorTable[error->code].text)));
            pbErrorOt39.regFunc(&pbErrorOt39, tag);
        }
        {
            /* Retail stores the updated word before setting the return value.
             * Constrain only this live store operand, after all drawing calls. */
            FAKEMATCH_ASM_REG(s32, count, "$2") = (s32)((u32)error->unk04 + 1);
            FAKEMATCH_ASM_LIVE(count);
            error->unk04 = count;
        }
        return 0;
    }
    return -1;
}

void pb_draw_error_Shutdown(dds3ProcessID_t *id) {
    pbProcHandle_t *panel = id->workdata;
    sdfChipFreeDelayedDirect(panel->data.value);
    sdfChipFreeDelayedDirect(panel);
}
