#include "common.h"
#include "ps2asm.h"

struct EffRandState;
struct EffThunderAlphaParams;
struct EffThunderCell;
struct EffThunderCellParams;
struct EffThunderCellWork;
struct EffThunderFrag;
struct EffThunderFragmentParams;
struct EffThunderGroup;
struct EffThunderGroupParams;
union EffThunderParameterHead;
struct EffThunderSpark;
struct EffThunderSparkParams;
struct EffThunderSparkWork;
struct EffThunderVectorCell;
struct EffThunderVectorParams;
struct EffThunderVectorWork;
struct ParCell;
struct sdfVector3_t;
struct sdfMatPacket_t;
union SdfDrawTransform;
struct SdfGsTextureStateValues;
struct sdfGsMemSlice_t;

typedef struct EffRandState EffRandState;
typedef struct EffThunderCell EffThunderCell;
typedef struct EffThunderCellParams EffThunderCellParams;
typedef struct ParCell ParCell;
typedef union SdfDrawTransform SdfDrawTransform;
typedef struct SdfGsTextureStateValues SdfGsTextureStateValues;
typedef struct sdfMatPacket_t sdfMatPacket_t;
typedef union sdfMatUvwParm_t sdfMatUvwParm_t;
typedef struct sdfTexPacket_t sdfTexPacket_t;
typedef struct sdfGsMemSlice_t sdfGsMemSlice_t;
typedef struct sdfTexHandle_t sdfTexHandle_t;
typedef struct sdfMat_t sdfMat_t;
typedef struct sdfMemHandle_t sdfMemHandle_t;
typedef struct ParSystem ParSystem;
typedef struct EffThunderCellWork EffThunderCellWork;
typedef struct EffThunderFrag EffThunderFrag;
typedef struct EffThunderFragmentParams EffThunderFragmentParams;
typedef struct EffThunderGroupParams EffThunderGroupParams;
typedef struct EffThunderSpark EffThunderSpark;
typedef struct EffThunderSparkParams EffThunderSparkParams;
typedef struct EffThunderSparkWork EffThunderSparkWork;
typedef struct EffThunderVectorCell EffThunderVectorCell;
typedef struct EffThunderAlphaParams EffThunderAlphaParams;
typedef struct EffThunderVectorParams EffThunderVectorParams;
typedef union EffThunderParameterHead EffThunderParameterHead;
typedef struct EffThunderVectorWork EffThunderVectorWork;
typedef struct sdfVector3_t sdfVector3_t;

#define EFF_THUNDER_VECTOR_COMPONENTS 4

#define EFF_THUNDER_NEUTRAL_COLOR 0x80808080

#define EFF_THUNDER_ALPHA_MASK 0xFF000000

#define EFF_THUNDER_ALPHA_WRAP_ADD 0xE0000000

#define EFF_THUNDER_ALPHA_FADE_STEP 0x20000000

#define EFF_THUNDER_RANDOM_MIDPOINT 0.5f

#define EFF_THUNDER_RANDOM_SPAN 2.0f

#define EFF_THUNDER_AXIS_JITTER 0.5f

#define EFF_THUNDER_ROTATION_JITTER 0.4f

#define EFF_THUNDER_RADIUS_JITTER 0.3f

#define EFF_THUNDER_HALF_SCALE 0.5f

#define EFF_THUNDER_HALF_TURN 3.14159265f

#define EFF_THUNDER_MIN_ACTIVE_FRAMES 1

/* Mid-grey fragment colour (0x80 in each channel). */
#define EFF_THUNDER_FRAGMENT_GREY 0x80808080

/* Four ordered 32-bit words in the shared effect random-number state. */
struct EffRandState {
    u32 words[4];
};

/* Per-particle countdown state in the paired thunder-cell implementations. */
struct EffThunderCell {
    f32 directionX; /* 0x00 */
    f32 directionY; /* 0x04 */
    f32 directionZ; /* 0x08 */
    u32 delayFrames; /* 0x0C */
    u32 activeFrames; /* 0x10 */
};

/* Copied parameter prefix for the counted-down cell effect. */
struct EffThunderCellParams {
    f32 origin[4];
    u16 systemParam;
    u8 pad12[2];
    f32 length;
    u32 cellCount;
    u8 opaque1C[8];
    u32 startDelayRange;
    u32 activeFrameRange;
    u16 halfLife;
    u8 pad2E[2];
    f32 coreWidth;
    u32 centerColor;
    f32 bandWidth;
    u32 middleColor;
    f32 edgeWidth;
    u32 edgeColor;
};

struct ParSystem;

struct ParCell {
    u128 *history;   /* 0x00 */
    u32 *colors;     /* 0x04: packed per-history-vector colors */
    s32 vertexCount; /* 0x08: active vectors in the topology-specific history */
    s32 historyAdvanceCountdown; /* 0x0C: updates until the next history shift */
    u32 color;       /* 0x10 initialized to grey 0x80808080 */
};

struct sdfMat_t;

/* Draw entries store six transform floats and expose the same bytes as three qwords. */
union SdfDrawTransform {
    f32 m[6];
    u64 words[3];
};

/* The cached GS TEX1, TEX0 and CLAMP values shared by texture draw records. */
struct SdfGsTextureStateValues {
    u64 sampling;
    u64 texture;
    u64 clamp;
};

/* Native 0xA0-byte draw-entry block allocated by SDF_ASSET_DRAW_ENTRY_BYTES. */
struct sdfMatPacket_t {
    u32 pad00;             /* 0x00 */
    u32 unk04;             /* 0x04 */
    u32 unk08;             /* 0x08 */
    u32 secondaryColor;    /* 0x0C: packed color applied by the secondary draw state */
    u32 primaryStateWordThird; /* 0x10: copied from the owning asset, or forced to zero. */
    u32 unk14;             /* 0x14 */
    u32 pad18;             /* 0x18 */
    f32 unk1C;             /* 0x1C */
    u32 alphaState;        /* 0x20: packed GS ALPHA_2 state emitted by the VU transfer builder */
    u32 secondaryMode;     /* 0x24: secondary packet mode copied from SdfAsset */
    f32 y;                 /* 0x28 */
    f32 x;                 /* 0x2C */
    u8 pad30[8];           /* 0x30 */
    SdfGsTextureStateValues primaryTextureState; /* 0x38 */
    SdfGsTextureStateValues secondaryTextureState; /* 0x50 */
    SdfDrawTransform transforms[2]; /* 0x68 */
    u32 vifItopCommand;    /* 0x98: VIF ITOP with immediate 0xC */
    u32 vifMscalCommand;   /* 0x9C: VIF MSCAL with microprogram index zero */
};

