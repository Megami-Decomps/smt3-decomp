#include "common.h"
#include "libsdf.h"
#include "model.h"
#include "kernel.h"
#include "newdata.h"
#include "ps2asm.h"
#include "motion.h"

typedef void (*mdlOtRegFunc_t)(sdfGrOt_t *, sdfDrawTag_t *);

extern sdfGrOt_t sdfGrOtTable[];

extern u8 dds3PadBitTable[];

extern s32 D_0054F728, dds3BitEditorTable, dds3BitEditorTable2;

extern char D_0054F738[];

extern void *sdfDevPrintf(s32, s32, s32, s32, const char *, ...);

extern void sdfDrawTagLink(sdfDrawTag_t *, u32);

extern void *sdfGrAllocPacketBuff(u32);

extern void sdfDrawTagInit(sdfDrawTag_t *);

extern void dds3ConsPanelTagLink(sdfDrawTag_t *, s32, s32, s32, s32);

extern s32 dds3SelectMenu(s32, s32, s32, s32, s32, s32 *, s32 *, s32 *, s32 *);

extern void *memcpy(void *, const void *, u32);

void evtBitOff(s32 flag);

/* SDK matrix-column gather: EE GCC cannot express the MMI pack from C. */
typedef enum MdlResourceKind {
    MDL_RESOURCE_BILLBOARD = 0,
    MDL_RESOURCE_EFFECT = 1,
    MDL_RESOURCE_TRACK_POLY = 2,
    MDL_RESOURCE_OBJECT = 3
} MdlResourceKind;

typedef enum MdlViewerRecordKind {
    MDL_VIEWER_RECORD_KIND_BILLBOARD_PART = 1,
    MDL_VIEWER_RECORD_KIND_EFFECT_PART = 2,
    MDL_VIEWER_RECORD_KIND_TRACK_POLY = 3,
    MDL_VIEWER_RECORD_KIND_STREAM = 4,
    MDL_VIEWER_RECORD_KIND_OBJECT_PART = 5
} MdlViewerRecordKind;

extern char D_0054F648[];

extern char D_0054F650[];

extern char D_0054F678[];

extern char D_0054F680[];

extern char D_0054F6A0[];

extern char D_0054F6A8[];

extern char D_0054F6E0[];

extern char D_0054F700[];

extern char D_0054F708[];

extern char D_0054F710[];

extern char D_0054F718[];

extern char D_0054F720[];

extern char D_0054F740[];

extern char D_0054F748[];

s32 evtBitCheck(s32 flag);

/* Signed flag indices need a bias before arithmetic right shift divides by 32. */ void evtBitOn(s32 flag);

s32 dds3BitEditorProcess(void) {
    char glyph[2];
    sdfDrawTag_t *packets;
    s32 column, row, style, result;
    s32 cellWidth = 0xC0, cellHeight = 0x60;

    /* The DDS1 twin at 0021F664..74 copies this space/NUL template as a
     * block too; see DDS docs/idioms.md, "Bit-editor glyph template block copy". */
    memcpy(glyph, D_0054F738, sizeof(glyph));
    packets = (sdfDrawTag_t *)sdfGrAllocPacketBuff(sizeof(*packets));
    sdfDrawTagInit(packets);
    dds3ConsPanelTagLink(packets, 50, 50, 23, 20);
    sdfDrawTagLink(packets, (u32)sdfDevPrintf(
        0x7320, 0x7A90, 0xFF0000, 0, "BitEditor"));
    if (evtBitCheck((D_0054F728 + dds3BitEditorTable2) * 16 + dds3BitEditorTable)) {
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x7AA0, 0x7A90, 0xFF0000, 7, "%04X = ON",
            (D_0054F728 + dds3BitEditorTable2) * 16 + dds3BitEditorTable));
    } else {
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x7AA0, 0x7A90, 0xFF0000, 7, "%04X = OFF",
            (D_0054F728 + dds3BitEditorTable2) * 16 + dds3BitEditorTable));
    }
    for (column = 0; column < 16; column++) {
        style = dds3BitEditorTable == column ? 14 : 7;
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x7320 + (column + 6) * cellWidth, 0x7A90 + cellHeight * 2,
            0xFF0000, style, D_0054F740, column));
    }
    for (row = 0; row < 16; row++) {
        style = dds3BitEditorTable2 == row ? 14 : 7;
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x73E0, 0x7A90 + (row + 3) * cellHeight, 0xFF0000,
            style, D_0054F748, (D_0054F728 + row) * 16));
        for (column = 0; column < 16; column++) {
            if (evtBitCheck((D_0054F728 + row) * 16 + column)) {
                glyph[0] = 'O';
                style = dds3BitEditorTable == column || dds3BitEditorTable2 == row ? 6 : 0;
            } else {
                glyph[0] = '+';
                style = dds3BitEditorTable == column || dds3BitEditorTable2 == row ? 14 : 8;
            }
            if (dds3BitEditorTable == column && dds3BitEditorTable2 == row) {
                style = 4;
            }
            sdfDrawTagLink(packets, (u32)sdfDevPrintf(
                0x7320 + (column + 6) * cellWidth,
                0x7A90 + (row + 3) * cellHeight, 0xFF0000, style, glyph));
        }
    }
    { sdfGrOt_t *ot = &sdfGrOtTable[81]; ((mdlOtRegFunc_t)ot->regFunc)(ot, packets); }
    if ((dds3PadBitTable[24] & 2) && D_0054F728 > 0) {
        D_0054F728 -= 16;
        if (D_0054F728 < 0) D_0054F728 = 0;
    }
    if (dds3PadBitTable[26] & 2) {
        if (D_0054F728 + 16 < 256) {
            if (D_0054F728 + 32 > 256) D_0054F728 = 240;
            else D_0054F728 += 16;
        }
    }
    result = dds3SelectMenu(1, 16, 256, 16, 16, NULL,
        &D_0054F728, &dds3BitEditorTable, &dds3BitEditorTable2);
    if (result == -1) return -1;
    if (result == 1) {
        if (evtBitCheck((D_0054F728 + dds3BitEditorTable2) * 16 + dds3BitEditorTable)) {
            evtBitOff((D_0054F728 + dds3BitEditorTable2) * 16 + dds3BitEditorTable);
        } else {
            evtBitOn((D_0054F728 + dds3BitEditorTable2) * 16 + dds3BitEditorTable);
        }
    }
    return 0;
}

