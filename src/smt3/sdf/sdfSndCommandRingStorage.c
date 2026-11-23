#include "sdfSndCommandRingStorage.h"

SIZE_CHECK(sdfSndCommandRingPacketSize, sndRingPacket_t, 0x80);

/* The producer and SIF transport advance their indices modulo 32. */
sndRingPacket_t sdfSndCmdPacketRing[32] __attribute__((section(".bss"), aligned(16)));