union sdfMatUvwParm_t;

/* Optional packed or five-float state block owned by an SdfAsset. */
union sdfMatUvwParm_t {
    struct {
        u64 unk0;
        u64 unk8;
        u32 unk10;
        u32 unk14;
    } packed;
    struct {
        f32 values[5];
        u32 unk14;
    } scalar;
    /* Named view of scalar.values; the last word remains opaque. */
    struct {
        f32 centerOffsetX;
        f32 centerOffsetY;
        f32 basisScaleX;
        f32 basisScaleY;
        f32 rotationInput;
        u32 unk14;
    } transformInputs;
};

struct sdfTexHandle_t;

struct sdfTexPacket_t;

struct sdfTexPacket_t {
    s32 gifTagWord; /* Low GIFtag word; NLOOP occupies bits 0-14. */
    u32 gifTagControl; /* High GIFtag control word; one packed register. */
    u64 gifTagRegisters; /* Packed register list: A+D. */
    u64 samplingState; /* GS TEX1 data, including minification/magnification filters. */
    u64 samplingRegister; /* GS TEX1_1/TEX1_2 register selector. */
    u64 textureState; /* GS TEX0 data. */
    u64 textureRegister; /* GS TEX0_1/TEX0_2 register selector. */
    u64 clampState; /* GS CLAMP data. */
    u64 clampRegister; /* GS CLAMP_1/CLAMP_2 register selector. */
};

/* Per-texture reference count and non-owning clone source (0x8). */

/* Native VRAM range descriptor (0x1C), shared by textures, graph buffers and streams. */
struct sdfGsMemSlice_t {
    struct sdfGsMemSlice_t *prev; /* 0x00 */
    struct sdfGsMemSlice_t *next; /* 0x04 */
    u32 allocationMode;         /* 0x08: known categories above; unknown values remain valid */
    u32 word;                   /* 0x0C: VRAM offset in 32-bit words */
    s32 size;                   /* 0x10: range length in 32-bit words */
    s16 width;                  /* 0x14 */
    s16 height;                 /* 0x16 */
    s32 format;                 /* 0x18: GS pixel-storage mode */
};

/* Serialized byte consumed by the GS texture-state packet builder. */
typedef u8 SdfTextureClampMode;

struct sdfTexHandle_t {
    struct sdfTexHandle_t *prev;
    struct sdfTexHandle_t *next;
    struct sdfTexHandle_t *origin;
    s16 width;
    s16 height;
    sdfGsMemSlice_t *primaryResource;
    sdfGsMemSlice_t *secondaryResource;
    u8 paletteCount;
    u8 clutFormat;
    u8 pixelFormat;
    u8 maxMipLevel;
    u16 lodParameters; /* Packed GS TEX1 L/K parameters. */
    u8 unk1E;
    SdfTextureClampMode clampMode; /* 0x1F: 0xFF leaves GS CLAMP state zero; otherwise low nibble is copied. */
    s32 resourceKey;
    s32 battleTextureSlot;
    sdfTexPacket_t *primaryBuffer;
    sdfTexPacket_t *secondaryBuffer;
    u8 *paletteData; /* 0x30: retained copy of the texture's palette colors. */
    s32 paletteDataSize; /* 0x34: palette byte count from sdfTexGetClutDataSize. */
    s32 unk38;
    u8 *intensityMap; /* 0x3C: lazily generated weighted-RGB intensity bytes. */
};

struct sdfMat_t {
    u32 unk00; /* Word from the serialized asset-parameter header. */
    u16 unk04; /* Halfword from the serialized asset-parameter header. */
    u8 dirtyFlags; /* 0x06: pending state-group changes for the two draw entries */
    u8 pad07;
    struct sdfMatPacket_t *drawEntries[2]; /* 0x08: owned 0xA0-byte draw entries */
    u32 primaryStateWordFirst; /* 0x10: first word set by primary-state updates */
    u32 primaryStateWordSecond; /* 0x14: second word set by primary-state updates */
    u32 secondaryColor; /* 0x18 */
    f32 primaryStateFloat; /* 0x1C: interpolated primary-state float */
    u32 primaryStateWordThird; /* 0x20 */
    u32 unk24;
    u32 primaryStateWordFourth; /* 0x28: retained as raw bits */
    sdfTexHandle_t *texture;
    sdfTexHandle_t *secondaryTexture; /* 0x30 */
    u32 secondaryMode;       /* 0x34: secondary packet and palette selector */
    sdfMatUvwParm_t *primarySubParam; /* 0x38 */
    sdfMatUvwParm_t *secondarySubParam; /* 0x3C */
    f32 scalarPairFirst; /* 0x40 */
    f32 scalarPairSecond; /* 0x44 */
};

struct sdfMemHandle_t;

/* Embedded command work at actor +0x20. Neither record is the whole owning
 * actor. */
struct sdfMemHandle_t;

/* General-heap allocation descriptor (0x10); links bound the represented span. */
struct sdfMemHandle_t {
    struct sdfMemHandle_t *prev; /* 0x00 */
    struct sdfMemHandle_t *next; /* 0x04 */
    s32 address;              /* 0x08: start of represented span */
    u16 state;                /* 0x0C: free, used, or end sentinel */
    s16 referenceCount;       /* 0x0E: -1 for sentinels */
};

/* The cell-system allocation ends with this complete 0x2C-byte owner. */
struct ParSystem {
    u16 kind;            /* 0x00: ParCellTopology code; native integer storage */
    u16 bucket;          /* 0x02: packet submission bucket */
    s32 cellCount;       /* 0x04 */
    s32 vertexWordCount; /* 0x08 */
    s32 groupDivisor;   /* 0x0C: cell-system allocator input */
    struct sdfMemHandle_t *allocation; /* 0x10: geometry allocation descriptor */
    ParCell *cells;      /* 0x14 */
    u128 *vertices;      /* 0x18: backing vector history for all cells */
    u32 *colors;         /* 0x1C: backing color stream for all cells */
    struct sdfMat_t *asset; /* 0x20: submitted draw asset */
    struct ParSystem *next; /* 0x24: pending cell-system list */
    s32 unk28;           /* 0x28 */
};

