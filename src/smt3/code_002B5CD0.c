#include "common.h"
#include "ps2asm.h"

#include "sdf_lighting.h"

typedef f32 (*sdfLightSources_t[3])[4];
extern f32 D_00529630[4];

struct IpuDmaState;
struct sdfDrawTag_t;
union sdfMatUvwParm_t;
union SdfDrawTransform;
struct SdfGsTextureStateValues;
struct SdfDmaTagHeader;
struct SdfObjectRefCommand;
struct MotionEntry;
struct SdfMotionKeyTrack;
struct SdfVuColorTransform;
struct sdf3DFogParam_t;
struct sdf3DTmPacketData_t;
struct SdfDrawPacket;
struct SdfCamera;
struct VuBlendNode;
struct SdfPadEntry;
struct sdf3DViewportPacket_t;
struct DmaPacketHeader;
struct sdf3DFogPacket_t;
struct SdfChunkHeader;
struct SoundFormat;
struct sdfMatPacket_t;
struct SdfDmaReferenceChainPacket;
struct SdfTextureDrawPacket;
struct sdf3DScreenParam_t;
struct sdfThCB_t;
struct sdfTexPacket_t;
struct sdfGsMemSlice_t;
struct sdfIpuDecodeCb_t;
union MovObjStream;
struct movieStream_t;
struct sdfGrFrameSet_t;
struct sdfTexHandle_t;
struct sdfGrFramePacket_t;
struct sdfMat_t;
struct sdfMemHandle_t;
struct DevRequest;
struct SdfCommandNode;
struct SdfDrawNode;
struct sdfModelRootNode_t;
struct SdfMotionCommand;
struct MotionTable;
struct sdfMotionCtrlr_t;
struct SdfMotionBindingDispatch;
struct SdfMotionBindingHead;
struct SdfMotionKeyBinding;
struct SdfMotionIndexedBinding;
struct SdfPrimitiveRequest;
struct TMX_HEADER_t;
struct SdfGeneratedLookupTexture;
struct MovPacStream;
struct sdfFileCmdHandle_t;
/* Write three axis components and twice acos(W), using the native small
 * denominator fallback when sqrt(1-W*W) is near zero. */
void sdfQuatToAxisAngle(f32 *axis, f32 *angle, const f32 *quaternion);
/* Register selectors carried by GIF A+D writes in the two GS contexts. */
enum {
    SDF_GS_PRIM = 0x00,
    SDF_GS_RGBAQ = 0x01,
    SDF_GS_XYZ2 = 0x05,
    SDF_GS_TEX0_1 = 0x06,
    SDF_GS_TEX0_2 = 0x07,
    SDF_GS_CLAMP_1 = 0x08,
    SDF_GS_CLAMP_2 = 0x09,
    SDF_GIF_REGISTER_AD = 0x0E,
    SDF_GS_TEX1_1 = 0x14,
    SDF_GS_TEX1_2 = 0x15,
    SDF_GS_XYOFFSET_1 = 0x18,
    SDF_GS_XYOFFSET_2 = 0x19,
    SDF_GS_PRMODECONT = 0x1A,
    SDF_GS_TEXA = 0x3B,
    SDF_GS_TEXFLUSH = 0x3F,
    SDF_GS_SCISSOR_1 = 0x40,
    SDF_GS_SCISSOR_2 = 0x41,
    SDF_GS_ALPHA_1 = 0x42,
    SDF_GS_ALPHA_2 = 0x43,
    SDF_GS_DTHE = 0x45,
    SDF_GS_COLCLAMP = 0x46,
    SDF_GS_TEST_1 = 0x47,
    SDF_GS_TEST_2 = 0x48,
    SDF_GS_FRAME_1 = 0x4C,
    SDF_GS_FRAME_2 = 0x4D,
    SDF_GS_ZBUF_1 = 0x4E,
    SDF_GS_ZBUF_2 = 0x4F,
    SDF_GS_BITBLTBUF = 0x50,
    SDF_GS_TRXPOS = 0x51,
    SDF_GS_TRXREG = 0x52,
    SDF_GS_TRXDIR = 0x53
};
/* Serialized byte consumed by the GS texture-state packet builder. */
typedef u8 SdfTextureClampMode;
/* IPU register state saved across stream callbacks (0x24 bytes). */
typedef struct IpuDmaState {
    u32 inputAddress;
    u32 inputTagAddress;
    u32 inputQwords;
    u32 inputControl;
    u32 outputAddress;
    u32 outputQwords;
    u32 outputControl;
    u32 bitPointer;
    u32 ipuControl;
} IpuDmaState;
/* One 0x8C allocation owns the stream/sound links and IPU transfer state. */
struct SdfStreamInputDmaTag;
/* The +0x1C movie work pointer selects one of two separately allocated records.
 * isPac at +3 chooses the active arm; the variant payload types live in
 * sdf_movie_stream.h. */
struct MovLinearStream;
/* Semaphore ID and attached work pointers (0x14); DDS1/2 game/code_002D10B0/00329F60.c. */


/* DMA packet list cursors and endpoints (0x20); DDS1/2 game/code_002D33C8/0032C278.c. */
typedef struct sdfDrawTag_t {
    struct sdfDrawTag_t *nextList; /* CPU link between lists; first/last are DMA addresses. */
    u32 first;
    u32 last;
    u32 unkC;
    u32 firstReferenceSource; /* Optional first DMA reference prefix. */
    u32 secondReferenceSource; /* Optional second DMA reference prefix. */
    u32 unk18;
    u32 unk1C;
} sdfDrawTag_t;
/* Optional packed or five-float state block owned by an sdfMat_t. */
typedef union sdfMatUvwParm_t {
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
} sdfMatUvwParm_t;
/* Draw entries store six transform floats and expose the same bytes as three qwords. */
typedef union SdfDrawTransform {
    f32 m[6];
    u64 words[3];
} SdfDrawTransform;
/* The cached GS TEX1, TEX0 and CLAMP values shared by texture draw records. */
typedef struct SdfGsTextureStateValues {
    u64 sampling;
    u64 texture;
    u64 clamp;
} SdfGsTextureStateValues;
/* DMA tag prefix followed by the two VIF words used by indexed-model writers.
 * The control byte stays whole, and address is a hardware word rather than a
 * CPU pointer. */
typedef struct SdfDmaTagHeader {
    u16 quadwordCount;
    u8 reservedByte;
    u8 control;
    u32 address;
    u32 firstVifCode;
    u32 secondVifCode;
} QWORD_ALIGNED SdfDmaTagHeader;
/* Serialized kinds 5/7 name draw nodes by index in the model's buffered list. */
typedef struct SdfObjectRefCommand {
    u8 kind;
    u8 pad01[0x15];
    u16 flags;
    u16 count;
    u16 objectIndices[1];
} SdfObjectRefCommand;
/* Serialized clip duration/reserved remain halfwords (tools/fld.py).
 * The viewer MAXFRAME diagnostic reads their packed little-endian word. */
typedef struct MotionEntry {
    union {
        struct {
            u16 frameCount;
            u16 unk02;
        };
        u32 packedHeader;
    };
    u32 bindingData[1];
} MotionEntry;
/* The first word advances to the next binding record; aligned key-frame words
 * and a variable-length key payload follow the fixed header. */
typedef struct SdfMotionKeyTrack {
    u32 nextTrackOffsetBytes;
    u16 keyCount;
    u16 keyStride;
    u16 keyFrames[1];
} SdfMotionKeyTrack;
/* VU0 input basis and emitted base/delta color vectors. */
typedef struct SdfVuColorTransform {
    u8 opaque00[4]; /* 0x00 */
    f32 scaledY; /* 0x04 */
    u8 opaque08[4]; /* 0x08 */
    f32 scale; /* 0x0C */
    f32 colorBase[4]; /* 0x10 */
    f32 colorDelta[4]; /* 0x20 */
} SdfVuColorTransform;
/* Shared fog/depth state consumed by sdf3DMakeFogPacket (0x14).
 * The viewer config serializes fogNear, depthNear, fogFar, depthFar, fogColor;
 * timeline payloads store fogFar before fogNear instead. */
typedef struct sdf3DFogParam_t {
    f32 fogNear;    /* 0x00: minimum fog range. */
    f32 fogFar;     /* 0x04: maximum fog range. */
    f32 depthNear;  /* 0x08: near depth used in the projection coefficients. */
    f32 depthFar;   /* 0x0C: far depth used in the projection coefficients. */
    u32 fogColor;   /* 0x10: packed GS FOGCOL value. */
} sdf3DFogParam_t;
/* One frame of transformed matrix rows, normalized lighting rows and VIF codes. */
typedef struct sdf3DTmPacketData_t {
    u128 matrix[4];
    u128 scaledRows[3];
    u32 tag[4];
} sdf3DTmPacketData_t;
/* Variable-sized packet header initialized by sdf2DMakePrimPacketHeader. */
typedef struct SdfDrawPacket {
    u16 quadwords;
    u8 pad02[6];
    u32 pad08;
    u32 command;
    u64 gifTag;
    u64 payloadHeader;
} SdfDrawPacket;
/* Native 0x90-byte camera/viewport owner shared by both projection builders. */
typedef struct SdfCamera {
    u32 flags;       // 0x00: 1 half-height, 2 field-of-view projection
    f32 aspect;      // 0x04
    f32 scale;       // 0x08
    f32 fov;         // 0x0C
    f32 offsetX;     // 0x10
    f32 offsetY;     // 0x14
    f32 width;       // 0x18
    f32 height;      // 0x1C
    f32 top;         // 0x20
    f32 bottom;      // 0x24
    f32 nearZ;       // 0x28
    f32 farZ;        // 0x2C
    u8 matrix[0x40]; // 0x30
    f32 halfWidth;   // 0x70
    f32 halfHeight;  // 0x74
    f32 centerY;     // 0x78
    f32 one;         // 0x7C
    f32 originX;     // 0x80
    f32 originY;     // 0x84
    f32 bottomY;     // 0x88
    u32 zero;        // 0x8C
} SdfCamera;
typedef struct VuBlendNode {
    f32 position[4];            /* 0x00: position vector consumed by VU clipping */
    u8 opaque10[0x10];          /* 0x10: only the W lane is used by clipping */
    f32 color[4];               /* 0x20: copied and interpolated as a vector */
    f32 xy[2];                  /* 0x30: interpolated XY pair */
    u8 opaque38[8];             /* 0x38: other lanes are unresolved */
    struct VuBlendNode *next;   /* 0x40 */
    struct VuBlendNode *sourceA; /* 0x44 */
    struct VuBlendNode *sourceB; /* 0x48 */
    f32 weight;                 /* 0x4C */
    u8 region;                  /* 0x50: bit 0 selects the W = 0 clipping path */
    u8 clipFlags;               /* 0x51: clip-plane bits used for triangle culling */
    u8 opaque52[0x0E];          /* 0x52 */
} VuBlendNode;
#define HW_READ(reg) (*(reg))
#define HW_WRITE(reg, value) (*(reg) = (value))
typedef struct SdfPadEntry {
    u8 port;                 /* 0x00 */
    u8 slot;                 /* 0x01 */
    u8 state;                /* 0x02 */
    u8 opaque03;
    u8 mode;                 /* 0x04 */
    u8 requestedMode;        /* 0x05 */
    u16 buttons;             /* 0x06 */
    u16 prevButtons;         /* 0x08 */
    u8 opaque0A[2];
    u32 repeatDeadline;      /* 0x0C */
    u8 stick[4];             /* 0x10 */
    u8 pressure[12];         /* 0x14 */
    s16 smallMotor;          /* 0x20 */
    s16 largeMotor;          /* 0x22 */
    s16 lastSmallMotor;      /* 0x24 */
    s16 lastLargeMotor;      /* 0x26 */
} SdfPadEntry;
/* Shared 0xD0 VIF packet: two stored matrices, three vector slots, and trailer. */
typedef struct sdf3DViewportPacket_t {
    u16 quadwords;        /* 0x00 */
    u8 unknown02[6];      /* 0x02 */
    u32 reservedWord;     /* 0x08 */
    u32 command;          /* 0x0C */
    u8 matrixA[0x40];     /* 0x10 */
    u8 matrixB[0x40];     /* 0x50 */
    u8 vecA[0x10];        /* 0x90 */
    u8 vecB[0x10];        /* 0xA0 */
    u8 vecC[0x10];        /* 0xB0 */
    u32 stmodCommand;     /* 0xC0 */
    u32 mscalCommand;     /* 0xC4 */
    u32 unknownC8;        /* 0xC8 */
    u32 unknownCC;        /* 0xCC */
} QWORD_ALIGNED sdf3DViewportPacket_t;
/* Shared 0x20-byte DMA/VIF reference header. Unrecovered bytes stay opaque. */
typedef struct DmaPacketHeader {
    u16 quadwords; /* 0x00 */
    u8 pad02[2]; /* 0x02 */
    u32 address; /* 0x04 */
    u32 tag; /* 0x08 */
    u32 command; /* 0x0C */
    u16 unknown10; /* 0x10 */
    u8 opaque12[6]; /* 0x12 */
    u32 unknown18; /* 0x18 */
    u32 unknown1C; /* 0x1C */
} QWORD_ALIGNED DmaPacketHeader;
/* Bind resource and asset release queues to their matching destruction callbacks. */
void sdfMatInitSystem(void);
/* Initialize the buffered object-list release queue with its object destructor. */
void sdfModelInitSystem(void);
#define SDF_GIF_PRIM_SHIFT 47
#define SDF_VU_TEXTURED_FLAG 0x10
extern u32 corePktCurrLight;
extern u128 *corePktFaceList[][3];
extern u16 corePktPrimBase;
extern u8 corePktSoftenBias;
extern u16 corePktBackFaceList[];
/* vu0 routine: native clip facing and ordinary triangle fans. */


extern f32 corePktZBias;
extern u32 sdf3DLightDefault;
extern u32 sdf3DFogDefault;
extern f32 sdf3DEyeOffset[4];
extern u8 sdf3DScreenParam[];
/* vu0 routine: native clip-plane setup and ordinary triangle fans. */


/* Indexed triangle count occupies the two bytes following nodeCount. */
typedef struct {
    u8 pad00[0x52];
    s16 indexedTriangleCount;
} VuIndexedTriangleCount;
void corePktEmitTexTris();
extern u8 corePktWorldLightMtxCache[];
#define SDF_DMA_ADDRESS_MASK 0x0FFFFFFF
#define SDF_PAD_ENTRY_COUNT 2
#define SDF_PAD_REPLY_BUFFER_BYTES 0x20
#define SDF_PAD_ACTUATOR_BYTES 6
#define SDF_PAD_REPLY_DIGITAL 0x41
#define SDF_PAD_REPLY_ANALOG 0x73
#define SDF_PAD_REPLY_PRESSURE 0x79
#define SDF_PAD_PRESSURE_COUNT 12
#define SDF_PAD_STICK_CENTER 0x80
#define SDF_PAD_PORT_BUFFER_BYTES 0x100
#define SDF_PAD_BUTTON_STATE_BYTES 0x20
#define SDF_PAD_STICK_STATE_BYTES 8
#define SDF_PAD_PRESSURE_STATE_BYTES 0x18
#define SDF_DMA_QWORD_SHIFT 4
#define SDF_DMA_CHCR_TTE 0x40
#define SDF_QWORD_ALIGNMENT_MASK 0xF
#define SDF_GS_FOGCOL_REGISTER 0x3D
#define SDF_GIF_NREG_SHIFT 60
#define SDF_GIF_PRE_EOP_FLAGS 0x400000008000LL
#define SDF_VIF_DIRECT_COMMAND 0x50000000
#define SDF_VIF_FLUSHA_COMMAND 0x13000000
#define SDF_VIF_ITOP_MATRIX 0x04000002
#define SDF_VIF_ITOP_COMPACT_VERTEX 0x04000002
#define SDF_VIF_ITOP_WIDE_VERTEX 0x04000008
#define SDF_VIF_ITOP_LIGHTING 0x04000010
#define SDF_VIF_MSCAL_COMMAND 0x14000000
#define SDF_VIF_MSCAL_VERTICES 0x14000008
extern u32 corePktCurrFog;
extern u128 corePktFogParm;
/* Cache the object's vector only when its address changes, not its contents. */


extern u8 vu0MicroPacket[];
extern u8 vu1MicroPacket[];
extern s32 sceDmaSync(void *, s32, s32);
extern u8 sdf3DScreenTm[];
extern u8 sdfCameraTm[];
/* fallback up vector */

/* Pack up to three directional lights and the ambient color for VU1: light
 * directions as columns (negated, unit length), colors and their sum, and a
 * basis built from the first light's direction. The axis scratch is volatile:
 * retail keeps its stores in source order. */


/* DMA/VIF prefix, left/right projection terms, then a one-register GIF write
 * and the VU execution command. */
typedef struct sdf3DFogPacket_t {
    u64 dmaTag;
    u64 vifUnpackCode;
    f32 right;
    f32 left;
    f32 mid;
    f32 scale;
    u64 gifTag;
    u64 gifRegister;
    u64 registerValue;
    u64 fogColorRegister;
    u32 mscalCommand;
    u32 reservedA;
    u32 reservedB;
    u32 reservedC;
} sdf3DFogPacket_t;
/* Emit projection depth coefficients and a GS FOGCOL A+D write. */


/* Round payloadBytes up to quadwords and pair FLUSHA with VIF DIRECT. */


extern u8 D_002F5600[];
/* Acquire a 128x64 grayscale power lookup texture; release its temporary image. */


extern u64 sdf3DRefMapParam;
extern u8 D_002E8ED0[];
extern u8 D_002ECF10[];
/* Acquire texture resources, split their TEX0 words, and initialize queues. */


extern u8 sdf3DPrimIdentityTm[];
/* Append the fixed clear block; NULL allocatePacket selects the packet allocator. */


/* vu0 routine: store the resident matrix and inverse-column-length scaled rows, then VIF ITOP/MSCAL. */


/* Append inline matrix/lighting data using allocatePacket or the default allocator. */


