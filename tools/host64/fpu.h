#ifndef FPU_H
#define FPU_H

/* Host view of include/fpu.h: the four EE FPU instructions it wraps in inline
 * asm, as plain C. */
#include <math.h>

static inline float fsqrtf(float x) { return sqrtf(x); }
static inline float frsqrtf(float numerator, float x) { return numerator / sqrtf(x); }
static inline float ffabsf(float x) { return fabsf(x); }
/* cvt.w.s with the EE's default round-to-zero. */
static inline int fcvtws(float value) { return (int)value; }

#endif /* FPU_H */
