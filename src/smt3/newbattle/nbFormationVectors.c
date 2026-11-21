#include "nbFormationVectors.h"

nbFormationVector_t nbFormPartyRot = { 0.0f, 0.0f, 0.0f, 1.0f };
nbFormationVector_t nbFormEnemyRot = { 0.0f, 1.0f, 0.0f, 0.0f };

/* Native default position belongs to the file-backed initialized vectors. */
nbFormationVector_t nbFormDefaultPos = {
    0.0f, 0.0f, 0.0f, 0.0f,
};
nbFormationVector_t nbFormDefaultRot = { 0.0f, 0.0f, 0.0f, 1.0f };
