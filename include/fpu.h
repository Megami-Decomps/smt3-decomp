#ifndef FPU_H
#define FPU_H

/* DDS's scalar FPU convention: ee-gcc 2.96 normally calls the errno-checking
 * sqrtf library entry. Even with -fno-math-errno its sqrt.s has hazard nops
 * absent from retail. Keep the bare instruction shared, rather than adding
 * per-unit wrappers. See DDS docs/idioms.md, "Square roots: sqrt.s".
 * SMT3 users include effPCPBlurScreenOffset and the quaternion routines. */
static inline float fsqrtf(float x)
{
    float result;
    __asm__(".set noreorder\n\tsqrt.s %0, %1\n\t.set reorder"
            : "=f" (result) : "f" (x));
    return result;
}

/* EE numerator / sqrt(x): gcc does not combine a square root and division
 * into rsqrt.s. SMT3 LookAt uses this operation, as do the DDS quaternion
 * normalizers. */
static inline float frsqrtf(float numerator, float x)
{
    float result;
    __asm__(".set noreorder\n\trsqrt.s %0, %1, %2\n\t.set reorder"
            : "=f" (result) : "f" (numerator), "f" (x));
    return result;
}

/* Preserve the SDK inline-asm scheduling boundary around abs.s. Independent
 * constant loads and argument moves may cross builtin fabsf calls, but stay
 * ordered around this helper. */
static inline float ffabsf(float x)
{
    float result;
    __asm__(".set noreorder\n\tabs.s %0, %1\n\t.set reorder"
            : "=f" (result) : "f" (x));
    return result;
}

/* Convert using the EE FPU's current rounding mode. */
static inline int fcvtws(float value)
{
    float bits;
    int result;
    __asm__ volatile ("cvt.w.s %0, %1" : "=f" (bits) : "f" (value));
    __asm__ volatile ("mfc1 %0, %1" : "=r" (result) : "f" (bits));
    return result;
}

#endif /* FPU_H */