/* Header followed by a variable-length EffThunderCell array in one allocation. */
struct EffThunderCellWork {
    EffThunderCellParams head;
    EffThunderCell *cells; /* 0x48 */
    u32 unk4C; /* 0x4C: settable, otherwise unobserved */
    ParSystem *system; /* 0x50 */
    sdfMemHandle_t *allocationHandle; /* 0x54 */
};

/* Per-fragment countdowns and the packed color consumed by thunder updates. */
struct EffThunderFrag {
    u32 delayFrames;  /* 0x00 */
    u32 activeFrames; /* 0x04 */
    u32 color;        /* 0x08 */
};

struct EffThunderFragmentWork;

typedef struct EffThunderFragmentWork EffThunderFragmentWork;

/* Parameter head (0x54 bytes) of the fragment effect, copied verbatim into the work. */
struct EffThunderFragmentParams {
    f32 start[4];
    f32 end[4];
    u16 systemParam;         /* 0x20 */
    u8 pad22[2];
    u32 fragmentCount;       /* 0x24 */
    u32 restartFrameLimit;  /* 0x28: zero permits unlimited restarts */
    f32 waveAmplitude;      /* 0x2C: sinusoidal displacement */
    u32 startDelayRange;     /* 0x30 modulus of delayFrames */
    u32 activeFrameRange;    /* 0x34 modulus of activeFrames before adding one */
    u16 halfLife;            /* 0x38 */
    u8 pad3A[2];
    f32 coreWidth;          /* 0x3C: half-width of the primary strip */
    u32 centerColorWord;     /* 0x40: packed center color used by symmetric cell ramps */
    f32 bandWidth;          /* 0x44: width of the continued strip */
    u32 middleColorWord;    /* 0x48: packed middle color used by symmetric cell ramps */
    f32 edgeWidth;          /* 0x4C: added outer margin */
    u32 edgeColorWord;      /* 0x50: packed edge color used by symmetric cell ramps */
};

struct EffThunderFragmentWork {
    EffThunderFragmentParams head;
    EffThunderFrag *fragments; /* 0x54 */
    u32 color;               /* 0x58 */
    union {
        u32 updateCount;     /* single-system variant */
        ParSystem *secondarySystem; /* dual-system variant */
    } state;                 /* 0x5C */
    ParSystem *system;            /* 0x60 */
    sdfMemHandle_t *allocationHandle; /* 0x64 */
}; /* 0x68 */

typedef struct EffThunderGroup EffThunderGroup;

/* Shared paired-chain parameter and resource owners. Points are filled before rendering. */
struct EffThunderGroupParams {
    f32 points[10][4];
    s32 count;
    EffThunderFragmentParams params;
};

struct EffThunderGroup {
    EffThunderGroupParams head;
    EffThunderFragmentWork *handles[10];
    u32 color;
};

/* One orbiting spark and its independently owned particle cell system. */
struct EffThunderSpark {
    s32 age;              /* 0x00 */
    f32 verticalOffset;   /* 0x04 */
    f32 verticalSpeed;    /* 0x08 */
    f32 orbitAngle;       /* 0x0C */
    f32 angularSpeed;     /* 0x10 */
    f32 orbitRadius;      /* 0x14 */
    f32 heightOffset;     /* 0x18 */
    ParSystem *system;    /* 0x1C */
};

/* Copied spark constructor input; gaps remain opaque without stronger evidence. */
struct EffThunderSparkParams {
    f32 origin[4];                   /* 0x00 */
    f32 loweredPosition[4];          /* 0x10 */
    f32 position[4];                 /* 0x20 */
    u16 systemParam;                 /* 0x30 */
    u8 pad32[0x0A];
    f32 waveAmplitude;               /* 0x3C */
    u8 pad40[8];
    u16 halfLife;                    /* 0x48 */
    u8 pad4A[2];
    f32 width;                       /* 0x4C */
    void *dispatchArg;               /* 0x50 */
    u8 pad54[0x10];
    f32 heightOffset;                /* 0x64 */
    u32 sparkCount;                  /* 0x68 */
    u8 loop;                         /* 0x6C */
    u8 pad6D[3];
    s32 duration;                    /* 0x70 */
    s32 startDelaySpread;            /* 0x74 */
    s32 fadeInTime;                  /* 0x78 */
    s32 fadeOutTime;                 /* 0x7C */
    f32 orbitRadius;                 /* 0x80 */
    f32 radiusRandomness;            /* 0x84 */
    f32 heightOffsetRange;           /* 0x88 */
    f32 angularSpeed;                /* 0x8C */
    f32 angularSpeedRandomness;      /* 0x90 */
    f32 angularDamping;              /* 0x94 */
    f32 verticalSpeed;               /* 0x98 */
    f32 verticalSpeedRandomness;     /* 0x9C */
    f32 verticalDamping;             /* 0xA0 */
};

/* Allocation header followed by sparkCount records of EffThunderSpark. */
struct EffThunderSparkWork {
    EffThunderSparkParams head;      /* 0x00 */
    EffThunderSpark *sparks;         /* 0xA4 */
    u32 tintColor;                   /* 0xA8 */
    sdfMemHandle_t *allocationHandle;   /* 0xAC */
};

struct EffThunderVectorCell {
    u32 delayFrames;
    u32 activeFrames;
    f32 placementVector[3];
    f32 rotationAxis[3];
    f32 rotationScale;
    f32 radius;
    u32 color;
};

/* Alpha-system parameters supply three packed words to the particle system. */
struct EffThunderAlphaParams {
    f32 origin[4];
    u16 systemParam;
    u8 pad12[2];
    u32 cellCount;
    u8 pad18[4];
    f32 radiusScale;
    f32 heightScale;
    f32 rotationScale;
    u32 startDelayRange;
    u32 activeFrameRange;
    u16 perCell;
    u8 pad32[6];
    u32 centerAlphaWord;
    u32 reserved3C;
    u32 middleAlphaWord;
    u32 reserved44;
    u32 edgeAlphaWord;
};

