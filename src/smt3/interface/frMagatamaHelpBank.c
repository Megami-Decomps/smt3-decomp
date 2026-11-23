#include "frTextBank.h"

/* Font command sequences are part of the authored text. Keep the command
 * operands separate from the readable lines rather than storing a blob. */
#define HELP_PREFIX "\xF2\x08\xFF\xFF\xF2\x06\x01\xFF"
#define HELP_COLOR_ARG_01 "\xF2\x02\x01\xFF"
#define HELP_COLOR_ARG_02 "\xF2\x02\x02\xFF"
#define HELP_COLOR_ARG_03 "\xF2\x02\x03\xFF"
#define HELP_COLOR_ARG_05 "\xF2\x02\x05\xFF"
#define HELP_STYLE_ARG_02 "\xF2\x06\x02\xFF"

/* Each row names its record, the previous fixup word, its label, and text. */
#define HELP_PAGES(ROW) \
    ROW(m00, speakerTableOffset, "hearts_000", \
        HELP_PREFIX HELP_COLOR_ARG_01 "Repel:death Weak:expel\nStr " HELP_COLOR_ARG_05 "+1" HELP_COLOR_ARG_01 \
        " Mag " HELP_COLOR_ARG_05 "+1" HELP_COLOR_ARG_01 " Vit " HELP_COLOR_ARG_05 "+2" HELP_COLOR_ARG_01 " Agi " \
        HELP_COLOR_ARG_05 "+1" HELP_COLOR_ARG_01 " Luc " HELP_COLOR_ARG_05 "+1\n") \
    ROW(m01, m00.page.pageOffset, "hearts_001", \
        HELP_PREFIX "Imparts physical attack skills\nNormal resistance\n") \
    ROW(m02, m01.page.pageOffset, "hearts_002", \
        HELP_PREFIX "Imparts ice spells\n" HELP_COLOR_ARG_03 "Null:ice" HELP_COLOR_ARG_01 " " \
        HELP_COLOR_ARG_02 "Weak:elec\n") \
    ROW(m03, m02.page.pageOffset, "hearts_003", \
        HELP_PREFIX "Imparts healing spells\n" HELP_COLOR_ARG_03 "Null:expel" HELP_COLOR_ARG_01 " " \
        HELP_COLOR_ARG_02 "Weak:death\n") \
    ROW(m04, m03.page.pageOffset, "hearts_004", \
        HELP_PREFIX "Imparts supportive spells\n" HELP_COLOR_ARG_03 "Null:mind\n" HELP_STYLE_ARG_02 \
        "(panic/charm/sleep)\n") \
    ROW(m05, m04.page.pageOffset, "hearts_005", \
        HELP_PREFIX "Imparts fire spells\n" HELP_COLOR_ARG_03 "Null:fire" HELP_COLOR_ARG_01 " " \
        HELP_COLOR_ARG_02 "Weak:force\n") \
    ROW(m06, m05.page.pageOffset, "hearts_006", \
        HELP_PREFIX "Imparts force spells\n" HELP_COLOR_ARG_03 "Null:force" HELP_COLOR_ARG_01 " " \
        HELP_COLOR_ARG_02 "Weak:fire\n") \
    ROW(m07, m06.page.pageOffset, "hearts_007", \
        HELP_PREFIX "Imparts physical attack skills\n" HELP_COLOR_ARG_03 "Resist:phys" HELP_COLOR_ARG_01 \
        " " HELP_COLOR_ARG_02 "Weak:ailments\n") \
    ROW(m08, m07.page.pageOffset, "hearts_008", \
        HELP_PREFIX "Imparts electrical spells\n" HELP_COLOR_ARG_03 "Null:elec" HELP_COLOR_ARG_01 " " \
        HELP_COLOR_ARG_02 "Weak:ice\n") \
    ROW(m09, m08.page.pageOffset, "hearts_009", \
        HELP_PREFIX "Imparts death spells\n" HELP_COLOR_ARG_03 "Null:death" HELP_COLOR_ARG_01 " " \
        HELP_COLOR_ARG_02 "Weak:expel\n") \
    ROW(m0A, m09.page.pageOffset, "hearts_00A", \
        HELP_PREFIX "Imparts ice spells\n" HELP_COLOR_ARG_03 "Null:ice" HELP_COLOR_ARG_01 " " \
        HELP_COLOR_ARG_02 "Weak:fire\n") \
    ROW(m0B, m0A.page.pageOffset, "hearts_00B", \
        HELP_PREFIX "Imparts expel spells\n" HELP_COLOR_ARG_03 "Null:expel" HELP_COLOR_ARG_01 " " \
        HELP_COLOR_ARG_02 "Weak:death\n") \
    ROW(m0C, m0B.page.pageOffset, "hearts_00C", \
        HELP_PREFIX "Imparts ailment-void skills\n" HELP_COLOR_ARG_03 "Resist:phys" HELP_COLOR_ARG_01 " " \
        HELP_COLOR_ARG_02 "Weak:fire/ice\n") \
    ROW(m0D, m0C.page.pageOffset, "hearts_00D", \
        HELP_PREFIX "Imparts healing spells\n" HELP_COLOR_ARG_03 "Null:expel\n") \
    ROW(m0E, m0D.page.pageOffset, "hearts_00E", \
        HELP_PREFIX "Imparts special spells\n" HELP_COLOR_ARG_03 "Null:curse\n" HELP_STYLE_ARG_02 \
        "(mute/poison)\n") \
    ROW(m0F, m0E.page.pageOffset, "hearts_00F", \
        HELP_PREFIX "Imparts ailment spells\n" HELP_COLOR_ARG_03 "Resist:ailments\n") \
    ROW(m10, m0F.page.pageOffset, "hearts_010", \
        HELP_PREFIX "Imparts fire spells\n" HELP_COLOR_ARG_03 "Drain:fire" HELP_COLOR_ARG_01 " " \
        HELP_COLOR_ARG_02 "Weak:ice\n") \
    ROW(m11, m10.page.pageOffset, "hearts_011", \
        HELP_PREFIX "Imparts physical attack skills\n" HELP_COLOR_ARG_03 "Resist:phys" HELP_COLOR_ARG_01 \
        " " HELP_COLOR_ARG_02 "Weak:magic\n") \
    ROW(m12, m11.page.pageOffset, "hearts_012", \
        HELP_PREFIX "Imparts special spells\n" HELP_COLOR_ARG_03 "Null:death" HELP_COLOR_ARG_01 " " \
        HELP_COLOR_ARG_02 "Weak:expel\n") \
    ROW(m13, m12.page.pageOffset, "hearts_013", \
        HELP_PREFIX "Imparts electrical spells\n" HELP_COLOR_ARG_03 "Repel:elec" HELP_COLOR_ARG_01 " " \
        HELP_COLOR_ARG_02 "Weak:force\n") \
    ROW(m14, m13.page.pageOffset, "hearts_014", \
        HELP_PREFIX "Imparts physical attack skills\n" HELP_COLOR_ARG_03 "Null:nerve\n" HELP_STYLE_ARG_02 \
        "(stun/bind)\n") \
    ROW(m15, m14.page.pageOffset, "hearts_015", \
        HELP_PREFIX "Imparts force spells\n" HELP_COLOR_ARG_03 "Drain:force" HELP_COLOR_ARG_01 " " \
        HELP_COLOR_ARG_02 "Weak:elec\n") \
    ROW(m16, m15.page.pageOffset, "hearts_016", \
        HELP_PREFIX "Imparts healing spells\n" HELP_COLOR_ARG_03 "Null:expel\n") \
    ROW(m17, m16.page.pageOffset, "hearts_017", \
        HELP_PREFIX "Imparts physical attack skills\n" HELP_COLOR_ARG_03 "Resist:phys" HELP_COLOR_ARG_01 \
        " " HELP_COLOR_ARG_02 "Weak:force/expel/death\n") \
    ROW(m18, m17.page.pageOffset, "hearts_018", \
        HELP_PREFIX "Imparts almighty spells\nNormal resistance\n") \
    ROW(m19, m18.page.pageOffset, "hearts_019", \
        HELP_PREFIX "Magatama that holds ultimate power\n" HELP_COLOR_ARG_03 \
        "Null:all except almighty\n") \
    ROW(m1A, m19.page.pageOffset, "hearts_01A", \
        HELP_PREFIX "Resist:fire\nImparts fire spells\n") \
    ROW(m1B, m1A.page.pageOffset, "hearts_01B", \
        HELP_PREFIX "Resist:fire\nImparts fire spells\n")

