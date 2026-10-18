#include "common.h"
#include "libsdf.h"
#include "ps2asm.h"

#include "effect.h"


extern sdfGrOt_t *D_00320030[4];

/* Creation-table entries return the newly allocated type-specific work. */
typedef struct effCreateHandler_t {
    void *(*handler)(void *);
    u8 unk4[0x14];
} effCreateHandler_t;

extern effCreateHandler_t D_00320040[];

/* Effect callback dispatch entry (0x18); DDS1/2 game/code_0018CAC8/00194700.c. */
typedef struct effHandler_t {
    void (*handler)(void *);
    u8 unk4[0x14];
} effHandler_t;

extern effHandler_t D_00320044[];

extern effHandler_t D_00320048[];

extern effHandler_t D_0032004C[];

extern effHandler_t D_00320050[];

extern effHandler_t D_00320054[];

extern char *dds3EffectFileNameTable[];

extern s8 D_0054F220;

extern char D_0054F260[];

extern u32 D_00551DDC;

extern s16 D_0056AF30[15][8];

/* Shared 0x20-byte DMA/VIF reference header. Unrecovered bytes stay opaque. */
typedef struct dmaPacketHeader_t {
    u16 quadwords; /* 0x00 */
    u8 pad02[2]; /* 0x02 */
    u32 address; /* 0x04 */
    u32 tag; /* 0x08 */
    u32 command; /* 0x0C */
    u16 unknown10; /* 0x10 */
    u8 opaque12[6]; /* 0x12 */
    u32 unknown18; /* 0x18 */
    u32 unknown1C; /* 0x1C */
} QWORD_ALIGNED dmaPacketHeader_t;


#define EFF_BUILTIN_NAME_COUNT 0x2F

#define EFF_DIRECTORY_FLAG_CLEAR 0x1000

#define EFF_DIRECTORY_PATH_BYTES 0x70

#define EFF_MAGATUHI_ALPHA_SHIFT 24

#define EFF_MAGATUHI_BEZIER_GROUP_STRIDE 4

#define EFF_MAGATUHI_CALLBACK_TYPE 3

#define EFF_MAGATUHI_CONTROL_POINT_COUNT 4

#define EFF_MAGATUHI_DELAY_WORD_BYTES 4

#define EFF_MAGATUHI_FADE_ALPHA_SCALE 127.0f

#define EFF_MAGATUHI_FULL_TURN 6.2831853f

#define EFF_MAGATUHI_HALF_TURN 3.14159265f

#define EFF_MAGATUHI_HISTORY_MINIMUM 8

#define EFF_MAGATUHI_MATRIX_WORD_COUNT 16

#define EFF_MAGATUHI_NEUTRAL_COLOR 0x80808080

#define EFF_MAGATUHI_NEUTRAL_RGB 0x808080

#define EFF_MAGATUHI_RANDOM_MIDPOINT 0.5f

#define EFF_MAGATUHI_REPLAY_BEZIER 1

#define EFF_MAGATUHI_REPLAY_DRIFT 4

#define EFF_MAGATUHI_REPLAY_GROUP_STRIDE 3

#define EFF_MAGATUHI_REPLAY_NONE 0

#define EFF_MAGATUHI_REPLAY_ORBIT 3

#define EFF_MAGATUHI_REPLAY_RING 2

#define EFF_MAGATUHI_VECTOR_WORD_COUNT 4

#define EFF_MAGATUHI_XYZ_COMPONENT_COUNT 3

#define EFF_SUBWORK_PREFIX_BYTES 0x40


/* SDK directory entry filled by the directory-read provider. */
typedef struct effDirEnt_t {
    u32 flags;
    u8 opaque04[0x3C];
    char name[0x100];
    void *privateData;
} effDirEnt_t;

/* The creation callback's returned payload is retained in this 8-byte dispatch owner. */
typedef struct effWork_t {
    u32 type;
    void *payload;
} effWork_t;

/* Callback input prefix; type 3 contains an effect-dispatch object at +8. */
typedef struct {
    s32 type;
    u8 pad04[4];
    effWork_t *effect;
} effMagatuhiCallback_t;

/* Drift parameters are not ring parameters despite their equal size (0xDC). */
typedef struct {
    f32 origin[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    u8 respawn;
    u8 pad11[3];
    s32 lifetimeFrames;
    s32 delaySpread;
    s32 fadeIn;
    s32 fadeOut;
    f32 initialRadius;
    f32 initialLift;
    f32 initialLiftRandomness;
    f32 angleStep;
    f32 initialScale;
    f32 scaleStep;
    f32 initialScaleRandomness;
    u32 particleCount;
    u32 maxSteps;
    u8 pad48[0x94];
} effMagatuhiDriftParams_t;

/* Shared 0x30-byte state: base position, planar drift, and accumulated lift. */
typedef struct {
    f32 position[EFF_MAGATUHI_XYZ_COMPONENT_COUNT];
    u8 pad0C[4];
    f32 driftState[EFF_MAGATUHI_XYZ_COMPONENT_COUNT]; /* XZ direction, Y accumulated lift */
    u8 pad1C[4];
    s32 age; /* negative: waiting to start; nonnegative: elapsed frames */
    f32 scale;
    f32 angle;
    f32 liftStep;
} effMagatuhiDriftParticle_t;

/* Seven copied words; the parent appends its independently allocated history. */
typedef struct effMagatuhiHistoryParams_t {
    s32 count;
    s32 historyCount;
    f32 historyDistanceScale;
    u32 colorA;
    u32 colorB;
    s32 drawBucket;
    f32 historyAngleStep;
} effMagatuhiHistoryParams_t;

/* History arrays precede this 0x38-byte owner in one retained allocation. */
typedef struct effMagatuhiValueWork_t {
    s32 count;
    u16 historyCount;
    u8 pad06[2];
    f32 historyDistanceScale; /* Segment length is clamped to three times this value. */
    f32 historyAngleStep; /* Radians per history update. */
    u32 drawBucket;
    f32 (*positions)[4];
    u32 *colorTable;
    f32 *historyScalars;
    u32 *slotColors;
    u16 *writeIndices;
    u16 *validCounts;
    f32 (*angleRows)[4];
    sdfTexHandle_t *texture;
    struct sdfMemHandle_t *allocationHandle;
} effMagatuhiValueWork_t;

typedef struct effMagatuhiOwner_t {
    effMagatuhiHistoryParams_t params;
    effMagatuhiValueWork_t *valueWork;
} effMagatuhiOwner_t;

/* The 0x30-byte states precede this 0x12C-byte work allocation. */
typedef struct {
    f32 matrix[EFF_MAGATUHI_MATRIX_WORD_COUNT];
    effMagatuhiDriftParams_t head;
    effMagatuhiDriftParticle_t *particles;
    u32 tintColor;
    effMagatuhiOwner_t *managedResource;
    sdfMemHandle_t *allocationHandle;
} effMagatuhiDriftWork_t;

/* Float source block read by effMagatuhiCopyFloatBlock. */
typedef struct {
    f32 originX, originY, originZ;
    u8 pad0C[4];
    f32 targetX, targetY, targetZ;
    u8 pad1C[4];
    f32 initialRadius, captureDistance;
} effMagatuhiFloatParams_t; /* 0x28 */

/* Parameter head of the first family: particles of 0x30 bytes precede the work. */
typedef struct {
    f32 originX, originY, originZ;
    u8 pad0C[4];
    f32 targetX, targetY, targetZ;
    u8 pad1C[4];
    u8 respawn;            /* 0x20 */
    u8 pad21[3];
    s32 lifetimeFrames;    /* 0x24 */
    s32 delaySpread;       /* 0x28 modulus of the particle delay */
    s32 fadeInFrames;      /* 0x2C */
    s32 fadeOutFrames;     /* 0x30 */
    f32 initialRadius;
    f32 baseLiftStep;      /* 0x38 */
    f32 liftVariation;
    f32 angleStep;         /* 0x40 */
    f32 baseScale;         /* 0x44 */
    f32 scaleStep;         /* 0x48 */
    f32 scaleVariation;    /* 0x4C */
    f32 captureDistance;   /* 0x50: transition from drift to the path */
    f32 pathJitter;        /* 0x54 */
    f32 pathStep;          /* 0x58 */
    f32 pathAcceleration;  /* 0x5C: multiplier applied to the path step */
    u32 particleCount;     /* 0x60 */
    u8 pad64[0x118];
} effMagatuhiHeadFirst_t; /* 0x17C */

/* Bezier parameter head: particle count and delay spread at their native offsets. */
typedef struct {
    f32 controlPoints[EFF_MAGATUHI_CONTROL_POINT_COUNT][EFF_MAGATUHI_VECTOR_WORD_COUNT];
    u8 respawn;            /* 0x40 restart finished particles */
    u8 pad41[3];
    s32 lifetimeFrames;    /* 0x44 frames a particle lives */
    s32 delaySpread;       /* 0x48 modulus of the particle delay */
    s32 fadeIn;            /* 0x4C */
    s32 fadeOut;           /* 0x50 */
    f32 jitterScales[EFF_MAGATUHI_CONTROL_POINT_COUNT]; /* 0x54 */
    u32 particleCount;     /* 0x64 */
    u32 duration;          /* 0x68 */
    u8 pad6C[0x114];
} effMagatuhiHeadSecond_t; /* 0x180 */

/* Orbit parameters copied into the work record; trailing bytes stay opaque. */
typedef struct effMagatuhiOrbitParams_t {
    f32 origin[4];
    u8 respawn;            /* 0x10 */
    u8 pad11[3];
    s32 lifetimeFrames;    /* 0x14 */
    s32 delaySpread;       /* 0x18 */
    s32 fadeIn;            /* 0x1C */
    s32 fadeOut;           /* 0x20 */
    f32 angleStep;         /* 0x24 */
    f32 angleJitter;       /* 0x28 */
    f32 heightStep;        /* 0x2C */
    f32 heightJitter;      /* 0x30 */
    f32 startRadius;       /* 0x34 */
    f32 endRadius;         /* 0x38 */
    f32 startJitter;       /* 0x3C */
    f32 endJitter;         /* 0x40 */
    u32 particleCount;     /* 0x44 */
    u32 maxSteps;          /* 0x48 */
    u8 pad4C[0x94];
} effMagatuhiOrbitParams_t;

/* One particle slot; replay visits the first slot in each triplet. */
typedef struct effMagatuhiOrbitParticle_t {
    s32 age;        /* 0x00: negative while waiting; otherwise elapsed frames */
    f32 height;     /* 0x04 */
    f32 heightStep; /* 0x08 */
    f32 angle;      /* 0x0C */
    f32 angleStep;  /* 0x10 */
    f32 radius;     /* 0x14 */
    f32 radiusStep; /* 0x18 */
} effMagatuhiOrbitParticle_t;

/* Work is followed by the orbit slots in the same retained allocation. */
typedef struct effMagatuhiOrbitWork_t {
    f32 matrix[16];
    effMagatuhiOrbitParams_t head;
    effMagatuhiOrbitParticle_t *particles;
    effMagatuhiOwner_t *managedResource;
    u32 tintColor;
    sdfMemHandle_t *allocationHandle;
} effMagatuhiOrbitWork_t;

/* Ring parameters copied verbatim into the work at +0x40 (0xDC). */
typedef struct {
    f32 origin[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    u8 respawn;
    u8 pad11[3];
    s32 lifetimeFrames;
    s32 delaySpread;
    s32 fadeIn;
    s32 fadeOut;
    f32 angleStep;
    f32 angleJitter;
    f32 heightSpread;
    f32 startRadius;
    f32 endRadius;
    f32 startJitter;
    f32 endJitter;
    u32 particleCount;
    u32 maxSteps;
    u8 pad48[0x94];
} effMagatuhiRingParams_t;

/* Per-particle state allocated after EffMagatuhiRingWork by both titles. */
typedef struct effMagatuhiRingParticle_t {
    s32 age;        /* 0x00: negative while waiting; otherwise elapsed frames */
    f32 height;     /* 0x04 */
    f32 angle;      /* 0x08 */
    f32 angleStep;  /* 0x0C */
    f32 radius;     /* 0x10 */
    f32 radiusStep; /* 0x14 */
} effMagatuhiRingParticle_t;

/* Work precedes its array of 0x18-byte ring states (0x12C). */
typedef struct {
    f32 matrix[EFF_MAGATUHI_MATRIX_WORD_COUNT];
    effMagatuhiRingParams_t head;
    effMagatuhiRingParticle_t *particles;
    effMagatuhiOwner_t *managedResource;
    u32 tintColor;
    sdfMemHandle_t *allocationHandle;
} effMagatuhiRingWork_t;

/* Four control points and their jitter scales; copy xyz but preserve each w. */
typedef struct {
    f32 controlPoints[EFF_MAGATUHI_CONTROL_POINT_COUNT][EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 jitterScales[EFF_MAGATUHI_CONTROL_POINT_COUNT];
} effMagatuhiRowsSrc_t;

typedef struct {
    effMagatuhiHeadFirst_t head;
    effMagatuhiDriftParticle_t *particles; /* 0x17C */
    effArrHdr_t *mathResource;    /* 0x180 */
    u8 pad184[8];
    effMagatuhiOwner_t *managedResource; /* 0x18C */
    sdfMemHandle_t *allocationHandle; /* 0x190 backing allocation, not the particle address */
} effMagatuhiWideFirst_t;

typedef struct {
    effMagatuhiHeadSecond_t head;
    s32 *delays;           /* 0x180 */
    effArrHdr_t *mathResource;    /* 0x184 */
    effMagatuhiOwner_t *managedResource; /* 0x188 */
    sdfMemHandle_t *allocationHandle; /* 0x18C backing allocation, not the work address */
} effMagatuhiWideSecond_t;

/* Effect subrecord with byte fields at +0x20/+0x40/+0x50 (0x51); DDS1/2 game/code_0018CAC8/00194700.c. */
typedef struct effSub_t {
    u8 unk0[0x20];
    u8 type0SubSlot;
    u8 unk21[0x1F];
    u8 type1SubSlot;
    u8 unk41[0xF];
    u8 type2To4SubSlot;
} effSub_t;


#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))


extern void dds3AdminiCallSequence(s32, void *, u32, s32);

/* Float-input SDK form: retain the broad memory effect and let GCC transfer
 * the binary32 scale to the unpack's GPR operand. The exact surface-grid
 * gradient uses four such 16-byte carriers (DDS1 002A7288, DDS2 002E9840).
 * This is distinct from the integer-bit, read-only form above. */
static inline void eeUnpackRGBAWordFloat(u32 color, f32 unit) {
    u32 rgba[4];
    rgba[0] = color;
    MMI_RGBA_UNPACK(rgba, unit);
}

/* Allocate/retain slot storage and return its header after the slots, not the slot base.
   Only the two native slot defaults are initialized; count/allocation validity is unchecked. */
effArrHdr_t *dds3Bezier4PCreate(s32 count);

/* vu0 routine: blend two RGBA8888 colours by t (lerp in float, packed back to RGBA8888) */
u32 effColorBlend(u32 colorA, u32 colorB, f32 t);

extern u32 D_00551DE4;

extern u8 dds3EffectBase[];

sdfTexHandle_t *dds3BillboardGetGeneralTex(s32 index);

effCubicBezierSlot_t *dds3Bezier4PGetPart(struct effArrHdr_t *table, s32 slotIndex);

void dds3Bezier4PDelete(struct effArrHdr_t *work);

s32 dds3Bezier4PPartRun(struct effArrHdr_t *table, s32 slotIndex, f32 *out);

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(u8 *state);

/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(u8 *state);

/* vu0 routine: multiply RGBA channels normalized by 128, then pack at the same scale.
   Native integer conversion/byte packing remains unclamped; quadword storage is unchanged. */
u32 effMagaCylinderCalcColor(u32 colorA, u32 colorB);

extern s32 effDebugPosX;

extern u32 effDebugPosY;

extern void *memcpy(void *dest, const void *src, u32 n);

/* Fixed-scale form of the SDK's four-word RGBA packing scratch buffer. */
static inline u32 packMagatuhiTrailColor(void) {
    u32 rgba[4];
    MMI_RGBA_PACK_UNIT(rgba[0], 128.0f);
    return rgba[0];
}

extern s32 sceDclose(s32 directory);

extern s32 sceDopen(void *path);

extern s32 sceDread(s32 directory, effDirEnt_t *entry);

struct sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 size);

void *sdfChipAllocDirect(s32 size);

void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);

void sdfDrawTagLinkRef(struct sdfDrawTag_t *list, u32 packetAddress);

/* Build XYZ and attribute VIF streams; return the packet address.
 * vertexCount must be positive; n retains its native count/byte-size reuse. */
/* vu0 routine: pack aligned positions into the VIF three-word stream. */
u32 *sdfBillBoardMakeN(const u128 *positions, const void *byteAttributes, const void *halfAttributes, const void *wordAttributes, s32 vertexCount, void *(*allocatePacket)(s32));

/* Round payloadBytes up to quadwords and pair FLUSHA with VIF DIRECT. */
void sdf3DMakeDirectRefPacket(dmaPacketHeader_t *packet, u32 sourceAddress, s32 payloadBytes);

f32 sdfCos(f32 angle);

void sdfBillBoardMakeTMPacket(u8 *packet, const f32 *matrix);

void sdfDrawTagInit(struct sdfDrawTag_t *list);

extern u8 sdfHddValid;

void sdfChipFreeDirect(void *memory);

void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);