/* Indexed-vector parameters retain a dispatch pointer and geometry widths. */
struct EffThunderVectorParams {
    f32 origin[4];
    u16 systemParam;
    u8 pad12[2];
    u32 cellCount;
    u8 pad18[4];
    f32 radiusScale;
    f32 heightScale;
    f32 rotationScale;
    u32 startDelayRange;
    u32 activeFrameRange;
    u16 perCell;
    u8 pad32[0xA];
    f32 bandWidth;
    void *dispatchArg;
    f32 edgeWidth;
    u8 pad48[4];
};

/* Constructors copy all 0x4C bytes. The vector updater reads the retained
 * geometry bytes through the vector view for both constructor variants. */
union EffThunderParameterHead {
    EffThunderVectorParams vector;
    EffThunderAlphaParams alpha;
};

/* The complete work is followed by cellCount cells in the same allocation. */
struct EffThunderVectorWork {
    EffThunderParameterHead head;
    EffThunderVectorCell *cells;
    u32 tintColor;
    f32 baseRadiusScale;
    f32 baseHeightScale;
    struct ParSystem *cellSystem;
    struct sdfMemHandle_t *allocationHandle;
};

/* Three-component vector; the VU0 loaders read it as the xyz of a quadword. */
struct sdfVector3_t {
    f32 x;
    f32 y;
    f32 z;
};

/*
 * Fill the x, y and z components of a float vector with one value (plain C;
 * the w component is left alone). Used by the PCP particle and strip
 * builders to set up per-axis scales before the VU0 multiply.
 */
#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

/* vf10 += vf12: the vf12 spare the thunder builders park a vector in. */
#define VU0_ADD_VF10_VF12() __asm__ volatile ( \
    ".set noreorder\n\tvadd.xyzw vf10, vf10, vf12\n\t.set reorder")

/* vf11 -= vf10. */
#define VU0_SUB_VF11_VF10() __asm__ volatile ( \
    ".set noreorder\n\tvsub.xyzw vf11, vf11, vf10\n\t.set reorder")

/* One quadword out of vf10 or vf11 with no memory clobber: the sphere rotate
 * loops keep the vertex address in a register across the store. */
#define VU0_STORE_VF10_NOMEM(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" \
    : : "r" (dst))
#define VU0_STORE_VF11_NOMEM(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf11, 0(%0)\n\t.set reorder" \
    : : "r" (dst))

extern EffRandState dds3EffectBase[];

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(EffRandState *state);

/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(EffRandState *state);

extern s32 effMagaCylinderCalcColor(s32 color, s32 param);

extern void effPCP_ThunderRadiatePolyPartRefresh(EffThunderCellWork *work, s32 index);

extern void effPCP_ThunderPolyLowPartRefresh(EffThunderFragmentWork *work, s32 fragmentIndex);

extern void effPCP_ThunderPolyPartRefresh(EffThunderFragmentWork *work, s32 index);

extern void effPCP_ThunderPolyListPartRefresh(EffThunderFragmentWork *work, s32 index, const u128 *seed);

void effPCP_ThunderPolyDelete(EffThunderFragmentWork *work);

void dds3TrackPolyListInit(struct ParSystem *system, s32 cellIndex);

void dds3TrackPolyDraw(struct ParSystem *system);

void dds3TrackPolyDelete(struct ParSystem *system);

/* Load axis-angle rotation into VU0 vf28-vf31. */
void SetRotateMatrixAxis(f32 angle, const sdfVector3_t *axis);

void sdfChipFreeDirect(void *memory);

void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);

extern f32 aViewTarget[4], aViewEye[4];

extern void SetRotateMatrixXYZ(const struct sdfVector3_t *);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCPThunderSpherePolyCreate);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCPThunderSpherePolyCreatePCF);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCPThunderSpherePolyCopy);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCPThunderSpherePolyDelete);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCPThunderSpherePolySetPosition);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCPThunderSpherePolySetColor);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCPThunderSpherePolySetScale);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCPThunderSpherePolyGetParam);

