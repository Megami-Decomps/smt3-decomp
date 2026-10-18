#ifndef COMMON_H
#define COMMON_H

/* ee-gcc: int/pointers are 32-bit, long long is 64-bit, TImode is 128-bit. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int s128 __attribute__((mode(TI)));
typedef unsigned int u128 __attribute__((mode(TI)));

/* Marks a DMA tag or packet type: the DMAC reads it at a quadword (16-byte)
 * boundary. The PS2 layouts already place these types there, so this build
 * adds nothing. A build whose layouts differ (8-byte pointers, say) defines
 * QWORD_ALIGNED as __attribute__((aligned(16))). */
#ifndef QWORD_ALIGNED
#define QWORD_ALIGNED
#endif
/* Stores a zero quadword. ee-gcc materialises the 128-bit zero in a
 * register and stores that, which is what retail does, so the operation stays
 * written as an assignment here. A build with no 128-bit integer type defines
 * this as a 16-byte clear instead. */
#ifndef QUAD_ZERO
#define QUAD_ZERO(lvalue) ((lvalue) = 0)
#endif

typedef float f32;
typedef double f64;

/* Binds NAME as an assembler alias for BASE + OFFSET, for a view into
 * another object that has no symbol of its own. The assembler does the
 * arithmetic, so nothing is allocated and the reference stays gp-relative.
 * A build whose assembler cannot resolve the expression, which is every
 * target but the EE, defines this differently. */
#ifndef PS2_SYMBOL_AT
#define PS2_SYMBOL_AT(type, name, base, offset) \
    extern type name; \
    __asm__(#name " = " #base " + " #offset)
#endif

typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;

#ifndef NULL
#define NULL ((void *)0)
#endif

#define TRUE 1
#define FALSE 0

#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))

#include "include_asm.h"

#endif /* COMMON_H */
