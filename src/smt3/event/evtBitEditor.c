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

extern s32 dds3BitEditorRowBase, dds3BitEditorTable, dds3BitEditorTable2;

extern char dds3BitEditorGlyphTemplate[];

extern void *sdfDevPrintf(s32, s32, s32, s32, const char *, ...);

extern void sdfDrawTagLink(sdfDrawTag_t *, addr_t);

extern void *sdfGrAllocPacketBuff(u32);

extern void sdfDrawTagInit(sdfDrawTag_t *);

extern void dds3ConsPanelTagLink(sdfDrawTag_t *, s32, s32, s32, s32);

extern s32 dds3SelectMenu(s32, s32, s32, s32, s32, s32 *, s32 *, s32 *, s32 *);

extern void *memcpy(void *, const void *, size_t);

void evtBitOff(s32 no);

/* dds3ModelEffectCtrlr_t.type: which effect a controller drives. */
typedef enum dds3ModelEffectType {
    dds3ModelEffectType_P2A = 0,
    dds3ModelEffectType_D3P = 1,
    dds3ModelEffectType_TRACK = 2,
    dds3ModelEffectType_IPU = 3
} dds3ModelEffectType;

/* pibHeaderChunk_t.kind, as mdlCreateEffect dispatches on it. */
typedef enum PIB_CHUNK_TYPE {
    PIB_CHUNK_TYPE_P2A = 1,
    PIB_CHUNK_TYPE_D3P = 2,
    PIB_CHUNK_TYPE_TRACK = 3,
    PIB_CHUNK_TYPE_BLUR = 4,
    IPU = 5,
    PIB_CHUNK_TYPE_END = 0xFFFF
} PIB_CHUNK_TYPE;

extern char mdlPibLabelP2A[];

extern char mdlPibLabelD3P[];

extern char mdlColorByteFormat[];

extern char mdlPibCursorMark[];

extern char mdlDebugLightCursor[];

extern char mdlDebugLightFormat[];

extern char mdlDebugScaleCursor[];

extern char softenBiasCursor[];

extern char softenBiasValueFormat[];

extern char softenBiasAdjustFormat[];

extern char softenBiasLabelOn[];

extern char softenBiasLabelOff[];

extern char dds3BitEditorColumnFormat[];

extern char dds3BitEditorRowFormat[];

s32 evtBitCheck(s32 no);

/* Signed flag indices need a bias before arithmetic right shift divides by 32. */ void evtBitOn(s32 flag);

s32 dds3BitEditorProcess(void) {
    char glyph[2];
    sdfDrawTag_t *packets;
    s32 column, row, style, result;
    s32 cellWidth = 0xC0, cellHeight = 0x60;

    /* The DDS1 twin at 0021F664..74 copies this space/NUL template as a
     * block too; see DDS docs/idioms.md, "Bit-editor glyph template block copy". */
    memcpy(glyph, dds3BitEditorGlyphTemplate, sizeof(glyph));
    packets = (sdfDrawTag_t *)sdfGrAllocPacketBuff(sizeof(*packets));
    sdfDrawTagInit(packets);
    dds3ConsPanelTagLink(packets, 50, 50, 23, 20);
    sdfDrawTagLink(packets, EE_ADDR(sdfDevPrintf(
        0x7320, 0x7A90, 0xFF0000, 0, "BitEditor")));
    if (evtBitCheck((dds3BitEditorRowBase + dds3BitEditorTable2) * 16 + dds3BitEditorTable)) {
        sdfDrawTagLink(packets, EE_ADDR(sdfDevPrintf(
            0x7AA0, 0x7A90, 0xFF0000, 7, "%04X = ON",
            (dds3BitEditorRowBase + dds3BitEditorTable2) * 16 + dds3BitEditorTable)));
    } else {
        sdfDrawTagLink(packets, EE_ADDR(sdfDevPrintf(
            0x7AA0, 0x7A90, 0xFF0000, 7, "%04X = OFF",
            (dds3BitEditorRowBase + dds3BitEditorTable2) * 16 + dds3BitEditorTable)));
    }
    for (column = 0; column < 16; column++) {
        style = dds3BitEditorTable == column ? 14 : 7;
        sdfDrawTagLink(packets, EE_ADDR(sdfDevPrintf(
            0x7320 + (column + 6) * cellWidth, 0x7A90 + cellHeight * 2,
            0xFF0000, style, dds3BitEditorColumnFormat, column)));
    }
    for (row = 0; row < 16; row++) {
        style = dds3BitEditorTable2 == row ? 14 : 7;
        sdfDrawTagLink(packets, EE_ADDR(sdfDevPrintf(
            0x73E0, 0x7A90 + (row + 3) * cellHeight, 0xFF0000,
            style, dds3BitEditorRowFormat, (dds3BitEditorRowBase + row) * 16)));
        for (column = 0; column < 16; column++) {
            if (evtBitCheck((dds3BitEditorRowBase + row) * 16 + column)) {
                glyph[0] = 'O';
                style = dds3BitEditorTable == column || dds3BitEditorTable2 == row ? 6 : 0;
            } else {
                glyph[0] = '+';
                style = dds3BitEditorTable == column || dds3BitEditorTable2 == row ? 14 : 8;
            }
            if (dds3BitEditorTable == column && dds3BitEditorTable2 == row) {
                style = 4;
            }
            sdfDrawTagLink(packets, EE_ADDR(sdfDevPrintf(
                0x7320 + (column + 6) * cellWidth,
                0x7A90 + (row + 3) * cellHeight, 0xFF0000, style, glyph)));
        }
    }
    { sdfGrOt_t *ot = &sdfGrOtTable[81]; ((mdlOtRegFunc_t)ot->regFunc)(ot, packets); }
    if ((dds3PadBitTable[24] & 2) && dds3BitEditorRowBase > 0) {
        dds3BitEditorRowBase -= 16;
        if (dds3BitEditorRowBase < 0) dds3BitEditorRowBase = 0;
    }
    if (dds3PadBitTable[26] & 2) {
        if (dds3BitEditorRowBase + 16 < 256) {
            if (dds3BitEditorRowBase + 32 > 256) dds3BitEditorRowBase = 240;
            else dds3BitEditorRowBase += 16;
        }
    }
    result = dds3SelectMenu(1, 16, 256, 16, 16, NULL,
        &dds3BitEditorRowBase, &dds3BitEditorTable, &dds3BitEditorTable2);
    if (result == -1) return -1;
    if (result == 1) {
        if (evtBitCheck((dds3BitEditorRowBase + dds3BitEditorTable2) * 16 + dds3BitEditorTable)) {
            evtBitOff((dds3BitEditorRowBase + dds3BitEditorTable2) * 16 + dds3BitEditorTable);
        } else {
            evtBitOn((dds3BitEditorRowBase + dds3BitEditorTable2) * 16 + dds3BitEditorTable);
        }
    }
    return 0;
}