void effPCPThunderSphereInit(EffThunderVectorWork *work, s32 index) {
    EffThunderVectorCell *cell = work->cells + index;
    f32 scratchVector[EFF_THUNDER_VECTOR_COMPONENTS];

    cell->delayFrames = dds3GetRandA((EffRandState *)dds3EffectBase) % work->head.vector.startDelayRange;
    cell->activeFrames = dds3GetRandA((EffRandState *)dds3EffectBase) % work->head.vector.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    scratchVector[0] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    scratchVector[1] = 0;
    scratchVector[2] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    VU0_LOAD_VF10(scratchVector);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF10(scratchVector);
    scratchVector[1] = work->head.vector.heightScale * EFF_THUNDER_HALF_SCALE * ((dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN);
    cell->placementVector[0] = scratchVector[0];
    cell->placementVector[1] = scratchVector[1];
    cell->placementVector[2] = scratchVector[2];
    scratchVector[0] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    scratchVector[1] = 1.0f;
    scratchVector[2] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    VU0_LOAD_VF10(scratchVector);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF10(scratchVector);
    cell->rotationAxis[0] = scratchVector[0];
    cell->rotationAxis[1] = scratchVector[1];
    cell->rotationAxis[2] = scratchVector[2];
    cell->rotationScale = work->head.vector.rotationScale * ((dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_ROTATION_JITTER + 1.0f);
    cell->radius = work->head.vector.radiusScale * EFF_THUNDER_HALF_SCALE * ((dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_RADIUS_JITTER + 1.0f);
    cell->color = EFF_THUNDER_NEUTRAL_COLOR;
}

void effPCPThunderSphereRotate(EffThunderVectorWork *work, s32 index) {
    f32 position[4] __attribute__((aligned(16)));
    f32 placement[4] __attribute__((aligned(16)));
    f32 axis[4] __attribute__((aligned(16)));
    f32 originalAxis[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    f32 cameraDirection[4] __attribute__((aligned(16)));
    f32 origin[4] __attribute__((aligned(16)));
    f32 width[4] __attribute__((aligned(16)));
    f32 outerWidth[4] __attribute__((aligned(16)));
    f32 rotation[4] __attribute__((aligned(16)));
    ParSystem *system = work->cellSystem;
    ParCell *cell = &system->cells[index];
    EffThunderVectorCell *source;
    u128 *vertices;
    s32 count;
    s32 i;
    f32 widthScale;
    f32 rotationScale;
    f32 radius;

    count = system->vertexWordCount / 5;
    cell->vertexCount = system->vertexWordCount;
    source = &work->cells[index];
    vertices = cell->history;
    widthScale = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f * 0.2f + 1.0f;
    VEC3_SPLAT(width, work->head.vector.bandWidth * widthScale);
    VEC3_SPLAT(outerWidth, width[0] + work->head.vector.edgeWidth * widthScale);
    originalAxis[0] = source->rotationAxis[0];
    originalAxis[1] = source->rotationAxis[1];
    originalAxis[2] = source->rotationAxis[2];
    originalAxis[3] = 0.0f;
    rotationScale = -source->rotationScale;
    radius = source->radius;
    VU0_LOAD_VF10(aViewTarget);
    VU0_LOAD_VF11(aViewEye);
    VU0_SUB_VF10_VF11();
    VU0_STORE_VF10_NOMEM(cameraDirection);
    placement[0] = source->placementVector[0];
    placement[1] = 0.0f;
    placement[2] = source->placementVector[2];
    PS2_COPY_QUAD(origin, work->head.vector.origin);
    origin[1] += source->placementVector[1];

    for (i = 0; i < count; i++) {
        position[0] = origin[0] + placement[0] * radius;
        position[1] = origin[1] + placement[1] * radius;
        position[2] = origin[2] + placement[2] * radius;
        PS2_COPY_QUAD(axis, originalAxis);
        rotation[0] = dds3GetRandFloatA(dds3EffectBase) * (75.0f * (EFF_THUNDER_HALF_TURN / 180.0f));
        rotation[1] = dds3GetRandFloatA(dds3EffectBase) * (75.0f * (EFF_THUNDER_HALF_TURN / 180.0f));
        rotation[2] = dds3GetRandFloatA(dds3EffectBase) * (75.0f * (EFF_THUNDER_HALF_TURN / 180.0f));
        SetRotateMatrixXYZ((const struct sdfVector3_t *)rotation);
        VU0_LOAD_VF10(axis);
        VU0_ROTATE_VF10();
        VU0_STORE_VF10_NOMEM(axis);
        SetRotateMatrixAxis(((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f * 0.7f) * rotationScale + rotationScale, (const struct sdfVector3_t *)axis);
        VU0_LOAD_VF10(placement);
        VU0_COPY_VF10_TO_VF11();
        VU0_ROTATE_VF10();
        VU0_STORE_VF10_NOMEM(placement);
        VU0_SUB_VF10_VF11();
        VU0_LOAD_VF11(cameraDirection);
        VU0_CROSS_VF10_VF11();
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF10_NOMEM(side);
        VU0_LOAD_VF11(width);
        VU0_MUL_VF10_VF11();
        VU0_COPY_VF10_TO_VF12();
        VU0_LOAD_VF10(position);
        VU0_STORE_VF10_NOMEM(vertices + 2);
        VU0_COPY_VF10_TO_VF11();
        VU0_ADD_VF10_VF12();
        VU0_STORE_VF10_NOMEM(vertices + 1);
        VU0_COPY_VF12_TO_VF10();
        VU0_SUB_VF11_VF10();
        VU0_STORE_VF11_NOMEM(vertices + 3);
        VU0_LOAD_VF10(side);
        VU0_LOAD_VF11(outerWidth);
        VU0_MUL_VF10_VF11();
        VU0_COPY_VF10_TO_VF12();
        VU0_LOAD_VF10(position);
        VU0_COPY_VF10_TO_VF11();
        VU0_ADD_VF10_VF12();
        VU0_STORE_VF10_NOMEM(vertices);
        VU0_COPY_VF12_TO_VF10();
        VU0_SUB_VF11_VF10();
        VU0_STORE_VF11_NOMEM(vertices + 4);
        vertices += 5;
    }
}

void effPCPThunderSphereRotateAxis(EffThunderVectorWork *work, s32 index) {
    f32 axis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    ParSystem *system = work->cellSystem;
    EffThunderVectorCell *source = &work->cells[index];
    ParCell *cell = &system->cells[index];
    u128 *vertices = cell->history;
    s32 rows = system->vertexWordCount / 5;
    s32 i;
    f32 sample;
    f32 angle;

    sample = dds3GetRandFloatA(dds3EffectBase);
    angle = source->rotationScale * sample * 3.0f;
    direction[0] = source->placementVector[0];
    direction[1] = 0.0f;
    direction[2] = source->placementVector[2];
    axis[0] = source->rotationAxis[0];
    axis[1] = source->rotationAxis[1];
    axis[2] = source->rotationAxis[2];
    SetRotateMatrixAxis(angle, (const struct sdfVector3_t *)axis);
    VU0_LOAD_VF10(direction);
    VU0_ROTATE_VF10();
    VU0_STORE_VF10(direction);
    source->placementVector[0] = direction[0];
    source->placementVector[2] = direction[2];
    VU0_LOAD_VF11(work->head.vector.origin);
    for (i = 0; i < rows; i++) {
        VU0_LOAD_VF10(vertices);
        VU0_SUB_VF10_VF11();
        VU0_ROTATE_VF10();
        VU0_ADD_VF10_VF11();
        VU0_STORE_VF10(vertices);
        VU0_LOAD_VF10(vertices + 1);
        VU0_SUB_VF10_VF11();
        VU0_ROTATE_VF10();
        VU0_ADD_VF10_VF11();
        VU0_STORE_VF10(vertices + 1);
        VU0_LOAD_VF10(vertices + 2);
        VU0_SUB_VF10_VF11();
        VU0_ROTATE_VF10();
        VU0_ADD_VF10_VF11();
        VU0_STORE_VF10(vertices + 2);
        VU0_LOAD_VF10(vertices + 3);
        VU0_SUB_VF10_VF11();
        VU0_ROTATE_VF10();
        VU0_ADD_VF10_VF11();
        VU0_STORE_VF10(vertices + 3);
        VU0_LOAD_VF10(vertices + 4);
        VU0_SUB_VF10_VF11();
        VU0_ROTATE_VF10();
        VU0_ADD_VF10_VF11();
        VU0_STORE_VF10(vertices + 4);
        vertices += 5;
    }
}

void effPCPThunderSpherePolyDraw(EffThunderVectorWork *work) {
    s32 i = 0;
    ParSystem *renderSystem = work->cellSystem;
    s32 cellCount = work->head.vector.cellCount;
    u32 tintColor = work->tintColor;
    EffThunderVectorCell *cell = work->cells;
    ParCell *renderCells = renderSystem->cells;
    u32 *renderColor;

    if (cellCount > 0) {
        renderColor = &renderCells->color;
        do {
            if (cell->delayFrames == 0) {
                if (cell->activeFrames != 0) {
                    effPCPThunderSphereRotate(work, i);
                    effPCPThunderSphereRotateAxis(work, i);
                    cell->activeFrames--;
                } else if (cell->color & EFF_THUNDER_ALPHA_MASK) {
                    cell->color += EFF_THUNDER_ALPHA_WRAP_ADD;
                    effPCPThunderSphereRotateAxis(work, i);
                } else {
                    effPCPThunderSphereInit(work, i);
                    dds3TrackPolyListInit(work->cellSystem, i);
                }
            } else {
                cell->delayFrames--;
            }
            *renderColor = effMagaCylinderCalcColor(cell->color, tintColor);
            i++;
            cell++;
            renderColor = (u32 *)((u8 *)renderColor + sizeof(ParCell));
        } while (i < cellCount);
    }
    dds3TrackPolyDraw(work->cellSystem);
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", effBTLBindCreateTrack);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCPThunderSpherePolyLowDelete);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCPThunderSpherePolyLowCreate);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCPThunderSpherePolyLowCopy);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCPThunderSpherePolyLowSetPosition);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCPThunderSpherePolyLowSetColor);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCPThunderSpherePolyLowSetScale);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCPThunderSpherePolyLowGetParam);

void effPCPThunderSphereLowInit(EffThunderVectorWork *work, s32 index) {
    EffThunderVectorCell *cell = work->cells + index;
    f32 scratchVector[EFF_THUNDER_VECTOR_COMPONENTS];

    cell->delayFrames = dds3GetRandA((EffRandState *)dds3EffectBase) % work->head.vector.startDelayRange;
    cell->activeFrames = dds3GetRandA((EffRandState *)dds3EffectBase) % work->head.vector.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    scratchVector[0] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    scratchVector[1] = 0;
    scratchVector[2] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    VU0_LOAD_VF10(scratchVector);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF10(scratchVector);
    scratchVector[1] = work->head.vector.heightScale * EFF_THUNDER_HALF_SCALE * ((dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN);
    cell->placementVector[0] = scratchVector[0];
    cell->placementVector[1] = scratchVector[1];
    cell->placementVector[2] = scratchVector[2];
    scratchVector[0] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    scratchVector[1] = 1.0f;
    scratchVector[2] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    VU0_LOAD_VF10(scratchVector);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF10(scratchVector);
    cell->rotationAxis[0] = scratchVector[0];
    cell->rotationAxis[1] = scratchVector[1];
    cell->rotationAxis[2] = scratchVector[2];
    cell->rotationScale = work->head.vector.rotationScale * ((dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_ROTATION_JITTER + 1.0f);
    cell->radius = work->head.vector.radiusScale * EFF_THUNDER_HALF_SCALE * ((dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_RADIUS_JITTER + 1.0f);
    cell->color = EFF_THUNDER_NEUTRAL_COLOR;
}

void effPCPThunderSphereLowRotate(EffThunderVectorWork *work, s32 index) {
    f32 position[4] __attribute__((aligned(16)));
    f32 placement[4] __attribute__((aligned(16)));
    f32 axis[4] __attribute__((aligned(16)));
    f32 originalAxis[4] __attribute__((aligned(16)));
    f32 cameraDirection[4] __attribute__((aligned(16)));
    f32 origin[4] __attribute__((aligned(16)));
    f32 width[4] __attribute__((aligned(16)));
    f32 rotation[4] __attribute__((aligned(16)));
    ParSystem *system = work->cellSystem;
    ParCell *cell = &system->cells[index];
    EffThunderVectorCell *source;
    u128 *vertices;
    s32 count;
    s32 i;
    f32 widthScale;
    f32 rotationScale;
    f32 angleJitter;
    f32 radius;

    count = system->vertexWordCount >> 1;
    cell->vertexCount = system->vertexWordCount;
    source = &work->cells[index];
    vertices = cell->history;
    widthScale = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f * 0.3f + 1.0f;
    VEC3_SPLAT(width, work->head.vector.bandWidth * widthScale);
    originalAxis[0] = source->rotationAxis[0];
    originalAxis[1] = source->rotationAxis[1];
    originalAxis[2] = source->rotationAxis[2];
    originalAxis[3] = 0.0f;
    radius = source->radius;
    VU0_LOAD_VF10(aViewTarget);
    VU0_LOAD_VF11(aViewEye);
    VU0_SUB_VF10_VF11();
    VU0_STORE_VF10_NOMEM(cameraDirection);
    placement[0] = source->placementVector[0];
    placement[1] = 0.0f;
    placement[2] = source->placementVector[2];
    PS2_COPY_QUAD(origin, work->head.vector.origin);
    origin[1] += source->placementVector[1];

    for (i = 0; i < count; i++) {
        position[0] = origin[0] + placement[0] * radius;
        position[1] = origin[1] + placement[1] * radius;
        position[2] = origin[2] + placement[2] * radius;
        PS2_COPY_QUAD(axis, originalAxis);
        rotation[0] = dds3GetRandFloatA(dds3EffectBase) * (EFF_THUNDER_HALF_TURN * 0.5f);
        rotation[1] = dds3GetRandFloatA(dds3EffectBase) * (EFF_THUNDER_HALF_TURN * 0.5f);
        rotation[2] = dds3GetRandFloatA(dds3EffectBase) * (EFF_THUNDER_HALF_TURN * 0.5f);
        SetRotateMatrixXYZ((const struct sdfVector3_t *)rotation);
        VU0_LOAD_VF10(axis);
        VU0_ROTATE_VF10();
        VU0_STORE_VF10_NOMEM(axis);
        angleJitter = dds3GetRandFloatA(dds3EffectBase);
        rotationScale = -source->rotationScale;
        SetRotateMatrixAxis(((angleJitter - 0.5f) * 2.0f * 0.8f) * rotationScale + rotationScale, (const struct sdfVector3_t *)axis);
        VU0_LOAD_VF10(placement);
        VU0_COPY_VF10_TO_VF11();
        VU0_ROTATE_VF10();
        VU0_STORE_VF10_NOMEM(placement);
        VU0_SUB_VF10_VF11();
        VU0_LOAD_VF11(cameraDirection);
        VU0_CROSS_VF10_VF11();
        VU0_NORMALIZE_VF10();
        VU0_LOAD_VF11(width);
        VU0_MUL_VF10_VF11();
        VU0_COPY_VF10_TO_VF12();
        VU0_LOAD_VF10(position);
        VU0_COPY_VF10_TO_VF11();
        VU0_ADD_VF10_VF12();
        VU0_STORE_VF10_NOMEM(vertices);
        VU0_COPY_VF12_TO_VF10();
        VU0_SUB_VF11_VF10();
        VU0_STORE_VF11_NOMEM(vertices + 1);
        vertices += 2;
    }
}

void effPCPThunderSphereLowRotateAxis(EffThunderVectorWork *work, s32 index) {
    f32 axis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    ParSystem *system = work->cellSystem;
    EffThunderVectorCell *source = &work->cells[index];
    ParCell *cell = &system->cells[index];
    u128 *vertices = cell->history;
    s32 rows = system->vertexWordCount >> 1;
    s32 i;
    f32 sample;
    f32 angle;

    sample = dds3GetRandFloatA(dds3EffectBase);
    angle = source->rotationScale * sample * 3.0f;
    direction[0] = source->placementVector[0];
    direction[1] = 0.0f;
    direction[2] = source->placementVector[2];
    axis[0] = source->rotationAxis[0];
    axis[1] = source->rotationAxis[1];
    axis[2] = source->rotationAxis[2];
    SetRotateMatrixAxis(angle, (const struct sdfVector3_t *)axis);
    VU0_LOAD_VF10(direction);
    VU0_ROTATE_VF10();
    VU0_STORE_VF10(direction);
    source->placementVector[0] = direction[0];
    source->placementVector[2] = direction[2];
    VU0_LOAD_VF11(work->head.vector.origin);
    for (i = 0; i < rows; i++) {
        VU0_LOAD_VF10(vertices);
        VU0_SUB_VF10_VF11();
        VU0_ROTATE_VF10();
        VU0_ADD_VF10_VF11();
        VU0_STORE_VF10(vertices);
        VU0_LOAD_VF10(vertices + 1);
        VU0_SUB_VF10_VF11();
        VU0_ROTATE_VF10();
        VU0_ADD_VF10_VF11();
        VU0_STORE_VF10(vertices + 1);
        vertices += 2;
    }
}

void effPCPThunderSpherePolyLowDraw(EffThunderVectorWork *work) {
    s32 i = 0;
    ParSystem *renderSystem = work->cellSystem;
    s32 cellCount = work->head.vector.cellCount;
    u32 tintColor = work->tintColor;
    EffThunderVectorCell *cell = work->cells;
    ParCell *renderCells = renderSystem->cells;
    u32 *renderColor;

    if (cellCount > 0) {
        renderColor = &renderCells->color;
        do {
            if (cell->delayFrames == 0) {
                if (cell->activeFrames != 0) {
                    effPCPThunderSphereLowRotate(work, i);
                    effPCPThunderSphereLowRotateAxis(work, i);
                    cell->activeFrames--;
                } else if (cell->color & EFF_THUNDER_ALPHA_MASK) {
                    cell->color += EFF_THUNDER_ALPHA_WRAP_ADD;
                    effPCPThunderSphereLowRotateAxis(work, i);
                } else {
                    effPCPThunderSphereLowInit(work, i);
                    dds3TrackPolyListInit(work->cellSystem, i);
                }
            } else {
                cell->delayFrames--;
            }
            *renderColor = effMagaCylinderCalcColor(cell->color, tintColor);
            i++;
            cell++;
            renderColor = (u32 *)((u8 *)renderColor + sizeof(ParCell));
        } while (i < cellCount);
    }
    dds3TrackPolyDraw(work->cellSystem);
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", eff_177850);

void eff_177A48(EffThunderSparkWork *work) {
    s32 sparkCount = (s32)work->head.sparkCount;
    s32 sparkIndex = 0;

    if (sparkCount > 0) {
        do {
            dds3TrackPolyDelete(work->sparks[sparkIndex].system);
            sparkIndex++;
        } while (sparkIndex < sparkCount);
    }
    sdfMemFreeDirect(work->allocationHandle);
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00177AB0);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00177AC0);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00177AC8);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00177BC0);

INCLUDE_ASM(const s32, "effect/effPCPThunder", eff_178128);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderPolyCreate);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderPolyDelete);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderPolySetPosition);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderPolySetColor);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderPolyGetParam);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderPolySetVCAcute);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderPolySetVCAcute2);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderPolySetVCNormal);