extern vu8 sdfFrameId;
/* Apply current-buffer changes and append the asset reference; keep both index reads. */


extern f32 sdfFaceDivSoften;
extern f32 sdfFaceDivBias;
extern u32 sdf3DPrimDefaultFaceIndex[];
/* Serialize the requested VIF attribute streams into one aligned DMA packet. */


/* Build a one-vertex textured-quad VIF packet: the position, color, the four
 * UV corners (copied as unaligned words) and the eight quad corner coordinates. */


/* Build XYZ and attribute VIF streams; return the packet address.
 * vertexCount must be positive; n retains its native count/byte-size reuse. */
/* vu0 routine: pack aligned positions into the VIF three-word stream. */


/* Build a one-vertex VIF packet for a two-texture quad: the position, both
 * colors, the interleaved primary/secondary UV corners (copied as unaligned
 * words) and the eight quad corner coordinates. */


/* Build a quad packet from paired colors and per-corner UV sets. */


/* Build a one-vertex textured-quad VIF packet with four corner colors: the
 * position, colors, UV corners (copied as unaligned words) and the eight quad
 * corner coordinates. */


/* Preserve the native 76-byte-record formula and 16-byte alignment rounding. */


/* Build a quad packet with four corner colors, UV pairs and XY offset pairs. */


/* Build a one-vertex VIF packet for a two-texture quad with eight colors: the
 * position, colors, interleaved primary/secondary UV corners (copied as
 * unaligned words) and the eight quad corner coordinates. */


/* Build the wider attribute VIF stream; return the packet address.
 * vertexCount must be positive; preserve the native copy lengths and n reuse. */
/* vu0 routine: pack aligned positions into the VIF three-word stream. */


extern u8 sdfPadCb_0[];
extern u8 D_005518E0[8];
extern s8 dds3PadBitTable[0x20];
extern u8 D_005298E8[0x18];
/* Emit held bit 0, press/repeat trigger bit 1, and new-press bit 7 per button.
 * Changes arm a 15-tick deadline; repeats set it to current tick + lateness + 4,
 * retaining the original extra delay when a deadline is missed. */


/* Store a mode request for the indexed logical pad; setup applies it later. */


/* Store the small-motor request without validating the logical pad index. */


/* Store the large-motor request without validating the logical pad index. */


/* Despite the legacy console-prefixed name, set both pad motor low bytes. */


extern u8 sdfPadPortSlotPairs[4];
extern u8 D_0058F0C0[];
extern u8 sdfPadManagerMode;
/* Variable-size serialized chunk streams share only this eight-byte header.
 * The word at +8 belongs to each chunk's payload. */
typedef struct SdfChunkHeader {
    u32 id;   /* 0x00: zero terminates the chunk stream */
    u32 size; /* 0x04: byte extent to the next chunk */
} SdfChunkHeader;
/* Protocol IDs use distinct byte-sized storage codes in sdfFileCmdHandle_t. */
typedef u8 SdfDevOperationCode;
typedef s8 SdfDevStateCode;
/* These four bytes are consumed by the same sound-format initializer. */
typedef struct SoundFormat {
    u8 hasAudio;
    u8 stereo;
    u8 loopMode;
    u8 playbackCadenceStep;
} SoundFormat;
/* Normalize the supplied axis and write an XYZW quaternion. The native
 * conversion uses nonnegative square roots for both half-angle components. */
void sdfQuatFromAxisAngle(f32 *out, f32 x, f32 y, f32 z, f32 angle);
/* Native 0xA0-byte draw-entry block allocated by SDF_ASSET_DRAW_ENTRY_BYTES. */
typedef struct sdfMatPacket_t {
    u32 pad00;             /* 0x00 */
    u32 unk04;             /* 0x04 */
    u32 unk08;             /* 0x08 */
    u32 secondaryColor;    /* 0x0C: packed color applied by the secondary draw state */
    u32 primaryStateWordThird; /* 0x10: copied from the owning asset, or forced to zero. */
    u32 unk14;             /* 0x14 */
    u32 pad18;             /* 0x18 */
    f32 unk1C;             /* 0x1C */
    u32 alphaState;        /* 0x20: packed GS ALPHA_2 state emitted by the VU transfer builder */
    u32 secondaryMode;     /* 0x24: secondary packet mode copied from sdfMat_t */
    f32 y;                 /* 0x28 */
    f32 x;                 /* 0x2C */
    u8 pad30[8];           /* 0x30 */
    SdfGsTextureStateValues primaryTextureState; /* 0x38 */
    SdfGsTextureStateValues secondaryTextureState; /* 0x50 */
    SdfDrawTransform transforms[2]; /* 0x68 */
    u32 vifItopCommand;    /* 0x98: VIF ITOP with immediate 0xC */
    u32 vifMscalCommand;   /* 0x9C: VIF MSCAL with microprogram index zero */
} sdfMatPacket_t;
/* GIF tag followed by a REF transfer and the patchable NEXT link. */
typedef struct SdfDmaReferenceChainPacket {
    sdfGrPacketHeader_t header;
    u64 referenceTag;
    u64 referenceVifCommands;
    u64 nextTag;
    u64 zeroTailWord;
} SdfDmaReferenceChainPacket;
void sdfAppendPacketRange(struct sdfDrawTag_t *list, u32 packetAddress,
                          u32 rangeTailAddress);
/* Fixed-size texture packet; constructor allocation and writes cover 0x50. */
typedef struct SdfTextureDrawPacket {
    SdfDrawPacket header;
    u64 textureWordA;
    u64 registerAddressA;
    u64 textureWordB;
    u64 registerAddressB;
    u64 textureWordC;
    u64 registerAddressC;
} SdfTextureDrawPacket;
/* Complete static projection record, including the inverse-origin cache. */
typedef struct sdf3DScreenParam_t {
    SdfCamera camera;
    f32 inverseOrigin[4]; /* 0x90 */
} sdf3DScreenParam_t;
/* Direct secondary-text sampling has an intentionally empty capturePrevious callback. */


/* Bind the selected pair of model slot indices and weights. */


/* Sample the supplied frame and interpolate, retaining separate keys when IDs differ. */


/* Blend sampled and retained weights, keeping at most two model slots. */


/* Preserve the complete current pair for the next weighted transition. */


/* Matrix registers: vf28-vf31 are the primary matrix, vf24-vf27 its
 * alternate bank, and vf20-vf23 a third bank copied between the two. */


/* libvu0: sceVu0UnitMatrix */


/* Transpose the four VU rows; EE MMI interleave is needed for packed vectors. */
/* libvu0: sceVu0TransposeMatrix, register form (vf28-vf31 in and out) */


/* vu0 routine: rigid inverse of vf28-vf31 (transpose the 3x3, translation = -(R^T * t)) */


/* vu0 routine: inverse of vf28-vf31 with per-axis scale removed (transpose, rows / |row|^2, translation = -(R^T * t)) */


extern f32 cosf(f32 angle);
extern f32 sinf(f32 angle);
extern SdfPadEntry sdfPadCb[];
/* Convert three Euler angle components to the VU0 quaternion output.
 * Providers: DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */
void FromEuler(f32 x, f32 y, f32 z);
void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);
void sdfDrawTagLinkRef(struct sdfDrawTag_t *list, u32 packetAddress);
typedef struct {
    f32 matrix[4][4];      /* 0x00 */
    u16 param0;            /* 0x40 */
    s16 param1;            /* 0x42 */
    u32 selectedFlags;     /* 0x44 */
    u32 nextParam;         /* 0x48 */
    u32 flags;             /* 0x4C */
    s16 nodeCount;         /* 0x50 */
    s16 selectedTriangleCount; /* 0x52: rejected-triangle index count */
    sdfMatPacket_t *node;   /* 0x54 */
    u32 packedColor;       /* 0x58: packed RGBA word */
    f32 offsetX;           /* 0x5C */
    f32 offsetY;           /* 0x60 */
    u32 dmaBase;           /* 0x64 */
    VuBlendNode *dmaAddrA; /* 0x68: next free ring node */
    VuBlendNode *dmaAddrB; /* 0x6C: ring wrap node */
    u32 dmaCountA;         /* 0x70 */
    u32 dmaCountB;         /* 0x74 */
    VuBlendNode *blendList; /* 0x78 */
    VuBlendNode **dmaEnd;  /* 0x7C: list tail link */
    u32 packetStart;       /* 0x80 */
    u32 header;            /* 0x84: aligned packet cursor + 0x30 */
    u32 *ringStart;        /* 0x88 */
    u32 *cursor;           /* 0x8C */
    u8 *payload;           /* 0x90 */
    u8 *strip;             /* 0x94 */
    u8 *positions;         /* 0x98 */
    u8 *normals;           /* 0x9C */
    u8 *coordinates;       /* 0xA0 */
    u8 *secondCoordinates; /* 0xA4 */
    u8 *vertexColors;      /* 0xA8 */
    f32 depth;             /* 0xAC */
} VuWork;

extern void corePktXfmUV(void *vertices, s32 count, void *uv, void *transform);
extern void corePktApplyFaceDivColor(VuWork *work, void *parameters);
extern void sdfGrClosePacketBuff(u32 cursor);
void corePktEmitTexTrisCtx2(VuWork *work, u64 samplingState, u64 textureState,
                          u64 clampState, u64 alphaState, s32 clearPrimitiveFlags);
/* Finish the selected node's transformed rows and emit its textured geometry. */ void corePktEmitMultiTexPass(VuWork *work);
void sdfTexSetWrapMode(struct sdfTexHandle_t *texture, SdfTextureClampMode mode);
/* Linked registry node stored in the title's tracked-thread table (0x8). */
typedef struct sdfThCB_t {
    struct sdfThCB_t *next; /* 0x00 */
    s32 threadId;               /* 0x04 */
} sdfThCB_t;
/* Register a thread node, then start its entry function on the supplied stack. */
void sdfStartThreadAndWait(sdfThCB_t *node, void (*entry)(void),
                           void *stack, s64 stackSize, s32 priority, s32 arg);
/* Per-texture reference count and non-owning clone source (0x8). */
/* Texture buffer GPU command words (0x40); DDS1/2 game/code_002D10B0/00329F60.c. */
typedef struct sdfTexPacket_t {
    s32 gifTagWord; /* Low GIFtag word; NLOOP occupies bits 0-14. */
    u32 gifTagControl; /* High GIFtag control word; one packed register. */
    u64 gifTagRegisters; /* Packed register list: A+D. */
    u64 samplingState; /* GS TEX1 data, including minification/magnification filters. */
    u64 samplingRegister; /* GS TEX1_1/TEX1_2 register selector. */
    u64 textureState; /* GS TEX0 data. */
    u64 textureRegister; /* GS TEX0_1/TEX0_2 register selector. */
    u64 clampState; /* GS CLAMP data. */
    u64 clampRegister; /* GS CLAMP_1/CLAMP_2 register selector. */
} sdfTexPacket_t;
/* Native VRAM range descriptor (0x1C), shared by textures, graph buffers and streams. */
typedef struct sdfGsMemSlice_t {
    struct sdfGsMemSlice_t *prev; /* 0x00 */
    struct sdfGsMemSlice_t *next; /* 0x04 */
    u32 allocationMode;         /* 0x08: known categories above; unknown values remain valid */
    u32 word;                   /* 0x0C: VRAM offset in 32-bit words */
    s32 size;                   /* 0x10: range length in 32-bit words */
    s16 width;                  /* 0x14 */
    s16 height;                 /* 0x16 */
    s32 format;                 /* 0x18: GS pixel-storage mode */
} sdfGsMemSlice_t;
typedef struct sdfIpuDecodeCb_t {
    struct sdfIpuDecodeCb_t *streamPrev;
    struct sdfIpuDecodeCb_t *streamNext;
    struct sdfIpuDecodeCb_t *next;
    u8 active;
    u8 queued;
    u8 ipuCycleInitialized; /* First-use IPU setup has run for this playback cycle. */
    u8 drained;
    u8 playbackPhase;
    u8 inputFeedEnabled; /* Restart input feeding after DMA completion while input remains. */
    u8 inputDmaStartPending;
    u8 pendingPlaybackSubmissions; /* Queued output submissions not yet retired by a playback update. */
    u8 audioMode;
    u8 loopMode;
    u8 playbackCadenceStep;
    u8 playbackCadenceRemainder;
    u8 bufferIndex;
    u8 transferPacketIndex;
    u8 completedBufferCount; /* Completed output buffers waiting for playback submission. */
    u8 pad1B;
    s32 bufferSize;
    void *frameBuffers[2];
    void *transferPacketBuffers[2];
    u32 transferPacketBytes;
    s32 resourceWord;
    sdfGsMemSlice_t *textureHead;
    u16 width;
    u16 height;
    u32 cycleLength;
    u32 tickCount;
    s32 playbackFrameIndex;
    struct SdfStreamInputDmaTag *inputDmaChain;
    u8 headerReady;
    u8 done;
    u8 filledSlots;
    u8 firstSlot;
    u8 *scratchBuffer;
    u8 pad58[4];
    s32 (*read)(struct sdfIpuDecodeCb_t *, struct movieStream_t *, s32, void *, s32);
    struct movieStream_t *readContext; /* Owner context passed to the read callback. */
    u8 inputFeedDmaInFlight;
    u8 unk65;
    u8 pad66[2];
    IpuDmaState dma;
} sdfIpuDecodeCb_t;
typedef s32 (*SdfStreamRead)(sdfIpuDecodeCb_t *, struct movieStream_t *, s32, void *, s32);
void sdfIpuStrmDecodeInit(sdfIpuDecodeCb_t *node, struct SoundFormat *format, SdfStreamRead read, struct movieStream_t *readContext);
void sdfSoundInitFormattedAndAppendNode(sdfIpuDecodeCb_t *node, struct SoundFormat *format, SdfStreamRead read, struct movieStream_t *readContext, s32 resource);
typedef union MovObjStream {
    struct MovLinearStream *linear;
    struct MovPacStream *pac;
} MovObjStream;
typedef struct movieStream_t {
    u8 active;
    u8 state;
    u8 stopRequested;
    u8 isPac;
    u16 unk04;
    u16 unk06;
    u32 unk08;
    u16 unk0C;
    u16 unk0E;
    struct sdfFileCmdHandle_t *deviceState;
    s32 totalBytes;
    s32 remainingBytes;
    MovObjStream stream;
    u8 pacEnabled;
    u8 pad21;
    u8 packetLimit;
    u8 pad23;
    sdfIpuDecodeCb_t soundNode;
} movieStream_t;
/* Graph target: two color buffers followed by the auxiliary/depth buffer (0x14). */
typedef struct sdfGrFrameSet_t {
    s16 width;
    s16 unk2;
    s16 height;
    u8 bufferFormat;
    u8 auxiliaryFormat;
    sdfGsMemSlice_t *buffers[3];
} sdfGrFrameSet_t;
/* Linked texture and its two buffers/resources (0x40); DDS1/2 sdf/sdfTex.c and game texture units. */
typedef struct sdfTexHandle_t {
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
    s32 paletteDataSize; /* 0x34: palette byte count from sdfTexGetPaletteByteSize. */
    s32 unk38;
    u8 *intensityMap; /* 0x3C: lazily generated weighted-RGB intensity bytes. */
} sdfTexHandle_t;
/* Return packed 64-bit TEX0, TEX1, and CLAMP states from the primary GS packet. */
u64 sdfTexGetTex0Reg(sdfTexHandle_t *texture);
u64 sdfTexGetTex1Reg(sdfTexHandle_t *texture);
u64 sdfTexGetClampReg(sdfTexHandle_t *texture);
/* Frame/depth builder state: one DMA/GIF header and two A+D register writes. */
typedef struct sdfGrFramePacket_t {
    u8 pad00[4];
    void (*prepare)(struct sdfGrFramePacket_t *, s32 bufferIndex);
    u8 pad08[8];
    sdfGrPacketHeader_t packetHeader;
    sdfGrTagStdReg_t frame;
    sdfGrTagStdReg_t zbuf;
    sdfGrFrameSet_t *source;
    u32 frameMask;
    s32 region;
    s32 mode;
} sdfGrFramePacket_t;
void sdfGrFramePacketPredraw(sdfGrFramePacket_t *packet, s32 bufferIndex);
void sdfGrSetFramePacket(sdfGrFramePacket_t *packet, sdfGrFrameSet_t *source,
    u32 frameMask, s32 region, s32 mode);
/* Asset holding texture and resource entries (0x48); DDS1/2 game/code_002D9748/003325F8.c. */
typedef struct sdfMat_t {
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
} sdfMat_t;
/* General-heap allocation descriptor (0x10); links bound the represented span. */
typedef struct sdfMemHandle_t {
    struct sdfMemHandle_t *prev; /* 0x00 */
    struct sdfMemHandle_t *next; /* 0x04 */
    s32 address;              /* 0x08: start of represented span */
    u16 state;                /* 0x0C: free, used, or end sentinel */
    s16 referenceCount;       /* 0x0E: -1 for sentinels */
} sdfMemHandle_t;
typedef struct DevRequest {
    struct sdfMemHandle_t *backingAllocation;
    s16 usedCount;
    u16 capacity;
    s16 stride;
    s16 growStep;
    void *buffer;
} DevRequest;
DevRequest *sdfItemArrayNew(s32 count, s32 stride, s32 growStep);
/* Command-list node: allocation owns the node and trailing command data.
 * A null descriptor identifies a separately chip-allocated node. */