u32 sdfMemLock(struct sdfMemHandle_t *allocation);

f32 sdfSin(f32 angle);

/* Requires a non-NULL texture; returns its primary buffer as a borrowed view. */
struct sdfTexPacket_t *sdfTexGetPacket(struct sdfTexHandle_t *texture);

/* Requires a non-NULL texture; returns 0 for no primary buffer, otherwise
 * ((gifTagWord & 0x7fff) + 1) * 16 bytes. */
s32 sdfTexGetPacketLength(const struct sdfTexHandle_t *texture);

extern f32 aViewTarget[EFF_MAGATUHI_VECTOR_WORD_COUNT];

extern f32 aViewEye[EFF_MAGATUHI_VECTOR_WORD_COUNT];

extern s32 sprintf(char *destination, const char *format, ...);

extern char * strcpy(char *, const char *);

extern u32 strlen(const char *text);

effMagatuhiOwner_t *effMagaCylinderAllocPart(const u32 *source);
void effMagaCylinderFree(effMagatuhiOwner_t *resource);
void effMagaCylinderDrawTrack(effMagatuhiOwner_t *resource);
void effMagaCylinderSetPartPtrs(effMagatuhiValueWork_t *work, s32 slotIndex);
effMagatuhiValueWork_t *effMagaCylinderAllocTex(s32 count, s32 historyCount, f32 historyDistanceScale, s32 param, f32 historyAngleStep);
/* Release the allocation retained by the render/history owner. */
void effMagaCylinderFreeParts(effMagatuhiValueWork_t *work);
void effSwordTrackDrawTex(effMagatuhiValueWork_t *work);
void effMagaCylinderSetPartPos(effMagatuhiValueWork_t *work, s32 index, const f32 vector[4]);
/* Store a per-slot packed color; the native path does not validate the index. */
void effMagaCylinderSetPart(effMagatuhiValueWork_t *work, s32 index, u32 color);
/* Accumulate 1/historyCount per entry; the nominal last fraction is below 1. */
void effMagaCylinderBlendColor(effMagatuhiValueWork_t *work, u32 colorA, u32 colorB);
void effMagaSmokeInitPart(effMagatuhiValueWork_t *valueWork, s32 index);
/* Clone the first-family parameters; return the work after its state array.
 * Clamp only the copied delay modulus, leaving the caller's parameters intact. */
effMagatuhiWideFirst_t *effMagaSmokeConvCreate(effMagatuhiHeadFirst_t *src);
/* Release math state, the history owner, then the backing allocation. */
void effMagaSmokeConvDelete(effMagatuhiWideFirst_t *work);
/* Seed independent normalized XZ position/drift vectors and clear slot history.
 * The vector scratch w remains unwritten, as in the retail routine. */
void effMagaSmokeConvPartInit(effMagatuhiWideFirst_t *work, s32 index);
/* Drift until the particle enters the capture radius, then follow a randomized
 * cubic path to the target. Fade-in takes priority over an overlapping fade-out.
 * Preserve the native unwritten point w and unchecked fade divisors. */
void effMagaSmokeConvDraw(effMagatuhiWideFirst_t *work);
/* Update the first family's scattered float parameters without touching gaps. */
void effMagaSCSetArea(effMagatuhiCallback_t *work, effMagatuhiFloatParams_t *src);
/* Clone history parameters; signed delays follow the returned work block. */
effMagatuhiWideSecond_t *effMagaPoint4Create(effMagatuhiHeadSecond_t *src);
/* Release Bezier slots, their history owner, then the backing allocation. */
void effMagaPoint4Delete(effMagatuhiWideSecond_t *work);
/* Jitter control points perpendicular to the path and camera viewing direction. */
void effMagaPoint4PartInit(effMagatuhiWideSecond_t *work, s32 index);
/* Advance delayed Bezier particles, pack their fade, and submit the owner.
 * Fade-in wins when its interval overlaps fade-out; divisors are unchecked. */
void effMagaPoint4Draw(effMagatuhiWideSecond_t *work);
/* Replace control-point xyz and jitter amplitudes without changing point w. */
void effMagaP4SetPoint(effMagatuhiCallback_t *work, effMagatuhiRowsSrc_t *src);
/* Seed every fourth slot with interpolated samples between two curve positions.
 * Keep the asymmetric duration/age subtraction and unchecked span divisor. */
void effMagaP4Randomize(effMagatuhiCallback_t *callback);
/* Clone ring parameters; the returned work precedes its individual slots. */
effMagatuhiRingWork_t *effMagaCylinderCreate(effMagatuhiRingParams_t *src);
/* Release the ring history owner before the work/state allocation. */
void effMagaCylinderDelete(effMagatuhiRingWork_t *work);
/* Initialize a ring slot and reset the history/value owned by that slot. */
void effMagaCylinderPartInit(effMagatuhiRingWork_t *work, s32 index);
/* Sample the current fixed-height ring before advancing angle and radius.
 * Keep the cached age across initialization and preserve fade-in priority. */
