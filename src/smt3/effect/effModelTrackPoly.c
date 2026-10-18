#include "common.h"
#include "ps2asm.h"

typedef struct
{
    u8 pad00[0x18];
    void *model;
} EffTrackPolyModelObject_t;

typedef struct
{
    EffTrackPolyModelObject_t *object;
    s32 mark0;
    s32 mark1;
    u8 pad0C[0x2C];
    void *trackPoly;
} EffModelTrackPoly_t;

/* The ring holds `capacity` vertex pairs; `head` is one past the newest. */
typedef struct
{
    u8 pad00[8];
    s32 capacity;
    s32 count;
    s32 head;
    s32 division;
    void *vertices;
} Dds3HermitianTrackPoly_t;

extern s32 sdfModelGetMarkPos(void *model, s32 mark);
extern void dds3HermitianTrackPolySetVertex(void *poly, void *pair);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", Eff_ModelTrackPolyCreate);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", Eff_ModelTrackPolyDelete);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", Eff_ModelTrackPolyReset);

void Eff_ModelTrackPolyGetMarkPosition(EffModelTrackPoly_t *work)
{
    QuadF32 pair[2];
    EffTrackPolyModelObject_t *object = work->object;

    sdfModelGetMarkPos(object->model, work->mark0);
    VU0_STORE_VF10_TO(pair[0]);
    sdfModelGetMarkPos(object->model, work->mark1);
    VU0_STORE_VF10(&pair[1]);
    dds3HermitianTrackPolySetVertex(work->trackPoly, pair);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", Eff_ModelTrackPolyVertexSet);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", Eff_ModelTrackPolyColorSet);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", Eff_ModelTrackPolyDrawOnly);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", Eff_ModelTrackPolyDraw);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", Eff_ModelTrackPolyListCreate);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", Eff_ModelTrackPolyListDelete);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", Eff_ModelTrackPolyListVertexSet);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", Eff_ModelTrackPolyListReset);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", Eff_ModelTrackPolyListColorSet);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", Eff_ModelTrackPolyListDrawOnly);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", Eff_HermitianSpline);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", dds3HermitianTrackPolyCreate);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", pMeshDelete);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", dds3HermitianTrackPolyReset);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", dds3HermitianTrackPolySetBlend);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", dds3HermitianTrackPolySetVC);

void dds3HermitianTrackPolyGetVertex(Dds3HermitianTrackPoly_t *poly, void *first, void *second, s32 back)
{
    s32 index = poly->head - 2 * (poly->division * (back - 1) + back);
    u8 *vertex;

    if (index < 2) {
        index += poly->capacity - 2;
    }
    vertex = (u8 *)poly->vertices + index * 16;
    PS2_COPY_QUAD(first, vertex);
    PS2_COPY_QUAD(second, vertex + 16);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", dds3HermitianTrackPolySetCurrent);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", dds3HermitianTrackPolySetVertexSub);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", dds3HermitianTrackPolySetVertex);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", dds3HermitianTrackPolyDraw);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", Eff_ModelTrackPoly_DANTE_Check);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", effSwordTrackCreate);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", effSwordTrackDelete);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", effSwordTrackRecord);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", effSwordTrackPolygonDraw);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", effSwordTrackPolygon2DDraw);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", effSwordTrackDraw);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", effSwordTrackReset);