typedef struct SdfCommandNode {
    struct SdfCommandNode *next;
    u8 kind;
    s8 packetSelector;
    s16 quadwordCount;
    void *payload; /* Packet bytes for kind 1; serialized commands for kinds 2/3. */
    struct sdfMemHandle_t *allocation;
} SdfCommandNode;
/* Allocated as 0x100-byte nodes; each owns a circular list of child draw nodes. */
typedef struct SdfDrawNode {
    struct SdfDrawNode *previous; /* 0x00 */
    struct SdfDrawNode *next;     /* 0x04 */
    struct SdfDrawNode *parent;   /* 0x08 */
    struct SdfDrawNode *children; /* 0x0C */
    struct sdfModelRootNode_t *root;        /* 0x10: backlink installed by sdfModelNodeAdd */
    u16 flags;                   /* 0x14 */
    s16 unk16;                   /* 0x16: constructor sentinel is -1 */
    s32 nodeId;                   /* 0x18: lookup key when model flags bit 0 is set */
    u32 color;                   /* 0x1C */
    u8 pad20[8];
    SdfCommandNode *lists[2];     /* 0x28: command lists for both buffered frames */
    u8 *workspace; /* Two 0x80-byte frame packets, allocated on first use. */                  /* 0x30 */
    const f32 *boundsCorners;     /* 0x34: optional two xyz corners used by clipping */
    void *sourceItem;             /* 0x38: item this node was built from */
    u8 pad3C[0x14];
    f32 quaternion[4];           /* 0x50 */
    f32 translation[4];          /* 0x60 */
    f32 scale[4];                /* 0x70 */
    f32 localMatrix[4][4];       /* 0x80: all four local basis/translation rows */
    f32 worldMatrix[4][4];       /* 0xC0: local transform composed with its parent */
} SdfDrawNode;
/* The buffered-transform constructor allocates and clears all 0x9C bytes. */
typedef struct sdfModelRootNode_t {
    DevRequest *list;
    SdfDrawNode *rootNode;
    void *assetData;
    DevRequest *resources;
    DevRequest *slotPairs;
    struct sdfMotionCtrlr_t *motionList;
    u8 unk18;
    u8 flags;
    u16 drawFlags;
    u32 color;
    f32 matrix[4][4];
    f32 rotationQuaternion[4];
    f32 scaleVector[4];
    struct SdfDmaTagHeader *lighting; /* Borrowed first DMA reference prefix. */
    struct SdfDmaTagHeader *secondaryLighting; /* Borrowed second DMA reference prefix. */
    f32 scalarOverrideFirst;
    f32 scalarOverrideSecond;
    struct SdfChunkHeader *chunkTable; /* Borrowed from the model resource data. */
    /* Model scale read by sdfVuSetGlobalScale and copied unchanged into the
     * three-scalar VIF payload (DDS1 0x2D9530). */
    f32 modelScale;
    u8 lodIndex; /* Native model-viewer and MODEL_LOD_CHG setters use byte 0x98. */
    u8 pad99[3];
} sdfModelRootNode_t;
SdfDrawNode *sdfModelSearchNode(sdfModelRootNode_t *model, s32 nodeIdOrIndex);
typedef struct SdfMotionCommand {
    union {
        u32 packedCommand;
        struct {
            /* Runtime dispatch shifts the command word right by 16. */
            u16 selector;
            u16 family;
        } binding;
    };
    u32 targetIndex;
} SdfMotionCommand;
/* Resource header followed by the binding commands used to construct a motion. */
typedef struct MotionTable {
    u16 entryCount; /* Motion-index upper bound returned by mdlGetNumOfCut. */
    u16 commandCount;
    MotionEntry **entries;
    SdfMotionCommand commands[1];
} MotionTable;
/* SDK motions are allocated as 0x34-byte records; model slots retain these nodes. */
typedef struct sdfMotionCtrlr_t {
    struct sdfMotionCtrlr_t *next;
    sdfModelRootNode_t *owner;
    MotionTable *motionTable;
    s32 unkC;
    DevRequest *request;
    f32 blendDurationFrames;
    f32 blendStartFrame;
    f32 currentFrame;
    f32 frameStep;
    s32 unk24;
    s16 searchId;
    s16 slotIndex;
    u16 motionIndex;
    u16 frameCount;
    u8 state;
    u8 previousState;
    u8 loopEnabled;
    u8 pad33;
} sdfMotionCtrlr_t;
/* Motion callbacks release storage, bind keys, sample, capture and blend. */
typedef struct SdfMotionBindingDispatch {
    void (*releaseStorage)(void *binding);
    void (*bindKeyTrack)(void *binding, struct SdfMotionKeyTrack *track);
    void (*sample)(void *binding, f32 frame);
    void (*capturePrevious)(void *binding);
    void (*blend)(void *binding, f32 frame, f32 weight);
    u32 unk14;
} SdfMotionBindingDispatch;
/* Common dispatch/motion prefix used by motion-owned bindings. */
typedef struct SdfMotionBindingHead {
    SdfMotionBindingDispatch *dispatch;
    sdfMotionCtrlr_t *motion;
} SdfMotionBindingHead;
void sdfSetMotionPointerPair(SdfMotionBindingHead *binding, sdfMotionCtrlr_t *motion,
                             SdfMotionBindingDispatch *dispatch);
typedef struct SdfMotionKeyBinding {
    SdfMotionBindingDispatch *dispatch;
    sdfMotionCtrlr_t *motion;
    SdfMotionKeyTrack *track;
} SdfMotionKeyBinding;
/* Indexed asset bindings share the key prefix and retain the selected asset. */
typedef struct SdfMotionIndexedBinding {
    SdfMotionKeyBinding keys;
    sdfMat_t *target;
} SdfMotionIndexedBinding;
void sdfMotGenMatInit(SdfMotionIndexedBinding *binding, sdfMotionCtrlr_t *motion,
    SdfMotionBindingDispatch *dispatch, s32 options);
/* DMA/VIF transfer header and packed A+D GIF tag, excluding register payload. */
void sdfGrSetRegsPacketHeader(sdfGrPacketHeader_t *header, s32 payloadQwords);
void sdfBuildDmaReferenceChain(SdfDmaReferenceChainPacket *packet,
    u32 sourceAddress, s32 qwordCount);
/* Append the common rectangle used to frame a grid of sprite cells. */
void dds3ConsPanelTagLink(struct sdfDrawTag_t *packetList, s32 column, s32 row,
                        s32 columnCount, s32 rowCount);
/* Write the reference header for one buffered asset draw entry and return
 * the packet payload immediately after the header. */
struct SdfDmaTagHeader *sdfMatMakePacket(struct sdfMat_t *asset,
                                                           struct SdfDmaTagHeader *tag,
                                                           s32 frame);
/* Refresh one of the two draw entries and retain the other entry's dirty bits. */
void sdfMatUpdatePacket(struct sdfMat_t *asset, s32 entryIndex);
void sdfMatSetTexUvw(struct sdfMat_t *asset, f32 a, f32 b, f32 c, f32 d, f32 e);
void sdfMatSetFaceDivParm(struct sdfMat_t *asset, f32 first, f32 second);
u32 sdf2DGetTexPacketSize(sdfTexHandle_t *texture);
struct sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);
u32 sdfMemLock(struct sdfMemHandle_t *allocation);
void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);
void sdfMemFreeHandleDirect(s32 *slot);
/* Native 0x2C VIF primitive request, shared by its quad/triangle producers.
 * Attribute buffers carry serialized words; coordinate floats retain their bits.
 * Native callers supply a positive vertex count and aligned 16-byte position/normal rows. */
typedef struct SdfPrimitiveRequest {
    s16 stripWordCount;
    s16 vertexCount;
    u16 primitiveFlags;
    u16 clipMask;
    u32 color;
    const void *strip;
    const void *positions;
    const void *normals;
    const void *coordinates;
    const void *secondCoordinates;
    const void *vertexColors;
    void *(*allocate)(s32);
    f32 depth;
} SdfPrimitiveRequest;
/* Serialized texture-resource header; payload begins after this 0x40-byte prefix. */
typedef struct TMX_HEADER_t {
    u8 unk00;
    u8 payloadOffsetFlags; /* 0x01: high nibble adds payload offset bytes; low bits stay opaque. */
    u8 pad02[2];       /* 0x02 */
    u32 byteCount;     /* 0x04 */
    u32 magic;         /* 0x08 */
    u8 pad0C[4];       /* 0x0C */
    u8 paletteCount;   /* 0x10 */
    u8 clutFormat;     /* 0x11 */
    s16 width;         /* 0x12 */
    s16 height;        /* 0x14 */
    u8 pixelFormat;    /* 0x16 */
    u8 maxMipLevel;    /* 0x17 */
    u16 lodParameters; /* 0x18 */
    u8 unk1A;          /* 0x1A */
    SdfTextureClampMode clampMode; /* 0x1B: serialized GS clamp mode */
    s32 resourceKey;   /* 0x1C */
    s32 battleTextureSlot; /* 0x20: selects one of two battle-model texture headers */
    u8 pad24[0x1C];    /* 0x24 */
} TMX_HEADER_t;
/* Acquire a texture from a serialized texture-file header. */
struct sdfTexHandle_t *sdfTexSet(const TMX_HEADER_t *header);
struct sdfTexHandle_t *sdfTexSetDirect(const TMX_HEADER_t *header);
/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 size);
/* Read the active packet cursor or restore it at a quadword boundary. */
s32 sdfGrOpenPacketBuff(void);
/* Store the mode request for a logical pad; setup applies it later. */
void sdfPadRequestMode(s32 padIndex, u8 mode);
void sdfDevResizeBufferedRequest(DevRequest *request, s32 count);
/* The draw node owns the lazily allocated workspace; callers receive a borrowed
 * pointer to it, and must not release it independently. */
u8 *sdfModelAllocTmPacket(SdfDrawNode *root);
/* Generated 128x64 grayscale power lookup texture: a serialized texture
 * header, 256 RGBA palette words, then the full 64x128 index image. */
typedef struct SdfGeneratedLookupTexture {
    TMX_HEADER_t header; /* 0x0000 */
    u32 palette[256];            /* 0x0040 */
    u8 indices[64][128];         /* 0x0440 */
} SdfGeneratedLookupTexture;
static inline VuBlendNode *vuAllocNode(VuWork *work) {
    VuBlendNode *node = work->dmaAddrA;

    if (--work->dmaCountA == 0) {
        work->dmaAddrA = work->dmaAddrB;
        work->dmaCountA = work->dmaCountB;
    } else {
        work->dmaAddrA = node + 1;
    }
    *work->dmaEnd = node;
    work->dmaEnd = &node->next;
    return node;
}
extern s32 corePktPointBuff;
extern void corePktRelightBackFaces(u32 base, s32 count);
extern sdfTexHandle_t *D_005518B8;
extern sdfTexHandle_t *sdf3DSpecularTex;
extern sdfTexHandle_t *D_005518C8;
extern s32 sdfCoreDrawBuffHandle;
extern void *sceDmaGetChan(s32);
extern void sceDmaSendN(void *, void *, s32);
/* Append the second fixed program block as a DMA reference packet. */


extern void sdfTmxSetHeader(TMX_HEADER_t *, s32, s32, s32, s32, s32);
extern s32 vu1Tex0Slot0[];
extern s32 vu1Tex0Slot2[];
extern s32 vu1Tex0Slot1[];
extern s32 scePadGetState(s32 port, s32 slot);
extern s32 scePadRead(s32 port, s32 slot, void *data);
extern s32 scePadGetReqState(s32 port, s32 slot);
extern s32 scePadInfoAct(s32 port, s32 slot, s32 actNo, s32 term);
extern s32 scePadSetActDirect(s32 port, s32 slot, void *actData);
extern s32 scePadSetMainMode(s32 port, s32 slot, s32 offs, s32 lock);
extern s32 scePadSetActAlign(s32 port, s32 slot, void *data);
extern s32 scePadInit(s32);
extern s32 scePadPortOpen(s32 port, s32 slot, void *buffer);
/* Chip-heap work record for movie-PAC streaming (0x78 bytes). The
 * block-mask allocation and the backing ring allocation are separate owners. */
typedef struct MovPacStream {
    u8 pad00[0x18];
    s32 packetBytes;
    s32 blockBytes;
    u8 scratchBuffer[0x20];
    struct sdfMemHandle_t *payloadAllocation;
    u8 *blockMask;
    s32 blockIndex;
    struct sdfMemHandle_t *allocation;
    u8 *pacInputBuffer;
    u8 *pacBuffer;
    s32 pacReadOffset;
    s32 pacBufferedBytes;
    u8 *ringBuffer;
    s32 ringOffset;
    s32 ringLength;
    s32 unk6C;
    s32 scratchSize;
    u8 *scratch;
} MovPacStream;
/* Complete 0x40-byte owner allocated by sdfFileCmdNewHandle. */
typedef struct sdfFileCmdHandle_t {
    struct sdfFileCmdHandle_t *next; /* 0x00 */
    struct sdfFileCmdHandle_t *previous; /* 0x04 */
    struct sdfFileCmdHandle_t *workerNext; /* 0x08 */
    struct sdfFileCmdHandle_t *workerPrev; /* 0x0C */
    char *resolvedPath; /* 0x10 */
    u8 workerIndex; /* 0x14 */
    SdfDevOperationCode operation; /* 0x15 */
    SdfDevStateCode state; /* 0x16 */
    u8 pad17; /* 0x17 */
    s32 operationArg; /* 0x18 */
    s32 requestExtra; /* 0x1C */
    void *requestData; /* 0x20 */
    s32 options; /* 0x24 */
    s32 resourceId; /* 0x28 */
    s32 result; /* 0x2C */
    s32 transferred; /* 0x30 */
    u8 pad34[4]; /* 0x34 */
    void (*callback)(struct sdfFileCmdHandle_t *, s32, s32, s32, s32); /* 0x38 */
    s32 callbackContext; /* 0x3C: opaque callback-context word */
} sdfFileCmdHandle_t;
s32 sdfBillBoardGetPacketSize(s32 count);
extern void *memcpy(void *, const void *, unsigned int);
/* Copy the selected fixed order into its actor-slot bank. */
void btlInitializeActorSlotOrder(s32 selector, s32 count);

void LoadIdentity(void);
void DiagMatrix(void);
void ReverseMatrix(void);
void InversMatrix(void);
void SetRotateMatrixX(f32 angle);
void SetRotateMatrixY(f32 angle);
void SetRotateMatrixZ(f32 angle);
void StoreRotateMatrixAxis(f32 angle, const sdfVector3_t *axis, sdfMatrix4_t *matrix);
void SetRotateMatrixAxis(f32 angle, const sdfVector3_t* axis);
void SetRotateMatrixX_B(f32 angle);
void SetRotateMatrixY_B(f32 angle);
void SetRotateMatrixZ_B(f32 angle);
void SetRotateMatrixAxis_B(const sdfVector3_t* axis, f32 angle);
void RotateMatrixX(f32 angle);
void RotateMatrixY(f32 angle);
void RotateMatrixZ(f32 angle);
void SetRotateMatrixXYZ(const sdfVector3_t *rot);
void MultMatrixNoArg(void);
void MultMatrixRevNoArg(void);
void MultMatrix(void *matrix);
void MultMatrixRev(const void *matrix);
void LookAtMatrix(const void *directionPoint, const void *basisOrigin, const void *up);
void corePktSetupPointBuff(VuWork *work, s32 ringPosition);
void corePktInitCb(VuWork *work, u16 *parameterWords, u32 selectionMask);
void corePktLoadWorldLightMtx(void *vectors);
void corePktMakeFaceDivParm(SdfVuColorTransform *result, u32 reference, u32 packedColor,
                   u32 firstColor, u32 secondColor,
                   f32 blend, f32 scale, f32 y);
void corePktSetFaceDivParm(SdfVuColorTransform *out, sdfMatPacket_t *work, u32 packedColor, f32 deltaX, f32 deltaY);
void corePktLerpUVColorChain(VuBlendNode *node);
void corePktSetConstColor(VuWork *work, u32 color);
void corePktMakeEdgePoint(VuBlendNode *out, VuBlendNode *first, VuBlendNode *second);
void corePktClipFace(VuWork *work);
void corePktBuildFaceList(VuWork *work);
void corePktBuildBackFaceList(VuWork *work);
void corePktEmitBase(VuWork *work);
void corePktEmitAllPasses(VuWork *work);
void corePktOpenAndEmit(VuWork *work);
void sdfCoreDrawScanOneSkinNodes(sdfModelRootNode_t *model, SdfObjectRefCommand *source);
void sdfCoreDrawSetFog(u8 *sourceObject);
void sdfCoreDrawInit(s32 workspaceBytes);
SdfTextureDrawPacket *sdf2DMakeTexPacket(SdfTextureDrawPacket *drawPacket, sdfTexHandle_t *texture, s32 contextOffset);
s32 sdf2DLinkTexPacket(sdfDrawTag_t *packetList, sdfTexHandle_t *texture, s32 contextOffset);
u32 sdf2DMakeStdPacket(u32 packetAddress, s32 packetBytes);
s32 sdf2DGetPrimPacketSize(s32 registerCount, s32 loopCount);
SdfDrawPacket *sdf2DMakePrimPacketHeader(SdfDrawPacket *packet, s32 primitiveFlags, s32 registerCount, s64 registerList, s32 loopCount);
SdfDrawPacket *sdf2DAllocSprite(s32 loopCount);
void sdf3DMakeViewportPacket(sdf3DViewportPacket_t *packet, sdf3DScreenParam_t *node, void *transformMatrix);
void sdf3DSetCamera(sdf3DScreenParam_t *node, void *transformMatrix);
void sdf3DMakeFogPacket(sdf3DFogPacket_t *packet, sdf3DFogParam_t *projectionParams);
void sdf3DMakeDirectRefPacket(DmaPacketHeader *packet, u32 sourceAddress, s32 payloadBytes);
void sdf3DLinkInitPacket(sdfDrawTag_t *packetList, DmaPacketHeader *packet);
void sdf3DInitSpecularTex2(void);
void sdf3DInit(void);
void sdf3DPrimLinkResetTm(sdfDrawTag_t *packetList,
                             s32 (*allocatePacket)(s32));
void sdf3DPrimSetTmPacket(sdf3DTmPacketData_t *lightingPacket);
void sdf3DPrimLinkTm(sdfDrawTag_t *packetList,
                           s32 (*allocatePacket)(s32));
void sdf3DPrimLinkMat(sdfDrawTag_t *packetList, sdfMat_t *asset,
                              s32 (*allocatePacket)(s32));
