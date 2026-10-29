#ifndef SDF_PACKET_LIST_H
#define SDF_PACKET_LIST_H

#include "libsdf.h"

typedef struct sdfGrRefPacketHeader_t sdfGrRefPacketHeader_t;
void sdfGrLinkDispFrameTex(sdfDrawTag_t *list, u32 source, sdfGrRefPacketHeader_t *packet);

#endif /* SDF_PACKET_LIST_H */
