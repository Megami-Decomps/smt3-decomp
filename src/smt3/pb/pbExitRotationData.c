#include "libsdf.h"

/* Shared writable rotation; its native location is only eight-byte aligned. */
sdfVector4Unaligned_t pbExitRotation __attribute__((section(".data"))) = {
    0.0f, 0.0f, 0.0f, 0.0f,
};
