#ifndef IRX_H
#define IRX_H

/* Shared declarations for the three IOP modules in USERIRX/.
 * The build is described in docs/irx-modules.md. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef short s16;
typedef int s32;
typedef unsigned long long u64;
typedef long long s64;

typedef char *va_list;
#define va_start(ap, last) ((ap) = (va_list)__builtin_next_arg(last))
#define va_end(ap) ((void)0)

#ifndef NULL
#define NULL 0
#endif

#ifndef ASM_ROOT
#error "ASM_ROOT must be defined by the build (see tools/irx_configure.py)"
#endif

/* Pull a chunk of .text that is still assembly. The chunks appear in the C
 * file in link order; replacing one with a C function keeps that order. */
#define INCLUDE_ASM(MODULE, NAME)                              \
    __asm__(".section .text\n"                                 \
            "\t.set noat\n"                                    \
            "\t.set noreorder\n"                               \
            "\t.include \"" ASM_ROOT MODULE "/text/" #NAME ".s\"\n" \
            "\t.set reorder\n"                                 \
            "\t.set at\n")

/* A function whose C is equivalent but does not reproduce the retail
 * instructions sits under #ifdef NON_MATCHING, with the INCLUDE_ASM the
 * default build uses in the #else. */

#endif /* IRX_H */
