#ifndef NB_FORMATION_VECTORS_H
#define NB_FORMATION_VECTORS_H

#include "sdf_types.h"

/* Each source is read by the formation's aligned quadword copy. */
typedef sdfVector4_t nbFormationVector_t __attribute__((aligned(16)));

extern nbFormationVector_t nbFormPartyRot;
extern nbFormationVector_t nbFormEnemyRot;
extern nbFormationVector_t nbFormDefaultPos;
extern nbFormationVector_t nbFormDefaultRot;

#endif