typedef struct {
    u8 label[24];
    u16 pageCount;
    s16 speaker;
    u32 pageOffset;
    u32 textBytes;
} frMagatamaHelpPage_t;

#define HELP_RECORD(member, previous, label, text) \
    struct { frMagatamaHelpPage_t page; u8 encoded[sizeof(text)]; } member;

/* Inner records use real word rounding; the outer image ends before the
 * seven separate zero bytes that precede the default command bank. */
typedef struct __attribute__((packed)) frMagatamaHelpBank_t {
    frTextBank_t header;
    frTextIndex_t entries[28];
    u32 speakerTableOffset;
    u32 speakerInfo;
    u32 additionalEntryOffset;
    u8 directoryZeroTail[4];
    HELP_PAGES(HELP_RECORD)
    u8 fixups[57];
} frMagatamaHelpBank_t;

#define HELP_AT(member) ((u32)&((frMagatamaHelpBank_t *)0)->member)
#define HELP_PAYLOAD_OFFSET(member) (HELP_AT(member) - HELP_AT(entries))
#define HELP_ENTRY(member, previous, label, text) \
    {0, HELP_PAYLOAD_OFFSET(member)},
#define HELP_PAGE(member, previous, label, text) \
    .member = {{label, 1, -1, HELP_PAYLOAD_OFFSET(member.encoded), sizeof(text)}, text},

