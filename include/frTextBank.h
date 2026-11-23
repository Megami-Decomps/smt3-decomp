#ifndef FR_TEXT_BANK_H
#define FR_TEXT_BANK_H

#include "sdf_types.h"

/* Fixed serialized header, not the extent of an entire message bank.
 * fixupOffset is image-relative; entry and page offsets are relative
 * to the start of the directory. */
typedef struct frTextBank_t {
    u32 word00;
    u32 byteLength;
    u32 magic;
    u32 word0C;
    s32 fixupOffset;
    s32 fixupBytes;
    s32 count;
    u8 relocated;
    u8 formatFlags[3];
} frTextBank_t;

typedef struct frTextIndex_t {
    u32 kind;
    u32 offset;
} frTextIndex_t;

typedef char frTextBankHeaderSize[sizeof(frTextBank_t) == 0x20 ? 1 : -1];
typedef char frTextIndexSize[sizeof(frTextIndex_t) == 8 ? 1 : -1];

#endif