void effMagaCylinderDraw(effMagatuhiRingWork_t *work);
/* Copy the ring origin as one quadword, including its fourth component. */
void effMagaCylinderSetPosition(effMagatuhiRingWork_t *work, void *origin);
/* The historical symbol names a resource, but this slot is packed color. */
void effMagaCylinderSetRGBA(effMagatuhiRingWork_t *work, u32 color);
/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagaCylinderSetRotateMatrix(effMagatuhiRingWork_t *work, void *matrix);
/* Replay the first slot of each three-slot group from a random age. */
void effMagaCylinderRandomize(effMagatuhiCallback_t *callback);
/* Clone orbit parameters; the returned work precedes its individual slots. */
effMagatuhiOrbitWork_t *effMagaSpiralCreate(effMagatuhiOrbitParams_t *src);
/* Release the orbit history owner before the work/state allocation. */
void effMagaSpiralDelete(effMagatuhiOrbitWork_t *work);
/* Seed randomized height/angular/radial increments and clear the slot history. */
void effMagaSpiralPartInit(effMagatuhiOrbitWork_t *work, s32 index);
/* Sample and transform the current orbit before advancing its three rates.
 * Keep the cached age across initialization and preserve fade-in priority. */
void effMagaSpiralDraw(effMagatuhiOrbitWork_t *work);
/* Copy the orbit origin as one quadword, preserving the source w. */
void effMagaSpiralSetPosition(effMagatuhiOrbitWork_t *work, void *origin);
/* This callback sets the packed orbit color, not an allocation handle. */
void effMagaSpiralSetRGBA(effMagatuhiOrbitWork_t *work, u32 color);
/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagaSpiralSetRotateMatrix(effMagatuhiOrbitWork_t *work, void *matrix);
/* Advance the orbiting-particle family: each group of three slots is replayed from a random delay, one orbit step at a time, into the value table. */
void effMagaSpiralRandomize(effMagatuhiCallback_t *callback);
/* Clone drift parameters; return the work after its 0x30-byte state array. */
effMagatuhiDriftWork_t *effMagaSmokeCreate(effMagatuhiDriftParams_t *src);
/* Release the drift history owner before its states-plus-work allocation. */
void effMagaSmokeDelete(effMagatuhiDriftWork_t *work);
/* vu0 routine: initialize a planar particle with normalized position/drift. */
void effMagaSmokePartInit(effMagatuhiDriftWork_t *work, s32 index);
/* Accumulate lift, transform the drift sample, then advance scale and angle.
 * Keep the cached age across initialization and preserve fade-in priority. */
void effMagaSmokeDraw(effMagatuhiDriftWork_t *work);
/* Copy the drift origin as one quadword, preserving the source w. */
void effMagaSmokeSetPosition(effMagatuhiDriftWork_t *work, void *origin);
/* This callback sets packed drift color; +0x120 is not an owner pointer. */
void effMagaSmokeSetRGBA(effMagatuhiDriftWork_t *work, u32 color);
/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagaSmokeSetRotateMatrix(effMagatuhiDriftWork_t *work, void *matrix);
/* Replay the first slot of each three-slot drift group from a random age. */
void effMagaSmokeRandomize(effMagatuhiCallback_t *callback);
/* Dispatch replay side effects; retain the native s32 definition's fall-through. */
s32 effMagaRandomize(effMagatuhiCallback_t *work);
/* Allocate the dispatch owner before calling the selected payload constructor. */
effWork_t *dds3MagatuhiParCreate(s32 type, void *params);
/* Invoke the primary type callback with the payload; no NULL or type bounds check. */
void dds3MagatuhiParDraw(effWork_t *work);
/* Invoke the release callback, then free the outer work; no callback NULL check. */
void dds3MagatuhiParDelete(effWork_t *work);
/* Return the retained payload passed to this type's callbacks. */
void *dds3MagatuhiGetHandle(effWork_t *work);
/* Read the dispatch owner's type word; the pointer remains unchecked. */
u32 dds3MagatuhiGetType(effWork_t *work);
/* Native node type-three condition slot: accept with the constant result one. */
u32 dds3MagatuhiParIsActive(void);
/* Node set-vector slot: invoke optional subtype callback A; preserve the one-formal surface. */
void dds3MagatuhiParSetPosition(effWork_t *work);
/* Node set-matrix slot: invoke optional subtype callback B; preserve the one-formal surface. */
void dds3MagatuhiParSetRotateMatrix(effWork_t *work);
/* Node parameter-word slot: invoke optional subtype callback C; preserve the one-formal surface. */
void dds3MagatuhiParSetRGBA(effWork_t *work);
/* Store the low byte of slotValue at the type-specific subslot.
   Types two through four share the same location; other types are left unchanged. */
void dds3MagatuhiParSetByType(effWork_t *work, s32 slotValue);
/* Read the type-specific subslot, or zero for other types.
   The callback's second argument is unused. */
u32 func_001A8DF8(effWork_t *work, s32 unusedSlotValue);
/* Types zero/one dispatch from the payload base; two through four skip its prefix.
   Other types still dispatch with a zero argument; no type bounds check is added. */
void dds3MagatuhiParCopy(effWork_t *work);
void InitMagatuhiEditorProcess(void);
void effDebugSetPos(s32 x, s32 y);
void effDebugSetAttr(u32 attributes);
s32 dds3_1AACF8(void);
/* Debug filesystem mode opens the formatted pfs0 directory; otherwise reset built-in iteration.
   Return the native directory-open result, or zero for built-in mode. */
s32 dds3EffectDirOpen(void *name);
/* Run the native directory callback only in debug filesystem mode; its result is ignored. */
void dds3EffectDirClose(s32 directory);
/* Debug mode delegates to the native iterator. Built-in mode copies the next name,
   clears flag mask 0x1000, and returns name length; zero also marks table exhaustion. */
s32 dds3EffectDirRead(s32 directory, effDirEnt_t *entry);

effMagatuhiOwner_t *effMagaCylinderAllocPart(const u32 *source) {
    effMagatuhiOwner_t *effect;

    effect = sdfChipAllocDirect(sizeof(*effect));
    memcpy(&effect->params, source, sizeof(effect->params));
    effect->valueWork = effMagaCylinderAllocTex(effect->params.count, effect->params.historyCount, effect->params.historyDistanceScale, effect->params.drawBucket, effect->params.historyAngleStep);
    effMagaCylinderBlendColor(effect->valueWork, effect->params.colorA, effect->params.colorB);
    return effect;
}

void effMagaCylinderFree(effMagatuhiOwner_t *resource) {
    effMagaCylinderFreeParts(resource->valueWork);
    sdfChipFreeDirect(resource);
}

void effMagaCylinderDrawTrack(effMagatuhiOwner_t *resource) {
    effSwordTrackDrawTex(resource->valueWork);
}

void effMagaCylinderSetPartPtrs(effMagatuhiValueWork_t *work, s32 slotIndex) {
    f32 *angleRow;

    work->writeIndices[slotIndex] = 0;
    work->validCounts[slotIndex] = 0;
    work->slotColors[slotIndex] = 0x80808080;
    angleRow = work->angleRows[slotIndex];
    angleRow[0] = 6.2831853f;
    angleRow[1] = 0.0f;
    angleRow[2] = 3.1415926f;
    angleRow[3] = 0.0f;
}

effMagatuhiValueWork_t *effMagaCylinderAllocTex(s32 count, s32 historyCount, f32 historyDistanceScale, s32 param, f32 historyAngleStep) {
    s32 countFrames;
    s32 frameTerm;
    s32 countTerm;
    s32 allocationSize;
    struct sdfMemHandle_t *allocation;
    s32 positions;
    s32 colorTable;
    s32 historyScalars;
    s32 values;
    s32 writeIndices;
    s32 validCounts;
    s32 angleValues;
    effMagatuhiValueWork_t *resource;
    s16 *defaults;
    u32 i;

    countFrames = count * historyCount;
    frameTerm = countFrames + historyCount;
    countTerm = (countFrames << 2) + count;
    allocationSize = (((count << 3) + ((countTerm + frameTerm) << 1) + (count << 1)) << 1) + sizeof(effMagatuhiValueWork_t);
    allocation = sdfMemAllocDirect(allocationSize);
    positions = sdfMemLock(allocation);
    colorTable = positions + (countFrames << 4);
    historyScalars = colorTable + (historyCount << 2);
    values = historyScalars + (countFrames << 2);
    writeIndices = values + (count << 2);
    validCounts = writeIndices + (count << 1);
    angleValues = validCounts + (count << 1);
    resource = (effMagatuhiValueWork_t *)(angleValues + (count << 4));

    resource->count = count;
    resource->historyCount = historyCount;
    resource->historyAngleStep = historyAngleStep;
    resource->historyDistanceScale = historyDistanceScale;
    resource->drawBucket = param;
    resource->positions = (f32 (*)[4])positions;
    resource->colorTable = (u32 *)colorTable;
    resource->historyScalars = (f32 *)historyScalars;
    resource->slotColors = (u32 *)values;
    resource->writeIndices = (u16 *)writeIndices;
    resource->validCounts = (u16 *)validCounts;
    resource->angleRows = (f32 (*)[4])angleValues;
    resource->allocationHandle = allocation;
    resource->texture = dds3BillboardGetGeneralTex(0);

    defaults = &D_0056AF30[0][0];
    i = 0;
    do {
        i++;
        defaults[0] = 0;
        defaults[1] = 0;
        defaults[2] = 0x400;
        defaults[3] = 0;
        defaults[4] = 0x400;
        defaults[5] = 0x400;
        defaults[6] = 0;
        defaults[7] = 0x400;
        defaults += 8;
    } while (i < 0xF);

    if (count != 0) {
        i = 0;
        do {
            effMagaCylinderSetPartPtrs(resource, i);
            i++;
        } while (i < count);
    }
    return resource;
}

/* Release the allocation retained by the render/history owner. */
void effMagaCylinderFreeParts(effMagatuhiValueWork_t *work) {
    sdfMemFreeDirect(work->allocationHandle);
}

