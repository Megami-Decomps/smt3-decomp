#include "frTextBank.h"

#define HELP_COMMANDS "\xF2\x08\xFF\xFF\xF2\x06\x02\xFF"
#define HELP_RESERVED HELP_COMMANDS "(reserve)\n"
#define HELP_ATTACK HELP_COMMANDS "Physical attack (low dmg)\n"
#define HELP_PASS HELP_COMMANDS "Skip action at 1/2 turn cost\n"
#define HELP_TALK HELP_COMMANDS "Ask a demon to join your side\n"
#define HELP_RETREAT HELP_COMMANDS "Retreat from battle\n"
#define HELP_DISMISS HELP_COMMANDS "Remove from active party\n"
#define HELP_FILLER HELP_COMMANDS "FILLER-AMH file\n"

typedef struct {
    u8 label[24];
    u16 pageCount;
    s16 speaker;
    u32 pageOffset;
    u32 textBytes;
} nbDefaultHelpPage_t;

#define HELP_RECORD(name, text) \
    struct { nbDefaultHelpPage_t page; u8 encoded[sizeof(text)]; } name

/* The native record slots are word-rounded; the exact full image is not.
 * Keep the unrelated five-byte seam outside this packed container. */
typedef struct __attribute__((packed)) nbDefaultCommandHelpBank_t {
    frTextBank_t header;
    frTextIndex_t entries[7];
    u32 speakerTableOffset;
    u32 speakerInfo;
    u32 additionalEntryOffset;
    u8 directoryZeroTail[4];
    HELP_RECORD(reserved, HELP_RESERVED);
    HELP_RECORD(attack, HELP_ATTACK);
    HELP_RECORD(pass, HELP_PASS);
    HELP_RECORD(talk, HELP_TALK);
    HELP_RECORD(retreat, HELP_RETREAT);
    HELP_RECORD(dismiss, HELP_DISMISS);
    HELP_RECORD(filler, HELP_FILLER);
    u8 fixups[15];
} nbDefaultCommandHelpBank_t;

#define HELP_AT(member) ((u32)&((nbDefaultCommandHelpBank_t *)0)->member)
#define HELP_PAYLOAD_OFFSET(member) (HELP_AT(member) - HELP_AT(entries))
#define HELP_PAGE(name, label, text) \
    { {label, 1, -1, HELP_PAYLOAD_OFFSET(name.encoded), sizeof(text)}, text }
/* sdfAddressResolver interprets low-bit-clear commands as doubled word deltas. */
#define HELP_FIXUP_DELTA(from, to) ((HELP_AT(to) - HELP_AT(from)) / 2)

typedef char nbDefaultHelpSize[sizeof(nbDefaultCommandHelpBank_t) == 0x25B ? 1 : -1];
typedef char nbDefaultHelpOffsets[HELP_AT(entries) == 0x20 &&
    HELP_AT(reserved) == 0x68 && HELP_AT(attack) == 0xA0 &&
    HELP_AT(pass) == 0xE8 && HELP_AT(talk) == 0x134 &&
    HELP_AT(retreat) == 0x180 && HELP_AT(dismiss) == 0x1C4 &&
    HELP_AT(filler) == 0x20C && HELP_AT(fixups) == 0x24C ? 1 : -1];

/* Word-aligned serialized directory offsets are loaded by MessageNumSearch. */
nbDefaultCommandHelpBank_t itfMesEmbedded_3F7FC8 __attribute__((section(".data"), aligned(8))) = {
    .header = {
        7, sizeof(nbDefaultCommandHelpBank_t), 0x3147534D, 0,
        HELP_AT(fixups), 15, 7, 0, {0, 2, 0}
    },
    .entries = {
        {0, HELP_PAYLOAD_OFFSET(reserved)},
        {0, HELP_PAYLOAD_OFFSET(attack)},
        {0, HELP_PAYLOAD_OFFSET(pass)},
        {0, HELP_PAYLOAD_OFFSET(talk)},
        {0, HELP_PAYLOAD_OFFSET(retreat)},
        {0, HELP_PAYLOAD_OFFSET(dismiss)},
        {0, HELP_PAYLOAD_OFFSET(filler)}
    },
    .speakerTableOffset = HELP_PAYLOAD_OFFSET(fixups),
    .reserved = HELP_PAGE(reserved, "DEFCOM_00", HELP_RESERVED),
    .attack = HELP_PAGE(attack, "DEFCOM_01", HELP_ATTACK),
    .pass = HELP_PAGE(pass, "DEFCOM_02", HELP_PASS),
    .talk = HELP_PAGE(talk, "DEFCOM_03", HELP_TALK),
    .retreat = HELP_PAGE(retreat, "DEFCOM_04", HELP_RETREAT),
    .dismiss = HELP_PAGE(dismiss, "DEFCOM_05", HELP_DISMISS),
    .filler = HELP_PAGE(filler, "ACTMISC_DVNAME", HELP_FILLER),
    .fixups = {
        HELP_FIXUP_DELTA(entries[0], entries[0].offset),
        HELP_FIXUP_DELTA(entries[0].offset, entries[1].offset),
        HELP_FIXUP_DELTA(entries[1].offset, entries[2].offset),
        HELP_FIXUP_DELTA(entries[2].offset, entries[3].offset),
        HELP_FIXUP_DELTA(entries[3].offset, entries[4].offset),
        HELP_FIXUP_DELTA(entries[4].offset, entries[5].offset),
        HELP_FIXUP_DELTA(entries[5].offset, entries[6].offset),
        HELP_FIXUP_DELTA(entries[6].offset, speakerTableOffset),
        HELP_FIXUP_DELTA(speakerTableOffset, reserved.page.pageOffset),
        HELP_FIXUP_DELTA(reserved.page.pageOffset, attack.page.pageOffset),
        HELP_FIXUP_DELTA(attack.page.pageOffset, pass.page.pageOffset),
        HELP_FIXUP_DELTA(pass.page.pageOffset, talk.page.pageOffset),
        HELP_FIXUP_DELTA(talk.page.pageOffset, retreat.page.pageOffset),
        HELP_FIXUP_DELTA(retreat.page.pageOffset, dismiss.page.pageOffset),
        HELP_FIXUP_DELTA(dismiss.page.pageOffset, filler.page.pageOffset)
    }
};
