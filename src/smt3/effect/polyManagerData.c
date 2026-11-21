#include "common.h"
#include "draw3d.h"
#include "polyManagerData.h"

struct dds3SpPolyOldDesc_t;
struct dds3SpPoly_Basic_t;
struct dds3SpPoly_Ring_Old_t3;
struct dds3SpPoly_Ring_t3;
struct dds3SpPoly_RollRing_t3;
struct dds3SpPoly_SphereRing_t3;
struct dds3TrackPoly_t;

void dds3TrackPolyVCSet_Normal(struct dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out);
void dds3TrackPolyVCSet_Acute(struct dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out);
void dds3TrackPolyVCSet_AcuteW(struct dds3TrackPoly_t *hTrackPoly, u32 color);
void dds3TrackPolyVCSet2_Normal(struct dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out);
void dds3TrackPolyVCSet2_Acute(struct dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out);
void dds3TrackPolyVCSet2_AcuteW(struct dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out);
void dds3TrackPolyVCSet4_Normal(struct dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out);
void dds3TrackPolyVCSet4_Acute(struct dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out);
void dds3TrackPolyVCSet4_AcuteW(struct dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out);
struct dds3SpPolyOldDesc_t *dds3SpPoly_Ring_Old_Create(struct dds3SpPolyOldDesc_t *src);
void dds3SpPoly_Ring_Old_Draw(struct dds3SpPoly_Ring_Old_t3 *hData);
void dds3SpPoly_Ring_Old_Delete(struct dds3SpPoly_Ring_Old_t3 *hData);
struct dds3SpPoly_Ring_t3 *dds3SpPoly_RingCreate(struct dds3SpPoly_Basic_t *templateHead);
void dds3SpPoly_RingDraw(struct dds3SpPoly_Ring_t3 *hData);
void dds3SpPoly_RingDelete(struct dds3SpPoly_Ring_t3 *hData);
struct dds3SpPoly_SphereRing_t3 *dds3SpPoly_SphereRingCreate(struct dds3SpPoly_SphereRing_t3 *source);
void dds3SpPoly_SphereRingDraw(struct dds3SpPoly_SphereRing_t3 *hData);
void dds3SpPoly_SphereRingDelete(struct dds3SpPoly_SphereRing_t3 *hData);
struct dds3SpPoly_RollRing_t3 *dds3SpPoly_RollRingCreate(struct dds3SpPoly_Basic_t *templateHead);
void dds3SpPoly_RollRingDraw(struct dds3SpPoly_RollRing_t3 *hData);
void dds3SpPoly_RollRingDelete(struct dds3SpPoly_RollRing_t3 *hData);
void dds3SpPoly_Ring_Old_BuffInit(struct dds3SpPoly_Ring_Old_t3 *hData);
void dds3SpPoly_RingAllBuffInit(struct dds3SpPoly_Ring_t3 *band);
void dds3SpPoly_SphereRingAllBuffInit(struct dds3SpPoly_SphereRing_t3 *arc);
void dds3SpPoly_RollRingAllBuffInit(struct dds3SpPoly_RollRing_t3 *rotatingBand);
void dds3SpPoly_Ring_Old_SetScale(struct dds3SpPoly_Ring_Old_t3 *hData, f32 scale);
void dds3SpPoly_RingSetScale(struct dds3SpPoly_Ring_t3 *hData, f32 scale);
void dds3SpPoly_SphereRingSetScale(struct dds3SpPoly_SphereRing_t3 *hData, f32 scale);
void dds3SpPoly_RollRingSetScale(struct dds3SpPoly_RollRing_t3 *hData, f32 scale);

extern sdfGrOt_t sdfGrOtTable[85];

typedef void (*dds3TrackPolyVCSetFunc_t)(void *, void *, void *);
typedef void *(*dds3SpPolyCreateFunc_t)(void *);
typedef void (*dds3SpPolyWorkFunc_t)(void *);
typedef void (*dds3SpPolyScaleFunc_t)(void *, f32);