void effSwordTrackDrawTex(effMagatuhiValueWork_t *work) {
    f32 positions[15][4];
    u32 colors[15];
    f32 offsets[15][8];
    f32 matrix[16];
    f32 point[4];
    sdfDrawTag_t *list;
    u32 packet;
    u32 *vertexPacket;
    f32 (*historyPositions)[4];
    f32 *widthFactors;
    u32 *palette;
    u32 *slotColors;
    u16 *writeIndices;
    u16 *validCounts;
    u32 historyOffset;
    u32 particleIndex;
    u32 particleCount;
    u32 slotColor;
    u32 validCount;
    u32 historyCount;
    s32 pendingCount;
    f32 baseSize;
    u16 index, historyIndex;
    f32 size, widthScale;
    u32 packed;

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    packet = sdfGrAllocPacketBuff(0x20);
    sdf3DMakeDirectRefPacket((dmaPacketHeader_t *)packet,
        (u32)sdfTexGetPacket(work->texture),
        sdfTexGetPacketLength((sdfTexHandle_t *)work->texture));
    sdfDrawTagLinkRef(list, packet);
    SDF_VU0_STOREIDENTITY(matrix);
    historyOffset = 0;
    packet = sdfGrAllocPacketBuff(0x38);
    sdfBillBoardMakeTMPacket((u8 *)packet, matrix);
    sdfDrawTagLink(list, packet);
    pendingCount = 0;
    particleCount = work->count;
    historyCount = work->historyCount;
    baseSize = work->historyDistanceScale;
    palette = work->colorTable;
    slotColors = work->slotColors;
    writeIndices = work->writeIndices;
    validCounts = work->validCounts;
    for (particleIndex = 0; particleIndex < particleCount; particleIndex++) {
        slotColor = *slotColors++;
        historyPositions = work->positions + historyOffset;
        widthFactors = work->historyScalars + historyOffset;
        validCount = *validCounts++;
        index = *writeIndices++;
        historyOffset += historyCount;
        if (slotColor & 0xFF000000) {
            SDF_VU0_COPYVECTOR(point, historyPositions[index]);
            historyIndex = 0;
            if (validCount != 0) {
                widthScale = baseSize * 4.0f;
                do {
                    f32 factor = widthFactors[index];
                    widthFactors[index] = factor * 0.75f + 0.25f;
                    size = widthScale * factor;
                    if (index == 0) {
                        index = historyCount - 1;
                    } else {
                        index--;
                    }
                    SDF_VU0_LOADVECTOR(point);
                    SDF_VU0_STOREVECTOR(positions[pendingCount]);
                    SDF_VU0_LOADVECTOR_B(historyPositions[index]);
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(point);
                    eeUnpackRGBAWordFloat(slotColor, 0.0078125f);
                    SDF_VU0_COPYVECTOR_A_TO_B();
                    eeUnpackRGBAWordFloat(palette[historyIndex], 0.0078125f);
                    SDF_VU0_MULVECTOR();
                    packed = packMagatuhiTrailColor();
                    colors[pendingCount] = packed;
                    offsets[pendingCount][0] = -size;
                    offsets[pendingCount][1] = -size;
                    offsets[pendingCount][2] = size;
                    offsets[pendingCount][3] = -size;
                    offsets[pendingCount][4] = size;
                    offsets[pendingCount][5] = size;
                    offsets[pendingCount][6] = -size;
                    offsets[pendingCount][7] = size;
                    pendingCount++;
                    if (pendingCount == 15) {
                        vertexPacket = sdfBillBoardMakeN((u128 *)positions,
                            colors, D_0056AF30, offsets, 15, 0);
                        sdfDrawTagLink(list, (u32)vertexPacket);
                        pendingCount = 0;
                    }
                    historyIndex++;
                } while (historyIndex < validCount);
            }
        }
    }
    if (pendingCount != 0) {
        vertexPacket = sdfBillBoardMakeN((u128 *)positions,
            colors, D_0056AF30, offsets, pendingCount, 0);
        sdfDrawTagLink(list, (u32)vertexPacket);
    }
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))D_00320030[work->drawBucket]->regFunc)(D_00320030[work->drawBucket], list);
}

void effMagaCylinderSetPartPos(effMagatuhiValueWork_t *work, s32 index, const f32 vector[4]) {
    f32 (*newSlot)[4];
    f32 (*replayPosition)[4];
    f32 (*historyPosition)[4];
    f32 *angleRow;
    f32 *scalarOutput;
    f32 viewDirection[4];
    f32 baseLimit = work->historyDistanceScale;
    f32 limit;
    f32 phaseScalar;
    f32 radial;
    u32 historyCount = work->historyCount;
    u32 writeIndex = work->writeIndices[index];
    u32 validCount = work->validCounts[index];
    u32 rowBase = index * historyCount;
    u32 linearIndex;
    u32 historyIndex;

    historyPosition = &work->positions[rowBase + writeIndex];
    writeIndex++;
    if (writeIndex >= historyCount) {
        writeIndex = 0;
    }

    linearIndex = rowBase + writeIndex;
    newSlot = work->positions + linearIndex;
    scalarOutput = work->historyScalars + linearIndex;
    SDF_VU0_COPYVECTOR(newSlot, vector);
    angleRow = work->angleRows[index];

    angleRow[2] += 0.21816613f;
    if (3.14159265f <= angleRow[2]) {
        angleRow[2] -= 3.14159265f;
        angleRow[3] = dds3GetRandFloatA(dds3EffectBase) * 1.5f;
    }

    phaseScalar = sdfSin(angleRow[2]);
    *scalarOutput = phaseScalar * angleRow[3] + 1.0f;

    if (validCount >= EFF_MAGATUHI_HISTORY_MINIMUM) {
        historyIndex = writeIndex < (EFF_MAGATUHI_REPLAY_GROUP_STRIDE + EFF_MAGATUHI_CONTROL_POINT_COUNT)
                           ? historyCount + writeIndex - (EFF_MAGATUHI_REPLAY_GROUP_STRIDE + EFF_MAGATUHI_CONTROL_POINT_COUNT)
                           : writeIndex - (EFF_MAGATUHI_REPLAY_GROUP_STRIDE + EFF_MAGATUHI_CONTROL_POINT_COUNT);
        replayPosition = &work->positions[rowBase + historyIndex];
        angleRow = work->angleRows[index];
        angleRow[0] += work->historyAngleStep;
        if (6.2831853f <= angleRow[0]) {
            angleRow[0] -= 6.2831853f;
            if (angleRow[1] < 1.0f) {
                angleRow[1] += (dds3GetRandFloatA(dds3EffectBase) * 0.4f + 0.60000003f) * 0.25f;
            } else if (0.5f < angleRow[1]) {
                angleRow[1] -= (dds3GetRandFloatA(dds3EffectBase) * 0.4f + 0.60000003f) * 0.25f;
            }
        }

        SDF_VU0_LOADVECTOR(aViewEye);
        SDF_VU0_LOADVECTOR_B(aViewTarget);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(vector);
        SDF_VU0_LOADVECTOR_B(historyPosition);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_COPYVECTOR_C_TO_B();
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(viewDirection);
        radial = sdfSin(angleRow[0]) * angleRow[1];
        (*replayPosition)[0] += viewDirection[0] * radial;
        (*replayPosition)[1] += viewDirection[1] * radial;
        (*replayPosition)[2] += viewDirection[2] * radial;
    }

    SDF_VU0_LOADVECTOR_B(vector);
    SDF_VU0_LOADVECTOR(historyPosition);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(radial);
    limit = baseLimit * 3.0f;
    if (limit < radial) {
        VEC3_SPLAT(viewDirection, limit);
        SDF_VU0_NORMALIZE();
        SDF_VU0_LOADVECTOR_B(viewDirection);
        SDF_VU0_MULVECTOR();
    }
    SDF_VU0_STOREVECTOR(historyPosition);

    work->writeIndices[index] = writeIndex;
    validCount++;
    if (validCount > historyCount) {
        validCount = historyCount;
    }
    work->validCounts[index] = validCount;
}

/* Store a per-slot packed color; the native path does not validate the index. */
void effMagaCylinderSetPart(effMagatuhiValueWork_t *work, s32 index, u32 color) {
    work->slotColors[index] = color;
}

/* Accumulate 1/historyCount per entry; the nominal last fraction is below 1. */
void effMagaCylinderBlendColor(effMagatuhiValueWork_t *work, u32 colorA, u32 colorB) {
    f32 blendFraction = 0.0f;
    u32 historyCount = work->historyCount;
    u32 *paletteEntry = work->colorTable;
    f32 blendStep = 1.0f / historyCount;
    u32 paletteEntryIndex;

    for (paletteEntryIndex = 0; paletteEntryIndex < historyCount; paletteEntryIndex++) {
        *paletteEntry++ = effColorBlend(colorA, colorB, blendFraction);
        blendFraction += blendStep;
    }
}

void effMagaSmokeInitPart(effMagatuhiValueWork_t *valueWork, s32 index) {
    effMagaCylinderSetPartPtrs(valueWork, index);
}

/* Clone the first-family parameters; return the work after its state array.
 * Clamp only the copied delay modulus, leaving the caller's parameters intact. */
effMagatuhiWideFirst_t *effMagaSmokeConvCreate(effMagatuhiHeadFirst_t *src) {
    u32 count = src->particleCount;
    u32 size = count * sizeof(effMagatuhiDriftParticle_t);
    sdfMemHandle_t *allocation = sdfMemAllocDirect(size + sizeof(effMagatuhiWideFirst_t));
    effMagatuhiDriftParticle_t *particle = (effMagatuhiDriftParticle_t *)sdfMemLock(allocation);
    effMagatuhiWideFirst_t *work = (effMagatuhiWideFirst_t *)((u8 *)particle + size);
    s32 spread;
    u32 particleIndex;

    work->head = *src;
    work->allocationHandle = allocation;
    work->particles = particle;
    if (work->head.delaySpread <= 0) {
        work->head.delaySpread = 1;
    }
    work->managedResource = effMagaCylinderAllocPart(&work->head.particleCount);
    work->mathResource = dds3Bezier4PCreate(count);
    spread = work->head.delaySpread;
    for (particleIndex = 0; particleIndex < count; particleIndex++) {
        particle->age = -(dds3GetRandA((u8 *)dds3EffectBase) % spread);
        particle++;
    }
    return work;
}

/* Release math state, the history owner, then the backing allocation. */
void effMagaSmokeConvDelete(effMagatuhiWideFirst_t *work) {
    dds3Bezier4PDelete(work->mathResource);
    effMagaCylinderFree(work->managedResource);
    sdfMemFreeDirect(work->allocationHandle);
}

/* Seed independent normalized XZ position/drift vectors and clear slot history.
 * The vector scratch w remains unwritten, as in the retail routine. */
