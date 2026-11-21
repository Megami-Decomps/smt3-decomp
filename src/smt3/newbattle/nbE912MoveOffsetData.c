#include "sdf_types.h"

/* MoveSub selects a row by the number of active formations, starting at one,
 * then loads each active slot's offset with lqc2. */
sdfVector4_t nbE912MoveOffset[4][4] __attribute__((aligned(16))) = {
    {
        { 0.0f, 0.0f, -450.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 0.0f },
    },
    {
        { -450.0f, 0.0f, 0.0f, 0.0f },
        { 450.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 0.0f },
    },
    {
        { -318.14996337890625f, 0.0f, -318.14996337890625f, 0.0f },
        { 0.0f, 0.0f, 450.0f, 0.0f },
        { 318.14996337890625f, 0.0f, -318.14996337890625f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 0.0f },
    },
    {
        { -450.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, -450.0f, 0.0f },
        { 0.0f, 0.0f, 450.0f, 0.0f },
        { 450.0f, 0.0f, 0.0f, 0.0f },
    },
};