/* Low-bit-clear commands encode the distance in words, doubled. */
#define HELP_DELTA(from, to) ((HELP_AT(to) - HELP_AT(from)) / 2)
#define HELP_PAGE_DELTA(member, previous, label, text) \
    HELP_DELTA(previous, member.page.pageOffset),
#define HELP_ENTRY_STRIDE (sizeof(frTextIndex_t) / 2)

typedef char frMagatamaHelpSize[sizeof(frMagatamaHelpBank_t) == 0xC71 ? 1 : -1];
typedef char frMagatamaHelpOffsets[HELP_AT(entries) == 0x20 &&
    HELP_AT(m00) == 0x110 && HELP_AT(m1B) == 0xBE8 &&
    HELP_AT(fixups) == 0xC38 ? 1 : -1];

/* The complete byte image contains real aligned offset words. */
frMagatamaHelpBank_t itfMesEmbedded_3F7350 __attribute__((section(".data"), aligned(8))) = {
    .header = {
        7, sizeof(frMagatamaHelpBank_t), 0x3147534D, 0,
        HELP_AT(fixups), 57, 28, 0, {0, 2, 0}
    },
    .entries = {
        HELP_PAGES(HELP_ENTRY)
    },
    .speakerTableOffset = HELP_PAYLOAD_OFFSET(fixups),
    HELP_PAGES(HELP_PAGE)
    .fixups = {
        HELP_DELTA(entries[0], entries[0].offset),
        /* 27 transitions between successive entry offset words. */
        HELP_ENTRY_STRIDE, HELP_ENTRY_STRIDE, HELP_ENTRY_STRIDE,
        HELP_ENTRY_STRIDE, HELP_ENTRY_STRIDE, HELP_ENTRY_STRIDE,
        HELP_ENTRY_STRIDE, HELP_ENTRY_STRIDE, HELP_ENTRY_STRIDE,
        HELP_ENTRY_STRIDE, HELP_ENTRY_STRIDE, HELP_ENTRY_STRIDE,
        HELP_ENTRY_STRIDE, HELP_ENTRY_STRIDE, HELP_ENTRY_STRIDE,
        HELP_ENTRY_STRIDE, HELP_ENTRY_STRIDE, HELP_ENTRY_STRIDE,
        HELP_ENTRY_STRIDE, HELP_ENTRY_STRIDE, HELP_ENTRY_STRIDE,
        HELP_ENTRY_STRIDE, HELP_ENTRY_STRIDE, HELP_ENTRY_STRIDE,
        HELP_ENTRY_STRIDE, HELP_ENTRY_STRIDE, HELP_ENTRY_STRIDE,
        HELP_DELTA(entries[27].offset, speakerTableOffset),
        HELP_PAGES(HELP_PAGE_DELTA)
    }
};
