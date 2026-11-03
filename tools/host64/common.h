#ifndef COMMON_H
#define COMMON_H

/* Host view of include/common.h, force-included ahead of it so that file's
 * own guard skips its body. A quoted #include "common.h" from inside the
 * decompilation's include directory finds its sibling first, so -I order
 * alone cannot replace it.
 *
 * This is the 64-bit host probe's copy: tools/host64_probe.py force-includes
 * it to ask whether the engine's C is acceptable to an LP64 compiler. It is
 * not part of the retail build and nothing in config/ references it. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
/* x86-64 gcc and clang both emulate TImode, so the retail spelling carries. */
typedef int s128 __attribute__((mode(TI)));
typedef unsigned int u128 __attribute__((mode(TI)));

typedef float f32;
typedef double f64;

typedef __SIZE_TYPE__ size_t;

#define QWORD_ALIGNED __attribute__((aligned(16)))
#define QUAD_ZERO(lvalue) __builtin_memset(&(lvalue), 0, sizeof(lvalue))

/* An EE address in a host-pointer-wide integer. The retail expansion is
 * ((u32)(ptr)); here the width follows the host pointer so no address is
 * truncated, and the two stay inverse to each other.
 *
 * The spelling is load-bearing for the probe's trunc mode, which recognizes a
 * truncated address by the source type gcc names. unsigned long is how LP64
 * spells size_t and the type of every sizeof, so this must not be that. */
typedef unsigned long long ee_addr_t;
#define EE_ADDR(ptr) ((ee_addr_t)(__UINTPTR_TYPE__)(ptr))
#define EE_ADDR_PTR(addr) ((void *)(__UINTPTR_TYPE__)(addr))
#define EE_ADDR_S(ptr) ((ee_addr_t)(__UINTPTR_TYPE__)(ptr))
#define EE_ADDR_AS(type, addr) ((type)(__UINTPTR_TYPE__)(addr))

/* Host view of the guest-address types. This header stands in front of
 * include/common.h's guard, so the typedefs are repeated here rather than
 * reached through ADDR_TYPE; a consumer that includes the real header
 * defines that macro instead and gets the same two types. Pointer-wide here,
 * which is the whole point: the decompilation spells these where an address
 * is stored and the retail expansion is u32, so nothing moves on target.
 * The unsigned one is ee_addr_t itself, so an EE_ADDR result lands in it with
 * no conversion, which is also what makes the probe's trunc mode stop
 * counting the site. */
typedef ee_addr_t addr_t;
typedef long long addrS_t;

#define SYMBOL_AT(type, name, base, offset) extern type name

/* The region form, which include/common.h gained after this header was
 * written. The alias is dropped here for the reason that file's own comment
 * gives, so the host has to supply the symbol itself; the declaration alone
 * is what the probe needs, since it only asks whether the unit compiles. */
#define SYMBOL_AT_REGION(type, name, base, offset) extern type name[]

/* Repeated from include/common.h because this header stands in front of that
 * one's guard: anything that file gains has to be carried here too. The
 * expansion is the same, and on LP64 the first term makes it vacuous. */
#define SIZE_CHECK(name, type, size) \
    typedef char name[(sizeof(void *) != 4 || sizeof(type) == (size)) ? 1 : -1]

#define FAKEMATCH_ASM_NAME(symbol) __asm__(symbol)
#define FAKEMATCH_ASM_REG(type, name, reg) type name
#define FAKEMATCH_ASM_LIVE(value) ((void)(value))
#define FAKEMATCH_ASM_LIVE_PAIR(first, second) ((void)(first), (void)(second))
#define FAKEMATCH_ASM_LIVE_F32(value) ((void)(value))
#define FAKEMATCH_ASM_LIVE_VOLATILE(value) ((void)(value))
#define FAKEMATCH_ASM_LIVE_F32_VOLATILE(value) ((void)(value))
#define FAKEMATCH_ASM_READ_PAIR(first, second) ((void)(first), (void)(second))
#define FAKEMATCH_ASM_LIVE_MEM(lvalue) ((void)(lvalue))
#define FAKEMATCH_ASM_READ_MEM(lvalue) ((void)(lvalue))

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

/* ee-gcc 2.96's spelling of the stdarg builtin, which a few units call by
 * name in their own va_start macro. */
#define __builtin_stdarg_start __builtin_va_start

#include "include_asm.h"

#endif /* COMMON_H */
