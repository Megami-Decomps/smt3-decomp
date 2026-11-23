#include "sdf_types.h"

/* BSNET endpoint and shot interpolation preserve the complete VU lanes. */
u128 Bsnet_E907_2_EndTarget __attribute__((section(".bss"), aligned(16)));
u128 Bsnet_E907_2_EndDirection __attribute__((section(".bss"), aligned(16)));
u128 nbCameraBsnetE907ShotTarget __attribute__((section(".bss"), aligned(16)));
u128 nbCameraBsnetE907ShotDirection __attribute__((section(".bss"), aligned(16)));
sdfVector4_t nbCameraBSARICut1Vec __attribute__((section(".bss"), aligned(16)));
sdfVector4_t nbCameraKaguVec2 __attribute__((section(".bss"), aligned(16)));
sdfVector4_t nbCameraKaguVec __attribute__((section(".bss"), aligned(16)));
