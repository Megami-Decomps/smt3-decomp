#include "common.h"
#include "ps2asm.h"
#include "sdf_resource.h"
#include "sdf_types.h"

struct effRandState_t;
struct effPCPThunderSpherePolyParam_t;
struct effPCP_ThunderRadiatePolyPart_t;
struct effPCP_ThunderRadiatePolyParam_t;
struct effPCP_ThunderRadiatePoly_t;
struct effPCP_ThunderPolyPart_t;
struct effPCP_ThunderPolyParam_t;
struct effPCP_ThunderPolyList_t;
struct effPCP_ThunderPolyListParam_t;
union EffThunderParameterHead;
struct effPCP_ThunderColumnPolyLowPart_t;
struct EffThunderSparkParams;
struct EffThunderSparkWork;
struct effPCPThunderSpherePolyPart_t;
struct EffThunderVectorParams;
struct effPCPThunderSpherePoly_t;
struct parCell_t;
struct sdfVector3_t;
struct sdfMatPacket_t;
struct sdfGsMemSlice_t;

typedef struct effRandState_t effRandState_t;
typedef struct effPCP_ThunderRadiatePolyPart_t effPCP_ThunderRadiatePolyPart_t;
typedef struct effPCP_ThunderRadiatePolyParam_t effPCP_ThunderRadiatePolyParam_t;
typedef struct parCell_t parCell_t;
typedef struct sdfMatPacket_t sdfMatPacket_t;
typedef union sdfMatUvwParm_t sdfMatUvwParm_t;
typedef struct sdfTexPacket_t sdfTexPacket_t;
typedef struct parSystem_t parSystem_t;
typedef struct effPCP_ThunderRadiatePoly_t effPCP_ThunderRadiatePoly_t;
typedef struct effPCP_ThunderPolyPart_t effPCP_ThunderPolyPart_t;
typedef struct effPCP_ThunderPolyParam_t effPCP_ThunderPolyParam_t;
typedef struct effPCP_ThunderPolyListParam_t effPCP_ThunderPolyListParam_t;
typedef struct effPCP_ThunderColumnPolyLowPart_t effPCP_ThunderColumnPolyLowPart_t;
typedef struct EffThunderSparkParams EffThunderSparkParams;
typedef struct EffThunderSparkWork EffThunderSparkWork;
typedef struct effPCPThunderSpherePolyPart_t effPCPThunderSpherePolyPart_t;
typedef struct effPCPThunderSpherePolyParam_t effPCPThunderSpherePolyParam_t;
typedef struct EffThunderVectorParams EffThunderVectorParams;
typedef union EffThunderParameterHead EffThunderParameterHead;
typedef struct effPCPThunderSpherePoly_t effPCPThunderSpherePoly_t;

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
/* Four ordered 32-bit words in the shared effect random-number state. */
struct effRandState_t {
    u32 words[4];
};

/* Per-particle countdown state in the paired thunder-cell implementations. */
struct effPCP_ThunderRadiatePolyPart_t {
    f32 directionX; /* 0x00 */
    f32 directionY; /* 0x04 */
    f32 directionZ; /* 0x08 */
    u32 delayFrames; /* 0x0C */
    u32 activeFrames; /* 0x10 */
};

/* Copied parameter prefix for the counted-down cell effect. */
struct effPCP_ThunderRadiatePolyParam_t {
    f32 origin[4];
    u16 blend;
    u8 pad12[2];
    f32 length;
    u32 number;
    u8 opaque1C[8];
    u32 startDelayRange;
    u32 activeFrameRange;
    u16 split;
    u8 pad2E[2];
    f32 coreWidth;
    u32 color_center;
    f32 bandWidth;
    u32 color_border;
    f32 edgeWidth;
    u32 color_edge;
};

struct parSystem_t;

struct parCell_t {
    u128 *history;   /* 0x00 */
    u32 *colors;     /* 0x04: packed per-history-vector colors */
    s32 vertexCount; /* 0x08: active vectors in the topology-specific history */
    s32 historyAdvanceCountdown; /* 0x0C: updates until the next history shift */
    u32 color;       /* 0x10 initialized to EFF_THUNDER_NEUTRAL_COLOR */
};

struct sdfMat_t;

/* A texture coordinate transform in a material packet. sdfMatSetUvwParm fills
 * it either as six floats or, for the identity case, as three quadword halves. */
typedef union {
    f32 m[6];
    u64 q[3];
} sdfMatTexTm_t;


