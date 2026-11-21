#include "sdf_types.h"

/* The initial automap target, eye, and up vectors, loaded by VU0. */
sdfVector4_t fldAutoMapRect __attribute__((aligned(16))) = { 0.0f, -100.0f, 0.0f, 0.0f };
sdfVector4_t fldAutoMapEye __attribute__((aligned(16))) = { 0.0f, -100.0f, -400.0f, 0.0f };
sdfVector4_t fldAutoMapUp __attribute__((aligned(16))) = { 0.0f, 1.0f, 0.0f, 0.0f };
