#ifndef SDF_SND_COMMAND_RING_STORAGE_H
#define SDF_SND_COMMAND_RING_STORAGE_H

#include "common.h"

typedef struct sndRingPacket_t {
    u32 transportHeader[3];
    u32 command;
    u32 sequence;
    u8 payload[108];
} sndRingPacket_t;

extern sndRingPacket_t sdfSndCmdPacketRing[32];

#endif