u32 *sdf3DPrimMakePoly(SdfPrimitiveRequest *request);
void sdfBillBoardMakeTMPacket(u8 *packet, const f32 *matrix);
void sdfBillBoardMake1(u8 *packet, const f32 *position, u32 color, const void *uv,
                                   const f32 *quad);
u32 *sdfBillBoardMakeN(const u128 *positions, const void *byteAttributes, const void *halfAttributes, const void *wordAttributes, s32 vertexCount, void *(*allocatePacket)(s32));
void sdfMultiBillBoardMake1(u8 *packet, const f32 *position, u32 primaryColor, u32 secondaryColor,
                   const void *primaryUv, const void *secondaryUv, const f32 *quad);
u32 sdfMultiBillBoardGetPacketSize(s32 count);
u32 *sdfMultiBillBoardMakeN(u128 *positions, void *attributes, void *halfAttributes, void *wordAttributes, s32 count, void *(*alloc)(s32));
u32 sdfBillBoardVcMakeN(const u128 *positions, const void *attributes,
                  const void *halfAttributes, const void *wordAttributes,
                  s32 count, void *(*alloc)(s32));
void sdfMultiBillBoardVcMake1(u8 *packet, const f32 *position, const void *colors, const void *primaryUv,
                   const void *secondaryUv, const void *quad);
u32 sdfMultiBillBoardVcGetPacketSize(s32 count);
u32 sdfMultiBillBoardVcMakeN(u128 *positions, void *byteAttributes, void *halfAttributes, void *wordAttributes, s32 vertexCount, void *(*allocatePacket)(s32));
void sdfPadControl(SdfPadEntry *entry);
void sdfPadManager(void);
void sdfPadSetAct0(s32 padIndex, u16 strength);
void sdfPadSetAct1(s32 padIndex, u8 strength);
void sdfPadInitManager(void);

void LoadMatrix(const void *matrix) {
    SDF_VU0_LOADMATRIX(matrix);
}

void LoadMatrix_B(const void *matrix) {
    SDF_VU0_LOADMATRIX_B(matrix);
}

void StoreMatrix(void *matrix) {
    SDF_VU0_STOREMATRIX(matrix);
}

void StoreMatrix_B(void *matrix) {
    SDF_VU0_STOREMATRIX_B(matrix);
}

void CopyMatrixAtoB(void) {
    SDF_VU0_COPYMATRIX_A_TO_B();
}

void CopyMatrixAtoC(void) {
    SDF_VU0_COPYMATRIX_A_TO_C();
}

void CopyMatrixBtoA(void) {
    SDF_VU0_COPYMATRIX_B_TO_A();
}

void CopyMatrixBtoC(void) {
    SDF_VU0_COPYMATRIX_B_TO_C();
}

void CopyMatrixCtoA(void) {
    SDF_VU0_COPYMATRIX_C_TO_A();
}

void CopyMatrixCtoB(void) {
    SDF_VU0_COPYMATRIX_C_TO_B();
}

void LoadIdentity(void) {
    SDF_VU0_LOADIDENTITY();
}

void LoadIdentity_B(void) {
    SDF_VU0_LOADIDENTITY_B();
}

/* vu0 routine: store the unit matrix built from vf0 by MMI lane shuffles */
void StoreIdentity(f32 *matrix) {
    SDF_VU0_STOREIDENTITY(matrix);
}

/* libvu0: sceVu0TransposeMatrix, register form (vf28-vf31 in and out) */
void DiagMatrix(void) {
    SDF_VU0_TRANSMATRIX();
}

/* vu0 routine: rigid inverse of vf28-vf31 (transpose the 3x3, translation = -(R^T * t)) */
void ReverseMatrix(void) {
    VU0_MATRIX4_INVERT_RIGID();
}

/* vu0 routine: inverse of vf28-vf31 with per-axis scale removed (transpose, rows / |row|^2, translation = -(R^T * t)) */
void InversMatrix(void) {
    VU0_MATRIX4_INVERT_SCALED();
}

void SetRotateMatrixX(f32 angle) {
    volatile f32 m[4][4] __attribute__((aligned(16)));
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    SDF_VU0_STOREIDENTITY((f32 *)m);
    m[1][1] = c;
    m[1][2] = s;
    m[2][1] = -s;
    m[2][2] = c;
    SDF_VU0_LOADMATRIX((f32 *)m);
}

void SetRotateMatrixY(f32 angle) {
    volatile f32 m[4][4] __attribute__((aligned(16)));
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    SDF_VU0_STOREIDENTITY((f32 *)m);
    m[0][0] = c;
    m[0][2] = -s;
    m[2][0] = s;
    m[2][2] = c;
    SDF_VU0_LOADMATRIX((f32 *)m);
}

void SetRotateMatrixZ(f32 angle) {
    volatile f32 m[4][4] __attribute__((aligned(16)));
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    SDF_VU0_STOREIDENTITY((f32 *)m);
    m[0][0] = c;
    m[0][1] = s;
    m[1][0] = -s;
    m[1][1] = c;
    SDF_VU0_LOADMATRIX((f32 *)m);
}

void StoreRotateMatrixAxis(f32 angle, const sdfVector3_t *axis, sdfMatrix4_t *matrix) {
    f32 normalizedAxis[4];
    f32 x;
    f32 y;
    f32 z;
    f32 cosine = cosf(angle);
    f32 sine = sinf(angle);

    SDF_VU0_LOADVECTOR(axis);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(normalizedAxis);

    x = normalizedAxis[0];
    y = normalizedAxis[1];
    z = normalizedAxis[2];

    matrix->m[0] = x * x + (1.0f - x * x) * cosine;
    matrix->m[1] = x * y * (1.0f - cosine) + z * sine;
    matrix->m[2] = x * z * (1.0f - cosine) - y * sine;
    matrix->m[3] = 0.0f;
    matrix->m[4] = x * y * (1.0f - cosine) - z * sine;
    matrix->m[5] = y * y + (1.0f - y * y) * cosine;
    matrix->m[6] = y * z * (1.0f - cosine) + x * sine;
    matrix->m[7] = 0.0f;
    matrix->m[8] = x * z * (1.0f - cosine) + y * sine;
    matrix->m[9] = y * z * (1.0f - cosine) - x * sine;
    matrix->m[10] = z * z + (1.0f - z * z) * cosine;
    matrix->m[11] = 0.0f;
    SDF_VU0_STOREIDENTITYVECTOR(&matrix->m[12]);
}

void SetRotateMatrixAxis(f32 angle, const sdfVector3_t* axis)
{
    sdfMatrix4_t matrix;
    StoreRotateMatrixAxis(angle, axis, &matrix);
    SDF_VU0_LOADMATRIX(&matrix);
}

void SetRotateMatrixX_B(f32 angle) {
    volatile f32 m[4][4] __attribute__((aligned(16)));
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    SDF_VU0_STOREIDENTITY((f32 *)m);
    m[1][1] = c;
    m[1][2] = s;
    m[2][1] = -s;
    m[2][2] = c;
    SDF_VU0_LOADMATRIX_B((f32 *)m);
}

void SetRotateMatrixY_B(f32 angle) {
    volatile f32 m[4][4] __attribute__((aligned(16)));
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    SDF_VU0_STOREIDENTITY((f32 *)m);
    m[0][0] = c;
    m[0][2] = -s;
    m[2][0] = s;
    m[2][2] = c;
    SDF_VU0_LOADMATRIX_B((f32 *)m);
}

void SetRotateMatrixZ_B(f32 angle) {
    volatile f32 m[4][4] __attribute__((aligned(16)));
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    SDF_VU0_STOREIDENTITY((f32 *)m);
    m[0][0] = c;
    m[0][1] = s;
    m[1][0] = -s;
    m[1][1] = c;
    SDF_VU0_LOADMATRIX_B((f32 *)m);
}

void SetRotateMatrixAxis_B(const sdfVector3_t* axis, f32 angle)
{
    sdfMatrix4_t matrix;
    StoreRotateMatrixAxis(angle, axis, &matrix);
    SDF_VU0_LOADMATRIX_B(&matrix);
}

/* vu0 routine: rotate the vf29/vf30 pair by angle (vf29 = vf29*c + vf30*s, vf30 = vf29*s - vf30*c) */
void RotateMatrixX(f32 angle)
{
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    VU0_ROTATE_BASIS_X(c, s);
}

/* vu0 routine: rotate the vf28/vf30 pair by angle (vf28 = vf30*s - vf28*c, vf30 = vf30*c + vf28*s) */
void RotateMatrixY(f32 angle)
{
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    VU0_ROTATE_BASIS_Y(c, s);
}

/* vu0 routine: rotate the vf28/vf29 pair by angle (vf28 = vf29*s + vf28*c, vf29 = vf28*s - vf29*c) */
void RotateMatrixZ(f32 angle)
{
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    VU0_ROTATE_BASIS_Z(c, s);
}

void SetRotateMatrixXYZ(const sdfVector3_t *rot)
{
    SetRotateMatrixX(rot->x);
    SetRotateMatrixY_B(rot->y);
    MultMatrixRevNoArg();
    SetRotateMatrixZ_B(rot->z);
    MultMatrixRevNoArg();
}

/* vu0 routine: vf28-vf31 = vf24-vf27 * vf28-vf31 (4x4 product) */
void MultMatrixNoArg(void)
{
    VU0_MATRIX4_MUL_BANK_B_RIGHT();
}

/* vu0 routine: vf28-vf31 = vf28-vf31 * vf24-vf27 (4x4 product) */
void MultMatrixRevNoArg(void)
{
        VU0_MATRIX4_MUL_BANK_B_LEFT();
}

/* vu0 routine: vf28-vf31 = vf20-vf23 * vf28-vf31 (4x4 product) */
void MultMatrixNoArg_C(void)
{
    VU0_MATRIX4_MUL_BANK_C_RIGHT();
}

/* vu0 routine: vf28-vf31 = vf28-vf31 * vf20-vf23 (4x4 product) */
void MultMatrixRevNoArg_C(void)
{
    VU0_MATRIX4_MUL_BANK_C_LEFT();
}

void MultMatrix(void *matrix) {
    SDF_VU0_LOADMATRIX_B(matrix);
    MultMatrixNoArg();
}

void MultMatrixRev(const void *matrix) {
    SDF_VU0_LOADMATRIX_B(matrix);
    MultMatrixRevNoArg();
}

void ApplyMatrix(void *dst, const void *src) {
    SDF_VU0_LOADVECTOR(src);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(dst);
}

f32 InnerProduct(const void *a, const void *b) {
    f32 product;

    SDF_VU0_LOADVECTOR(a);
    SDF_VU0_LOADVECTOR_B(b);
    SDF_VU0_INNERPRODUCT(product);
    return product;
}

void OuterProduct(void *dst, const void *a, const void *b) {
    SDF_VU0_LOADVECTOR(a);
    SDF_VU0_LOADVECTOR_B(b);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_STOREVECTOR(dst);
}

/* vu0 routine: look-at basis in vf28-vf31 (forward, right, up, eye), then its rigid inverse */
void LookAtMatrix(const void *directionPoint, const void *basisOrigin, const void *up) {
    SDF_VU0_LOADVECTOR(basisOrigin);
    SDF_VU0_SET_ROW3();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(directionPoint);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SET_ROW2();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(up);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SET_ROW0();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_GET_ROW2();
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_SET_ROW1();
    ReverseMatrix();
}

void sdfVu1Init(void) {
}

void corePktSetupPointBuff(VuWork *work, s32 ringPosition) {
    void *ringEnd = (u8 *)work + 0x78;
    if (ringPosition < 0x80) {
        work->dmaCountA = 0x80 - ringPosition;
        work->dmaAddrB = corePktPointBuff;
        work->dmaAddrA = ringPosition * 96 + 0x70001000;
        work->dmaCountB = 0x2000;
        work->dmaBase = 0x70001000;
    } else if (ringPosition == 0x80) {
        work->dmaBase = 0x70001000;
        work->dmaAddrA = corePktPointBuff;
        work->dmaCountA = 0x2000;
        work->dmaAddrB = 0;
        work->dmaCountB = 0;
    } else {
        work->dmaCountA = 0x80;
        work->dmaBase = corePktPointBuff;
        work->dmaAddrA = 0x70001000;
        work->dmaAddrB = corePktPointBuff + ringPosition * 96;
        work->dmaCountB = 0x2000 - ringPosition;
    }
    work->dmaEnd = ringEnd;
}

void corePktInitCb(VuWork *work, u16 *parameterWords, u32 selectionMask) {
    u8 *payload = (u8 *)(parameterWords + 4);
    u16 ringPosition;
    u16 parameterFlags;
    u16 nextParameter;
    u16 selectedFlags;
    work->param0 = parameterWords[0];
    ringPosition = parameterWords[1];
    work->param1 = ringPosition;
    parameterFlags = parameterWords[2];
    nextParameter = parameterWords[3];
    selectedFlags = parameterFlags & selectionMask;
    work->flags = parameterFlags;
    work->nextParam = nextParameter;
    work->selectedFlags = selectedFlags;
    corePktPrimBase = selectedFlags & 0x78;
    work->payload = payload;
    work->packetStart = 0;
    corePktSetupPointBuff(work, ringPosition);
}

/* vu0 routine: rotate the basis at vectors+0x40 into vf24-vf26 and cache the original vectors. */
void corePktLoadWorldLightMtx(void *vectors) {
    void *m = (void *)corePktCurrLight;
    VU0_ROTATE_BASIS_AND_CACHE(vectors, m, corePktWorldLightMtxCache);
}

/* vu0 routine: modulate and clamp packed colors against the reference rows. */
void corePktMakeFaceDivParm(SdfVuColorTransform *result, u32 reference, u32 packedColor,
                   u32 firstColor, u32 secondColor,
                   f32 blend, f32 scale, f32 y) {
    f32 scaledY;

    if (scale < 1.0f) {
        scale = 1.0f;
    }
    scaledY = y * scale;
    result->scale = scale;
    result->scaledY = scaledY;
    VU0_BUILD_PACKED_COLOR_TRANSFORM(result, reference, packedColor,
                                     firstColor, secondColor, blend);
}

void corePktSetFaceDivParm(SdfVuColorTransform *out, sdfMatPacket_t *work, u32 packedColor, f32 deltaX, f32 deltaY) {
    corePktMakeFaceDivParm(out, corePktCurrLight, packedColor,
                  work->unk08, work->unk04,
                  work->unk1C,
                  work->x + deltaX,
                  work->y + deltaY);
}

INCLUDE_ASM(const s32, "code_002B5CD0", corePktSkinPointsFirst);

INCLUDE_ASM(const s32, "code_002B5CD0", corePktSkinPointsAccum);

INCLUDE_ASM(const s32, "code_002B5CD0", corePktSkinVertFirst);

INCLUDE_ASM(const s32, "code_002B5CD0", corePktSkinVertAccum);

INCLUDE_ASM(const s32, "code_002B5CD0", corePktLoadVerts);

INCLUDE_ASM(const s32, "code_002B5CD0", corePktLoadVertsNorms);

INCLUDE_ASM(const s32, "code_002B5CD0", corePktMorphVertsNorms);

INCLUDE_ASM(const s32, "code_002B5CD0", corePktMorphVerts);

INCLUDE_ASM(const s32, "code_002B5CD0", corePktNormalizeNorms);

INCLUDE_ASM(const s32, "code_002B5CD0", corePktXfmLightClip);

INCLUDE_ASM(const s32, "code_002B5CD0", corePktXfmLightClipPB);

INCLUDE_ASM(const s32, "code_002B5CD0", corePktXfmLightClipUV);

INCLUDE_ASM(const s32, "code_002B5CD0", corePktXfmLightClipUVPB);

INCLUDE_ASM(const s32, "code_002B5CD0", corePktRelightBackFaces);

INCLUDE_ASM(const s32, "code_002B5CD0", corePktXfmUV);

/* vu0 routine: lerp of the XY pair at +0x30 by the weight at node + 0x40 */
void corePktLerpUVChain(VuBlendNode *node) {
    while (node != NULL) {
        void *firstSourceRow = node->sourceA;
        void *secondSourceRow = node->sourceB;
        VU0_BLEND_NODE_XY(node, firstSourceRow, secondSourceRow);
        node = node->next;
    }
}

/* vu0 routine: lerp of two source rows (+0x20, +0x30) by the weight at node + 0x40 */
void corePktLerpUVColorChain(VuBlendNode *node) {
    while (node != NULL) {
        void *firstSourceRow = node->sourceA;
        void *secondSourceRow = node->sourceB;
        VU0_BLEND_NODE_VECTORS(node, firstSourceRow, secondSourceRow);
        node = node->next;
    }
}

INCLUDE_ASM(const s32, "code_002B5CD0", corePktApplyFaceDivColor);

void corePktSetConstColor(VuWork *work, u32 color) {
    f32 colorScale = 1.0f / 128.0f;
    VuBlendNode *node;
    s32 remaining;
    u128 scaledColor;

    EE_MMI_RGBA_MULTIPLY_VECTOR(scaledColor, work->packedColor, color, colorScale);

    remaining = work->param1;
    node = (VuBlendNode *)work->dmaBase;
    if (remaining != 0) {
        do {
            remaining--;
            *(u128 *)node->color = scaledColor;
            node++;
        } while (remaining != 0);
    }

    node = work->blendList;
    while (node != NULL) {
        VuBlendNode *source = (VuBlendNode *)node->sourceA;
        SDF_VU0_COPYVECTOR_MEMOPS(node->color, source->color);
        node = node->next;
    }
}

/* vu0 routine: interpolate a clip vertex between two source rows, COP2 macro math only */
void corePktMakeEdgePoint(VuBlendNode *out, VuBlendNode *first, VuBlendNode *second) {
    out->sourceA = first;
    out->sourceB = second;
    VU0_INTERPOLATE_CLIP_VERTEX(out, first, second);
}

