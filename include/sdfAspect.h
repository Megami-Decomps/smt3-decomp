#ifndef SDF_ASPECT_H
#define SDF_ASPECT_H

/* Camera limits chosen so a 4:3 view stays inside the modelled set. Each macro
 * expands to retail's code, so this build is unchanged. A host that draws a
 * wider view defines them first and narrows each limit by the extra width. */

/* Clamps a camera's horizontal coordinate x to [lo, hi]. reach and rise are
 * the look-at point's offset from the eye this call, across and down; retail
 * does not read them. */
#ifndef SDF_ASPECT_CLAMP_X
#define SDF_ASPECT_CLAMP_X(x, lo, hi, reach, rise)                                              \
    do {                                                                            \
        if ((x) < (lo)) {                                                           \
            (x) = (lo);                                                             \
        }                                                                           \
        if ((x) > (hi)) {                                                           \
            (x) = (hi);                                                             \
        }                                                                           \
    } while (0)
#endif

#endif
