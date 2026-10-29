#ifndef DDS3OBJ_H
#define DDS3OBJ_H

#include "basic.h"

/* Native pose-setter interfaces; implementation in basic/dds3Basic.c. */
void dds3SetPosPoseSet(dds3Basic_t *basic, const sdfVector4_t *position);
void dds3SetRotPoseSet(dds3Basic_t *basic, const sdfVector4_t *rotation);

#endif /* DDS3OBJ_H */