/* vu0 routine: native clip facing and ordinary triangle fans. */
void corePktClipFace(VuWork *work) {
    s32 triangleCount;
    s32 i;
    u16 *triangles;
    VuBlendNode *nodes;
    s32 rejected;
    s32 acceptAll;
    u128 *(*out)[3];
    f32 scale;

    out = corePktFaceList;
    triangleCount = (s16)work->param0;
    rejected = 0;
    scale = work->depth;
    acceptAll = corePktSoftenBias;
    triangles = (u16 *)work->strip;
    nodes = (VuBlendNode *)work->dmaBase;
    for (i = 0; i != triangleCount; i++) {
        u16 *triangle = triangles;
        VuBlendNode *a = nodes + triangle[0];
        VuBlendNode *b = nodes + triangle[1];
        VuBlendNode *c = nodes + triangle[2];
        u32 bRegion;
        u32 aRegion;
        u32 cRegion;
        VuBlendNode *d, *e, *f, *g;
        f32 facing;
        u32 clipA = a->clipFlags;
        u32 clipB = b->clipFlags;
        u32 clipC = c->clipFlags;

        triangles += 3;
        if ((clipA & clipB & clipC & 0x3F) != 0 ||
            ((clipA | clipB | clipC) & 0x30) != 0) {
            continue;
        }
        VU0_TRIANGLE_FACING(facing, a->position, b->position, c->position);
        facing *= scale;
        if (facing < 0.0f) {
            corePktBackFaceList[rejected++] = i;
            continue;
        }
        aRegion = a->region;
        bRegion = b->region;
        cRegion = c->region;

        if (acceptAll || (aRegion == bRegion && aRegion == cRegion)) {
            out[0][0] = (u128 *)a;
            out[0][1] = (u128 *)b;
            out[0][2] = (u128 *)c;
            out++;
            continue;
        }
        for (;;) {
            VuBlendNode *firstVertex;
            u32 firstCode;

            if (bRegion == cRegion) {
                d = vuAllocNode(work);
                e = vuAllocNode(work);
                if (aRegion == 0) {
                    corePktMakeEdgePoint(d, b, a);
                    corePktMakeEdgePoint(e, c, a);
                    out[0][0] = (u128 *)a;
                    out[0][1] = (u128 *)d;
                    out[0][2] = (u128 *)e;
                    out[1][0] = (u128 *)e;
                    out[1][1] = (u128 *)d;
                    out[1][2] = (u128 *)b;
                    out[2][0] = (u128 *)e;
                    out[2][1] = (u128 *)b;
                    out[2][2] = (u128 *)c;
                    out += 3;
                    break;
                }
                corePktMakeEdgePoint(d, a, b);
                corePktMakeEdgePoint(e, a, c);
                out[0][0] = (u128 *)a;
                out[0][1] = (u128 *)d;
                out[0][2] = (u128 *)e;
                out++;
                if (bRegion == 0) {
                    out[0][0] = (u128 *)b;
                    out[0][1] = (u128 *)e;
                    out[0][2] = (u128 *)d;
                    out[1][0] = (u128 *)b;
                    out[1][1] = (u128 *)c;
                    out[1][2] = (u128 *)e;
                    out += 2;
                } else {
                    f = vuAllocNode(work);
                    g = vuAllocNode(work);
                    corePktMakeEdgePoint(f, b, a);
                    corePktMakeEdgePoint(g, c, a);
                    out[0][0] = (u128 *)e;
                    out[0][1] = (u128 *)d;
                    out[0][2] = (u128 *)f;
                    out[1][0] = (u128 *)e;
                    out[1][1] = (u128 *)f;
                    out[1][2] = (u128 *)g;
                    out[2][0] = (u128 *)g;
                    out[2][1] = (u128 *)f;
                    out[2][2] = (u128 *)b;
                    out[3][0] = (u128 *)g;
                    out[3][1] = (u128 *)b;
                    out[3][2] = (u128 *)c;
                    out += 4;
                }
                break;
            }
            if (aRegion == 0 && (bRegion ^ cRegion) == 3) {
                d = vuAllocNode(work);
                e = vuAllocNode(work);
                f = vuAllocNode(work);
                g = vuAllocNode(work);
                corePktMakeEdgePoint(d, b, a);
                corePktMakeEdgePoint(e, c, a);
                corePktMakeEdgePoint(f, b, c);
                corePktMakeEdgePoint(g, c, b);
                out[0][0] = (u128 *)a;
                out[0][1] = (u128 *)d;
                out[0][2] = (u128 *)e;
                out[1][0] = (u128 *)d;
                out[1][1] = (u128 *)b;
                out[1][2] = (u128 *)f;
                out[2][0] = (u128 *)e;
                out[2][1] = (u128 *)d;
                out[2][2] = (u128 *)f;
                out[3][0] = (u128 *)e;
                out[3][1] = (u128 *)f;
                out[3][2] = (u128 *)g;
                out[4][0] = (u128 *)e;
                out[4][1] = (u128 *)g;
                out[4][2] = (u128 *)c;
                out += 5;
                break;
            }
            firstVertex = a;
            firstCode = aRegion;
            a = b;
            aRegion = bRegion;
            b = c;
            bRegion = cRegion;
            c = firstVertex;
            cRegion = firstCode;
        }
    }
    work->selectedTriangleCount = rejected;
    work->nodeCount = out - corePktFaceList;
    *work->dmaEnd = NULL;
}

/* vu0 routine: native clip-plane setup and ordinary triangle fans. */
void corePktBuildFaceList(VuWork *work) {
    s32 triangleCount;
    u16 *triangles;
    VuBlendNode *nodes;
    s32 acceptAll;
    u128 *(*out)[3];
    f32 scale = work->depth;
    f32 clipBias = corePktZBias;
    const f32 *clipPlane = sdf3DEyeOffset;
    const u8 *projection = sdf3DScreenParam;

    VU0_SET_CLIP_PLANES(clipBias, clipPlane, projection);
    if (work->selectedFlags & 0x4000) {
        corePktClipFace(work);
    }
    triangleCount = (s16)work->param0;
    acceptAll = corePktSoftenBias;
    out = corePktFaceList;
    triangles = (u16 *)work->strip;
    nodes = (VuBlendNode *)work->dmaBase;
    for (; triangleCount != 0; triangleCount--) {
        u16 *triangle = triangles;
        VuBlendNode *a = nodes + triangle[0];
        VuBlendNode *b = nodes + triangle[1];
        VuBlendNode *c = nodes + triangle[2];
        u32 bRegion;
        u32 aRegion;
        u32 cRegion;
        VuBlendNode *d, *e, *f, *g;
        f32 facing;
        u32 clipA = a->clipFlags;
        u32 clipB = b->clipFlags;
        u32 clipC = c->clipFlags;

        triangles += 3;
        if ((clipA & clipB & clipC & 0x3F) != 0 ||
            ((clipA | clipB | clipC) & 0x30) != 0) {
            continue;
        }
        VU0_TRIANGLE_FACING(facing, a->position, b->position, c->position);
        facing *= scale;
        if (facing < 0.0f) {
            continue;
        }
        aRegion = a->region;
        bRegion = b->region;
        cRegion = c->region;

        if (acceptAll || (aRegion == bRegion && aRegion == cRegion)) {
            out[0][0] = (u128 *)a;
            out[0][1] = (u128 *)b;
            out[0][2] = (u128 *)c;
            out++;
            continue;
        }
        for (;;) {
            VuBlendNode *firstVertex;
            u32 firstCode;

            if (bRegion == cRegion) {
                d = vuAllocNode(work);
                e = vuAllocNode(work);
                if (aRegion == 0) {
                    corePktMakeEdgePoint(d, b, a);
                    corePktMakeEdgePoint(e, c, a);
                    out[0][0] = (u128 *)a;
                    out[0][1] = (u128 *)d;
                    out[0][2] = (u128 *)e;
                    out[1][0] = (u128 *)e;
                    out[1][1] = (u128 *)d;
                    out[1][2] = (u128 *)b;
                    out[2][0] = (u128 *)e;
                    out[2][1] = (u128 *)b;
                    out[2][2] = (u128 *)c;
                    out += 3;
                    break;
                }
                corePktMakeEdgePoint(d, a, b);
                corePktMakeEdgePoint(e, a, c);
                out[0][0] = (u128 *)a;
                out[0][1] = (u128 *)d;
                out[0][2] = (u128 *)e;
                out++;
                if (bRegion == 0) {
                    out[0][0] = (u128 *)b;
                    out[0][1] = (u128 *)e;
                    out[0][2] = (u128 *)d;
                    out[1][0] = (u128 *)b;
                    out[1][1] = (u128 *)c;
                    out[1][2] = (u128 *)e;
                    out += 2;
                } else {
                    f = vuAllocNode(work);
                    g = vuAllocNode(work);
                    corePktMakeEdgePoint(f, b, a);
                    corePktMakeEdgePoint(g, c, a);
                    out[0][0] = (u128 *)e;
                    out[0][1] = (u128 *)d;
                    out[0][2] = (u128 *)f;
                    out[1][0] = (u128 *)e;
                    out[1][1] = (u128 *)f;
                    out[1][2] = (u128 *)g;
                    out[2][0] = (u128 *)g;
                    out[2][1] = (u128 *)f;
                    out[2][2] = (u128 *)b;
                    out[3][0] = (u128 *)g;
                    out[3][1] = (u128 *)b;
                    out[3][2] = (u128 *)c;
                    out += 4;
                }
                break;
            }
            if (aRegion == 0 && (bRegion ^ cRegion) == 3) {
                d = vuAllocNode(work);
                e = vuAllocNode(work);
                f = vuAllocNode(work);
                g = vuAllocNode(work);
                corePktMakeEdgePoint(d, b, a);
                corePktMakeEdgePoint(e, c, a);
                corePktMakeEdgePoint(f, b, c);
                corePktMakeEdgePoint(g, c, b);
                out[0][0] = (u128 *)a;
                out[0][1] = (u128 *)d;
                out[0][2] = (u128 *)e;
                out[1][0] = (u128 *)d;
                out[1][1] = (u128 *)b;
                out[1][2] = (u128 *)f;
                out[2][0] = (u128 *)e;
                out[2][1] = (u128 *)d;
                out[2][2] = (u128 *)f;
                out[3][0] = (u128 *)e;
                out[3][1] = (u128 *)f;
                out[3][2] = (u128 *)g;
                out[4][0] = (u128 *)e;
                out[4][1] = (u128 *)g;
                out[4][2] = (u128 *)c;
                out += 5;
                break;
            }
            firstVertex = a;
            firstCode = aRegion;
            a = b;
            aRegion = bRegion;
            b = c;
            bRegion = cRegion;
            c = firstVertex;
            cRegion = firstCode;
        }
    }
    work->nodeCount = out - corePktFaceList;
    *work->dmaEnd = NULL;
}

void corePktBuildBackFaceList(VuWork *work) {
    /* DDS1 node region uses the tail byte at +0x50. */
    f32 scale = corePktZBias;
    s32 remaining;
    u16 *triangles;
    u16 *indices;
    VuBlendNode *nodes;
    s32 acceptAll;
    u128 *(*out)[3];
    const f32 *clipPlane = sdf3DEyeOffset;
    const u8 *projection = sdf3DScreenParam;

    VU0_SET_CLIP_PLANES(scale, clipPlane, projection);
    acceptAll = corePktSoftenBias;
    remaining = ((VuIndexedTriangleCount *)work)->indexedTriangleCount;
    indices = corePktBackFaceList;
    triangles = (u16 *)work->strip;
    nodes = (VuBlendNode *)work->dmaBase;
    out = (u128 *(*)[3])corePktFaceList;
    if (remaining != 0) {
        do {
            u16 *triangle = triangles + *indices++ * 3;
            VuBlendNode *a = nodes + triangle[0];
            VuBlendNode *b = nodes + triangle[1];
            VuBlendNode *c = nodes + triangle[2];
            u32 bRegion = b->region;
            u32 aRegion = a->region;
            u32 cRegion = c->region;
            VuBlendNode *d, *e, *f, *g;
            if (!acceptAll && (aRegion != bRegion || aRegion != cRegion)) {
                goto classifyRegions;
            }
        out[0][0] = (u128 *)a;
        out[0][1] = (u128 *)b;
        out[0][2] = (u128 *)c;
        out += 1;
            goto advanceTriangle;
        appendInsidePair:
        out[0][0] = (u128 *)b;
        out[0][1] = (u128 *)e;
        out[0][2] = (u128 *)d;
        out[1][0] = (u128 *)b;
        out[1][1] = (u128 *)c;
        out[1][2] = (u128 *)e;
        out += 2;
            goto advanceTriangle;
        interpolateOutsideA:
        corePktMakeEdgePoint(d, a, b);
        corePktMakeEdgePoint(e, a, c);
        out[0][0] = (u128 *)a;
        out[0][1] = (u128 *)d;
        out[0][2] = (u128 *)e;
        out += 1;
            if (bRegion != 0) {
            f = vuAllocNode(work);
            g = vuAllocNode(work);
            corePktMakeEdgePoint(f, b, a);
            corePktMakeEdgePoint(g, c, a);
            out[0][0] = (u128 *)e;
            out[0][1] = (u128 *)d;
            out[0][2] = (u128 *)f;
            out[1][0] = (u128 *)e;
            out[1][1] = (u128 *)f;
            out[1][2] = (u128 *)g;
            out[2][0] = (u128 *)g;
            out[2][1] = (u128 *)f;
            out[2][2] = (u128 *)b;
            out[3][0] = (u128 *)g;
            out[3][1] = (u128 *)b;
            out[3][2] = (u128 *)c;
            out += 4;

            } else goto appendInsidePair;
            goto advanceTriangle;
        threeDifferentRegions: {
            if (aRegion != 0 || (bRegion ^ cRegion) != 3) goto rotateTriangle;
        d = vuAllocNode(work);
        e = vuAllocNode(work);
        f = vuAllocNode(work);
        g = vuAllocNode(work);
        corePktMakeEdgePoint(d, b, a);
        corePktMakeEdgePoint(e, c, a);
        corePktMakeEdgePoint(f, b, c);
        corePktMakeEdgePoint(g, c, b);
        out[0][0] = (u128 *)a;
        out[0][1] = (u128 *)d;
        out[0][2] = (u128 *)e;
        out[1][0] = (u128 *)d;
        out[1][1] = (u128 *)b;
        out[1][2] = (u128 *)f;
        out[2][0] = (u128 *)e;
        out[2][1] = (u128 *)d;
        out[2][2] = (u128 *)f;
        out[3][0] = (u128 *)e;
        out[3][1] = (u128 *)f;
        out[3][2] = (u128 *)g;
        out[4][0] = (u128 *)e;
        out[4][1] = (u128 *)g;
        out[4][2] = (u128 *)c;
        out += 5;
            goto advanceTriangle;
        }
        rotateTriangle: {
            VuBlendNode *oldA = a;
            u32 oldRegion = aRegion;
            a = b; b = c; c = oldA;
            aRegion = bRegion; bRegion = cRegion; cRegion = oldRegion;
        }
        classifyRegions:
            if (bRegion != cRegion) goto threeDifferentRegions;
        d = vuAllocNode(work);
        e = vuAllocNode(work);
            if (aRegion != 0) goto interpolateOutsideA;
        corePktMakeEdgePoint(d, b, a);
        corePktMakeEdgePoint(e, c, a);
        out[0][0] = (u128 *)a;
        out[0][1] = (u128 *)d;
        out[0][2] = (u128 *)e;
        out[1][0] = (u128 *)e;
        out[1][1] = (u128 *)d;
        out[1][2] = (u128 *)b;
        out[2][0] = (u128 *)e;
        out[2][1] = (u128 *)b;
        out[2][2] = (u128 *)c;
        out += 3;
        advanceTriangle:
            ;
        } while (--remaining != 0);
    }
    work->nodeCount = out - (u128 *(*)[3])corePktFaceList;
    *(VuBlendNode **)work->dmaEnd = NULL;
}

void corePktEmitTexTris(work)
    VuWork *work;
{
    s32 remaining = work->nodeCount;
    if (remaining != 0) {
        u128 *(*nodes)[3] = corePktFaceList;
        u32 *cursor = work->cursor;
        s32 first = 1;
        do {
            s32 chunk = (remaining <= 0x10) ? remaining : 0x10;
            remaining -= chunk;
            while (((u32)cursor & 0xC) != 4) {
                *cursor++ = 0;
            }
            if (first) {
                sdfMatPacket_t *node;
                *cursor++ = 0x6501C000;
                *cursor++ = 4;
                *cursor++ = ((chunk * 9 + 5) << 16) | 0x6C00C001;
                node = work->node;
                first = 0;
                *(u64 *)cursor = 0x1000000000000003ULL;
                cursor += 2;
                *(u64 *)cursor = 0xE;
                cursor += 2;
                *(u64 *)cursor = node->primaryTextureState.sampling;
                cursor += 2;
                *(u64 *)cursor = 0x14;
                cursor += 2;
                *(u64 *)cursor = node->primaryTextureState.texture;
                cursor += 2;
                *(u64 *)cursor = 6;
                cursor += 2;
                *(u64 *)cursor = node->primaryTextureState.clamp;
                cursor += 2;
                *(u64 *)cursor = 8;
                cursor += 2;
            } else {
                *cursor++ = 0x6501C000;
                *cursor++ = 0;
                *cursor++ = ((chunk * 9 + 1) << 16) | 0x6C00C001;
            }
            *(u64 *)cursor = ((u64)(corePktPrimBase | 3) << 47) | (chunk * 3) | 0x3000400000008000ULL;
            cursor += 2;
            *(u64 *)cursor = 0x412;
            cursor += 2;
            do {
                u128 *a = (*nodes)[0];
                u128 *b = (*nodes)[1];
                u128 *c = (*nodes)[2];
                nodes++;
                ((u128 *)cursor)[0] = a[0];
                ((u128 *)cursor)[1] = a[3];
                ((u128 *)cursor)[2] = a[2];
                ((u128 *)cursor)[3] = b[0];
                ((u128 *)cursor)[4] = b[3];
                ((u128 *)cursor)[5] = b[2];
                ((u128 *)cursor)[6] = c[0];
                ((u128 *)cursor)[7] = c[3];
                ((u128 *)cursor)[8] = c[2];
                cursor += 0x24;
            } while (--chunk != 0);
            *cursor++ = 0x14000004;
        } while (remaining != 0);
        work->cursor = cursor;
    }
}

