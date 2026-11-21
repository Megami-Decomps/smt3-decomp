#include "common.h"

/* The board's camera startup fills these file-backed vectors after loading
 * textures. Camera setup reads aligned quadwords from them. */
f32 pbGameCameraParam[3][4] __attribute__((aligned(16))) = { 0 };
f32 pbCameraVec[3][4] __attribute__((aligned(16))) = { 0 };
