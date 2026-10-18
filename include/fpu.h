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

#endif /* FPU_H */