/* Native 0xA0-byte draw-entry block allocated by SDF_ASSET_DRAW_ENTRY_BYTES. */
struct sdfMatPacket_t {
    u32 pad00;             /* 0x00 */
    u32 ambient;             /* 0x04 */
    u32 diffuse;             /* 0x08 */
    u32 multiTexDiffuse;    /* 0x0C: packed color applied by the secondary draw state */
    u32 specular; /* 0x10: copied from the owning asset, or forced to zero. */
    u32 refmapColor;             /* 0x14 */
    u32 pad18;             /* 0x18 */
    f32 constant;             /* 0x1C */
    u32 alphaState;        /* 0x20: packed GS ALPHA_2 state emitted by the VU transfer builder */
    u32 multiTexMethod;     /* 0x24: secondary packet mode copied from SdfAsset */
    f32 y;                 /* 0x28 */
    f32 x;                 /* 0x2C */
    u8 pad30[8];           /* 0x30 */
    u64 tex1Data;          /* 0x38 */
    u64 tex0Data;
    u64 clampData;
    u64 mtexTex1Data;      /* 0x50 */
    u64 mtexTex0Data;
    u64 mtexClampData;
    /* sdfMatSetUvwParm writes one transform per texture: a 2x2 then an offset. */
    sdfMatTexTm_t texTm;   /* 0x68 */
    sdfMatTexTm_t mtexTexTm;
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


/* Serialized byte consumed by the GS texture-state packet builder. */
typedef u8 SdfTextureClampMode;



struct sdfMemHandle_t;

/* Embedded command work at actor +0x20. Neither record is the whole owning
 * actor. */
struct sdfMemHandle_t;


/* The cell-system allocation ends with this complete 0x2C-byte owner. */
struct parSystem_t {
    u16 kind;            /* 0x00: ParCellTopology code; native integer storage */
    u16 bucket;          /* 0x02: packet submission bucket */
    s32 cellCount;       /* 0x04 */
    s32 vertexWordCount; /* 0x08 */
    s32 groupDivisor;   /* 0x0C: cell-system allocator input */
    struct sdfMemHandle_t *allocation; /* 0x10: geometry allocation descriptor */
    parCell_t *cells;      /* 0x14 */
    u128 *vertices;      /* 0x18: backing vector history for all cells */
    u32 *colors;         /* 0x1C: backing color stream for all cells */
    struct sdfMat_t *asset; /* 0x20: submitted draw asset */
    struct parSystem_t *next; /* 0x24: pending cell-system list */
    s32 unk28;           /* 0x28 */
};

/* Header followed by a variable-length effPCP_ThunderRadiatePolyPart_t array in one allocation. */
struct effPCP_ThunderRadiatePoly_t {
    effPCP_ThunderRadiatePolyParam_t head;
    effPCP_ThunderRadiatePolyPart_t *pPart; /* 0x48 */
    u32 abgr; /* 0x4C: settable, otherwise unobserved */
    parSystem_t *hPoly; /* 0x50 */
    sdfMemHandle_t *hMem; /* 0x54 */
};

/* Per-fragment countdowns and the packed color consumed by thunder updates. */
struct effPCP_ThunderPolyPart_t {
    u32 wait;  /* 0x00 */
    u32 refresh; /* 0x04 */
    u32 abgr;        /* 0x08 */
};

struct effPCP_ThunderPoly_t;

typedef struct effPCP_ThunderPoly_t effPCP_ThunderPoly_t;

/* Parameter head (0x54 bytes) of the fragment effect, copied verbatim into the work. */
struct effPCP_ThunderPolyParam_t {
    f32 start[4];
    f32 end[4];
    u16 blend;         /* 0x20 */
    u8 pad22[2];
    u32 number;       /* 0x24 */
    u32 restartFrameLimit;  /* 0x28: zero permits unlimited restarts */
    f32 waveAmplitude;      /* 0x2C: sinusoidal displacement */
    u32 startDelayRange;     /* 0x30 modulus of delayFrames */
    u32 activeFrameRange;    /* 0x34 modulus of activeFrames before adding one */
    u16 split;            /* 0x38 */
    u8 pad3A[2];
    f32 coreWidth;          /* 0x3C: half-width of the primary strip */
    u32 color_center;     /* 0x40: packed center color used by symmetric cell ramps */
    f32 width_border;          /* 0x44: width of the continued strip */
    u32 color_border;    /* 0x48: packed middle color used by symmetric cell ramps */
    f32 width_edge;          /* 0x4C: added outer margin */
    u32 color_edge;      /* 0x50: packed edge color used by symmetric cell ramps */
};

struct effPCP_ThunderPoly_t {
    effPCP_ThunderPolyParam_t head;
    effPCP_ThunderPolyPart_t *pPart; /* 0x54 */
    u32 color;               /* 0x58 */
    union {
        u32 updateCount;     /* single-hPoly variant */
        parSystem_t *secondarySystem; /* dual-hPoly variant */
    } state;                 /* 0x5C */
    parSystem_t *hPoly;            /* 0x60 */
    sdfMemHandle_t *allocationHandle; /* 0x64 */
}; /* 0x68 */

typedef struct effPCP_ThunderPolyList_t effPCP_ThunderPolyList_t;

/* Shared paired-chain parameter and resource owners. Points are filled before rendering. */
struct effPCP_ThunderPolyListParam_t {
    f32 points[10][4];
    s32 count;
    effPCP_ThunderPolyParam_t TPParam;
};

struct effPCP_ThunderPolyList_t {
    effPCP_ThunderPolyListParam_t head;
    effPCP_ThunderPoly_t *handles[10];
    u32 color;
};

/* One orbiting spark and its independently owned particle cell system. */
struct effPCP_ThunderColumnPolyLowPart_t {
    s32 frame;              /* 0x00 */
    f32 up;   /* 0x04 */
    f32 up_s;    /* 0x08 */
    f32 rotate;       /* 0x0C */
    f32 rotate_s;     /* 0x10 */
    f32 radius;      /* 0x14 */
    f32 height;     /* 0x18 */
    parSystem_t *hPoly;    /* 0x1C */
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

/* Allocation header followed by sparkCount records of effPCP_ThunderColumnPolyLowPart_t. */
struct EffThunderSparkWork {
    EffThunderSparkParams head;      /* 0x00 */
    effPCP_ThunderColumnPolyLowPart_t *sparks;         /* 0xA4 */
    u32 tintColor;                   /* 0xA8 */
    sdfMemHandle_t *allocationHandle;   /* 0xAC */
};

struct effPCPThunderSpherePolyPart_t {
    u32 wait;
    u32 refresh;
    f32 dir[3];
    f32 axis[3];
    f32 rotate;
    f32 radius;
    u32 abgr;
};

/* Alpha-system parameters supply three packed words to the particle system. */
struct effPCPThunderSpherePolyParam_t {
    f32 origin[4];
    u16 systemParam;
    u8 pad12[2];
    u32 cellCount;
    u8 pad18[4];
    f32 width;
    f32 height;
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
    effPCPThunderSpherePolyParam_t alpha;
};

/* The complete work is followed by cellCount cells in the same allocation. */
struct effPCPThunderSpherePoly_t {
    EffThunderParameterHead head;
    effPCPThunderSpherePolyPart_t *pPart;
    u32 tintColor;
    f32 baseRadiusScale;
    f32 baseHeightScale;
    struct parSystem_t *hPoly;
    struct sdfMemHandle_t *allocationHandle;
};


/*
 * Fill the x, y and z components of a float vector with one value (plain C;
 * the w component is left alone). Used by the PCP particle and strip
 * builders to set up per-axis scales before the VU0 multiply.
 */
#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))


extern effRandState_t dds3EffectBase[];

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(effRandState_t *name);

/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(effRandState_t *name);

extern s32 effMagaCylinderCalcColor(s32 color, s32 param);

extern void effPCP_ThunderRadiatePolyPartRefresh(effPCP_ThunderRadiatePoly_t *pThunderRadiatePoly, s32 number);

extern void effPCP_ThunderPolyLowPartRefresh(effPCP_ThunderPoly_t *pThunderPoly, s32 number);

extern void effPCP_ThunderPolyPartRefresh(effPCP_ThunderPoly_t *pThunderPoly, s32 number);

extern void effPCP_ThunderPolyListPartRefresh(effPCP_ThunderPoly_t *work, s32 index, const u128 *seed);

void effPCP_ThunderPolyDelete(effPCP_ThunderPoly_t *hData);

void dds3TrackPolyListInit(struct parSystem_t *hTrackPoly, s32 number);

void dds3TrackPolyDraw(struct parSystem_t *system);

void dds3TrackPolyDelete(struct parSystem_t *hTrackPoly);

/* Load axis-angle rotation into VU0 vf28-vf31. */
void SetRotateMatrixAxis(f32 aAngle, const sdfVector3_t *aAxis);

void sdfChipFreeDirect(void *aPtr);

void sdfMemFreeDirect(struct sdfMemHandle_t *aHandle);

extern f32 aViewTarget[4], aViewEye[4];

extern void SetRotateMatrixXYZ(const struct sdfVector3_t *);

#define EFF_THUNDER_PARAMETER_BLOCK 0
#define EFF_THUNDER_MIN_DELAY_SPREAD 1
#define EFF_THUNDER_SINGLE_CELL 1
#define EFF_THUNDER_ENDPOINT_VECTOR_BYTES 0x10

enum trackPolyTopology {
    PAR_CELL_TOPOLOGY_PAIR = 0,
    PAR_CELL_TOPOLOGY_TRIANGLE = 1,
    PAR_CELL_TOPOLOGY_SIX_VECTOR = 2,
    PAR_CELL_TOPOLOGY_FIVE_VECTOR = 4
};

extern sdfMemHandle_t *sdfMemAllocDirect(u32 aSize);
extern void *sdfMemLock(sdfMemHandle_t *aHandle);
extern void *sdfChipAllocDirect(u32 aSize);
extern void *memset(void *a, s32 b, u32 c);
extern void *dds3EffectPCPDataGetData(void *table, s32 index);
extern parSystem_t *dds3TrackPolyCreate(s32 number, s32 face, s32 rec, u32 type);
extern void dds3TrackPolySetMode(parSystem_t *hTrackPoly, u16 mode);
extern void dds3TrackPolyVCSet(parSystem_t *hTrackPoly, s32 type, u32 abgr_in, u32 abgr_out);
extern void dds3TrackPolyVC3Set_Acute(parSystem_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2);
extern void dds3TrackPolyVC5Set_Acute(parSystem_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2);
extern void dds3TrackPolyVC5Set_AcuteR(parSystem_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2);
extern void dds3TrackPolyVC5Set_AcuteW(parSystem_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2);
extern void dds3TrackPolyVC5Set_Normal(parSystem_t *hTrackPoly, s32 abgr_0, s32 abgr_1, s32 abgr_2);
extern f32 sdfSin(f32 aRadius);
extern f32 sdfCos(f32 aRadius);
void func_00177AC8(EffThunderSparkWork *work, s32 index);
void effPCP_ThunderPolyDrawOff(effPCP_ThunderPoly_t *work, s32 index);
void effPCP_ThunderPolyLowDrawOff(effPCP_ThunderPoly_t *work, s32 index);
void effPCP_ThunderRadiatePolyDrawOff(effPCP_ThunderRadiatePoly_t *work, s32 index);


effPCPThunderSpherePoly_t *effPCPThunderSpherePolyCreate(effPCPThunderSpherePolyParam_t *source) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(source->cellCount * sizeof(effPCPThunderSpherePolyPart_t) + sizeof(effPCPThunderSpherePoly_t));
    effPCPThunderSpherePoly_t *work = (effPCPThunderSpherePoly_t *)sdfMemLock(allocation);
    u32 cellIndex;