void effPCP_ThunderPolyDrawOff(EffThunderFragmentWork *work, s32 index) {
    EffThunderFrag *fragment = &work->fragments[index];

    fragment->delayFrames = dds3GetRandA((EffRandState *)dds3EffectBase) % work->head.startDelayRange;
    fragment->activeFrames = dds3GetRandA((EffRandState *)dds3EffectBase) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    fragment->color = EFF_THUNDER_FRAGMENT_GREY;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderPolyPartRefresh);

void effPCP_ThunderPolyDraw(EffThunderFragmentWork *work) {
    s32 i = 0;
    ParSystem *system = work->system;
    s32 count = work->head.fragmentCount;
    u32 restartLimit = work->head.restartFrameLimit;
    u32 tintColor = work->color;
    u32 updateCount = work->state.updateCount;
    EffThunderFrag *fragment = work->fragments;
    ParCell *renderCells = system->cells;
    s32 canRestart;

    if (count > 0) {
        canRestart = updateCount < restartLimit;
        do {
            if (fragment->delayFrames == 0) {
                if (fragment->activeFrames != 0) {
                    effPCP_ThunderPolyPartRefresh(work, i);
                    fragment->activeFrames--;
                } else if (fragment->color & EFF_THUNDER_ALPHA_MASK) {
                    fragment->color += EFF_THUNDER_ALPHA_WRAP_ADD;
                } else if (restartLimit == 0 || canRestart) {
                    effPCP_ThunderPolyDrawOff(work, i);
                    dds3TrackPolyListInit(work->system, i);
                } else {
                    renderCells[i].vertexCount = 0;
                }
            } else {
                fragment->delayFrames--;
            }
            renderCells[i].color = effMagaCylinderCalcColor(fragment->color, tintColor);
            i++;
            fragment++;
        } while (i < count);
        updateCount = work->state.updateCount;
    }
    work->state.updateCount = updateCount + 1;
    dds3TrackPolyDraw(work->system);
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderPolyLowCreate);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderPolyLowDelete);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderPolyLowSetPosition);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderPolyLowSetColor);

