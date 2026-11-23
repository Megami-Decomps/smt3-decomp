#ifndef SDF_IMAGE_SET_CB_STORAGE_H
#define SDF_IMAGE_SET_CB_STORAGE_H

#include "libsdf.h"

typedef struct sdfImageSetRelease
{
    struct sdfImageSetRelease *next;
    void *chip;
    sdfMemHandle_t *memHandle;
    u8 freeMode;
} sdfImageSetPacket_tag;

/* Terminal DMA tag of a queued upload packet. Linking the next packet writes
 * its address into tag and clears the two VIF codes behind it. */
typedef struct sdfImageSetTag
{
    u64 tag;
    u64 vifCodes;
} sdfImageSetTag_t;

typedef struct sdfImageSetCB
{
    s32 semaId;
    sdfImageSetPacket_tag *packetHead;
    sdfImageSetPacket_tag *packetTail;
    addr_t tagHead;
    sdfImageSetTag_t *tagTail;
} sdfImageSetCb_t;

extern sdfImageSetCb_t sdfImageSetCb;

#endif
