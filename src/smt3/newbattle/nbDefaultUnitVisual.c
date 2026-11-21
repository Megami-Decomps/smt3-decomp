#include "newdata.h"

/* Native little-endian motion-sound bytes in a signed word field. */
#define MOTION_SOUND(low, high, type, data) \
    { (low) + (high) * 0x100 + (type) * 0x10000 + (data) * 0x1000000 }

datUnitVisual_t nbDefaultUnitVisual __attribute__((aligned(16))) = {
    { 0.0f, -90.0f, 0.0f, 0.0f },
    50.0f, 180.0f, 1.0f, 53.0f, 83.0f,
    1, 2, 10, 0,
    {
        { 0, 30, MOTION_SOUND(0, 255, 0, -1), 1.0f, 100.0f, 0.0f, 0.0f, 0, 10 },
        { 0, 30, MOTION_SOUND(0, 255, 0, -1), 1.0f, 100.0f, 0.0f, 0.0f, 0, 10 },
        { 0, 30, MOTION_SOUND(0, 255, 0, -1), 1.0f, 100.0f, 0.0f, 0.0f, 0, 10 },
        { 2, 21, MOTION_SOUND(0, 0, 3, -1), 1.5f, 100.0f, 0.0f, 0.0f, 0, 10 },
        { 1, 32, MOTION_SOUND(0, 1, 1, -1), 1.0f, 100.0f, 0.0f, 0.0f, 0, 17 },
        { 1, 30, MOTION_SOUND(0, 2, 1, -1), 1.0f, 100.0f, 0.0f, 0.0f, 0, 14 },
        { 2, 15, MOTION_SOUND(0, 3, 0, -1), 1.0f, 100.0f, 0.0f, 0.0f, 0, 12 },
        { 1, 35, MOTION_SOUND(0, 2, 0, -1), 1.0f, 100.0f, 0.0f, 0.0f, 0, 14 },
        { 2, 33, MOTION_SOUND(0, 4, 3, -1), 1.0f, 270.0f, 0.0f, 0.0f, 0, 10 },
        { 2, 30, MOTION_SOUND(0, 255, 0, -1), 1.0f, 100.0f, 0.0f, 0.0f, 0, 10 },
        { 2, 31, MOTION_SOUND(0, 5, 3, -1), 1.0f, 120.0f, 0.0f, 0.0f, 0, 10 },
        { 1, 30, MOTION_SOUND(0, 2, 0, -1), 1.0f, 100.0f, 0.0f, 0.0f, 0, 14 },
        { 2, 35, MOTION_SOUND(0, 6, 3, -1), 1.0f, 180.0f, 0.0f, 0.0f, 0, 10 },
        { 0, 30, MOTION_SOUND(0, 255, 0, -1), 1.0f, 100.0f, 0.0f, 0.0f, 0, 10 },
        { 2, 20, MOTION_SOUND(0, 7, 3, -1), 1.0f, 100.0f, 0.0f, 0.0f, 0, 10 },
        { 2, 20, MOTION_SOUND(0, 8, 0, -1), 1.0f, 100.0f, 0.0f, 0.0f, 0, 12 },
        { 1, 25, MOTION_SOUND(0, 9, 1, -1), 1.0f, 100.0f, 0.0f, 0.0f, 0, 20 },
        { 0, 20, MOTION_SOUND(0, 10, 3, -1), 1.0f, 100.0f, 0.0f, 0.0f, 0, 10 },
        { 0, 15, MOTION_SOUND(0, 11, 1, -1), 1.0f, 100.0f, 0.0f, 0.0f, 0, 12 },
        { 1, 15, MOTION_SOUND(0, 11, 1, -1), 1.0f, 100.0f, 0.0f, 0.0f, 0, 12 },
        { 1, 15, MOTION_SOUND(0, 12, 1, -1), 1.0f, 100.0f, 0.0f, 0.0f, 0, 10 },
        { 2, 30, MOTION_SOUND(0, 13, 3, -1), 1.0f, 180.0f, 0.0f, 0.0f, 0, 10 },
        { 0, 30, MOTION_SOUND(0, 255, 0, -1), 1.0f, 100.0f, 0.0f, 0.0f, 0, 10 },
        { 0, 30, MOTION_SOUND(0, 255, 0, -1), 1.0f, 100.0f, 0.0f, 0.0f, 0, 10 },
        { 1, 58, MOTION_SOUND(0, 14, 1, -1), 1.0f, 100.0f, 0.0f, 0.0f, 0, 17 },
        { 1, 32, MOTION_SOUND(0, 1, 1, -1), 1.0f, 100.0f, 0.0f, 0.0f, 0, 17 },
    },
};