void effPCP_ThunderPolyLowDrawOff(EffThunderFragmentWork *work, s32 index) {
    EffThunderFrag *fragment = &work->fragments[index];

    fragment->delayFrames = dds3GetRandA((EffRandState *)dds3EffectBase) % work->head.startDelayRange;
    fragment->activeFrames = dds3GetRandA((EffRandState *)dds3EffectBase) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    fragment->color = EFF_THUNDER_FRAGMENT_GREY;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderPolyLowPartRefresh);

void effPCP_ThunderPolyLowDraw(EffThunderFragmentWork *work) {
    s32 index = 0;
    ParSystem *secondarySystem = work->state.secondarySystem;
    s32 fragmentCount = work->head.fragmentCount;
    ParSystem *primarySystem = work->system;
    ParCell *primaryCells = primarySystem->cells;
    u32 tintColor = work->color;
    EffThunderFrag *fragment = work->fragments;
    ParCell *secondaryCells = secondarySystem->cells;
    s32 packedColor;

    if (fragmentCount > 0) {
        do {
            if (fragment->delayFrames == 0) {
                if (fragment->activeFrames != 0) {
                    effPCP_ThunderPolyLowPartRefresh(work, index);
                    fragment->activeFrames--;
                } else if (fragment->color & EFF_THUNDER_ALPHA_MASK) {
                    fragment->color -= EFF_THUNDER_ALPHA_FADE_STEP;
                } else {
                    effPCP_ThunderPolyLowDrawOff(work, index);
                    dds3TrackPolyListInit(work->state.secondarySystem, index);
                    dds3TrackPolyListInit(work->system, index);
                }
            } else {
                fragment->delayFrames--;
            }
            packedColor = effMagaCylinderCalcColor(fragment->color, tintColor);
            secondaryCells[index].color = packedColor;
            primaryCells[index].color = packedColor;
            fragment++;
            index++;
        } while (index < fragmentCount);
    }
    dds3TrackPolyDraw(work->state.secondarySystem);
    dds3TrackPolyDraw(work->system);
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderRadiatePolyCreate);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderRadiatePolyDelete);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderRadiatePolySetPosition);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderRadiatePolySetColor);