    work->head.alpha = *source;
    work->pPart = (effPCPThunderSpherePolyPart_t *)(work + 1);
    work->baseRadiusScale = source->width;
    work->baseHeightScale = source->height;
    work->allocationHandle = allocation;
    work->hPoly = dds3TrackPolyCreate(work->head.alpha.cellCount, work->head.alpha.perCell, 0, PAR_CELL_TOPOLOGY_FIVE_VECTOR);
    dds3TrackPolyVC5Set_AcuteW(work->hPoly, work->head.alpha.centerAlphaWord, work->head.alpha.middleAlphaWord, work->head.alpha.edgeAlphaWord);
    dds3TrackPolySetMode(work->hPoly, work->head.alpha.systemParam);
    for (cellIndex = 0; cellIndex < work->head.alpha.cellCount; cellIndex++) {
        work->pPart[cellIndex].wait = 0;
        work->pPart[cellIndex].refresh = 0;
        work->pPart[cellIndex].abgr = 0;
    }
    work->tintColor = EFF_THUNDER_NEUTRAL_COLOR;
    return work;
}

void *effPCPThunderSpherePolyCreatePCF(void *parameterTable) {
    void *parameters;

    parameters = dds3EffectPCPDataGetData(parameterTable, EFF_THUNDER_PARAMETER_BLOCK);
    return effPCPThunderSpherePolyCreate(parameters);
}

void *effPCPThunderSpherePolyCopy(void *source) {
    return effPCPThunderSpherePolyCreate(source);
}

void effPCPThunderSpherePolyDelete(effPCPThunderSpherePoly_t *hData) {
    dds3TrackPolyDelete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}

void effPCPThunderSpherePolySetPosition(void *hData, void *pPos) {
    SDF_VU0_COPYVECTOR(hData, pPos);
}

void effPCPThunderSpherePolySetColor(effPCPThunderSpherePoly_t *hData, u32 abgr) {
    hData->tintColor = abgr;
}

void effPCPThunderSpherePolySetScale(f32 hData, effPCPThunderSpherePoly_t *scale) {
    scale->head.vector.radiusScale = scale->baseRadiusScale * hData;
    scale->head.vector.heightScale = scale->baseHeightScale * hData;
}

EffThunderParameterHead *effPCPThunderSpherePolyGetParam(effPCPThunderSpherePoly_t *pThunderPoly) {
    return &pThunderPoly->head;
}

void effPCPThunderSphereInit(effPCPThunderSpherePoly_t *work, s32 index) {
    effPCPThunderSpherePolyPart_t *cell = work->pPart + index;
    f32 scratchVector[EFF_THUNDER_VECTOR_COMPONENTS];

    cell->wait = dds3GetRandA((effRandState_t *)dds3EffectBase) % work->head.vector.startDelayRange;
    cell->refresh = dds3GetRandA((effRandState_t *)dds3EffectBase) % work->head.vector.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    scratchVector[0] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    scratchVector[1] = 0;
    scratchVector[2] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    SDF_VU0_LOADVECTOR(scratchVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(scratchVector);
    scratchVector[1] = work->head.vector.heightScale * EFF_THUNDER_HALF_SCALE * ((dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN);
    cell->dir[0] = scratchVector[0];
    cell->dir[1] = scratchVector[1];
    cell->dir[2] = scratchVector[2];
    scratchVector[0] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    scratchVector[1] = 1.0f;
    scratchVector[2] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    SDF_VU0_LOADVECTOR(scratchVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(scratchVector);
    cell->axis[0] = scratchVector[0];
    cell->axis[1] = scratchVector[1];
    cell->axis[2] = scratchVector[2];
    cell->rotate = work->head.vector.rotationScale * ((dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_ROTATION_JITTER + 1.0f);
    cell->radius = work->head.vector.radiusScale * EFF_THUNDER_HALF_SCALE * ((dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_RADIUS_JITTER + 1.0f);
    cell->abgr = EFF_THUNDER_NEUTRAL_COLOR;
}

void effPCPThunderSphereRotate(effPCPThunderSpherePoly_t *work, s32 index) {
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
    parSystem_t *system = work->hPoly;
    parCell_t *cell = &system->cells[index];
    effPCPThunderSpherePolyPart_t *source;
    u128 *vertices;
    s32 count;
    s32 i;
    f32 widthScale;
    f32 rotationScale;
    f32 radius;

    count = system->vertexWordCount / 5;
    cell->vertexCount = system->vertexWordCount;
    source = &work->pPart[index];
    vertices = cell->history;
    widthScale = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f * 0.2f + 1.0f;
    VEC3_SPLAT(width, work->head.vector.bandWidth * widthScale);
    VEC3_SPLAT(outerWidth, width[0] + work->head.vector.edgeWidth * widthScale);
    originalAxis[0] = source->axis[0];
    originalAxis[1] = source->axis[1];
    originalAxis[2] = source->axis[2];
    originalAxis[3] = 0.0f;
    rotationScale = -source->rotate;
    radius = source->radius;
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(cameraDirection);
    placement[0] = source->dir[0];
    placement[1] = 0.0f;
    placement[2] = source->dir[2];
    SDF_VU0_COPYVECTOR(origin, work->head.vector.origin);
    origin[1] += source->dir[1];

    for (i = 0; i < count; i++) {
        position[0] = origin[0] + placement[0] * radius;
        position[1] = origin[1] + placement[1] * radius;
        position[2] = origin[2] + placement[2] * radius;
        SDF_VU0_COPYVECTOR(axis, originalAxis);
        rotation[0] = dds3GetRandFloatA(dds3EffectBase) * (75.0f * (EFF_THUNDER_HALF_TURN / 180.0f));
        rotation[1] = dds3GetRandFloatA(dds3EffectBase) * (75.0f * (EFF_THUNDER_HALF_TURN / 180.0f));
        rotation[2] = dds3GetRandFloatA(dds3EffectBase) * (75.0f * (EFF_THUNDER_HALF_TURN / 180.0f));
        SetRotateMatrixXYZ((const struct sdfVector3_t *)rotation);
        SDF_VU0_LOADVECTOR(axis);
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_STOREVECTOR_NOMEM(axis);
        SetRotateMatrixAxis(((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f * 0.7f) * rotationScale + rotationScale, (const struct sdfVector3_t *)axis);
        SDF_VU0_LOADVECTOR(placement);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_STOREVECTOR_NOMEM(placement);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR_NOMEM(side);
        SDF_VU0_LOADVECTOR_B(width);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 2);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 3);
        SDF_VU0_LOADVECTOR(side);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 4);
        vertices += 5;
    }
}

void effPCPThunderSphereRotateAxis(effPCPThunderSpherePoly_t *work, s32 index) {
    f32 axis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    parSystem_t *system = work->hPoly;
    effPCPThunderSpherePolyPart_t *source = &work->pPart[index];
    parCell_t *cell = &system->cells[index];
    u128 *vertices = cell->history;
    s32 rows = system->vertexWordCount / 5;
    s32 i;
    f32 sample;
    f32 angle;

    sample = dds3GetRandFloatA(dds3EffectBase);
    angle = source->rotate * sample * 3.0f;
    direction[0] = source->dir[0];
    direction[1] = 0.0f;
    direction[2] = source->dir[2];
    axis[0] = source->axis[0];
    axis[1] = source->axis[1];
    axis[2] = source->axis[2];
    SetRotateMatrixAxis(angle, (const struct sdfVector3_t *)axis);
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR(direction);
    source->dir[0] = direction[0];
    source->dir[2] = direction[2];
    SDF_VU0_LOADVECTOR_B(work->head.vector.origin);
    for (i = 0; i < rows; i++) {
        SDF_VU0_LOADVECTOR(vertices);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(vertices);
        SDF_VU0_LOADVECTOR(vertices + 1);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(vertices + 1);
        SDF_VU0_LOADVECTOR(vertices + 2);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(vertices + 2);
        SDF_VU0_LOADVECTOR(vertices + 3);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(vertices + 3);
        SDF_VU0_LOADVECTOR(vertices + 4);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(vertices + 4);
        vertices += 5;
    }
}

void effPCPThunderSpherePolyDraw(effPCPThunderSpherePoly_t *hData) {
    s32 i = 0;
    parSystem_t *renderSystem = hData->hPoly;
    s32 cellCount = hData->head.vector.cellCount;
    u32 tintColor = hData->tintColor;
    effPCPThunderSpherePolyPart_t *cell = hData->pPart;
    parCell_t *renderCells = renderSystem->cells;
    u32 *renderColor;

    if (cellCount > 0) {
        renderColor = &renderCells->color;
        do {
            if (cell->wait == 0) {
                if (cell->refresh != 0) {
                    effPCPThunderSphereRotate(hData, i);
                    effPCPThunderSphereRotateAxis(hData, i);
                    cell->refresh--;
                } else if (cell->abgr & EFF_THUNDER_ALPHA_MASK) {
                    cell->abgr += EFF_THUNDER_ALPHA_WRAP_ADD;
                    effPCPThunderSphereRotateAxis(hData, i);
                } else {
                    effPCPThunderSphereInit(hData, i);
                    dds3TrackPolyListInit(hData->hPoly, i);
                }
            } else {
                cell->wait--;
            }
            *renderColor = effMagaCylinderCalcColor(cell->abgr, tintColor);
            i++;
            cell++;
            renderColor = (u32 *)((u8 *)renderColor + sizeof(parCell_t));
        } while (i < cellCount);
    }
    dds3TrackPolyDraw(hData->hPoly);
}

effPCPThunderSpherePoly_t *effBTLBindCreateTrack(EffThunderVectorParams *parameters) {
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(parameters->cellCount * sizeof(effPCPThunderSpherePolyPart_t) + sizeof(effPCPThunderSpherePoly_t));
    effPCPThunderSpherePoly_t *work = (effPCPThunderSpherePoly_t *)sdfMemLock(allocationHandle);
    u32 cellIndex;

    work->head.vector = *parameters;
    work->pPart = (effPCPThunderSpherePolyPart_t *)(work + 1);
    work->baseRadiusScale = parameters->radiusScale;
    work->baseHeightScale = parameters->heightScale;
    work->allocationHandle = allocationHandle;
    work->hPoly = dds3TrackPolyCreate(work->head.vector.cellCount, work->head.vector.perCell, 0, PAR_CELL_TOPOLOGY_PAIR);
    dds3TrackPolyVCSet(work->hPoly, 2, (u32)work->head.vector.dispatchArg, (u32)work->head.vector.dispatchArg);
    dds3TrackPolySetMode(work->hPoly, work->head.vector.systemParam);
    for (cellIndex = 0; cellIndex < work->head.vector.cellCount; cellIndex++) {
        work->pPart[cellIndex].wait = 0;
        work->pPart[cellIndex].refresh = 0;
        work->pPart[cellIndex].abgr = 0;
    }
    work->tintColor = EFF_THUNDER_NEUTRAL_COLOR;
    return work;
}

void effPCPThunderSpherePolyLowDelete(effPCPThunderSpherePoly_t *hData) {
    dds3TrackPolyDelete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}

void effPCPThunderSpherePolyLowCreate(void *pParam) {
    void *parameters;

    parameters = dds3EffectPCPDataGetData(pParam, EFF_THUNDER_PARAMETER_BLOCK);
    effBTLBindCreateTrack(parameters);
}

void effPCPThunderSpherePolyLowCopy(void *parameters) {
    effBTLBindCreateTrack(parameters);
}

void effPCPThunderSpherePolyLowSetPosition(void *hData, void *pPos) {
    SDF_VU0_COPYVECTOR(hData, pPos);
}

void effPCPThunderSpherePolyLowSetColor(effPCPThunderSpherePoly_t *hData, u32 abgr) {
    hData->tintColor = abgr;
}

void effPCPThunderSpherePolyLowSetScale(f32 hData, effPCPThunderSpherePoly_t *scale) {
    scale->head.vector.radiusScale = scale->baseRadiusScale * hData;
    scale->head.vector.heightScale = scale->baseHeightScale * hData;
}

u32 effPCPThunderSpherePolyLowGetParam(u32 pThunderPoly) {
    return pThunderPoly;
}

void effPCPThunderSphereLowInit(effPCPThunderSpherePoly_t *work, s32 index) {
    effPCPThunderSpherePolyPart_t *cell = work->pPart + index;
    f32 scratchVector[EFF_THUNDER_VECTOR_COMPONENTS];

    cell->wait = dds3GetRandA((effRandState_t *)dds3EffectBase) % work->head.vector.startDelayRange;
    cell->refresh = dds3GetRandA((effRandState_t *)dds3EffectBase) % work->head.vector.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    scratchVector[0] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    scratchVector[1] = 0;
    scratchVector[2] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    SDF_VU0_LOADVECTOR(scratchVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(scratchVector);
    scratchVector[1] = work->head.vector.heightScale * EFF_THUNDER_HALF_SCALE * ((dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN);
    cell->dir[0] = scratchVector[0];
    cell->dir[1] = scratchVector[1];
    cell->dir[2] = scratchVector[2];
    scratchVector[0] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    scratchVector[1] = 1.0f;
    scratchVector[2] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    SDF_VU0_LOADVECTOR(scratchVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(scratchVector);
    cell->axis[0] = scratchVector[0];
    cell->axis[1] = scratchVector[1];
    cell->axis[2] = scratchVector[2];
    cell->rotate = work->head.vector.rotationScale * ((dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_ROTATION_JITTER + 1.0f);
    cell->radius = work->head.vector.radiusScale * EFF_THUNDER_HALF_SCALE * ((dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_RADIUS_JITTER + 1.0f);
    cell->abgr = EFF_THUNDER_NEUTRAL_COLOR;
}

void effPCPThunderSphereLowRotate(effPCPThunderSpherePoly_t *work, s32 index) {
    f32 position[4] __attribute__((aligned(16)));
    f32 placement[4] __attribute__((aligned(16)));
    f32 axis[4] __attribute__((aligned(16)));
    f32 originalAxis[4] __attribute__((aligned(16)));
    f32 cameraDirection[4] __attribute__((aligned(16)));
    f32 origin[4] __attribute__((aligned(16)));
    f32 width[4] __attribute__((aligned(16)));
    f32 rotation[4] __attribute__((aligned(16)));
    parSystem_t *system = work->hPoly;
    parCell_t *cell = &system->cells[index];
    effPCPThunderSpherePolyPart_t *source;
    u128 *vertices;
    s32 count;
    s32 i;
    f32 widthScale;
    f32 rotationScale;
    f32 angleJitter;
    f32 radius;

    count = system->vertexWordCount >> 1;
    cell->vertexCount = system->vertexWordCount;
    source = &work->pPart[index];
    vertices = cell->history;
    widthScale = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f * 0.3f + 1.0f;
    VEC3_SPLAT(width, work->head.vector.bandWidth * widthScale);
    originalAxis[0] = source->axis[0];
    originalAxis[1] = source->axis[1];
    originalAxis[2] = source->axis[2];
    originalAxis[3] = 0.0f;
    radius = source->radius;
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(cameraDirection);
    placement[0] = source->dir[0];
    placement[1] = 0.0f;
    placement[2] = source->dir[2];
    SDF_VU0_COPYVECTOR(origin, work->head.vector.origin);
    origin[1] += source->dir[1];

    for (i = 0; i < count; i++) {
        position[0] = origin[0] + placement[0] * radius;
        position[1] = origin[1] + placement[1] * radius;
        position[2] = origin[2] + placement[2] * radius;
        SDF_VU0_COPYVECTOR(axis, originalAxis);
        rotation[0] = dds3GetRandFloatA(dds3EffectBase) * (EFF_THUNDER_HALF_TURN * 0.5f);
        rotation[1] = dds3GetRandFloatA(dds3EffectBase) * (EFF_THUNDER_HALF_TURN * 0.5f);
        rotation[2] = dds3GetRandFloatA(dds3EffectBase) * (EFF_THUNDER_HALF_TURN * 0.5f);
        SetRotateMatrixXYZ((const struct sdfVector3_t *)rotation);
        SDF_VU0_LOADVECTOR(axis);
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_STOREVECTOR_NOMEM(axis);
        angleJitter = dds3GetRandFloatA(dds3EffectBase);
        rotationScale = -source->rotate;
        SetRotateMatrixAxis(((angleJitter - 0.5f) * 2.0f * 0.8f) * rotationScale + rotationScale, (const struct sdfVector3_t *)axis);
        SDF_VU0_LOADVECTOR(placement);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_STOREVECTOR_NOMEM(placement);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_LOADVECTOR_B(width);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 1);
        vertices += 2;
    }
}

void effPCPThunderSphereLowRotateAxis(effPCPThunderSpherePoly_t *work, s32 index) {
    f32 axis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    parSystem_t *system = work->hPoly;
    effPCPThunderSpherePolyPart_t *source = &work->pPart[index];
    parCell_t *cell = &system->cells[index];
    u128 *vertices = cell->history;
    s32 rows = system->vertexWordCount >> 1;
    s32 i;
    f32 sample;
    f32 angle;

    sample = dds3GetRandFloatA(dds3EffectBase);
    angle = source->rotate * sample * 3.0f;
    direction[0] = source->dir[0];
    direction[1] = 0.0f;
    direction[2] = source->dir[2];
    axis[0] = source->axis[0];
    axis[1] = source->axis[1];
    axis[2] = source->axis[2];
    SetRotateMatrixAxis(angle, (const struct sdfVector3_t *)axis);
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR(direction);
    source->dir[0] = direction[0];
    source->dir[2] = direction[2];
    SDF_VU0_LOADVECTOR_B(work->head.vector.origin);
    for (i = 0; i < rows; i++) {
        SDF_VU0_LOADVECTOR(vertices);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(vertices);
        SDF_VU0_LOADVECTOR(vertices + 1);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(vertices + 1);
        vertices += 2;
    }
}

void effPCPThunderSpherePolyLowDraw(effPCPThunderSpherePoly_t *hData) {
    s32 i = 0;
    parSystem_t *renderSystem = hData->hPoly;
    s32 cellCount = hData->head.vector.cellCount;
    u32 tintColor = hData->tintColor;
    effPCPThunderSpherePolyPart_t *cell = hData->pPart;
    parCell_t *renderCells = renderSystem->cells;
    u32 *renderColor;

    if (cellCount > 0) {
        renderColor = &renderCells->color;
        do {
            if (cell->wait == 0) {
                if (cell->refresh != 0) {
                    effPCPThunderSphereLowRotate(hData, i);
                    effPCPThunderSphereLowRotateAxis(hData, i);
                    cell->refresh--;
                } else if (cell->abgr & EFF_THUNDER_ALPHA_MASK) {
                    cell->abgr += EFF_THUNDER_ALPHA_WRAP_ADD;
                    effPCPThunderSphereLowRotateAxis(hData, i);
                } else {
                    effPCPThunderSphereLowInit(hData, i);
                    dds3TrackPolyListInit(hData->hPoly, i);
                }
            } else {
                cell->wait--;
            }
            *renderColor = effMagaCylinderCalcColor(cell->abgr, tintColor);
            i++;
            cell++;
            renderColor = (u32 *)((u8 *)renderColor + sizeof(parCell_t));
        } while (i < cellCount);
    }
    dds3TrackPolyDraw(hData->hPoly);
}

EffThunderSparkWork *eff_177850(EffThunderSparkParams *parameters) {
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(parameters->sparkCount * sizeof(effPCP_ThunderColumnPolyLowPart_t) + sizeof(EffThunderSparkWork));
    EffThunderSparkWork *work = (EffThunderSparkWork *)sdfMemLock(allocationHandle);
    s32 delaySpread;
    u32 sparkIndex;

    work->head = *parameters;
    work->sparks = (effPCP_ThunderColumnPolyLowPart_t *)(work + 1);
    work->tintColor = EFF_THUNDER_NEUTRAL_COLOR;
    work->allocationHandle = allocationHandle;
    if (work->head.startDelaySpread <= 0) {
        work->head.startDelaySpread = EFF_THUNDER_MIN_DELAY_SPREAD;
    }
    delaySpread = work->head.startDelaySpread;
    for (sparkIndex = 0; sparkIndex < work->head.sparkCount; sparkIndex++) {
        work->sparks[sparkIndex].hPoly = dds3TrackPolyCreate(EFF_THUNDER_SINGLE_CELL, work->head.halfLife * 2 - 1, 0, PAR_CELL_TOPOLOGY_PAIR);
        dds3TrackPolyVCSet(work->sparks[sparkIndex].hPoly, 2, (u32)work->head.dispatchArg, (u32)work->head.dispatchArg);
        dds3TrackPolySetMode(work->sparks[sparkIndex].hPoly, work->head.systemParam);
        func_00177AC8(work, sparkIndex);
        work->sparks[sparkIndex].frame = -(dds3GetRandA((effRandState_t *)dds3EffectBase) % delaySpread);
    }
    return work;
}

void eff_177A48(EffThunderSparkWork *work) {
    s32 sparkCount = (s32)work->head.sparkCount;
    s32 sparkIndex = 0;

    if (sparkCount > 0) {
        do {
            dds3TrackPolyDelete(work->sparks[sparkIndex].hPoly);
            sparkIndex++;
        } while (sparkIndex < sparkCount);
    }
    sdfMemFreeDirect(work->allocationHandle);
}

void func_00177AB0(void *destination, void *source) {
    SDF_VU0_COPYVECTOR(destination, source);
}

void func_00177AC0(EffThunderSparkWork *work, u32 tintColor) {
    work->tintColor = tintColor;
}

void func_00177AC8(EffThunderSparkWork *work, s32 index) {
    effPCP_ThunderColumnPolyLowPart_t *spark = work->sparks + index;
    f32 randomness;

    spark->up = 0;
    randomness = work->head.verticalSpeedRandomness;
    spark->up_s = work->head.verticalSpeed * (dds3GetRandFloatA(dds3EffectBase) * randomness + (1.0f - randomness));
    spark->height = work->head.heightOffsetRange * dds3GetRandFloatA(dds3EffectBase);
    spark->rotate = dds3GetRandFloatA(dds3EffectBase) * (EFF_THUNDER_HALF_TURN * 2.0f);
    randomness = work->head.angularSpeedRandomness;
    spark->rotate_s = work->head.angularSpeed * (dds3GetRandFloatA(dds3EffectBase) * randomness + (1.0f - randomness));
    randomness = work->head.radiusRandomness;
    spark->radius = work->head.orbitRadius * (dds3GetRandFloatA(dds3EffectBase) * randomness + (1.0f - randomness));
}

void func_00177BC0(EffThunderSparkWork *work, s32 index) {
    f32 width[4] __attribute__((aligned(16)));
    f32 negativeWidth[4] __attribute__((aligned(16)));
    f32 position[4] __attribute__((aligned(16)));
    f32 next[4] __attribute__((aligned(16)));
    f32 basis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    f32 cameraDirection[4] __attribute__((aligned(16)));
    f32 waveScale[4] __attribute__((aligned(16)));
    f32 stepScale[4] __attribute__((aligned(16)));
    f32 previousSide[4] __attribute__((aligned(16)));
    f32 delta[4] __attribute__((aligned(16)));
    parSystem_t *system;
    parCell_t *cell;
    u128 *vertices;
    u128 *join;
    s32 i;
    s32 segments;
    f32 distance;
    f32 amplitude;
    f32 phase;
    f32 cycle;
    f32 currentWave;
    f32 previousWave;
    f32 phaseStep;
    f32 facing;

    VEC3_SPLAT(width, work->head.width);
    VEC3_SPLAT(negativeWidth, -width[0]);
    system = work->sparks[index].hPoly;
    cell = system->cells;
    cell->vertexCount = system->vertexWordCount;
    vertices = cell->history;
    segments = work->head.halfLife;
    SDF_VU0_LOADVECTOR(work->head.position);
    SDF_VU0_LOADVECTOR_B(work->head.loweredPosition);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(distance);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(direction);
    distance /= work->head.halfLife;
    amplitude = work->head.waveAmplitude;
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(cameraDirection);
    SDF_VU0_LOADVECTOR_B(direction);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(side);
    phase = dds3GetRandFloatA(dds3EffectBase) * (EFF_THUNDER_HALF_TURN * 2.0f);
    cycle = phase - (EFF_THUNDER_HALF_TURN * 2.0f);
    SDF_VU0_COPYVECTOR(position, work->head.loweredPosition);
    currentWave = amplitude * sdfSin(phase);
    VEC3_SPLAT(waveScale, currentWave);
    SDF_VU0_LOADVECTOR(side);
    SDF_VU0_LOADVECTOR_B(waveScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(position);
    SDF_VU0_ADDVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B_NOMEM(position);
    VEC3_SPLAT(stepScale, distance);
    previousWave = currentWave;
    currentWave = amplitude * sdfSin(phase);
    VEC3_SPLAT(waveScale, currentWave - previousWave);
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_LOADVECTOR_B(stepScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(side);
    SDF_VU0_LOADVECTOR_B(waveScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(position);
    SDF_VU0_ADDVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B_NOMEM(next);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(basis);

    for (i = 0; i < segments; i++) {
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_LOADVECTOR_B(width);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 1);
        vertices += 2;
        SDF_VU0_COPYVECTOR(previousSide, basis);
        SDF_VU0_COPYVECTOR(position, next);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(width);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 1);
        vertices += 2;
        phaseStep = (EFF_THUNDER_HALF_TURN / segments) * 4.0f;
        cycle += phaseStep;
        phase += phaseStep;
        if (cycle > EFF_THUNDER_HALF_TURN) {
            cycle -= EFF_THUNDER_HALF_TURN;
            amplitude = work->head.waveAmplitude * (1.0f - dds3GetRandFloatA(dds3EffectBase) * 0.95f);
        }
        previousWave = currentWave;
        currentWave = amplitude * sdfSin(phase);
        VEC3_SPLAT(waveScale, currentWave - previousWave);
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_LOADVECTOR_B(stepScale);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(side);
        SDF_VU0_LOADVECTOR_B(waveScale);
        SDF_VU0_MULVECTOR();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(delta);
        SetRotateMatrixAxis(((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f) * (20.0f * (EFF_THUNDER_HALF_TURN / 180.0f)), (const struct sdfVector3_t *)cameraDirection);
        SDF_VU0_LOADVECTOR(delta);
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_ADDVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(next);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_LOADVECTOR(next);
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
        SDF_VU0_LOADVECTOR_B(previousSide);
        SDF_VU0_INNERPRODUCT(facing);
        /* Choose the join edge from the new direction relative to the prior side. */
        if (facing < 0.0f) {
            join = vertices - 1;
            SDF_VU0_LOADVECTOR_C(width);
        } else {
            join = vertices - 2;
            SDF_VU0_LOADVECTOR_C(negativeWidth);
        }
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_COPYVECTOR_C_TO_B();
        SDF_VU0_MULVECTOR();
        SDF_VU0_LOADVECTOR_B(join);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(position);
    }
}

void eff_178128(EffThunderSparkWork *work) {
    f32 position[3];
    f32 origin[4] __attribute__((aligned(16)));
    u32 tintColor;
    s32 count;
    s32 loop = work->head.loop;
    s32 duration = work->head.duration;
    s32 delaySpread = work->head.startDelaySpread;
    s32 fadeInTime = work->head.fadeInTime;
    s32 fadeOutTime = work->head.fadeOutTime;
    f32 heightOffset = work->head.heightOffset;
    f32 verticalDamping = work->head.verticalDamping;
    f32 angularDamping = work->head.angularDamping;
    effPCP_ThunderColumnPolyLowPart_t *spark;
    parCell_t *renderCell;
    s32 i;
    s32 age;
    f32 angle;
    f32 radius;
    f32 alpha;
    f32 lowerHeight;
    u32 color;

    tintColor = work->tintColor;
    SDF_VU0_COPYVECTOR(origin, work->head.origin);
    count = work->head.sparkCount;
    spark = work->sparks;
    for (i = 0; i < count; i++, spark++) {
        age = spark->frame;
        if (age == 0) {
            func_00177AC8(work, i);
            dds3TrackPolyListInit(spark->hPoly, 0);
        } else if (age > 0 && age <= duration) {
            angle = spark->rotate;
            radius = spark->radius;
            position[0] = origin[0] + sdfCos(angle) * radius;
            position[1] = origin[1] + spark->up - spark->height;
            position[2] = origin[2] + sdfSin(angle) * radius;
            spark->up += spark->up_s;
            spark->rotate += spark->rotate_s;
            spark->up_s *= verticalDamping;
            spark->rotate_s *= angularDamping;
            if (age < fadeInTime) {
                alpha = (f32)age / fadeInTime;
            } else if (duration - age <= fadeOutTime) {
                alpha = (f32)(duration - age) / fadeOutTime;
            } else {
                alpha = 1.0f;
            }
            color = effMagaCylinderCalcColor(((u32)(alpha * 127.0f) << 24) | 0x808080, tintColor);
            renderCell = spark->hPoly->cells;
            lowerHeight = position[1] - heightOffset;
            work->head.loweredPosition[0] = position[0];
            work->head.loweredPosition[1] = lowerHeight;
            work->head.loweredPosition[2] = position[2];
            work->head.position[0] = position[0];
            work->head.position[1] = position[1];
            work->head.position[2] = position[2];
            func_00177BC0(work, i);
            renderCell->color = color;
            dds3TrackPolyDraw(spark->hPoly);
        }
        if (age >= duration && loop) {
            spark->frame = -(dds3GetRandA((effRandState_t *)dds3EffectBase) % delaySpread);
        } else {
            spark->frame++;
        }
    }
}

effPCP_ThunderPoly_t *effPCP_ThunderPolyCreate(effPCP_ThunderPolyParam_t *parameters) {
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(parameters->number * sizeof(effPCP_ThunderPolyPart_t) + sizeof(effPCP_ThunderPoly_t));
    effPCP_ThunderPoly_t *work = (effPCP_ThunderPoly_t *)sdfMemLock(allocationHandle);
    u32 i;

    work->head = *parameters;
    work->pPart = (effPCP_ThunderPolyPart_t *)(work + 1);
    work->allocationHandle = allocationHandle;
    work->hPoly = dds3TrackPolyCreate(work->head.number, work->head.split * 2 - 1, 0, PAR_CELL_TOPOLOGY_FIVE_VECTOR);
    dds3TrackPolyVC5Set_AcuteW(work->hPoly, work->head.color_center, work->head.color_border, work->head.color_edge);
    dds3TrackPolySetMode(work->hPoly, work->head.blend);
    for (i = 0; i < work->head.number; i++) {
        effPCP_ThunderPolyDrawOff(work, i);
    }
    work->state.updateCount = 0;
    work->color = EFF_THUNDER_NEUTRAL_COLOR;
    return work;
}

void effPCP_ThunderPolyDelete(effPCP_ThunderPoly_t *hData) {
    dds3TrackPolyDelete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}

void effPCP_ThunderPolySetPosition(u8 *hData, void *pPos) {
        SDF_VU0_LOADVECTOR(hData + EFF_THUNDER_ENDPOINT_VECTOR_BYTES);
        SDF_VU0_LOADVECTOR_B(pPos);
        SDF_VU0_STOREVECTOR_B(hData + EFF_THUNDER_ENDPOINT_VECTOR_BYTES);
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_LOADVECTOR(hData);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(hData);
}

void effPCP_ThunderPolySetColor(effPCP_ThunderPoly_t *hData, u32 abgr) {
    hData->color = abgr;
}

effPCP_ThunderPolyParam_t *effPCP_ThunderPolyGetParam(effPCP_ThunderPoly_t *pThunderPoly) {
    return &pThunderPoly->head;
}

void effPCP_ThunderPolySetVCAcute(effPCP_ThunderPoly_t *pThunderPoly) {
    dds3TrackPolyVC5Set_Acute(pThunderPoly->hPoly, pThunderPoly->head.color_center, pThunderPoly->head.color_border, pThunderPoly->head.color_edge);
}

void effPCP_ThunderPolySetVCAcute2(effPCP_ThunderPoly_t *pThunderPoly) {
    dds3TrackPolyVC5Set_AcuteR(pThunderPoly->hPoly, pThunderPoly->head.color_center, pThunderPoly->head.color_border, pThunderPoly->head.color_edge);
}

void effPCP_ThunderPolySetVCNormal(effPCP_ThunderPoly_t *pThunderPoly) {
    dds3TrackPolyVC5Set_Normal(pThunderPoly->hPoly, pThunderPoly->head.color_center, pThunderPoly->head.color_border, pThunderPoly->head.color_edge);
}

void effPCP_ThunderPolyDrawOff(effPCP_ThunderPoly_t *work, s32 index) {
    effPCP_ThunderPolyPart_t *fragment = &work->pPart[index];

    fragment->wait = dds3GetRandA((effRandState_t *)dds3EffectBase) % work->head.startDelayRange;
    fragment->refresh = dds3GetRandA((effRandState_t *)dds3EffectBase) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    fragment->abgr = EFF_THUNDER_NEUTRAL_COLOR;
}

void effPCP_ThunderPolyPartRefresh(effPCP_ThunderPoly_t *pThunderPoly, s32 number) {
    f32 width[4] __attribute__((aligned(16)));
    f32 outerWidth[4] __attribute__((aligned(16)));
    f32 negativeOuterWidth[4] __attribute__((aligned(16)));
    f32 position[4] __attribute__((aligned(16)));
    f32 next[4] __attribute__((aligned(16)));
    f32 basis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    f32 cameraDirection[4] __attribute__((aligned(16)));
    f32 waveScale[4] __attribute__((aligned(16)));
    f32 stepScale[4] __attribute__((aligned(16)));
    f32 previousSide[4] __attribute__((aligned(16)));
    f32 delta[4] __attribute__((aligned(16)));
    parSystem_t *system = pThunderPoly->hPoly;
    parCell_t *cell = &system->cells[number];
    u128 *vertices;
    u128 *join;
    s32 i;
    s32 segments;
    f32 distance;
    f32 amplitude;
    f32 phase;
    f32 cycle;
    f32 currentWave;
    f32 previousWave;
    f32 phaseStep;
    f32 facing;

    VEC3_SPLAT(width, pThunderPoly->head.width_border);
    VEC3_SPLAT(outerWidth, width[0] + pThunderPoly->head.width_edge);
    VEC3_SPLAT(negativeOuterWidth, -outerWidth[0]);
    cell->vertexCount = system->vertexWordCount;
    vertices = cell->history;
    segments = pThunderPoly->head.split;
    SDF_VU0_LOADVECTOR(pThunderPoly->head.end);
    SDF_VU0_LOADVECTOR_B(pThunderPoly->head.start);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(distance);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(direction);
    distance /= pThunderPoly->head.split;
    amplitude = pThunderPoly->head.waveAmplitude;
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(cameraDirection);
    SDF_VU0_LOADVECTOR_B(direction);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(side);
    phase = dds3GetRandFloatA(dds3EffectBase) * (EFF_THUNDER_HALF_TURN * 2.0f);
    cycle = phase - (EFF_THUNDER_HALF_TURN * 2.0f);
    SDF_VU0_COPYVECTOR(position, pThunderPoly->head.start);
    currentWave = amplitude * sdfSin(phase);
    VEC3_SPLAT(waveScale, currentWave);
    SDF_VU0_LOADVECTOR(side);
    SDF_VU0_LOADVECTOR_B(waveScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(position);
    SDF_VU0_ADDVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B_NOMEM(position);
    VEC3_SPLAT(stepScale, distance);
    previousWave = currentWave;
    currentWave = amplitude * sdfSin(phase);
    VEC3_SPLAT(waveScale, currentWave - previousWave);
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_LOADVECTOR_B(stepScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(side);
    SDF_VU0_LOADVECTOR_B(waveScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(position);
    SDF_VU0_ADDVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B_NOMEM(next);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(basis);

    for (i = 0; i < segments; i++) {
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 2);
        SDF_VU0_COPYVECTOR_B_TO_A();
        SDF_VU0_LOADVECTOR_B(width);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 3);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 4);
        vertices += 5;
        SDF_VU0_COPYVECTOR(previousSide, basis);
        SDF_VU0_COPYVECTOR(position, next);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 2);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(width);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 3);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 4);
        vertices += 5;
        phaseStep = (EFF_THUNDER_HALF_TURN / segments) * 4.0f;
        cycle += phaseStep;
        phase += phaseStep;
        if (cycle > EFF_THUNDER_HALF_TURN) {
            cycle -= EFF_THUNDER_HALF_TURN;
            amplitude = pThunderPoly->head.waveAmplitude * (1.0f - dds3GetRandFloatA(dds3EffectBase) * 0.95f);
        }
        previousWave = currentWave;
        currentWave = amplitude * sdfSin(phase);
        VEC3_SPLAT(waveScale, currentWave - previousWave);
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_LOADVECTOR_B(stepScale);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(side);
        SDF_VU0_LOADVECTOR_B(waveScale);
        SDF_VU0_MULVECTOR();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(delta);
        SetRotateMatrixAxis(((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f) * (20.0f * (EFF_THUNDER_HALF_TURN / 180.0f)), (const struct sdfVector3_t *)cameraDirection);
        SDF_VU0_LOADVECTOR(delta);
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_ADDVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(next);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_LOADVECTOR(next);
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
        SDF_VU0_LOADVECTOR_B(previousSide);
        SDF_VU0_INNERPRODUCT(facing);
        /* Choose the join edge from the new direction relative to the prior side. */
        if (facing < 0.0f) {
            join = vertices - 1;
            SDF_VU0_LOADVECTOR_C(outerWidth);
        } else {
            join = vertices - 5;
            SDF_VU0_LOADVECTOR_C(negativeOuterWidth);
        }
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_COPYVECTOR_C_TO_B();
        SDF_VU0_MULVECTOR();
        SDF_VU0_LOADVECTOR_B(join);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(position);
    }
}

void effPCP_ThunderPolyDraw(effPCP_ThunderPoly_t *hData) {
    s32 i = 0;
    parSystem_t *system = hData->hPoly;
    s32 count = hData->head.number;
    u32 restartLimit = hData->head.restartFrameLimit;
    u32 tintColor = hData->color;
    u32 updateCount = hData->state.updateCount;
    effPCP_ThunderPolyPart_t *fragment = hData->pPart;
    parCell_t *renderCells = system->cells;
    s32 canRestart;

    if (count > 0) {
        canRestart = updateCount < restartLimit;
        do {
            if (fragment->wait == 0) {
                if (fragment->refresh != 0) {
                    effPCP_ThunderPolyPartRefresh(hData, i);
                    fragment->refresh--;
                } else if (fragment->abgr & EFF_THUNDER_ALPHA_MASK) {
                    fragment->abgr += EFF_THUNDER_ALPHA_WRAP_ADD;
                } else if (restartLimit == 0 || canRestart) {
                    effPCP_ThunderPolyDrawOff(hData, i);
                    dds3TrackPolyListInit(hData->hPoly, i);
                } else {
                    renderCells[i].vertexCount = 0;
                }
            } else {
                fragment->wait--;
            }
            renderCells[i].color = effMagaCylinderCalcColor(fragment->abgr, tintColor);
            i++;
            fragment++;
        } while (i < count);
        updateCount = hData->state.updateCount;
    }
    hData->state.updateCount = updateCount + 1;
    dds3TrackPolyDraw(hData->hPoly);
}

effPCP_ThunderPoly_t *effPCP_ThunderPolyLowCreate(effPCP_ThunderPolyParam_t *parameters) {
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(parameters->number * sizeof(effPCP_ThunderPolyPart_t) + sizeof(effPCP_ThunderPoly_t));
    effPCP_ThunderPoly_t *work = (effPCP_ThunderPoly_t *)sdfMemLock(allocationHandle);
    u32 i;

    work->head = *parameters;
    work->pPart = (effPCP_ThunderPolyPart_t *)(work + 1);
    work->allocationHandle = allocationHandle;
    work->state.secondarySystem = dds3TrackPolyCreate(work->head.number, work->head.split * 2 - 1, 0, PAR_CELL_TOPOLOGY_TRIANGLE);
    dds3TrackPolyVCSet(work->state.secondarySystem, 2, work->head.color_border, work->head.color_edge);
    dds3TrackPolySetMode(work->state.secondarySystem, work->head.blend);
    work->hPoly = dds3TrackPolyCreate(work->head.number, work->head.split * 2 - 1, 0, PAR_CELL_TOPOLOGY_PAIR);
    dds3TrackPolyVCSet(work->hPoly, 2, work->head.color_center, work->head.color_center);
    dds3TrackPolySetMode(work->hPoly, work->head.blend);
    for (i = 0; i < work->head.number; i++) {
        effPCP_ThunderPolyLowDrawOff(work, i);
    }
    work->color = EFF_THUNDER_NEUTRAL_COLOR;
    return work;
}

void effPCP_ThunderPolyLowDelete(effPCP_ThunderPoly_t *hData) {
    dds3TrackPolyDelete(hData->state.secondarySystem);
    dds3TrackPolyDelete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}

void effPCP_ThunderPolyLowSetPosition(u8 *hData, void *pPos) {
        SDF_VU0_LOADVECTOR(hData + EFF_THUNDER_ENDPOINT_VECTOR_BYTES);
        SDF_VU0_LOADVECTOR_B(pPos);
        SDF_VU0_STOREVECTOR_B(hData + EFF_THUNDER_ENDPOINT_VECTOR_BYTES);
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_LOADVECTOR(hData);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(hData);
}

void effPCP_ThunderPolyLowSetColor(effPCP_ThunderPoly_t *hData, u32 abgr) {
    hData->color = abgr;
}

void effPCP_ThunderPolyLowDrawOff(effPCP_ThunderPoly_t *work, s32 index) {
    effPCP_ThunderPolyPart_t *fragment = &work->pPart[index];

    fragment->wait = dds3GetRandA((effRandState_t *)dds3EffectBase) % work->head.startDelayRange;
    fragment->refresh = dds3GetRandA((effRandState_t *)dds3EffectBase) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    fragment->abgr = EFF_THUNDER_NEUTRAL_COLOR;
}

void effPCP_ThunderPolyLowPartRefresh(effPCP_ThunderPoly_t *pThunderPoly, s32 number) {
    f32 width[4] __attribute__((aligned(16)));
    f32 outerWidth[4] __attribute__((aligned(16)));
    f32 negativeOuterWidth[4] __attribute__((aligned(16)));
    f32 position[4] __attribute__((aligned(16)));
    f32 next[4] __attribute__((aligned(16)));
    f32 basis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    f32 cameraDirection[4] __attribute__((aligned(16)));
    f32 waveScale[4] __attribute__((aligned(16)));
    f32 stepScale[4] __attribute__((aligned(16)));
    f32 previousSide[4] __attribute__((aligned(16)));
    f32 delta[4] __attribute__((aligned(16)));
    parSystem_t *secondarySystem = pThunderPoly->state.secondarySystem;
    parSystem_t *primarySystem = pThunderPoly->hPoly;
    parCell_t *secondaryCell = &secondarySystem->cells[number];
    parCell_t *primaryCell = &primarySystem->cells[number];
    u128 *vertices;
    u128 *coreVertices;
    u128 *join;
    s32 i;
    s32 segments;
    f32 distance;
    f32 amplitude;
    f32 phase;
    f32 cycle;
    f32 currentWave;
    f32 previousWave;
    f32 phaseStep;
    f32 facing;

    VEC3_SPLAT(width, pThunderPoly->head.coreWidth);
    VEC3_SPLAT(outerWidth, (width[0] + pThunderPoly->head.width_edge) + pThunderPoly->head.width_border);
    VEC3_SPLAT(negativeOuterWidth, -outerWidth[0]);
    secondaryCell->vertexCount = secondarySystem->vertexWordCount;
    primaryCell->vertexCount = primarySystem->vertexWordCount;
    vertices = secondaryCell->history;
    coreVertices = primaryCell->history;
    segments = pThunderPoly->head.split;
    SDF_VU0_LOADVECTOR(pThunderPoly->head.end);
    SDF_VU0_LOADVECTOR_B(pThunderPoly->head.start);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(distance);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(direction);
    distance /= pThunderPoly->head.split;
    amplitude = pThunderPoly->head.waveAmplitude;
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(cameraDirection);
    SDF_VU0_LOADVECTOR_B(direction);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(side);
    phase = dds3GetRandFloatA(dds3EffectBase) * (EFF_THUNDER_HALF_TURN * 2.0f);
    cycle = phase - (EFF_THUNDER_HALF_TURN * 2.0f);
    SDF_VU0_COPYVECTOR(position, pThunderPoly->head.start);
    currentWave = amplitude * sdfSin(phase);
    VEC3_SPLAT(waveScale, currentWave);
    SDF_VU0_LOADVECTOR(side);
    SDF_VU0_LOADVECTOR_B(waveScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(position);
    SDF_VU0_ADDVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B_NOMEM(position);
    VEC3_SPLAT(stepScale, distance);
    previousWave = currentWave;
    currentWave = amplitude * sdfSin(phase);
    VEC3_SPLAT(waveScale, currentWave - previousWave);
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_LOADVECTOR_B(stepScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(side);
    SDF_VU0_LOADVECTOR_B(waveScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(position);
    SDF_VU0_ADDVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B_NOMEM(next);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(basis);

    for (i = 0; i < segments; i++) {
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_LOADVECTOR_B(width);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(coreVertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(coreVertices + 1);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 2);
        coreVertices += 2;
        vertices += 3;
        SDF_VU0_COPYVECTOR(previousSide, basis);
        SDF_VU0_COPYVECTOR(position, next);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(width);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(coreVertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(coreVertices + 1);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 2);
        coreVertices += 2;
        vertices += 3;
        phaseStep = (EFF_THUNDER_HALF_TURN / segments) * 4.0f;
        cycle += phaseStep;
        phase += phaseStep;
        if (cycle > EFF_THUNDER_HALF_TURN) {
            cycle -= EFF_THUNDER_HALF_TURN;
            amplitude = pThunderPoly->head.waveAmplitude * (1.0f - dds3GetRandFloatA(dds3EffectBase) * 0.95f);
        }
        previousWave = currentWave;
        currentWave = amplitude * sdfSin(phase);
        VEC3_SPLAT(waveScale, currentWave - previousWave);
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_LOADVECTOR_B(stepScale);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(side);
        SDF_VU0_LOADVECTOR_B(waveScale);
        SDF_VU0_MULVECTOR();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(delta);
        SetRotateMatrixAxis(((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f) * (20.0f * (EFF_THUNDER_HALF_TURN / 180.0f)), (const struct sdfVector3_t *)cameraDirection);
        SDF_VU0_LOADVECTOR(delta);
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_ADDVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(next);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_LOADVECTOR(next);
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
        SDF_VU0_LOADVECTOR_B(previousSide);
        SDF_VU0_INNERPRODUCT(facing);
        /* Choose the join edge from the new direction relative to the prior side. */
        if (facing < 0.0f) {
            join = vertices - 1;
            SDF_VU0_LOADVECTOR_C(outerWidth);
        } else {
            join = vertices - 3;
            SDF_VU0_LOADVECTOR_C(negativeOuterWidth);
        }
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_COPYVECTOR_C_TO_B();
        SDF_VU0_MULVECTOR();
        SDF_VU0_LOADVECTOR_B(join);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(position);
    }
}

void effPCP_ThunderPolyLowDraw(effPCP_ThunderPoly_t *hData) {
    s32 index = 0;
    parSystem_t *secondarySystem = hData->state.secondarySystem;
    s32 fragmentCount = hData->head.number;
    parSystem_t *primarySystem = hData->hPoly;
    parCell_t *primaryCells = primarySystem->cells;
    u32 tintColor = hData->color;
    effPCP_ThunderPolyPart_t *fragment = hData->pPart;
    parCell_t *secondaryCells = secondarySystem->cells;
    s32 packedColor;

    if (fragmentCount > 0) {
        do {
            if (fragment->wait == 0) {
                if (fragment->refresh != 0) {
                    effPCP_ThunderPolyLowPartRefresh(hData, index);
                    fragment->refresh--;
                } else if (fragment->abgr & EFF_THUNDER_ALPHA_MASK) {
                    fragment->abgr -= EFF_THUNDER_ALPHA_FADE_STEP;
                } else {
                    effPCP_ThunderPolyLowDrawOff(hData, index);
                    dds3TrackPolyListInit(hData->state.secondarySystem, index);
                    dds3TrackPolyListInit(hData->hPoly, index);
                }
            } else {
                fragment->wait--;
            }
            packedColor = effMagaCylinderCalcColor(fragment->abgr, tintColor);
            secondaryCells[index].color = packedColor;
            primaryCells[index].color = packedColor;
            fragment++;
            index++;
        } while (index < fragmentCount);
    }
    dds3TrackPolyDraw(hData->state.secondarySystem);
    dds3TrackPolyDraw(hData->hPoly);
}

effPCP_ThunderRadiatePoly_t *effPCP_ThunderRadiatePolyCreate(effPCP_ThunderRadiatePolyParam_t *parameters) {
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(parameters->number * sizeof(effPCP_ThunderRadiatePolyPart_t) + sizeof(effPCP_ThunderRadiatePoly_t));
    effPCP_ThunderRadiatePoly_t *work = (effPCP_ThunderRadiatePoly_t *)sdfMemLock(allocationHandle);
    u32 i;

    work->head = *parameters;
    work->pPart = (effPCP_ThunderRadiatePolyPart_t *)(work + 1);
    work->hMem = allocationHandle;
    work->hPoly = dds3TrackPolyCreate(work->head.number, work->head.split * 2 - 1, 0, PAR_CELL_TOPOLOGY_SIX_VECTOR);
    dds3TrackPolyVC3Set_Acute(work->hPoly, work->head.color_center, work->head.color_border, work->head.color_edge);
    dds3TrackPolySetMode(work->hPoly, work->head.blend);
    for (i = 0; i < work->head.number; i++) {
        effPCP_ThunderRadiatePolyDrawOff(work, i);
    }
    return work;
}

void effPCP_ThunderRadiatePolyDelete(effPCP_ThunderRadiatePoly_t *hData) {
    dds3TrackPolyDelete(hData->hPoly);
    sdfMemFreeDirect(hData->hMem);
}

void effPCP_ThunderRadiatePolySetPosition(void *hData, void *pPos) {
    SDF_VU0_COPYVECTOR(hData, pPos);
}

void effPCP_ThunderRadiatePolySetColor(effPCP_ThunderRadiatePoly_t *hData, u32 abgr) {
    hData->abgr = abgr;
}

void effPCP_ThunderRadiatePolyDrawOff(effPCP_ThunderRadiatePoly_t *work, s32 index) {
    effPCP_ThunderRadiatePolyPart_t *cell = work->pPart + index;
    f32 centeredRandom;

    centeredRandom = dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT;
    cell->directionX = centeredRandom + centeredRandom;
    centeredRandom = dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT;
    cell->directionY = centeredRandom + centeredRandom;
    centeredRandom = dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT;
    cell->directionZ = centeredRandom + centeredRandom;
    cell->delayFrames = dds3GetRandA((effRandState_t *)dds3EffectBase) % work->head.startDelayRange;
    cell->activeFrames = dds3GetRandA((effRandState_t *)dds3EffectBase) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
}

void effPCP_ThunderRadiatePolyPartRefresh(effPCP_ThunderRadiatePoly_t *pThunderRadiatePoly, s32 number) {
    f32 width[4] __attribute__((aligned(16)));
    f32 bandWidth[4] __attribute__((aligned(16)));
    f32 outerWidth[4] __attribute__((aligned(16)));
    f32 negativeOuterWidth[4] __attribute__((aligned(16)));
    f32 stepScale[4] __attribute__((aligned(16)));
    f32 position[4] __attribute__((aligned(16)));
    f32 basis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    f32 cameraDirection[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    parSystem_t *system = pThunderRadiatePoly->hPoly;
    effPCP_ThunderRadiatePolyPart_t *source = &pThunderRadiatePoly->pPart[number];
    parCell_t *cell = &system->cells[number];
    u128 *vertices;
    u128 *join;
    s32 i;
    s32 segments;
    f32 distance;
    f32 facing;
    f32 stepLength;

    VEC3_SPLAT(width, pThunderRadiatePoly->head.coreWidth);
    VEC3_SPLAT(bandWidth, width[0] + pThunderRadiatePoly->head.bandWidth);
    VEC3_SPLAT(outerWidth, bandWidth[0] + pThunderRadiatePoly->head.edgeWidth);
    VEC3_SPLAT(negativeOuterWidth, -outerWidth[0]);
    cell->vertexCount = system->vertexWordCount;
    vertices = cell->history;
    segments = pThunderRadiatePoly->head.split;
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(cameraDirection);
    direction[0] = source->directionX;
    direction[1] = source->directionY;
    direction[2] = source->directionZ;
    SDF_VU0_LOADVECTOR_B(direction);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(side);
    distance = pThunderRadiatePoly->head.length / pThunderRadiatePoly->head.split;
    SDF_VU0_COPYVECTOR(position, pThunderRadiatePoly->head.origin);

    for (i = 0; i < segments; i++) {
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_LOADVECTOR_B(width);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 2);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 3);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(bandWidth);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 4);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 5);
        vertices += 6;
        stepLength = distance * (1.0f - dds3GetRandFloatA(dds3EffectBase) * 0.9f);
        VEC3_SPLAT(stepScale, stepLength);
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_LOADVECTOR_B(stepScale);
        SDF_VU0_MULVECTOR();
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(position);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(width);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 2);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 3);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(bandWidth);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 4);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 5);
        vertices += 6;
        SetRotateMatrixAxis(((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f) * (70.0f * (EFF_THUNDER_HALF_TURN / 180.0f)), (const struct sdfVector3_t *)cameraDirection);
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_STOREVECTOR_NOMEM(direction);
        SDF_VU0_LOADVECTOR_B(basis);
        SDF_VU0_INNERPRODUCT(facing);
        if (facing < 0.0f) {
            join = vertices - 1;
            SDF_VU0_LOADVECTOR_C(outerWidth);
        } else {
            join = vertices - 6;
            SDF_VU0_LOADVECTOR_C(negativeOuterWidth);
        }
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_COPYVECTOR_C_TO_B();
        SDF_VU0_MULVECTOR();
        SDF_VU0_LOADVECTOR_B(join);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(position);
    }
}

void effPCP_ThunderRadiatePolyDraw(effPCP_ThunderRadiatePoly_t *hData) {
    s32 i = 0;
    s32 cellCount = hData->head.number;
    effPCP_ThunderRadiatePolyPart_t *cell = hData->pPart;
    parCell_t *renderCells = hData->hPoly->cells;

    if (cellCount > 0) {
        do {
            if (cell->delayFrames == 0) {
                if (cell->activeFrames != 0) {
                    effPCP_ThunderRadiatePolyPartRefresh(hData, i);
                    cell->activeFrames--;
                } else if (renderCells[i].color & EFF_THUNDER_ALPHA_MASK) {
                    renderCells[i].color -= EFF_THUNDER_ALPHA_FADE_STEP;
                } else {
                    effPCP_ThunderRadiatePolyDrawOff(hData, i);
                    dds3TrackPolyListInit(hData->hPoly, i);
                }
            } else {
                cell->delayFrames--;
            }
            cell++;
            i++;
        } while (i < cellCount);
    }
    dds3TrackPolyDraw(hData->hPoly);
}

effPCP_ThunderPolyList_t *effPCP_ThunderPolyListCreate(effPCP_ThunderPolyListParam_t *src) {
    effPCP_ThunderPolyList_t *group = sdfChipAllocDirect(sizeof(effPCP_ThunderPolyList_t));
    s32 i;

    memset(group->handles, 0, sizeof(group->handles));
    group->head = *src;
    group->handles[0] = effPCP_ThunderPolyCreate(&group->head.TPParam);
    effPCP_ThunderPolySetVCAcute2(group->handles[0]);
    for (i = 1; i < src->count - 2; i++) {
        group->handles[i] = effPCP_ThunderPolyCreate(&group->head.TPParam);
        effPCP_ThunderPolySetVCNormal(group->handles[i]);
    }
    group->handles[i] = effPCP_ThunderPolyCreate(&group->head.TPParam);
    effPCP_ThunderPolySetVCAcute(group->handles[i]);
    group->color = EFF_THUNDER_NEUTRAL_COLOR;
    return group;
}

void effPCP_ThunderPolyListDelete(effPCP_ThunderPolyList_t *hData) {
    s32 fragmentIndex;

    for (fragmentIndex = 0; fragmentIndex < hData->head.count - 1; fragmentIndex++) {
        effPCP_ThunderPolyDelete(hData->handles[fragmentIndex]);
    }
    sdfChipFreeDirect(hData);
}

effPCP_ThunderPolyListParam_t *effPCP_ThunderPolyListGetParam(effPCP_ThunderPolyList_t *pTPolyList) {
    return (effPCP_ThunderPolyListParam_t *)pTPolyList;
}

void effPCP_ThunderPolyListSetColor(effPCP_ThunderPolyList_t *hData, u32 abgr) {
    hData->color = abgr;
}

void effPCP_ThunderPolyListPartRefresh(effPCP_ThunderPoly_t *work, s32 index, const u128 *seed) {
    f32 width[4] __attribute__((aligned(16)));
    f32 outerWidth[4] __attribute__((aligned(16)));
    f32 negativeOuterWidth[4] __attribute__((aligned(16)));
    f32 position[4] __attribute__((aligned(16)));
    f32 next[4] __attribute__((aligned(16)));
    f32 basis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    f32 cameraDirection[4] __attribute__((aligned(16)));
    f32 waveScale[4] __attribute__((aligned(16)));
    f32 stepScale[4] __attribute__((aligned(16)));
    f32 previousSide[4] __attribute__((aligned(16)));
    f32 delta[4] __attribute__((aligned(16)));
    parSystem_t *system = work->hPoly;
    parCell_t *cell = &system->cells[index];
    u128 *vertices;
    u128 *join;
    s32 i;
    s32 segments;
    f32 distance;
    f32 amplitude;
    f32 phase;
    f32 cycle;
    f32 currentWave;
    f32 previousWave;
    f32 phaseStep;
    f32 facing;

    VEC3_SPLAT(width, work->head.width_border);
    VEC3_SPLAT(outerWidth, width[0] + work->head.width_edge);
    VEC3_SPLAT(negativeOuterWidth, -outerWidth[0]);
    cell->vertexCount = system->vertexWordCount;
    vertices = cell->history;
    segments = work->head.split;
    SDF_VU0_LOADVECTOR(work->head.end);
    SDF_VU0_LOADVECTOR_B(seed + 3);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(distance);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(direction);
    distance /= work->head.split;
    amplitude = work->head.waveAmplitude;
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(cameraDirection);
    SDF_VU0_LOADVECTOR_B(direction);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(side);
    phase = dds3GetRandFloatA(dds3EffectBase) * (EFF_THUNDER_HALF_TURN * 2.0f);
    cycle = phase - (EFF_THUNDER_HALF_TURN * 2.0f);
    SDF_VU0_COPYVECTOR(position, seed + 3);
    currentWave = amplitude * sdfSin(phase);
    VEC3_SPLAT(waveScale, currentWave);
    SDF_VU0_LOADVECTOR(side);
    SDF_VU0_LOADVECTOR_B(waveScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(position);
    SDF_VU0_ADDVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B_NOMEM(position);
    VEC3_SPLAT(stepScale, distance);
    previousWave = currentWave;
    currentWave = amplitude * sdfSin(phase);
    VEC3_SPLAT(waveScale, currentWave - previousWave);
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_LOADVECTOR_B(stepScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(side);
    SDF_VU0_LOADVECTOR_B(waveScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(position);
    SDF_VU0_ADDVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B_NOMEM(next);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(basis);

    for (i = 0; i < segments; i++) {
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        if (i > 0) {
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADVECTOR(position);
            SDF_VU0_STOREVECTOR_NOMEM(vertices + 2);
            SDF_VU0_COPYVECTOR_B_TO_A();
            SDF_VU0_LOADVECTOR_B(width);
            SDF_VU0_MULVECTOR();
            SDF_VU0_COPYVECTOR_A_TO_C();
            SDF_VU0_LOADVECTOR(position);
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_ADDVECTOR_A_C();
            SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
            SDF_VU0_COPYVECTOR_C_TO_A();
            SDF_VU0_SUBVECTOR_B_A();
            SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 3);
            SDF_VU0_LOADVECTOR(basis);
            SDF_VU0_LOADVECTOR_B(outerWidth);
            SDF_VU0_MULVECTOR();
            SDF_VU0_COPYVECTOR_A_TO_C();
            SDF_VU0_LOADVECTOR(position);
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_ADDVECTOR_A_C();
            SDF_VU0_STOREVECTOR_NOMEM(vertices);
            SDF_VU0_COPYVECTOR_C_TO_A();
            SDF_VU0_SUBVECTOR_B_A();
            SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 4);
            vertices += 5;
        } else {
            SDF_VU0_COPYVECTOR(vertices, seed);
            SDF_VU0_COPYVECTOR(vertices + 1, seed + 1);
            SDF_VU0_COPYVECTOR(vertices + 2, seed + 2);
            SDF_VU0_COPYVECTOR(vertices + 3, seed + 3);
            SDF_VU0_COPYVECTOR(vertices + 4, seed + 4);
            vertices += 5;
        }
        SDF_VU0_COPYVECTOR(previousSide, basis);
        SDF_VU0_COPYVECTOR(position, next);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 2);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(width);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 3);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 4);
        vertices += 5;
        phaseStep = (EFF_THUNDER_HALF_TURN / segments) * 4.0f;
        cycle += phaseStep;
        phase += phaseStep;
        if (cycle > EFF_THUNDER_HALF_TURN) {
            cycle -= EFF_THUNDER_HALF_TURN;
            amplitude = work->head.waveAmplitude * (1.0f - dds3GetRandFloatA(dds3EffectBase) * 0.95f);
        }
        previousWave = currentWave;
        currentWave = amplitude * sdfSin(phase);
        VEC3_SPLAT(waveScale, currentWave - previousWave);
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_LOADVECTOR_B(stepScale);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(side);
        SDF_VU0_LOADVECTOR_B(waveScale);
        SDF_VU0_MULVECTOR();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(delta);
        SetRotateMatrixAxis(((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f) * (20.0f * (EFF_THUNDER_HALF_TURN / 180.0f)), (const struct sdfVector3_t *)cameraDirection);
        SDF_VU0_LOADVECTOR(delta);
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_ADDVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(next);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_LOADVECTOR(next);
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
        SDF_VU0_LOADVECTOR_B(previousSide);
        SDF_VU0_INNERPRODUCT(facing);
        /* Choose the join edge from the new direction relative to the prior side. */
        if (facing < 0.0f) {
            join = vertices - 1;
            SDF_VU0_LOADVECTOR_C(outerWidth);
        } else {
            join = vertices - 5;
            SDF_VU0_LOADVECTOR_C(negativeOuterWidth);
        }
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_COPYVECTOR_C_TO_B();
        SDF_VU0_MULVECTOR();
        SDF_VU0_LOADVECTOR_B(join);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(position);
    }
}

void effPCP_ThunderPolyListDraw(effPCP_ThunderPolyList_t *hData) {
    s32 i = 0;
    s32 segmentCount = hData->head.count - 1;
    s32 j;
    s32 fragmentCount;
    u32 color;
    f32 (*points)[4] = hData->head.points;
    effPCP_ThunderPoly_t *work;
    effPCP_ThunderPoly_t *previous;
    parCell_t *cell;

    for (; i < segmentCount; i++) {
        work = hData->handles[i];
        SDF_VU0_COPYVECTOR(work->head.start, points[0]);
        SDF_VU0_COPYVECTOR(work->head.end, points[1]);
        points++;
        fragmentCount = work->head.number;
        color = effMagaCylinderCalcColor(hData->color, work->color);
        cell = work->hPoly->cells;
        for (j = 0; j < fragmentCount; j++) {
            if (i > 0) {
                previous = hData->handles[i - 1];
                effPCP_ThunderPolyListPartRefresh(work, j,
                    previous->hPoly->cells[j].history + previous->hPoly->cells[j].vertexCount - 5);
            } else {
                effPCP_ThunderPolyPartRefresh(work, j);
            }
            cell[j].color = color;
        }
        work->state.updateCount++;
        dds3TrackPolyDraw(work->hPoly);
    }
}
