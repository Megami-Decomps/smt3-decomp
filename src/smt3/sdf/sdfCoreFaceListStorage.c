#include "common.h"

/* The two native lists occupy 0xC000 and 0x2000 bytes, respectively. */
u128 *corePktFaceList[4096][3] __attribute__((section(".bss"), aligned(16)));
u16 corePktBackFaceList[4096] __attribute__((section(".bss"), aligned(16)));

SIZE_CHECK(corePktFaceListRowSize, u128 *[3], 12);
SIZE_CHECK(corePktBackFaceIndexSize, u16, 2);