u32 dds3TrackPolyPairFaceIndex[32] = {
    0x00030100, 0x00020300, 0x00050302, 0x00040502,
    0x00070504, 0x00060704, 0x00090706, 0x00080906,
    0x000B0908, 0x000A0B08, 0x000D0B0A, 0x000C0D0A,
    0x000F0D0C, 0x000E0F0C, 0x00110F0E, 0x0010110E,
    0x00131110, 0x00121310, 0x00151312, 0x00141512,
    0x00171514, 0x00161714, 0x00191716, 0x00181916,
    0x001B1918, 0x001A1B18, 0x001D1B1A, 0x001C1D1A,
    0x001F1D1C, 0x001E1F1C, 0x00000000, 0x00000000,
};
u32 dds3TrackPolyTriangleFaceIndex[28] = {
    0x00040100, 0x00030400, 0x00050201, 0x00040501,
    0x00070403, 0x00060703, 0x00080504, 0x00070804,
    0x000A0706, 0x00090A06, 0x000B0807, 0x000A0B07,
    0x000D0A09, 0x000C0D09, 0x000E0B0A, 0x000D0E0A,
    0x00100D0C, 0x000F100C, 0x00110E0D, 0x0010110D,
    0x0013100F, 0x0012130F, 0x00141110, 0x00131410,
    0x00161312, 0x00151612, 0x00171413, 0x00161713,
};
u32 dds3TrackPolySixVectorFaceIndex[32] = {
    0x00070100, 0x00060700, 0x00080201, 0x00070801,
    0x00090302, 0x00080902, 0x000A0403, 0x00090A03,
    0x000B0504, 0x000A0B04, 0x000D0706, 0x000C0D06,
    0x000E0807, 0x000D0E07, 0x000F0908, 0x000E0F08,
    0x00100A09, 0x000F1009, 0x00110B0A, 0x0010110A,
    0x00130D0C, 0x0012130C, 0x00140E0D, 0x0013140D,
    0x00150F0E, 0x0014150E, 0x0016100F, 0x0015160F,
    0x00171110, 0x00161710, 0x00000000, 0x00000000,
};
u32 dds3TrackPolyDefaultFaceIndex[20] = {
    0x00050100, 0x00040500, 0x00060201, 0x00050601,
    0x00070302, 0x00060702, 0x000D0908, 0x000C0D08,
    0x000E0A09, 0x000D0E09, 0x000F0B0A, 0x000E0F0A,
    0x00151110, 0x00141510, 0x00161211, 0x00151611,
    0x00171312, 0x00161712, 0x00000000, 0x00000000,
};
u32 dds3TrackPolyFiveVectorFaceIndex[24] = {
    0x00060100, 0x00050600, 0x00070201, 0x00060701,
    0x00080302, 0x00070802, 0x00090403, 0x00080903,
    0x00100B0A, 0x000F100A, 0x00110C0B, 0x0010110B,
    0x00120D0C, 0x0011120C, 0x00130E0D, 0x0012130D,
    0x001A1514, 0x00191A14, 0x001B1615, 0x001A1B15,
    0x001C1716, 0x001B1C16, 0x001D1817, 0x001C1D17,
};
f32 dds3PolygonTriangleParam[4] = { 0.0f, 0.0f, -1.0f, 0.0f };
f32 dds3TrackPolyVCType1Param[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 dds3TrackPolyVCType2Param[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 dds3TrackPolyVCSetAcuteParam[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 dds3TrackPolyVCSetAcuteWParam[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 dds3TrackPolyVCAcuteParam[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
/* Indexed by track type * 3 + colour mode. Types 0, 1 and 3 are populated;
 * the IDB's dds3TrackPolyVCSet4Table is entry 9. */
void (*VCSetFunc[16])(void *, void *, void *) = {
    (dds3TrackPolyVCSetFunc_t)dds3TrackPolyVCSet_Normal,
    (dds3TrackPolyVCSetFunc_t)dds3TrackPolyVCSet_Acute,
    (dds3TrackPolyVCSetFunc_t)dds3TrackPolyVCSet_AcuteW,
    (dds3TrackPolyVCSetFunc_t)dds3TrackPolyVCSet2_Normal,
    (dds3TrackPolyVCSetFunc_t)dds3TrackPolyVCSet2_Acute,
    (dds3TrackPolyVCSetFunc_t)dds3TrackPolyVCSet2_AcuteW,
    NULL,
    NULL,
    NULL,
    (dds3TrackPolyVCSetFunc_t)dds3TrackPolyVCSet4_Normal,
    (dds3TrackPolyVCSetFunc_t)dds3TrackPolyVCSet4_Acute,
    (dds3TrackPolyVCSetFunc_t)dds3TrackPolyVCSet4_AcuteW,
};
sdfGrOt_t *dds3TrackPolyOtTable[4] = {
    &sdfGrOtTable[43],
    &sdfGrOtTable[43],
    &sdfGrOtTable[44],
    &sdfGrOtTable[45],
};
f32 dds3SpPolyColorTo[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
f32 dds3SpPolyColorFrom[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
dds3SpPolyEntry_t dds3SpPolyTable[4] = {
    { (dds3SpPolyCreateFunc_t)dds3SpPoly_Ring_Old_Create, (dds3SpPolyWorkFunc_t)dds3SpPoly_Ring_Old_Draw,
      (dds3SpPolyWorkFunc_t)dds3SpPoly_Ring_Old_Delete },
    { (dds3SpPolyCreateFunc_t)dds3SpPoly_RingCreate, (dds3SpPolyWorkFunc_t)dds3SpPoly_RingDraw,
      (dds3SpPolyWorkFunc_t)dds3SpPoly_RingDelete },
    { (dds3SpPolyCreateFunc_t)dds3SpPoly_SphereRingCreate, (dds3SpPolyWorkFunc_t)dds3SpPoly_SphereRingDraw,
      (dds3SpPolyWorkFunc_t)dds3SpPoly_SphereRingDelete },
    { (dds3SpPolyCreateFunc_t)dds3SpPoly_RollRingCreate, (dds3SpPolyWorkFunc_t)dds3SpPoly_RollRingDraw,
      (dds3SpPolyWorkFunc_t)dds3SpPoly_RollRingDelete },
};
void (*dds3SpPolyResetTable[4])(void *work) = {
    (dds3SpPolyWorkFunc_t)dds3SpPoly_Ring_Old_BuffInit,
    (dds3SpPolyWorkFunc_t)dds3SpPoly_RingAllBuffInit,
    (dds3SpPolyWorkFunc_t)dds3SpPoly_SphereRingAllBuffInit,
    (dds3SpPolyWorkFunc_t)dds3SpPoly_RollRingAllBuffInit,
};
void (*dds3SpPolySetScaleTable[4])(void *work, f32 scale) = {
    (dds3SpPolyScaleFunc_t)dds3SpPoly_Ring_Old_SetScale,
    (dds3SpPolyScaleFunc_t)dds3SpPoly_RingSetScale,
    (dds3SpPolyScaleFunc_t)dds3SpPoly_SphereRingSetScale,
    (dds3SpPolyScaleFunc_t)dds3SpPoly_RollRingSetScale,
};
