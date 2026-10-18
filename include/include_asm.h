#ifndef INCLUDE_ASM_H
#define INCLUDE_ASM_H

/*
 * INCLUDE_ASM(const s32, "unit/path", func) pulls a not-yet-decompiled function
 * from the split assembly into the C translation unit, so the object keeps the
 * original function order. ee-gcc 2.96 emits top-level asm in source order (no
 * unit-at-a-time), and the same ee-as assembles the compiler output and the
 * included body.
 *
 * ee-as cannot read spimdisasm's syntax directly, so configure.py rewrites each
 * body into build/eeasm/ (tools/eeas_compat.py); ASM_ROOT is that tree for the
 * version being compiled (e.g. "build/eeasm/asm/smt3/nonmatchings/").
 *
 * M2CTX/PERMUTER (context and permuter builds) and SKIP_ASM (objdiff progress
 * objects: an asm fallback must not count as matched code) drop the asm.
 *
 * A function whose C is close but not exact is kept beside its fallback:
 *
 *     \/* NON_MATCHING: 12 of 694 words. Three reload spill slots are
 *      * rotated ... Ruled out: ... *\/
 *     #ifdef NON_MATCHING
 *     void f(void) { ... }
 *     #else
 *     INCLUDE_ASM(const s32, "unit/path", f);
 *     #endif
 *
 * The normal build never defines NON_MATCHING, so it takes the asm and stays
 * byte-identical, and the function still counts as unmatched. The draft is
 * there for the next person instead of being thrown away, and
 * tools/check_nonmatching.py compiles it so it cannot rot unnoticed.
 *
 * The comment is the point of keeping it. It must say how close the draft is,
 * what still differs, and which source shapes have been ruled out, or the next
 * person repeats the search. tools/revert_differing.py writes the measured
 * part and leaves a TODO for the rest.
 */
#if !defined(M2CTX) && !defined(PERMUTER) && !defined(SKIP_ASM)

#ifndef ASM_ROOT
#error "ASM_ROOT must be defined by the build (see configure.py)"
#endif

#define INCLUDE_ASM(TYPE, FOLDER, NAME)                        \
    __asm__(".section .text\n"                                 \
            "\t.set noat\n"                                    \
            "\t.set noreorder\n"                               \
            "\t.include \"" ASM_ROOT FOLDER "/" #NAME ".s\"\n" \
            "\t.set reorder\n"                                 \
            "\t.set at\n")

#define INCLUDE_RODATA(TYPE, FOLDER, NAME)                     \
    __asm__(".section .rodata\n"                               \
            "\t.include \"" ASM_ROOT FOLDER "/" #NAME ".s\"\n" \
            ".section .text\n")

/* Small data not yet defined in C (tools/include_sdata.py places these). */
#define INCLUDE_SDATA(TYPE, FOLDER, NAME)                      \
    __asm__(".section .sdata\n"                                \
            "\t.include \"" ASM_ROOT FOLDER "/" #NAME ".s\"\n" \
            ".section .text\n")

__asm__(".include \"macro.inc\"\n");

#else

#define INCLUDE_ASM(TYPE, FOLDER, NAME)
#define INCLUDE_RODATA(TYPE, FOLDER, NAME)
#define INCLUDE_SDATA(TYPE, FOLDER, NAME)

#endif

#endif /* INCLUDE_ASM_H */