void corePktEmitTexTrisCtx2(VuWork *work, u64 samplingState, u64 textureState, u64 clampState, u64 alphaState, s32 clearPrimitiveFlags) {
    s32 remainingTriangles = work->nodeCount;
    if (remainingTriangles != 0) {
        u128 *(*triangleRefs)[3] = corePktFaceList;
        u32 *packetCursor = work->cursor;
        s32 firstBatch = 1;
        do {
            s32 batchTriangles = (remainingTriangles <= 0x10) ? remainingTriangles : 0x10;
            remainingTriangles -= batchTriangles;
            while (((u32)packetCursor & 0xC) != 4) {
                *packetCursor++ = 0;
            }
            if (firstBatch) {
                *packetCursor++ = 0x6501C000;
                *packetCursor++ = 6;
                *packetCursor++ = ((batchTriangles * 9 + 7) << 16) | 0x6C00C001;
                firstBatch = 0;
                *(u64 *)packetCursor = 0x1000000000000005ULL;
                packetCursor += 2;
                *(u64 *)packetCursor = SDF_GIF_REGISTER_AD;
                packetCursor += 2;
                *(u64 *)packetCursor = samplingState;
                packetCursor += 2;
                *(u64 *)packetCursor = 0x15;
                packetCursor += 2;
                *(u64 *)packetCursor = textureState;
                packetCursor += 2;
                *(u64 *)packetCursor = 7;
                packetCursor += 2;
                *(u64 *)packetCursor = clampState;
                packetCursor += 2;
                *(u64 *)packetCursor = 9;
                packetCursor += 2;
                *(u64 *)packetCursor = 0x51801;
                packetCursor += 2;
                *(u64 *)packetCursor = 0x48;
                packetCursor += 2;
                *(u64 *)packetCursor = alphaState;
                packetCursor += 2;
                *(u64 *)packetCursor = 0x43;
                packetCursor += 2;
            } else {
                *packetCursor++ = 0x6501C000;
                *packetCursor++ = 0;
                *packetCursor++ = ((batchTriangles * 9 + 1) << 16) | 0x6C00C001;
            }
            *(u64 *)packetCursor = ((u64)((corePktPrimBase & ~clearPrimitiveFlags) | 0x203) << SDF_GIF_PRIM_SHIFT) | (batchTriangles * 3) | 0x3000400000008000ULL;
            packetCursor += 2;
            *(u64 *)packetCursor = 0x412;
            packetCursor += 2;
            do {
                u128 *vertexA = (*triangleRefs)[0];
                u128 *vertexB = (*triangleRefs)[1];
                u128 *vertexC = (*triangleRefs)[2];
                triangleRefs++;
                ((u128 *)packetCursor)[0] = vertexA[0];
                ((u128 *)packetCursor)[1] = vertexA[3];
                ((u128 *)packetCursor)[2] = vertexA[2];
                ((u128 *)packetCursor)[3] = vertexB[0];
                ((u128 *)packetCursor)[4] = vertexB[3];
                ((u128 *)packetCursor)[5] = vertexB[2];
                ((u128 *)packetCursor)[6] = vertexC[0];
                ((u128 *)packetCursor)[7] = vertexC[3];
                ((u128 *)packetCursor)[8] = vertexC[2];
                packetCursor += 0x24;
            } while (--batchTriangles != 0);
            *packetCursor++ = 0x14000004;
        } while (remainingTriangles != 0);
        work->cursor = packetCursor;
    }
}

void corePktEmitTris(work)
    VuWork *work;
{
    s32 remainingTriangles = work->nodeCount;
    if (remainingTriangles != 0) {
        u128 *(*triangleRefs)[3] = corePktFaceList;
        u32 *packetCursor = work->cursor;
        do {
            s32 batchTriangles = (remainingTriangles <= 0x18) ? remainingTriangles : 0x18;
            remainingTriangles -= batchTriangles;
            while (((u32)packetCursor & 0xC) != 4) {
                *packetCursor++ = 0;
            }
            *packetCursor++ = 0x6501C000;
            *packetCursor++ = 0x10000;
            *packetCursor++ = ((batchTriangles * 6 + 1) << 16) | 0x6C00C001;
            *(u64 *)packetCursor = (batchTriangles * 3) | ((u64)(corePktPrimBase | 3) << SDF_GIF_PRIM_SHIFT) | 0x2000400000008000ULL;
            packetCursor += 2;
            *(u64 *)packetCursor = 0x41;
            packetCursor += 2;
            do {
                u128 *vertexA = (*triangleRefs)[0];
                u128 *vertexB = (*triangleRefs)[1];
                u128 *vertexC = (*triangleRefs)[2];
                triangleRefs++;
                ((u128 *)packetCursor)[0] = vertexA[0];
                ((u128 *)packetCursor)[1] = vertexA[2];
                ((u128 *)packetCursor)[2] = vertexB[0];
                ((u128 *)packetCursor)[3] = vertexB[2];
                ((u128 *)packetCursor)[4] = vertexC[0];
                ((u128 *)packetCursor)[5] = vertexC[2];
                packetCursor += 0x18;
            } while (--batchTriangles != 0);
            *packetCursor++ = 0x14000004;
        } while (remainingTriangles != 0);
        work->cursor = packetCursor;
    }
}

void corePktEmitBase(VuWork *work) {
    if ((work->selectedFlags & SDF_VU_TEXTURED_FLAG) != 0) {
        corePktEmitTexTris();
        return;
    }
    corePktEmitTris();
}

void corePktEmitMultiTexPass(VuWork *work) {
    sdfMatPacket_t *node;
    u32 mode;
    u32 secondaryColor;
    u32 primaryStateWordSecond;

    corePktXfmUV((void *)work->dmaBase, work->param1, work->secondCoordinates,
                  (u8 *)work->node + 0x80);
    corePktLerpUVChain(work->blendList);
    node = work->node;
    mode = node->secondaryMode;
    secondaryColor = node->secondaryColor;
    primaryStateWordSecond = node->unk08;
    switch (mode) {
        case 0:
        case 1:
        case 2:
            if (primaryStateWordSecond != secondaryColor) {
                u128 parameters[3];

                corePktMakeFaceDivParm((SdfVuColorTransform *)parameters, corePktCurrLight, work->packedColor,
                              secondaryColor, node->unk04, node->unk1C,
                              node->x + work->offsetX, node->y + work->offsetY);
                corePktApplyFaceDivColor(work, parameters);
            }
            break;
        case 3:
            corePktSetConstColor(work, secondaryColor);
            break;
    }
    corePktEmitTexTrisCtx2(work, node->secondaryTextureState.sampling,
                                  node->secondaryTextureState.texture,
                                  node->secondaryTextureState.clamp, node->alphaState, 0);
}

INCLUDE_ASM(const s32, "code_002B5CD0", corePktEmitRefMapPass);

void corePktEmitAllPasses(VuWork *work) {
    u32 selectedFlags;

    corePktEmitBase(work);
    selectedFlags = work->selectedFlags;
    if ((selectedFlags & 0x1000) != 0) {
        corePktEmitMultiTexPass(work);
        selectedFlags = work->selectedFlags;
    }
    if ((selectedFlags & 1) != 0) {
        corePktEmitRefMapPass((u32)work);
        return;
    }
}

void corePktOpenAndEmit(VuWork *work) {
    u32 packetStart = sdfGrOpenPacketBuff();
    u32 alignedStart = (packetStart + 0x3F) & ~0x3F;
    u32 ringAddress = ((alignedStart + 0x40) & SDF_DMA_ADDRESS_MASK) | 0x30000000;
    work->packetStart = packetStart;
    work->header = alignedStart + 0x30;
    work->ringStart = (u32 *)ringAddress;
    work->cursor = (u32 *)ringAddress;
    if ((work->selectedFlags & 0x4000) != 0) {
        u8 savedVectors[0x30];
        VU0_STORE_VF24_VF25_VF26(savedVectors);
        corePktBuildFaceList(work);
        corePktEmitAllPasses(work);
        corePktSetupPointBuff(work, work->param1);
        VU0_LOAD_VF24_VF25_VF26(savedVectors);
        corePktRelightBackFaces(work->dmaBase, work->param1);
        corePktBuildBackFaceList(work);
        corePktEmitAllPasses(work);
    } else {
        corePktBuildFaceList(work);
        corePktEmitAllPasses(work);
    }
}

u64 *corePktClosePacket(VuWork *work) {
    u32 cursor;
    u32 start;
    s32 quadwords;
    u32 descriptor;
    u64 gifTag;
    u64 *header;

    if (work->packetStart == 0) {
        return NULL;
    }

    cursor = (u32)work->cursor;
    start = (u32)work->ringStart;
    if (cursor == start) {
        sdfGrClosePacketBuff(work->packetStart);
        return NULL;
    }

    while (((u32)cursor & 0xC) != 0) {
        *(u32 *)cursor = 0;
        cursor += 4;
    }
    quadwords = (s32)(cursor - start) >> 4;

    while (((u32)cursor & 0x30) != 0) {
        QUAD_ZERO(*(u128 *)cursor);
        cursor += 0x10;
    }

    sdfGrClosePacketBuff(cursor & 0x0FFFFFFF);
    header = (u64 *)work->header;
    descriptor = 0x20000000 | (quadwords & 0xFFFF);
    gifTag = 0x1100000000000000ULL;
    header[0] = descriptor;
    header[1] = gifTag;
    return header;
}

INCLUDE_ASM(const s32, "code_002B5CD0", corePktVu0Run);

INCLUDE_ASM(const s32, "code_002B5CD0", sdfCorePacketGen);

void sdfCoreDrawScanOneSkinNodes(sdfModelRootNode_t *model, SdfObjectRefCommand *source) {
    SdfDrawNode **objects = model->list->buffer;
    u16 *indices = &source->count;
    if ((source->flags & 0x800) != 0) {
        s32 count = *indices;
        if (count != 0) {
            indices++;
            do {
                sdfModelAllocTmPacket(objects[*indices++]);
            } while (--count != 0);
        }
    }
}

void sdfCoreDrawSetLight(u8 *sourceObject) {
    corePktCurrLight = (u32)sourceObject;
}

void sdfCoreDrawSetFog(u8 *sourceObject) {
    if (corePktCurrFog != (u32)sourceObject) {
        corePktCurrFog = (u32)sourceObject;
        SDF_VU0_COPYVECTOR(&corePktFogParm, sourceObject + 0x10);
    }
}

void sdfCoreDrawSetZBias(f32 bias) {
    corePktZBias = bias;
}

void sdfCoreDrawReset(void) {
    corePktCurrLight = 0;
    corePktCurrFog = 0;
}

void sdfCoreDrawInit(s32 workspaceBytes) {
    u32 *dmaChannel = sceDmaGetChan(0);
    HW_WRITE(dmaChannel, HW_READ(dmaChannel) & ~SDF_DMA_CHCR_TTE);
    sceDmaSendN(dmaChannel, vu0MicroPacket, (vu1MicroPacket - vu0MicroPacket) >> SDF_DMA_QWORD_SHIFT);
    sceDmaSync(dmaChannel, 0, 0);
    sdfMemFreeHandleDirect(&sdfCoreDrawBuffHandle);
    sdfCoreDrawBuffHandle = (u32)sdfMemAllocDirect(workspaceBytes);
    corePktPointBuff = sdfMemLock((struct sdfMemHandle_t *)(sdfCoreDrawBuffHandle));
}

u32 sdf2DGetTexPacketSize(sdfTexHandle_t *texture) {
    return 5 << SDF_DMA_QWORD_SHIFT;
}

SdfTextureDrawPacket *sdf2DMakeTexPacket(SdfTextureDrawPacket *drawPacket, sdfTexHandle_t *texture, s32 contextOffset) {
    drawPacket->header.quadwords = 4;
    drawPacket->header.gifTag = 0x1000000000008003ULL;
    drawPacket->header.command = 0x50000004;
    drawPacket->header.pad08 = 0;
    drawPacket->header.payloadHeader = SDF_GIF_REGISTER_AD;
    drawPacket->textureWordA = sdfTexGetTex1Reg(texture);
    drawPacket->registerAddressA = contextOffset + 0x14;
    drawPacket->textureWordB = sdfTexGetTex0Reg(texture);
    drawPacket->registerAddressB = contextOffset + 6;
    drawPacket->textureWordC = sdfTexGetClampReg(texture);
    drawPacket->registerAddressC = contextOffset + 8;
    return drawPacket;
}

s32 sdf2DLinkTexPacket(sdfDrawTag_t *packetList, sdfTexHandle_t *texture, s32 contextOffset) {
    s32 packetBytes = sdf2DGetTexPacketSize(texture);
    SdfTextureDrawPacket *packet = (SdfTextureDrawPacket *)sdfGrAllocPacketBuff(packetBytes);
    s32 packetAddress = (s32)sdf2DMakeTexPacket(packet, texture, contextOffset);
    sdfDrawTagLink(packetList, packetAddress);
    return packetAddress;
}

u32 sdf2DMakeStdPacket(u32 packetAddress, s32 packetBytes) {
    sdfGrSetRegsPacketHeader((sdfGrPacketHeader_t *)packetAddress, (packetBytes >> SDF_DMA_QWORD_SHIFT) - 2);
    return packetAddress;
}

s32 sdf2DGetPrimPacketSize(s32 registerCount, s32 loopCount) {
    return (registerCount * loopCount + 2) << SDF_DMA_QWORD_SHIFT;
}

void *sdf2DGetPrimPtr(SdfDrawPacket *packet) {
    return packet + 1;
}

SdfDrawPacket *sdf2DMakePrimPacketHeader(SdfDrawPacket *packet, s32 primitiveFlags, s32 registerCount, s64 registerList, s32 loopCount) {
    s32 packetQuadwords = registerCount * loopCount + 1;
    s64 gifTag = loopCount | ((s64)registerCount << SDF_GIF_NREG_SHIFT);

    gifTag |= (s64)primitiveFlags << SDF_GIF_PRIM_SHIFT;
    gifTag |= SDF_GIF_PRE_EOP_FLAGS;
    packet->gifTag = gifTag;
    packet->command = packetQuadwords | SDF_VIF_DIRECT_COMMAND;
    packet->payloadHeader = registerList;
    packet->pad08 = 0;
    packet->quadwords = packetQuadwords;
    return packet;
}

SdfDrawPacket *sdf2DAllocSprite(s32 loopCount) {
    SdfDrawPacket *packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(5, loopCount));
    sdf2DMakePrimPacketHeader(packet, 0x156, 5, 0x53531, loopCount);
    return packet;
}

void sdf3DMakeViewportPacket(sdf3DViewportPacket_t *packet, sdf3DScreenParam_t *node, void *transformMatrix) {
    packet->quadwords = 0xC;
    packet->command = 0x6C0BC000;
    packet->reservedWord = 0;
    SDF_VU0_LOADMATRIX(transformMatrix);
    SDF_VU0_STOREMATRIX(packet->matrixA);
    MultMatrixRev(node->camera.matrix);
    SDF_VU0_STOREMATRIX(packet->matrixB);
    SDF_VU0_LOADVECTOR(&node->camera.halfWidth);
    SDF_VU0_STOREVECTOR(packet->vecA);
    SDF_VU0_LOADVECTOR(&node->camera.originX);
    SDF_VU0_STOREVECTOR(packet->vecB);
    SDF_VU0_LOADMATRIX(transformMatrix);
    ReverseMatrix();
    SDF_VU0_GET_ROW3();
    SDF_VU0_STOREVECTOR(packet->vecC);
    SDF_VU0_STOREVECTOR(node->inverseOrigin);
    packet->stmodCommand = SDF_VIF_ITOP_MATRIX;
    packet->mscalCommand = SDF_VIF_MSCAL_COMMAND;
    packet->unknownC8 = 0;
    packet->unknownCC = 0;
}

void sdf3DSetCamera(sdf3DScreenParam_t *node, void *transformMatrix) {
    SDF_VU0_LOADMATRIX(transformMatrix);
    SDF_VU0_STOREMATRIX(sdfCameraTm);
    MultMatrixRev(node->camera.matrix);
    SDF_VU0_LOADVECTOR(node->inverseOrigin);
    SDF_VU0_STOREMATRIX(sdf3DScreenTm);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_STOREVECTOR(sdf3DEyeOffset);
    memcpy(sdf3DScreenParam, node, sizeof(*node));
}

void sdf3DSetDefaultLight(u8 *sourceObject) {
    sdf3DLightDefault = (u32)sourceObject;
}

void sdf3DSetDefaultFog(u8 *sourceObject) {
    sdf3DFogDefault = (u32)sourceObject;
}

INCLUDE_ASM(const s32, "code_002B5CD0", sdf3DMakeScreenParam);

void sdf3DMakeLightParallelPacket(sdf3DLightParallelPacket_t *packet, sdfLightSources_t sources, f32 *ambient);

INCLUDE_ASM(const s32, "code_002B5CD0", sdf3DMakeLightParallelPacket);

void sdf3DMakeFogPacket(sdf3DFogPacket_t *packet, sdf3DFogParam_t *projectionParams) {
    f32 rangeMax = projectionParams->fogFar;
    f32 rangeMin = projectionParams->fogNear;
    f32 nearZ = projectionParams->depthNear;
    f32 farZ = projectionParams->depthFar;
    f32 depthRange = farZ - nearZ;
    s32 fogColor = projectionParams->fogColor;
    packet->dmaTag = 0x20000004;
    packet->vifUnpackCode = 0x6C03C00013000000ULL;
    packet->gifTag = 0x1000000000008001ULL;
    packet->gifRegister = SDF_GIF_REGISTER_AD;
    packet->registerValue = (u32)fogColor;
    packet->fogColorRegister = SDF_GS_FOGCOL_REGISTER;
    packet->mscalCommand = 0x14000014;
    packet->reservedC = 0;
    packet->right = rangeMax;
    packet->left = rangeMin;
    packet->reservedA = 0;
    packet->reservedB = 0;
    packet->mid = (((rangeMax - rangeMin) * (farZ + nearZ)) / depthRange + (rangeMax + rangeMin)) * 0.5f;
    packet->scale = ((farZ * nearZ) * (rangeMin - rangeMax)) / depthRange;
}