void effPCP_ThunderRadiatePolyDrawOff(EffThunderCellWork *work, s32 index) {
    EffThunderCell *cell = work->cells + index;
    f32 centeredRandom;

    centeredRandom = dds3GetRandFloatA(&dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT;
    cell->directionX = centeredRandom + centeredRandom;
    centeredRandom = dds3GetRandFloatA(&dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT;
    cell->directionY = centeredRandom + centeredRandom;
    centeredRandom = dds3GetRandFloatA(&dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT;
    cell->directionZ = centeredRandom + centeredRandom;
    cell->delayFrames = dds3GetRandA((EffRandState *)dds3EffectBase) % work->head.startDelayRange;
    cell->activeFrames = dds3GetRandA((EffRandState *)dds3EffectBase) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderRadiatePolyPartRefresh);

void effPCP_ThunderRadiatePolyDraw(EffThunderCellWork *work) {
    s32 i = 0;
    s32 cellCount = work->head.cellCount;
    EffThunderCell *cell = work->cells;
    ParCell *renderCells = work->system->cells;

    if (cellCount > 0) {
        do {
            if (cell->delayFrames == 0) {
                if (cell->activeFrames != 0) {
                    effPCP_ThunderRadiatePolyPartRefresh(work, i);
                    cell->activeFrames--;
                } else if (renderCells[i].color & EFF_THUNDER_ALPHA_MASK) {
                    renderCells[i].color -= EFF_THUNDER_ALPHA_FADE_STEP;
                } else {
                    effPCP_ThunderRadiatePolyDrawOff(work, i);
                    dds3TrackPolyListInit(work->system, i);
                }
            } else {
                cell->delayFrames--;
            }
            cell++;
            i++;
        } while (i < cellCount);
    }
    dds3TrackPolyDraw(work->system);
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderPolyListCreate);

void effPCP_ThunderPolyListDelete(EffThunderGroup *group) {
    s32 fragmentIndex;

    for (fragmentIndex = 0; fragmentIndex < group->head.count - 1; fragmentIndex++) {
        effPCP_ThunderPolyDelete(group->handles[fragmentIndex]);
    }
    sdfChipFreeDirect(group);
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderPolyListGetParam);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderPolyListSetColor);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderPolyListPartRefresh);

void effPCP_ThunderPolyListDraw(EffThunderGroup *group) {
    s32 i = 0;
    s32 segmentCount = group->head.count - 1;
    s32 j;
    s32 fragmentCount;
    u32 color;
    f32 (*points)[4] = group->head.points;
    EffThunderFragmentWork *work;
    EffThunderFragmentWork *previous;
    ParCell *cell;

    for (; i < segmentCount; i++) {
        work = group->handles[i];
        PS2_COPY_QUAD(work->head.start, points[0]);
        PS2_COPY_QUAD(work->head.end, points[1]);
        points++;
        fragmentCount = work->head.fragmentCount;
        color = effMagaCylinderCalcColor(group->color, work->color);
        cell = work->system->cells;
        for (j = 0; j < fragmentCount; j++) {
            if (i > 0) {
                previous = group->handles[i - 1];
                effPCP_ThunderPolyListPartRefresh(work, j,
                    previous->system->cells[j].history + previous->system->cells[j].vertexCount - 5);
            } else {
                effPCP_ThunderPolyPartRefresh(work, j);
            }
            cell[j].color = color;
        }
        work->state.updateCount++;
        dds3TrackPolyDraw(work->system);
    }
}
