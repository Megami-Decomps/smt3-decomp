#include "libsdf.h"

typedef struct QWORD_ALIGNED { f32 component[4]; } pbMapVertex_t;

/* Board model orientation, side normals and the six cube faces. */
f32 pbPanelAngleTable[4] = { 3.141592502593994140625f, 4.712388515472412109375f, 0.0f, 1.5707962512969970703125f };

sdfVector4_t pbMapSideNormals[6] = {
    { 0.0f, -1.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, -1.0f, 0.0f },
    { 1.0f, 0.0f, 0.0f, 0.0f },
    { -1.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f, 0.0f },
};

pbMapVertex_t pbMapCubeVerts[8] = {
    { { -20.0f, 20.0f, -20.0f, 0.0f } },
    { { 20.0f, 20.0f, -20.0f, 0.0f } },
    { { 20.0f, 20.0f, 20.0f, 0.0f } },
    { { -20.0f, 20.0f, 20.0f, 0.0f } },
    { { -20.0f, -20.0f, -20.0f, 0.0f } },
    { { 20.0f, -20.0f, -20.0f, 0.0f } },
    { { 20.0f, -20.0f, 20.0f, 0.0f } },
    { { -20.0f, -20.0f, 20.0f, 0.0f } },
};

u8 pbMapCubeIndex[36] = {
    4, 5, 7, 7, 5, 6,
    2, 3, 6, 6, 3, 7,
    5, 4, 1, 1, 4, 0,
    6, 5, 2, 2, 5, 1,
    4, 7, 0, 0, 7, 3,
    0, 1, 3, 3, 1, 2,
};