void effMagaSmokeConvPartInit(effMagatuhiWideFirst_t *work, s32 index) {
    effMagatuhiDriftParticle_t *particle = &work->particles[index];
    f32 direction[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 radius;
    f32 random;
    effCubicBezierSlot_t *slot;

    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    radius = work->head.initialRadius * (random + random);
    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    direction[0] = random + random;
    direction[1] = 0.0f;
    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    direction[2] = random + random;
    VU0_LOAD_VF10_FROM(*(u128 *)direction);
    SDF_VU0_NORMALIZE();
    VU0_STORE_VF10_TO(*(u128 *)direction);
    particle->position[0] = radius * direction[0];
    particle->position[1] = 0.0f;
    particle->position[2] = radius * direction[2];
    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    particle->driftState[0] = random + random;
    particle->driftState[1] = 0.0f;
    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    particle->driftState[2] = random + random;
    SDF_VU0_LOADVECTOR(particle->driftState);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(particle->driftState);
    particle->scale = work->head.baseScale *
        (dds3GetRandFloatA(dds3EffectBase) * work->head.scaleVariation +
         (1.0f - work->head.scaleVariation));
    particle->angle = dds3GetRandFloatA(dds3EffectBase) * EFF_MAGATUHI_FULL_TURN;
    particle->liftStep = work->head.baseLiftStep *
        (dds3GetRandFloatA(dds3EffectBase) * work->head.liftVariation +
         (1.0f - work->head.liftVariation));
    slot = dds3Bezier4PGetPart(work->mathResource, index);
    slot->t = 0.0f;
    slot->step = 0.0f;
    effMagaSmokeInitPart(work->managedResource->valueWork, index);
    effMagaCylinderSetPart(work->managedResource->valueWork, index, 0);
}

/* Drift until the particle enters the capture radius, then follow a randomized
 * cubic path to the target. Fade-in takes priority over an overlapping fade-out.
 * Preserve the native unwritten point w and unchecked fade divisors. */
void effMagaSmokeConvDraw(effMagatuhiWideFirst_t *work) {
    f32 point[4];
    f32 origin[4];
    f32 basePosition[4];
    f32 target[4];
    f32 viewDirection[4];
    f32 sideAxis[4];
    f32 towardTarget[4];
    f32 startTangent[4];
    effArrHdr_t *mathResource;
    effMagatuhiValueWork_t *valueWork;
    effMagatuhiDriftParticle_t *particle;
    effCubicBezierSlot_t *slot;
    u8 respawn;
    u32 count;
    s32 life, spread, fadeIn, fadeOut;
    f32 angleStep, scaleStep;
    f32 captureDistance, pathJitter, pathStep, pathAcceleration;
    f32 radial, distance, segmentDistance, offset, random, fade;
    s32 age;
    u32 i;

    SDF_VU0_LOADVECTOR(aViewEye);
    SDF_VU0_LOADVECTOR_B(aViewTarget);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(viewDirection);
    mathResource = work->mathResource;
    valueWork = work->managedResource->valueWork;
    particle = work->particles;
    count = work->head.particleCount;
    respawn = work->head.respawn;
    life = work->head.lifetimeFrames;
    spread = work->head.delaySpread;
    angleStep = work->head.angleStep;
    scaleStep = work->head.scaleStep;
    fadeIn = work->head.fadeInFrames;
    fadeOut = work->head.fadeOutFrames;
    slot = dds3Bezier4PGetPart(mathResource, 0);
    SDF_VU0_COPYVECTOR(origin, &work->head.originX);
    captureDistance = work->head.captureDistance;
    pathJitter = work->head.pathJitter;
    pathStep = work->head.pathStep;
    pathAcceleration = work->head.pathAcceleration;
    SDF_VU0_COPYVECTOR(target, &work->head.targetX);
    for (i = 0; i < count; i++, particle++, slot++) {
        age = particle->age;
        if (age == 0) {
            effMagaSmokeConvPartInit(work, i);
        }
        if (age > 0 && age <= life) {
            if (slot->step == 0.0f) {
                SDF_VU0_COPYVECTOR(basePosition, particle->position);
                radial = particle->scale * sdfSin(particle->angle);
                particle->driftState[1] += particle->liftStep;
                point[0] = origin[0] + particle->position[0] + particle->driftState[0] * radial;
                point[1] = origin[1] + particle->position[1] + particle->driftState[1];
                point[2] = origin[2] + particle->position[2] + particle->driftState[2] * radial;
                particle->scale += scaleStep;
                particle->angle += angleStep;
                SDF_VU0_LOADVECTOR(target);
                SDF_VU0_LOADVECTOR_B(point);
                SDF_VU0_SUBVECTOR();
                SDF_VU0_GETLENGTH(distance);
                if (distance <= captureDistance) {
                    SDF_VU0_NORMALIZE();
                    SDF_VU0_STOREVECTOR(towardTarget);
                    SDF_VU0_LOADVECTOR_B(viewDirection);
                    SDF_VU0_OUTERPRODUCT();
                    SDF_VU0_NORMALIZE();
                    SDF_VU0_STOREVECTOR(sideAxis);
                    SDF_VU0_LOADVECTOR(point);
                    SDF_VU0_LOADVECTOR_B(basePosition);
                    SDF_VU0_SUBVECTOR();
                    SDF_VU0_NORMALIZE();
                    SDF_VU0_STOREVECTOR(startTangent);
                    slot->controlPoints[0][0] = point[0];
                    slot->controlPoints[0][1] = point[1];
                    slot->controlPoints[0][2] = point[2];
                    segmentDistance = captureDistance * 0.3999999762f;
                    offset = pathJitter * (dds3GetRandFloatA(dds3EffectBase) * 0.5f + 0.5f);
                    slot->controlPoints[1][0] = point[0] + towardTarget[0] * segmentDistance + startTangent[0] * offset;
                    slot->controlPoints[1][1] = point[1] + towardTarget[1] * segmentDistance + startTangent[1] * offset;
                    slot->controlPoints[1][2] = point[2] + towardTarget[2] * segmentDistance + startTangent[2] * offset;
                    segmentDistance = captureDistance * 0.6499999762f;
                    random = dds3GetRandFloatA(dds3EffectBase) - 0.5f;
                    offset = pathJitter * (random + random);
                    slot->controlPoints[2][0] = point[0] + towardTarget[0] * segmentDistance + sideAxis[0] * offset;
                    slot->controlPoints[2][1] = point[1] + towardTarget[1] * segmentDistance + sideAxis[1] * offset;
                    slot->controlPoints[2][2] = point[2] + towardTarget[2] * segmentDistance + sideAxis[2] * offset;
                    slot->controlPoints[3][0] = target[0];
                    slot->controlPoints[3][1] = target[1];
                    slot->controlPoints[3][2] = target[2];
                    slot->step = pathStep;
                }
            } else {
                dds3Bezier4PPartRun(mathResource, i, point);
                slot->step *= pathAcceleration;
            }
            if (age < fadeIn) {
                fade = (f32)age / (f32)fadeIn;
            } else if (life - age <= fadeOut) {
                fade = (f32)(life - age) / (f32)fadeOut;
            } else {
                fade = 1.0f;
            }
            effMagaCylinderSetPart(valueWork, i, ((u32)(fade * 127.0f) << 24) | 0x808080);
            effMagaCylinderSetPartPos(valueWork, i, point);
        }
        if (age >= life && respawn) {
            particle->age = -(dds3GetRandA((u8 *)dds3EffectBase) % spread);
        } else {
            particle->age++;
        }
    }
    effMagaCylinderDrawTrack(work->managedResource);
}

/* Update the first family's scattered float parameters without touching gaps. */
void effMagaSCSetArea(effMagatuhiCallback_t *work, effMagatuhiFloatParams_t *src) {
    effMagatuhiWideFirst_t *dst = dds3MagatuhiGetHandle(work->effect);

    dst->head.originX = src->originX;
    dst->head.originY = src->originY;
    dst->head.originZ = src->originZ;
    dst->head.initialRadius = src->initialRadius;
    dst->head.targetX = src->targetX;
    dst->head.targetY = src->targetY;
    dst->head.targetZ = src->targetZ;
    dst->head.captureDistance = src->captureDistance;
}

/* Clone history parameters; signed delays follow the returned work block. */
effMagatuhiWideSecond_t *effMagaPoint4Create(effMagatuhiHeadSecond_t *src) {
    u32 count = src->particleCount;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(count * EFF_MAGATUHI_DELAY_WORD_BYTES + sizeof(effMagatuhiWideSecond_t));
    effMagatuhiWideSecond_t *work = (effMagatuhiWideSecond_t *)sdfMemLock(allocation);
    s32 *delays = (s32 *)(work + 1);
    s32 spread;
    u32 i;

    work->head = *src;
    work->allocationHandle = allocation;
    work->delays = delays;
    if (work->head.delaySpread <= 0) {
        work->head.delaySpread = 1;
    }
    work->managedResource = effMagaCylinderAllocPart(&work->head.particleCount);
    work->mathResource = dds3Bezier4PCreate(count);
    spread = work->head.delaySpread;
    for (i = 0; i < count; i++) {
        *delays++ = -(dds3GetRandA((u8 *)dds3EffectBase) % spread);
    }
    return work;
}

/* Release Bezier slots, their history owner, then the backing allocation. */
void effMagaPoint4Delete(effMagatuhiWideSecond_t *work) {
    dds3Bezier4PDelete(work->mathResource);
    effMagaCylinderFree(work->managedResource);
    sdfMemFreeDirect(work->allocationHandle);
}

/* Jitter control points perpendicular to the path and camera viewing direction. */
void effMagaPoint4PartInit(effMagatuhiWideSecond_t *work, s32 index) {
    f32 scale[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 lastNormal[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 viewDirection[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 point[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    effCubicBezierSlot_t *slot;
    f32 step;
    f32 random;

    SDF_VU0_LOADVECTOR(aViewEye);
    SDF_VU0_LOADVECTOR_B(aViewTarget);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(viewDirection);
    step = 1.0f / (f32)work->head.lifetimeFrames;
    slot = dds3Bezier4PGetPart(work->mathResource, index);
    slot->t = 0;
    slot->step = step;

    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    scale[0] = work->head.jitterScales[0] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    SDF_VU0_LOADVECTOR(work->head.controlPoints[0]);
    SDF_VU0_LOADVECTOR_B(work->head.controlPoints[1]);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(viewDirection);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(point);
    slot->controlPoints[0][0] = point[0];
    slot->controlPoints[0][1] = point[1];
    slot->controlPoints[0][2] = point[2];

    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    scale[0] = work->head.jitterScales[1] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    SDF_VU0_LOADVECTOR(work->head.controlPoints[1]);
    SDF_VU0_LOADVECTOR_B(work->head.controlPoints[2]);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(viewDirection);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(point);
    slot->controlPoints[1][0] = point[0];
    slot->controlPoints[1][1] = point[1];
    slot->controlPoints[1][2] = point[2];

    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    scale[0] = work->head.jitterScales[2] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    SDF_VU0_LOADVECTOR(work->head.controlPoints[2]);
    SDF_VU0_LOADVECTOR_B(work->head.controlPoints[3]);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(viewDirection);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(lastNormal);
    SDF_VU0_LOADVECTOR_B(scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(point);
    slot->controlPoints[2][0] = point[0];
    slot->controlPoints[2][1] = point[1];
    slot->controlPoints[2][2] = point[2];

    /* The fourth point reuses the third segment's normalized side direction. */
    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    scale[0] = work->head.jitterScales[3] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    SDF_VU0_LOADVECTOR(lastNormal);
    SDF_VU0_LOADVECTOR_B(scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(work->head.controlPoints[3]);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(point);
    slot->controlPoints[3][0] = point[0];
    slot->controlPoints[3][1] = point[1];
    slot->controlPoints[3][2] = point[2];
    effMagaSmokeInitPart(work->managedResource->valueWork, index);
    effMagaCylinderSetPart(work->managedResource->valueWork, index, 0);
}

/* Advance delayed Bezier particles, pack their fade, and submit the owner.
 * Fade-in wins when its interval overlaps fade-out; divisors are unchecked. */
void effMagaPoint4Draw(effMagatuhiWideSecond_t *work) {
    effArrHdr_t *slots = work->mathResource;
    effMagatuhiValueWork_t *valueWork = work->managedResource->valueWork;
    s32 *delays = work->delays;
    u8 respawn = work->head.respawn;
    u32 count = work->head.particleCount;
    s32 life = work->head.lifetimeFrames;
    s32 spread = work->head.delaySpread;
    s32 fadeIn = work->head.fadeIn;
    s32 fadeOut = work->head.fadeOut;
    f32 out[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 fade;
    s32 frame;
    u32 particleIndex;

    /* Retain the native slot lookup even though its returned address is unused. */
    dds3Bezier4PGetPart(slots, 0);
    for (particleIndex = 0; particleIndex < count; particleIndex++) {
        frame = *delays;
        if (frame == 0) {
            effMagaPoint4PartInit(work, particleIndex);
        }
        if (frame > 0 && frame <= life) {
            dds3Bezier4PPartRun(slots, particleIndex, out);
            if (fadeIn > frame) {
                fade = (f32)frame / (f32)fadeIn;
            } else if (life - frame <= fadeOut) {
                fade = (f32)(life - frame) / (f32)fadeOut;
            } else {
                fade = 1.0f;
            }
            effMagaCylinderSetPart(valueWork, particleIndex, ((u32)(fade * EFF_MAGATUHI_FADE_ALPHA_SCALE) << EFF_MAGATUHI_ALPHA_SHIFT) | EFF_MAGATUHI_NEUTRAL_RGB);
            effMagaCylinderSetPartPos(valueWork, particleIndex, out);
        }
        if (frame >= life && respawn) {
            *delays = -(dds3GetRandA((u8 *)dds3EffectBase) % spread);
        } else {
            (*delays)++;
        }
        delays++;
    }
    effMagaCylinderDrawTrack(work->managedResource);
}

/* Replace control-point xyz and jitter amplitudes without changing point w. */
void effMagaP4SetPoint(effMagatuhiCallback_t *work, effMagatuhiRowsSrc_t *src) {
    effMagatuhiWideSecond_t *dst = dds3MagatuhiGetHandle(work->effect);
    u32 controlPointIndex;

    for (controlPointIndex = 0; controlPointIndex < EFF_MAGATUHI_CONTROL_POINT_COUNT; controlPointIndex++) {
        dst->head.controlPoints[controlPointIndex][0] = src->controlPoints[controlPointIndex][0];
        dst->head.controlPoints[controlPointIndex][1] = src->controlPoints[controlPointIndex][1];
        dst->head.controlPoints[controlPointIndex][2] = src->controlPoints[controlPointIndex][2];
        dst->head.jitterScales[controlPointIndex] = src->jitterScales[controlPointIndex];
    }
}

/* Seed every fourth slot with interpolated samples between two curve positions.
 * Keep the asymmetric duration/age subtraction and unchecked span divisor. */
void effMagaP4Randomize(effMagatuhiCallback_t *callback) {
    effMagatuhiWideSecond_t *work = dds3MagatuhiGetHandle(callback->effect);
    effMagatuhiValueWork_t *valueWork = work->managedResource->valueWork;
    u32 historyFrames = work->head.duration;
    s32 lifetimeFrames = work->head.lifetimeFrames;
    u32 count = work->head.particleCount;
    s32 *ages = work->delays;
    f32 samplePosition[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 startPosition[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 endPosition[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 blendFraction;
    f32 blendStep;
    u32 particleIndex;
    u32 sampleIndex;
    s32 sampleSpan;
    effCubicBezierSlot_t *slot;

    for (particleIndex = 0; particleIndex < count; particleIndex += EFF_MAGATUHI_BEZIER_GROUP_STRIDE) {
        effMagaPoint4PartInit(work, particleIndex);
        *ages = dds3GetRandA((u8 *)dds3EffectBase) % lifetimeFrames;
        slot = dds3Bezier4PGetPart(work->mathResource, particleIndex);
        if (historyFrames < *ages) {
            slot->t = slot->step * (f32)(*ages - historyFrames);
            sampleSpan = historyFrames;
        } else {
            slot->t = 0.0f;
            sampleSpan = historyFrames - *ages;
        }
        blendFraction = 0.0f;
        dds3Bezier4PPartRun(work->mathResource, particleIndex, startPosition);
        slot->t = slot->step * (f32)*ages;
        dds3Bezier4PPartRun(work->mathResource, particleIndex, endPosition);
        blendStep = 1.0f / (f32)sampleSpan;
        for (sampleIndex = 0; sampleIndex < sampleSpan; sampleIndex++) {
            SDF_VU0_LOADVECTOR(startPosition);
            SDF_VU0_LOADVECTOR_B(endPosition);
            SDF_VU0_INTERPOLATE(blendFraction);
            SDF_VU0_STOREVECTOR(samplePosition);
            effMagaCylinderSetPartPos(valueWork, particleIndex, samplePosition);
            blendFraction += blendStep;
        }
        (*ages)++;
        ages += EFF_MAGATUHI_BEZIER_GROUP_STRIDE;
    }
}

/* Clone ring parameters; the returned work precedes its individual slots. */
effMagatuhiRingWork_t *effMagaCylinderCreate(effMagatuhiRingParams_t *src) {
    u32 count = src->particleCount;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(count * sizeof(effMagatuhiRingParticle_t) + sizeof(effMagatuhiRingWork_t));
    effMagatuhiRingWork_t *work = (effMagatuhiRingWork_t *)sdfMemLock(allocation);
    effMagatuhiRingParticle_t *particle = (effMagatuhiRingParticle_t *)(work + 1);
    s32 spread;
    u32 i;

    work->head = *src;
    work->particles = particle;
    work->tintColor = EFF_MAGATUHI_NEUTRAL_COLOR;
    work->allocationHandle = allocation;
    SDF_VU0_STOREIDENTITY(work->matrix);
    if (work->head.delaySpread <= 0) {
        work->head.delaySpread = 1;
    }
    work->managedResource = effMagaCylinderAllocPart(&work->head.particleCount);
    spread = work->head.delaySpread;
    for (i = 0; i < count; i++) {
        particle->age = -(dds3GetRandA((u8 *)dds3EffectBase) % spread);
        particle++;
    }
    return work;
}

/* Release the ring history owner before the work/state allocation. */
void effMagaCylinderDelete(effMagatuhiRingWork_t *work) {
    effMagaCylinderFree(work->managedResource);
    sdfMemFreeDirect(work->allocationHandle);
}

/* Initialize a ring slot and reset the history/value owned by that slot. */
void effMagaCylinderPartInit(effMagatuhiRingWork_t *work, s32 index) {
    effMagatuhiRingParticle_t *elem = &work->particles[index];
    f32 blend;
    f32 t;

    elem->age = 0;
    elem->height = -work->head.heightSpread * dds3GetRandFloatA(dds3EffectBase);
    elem->angle = dds3GetRandFloatA(dds3EffectBase) * (EFF_MAGATUHI_HALF_TURN * 2.0f);
    blend = work->head.angleJitter;
    elem->angleStep = work->head.angleStep * (dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend));
    blend = work->head.startJitter;
    elem->radius = work->head.startRadius * (dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend));
    blend = work->head.endJitter;
    t = dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend);
    elem->radiusStep = (work->head.endRadius * t - elem->radius) / (f32)work->head.lifetimeFrames;
    effMagaSmokeInitPart(work->managedResource->valueWork, index);
    effMagaCylinderSetPart(work->managedResource->valueWork, index, 0);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", effMagaCylinderDraw);

/* Copy the ring origin as one quadword, including its fourth component. */
void effMagaCylinderSetPosition(effMagatuhiRingWork_t *work, void *origin) {
    SDF_VU0_COPYVECTOR(work->head.origin, origin);
}

/* The historical symbol names a resource, but this slot is packed color. */
void effMagaCylinderSetRGBA(effMagatuhiRingWork_t *work, u32 color) {
    work->tintColor = color;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagaCylinderSetRotateMatrix(effMagatuhiRingWork_t *work, void *matrix) {
    VU0_COPY_MATRIX(work->matrix, matrix);
}

/* Replay the first slot of each three-slot group from a random age. */
void effMagaCylinderRandomize(effMagatuhiCallback_t *callback) {
    u32 k;
    effMagatuhiRingWork_t *work;
    effMagatuhiValueWork_t *valueWork;
    effMagatuhiRingParticle_t *particle;
    u32 count;
    s32 frames;
    u32 maxSteps;
    u32 i;
    u32 steps;
    s32 frameOffset; /* random age, then the initial state offset skipped before replay */
    f32 out[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 origin[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 height;
    f32 angle;
    f32 radius;
    f32 angleStep;
    f32 radiusStep;

    if (callback->type == EFF_MAGATUHI_CALLBACK_TYPE) {
        if (dds3MagatuhiGetType(callback->effect) == EFF_MAGATUHI_REPLAY_RING) {
            work = dds3MagatuhiGetHandle(callback->effect);
            count = work->head.particleCount;
            valueWork = work->managedResource->valueWork;
            maxSteps = work->head.maxSteps;
            frames = work->head.lifetimeFrames;
            particle = work->particles;
            SDF_VU0_COPYVECTOR(origin, work->head.origin);
            SDF_VU0_LOADMATRIX(work->matrix);
            for (i = 0; i < count; i += EFF_MAGATUHI_REPLAY_GROUP_STRIDE, particle += EFF_MAGATUHI_REPLAY_GROUP_STRIDE) {
                effMagaCylinderPartInit(work, i);
                frameOffset = dds3GetRandA((u8 *)dds3EffectBase) % frames;
                particle->age = frameOffset;
                /* Keep at most maxSteps samples: skip older state only when
                 * the random age exceeds that window; otherwise start at zero. */
                if (maxSteps < frameOffset) {
                    steps = maxSteps;
                    frameOffset -= steps;
                } else {
                    steps = maxSteps - frameOffset;
                    frameOffset = 0;
                }
                angleStep = particle->angleStep;
                radiusStep = particle->radiusStep;
                height = particle->height;
                angle = particle->angle + angleStep * (f32)frameOffset;
                radius = particle->radius + radiusStep * (f32)frameOffset;
                out[3] = 0;
                for (k = 0; k < steps; k++) {
                    out[0] = sdfCos(angle) * radius;
                    out[1] = height;
                    out[2] = sdfSin(angle) * radius;
                    SDF_VU0_LOADVECTOR_B(origin);
                    SDF_VU0_LOADVECTOR(out);
                    SDF_VU0_APPLYMATRIX();
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(out);
                    effMagaCylinderSetPartPos(valueWork, i, out);
                    angle += angleStep;
                    radius += radiusStep;
                }
                particle->radius = radius;
                particle->angle = angle;
            }
        }
    }
}

/* Clone orbit parameters; the returned work precedes its individual slots. */
effMagatuhiOrbitWork_t *effMagaSpiralCreate(effMagatuhiOrbitParams_t *src) {
    u32 count = src->particleCount;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(count * sizeof(effMagatuhiOrbitParticle_t) + sizeof(effMagatuhiOrbitWork_t));
    effMagatuhiOrbitWork_t *work = (effMagatuhiOrbitWork_t *)sdfMemLock(allocation);
    effMagatuhiOrbitParticle_t *particle = (effMagatuhiOrbitParticle_t *)(work + 1);
    s32 spread;
    u32 i;

    work->head = *src;
    work->particles = particle;
    work->tintColor = EFF_MAGATUHI_NEUTRAL_COLOR;
    work->allocationHandle = allocation;
    SDF_VU0_STOREIDENTITY(work->matrix);
    if (work->head.delaySpread <= 0) {
        work->head.delaySpread = 1;
    }
    work->managedResource = effMagaCylinderAllocPart(&work->head.particleCount);
    spread = work->head.delaySpread;
    for (i = 0; i < count; i++) {
        particle->age = -(dds3GetRandA((u8 *)dds3EffectBase) % spread);
        particle++;
    }
    return work;
}

/* Release the orbit history owner before the work/state allocation. */
void effMagaSpiralDelete(effMagatuhiOrbitWork_t *work) {
    effMagaCylinderFree(work->managedResource);
    sdfMemFreeDirect(work->allocationHandle);
}

/* Seed randomized height/angular/radial increments and clear the slot history. */
void effMagaSpiralPartInit(effMagatuhiOrbitWork_t *work, s32 index) {
    effMagatuhiOrbitParticle_t *elem = &work->particles[index];
    f32 blend;
    f32 t;

    elem->age = 0;
    elem->height = 0;
    blend = work->head.heightJitter;
    elem->heightStep = work->head.heightStep * (dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend));
    elem->angle = dds3GetRandFloatA(dds3EffectBase) * (EFF_MAGATUHI_HALF_TURN * 2.0f);
    blend = work->head.angleJitter;
    elem->angleStep = work->head.angleStep * (dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend));
    blend = work->head.startJitter;
    elem->radius = work->head.startRadius * (dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend));
    blend = work->head.endJitter;
    t = dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend);
    elem->radiusStep = (work->head.endRadius * t - elem->radius) / (f32)work->head.lifetimeFrames;
    effMagaSmokeInitPart(work->managedResource->valueWork, index);
    effMagaCylinderSetPart(work->managedResource->valueWork, index, 0);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", effMagaSpiralDraw);

/* Copy the orbit origin as one quadword, preserving the source w. */
void effMagaSpiralSetPosition(effMagatuhiOrbitWork_t *work, void *origin) {
    SDF_VU0_COPYVECTOR(work->head.origin, origin);
}

/* This callback sets the packed orbit color, not an allocation handle. */
void effMagaSpiralSetRGBA(effMagatuhiOrbitWork_t *work, u32 color) {
    work->tintColor = color;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagaSpiralSetRotateMatrix(effMagatuhiOrbitWork_t *work, void *matrix) {
    VU0_COPY_MATRIX(work->matrix, matrix);
}

/* Advance the orbiting-particle family: each group of three slots is replayed from a random delay, one orbit step at a time, into the value table. */
void effMagaSpiralRandomize(effMagatuhiCallback_t *callback) {
    u32 k;
    effMagatuhiOrbitWork_t *work;
    effMagatuhiValueWork_t *valueWork;
    effMagatuhiOrbitParticle_t *particle;
    u32 count;
    s32 frames;
    u32 maxSteps;
    u32 i;
    u32 steps;
    s32 frameOffset; /* random age, then the initial state offset skipped before replay */
    f32 out[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 origin[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 height;
    f32 angle;
    f32 radius;
    f32 heightStep;
    f32 angleStep;
    f32 radiusStep;

    if (callback->type == EFF_MAGATUHI_CALLBACK_TYPE) {
        if (dds3MagatuhiGetType(callback->effect) == EFF_MAGATUHI_REPLAY_ORBIT) {
            work = dds3MagatuhiGetHandle(callback->effect);
            count = work->head.particleCount;
            valueWork = work->managedResource->valueWork;
            maxSteps = work->head.maxSteps;
            frames = work->head.lifetimeFrames;
            particle = work->particles;
            SDF_VU0_COPYVECTOR(origin, work->head.origin);
            SDF_VU0_LOADMATRIX(work->matrix);
            for (i = 0; i < count; i += EFF_MAGATUHI_REPLAY_GROUP_STRIDE, particle += EFF_MAGATUHI_REPLAY_GROUP_STRIDE) {
                effMagaSpiralPartInit(work, i);
                frameOffset = dds3GetRandA((u8 *)dds3EffectBase) % frames;
                particle->age = frameOffset;
                /* Keep at most maxSteps samples: skip older state only when
                 * the random age exceeds that window; otherwise start at zero. */
                if (maxSteps < frameOffset) {
                    steps = maxSteps;
                    frameOffset -= steps;
                } else {
                    steps = maxSteps - frameOffset;
                    frameOffset = 0;
                }
                angleStep = particle->angleStep;
                radiusStep = particle->radiusStep;
                heightStep = particle->heightStep;
                angle = particle->angle + angleStep * (f32)frameOffset;
                radius = particle->radius + radiusStep * (f32)frameOffset;
                height = particle->height + heightStep * (f32)frameOffset;
                out[3] = 0;
                for (k = 0; k < steps; k++) {
                    out[0] = sdfCos(angle) * radius;
                    out[1] = height;
                    out[2] = sdfSin(angle) * radius;
                    SDF_VU0_LOADVECTOR_B(origin);
                    SDF_VU0_LOADVECTOR(out);
                    SDF_VU0_APPLYMATRIX();
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(out);
                    effMagaCylinderSetPartPos(valueWork, i, out);
                    height += heightStep;
                    angle += angleStep;
                    radius += radiusStep;
                }
                particle->height = height;
                particle->radius = radius;
                particle->angle = angle;
            }
        }
    }
}

/* Clone drift parameters; return the work after its 0x30-byte state array. */
effMagatuhiDriftWork_t *effMagaSmokeCreate(effMagatuhiDriftParams_t *src) {
    u32 count = src->particleCount;
    u32 size = count * sizeof(effMagatuhiDriftParticle_t);
    sdfMemHandle_t *allocation = sdfMemAllocDirect(size + sizeof(effMagatuhiDriftWork_t));
    effMagatuhiDriftParticle_t *particle = (effMagatuhiDriftParticle_t *)sdfMemLock(allocation);
    effMagatuhiDriftWork_t *work = (effMagatuhiDriftWork_t *)((u8 *)particle + size);
    s32 spread;
    u32 i;

    work->head = *src;
    work->particles = particle;
    work->tintColor = EFF_MAGATUHI_NEUTRAL_COLOR;
    work->allocationHandle = allocation;
    SDF_VU0_STOREIDENTITY(work->matrix);
    if (work->head.delaySpread <= 0) {
        work->head.delaySpread = 1;
    }
    work->managedResource = effMagaCylinderAllocPart(&work->head.particleCount);
    spread = work->head.delaySpread;
    for (i = 0; i < count; i++) {
        particle->age = -(dds3GetRandA((u8 *)dds3EffectBase) % spread);
        particle++;
    }
    return work;
}

/* Release the drift history owner before its states-plus-work allocation. */
void effMagaSmokeDelete(effMagatuhiDriftWork_t *work) {
    effMagaCylinderFree(work->managedResource);
    sdfMemFreeDirect(work->allocationHandle);
}

/* vu0 routine: initialize a planar particle with normalized position/drift. */
void effMagaSmokePartInit(effMagatuhiDriftWork_t *work, s32 index) {
    effMagatuhiDriftParticle_t *particle = &work->particles[index];
    f32 direction[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 radius;
    f32 random;

    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    radius = work->head.initialRadius * (random + random);
    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    direction[0] = random + random;
    direction[1] = 0.0f;
    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    direction[2] = random + random;
    VU0_LOAD_VF10_FROM(*(u128 *)direction);
    SDF_VU0_NORMALIZE();
    VU0_STORE_VF10_TO(*(u128 *)direction);
    particle->position[0] = radius * direction[0];
    particle->position[1] = 0.0f;
    particle->position[2] = radius * direction[2];
    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    particle->driftState[0] = random + random;
    particle->driftState[1] = 0.0f;
    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    particle->driftState[2] = random + random;
    SDF_VU0_LOADVECTOR(particle->driftState);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(particle->driftState);
    particle->scale = work->head.initialScale *
        (dds3GetRandFloatA(dds3EffectBase) * work->head.initialScaleRandomness +
         (1.0f - work->head.initialScaleRandomness));
    particle->angle = dds3GetRandFloatA(dds3EffectBase) * EFF_MAGATUHI_FULL_TURN;
    particle->liftStep = work->head.initialLift *
        (dds3GetRandFloatA(dds3EffectBase) * work->head.initialLiftRandomness +
         (1.0f - work->head.initialLiftRandomness));
    effMagaSmokeInitPart(work->managedResource->valueWork, index);
    effMagaCylinderSetPart(work->managedResource->valueWork, index, 0);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", effMagaSmokeDraw);

/* Copy the drift origin as one quadword, preserving the source w. */
void effMagaSmokeSetPosition(effMagatuhiDriftWork_t *work, void *origin) {
    SDF_VU0_COPYVECTOR(work->head.origin, origin);
}

/* This callback sets packed drift color; +0x120 is not an owner pointer. */
void effMagaSmokeSetRGBA(effMagatuhiDriftWork_t *work, u32 color) {
    work->tintColor = color;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagaSmokeSetRotateMatrix(effMagatuhiDriftWork_t *work, void *matrix) {
    VU0_COPY_MATRIX(work->matrix, matrix);
}

/* Replay the first slot of each three-slot drift group from a random age. */
void effMagaSmokeRandomize(effMagatuhiCallback_t *callback) {
    u32 k;
    effMagatuhiDriftWork_t *work;
    effMagatuhiValueWork_t *valueWork;
    effMagatuhiDriftParticle_t *particle;
    u32 count;
    s32 frames;
    u32 maxSteps;
    u32 i;
    u32 steps;
    s32 frameOffset; /* random age, then the initial state offset skipped before replay */
    f32 out[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 origin[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 lift;
    f32 angle;
    f32 scale;
    f32 liftStep;
    f32 angleStep;
    f32 scaleStep;

    if (callback->type == EFF_MAGATUHI_CALLBACK_TYPE) {
        if (dds3MagatuhiGetType(callback->effect) == EFF_MAGATUHI_REPLAY_DRIFT) {
            work = dds3MagatuhiGetHandle(callback->effect);
            count = work->head.particleCount;
            valueWork = work->managedResource->valueWork;
            maxSteps = work->head.maxSteps;
            frames = work->head.lifetimeFrames;
            particle = work->particles;
            SDF_VU0_COPYVECTOR(origin, work->head.origin);
            SDF_VU0_LOADMATRIX(work->matrix);
            for (i = 0; i < count; i += EFF_MAGATUHI_REPLAY_GROUP_STRIDE, particle += EFF_MAGATUHI_REPLAY_GROUP_STRIDE) {
                effMagaSmokePartInit(work, i);
                frameOffset = dds3GetRandA((u8 *)dds3EffectBase) % frames;
                particle->age = frameOffset;
                /* Keep at most maxSteps samples: skip older state only when
                 * the random age exceeds that window; otherwise start at zero. */
                if (maxSteps < frameOffset) {
                    steps = maxSteps;
                    frameOffset -= steps;
                } else {
                    steps = maxSteps - frameOffset;
                    frameOffset = 0;
                }
                angleStep = work->head.angleStep;
                scaleStep = work->head.scaleStep;
                liftStep = particle->liftStep;
                angle = particle->angle + angleStep * (f32)frameOffset;
                scale = particle->scale + scaleStep * (f32)frameOffset;
                lift = particle->driftState[1] + liftStep * (f32)frameOffset;
                out[3] = 0;
                for (k = 0; k < steps; k++) {
                    /* Native ordering: lift advances before sampling, scale/angle after. */
                    lift += liftStep;
                    /* Keep the sine evaluation even though its result is unused here. */
                    sdfSin(angle);
                    out[0] = particle->position[0] + particle->driftState[0] * scale;
                    out[1] = particle->position[1] + lift;
                    out[2] = particle->position[2] + particle->driftState[2] * scale;
                    SDF_VU0_LOADVECTOR_B(origin);
                    SDF_VU0_LOADVECTOR(out);
                    SDF_VU0_APPLYMATRIX();
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(out);
                    effMagaCylinderSetPartPos(valueWork, i, out);
                    scale += scaleStep;
                    angle += angleStep;
                }
                particle->scale = scale;
                particle->angle = angle;
                particle->driftState[1] = lift;
            }
        }
    }
}

/* Dispatch replay side effects; retain the native s32 definition's fall-through. */
s32 effMagaRandomize(effMagatuhiCallback_t *work) {
    if (work->type == EFF_MAGATUHI_CALLBACK_TYPE) {
        switch (dds3MagatuhiGetType(work->effect)) {
        case EFF_MAGATUHI_REPLAY_NONE:
            break;
        case EFF_MAGATUHI_REPLAY_BEZIER:
            effMagaP4Randomize(work);
            break;
        case EFF_MAGATUHI_REPLAY_RING:
            effMagaCylinderRandomize(work);
            break;
        case EFF_MAGATUHI_REPLAY_ORBIT:
            effMagaSpiralRandomize(work);
            break;
        case EFF_MAGATUHI_REPLAY_DRIFT:
            effMagaSmokeRandomize(work);
            break;
        }
    }
}

/* Allocate the dispatch owner before calling the selected payload constructor. */
effWork_t *dds3MagatuhiParCreate(s32 type, void *params) {
    effWork_t *work = sdfChipAllocDirect(sizeof(*work));
    void *payload = D_00320040[type].handler(params);

    work->type = type;
    work->payload = payload;
    return work;
}

/* Invoke the primary type callback with the payload; no NULL or type bounds check. */
void dds3MagatuhiParDraw(effWork_t *work) {
    D_00320044[work->type].handler(work->payload);
}

/* Invoke the release callback, then free the outer work; no callback NULL check. */
void dds3MagatuhiParDelete(effWork_t *work) {
    D_00320048[work->type].handler(work->payload);
    sdfChipFreeDirect(work);
}

/* Return the retained payload passed to this type's callbacks. */
void *dds3MagatuhiGetHandle(effWork_t *work) {
    return work->payload;
}

/* Read the dispatch owner's type word; the pointer remains unchecked. */
u32 dds3MagatuhiGetType(effWork_t *work) {
    return work->type;
}

void func_001A8CD0(void) {
}

/* Native node type-three condition slot: accept with the constant result one. */
u32 dds3MagatuhiParIsActive(void) {
    return 1;
}

/* Node set-vector slot: invoke optional subtype callback A; preserve the one-formal surface. */
void dds3MagatuhiParSetPosition(effWork_t *work) {
    void (*handler)(void *) = D_0032004C[work->type].handler;

    if (handler != NULL) {
        handler(work->payload);
    }
}

/* Node set-matrix slot: invoke optional subtype callback B; preserve the one-formal surface. */
void dds3MagatuhiParSetRotateMatrix(effWork_t *work) {
    void (*handler)(void *) = D_00320054[work->type].handler;

    if (handler != NULL) {
        handler(work->payload);
    }
}

/* Node parameter-word slot: invoke optional subtype callback C; preserve the one-formal surface. */
void dds3MagatuhiParSetRGBA(effWork_t *work) {
    void (*handler)(void *) = D_00320050[work->type].handler;

    if (handler != NULL) {
        handler(work->payload);
    }
}

/* Store the low byte of slotValue at the type-specific subslot.
   Types two through four share the same location; other types are left unchanged. */
void dds3MagatuhiParSetByType(effWork_t *work, s32 slotValue) {
    u8 slotByte = slotValue;
    u32 type = work->type;

    switch (type) {
    case 0:
        ((effSub_t *)work->payload)->type0SubSlot = slotByte;
        return;
    case 1:
        ((effSub_t *)work->payload)->type1SubSlot = slotByte;
        return;
    case 2:
        ((effSub_t *)work->payload)->type2To4SubSlot = slotByte;
        return;
    case 3:
        ((effSub_t *)work->payload)->type2To4SubSlot = slotByte;
        return;
    case 4:
        ((effSub_t *)work->payload)->type2To4SubSlot = slotByte;
        return;
    default:
        return;
    }
}

/* Read the type-specific subslot, or zero for other types.
   The callback's second argument is unused. */
u32 func_001A8DF8(effWork_t *work, s32 unusedSlotValue) {
    u32 type = work->type;

    switch (type) {
    case 0:
        return ((effSub_t *)work->payload)->type0SubSlot;
    case 1:
        return ((effSub_t *)work->payload)->type1SubSlot;
    case 2:
        return ((effSub_t *)work->payload)->type2To4SubSlot;
    case 3:
        return ((effSub_t *)work->payload)->type2To4SubSlot;
    case 4:
        return ((effSub_t *)work->payload)->type2To4SubSlot;
    default:
        break;
    }
    return 0;
}

/* Types zero/one dispatch from the payload base; two through four skip its prefix.
   Other types still dispatch with a zero argument; no type bounds check is added. */
void dds3MagatuhiParCopy(effWork_t *work) {
    u32 type = work->type;
    void *handlerArg = NULL;

    switch (type) {
    case 0:
        handlerArg = work->payload;
        break;
    case 1:
        handlerArg = work->payload;
        break;
    case 2:
        handlerArg = (u8 *)work->payload + EFF_SUBWORK_PREFIX_BYTES;
        break;
    case 3:
        handlerArg = (u8 *)work->payload + EFF_SUBWORK_PREFIX_BYTES;
        break;
    case 4:
        handlerArg = (u8 *)work->payload + EFF_SUBWORK_PREFIX_BYTES;
        break;
    default:
        break;
    }
    dds3MagatuhiParCreate(type, handlerArg);
}

void func_001A8EA8(void) {
}

void InitMagatuhiEditorProcess(void) {
    dds3AdminiCallSequence(0, 0, 0, 0);
}

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00533EF0);

INCLUDE_ASM(const s32, "effect/effMagatuhi", dblurUpdateHandle);

INCLUDE_ASM(const s32, "effect/effMagatuhi", effDebugDrawRect3);

INCLUDE_ASM(const s32, "effect/effMagatuhi", effDebugDrawRect2);

INCLUDE_ASM(const s32, "effect/effMagatuhi", effDebugDrawRectLine);

INCLUDE_ASM(const s32, "effect/effMagatuhi", effDebugDrawMenu);

INCLUDE_ASM(const s32, "effect/effMagatuhi", effDebugAllocDrawTag);

INCLUDE_ASM(const s32, "effect/effMagatuhi", effDebugLinkDrawTag);

void effDebugSetPos(s32 x, s32 y) {
    effDebugPosX = x;
    effDebugPosY = y;
}

void effDebugSetAttr(u32 attributes) {
    D_00551DDC = attributes;
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", effDebugPrintf);

INCLUDE_ASM(const s32, "effect/effMagatuhi", effDebugDrawRectFull);

INCLUDE_ASM(const s32, "effect/effMagatuhi", effDebugDrawRect4);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_001AA310);

void effDebugInit(void) {
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", eff_1AA3B0);

INCLUDE_ASM(const s32, "effect/effMagatuhi", eff_1AA550);

void func_001AA598(void) {
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_001AA5A0);

INCLUDE_ASM(const s32, "effect/effMagatuhi", func_001AA6D0);

s32 dds3_1AACF8(void) {
    return D_0054F220;
}

/* Debug filesystem mode opens the formatted pfs0 directory; otherwise reset built-in iteration.
   Return the native directory-open result, or zero for built-in mode. */
s32 dds3EffectDirOpen(void *name) {
    u8 path[EFF_DIRECTORY_PATH_BYTES];

    if (sdfHddValid != 0) {
        sprintf(path, D_0054F260, name);
        return sceDopen(path);
    } else {
        D_00551DE4 = 0;
        return 0;
    }
}

/* Run the native directory callback only in debug filesystem mode; its result is ignored. */
void dds3EffectDirClose(s32 directory) {
    if (sdfHddValid != 0) {
        sceDclose(directory);
    }
}

/* Debug mode delegates to the native iterator. Built-in mode copies the next name,
   clears flag mask 0x1000, and returns name length; zero also marks table exhaustion. */
s32 dds3EffectDirRead(s32 directory, effDirEnt_t *entry) {
    if (sdfHddValid != 0) {
        return sceDread(directory, entry);
    }
    if ((u32)D_00551DE4 >= EFF_BUILTIN_NAME_COUNT) {
        return 0;
    }
    strcpy(entry->name, dds3EffectFileNameTable[D_00551DE4]);
    entry->flags &= ~EFF_DIRECTORY_FLAG_CLEAR;
    D_00551DE4++;
    return strlen(entry->name);
}

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534090);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_005340A0);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_005340B0);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_005340C0);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_005340D0);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_005340E0);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_005340F0);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534100);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534110);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534120);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534130);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534140);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534150);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534160);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534170);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534180);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534190);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_005341A0);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_005341B0);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_005341C0);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_005341D0);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_005341E0);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_005341F0);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534200);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534210);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534220);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534230);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534240);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534250);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534260);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534270);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534280);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534290);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_005342A0);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_005342B0);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_005342C0);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_005342D0);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_005342E0);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_005342F0);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534300);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534310);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534320);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534330);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534340);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534350);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534360);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534370);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534380);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_00534390);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_005343A0);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_005343B0);

INCLUDE_RODATA(const s32, "effect/effMagatuhi", D_005343C0);

INCLUDE_SDATA(const s32, "effect/effMagatuhi", D_0054F220);

INCLUDE_SDATA(const s32, "effect/effMagatuhi", D_0054F221);

INCLUDE_SDATA(const s32, "effect/effMagatuhi", D_0054F222);

INCLUDE_SDATA(const s32, "effect/effMagatuhi", D_0054F224);

INCLUDE_SDATA(const s32, "effect/effMagatuhi", D_0054F228);

INCLUDE_SDATA(const s32, "effect/effMagatuhi", D_0054F22C);

INCLUDE_SDATA(const s32, "effect/effMagatuhi", D_0054F230);

INCLUDE_SDATA(const s32, "effect/effMagatuhi", D_0054F238);

INCLUDE_SDATA(const s32, "effect/effMagatuhi", D_0054F240);

INCLUDE_SDATA(const s32, "effect/effMagatuhi", D_0054F248);

INCLUDE_SDATA(const s32, "effect/effMagatuhi", D_0054F250);

INCLUDE_SDATA(const s32, "effect/effMagatuhi", D_0054F258);

INCLUDE_SDATA(const s32, "effect/effMagatuhi", D_0054F260);

