#include "cmp_status_model_vectors.h"

/* Both camera vectors are sources of aligned EE quadword copies. */
sdfVector4_t cmpStatusModelPos __attribute__((aligned(16))) = { 0.0f, 0.0f, 0.0f, 0.0f };
sdfVector4_t cmpStatusModelBasePos __attribute__((aligned(16))) = { 0.0f, 0.0f, -400.0f, 0.0f };