void sdf3DMakeDirectRefPacket(DmaPacketHeader *packet, u32 sourceAddress, s32 payloadBytes) {
    s32 quadwordCount = (payloadBytes + 15) >> SDF_DMA_QWORD_SHIFT;
    packet->quadwords = quadwordCount;
    packet->address = sourceAddress & SDF_DMA_ADDRESS_MASK;
    packet->tag = SDF_VIF_FLUSHA_COMMAND;
    packet->command = quadwordCount | SDF_VIF_DIRECT_COMMAND;
    packet->unknown10 = 0;
    packet->unknown18 = 0;
    packet->unknown1C = 0;
}

void sdf3DLinkInitPacket(sdfDrawTag_t *packetList, DmaPacketHeader *packet) {
    packet->address = (u32)vu1MicroPacket & SDF_DMA_ADDRESS_MASK;
    packet->quadwords = (D_002F5600 - vu1MicroPacket) >> SDF_DMA_QWORD_SHIFT;
    packet->tag = 0;
    packet->command = 0;
    packet->unknown10 = 0;
    packet->unknown18 = 0;
    packet->unknown1C = 0;
    sdfDrawTagLinkRef(packetList, (u32)packet);
}

void sdf3DInitSpecularTex2(void) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(0x2440);
    SdfGeneratedLookupTexture *texture = (SdfGeneratedLookupTexture *)(u32)sdfMemLock(allocation);
    s32 i;
    s32 j;

    sdfTmxSetHeader(&texture->header, 0x80, 0x40, 0x13, 1, 0);
    texture->header.byteCount = 0x2440;
    texture->header.clampMode = 5;
    /* The physical CLUT slot j decodes to the logical intensity i. */
    for (j = 0; j != 0x100; j++) {
        i = ((j & 8) << 1) | ((j & 0x10) >> 1) | (j & ~0x18);
        texture->palette[j] = i | (i << 16) | (i << 8) | 0x80000000;
    }
    memset(&texture->indices[0][1], 0xFF, 0x7F);
    texture->indices[0][0] = 0;
    for (i = 0; i != 0x80; i++) {
        f32 power = i * (1.0f / 127.0f);
        f32 value = power;
        for (j = 1; j != 0x40; j++) {
            texture->indices[j][i] = (u8)(s32)(value * 255.0f + 0.5f);
            value *= power;
        }
    }
    sdf3DSpecularTex = sdfTexSet(&texture->header);
    sdfMemFreeDirect(allocation);
}

void sdf3DInit(void) {
    s64 textureWord;
    sdf3DInitSpecularTex2();
    textureWord = sdfTexGetTex0Reg(sdf3DSpecularTex);
    sdf3DRefMapParam = textureWord;
    vu1Tex0Slot0[0] = textureWord;
    vu1Tex0Slot0[1] = textureWord >> 32;
    D_005518B8 = sdfTexSetDirect((TMX_HEADER_t *)D_002E8ED0);
    textureWord = sdfTexGetTex0Reg(D_005518B8);
    vu1Tex0Slot2[0] = textureWord;
    vu1Tex0Slot2[1] = textureWord >> 32;
    D_005518C8 = sdfTexSetDirect((TMX_HEADER_t *)D_002ECF10);
    textureWord = sdfTexGetTex0Reg(D_005518C8);
    vu1Tex0Slot1[0] = textureWord;
    vu1Tex0Slot1[1] = textureWord >> 32;
    sdfModelInitSystem();
    sdfMatInitSystem();
}

void sdf3DPrimLinkResetTm(sdfDrawTag_t *packetList,
                             s32 (*allocatePacket)(s32)) {
    u64 *referencePacket;
    if (allocatePacket == NULL) {
        allocatePacket = sdfGrAllocPacketBuff;
    }
    referencePacket = (u64 *)allocatePacket(0x20);
    referencePacket[0] = ((u64)((u32)sdf3DPrimIdentityTm & SDF_DMA_ADDRESS_MASK) << 32) | 0x30000008;
    referencePacket[1] = 0x6C07C000ULL << 32;
    QUAD_ZERO(*(u128 *)&referencePacket[2]);
    sdfDrawTagLinkRef(packetList, (u32)referencePacket);
}

/* vu0 routine: store the resident matrix and inverse-column-length scaled rows, then VIF ITOP/MSCAL. */
void sdf3DPrimSetTmPacket(sdf3DTmPacketData_t *lightingPacket) {
    VU0_STORE_MATRIX_AND_UNIT_ROWS(lightingPacket);
    lightingPacket->tag[0] = SDF_VIF_ITOP_LIGHTING;
    lightingPacket->tag[1] = SDF_VIF_MSCAL_COMMAND;
    lightingPacket->tag[2] = 0;
    lightingPacket->tag[3] = 0;
}

void sdf3DPrimLinkTm(sdfDrawTag_t *packetList,
                           s32 (*allocatePacket)(s32)) {
    u64 *dmaPacket;
    if (allocatePacket == NULL) {
        allocatePacket = sdfGrAllocPacketBuff;
    }
    dmaPacket = (u64 *)allocatePacket(0x90);
    dmaPacket[0] = ((u64)((u32)(dmaPacket + 2) & SDF_DMA_ADDRESS_MASK) << 32) | 0x20000008;
    dmaPacket[1] = 0x6C07C000ULL << 32;
    sdf3DPrimSetTmPacket((sdf3DTmPacketData_t *)(dmaPacket + 2));
    sdfDrawTagLink(packetList, (u32)dmaPacket);
}

void sdf3DPrimLinkMat(sdfDrawTag_t *packetList, sdfMat_t *asset,
                              s32 (*allocatePacket)(s32)) {
    u64 *referencePacket;
    if (allocatePacket == NULL) {
        allocatePacket = sdfGrAllocPacketBuff;
    }
    sdfMatUpdatePacket(asset, (s8)sdfFrameId);
    referencePacket = (u64 *)allocatePacket(0x20);
    sdfMatMakePacket(asset, (struct SdfDmaTagHeader *)referencePacket, (s8)sdfFrameId);
    QUAD_ZERO(*(u128 *)&referencePacket[2]);
    sdfDrawTagLinkRef(packetList, (u32)referencePacket);
}

u32 *sdf3DPrimMakePoly(SdfPrimitiveRequest *request) {
    s32 stripWords = request->stripWordCount;
    s32 count = request->vertexCount;
    s32 headerWords = 18;
    s32 wordsPerVertex = 3;
    u32 primitive = request->primitiveFlags | 0x68;
    u32 format = 0x60;
    s32 bytes;
    void *(*allocate)(s32);
    u32 *packet;
    u32 *cursor;
    const u128 *vector;
    const u32 *coordinates;
    const u32 *secondCoordinates;
    s32 i;
    u32 countCode;

    if (request->normals != NULL) {
        format = 0xE0;
        headerWords = 19;
        wordsPerVertex = 6;
    }
    if (request->coordinates != NULL) {
        format |= 0x100;
        primitive |= 0x10;
        headerWords++;
        wordsPerVertex += 2;
        if (request->secondCoordinates != NULL) {
            primitive |= 0x1000;
            wordsPerVertex += 2;
        }
    }
    if (request->vertexColors != NULL) {
        format |= 0x200;
        primitive |= 0x800;
        headerWords++;
        wordsPerVertex++;
    }
    bytes = (headerWords + stripWords + wordsPerVertex * count) * 4;
    bytes = (bytes + 15) & ~15;
    allocate = request->allocate;
    if (allocate == NULL) allocate = sdfGrAllocPacketBuff;
    packet = allocate(bytes);
    *(u64 *)packet = (u64)(u16)((bytes >> 4) - 1) | 0x20000000ULL;
    packet[2] = 0x6102C000;
    packet[3] = (~request->clipMask) & 0xFFFF;
    packet[4] = 0x6E01C002;
    packet[5] = request->color;
    packet[6] = 0x6003C003;
    cursor = packet + 7;
    *cursor++ = (u32)request->depth;
    *cursor++ = (u32)sdfFaceDivSoften;
    *cursor++ = (u32)sdfFaceDivBias;
    *cursor++ = 0x04000004;
    *cursor++ = 0x14000000;
    *cursor++ = 0x6D01C000;
    *cursor++ = (u16)stripWords | ((u32)(u16)count << 16);
    *cursor++ = (primitive & 0xFFFF) | (format << 16);
    *cursor++ = (stripWords << 16) | 0x6E00C001;
    if (request->strip == NULL) {
        memcpy(cursor, sdf3DPrimDefaultFaceIndex, stripWords * 4);
    } else {
        memcpy(cursor, request->strip, stripWords * 4);
    }
    cursor += stripWords;
    stripWords++;
    countCode = count << 16;
    *cursor++ = countCode | stripWords | 0x6800C000;
    vector = request->positions;
    i = 0;
    do {
        MMI_STORE_VEC3_VALUE(cursor, *vector);
        cursor += 3;
        vector++;
        i++;
    } while (i != count);
    stripWords += count;
    vector = request->normals;
    if (vector != NULL) {
        *cursor++ = countCode | stripWords | 0x6800C000;
        i = 0;
        do {
            MMI_STORE_VEC3_VALUE(cursor, *vector);
            cursor += 3;
            vector++;
            i++;
        } while (i != count);
        stripWords += count;
    }
    coordinates = request->coordinates;
    if (coordinates != NULL) {
        secondCoordinates = request->secondCoordinates;
        if (secondCoordinates == NULL) {
            *cursor++ = countCode | stripWords | 0x6400C000;
            memcpy(cursor, coordinates, count * 8);
            cursor += count * 2;
        } else {
            *cursor++ = countCode | stripWords | 0x6C00C000;
            i = 0;
            do {
                *cursor++ = *coordinates++;
                *cursor++ = *coordinates++;
                *cursor++ = *secondCoordinates++;
                *cursor++ = *secondCoordinates++;
                i++;
            } while (i != count);
        }
        stripWords += count;
    }
    if (request->vertexColors != NULL) {
        *cursor++ = countCode | stripWords | 0x6E00C000;
        memcpy(cursor, request->vertexColors, count * 4);
        cursor += count;
    }
    *cursor = 0x1400000C;
    while ((u32)++cursor & 15) {
        *cursor = 0;
    }
    return packet;
}

void sdfBillBoardMakeTMPacket(u8 *packet, const f32 *matrix) {
    *(u16 *)packet = 2;
    *(u32 *)(packet + 8) = 0x6403C000;
    *(f32 *)(packet + 12) = matrix[0];
    *(f32 *)(packet + 16) = matrix[1];
    *(f32 *)(packet + 20) = matrix[4];
    *(f32 *)(packet + 24) = matrix[5];
    *(f32 *)(packet + 28) = matrix[12];
    *(f32 *)(packet + 32) = matrix[13];
    *(u32 *)(packet + 36) = 0x04000000;
    *(u32 *)(packet + 40) = 0x14000008;
    memset(packet + 0x2C, 0, 12);
}

void sdfBillBoardMake1(u8 *packet, const f32 *position, u32 color, const void *uv,
                                   const f32 *quad) {
    *(u16 *)packet = 7;
    *(u32 *)(packet + 8) = 0x6C01C000;
    *(u32 *)(packet + 12) = 1;
    *(u32 *)(packet + 16) = 0xA0000000;
    *(u32 *)(packet + 20) = 0x43434310;
    *(u32 *)(packet + 24) = 0x43;
    *(u32 *)(packet + 28) = 0x6001C001;
    *(u32 *)(packet + 32) = 0x155;
    *(u32 *)(packet + 36) = 0x6801C002;
    *(f32 *)(packet + 40) = position[0];
    *(f32 *)(packet + 44) = position[1];
    *(f32 *)(packet + 48) = position[2];
    *(u32 *)(packet + 52) = 0x6E01C003;
    *(u32 *)(packet + 56) = color;
    *(u32 *)(packet + 60) = 0x6504C004;
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 64), (const u8 *)uv);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 68), (const u8 *)uv + 4);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 72), (const u8 *)uv + 8);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 76), (const u8 *)uv + 12);
    *(u32 *)(packet + 80) = 0x6404C008;
    *(f32 *)(packet + 84) = quad[0];
    *(f32 *)(packet + 88) = quad[1];
    *(f32 *)(packet + 92) = quad[2];
    *(f32 *)(packet + 96) = quad[3];
    *(f32 *)(packet + 100) = quad[4];
    *(f32 *)(packet + 104) = quad[5];
    *(f32 *)(packet + 108) = quad[6];
    *(f32 *)(packet + 112) = quad[7];
    *(u32 *)(packet + 116) = 0x04000002;
    *(u32 *)(packet + 120) = 0x14000008;
    *(u32 *)(packet + 124) = 0;
}

s32 sdfBillBoardGetPacketSize(s32 count) {
    return (count << 6) + 0x40;
}

/* vu0 routine: pack aligned positions into the VIF three-word stream. */
u32 *sdfBillBoardMakeN(const u128 *positions, const void *byteAttributes, const void *halfAttributes, const void *wordAttributes, s32 vertexCount, void *(*allocatePacket)(s32)) {
    s32 packetBytes;
    u32 *packet;
    u32 *cursor;
    u32 vuAddress;
    u32 unpackCode;
    s32 n;
    s32 vertexIndex;

    packetBytes = sdfBillBoardGetPacketSize(vertexCount);
    if (allocatePacket == NULL) {
        cursor = (u32 *)sdfGrAllocPacketBuff(packetBytes);
    } else {
        cursor = allocatePacket(packetBytes);
    }
    packet = cursor;
    packet[0] = (packetBytes >> SDF_DMA_QWORD_SHIFT) - 1;
    packet[1] = 0;
    packet[2] = 0x6C01C000;
    packet[3] = vertexCount;
    packet[4] = 0xA0000000;
    packet[5] = 0x43434310;
    packet[6] = 0x43;
    packet[7] = 0x6001C001;
    packet[8] = 0x155;
    packet[9] = (vertexCount << 16) | 0x6800C002;
    cursor = packet + 10;
    vertexIndex = 0;
    do {
        MMI_STORE_VEC3_VALUE(cursor, positions[vertexIndex]);
        cursor += 3;
        vertexIndex++;
    } while (vertexIndex != vertexCount);
    n = vertexCount;
    unpackCode = n << 16;
    vuAddress = vertexCount + 2;
    *cursor++ = unpackCode | vuAddress | 0x6E00C000;
    memcpy(cursor, byteAttributes, n * 4);
    cursor += n;
    vuAddress += n;
    n = vertexCount * 4;
    unpackCode = n << 16;
    *cursor++ = unpackCode | vuAddress | 0x6500C000;
    vuAddress += n;
    unpackCode |= vuAddress;
    memcpy(cursor, halfAttributes, n * 4);
    cursor += n;
    n *= 8;
    unpackCode |= 0x6400C000;
    *cursor++ = unpackCode;
    memcpy(cursor, wordAttributes, n);
    cursor = (u32 *)((u8 *)cursor + n);
    cursor[0] = SDF_VIF_ITOP_COMPACT_VERTEX;
    cursor[1] = SDF_VIF_MSCAL_VERTICES;
    cursor += 2;
    while (((u32)cursor & SDF_QWORD_ALIGNMENT_MASK) != 0) {
        *cursor++ = 0;
    }
    return packet;
}

void sdfMultiBillBoardMake1(u8 *packet, const f32 *position, u32 primaryColor, u32 secondaryColor,
                   const void *primaryUv, const void *secondaryUv, const f32 *quad) {
    *(u16 *)packet = 8;
    *(u32 *)(packet + 8) = 0x6C01C000;
    *(u32 *)(packet + 12) = 1;
    *(u32 *)(packet + 16) = 0xA0000000;
    *(u32 *)(packet + 20) = 0x43434310;
    *(u32 *)(packet + 24) = 0x43;
    *(u32 *)(packet + 28) = 0x6001C001;
    *(u32 *)(packet + 32) = 0x155;
    *(u32 *)(packet + 36) = 0x6801C002;
    *(f32 *)(packet + 40) = position[0];
    *(f32 *)(packet + 44) = position[1];
    *(f32 *)(packet + 48) = position[2];
    *(u32 *)(packet + 52) = 0x6E02C003;
    *(u32 *)(packet + 56) = primaryColor;
    *(u32 *)(packet + 60) = secondaryColor;
    *(u32 *)(packet + 64) = 0x6D04C005;
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 68), (const u8 *)primaryUv);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 72), (const u8 *)secondaryUv);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 76), (const u8 *)primaryUv + 4);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 80), (const u8 *)secondaryUv + 4);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 84), (const u8 *)primaryUv + 8);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 88), (const u8 *)secondaryUv + 8);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 92), (const u8 *)primaryUv + 12);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 96), (const u8 *)secondaryUv + 12);
    *(u32 *)(packet + 100) = 0x6404C009;
    *(f32 *)(packet + 104) = quad[0];
    *(f32 *)(packet + 108) = quad[1];
    *(f32 *)(packet + 112) = quad[2];
    *(f32 *)(packet + 116) = quad[3];
    *(f32 *)(packet + 120) = quad[4];
    *(f32 *)(packet + 124) = quad[5];
    *(f32 *)(packet + 128) = quad[6];
    *(f32 *)(packet + 132) = quad[7];
    *(u32 *)(packet + 136) = 0x04000004;
    *(u32 *)(packet + 140) = 0x14000008;
}

u32 sdfMultiBillBoardGetPacketSize(s32 count) {
    return (count * 0x54 + 0x4bU) & 0xfffffff0;
}

