#include "frTextBank.h"

/* Text is a font command prefix followed by the original English lines. */
#define CONFIG_TEXT_PREFIX "\xF2\x08\xFF\xFF\xF2\x06\x02\xFF"
#define CONFIG_VIBRATION_TEXT CONFIG_TEXT_PREFIX \
    "The controller's vibration function\ncan be turned on/off.\n"
#define CONFIG_SELECTION_TEXT CONFIG_TEXT_PREFIX \
    "The last selection made will be\nhighlighted first.(only in the FIGHT\nmenu during battle)\n"
#define CONFIG_AUTO_TEXT CONFIG_TEXT_PREFIX \
    "Your last action will be repeatedly\nperformed when \"Auto\" is selected in\nbattle.\n"

typedef struct {
    u8 label[24];
    u16 pageCount;
    s16 speaker;
    u32 pageOffset;
    u32 textBytes;
} frConfigPage_t;

#define CONFIG_TEXT_RECORD(name, text) \
    struct { frConfigPage_t page; u8 encoded[sizeof(text)]; } name

/* Page-record padding is the format's real word alignment. Pack only
 * the full container, so its odd final length leaves the outer pad raw. */
typedef struct __attribute__((packed)) frConfigTextBank_t {
    frTextBank_t header;
    frTextIndex_t entries[3];
    u32 speakerTableOffset;
    u32 speakerInfo;
    u32 additionalEntryOffset;
    u8 directoryZeroTail[4];
    CONFIG_TEXT_RECORD(vibration, CONFIG_VIBRATION_TEXT);
    CONFIG_TEXT_RECORD(selection, CONFIG_SELECTION_TEXT);
    CONFIG_TEXT_RECORD(autoAction, CONFIG_AUTO_TEXT);
    u8 fixups[7];
} frConfigTextBank_t;

#define CONFIG_AT(member) ((u32)&((frConfigTextBank_t *)0)->member)
#define CONFIG_PAYLOAD_OFFSET(member) (CONFIG_AT(member) - CONFIG_AT(entries))
/* sdfAddressResolver low-bit-clear command: a delta in words, doubled. */
#define CONFIG_FIXUP_DELTA(from, to) ((CONFIG_AT(to) - CONFIG_AT(from)) / 2)

typedef char configBankSize[sizeof(frConfigTextBank_t) == 0x1BF ? 1 : -1];
typedef char configBankOffsets[CONFIG_AT(entries) == 0x20 &&
    CONFIG_AT(vibration) == 0x48 && CONFIG_AT(selection) == 0xB0 &&
    CONFIG_AT(autoAction) == 0x138 && CONFIG_AT(fixups) == 0x1B8 ? 1 : -1];

/* The packed image has aligned offset words that the reader loads directly. */
frConfigTextBank_t itfMesEmbedded_528D68 __attribute__((section(".data"), aligned(8))) = {
    .header = {
        7, sizeof(frConfigTextBank_t), 0x3147534D, 0,
        CONFIG_AT(fixups), 7, 3, 0, {0, 2, 0}
    },
    .entries = {
        {0, CONFIG_PAYLOAD_OFFSET(vibration)},
        {0, CONFIG_PAYLOAD_OFFSET(selection)},
        {0, CONFIG_PAYLOAD_OFFSET(autoAction)}
    },
    .speakerTableOffset = CONFIG_PAYLOAD_OFFSET(fixups),
    .vibration = {
        {"msg_02", 1, -1, CONFIG_PAYLOAD_OFFSET(vibration.encoded), sizeof(CONFIG_VIBRATION_TEXT)},
        CONFIG_VIBRATION_TEXT
    },
    .selection = {
        {"msg_03", 1, -1, CONFIG_PAYLOAD_OFFSET(selection.encoded), sizeof(CONFIG_SELECTION_TEXT)},
        CONFIG_SELECTION_TEXT
    },
    .autoAction = {
        {"msg_04", 1, -1, CONFIG_PAYLOAD_OFFSET(autoAction.encoded), sizeof(CONFIG_AUTO_TEXT)},
        CONFIG_AUTO_TEXT
    },
    .fixups = {
        CONFIG_FIXUP_DELTA(entries[0], entries[0].offset),
        CONFIG_FIXUP_DELTA(entries[0].offset, entries[1].offset),
        CONFIG_FIXUP_DELTA(entries[1].offset, entries[2].offset),
        CONFIG_FIXUP_DELTA(entries[2].offset, speakerTableOffset),
        CONFIG_FIXUP_DELTA(speakerTableOffset, vibration.page.pageOffset),
        CONFIG_FIXUP_DELTA(vibration.page.pageOffset, selection.page.pageOffset),
        CONFIG_FIXUP_DELTA(selection.page.pageOffset, autoAction.page.pageOffset)
    }
};
