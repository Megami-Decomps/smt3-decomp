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

/* A pointer in the EE's 32-bit address representation, which is how this
 * engine passes a packet or buffer address through a u32 field or argument.
 * It lives here rather than in eeregs.h because 60 units need it and almost
 * every unit already includes this header. On target the cast is the whole
 * of it; a build with wider pointers redefines both of these and must keep
 * them inverse to each other. */
#ifndef EE_ADDR
#define EE_ADDR(ptr) ((u32)(ptr))
#endif
/* The same address back as a pointer. The (u32) is kept because the sites
 * spell it: dropping it would change the target conversion, not only the
 * host one. */
#ifndef EE_ADDR_PTR
#define EE_ADDR_PTR(addr) ((void *)(u32)(addr))
#endif

typedef float f32;
typedef double f64;

/* The EE's size_t. ee-gcc is invoked with -D__SIZE_TYPE__=unsigned int, so
 * this is a 32-bit unsigned here; on a target whose size_t is wider it
 * follows that compiler. The C library declarations in this tree spell their
 * length parameter with it, the one width that differs between targets with
 * no cast at the call site to mark it. */
typedef __SIZE_TYPE__ ps2Size_t;

/* Asserts a structure's retail size, by the negative-array-size trick ee-gcc
 * 2.96 accepts. The typedef emits nothing, so this costs no code. The check
 * applies only where a pointer is four bytes: a wider pointer moves every
 * member after one, so the retail size is the wrong question there and the
 * members' own types stay the record of the layout. */
#ifndef PS2_SIZE_CHECK
#define PS2_SIZE_CHECK(name, type, size) \
    typedef char name[(sizeof(void *) != 4 || sizeof(type) == (size)) ? 1 : -1]
#endif


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

/* Localized compiler constraints on a real live value. None of them emits an
 * instruction: the binding only fixes which register cc1 keeps the value in,
 * and the empty template only forces it into a register, or into its storage,
 * at that point. docs/CONTRIBUTING.md says when these are permitted. */
/* Declares NAME while the symbol it binds to keeps its retail name, for a
 * second view of one object or a definition the linker must see under another
 * name. It emits nothing: the label only names the symbol. */
#ifndef FAKEMATCH_ASM_NAME
#define FAKEMATCH_ASM_NAME(symbol) __asm__(symbol)
#endif
#ifndef FAKEMATCH_ASM_REG
#define FAKEMATCH_ASM_REG(type, name, reg) register type name __asm__(reg)
#endif
#ifndef FAKEMATCH_ASM_LIVE
#define FAKEMATCH_ASM_LIVE(value) __asm__("" : "+r" (value))
#endif
#ifndef FAKEMATCH_ASM_LIVE_PAIR
#define FAKEMATCH_ASM_LIVE_PAIR(first, second) __asm__("" : "+r" (first), "+r" (second))
#endif
#ifndef FAKEMATCH_ASM_LIVE_F32
#define FAKEMATCH_ASM_LIVE_F32(value) __asm__("" : "+f" (value))
#endif
/* The _VOLATILE forms hold the constraint where cc1 would otherwise drop it. */
#ifndef FAKEMATCH_ASM_LIVE_VOLATILE
#define FAKEMATCH_ASM_LIVE_VOLATILE(value) __asm__ volatile ("" : "+r" (value))
#endif
#ifndef FAKEMATCH_ASM_LIVE_F32_VOLATILE
#define FAKEMATCH_ASM_LIVE_F32_VOLATILE(value) __asm__ volatile ("" : "+f" (value))
#endif
#ifndef FAKEMATCH_ASM_READ_PAIR
#define FAKEMATCH_ASM_READ_PAIR(first, second) __asm__("" : : "r" (first), "r" (second))
#endif
#ifndef FAKEMATCH_ASM_LIVE_MEM
#define FAKEMATCH_ASM_LIVE_MEM(lvalue) __asm__ volatile ("" : "+m" (lvalue))
#endif
#ifndef FAKEMATCH_ASM_READ_MEM
#define FAKEMATCH_ASM_READ_MEM(lvalue) __asm__ volatile ("" : : "m" (lvalue))
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