u32 *sdfMultiBillBoardMakeN(u128 *positions, void *attributes, void *halfAttributes, void *wordAttributes, s32 count, void *(*alloc)(s32)) {
    s32 bytes;
    u32 *packet;
    u32 *cursor;
    u32 index;
    u32 code;
    s32 n;
    s32 i;

    bytes = sdfMultiBillBoardGetPacketSize(count);
    if (alloc == NULL) {
        cursor = (u32 *)sdfGrAllocPacketBuff(bytes);
    } else {
        cursor = alloc(bytes);
    }
    packet = cursor;
    packet[0] = (bytes >> 4) - 1;
    packet[1] = 0;
    packet[2] = 0x6C01C000;
    packet[3] = count;
    packet[4] = 0xA0000000;
    packet[5] = 0x43434310;
    packet[6] = 0x43;
    packet[7] = 0x6001C001;
    packet[8] = 0x155;
    packet[9] = (count << 16) | 0x6800C002;
    cursor = packet + 10;
    i = 0;
    do {
        MMI_STORE_VEC3_VALUE(cursor, positions[i]);
        cursor += 3;
        i++;
    } while (i != count);
    n = count * 2;
    index = count + 2;
    *cursor++ = (n << 16) | index | 0x6E00C000;
    memcpy(cursor, attributes, n * 4);
    cursor += n;
    index += n;
    n = count * 4;
    code = n << 16;
    *cursor++ = code | index | 0x6D00C000;
    memcpy(cursor, halfAttributes, n * 8);
    cursor += n * 2;
    index += n;
    code |= index;
    *cursor++ = code | 0x6400C000;
    memcpy(cursor, wordAttributes, n * 8);
    cursor += n * 2;
    cursor[0] = 0x04000004;
    cursor[1] = 0x14000008;
    cursor += 2;
    while (((u32)cursor & 0xF) != 0) {
        *cursor++ = 0;
    }
    return packet;
}

void sdfBillBoardVcMake1(u8 *packet, const f32 *position, const u32 *colors, const void *uv,
                   const f32 *quad) {
    *(u16 *)packet = 8;
    *(u32 *)(packet + 8) = 0x6C01C000;
    *(u32 *)(packet + 12) = 1;
    *(u32 *)(packet + 16) = 0xD0000000;
    *(u32 *)(packet + 20) = 0x34134130;
    *(u32 *)(packet + 24) = 0x41341;
    *(u32 *)(packet + 28) = 0x6001C001;
    *(u32 *)(packet + 32) = 0x155;
    *(u32 *)(packet + 36) = 0x6801C002;
    *(f32 *)(packet + 40) = position[0];
    *(f32 *)(packet + 44) = position[1];
    *(f32 *)(packet + 48) = position[2];
    *(u32 *)(packet + 52) = 0x6E04C003;
    *(u32 *)(packet + 56) = colors[0];
    *(u32 *)(packet + 60) = colors[1];
    *(u32 *)(packet + 64) = colors[2];
    *(u32 *)(packet + 68) = colors[3];
    *(u32 *)(packet + 72) = 0x6504C007;
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 76), (const u8 *)uv);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 80), (const u8 *)uv + 4);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 84), (const u8 *)uv + 8);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 88), (const u8 *)uv + 12);
    *(u32 *)(packet + 92) = 0x6404C00B;
    *(f32 *)(packet + 96) = quad[0];
    *(f32 *)(packet + 100) = quad[1];
    *(f32 *)(packet + 104) = quad[2];
    *(f32 *)(packet + 108) = quad[3];
    *(f32 *)(packet + 112) = quad[4];
    *(f32 *)(packet + 116) = quad[5];
    *(f32 *)(packet + 120) = quad[6];
    *(f32 *)(packet + 124) = quad[7];
    *(u32 *)(packet + 128) = 0x04000006;
    *(u32 *)(packet + 132) = 0x14000008;
    *(u32 *)(packet + 136) = 0;
    *(u32 *)(packet + 140) = 0;
}

u32 sdfBillBoardVcGetPacketSize(s32 count) {
    return (count * 0x4c + 0x4bU) & 0xfffffff0;
}

u32 sdfBillBoardVcMakeN(const u128 *positions, const void *attributes,
                  const void *halfAttributes, const void *wordAttributes,
                  s32 count, void *(*alloc)(s32)) {
    s32 bytes;
    u32 *packet;
    u32 *cursor;
    u32 index;
    u32 code;
    s32 n;
    s32 i;

    bytes = sdfBillBoardGetPacketSize(count);
    if (alloc == NULL) {
        cursor = (u32 *)sdfGrAllocPacketBuff(bytes);
    } else {
        cursor = alloc(bytes);
    }
    packet = cursor;
    packet[0] = (bytes >> 4) - 1;
    packet[1] = 0;
    packet[2] = 0x6C01C000;
    packet[3] = count;
    packet[4] = 0xD0000000;
    packet[5] = 0x34134130;
    packet[6] = 0x41341;
    packet[7] = 0x6001C001;
    packet[8] = 0x155;
    packet[9] = (count << 16) | 0x6800C002;

    cursor = packet + 10;
    i = 0;
    do {
        MMI_STORE_VEC3_VALUE(cursor, positions[i]);
        cursor += 3;
        i++;
    } while (i != count);

    code = count << 18;
    index = count + 2;
    n = count * 4;
    *cursor++ = code | index | 0x6E00C000;
    memcpy(cursor, attributes, n * 4);
    cursor += n;
    index += n;
    *cursor++ = code | index | 0x6500C000;
    memcpy(cursor, halfAttributes, n * 4);
    cursor += n;
    index += n;
    n = count * 8;
    *cursor++ = code | index | 0x6400C000;
    memcpy(cursor, wordAttributes, n * 4);
    cursor += n;
    cursor[0] = 0x04000006;
    cursor[1] = 0x14000008;
    cursor += 2;
    while (((u32)cursor & 0xF) != 0) {
        *cursor++ = 0;
    }
    return (u32)packet;
}

void sdfMultiBillBoardVcMake1(u8 *packet, const f32 *position, const void *colors, const void *primaryUv,
                   const void *secondaryUv, const void *quad) {
    *(u16 *)packet = 10;
    *(u32 *)(packet + 8) = 0x6C01C000;
    *(u32 *)(packet + 12) = 1;
    *(u32 *)(packet + 16) = 0xD0000000;
    *(u32 *)(packet + 20) = 0x34134130;
    *(u32 *)(packet + 24) = 0x41341;
    *(u32 *)(packet + 28) = 0x6001C001;
    *(u32 *)(packet + 32) = 0x15D;
    *(u32 *)(packet + 36) = 0x6801C002;
    *(f32 *)(packet + 40) = position[0];
    *(f32 *)(packet + 44) = position[1];
    *(f32 *)(packet + 48) = position[2];
    *(u32 *)(packet + 52) = 0x6E08C003;
    memcpy(packet + 56, colors, 32);
    *(u32 *)(packet + 88) = 0x6D04C00B;
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 92), (const u8 *)primaryUv);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 96), (const u8 *)secondaryUv);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 100), (const u8 *)primaryUv + 4);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 104), (const u8 *)secondaryUv + 4);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 108), (const u8 *)primaryUv + 8);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 112), (const u8 *)secondaryUv + 8);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 116), (const u8 *)primaryUv + 12);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 120), (const u8 *)secondaryUv + 12);
    *(u32 *)(packet + 124) = 0x6404C00F;
    memcpy(packet + 128, quad, 32);
    *(u32 *)(packet + 160) = 0x04000008;
    *(u32 *)(packet + 164) = 0x14000008;
    *(u32 *)(packet + 168) = 0;
    *(u32 *)(packet + 172) = 0;
}

u32 sdfMultiBillBoardVcGetPacketSize(s32 count) {
    return (count * 0x6c + 0x4bU) & 0xfffffff0;
}

/* vu0 routine: pack aligned positions into the VIF three-word stream. */
u32 sdfMultiBillBoardVcMakeN(u128 *positions, void *byteAttributes, void *halfAttributes, void *wordAttributes, s32 vertexCount, void *(*allocatePacket)(s32)) {
    s32 packetBytes;
    u32 *packet;
    u32 *cursor;
    u32 vuAddress;
    u32 unpackCode;
    s32 n;
    s32 vertexIndex;

    packetBytes = sdfMultiBillBoardVcGetPacketSize(vertexCount);
    if (allocatePacket == NULL) {
        cursor = (u32 *)sdfGrAllocPacketBuff(packetBytes);
    } else {
        cursor = allocatePacket(packetBytes);
    }
    packet = cursor;
    packet[0] = (packetBytes >> SDF_DMA_QWORD_SHIFT) - 1;
    packet[1] = 0;
    packet[2] = 0x6C01C000;
    packet[3] = vertexCount;
    packet[4] = 0xD0000000;
    packet[5] = 0x34134130;
    packet[6] = 0x41341;
    packet[7] = 0x6001C001;
    packet[8] = 0x15D;
    packet[9] = (vertexCount << 16) | 0x6800C002;
    cursor = packet + 10;
    vertexIndex = 0;
    do {
        MMI_STORE_VEC3_VALUE(cursor, positions[vertexIndex]);
        cursor += 3;
        vertexIndex++;
    } while (vertexIndex != vertexCount);
    n = vertexCount * 8;
    vuAddress = vertexCount + 2;
    *cursor++ = (n << 16) | vuAddress | 0x6E00C000;
    memcpy(cursor, byteAttributes, n * 4);
    cursor += n;
    vuAddress += n;
    n = vertexCount * 4;
    unpackCode = n << 16;
    *cursor++ = unpackCode | vuAddress | 0x6D00C000;
    memcpy(cursor, halfAttributes, n * 8);
    cursor += n * 2;
    vuAddress += n;
    unpackCode |= vuAddress;
    *cursor++ = unpackCode | 0x6400C000;
    memcpy(cursor, wordAttributes, n * 8);
    cursor += n * 2;
    cursor[0] = SDF_VIF_ITOP_WIDE_VERTEX;
    cursor[1] = SDF_VIF_MSCAL_VERTICES;
    cursor += 2;
    while (((u32)cursor & SDF_QWORD_ALIGNMENT_MASK) != 0) {
        *cursor++ = 0;
    }
    return (u32)packet;
}

void sdfPadControl(SdfPadEntry *entry) {
    u8 reply[SDF_PAD_REPLY_BUFFER_BYTES];
    u8 actuatorData[SDF_PAD_ACTUATOR_BYTES];
    s32 port = entry->port;
    s32 slot = entry->slot;
    s32 padState;
    s32 hasButtons;
    s32 hasAnalog;
    s32 hasPressure;
    s32 alignmentStatus;
    s32 requestedMode;
    s32 smallMotor;
    s32 largeMotor;
    reply[0] = -1;
    padState = scePadGetState(port, slot);
    switch (entry->state) {
    case 0:
        if (padState == 2 || padState == 6) {
            entry->lastSmallMotor = -1;
            entry->lastLargeMotor = -1;
            requestedMode = entry->mode = entry->requestedMode;
            switch (requestedMode) {
            case 0:
                entry->state = 3;
                break;
            case 1:
                if (padState == 6) {
                    if (scePadSetMainMode(port, slot, 0, 0) == 1) {
                        entry->state = 1;
                    }
                } else {
                    entry->state = 3;
                }
                break;
            case 2:
                if (padState == 6) {
                    if (scePadSetMainMode(port, slot, 0, 3) == 1) {
                        entry->state = 1;
                    }
                } else {
                    entry->state = 3;
                }
                break;
            case 3:
                if (padState == 6) {
                    if (scePadSetMainMode(port, slot, 1, 3) == 1) {
                        entry->state = 1;
                    }
                } else {
                    entry->state = 3;
                }
                break;
            }
        }
        break;
    case 1:
        if (padState == 6) {
            entry->state = 3;
        } else if (padState != 5) {
            entry->state = 0;
        }
        break;
    case 2:
        break;
    case 3:
        if (scePadInfoAct(port, slot, -1, 0) != 0) {
            if (scePadSetActAlign(port, slot, sdfPadCb_0) != 0) {
                entry->state = 4;
            }
        } else {
            entry->state = 5;
        }
        break;
    case 4:
        alignmentStatus = scePadGetReqState(port, slot);
        if (alignmentStatus != 0) {
            if (alignmentStatus == 1) {
                entry->state = 3;
            }
        } else {
            entry->state = 5;
        }
        break;
    case 5:
        if (padState != 2 && padState != 6) {
            entry->state = 0;
        } else if (entry->mode != entry->requestedMode) {
            entry->state = 0;
        } else {
            scePadRead(port, slot, reply);
            smallMotor = entry->smallMotor;
            largeMotor = entry->largeMotor;
            if (smallMotor != entry->lastSmallMotor || largeMotor != entry->lastLargeMotor) {
                entry->lastSmallMotor = smallMotor;
                entry->lastLargeMotor = largeMotor;
                actuatorData[0] = smallMotor;
                actuatorData[1] = largeMotor;
                scePadSetActDirect(port, slot, actuatorData);
            }
        }
        break;
    }
    /* Pad reply IDs: digital, analog-stick, and pressure-sensitive modes.
     * Missing channels are normalized before consumers see this port. */
    hasAnalog = 0;
    hasPressure = 0;
    hasButtons = 0;
    if (reply[0] == 0) {
        switch (reply[1]) {
        case SDF_PAD_REPLY_DIGITAL:
            hasButtons = 1;
            break;
        case SDF_PAD_REPLY_ANALOG:
            hasButtons = 1;
            hasAnalog = 1;
            break;
        case SDF_PAD_REPLY_PRESSURE:
            hasButtons = 1;
            hasAnalog = 1;
            hasPressure = 1;
            break;
        }
    }
    if (hasButtons) {
        memset(entry->pressure, 0, SDF_PAD_PRESSURE_COUNT);
        entry->buttons = ~(reply[3] | (reply[2] << 8));
    } else {
        entry->buttons = 0;
    }
    if (hasAnalog) {
        entry->stick[0] = reply[4];
        entry->stick[1] = reply[5];
        entry->stick[2] = reply[6];
        entry->stick[3] = reply[7];
    } else {
        entry->stick[0] = SDF_PAD_STICK_CENTER;
        entry->stick[1] = SDF_PAD_STICK_CENTER;
        entry->stick[2] = SDF_PAD_STICK_CENTER;
        entry->stick[3] = SDF_PAD_STICK_CENTER;
    }
    if (hasPressure) {
        memcpy(entry->pressure, &reply[8], SDF_PAD_PRESSURE_COUNT);
    } else {
        memset(entry->pressure, 0, SDF_PAD_PRESSURE_COUNT);
    }
}

void sdfPadManager(void) {
    s32 padIndex;
    for (padIndex = 0; padIndex != SDF_PAD_ENTRY_COUNT; padIndex++) {
        sdfPadControl(&sdfPadCb[padIndex]);
    }
}

INCLUDE_ASM(const s32, "code_002B5CD0", sdfPadWork);

void sdfPadChangeMode(s32 padIndex, u8 mode) {
    sdfPadCb[padIndex].requestedMode = mode;
}

void sdfPadSetAct0(s32 padIndex, u16 strength) {
    sdfPadCb[padIndex].smallMotor = strength;
}

void sdfPadSetAct1(s32 padIndex, u8 strength) {
    sdfPadCb[padIndex].largeMotor = strength;
}

void sdfPadSetAct(s32 padIndex, s32 smallMotor, s32 largeMotor) {
    SdfPadEntry *entry = &sdfPadCb[padIndex];
    entry->smallMotor = smallMotor & 0xFF;
    sdfPadCb[padIndex].largeMotor = largeMotor & 0xFF;
}

void sdfPadInitManager(void) {
    s32 padIndex;

    scePadInit(0);
    for (padIndex = 0; padIndex != SDF_PAD_ENTRY_COUNT; padIndex++) {
        s32 port = sdfPadPortSlotPairs[padIndex * 2];
        s32 slot = sdfPadPortSlotPairs[padIndex * 2 + 1];
        SdfPadEntry *entry;

        scePadPortOpen(port, slot, &D_0058F0C0[padIndex * SDF_PAD_PORT_BUFFER_BYTES]);
        entry = &sdfPadCb[padIndex];
        entry->port = port;
        entry->slot = slot;
        entry->state = 0;
        entry->mode = 0;
        entry->requestedMode = 0;
        entry->buttons = 0;
        entry->prevButtons = 0;
        entry->smallMotor = 0;
        entry->largeMotor = 0;
    }
    memset(dds3PadBitTable, 0, SDF_PAD_BUTTON_STATE_BYTES);
    memset(D_005518E0, SDF_PAD_STICK_CENTER, SDF_PAD_STICK_STATE_BYTES);
    memset(D_005298E8, 0, SDF_PAD_PRESSURE_STATE_BYTES);
    sdfPadManagerMode = 0;
}

INCLUDE_RODATA(const s32, "code_002B5CD0", D_005495C8);

INCLUDE_RODATA(const s32, "code_002B5CD0", D_005495D8);

INCLUDE_SDATA(const s32, "code_002B5CD0", D_005518B8);

INCLUDE_SDATA(const s32, "code_002B5CD0", sdf3DSpecularTex);

INCLUDE_SDATA(const s32, "code_002B5CD0", sdf3DRefMapParam);

INCLUDE_SDATA(const s32, "code_002B5CD0", D_005518C8);

INCLUDE_SDATA(const s32, "code_002B5CD0", sdfPadPortSlotPairs);

INCLUDE_SDATA(const s32, "code_002B5CD0", sdfPadManagerMode);

INCLUDE_SDATA(const s32, "code_002B5CD0", D_005518D5);

INCLUDE_SDATA(const s32, "code_002B5CD0", D_005518D8);

INCLUDE_SDATA(const s32, "code_002B5CD0", D_005518E0);

INCLUDE_SDATA(const s32, "code_002B5CD0", D_005518E1);

INCLUDE_SDATA(const s32, "code_002B5CD0", D_005518E2);

