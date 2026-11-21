#include "common.h"

/* Four cardinal deltas followed by the vertical block movement (index 4). */
f32 pbDirVectorTable[5][3] = {
    {   0.0f,  0.0f,  10.0f },
    {  10.0f,  0.0f,   0.0f },
    {   0.0f,  0.0f, -10.0f },
    { -10.0f,  0.0f,   0.0f },
    {   0.0f, 10.0f,   0.0f },
};
