#ifndef SDF_TM_PACKET_H
#define SDF_TM_PACKET_H

#include "sdf_types.h"

/* One object's transform for VU1: the world matrix, then the three rows
 * sdf3DPrimSetTmPacket derives for normals, then the VIF codes that run the
 * transform. sdfModelNode_t.tmPacket is two of these, one per frame buffer. */
typedef struct QWORD_ALIGNED sdf3DTmPacketData_t
{
    sdfVector4_t tm[7];
    u32 cmd[4];
} sdf3DTmPacketData_t;

extern sdf3DTmPacketData_t sdf3DPrimIdentityTm;

#endif
