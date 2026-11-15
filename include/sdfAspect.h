#ifndef SDF_ASPECT_H
#define SDF_ASPECT_H

/* Camera limits chosen so a 4:3 view stays inside the modelled set. Each macro
 * expands to retail's code, so this build is unchanged. A host that draws a
 * wider view defines them first and narrows each limit by the extra width. */

/* Clamps a camera's horizontal coordinate x to [lo, hi]. first, second,
 * inverse and blend are the camera rows this call blends and their weights;
 * retail does not read them. */
#ifndef SDF_ASPECT_CLAMP_X
#define SDF_ASPECT_CLAMP_X(x, lo, hi, first, second, inverse, blend)                 \
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
