#include "common.h"
#include "ps2asm.h"

struct sdfGrPacketHeader_t;
struct sdfGrTagStdReg_t;
struct IpuDmaState;
struct SdfPacket;
struct SdfKeyTreeItem;
union MovObjStream;
struct DevRequest;
struct PacWork;
struct sdfThCB_t;
struct sdfTexPacket_t;
struct sdfGsMemSlice_t;
struct sdfIpuDecodeCb_t;
struct movieStream_t;
struct sdfGrFrameSet_t;
struct sdfTexHandle_t;
struct sdfGrFramePacket_t;
struct sdfMemHandle_t;
struct TMX_HEADER_t;
struct sdfFileCmdHandle_t;
struct SdfPacStreamPacketHeader;
struct PacBuf;
struct PacListAllocation;
struct PacState;
struct SdfPacDecoder;
struct SdfKeyPoolNode;
struct sdfSortOtNode_tag;
struct SdfPool;
struct SdfStreamReadRequest;
struct SdfStreamState;
typedef int s128 __attribute__((mode(TI)));
/* Convert three Euler angle components to the VU0 quaternion output.
 * Providers: DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */
void FromEuler(f32 x, f32 y, f32 z);
/* Load matrix into the VU0 scratch bank and postmultiply the primary bank. */
void MultMatrixRev(const void *matrix);
/* Read the raw free-running count from EE timer 0. */
extern u32 sdfGetRunTimeCountBase(void);
/* Return the modulo-16-bit elapsed count from the EE timer 0 value. */
extern u32 sdfGetRunTimeCount(u32 previous);
/* Appending the packet patches the DMA tag's link; the VIF and GIF words
 * retain the renderer's original commands and register-list encoding. */
typedef struct sdfGrPacketHeader_t {
    u64 dmaTag;
    u64 vifCommands;
    u64 gifTag;
    u64 gifRegisters;
} QWORD_ALIGNED sdfGrPacketHeader_t;
typedef struct sdfGrTagStdReg_t {
    u64 value;
    u64 registerId;
} sdfGrTagStdReg_t;
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
struct SoundFormat;
/* The +0x1C movie work pointer selects one of two separately allocated records.
 * isPac at +3 chooses the active arm; the variant payload types live in
 * sdf_movie_stream.h. */
struct MovLinearStream;
struct MovPacStream;
/* Four-doubleword DMA packet payload (0x20); DDS1/2 game/code_002D33C8/0032C278.c. */
typedef struct SdfPacket {
    u64 unk0;
    u64 unk8;
    u64 unk10;
    u64 unk18;
} SdfPacket;
#define SDF_POOL_FREE_KIND 0xFFFF
/* The wire-model clipper keeps the segment in vf10/vf11, its colours in
 * vf8/vf9, the viewport scale and offset in vf14/vf15 and the plane in vf2. */
#define VU0_LOAD_VF14(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf14, 0(%0)\n\t.set reorder" \
    : : "r" (src))
#define VU0_LOAD_VF15_AT16(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf15, 16(%0)\n\t.set reorder" \
    : : "r" (src))
#define VU0_TRANSFORM_POINT_VF11() __asm__ volatile ( \
    ".set noreorder\n\tvmulax.xyzw ACC, vf28, vf11x\n\tvmadday.xyzw ACC, vf29, vf11y\n\t" \
    "vmaddaz.xyzw ACC, vf30, vf11z\n\tvmaddw.xyzw vf11, vf31, vf0w\n\t.set reorder")
#define VU0_CLIP_VF11() __asm__ volatile ( \
    ".set noreorder\n\tvclipw.xyz vf11, vf11w\n\t.set reorder")
#define VU0_READ_CLIP_FLAGS_WAIT5(out) __asm__ volatile ( \
    ".set noreorder\n\tvnop\n\tvnop\n\tvnop\n\tvnop\n\tvnop\n\tcfc2.ni %0, $vi18\n\t.set reorder" \
    : "=r" (out))
#define VU0_STORE_VF8_AT0(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf8, 0(%0)\n\t.set reorder" \
    : : "r" (dst))
#define VU0_STORE_VF10_AT16(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf10, 16(%0)\n\t.set reorder" \
    : : "r" (dst))
#define VU0_STORE_VF9_AT32(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf9, 32(%0)\n\t.set reorder" \
    : : "r" (dst))
#define VU0_STORE_VF11_AT48(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf11, 48(%0)\n\t.set reorder" \
    : : "r" (dst))
#define VU0_POSITIVE_CLIP_PLANE_VF2() __asm__ volatile ( \
    ".set noreorder\n\tvaddw.xyz vf2, vf0, vf0w\n\t" \
    "vmulw.w vf2, vf0, vf0w\n\t.set reorder")
#define VU0_NEGATIVE_CLIP_PLANE_VF2() __asm__ volatile ( \
    ".set noreorder\n\tvsubw.xyzw vf2, vf0, vf0w\n\t" \
    "vsubw.w vf2, vf0, vf0w\n\t.set reorder")
#define VU0_CLIP_SEGMENT_PREP_FIRST() __asm__ volatile ( \
    ".set noreorder\n\t" \
    "vmulw.xyzw vf3, vf2, vf10w\n\t" \
    "vmulw.xyzw vf4, vf2, vf11w\n\t" \
    "vsub.xyzw vf5, vf11, vf10\n\t" \
    "vsub.xyzw vf6, vf9, vf8\n\t" \
    "vsub.xyzw vf3, vf10, vf3\n\t" \
    "vsub.xyzw vf4, vf11, vf4\n\t" \
    "vsub.xyzw vf4, vf4, vf3\n\t.set reorder")
#define VU0_CLIP_SEGMENT_PREP_SECOND() __asm__ volatile ( \
    ".set noreorder\n\t" \
    "vmulw.xyzw vf3, vf2, vf11w\n\t" \
    "vmulw.xyzw vf4, vf2, vf10w\n\t" \
    "vsub.xyzw vf5, vf10, vf11\n\t" \
    "vsub.xyzw vf6, vf8, vf9\n\t" \
    "vsub.xyzw vf3, vf11, vf3\n\t" \
    "vsub.xyzw vf4, vf10, vf4\n\t" \
    "vsub.xyzw vf4, vf4, vf3\n\t.set reorder")
#define VU0_CLIP_RATIO_X() __asm__ volatile ( \
    ".set noreorder\n\tvnop\n\tvnop\n\tvdiv Q, vf3x, vf4x\n\t.set reorder")
#define VU0_CLIP_RATIO_Y() __asm__ volatile ( \
    ".set noreorder\n\tvnop\n\tvnop\n\tvdiv Q, vf3y, vf4y\n\t.set reorder")
#define VU0_CLIP_RATIO_Z() __asm__ volatile ( \
    ".set noreorder\n\tvnop\n\tvnop\n\tvdiv Q, vf3z, vf4z\n\t.set reorder")
#define VU0_PROJECT_COLORED_SEGMENT() __asm__ volatile ( \
    ".set noreorder\n\t" \
    "vdiv Q, vf0w, vf10w\n\t" \
    "vmul.xyz vf10, vf10, vf14\n\t" \
    "vmulaw.xyz ACC, vf15, vf0w\n\t" \
    "vmulx.w vf10, vf10, vf0x\n\t" \
    "vftoi0.xyzw vf8, vf8\n\tvwaitq\n\t" \
    "vmaddq.xyz vf10, vf10, Q\n\t" \
    "vdiv Q, vf0w, vf11w\n\t" \
    "vmul.xyz vf11, vf11, vf14\n\t" \
    "vmulx.w vf11, vf11, vf0x\n\t" \
    "vftoi4.xyzw vf10, vf10\n\t" \
    "vftoi0.xyzw vf9, vf9\n\tvwaitq\n\t" \
    "vmaddq.xyz vf11, vf11, Q\n\t" \
    "vftoi4.xyzw vf11, vf11\n\t.set reorder")
#define EE_MMI_RGBA_UNPACK_READONLY(src, unit) __asm__ volatile ( \
    ".set noreorder\n" \
    "lw $2, 0(%1)\n" \
    "pextlb $2, $0, $2\n" \
    "pextlh $2, $0, $2\n" \
    "qmtc2.ni $2, vf10\n" \
    "vitof0.xyzw vf10, vf10\n" \
    "qmtc2.ni %0, vf2\n" \
    "vmulx.xyzw vf10, vf10, vf2x\n" \
    ".set reorder" \
    : : "r"(unit), "r"(src), "m"(*(const u32 *)(src)) : "$2")
#define EE_MMI_RGBA_TO_VF8(rgba) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "pextlb $2, $0, %0\n\t" \
    "pextlh $2, $0, $2\n\t" \
    "qmtc2.ni $2, vf8\n\t" \
    "vitof0.xyzw vf8, vf8\n\t" \
    ".set reorder" \
    : : "r" (rgba) : "$2")
#define EE_MMI_RGBA_TO_VF9(rgba) __asm__ volatile ( \
    ".set noreorder\n\t" \
    "pextlb $2, $0, %0\n\t" \
    "pextlh $2, $0, $2\n\t" \
    "qmtc2.ni $2, vf9\n\t" \
    "vitof0.xyzw vf9, vf9\n\t" \
    ".set reorder" \
    : : "r" (rgba) : "$2")
/* Protocol IDs use distinct byte-sized storage codes in DevState. */
typedef u8 SdfDevOperationCode;
typedef s8 SdfDevStateCode;
#define SDF_DEV_EVENT_INACTIVE 0
#define SDF_DEV_EVENT_OPENED 2
#define SDF_DEV_EVENT_SIZE_REPLY 4
#define SDF_DEV_EVENT_READ_REPLY 5
#define SDF_DEV_EVENT_CLOSED 7
/* Tree view of a pool node: the link word at +0 becomes the replaced-item
 * link, +0x4..+0xB stay opaque, and +0xC is compared as a float key. */
typedef struct SdfKeyTreeItem {
    struct SdfKeyTreeItem *replaced; /* 0x00 */
    u8 pad04[8];                     /* 0x04: opaque */
    f32 key;                         /* 0x0C */
} SdfKeyTreeItem;
enum {
    SDF_TMX_MAGIC = 0x30584D54,
    SDF_STREAM_CHUNK_BYTES = 0x4000,
    SDF_STREAM_COMMAND_CREATE_STATE = 32,
    SDF_STREAM_COMMAND_CONTROL = 33,
    SDF_STREAM_COMMAND_READ = 34,
    SDF_STREAM_COMMAND_RELEASE_STATE = 35,
    SDF_IOP_MODULES_PER_GROUP = 2,
    SDF_KEY_TREE_PATH_LIMIT = 32,
    SDF_POOL_REBUILD_CHAIN = 0,
    SDF_POOL_PUSH_FREE = 1
};
/* Read the entry count word, size the list for it and arm the skip handler. */




enum {
    SDF_DECODE_READ_COMMAND = 1,
    SDF_DECODE_READ_COUNT_LOW = 2,
    SDF_DECODE_READ_COUNT_HIGH = 3,
    SDF_DECODE_DISPATCH = 4,
    SDF_DECODE_COPY_LITERAL = 5,
    SDF_DECODE_FILL_BYTE = 6,
    SDF_DECODE_COPY_SHORT_BACKREF = 7,
    SDF_DECODE_READ_BACKREF_LOW = 8,
    SDF_DECODE_READ_BACKREF_HIGH = 9,
    SDF_DECODE_EXPAND_BYTES = 10
};
/* PAC command high bits choose the decoder operation; low five bits encode length.
 * Values at or above 0xE0 are reserved except for the 0xFF end marker. */
enum {
    SDF_DECODE_COMMAND_CLASS_MASK = 0xE0,
    SDF_DECODE_COMMAND_LENGTH_MASK = 0x1F,
    SDF_DECODE_COMMAND_END_MARKER = 0xFF,
    SDF_DECODE_COMMAND_RESERVED_MIN = 0xE0,
    SDF_DECODE_COMMAND_COPY_LITERAL = 0x00,
    SDF_DECODE_COMMAND_ZERO_FILL = 0x20,
    SDF_DECODE_COMMAND_REPEAT_BYTE = 0x40,
    SDF_DECODE_COMMAND_COPY_SHORT_BACKREFERENCE = 0x60,
    SDF_DECODE_COMMAND_COPY_LONG_BACKREFERENCE = 0x80,
    SDF_DECODE_COMMAND_EXPAND_BYTES = 0xA0
};
extern const u8 *D_00594780[16];
/* Incrementally decode the PAC byte stream, preserving partial commands when
 * the caller's input chunk ends. */




extern f32 sdfDrawFaceListParam[][4];
/* vu0 routine: clip homogeneous line pairs and build a coloured GS packet. */


extern u8 sdf3DScreenTm[];
extern u8 D_00594940[];
/* Load SDFIOPFL, wake the creator, and serve the registered stream RPC commands. */


extern void StartThread();
extern void SleepThread(void);
/* Start the newly created worker, then record and suspend the calling thread. */


extern char *mcModuleNames[];
extern s32 sceSifLoadModule(const char *, s32, const char *);
extern char *strcpy(char *, const char *);
extern char *strcat(char *, const char *);
extern void sceMcInit(void);
extern void sdfGrJoinDrawTag(void *left, void *right);
typedef union MovObjStream {
    struct MovLinearStream *linear;
    struct MovPacStream *pac;
} MovObjStream;
typedef struct DevRequest {
    struct sdfMemHandle_t *backingAllocation;
    s16 usedCount;
    u16 capacity;
    s16 stride;
    s16 growStep;
    void *buffer;
} DevRequest;
/*
 * SDK word carriers used by effSampleColorAlphaTracks and the bit-scaled
 * billboard blends. Each conversion owns its native 16-byte workspace.
 * Other unpack/pack operand contracts keep their existing macros.
 * See docs/idioms.md, "SDK RGBA word carriers".
 */
static inline void eeUnpackRGBAWord(u32 color, u32 unitBits) {
    u32 rgba[4];
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK_READONLY(rgba, unitBits);
}
/* Queue node allocated with an inline, variable-length PAC packet. */
typedef struct PacWork {
    struct PacWork *next; /* 0x00 */
    struct PacState *owner; /* 0x04 */
    void *resourceHandle; /* 0x08: tag-selected allocation or resource-list pointer */
    u8 *dataCursor; /* 0x0C */
    u8 packet[0]; /* 0x10: copied packet header, extensions, and data */
} PacWork;
#define VU0_CLIP_SEGMENT_UPDATE_FIRST() __asm__ volatile ( \
    ".set noreorder\n\tvwaitq\n\tvaddq.x vf2, vf0, Q\n\tvabs.x vf2, vf2\n\t" \
    "vmulaw.xyzw ACC, vf10, vf0w\n\t" \
    "vmaddx.xyzw vf10, vf5, vf2x\n\t" \
    "vmulaw.xyzw ACC, vf8, vf0w\n\t" \
    "vmaddx.xyzw vf8, vf6, vf2x\n\t" \
    "vclipw.xyz vf10, vf10w\n\t.set reorder")
#define VU0_CLIP_SEGMENT_UPDATE_SECOND() __asm__ volatile ( \
    ".set noreorder\n\tvwaitq\n\tvaddq.x vf2, vf0, Q\n\tvabs.x vf2, vf2\n\t" \
    "vmulaw.xyzw ACC, vf11, vf0w\n\t" \
    "vmaddx.xyzw vf11, vf5, vf2x\n\t" \
    "vmulaw.xyzw ACC, vf9, vf0w\n\t" \
    "vmaddx.xyzw vf9, vf6, vf2x\n\t" \
    "vclipw.xyz vf11, vf11w\n\t.set reorder")
/* Allocate a workspace sized by stackBytes, pass the thread entry to
 * CreateThreadSimple, and return that function's result unchanged. */
s32 sdfCreateThreadWithStack(void (*entryAddress)(), s32 stackBytes, s32 priority);
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
/* Allocate a low-VRAM image block and record its logical dimensions and format.
 * The caller owns the returned block and releases it with the masked updater below. */
sdfGsMemSlice_t *sdfGsAllocFrameBuff(s32 width, s32 height, s32 format);
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
/* General-heap allocation descriptor (0x10); links bound the represented span. */
typedef struct sdfMemHandle_t {
    struct sdfMemHandle_t *prev; /* 0x00 */
    struct sdfMemHandle_t *next; /* 0x04 */
    s32 address;              /* 0x08: start of represented span */
    u16 state;                /* 0x0C: free, used, or end sentinel */
    s16 referenceCount;       /* 0x0E: -1 for sentinels */
} sdfMemHandle_t;
DevRequest *sdfItemArrayNew(s32 count, s32 stride, s32 growStep);
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
s32 sdfFileCmdRead(sdfFileCmdHandle_t *state, void *data, s32 byteCount);
s32 sdfFileCmdFree(sdfFileCmdHandle_t *state);
s32 sdfFileCmdClearError(sdfFileCmdHandle_t *state);
s32 sdfFileCmdClose(sdfFileCmdHandle_t *request);
/* Fixed serialized header shared by the PAC stream reader and dispatcher.
 * The model packet view names the word at +0x08 as a tag. */
typedef struct SdfPacStreamPacketHeader {
    u8 command; /* 0x00 */
    u8 flags; /* 0x01 */
    u8 reserved02[2];
    s32 payloadSize; /* 0x04: serialized packet bytes */
    u8 reserved08[4];
    s32 decodedSize; /* 0x0C */
    u8 payload[0]; /* 0x10: variable packet contents */
} SdfPacStreamPacketHeader;
/* Complete 0x10-byte packet buffer: texture is the acquired output, while
 * resourceSlot owns the optional general-heap allocation descriptor. */
typedef struct PacBuf {
    struct sdfTexHandle_t *texture;
    struct sdfMemHandle_t *resourceSlot;
    u8 *cursor;
    s32 remainingBytes;
} PacBuf;
/* Per-queue allocation-list descriptor selected through PacState.slot. */
typedef struct PacListAllocation {
    s32 entryCount; /* 0x00 */
    s32 entryIndex; /* 0x04 */
    u8 pad08[8]; /* 0x08 */
    SdfPacStreamPacketHeader entry; /* 0x10 */
    PacBuf buffer; /* 0x20 */
} PacListAllocation;
/* Complete 0x38-byte PAC decoder state shared by file and model loaders. */
typedef struct PacState {
    s8 phase; /* 0x00: -1 complete, 0 boundary, 1 skip, 2 input handler */
    u8 flags; /* 0x01 */
    u16 packetCounter; /* 0x02: wraps from the initial 0xFFFF */
    s32 (*packetCallback)(); /* 0x04: event callbacks have different arities */
    void (*onInput)(struct PacState *); /* 0x08 */
    void (*onComplete)(struct PacState *); /* 0x0C */
    const u8 *inputCursor; /* 0x10 */
    s32 inputAvailable; /* 0x14 */
    s32 consumedBytes; /* 0x18 */
    u8 *outputCursor; /* 0x1C */
    s32 pendingBytes; /* 0x20 */
    u8 *decoder; /* 0x24: decoder work block, held as a byte pointer */
    PacBuf *resourceBuffer; /* 0x28 */
    union {
        PacBuf *resource;
        PacListAllocation *list;
    } slot; /* 0x2C */
    PacWork *queueHead; /* 0x30 */
    PacWork *queueTail; /* 0x34 */
} PacState;
s32 sdfPacDecode(PacState *state, const u8 *input, s32 inputByteCount);
/* Incremental decoder access view shared by both providers. The backing
 * decoder block is allocated as 0x20 bytes; only this 0x14-byte prefix is
 * described here, and its trailing allocation bytes remain unknown. */
typedef struct SdfPacDecoder {
    u8 state;             /* 0x00 */
    u8 command;           /* 0x01 */
    u16 pendingBytes;     /* 0x02 */
    u8 *output;           /* 0x04 */
    const u8 *input;      /* 0x08 */
    s32 inputBytes;       /* 0x0C */
    u8 backrefLow;        /* 0x10 */
    u8 pad11[3];          /* 0x11: opaque */
} SdfPacDecoder;
/* Pool release logic tests the packet head address and kind at 0xC;
 * bytes at 0x8 remain opaque. The float-key tree insertion helper reads the
 * same +0xC word through SdfKeyTreeItem. DDS1/2 game/code_002EEA18/003478C0.c. */
typedef struct SdfKeyPoolNode {
    struct SdfKeyPoolNode *next; /* 0x00 */
    s32 packetHeadAddress;       /* 0x04: first DMA packet address. */
    u8 unk8[4];                  /* 0x08 */
    s32 kind;                    /* 0x0C */
} SdfKeyPoolNode;
typedef struct sdfSortOtNode_tag {
    struct sdfSortOtNode_tag *right; /* 0x00 */
    struct sdfSortOtNode_tag *left;  /* 0x04 */
    struct SdfKeyTreeItem *item;  /* 0x08 */
    s32 balance;                  /* 0x0C */
} sdfSortOtNode_tag;
/* A key-sorted draw-surface pool: an SdfPoolNode whose append and prepend
 * callbacks release a node into the pool and update its free list. */
typedef struct SdfPool {
    void *link;                                                /* 0x00: the pool entry's link */
    SdfKeyPoolNode *head;                                      /* 0x04: first node in the emitted chain */
    SdfKeyPoolNode *tail;                                      /* 0x08: last node in the emitted chain */
    SdfKeyPoolNode *freeHead;                                  /* 0x0C */
    void (*release)(struct SdfPool *, SdfKeyPoolNode *);       /* 0x10: the entry's append */
    void (*update)(struct SdfPool *, s32, SdfKeyPoolNode *);   /* 0x14: the entry's prepend */
    sdfSortOtNode_tag *keyTree[1];                                /* 0x18: first root; allocation continues */
} SdfPool;
/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 size);
/* Read the active packet cursor or restore it at a quadword boundary. */
s32 sdfGrOpenPacketBuff(void);
void sdfGrClosePacketBuff(s32 cursorAddress);
typedef struct SdfStreamReadRequest {
    u8 opaque00[4];
    s32 destinationAddress; /* Caller-provided address word in the read request. */
    s32 byteCount;
} SdfStreamReadRequest;
typedef struct SdfStreamState {
    sdfFileCmdHandle_t *deviceState;
    s32 semaphore;
    s32 readResult;
    s32 remainingBytes;
    u8 *destination; /* +0x10: current DMA destination; advanced per completed chunk. */
    s32 transferredBytes;
} SdfStreamState;
extern s32 D_00552170;
/* vu0 routine: render line pairs using the cached composed matrix. */


/* vu0 routine: compose the current matrix with the cached transform, then render line pairs. */


/* Initialize a zeroed TMX0 header with the caller's palette metadata. */


extern SdfStreamState sdfFileRpcState;
extern s32 sdfFileRpcBuf[];
extern sdfFileCmdHandle_t *sdfFileCmdReadOpen(const char *path,
                                          s32 (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32),
                                          s32 callbackContext);
extern s32 sdfFileCmdFileSize();
extern s32 WaitSema(s32);
/* Queue at most one staging-buffer-sized read; the completion callback advances the byte counts. */


extern void FlushCache(s32);
extern s32 SignalSema(s32);
extern s32 sceSifSetDma(void *, s32);
extern s32 sceSifDmaStat(s32);
extern s32 GetThreadId(void);
extern s32 sdfFileRpcThreadId;
void SdfFileRpcServerThreadProc(void);
/* One row per administrative mode. The entry receives the restore flag and
 * mode data; destroy and cleanup return scheduler results. Each row is 12 bytes
 * on the EE. */
typedef struct Dds3AdminModeDispatch {
    void (*entry)(s32 restoring, void *data);
    s32 (*destroy)(void);
    s32 (*cleanup)(void);
} Dds3AdminModeDispatch;
extern void *memcpy(void *, const void *, unsigned int);

s32 sdfDecomp(SdfPacDecoder *decoder, const u8 *input, s32 inputBytes);
void sdfDecompInit(SdfPacDecoder *decoder, u8 *output);
SdfPacket *sdfDrawModelWithMatrix(const f32 vertices[][4], const u32 *colors, s32 vertexCount, u32 primitiveFlags);
SdfPacket *effDebugDrawWireModel(const f32 vertices[][4], const u32 *colors, s32 vertexCount, u32 primitiveFlags);
SdfPacket *sdfDrawRotatedModel(const f32 vertices[][4], const u32 *colors, s32 vertexCount, u32 primitiveFlags);
void sdfTmxSetHeader(TMX_HEADER_t *header, s32 width, s32 height, s32 depth, s32 paletteCount, s32 clutFormat);
void sdfFileRpcContinueRead(void);
s32 sdfFileRpcReadCallback(sdfFileCmdHandle_t *deviceState, s32 command, s32 sourceAddress,
                  s32 byteCount, s32 callbackArg);
s32 *sdfFileRpcHandler(u32 command, s32 request);
void sdfStartFileRpcServer(void);
void LoadMcModules(const char *prefix, s32 moduleGroup);
sdfSortOtNode_tag *sdfSortOtAvlRotateLR(sdfSortOtNode_tag *heavyChild, sdfSortOtNode_tag *parent);
sdfSortOtNode_tag *sdfSortOtAvlRotateRL(sdfSortOtNode_tag *heavyChild, sdfSortOtNode_tag *parent);
void sdfSortOtAvlCheckBalance(sdfSortOtNode_tag **path, s32 depth, sdfSortOtNode_tag *child,
                   sdfSortOtNode_tag **root);
void sdfSortOtAvlRegist(sdfSortOtNode_tag **tree, SdfKeyTreeItem *item);
void sdfGrSortOtReg(SdfPool *pool, SdfKeyPoolNode *node);
void sdfGrSortOtFixup(void *first, SdfKeyPoolNode **head, SdfKeyPoolNode **tail);
void sdfGrSortOtTraverse(sdfSortOtNode_tag *node, SdfKeyPoolNode **head, SdfKeyPoolNode **tail);
void sdfGrSortOtSetup(SdfPool *pool);
void sdfGrSortOtCallback(SdfPool *pool, s32 mode, SdfKeyPoolNode *freeNode);

s32 sdfDecomp(SdfPacDecoder *decoder, const u8 *input, s32 inputBytes) {
    s32 count;
    s32 offset;
    u8 *output;
    u32 sourceAddress;

    if (decoder->state == 0) {
        return 1;
    }

    D_00552170 = 0;
    output = decoder->output;
    if (inputBytes > 0) {
        do {
            switch (decoder->state) {
                case SDF_DECODE_READ_COMMAND: {
                    u8 command;
                    s32 commandBytes;
                    s32 historyIndex = D_00552170;

                    inputBytes--;
                    D_00594780[historyIndex] = input;
                    D_00552170 = (historyIndex + 1) & 0xF;
                    command = *input++;
                    if (command >= SDF_DECODE_COMMAND_RESERVED_MIN) {
                        if (command == SDF_DECODE_COMMAND_END_MARKER) {
                            decoder->output = output;
                            decoder->input = input;
                            decoder->inputBytes = inputBytes;
                            decoder->state = 0;
                            return 1;
                        }
                    } else {
                        decoder->command = command & SDF_DECODE_COMMAND_CLASS_MASK;
                        commandBytes = command & SDF_DECODE_COMMAND_LENGTH_MASK;
                        if (commandBytes == 0) {
                            decoder->state = SDF_DECODE_READ_COUNT_LOW;
                        } else {
                            decoder->pendingBytes = commandBytes;
                            decoder->state = SDF_DECODE_DISPATCH;
                        }
                    }
                    break;
                }
                case SDF_DECODE_READ_COUNT_LOW:
                    decoder->pendingBytes = *input++;
                    inputBytes--;
                    decoder->state = SDF_DECODE_READ_COUNT_HIGH;
                    break;
                case SDF_DECODE_READ_COUNT_HIGH:
                    decoder->pendingBytes |= *input++ << 8;
                    inputBytes--;
                    decoder->state = SDF_DECODE_DISPATCH;
                    break;
                case SDF_DECODE_DISPATCH:
                    switch (decoder->command) {
                    case SDF_DECODE_COMMAND_COPY_LITERAL:
                        decoder->state = SDF_DECODE_COPY_LITERAL;
                        break;
                    case SDF_DECODE_COMMAND_ZERO_FILL:
                        count = decoder->pendingBytes;
                        memset(output, 0, count);
                        output += count;
                        decoder->state = SDF_DECODE_READ_COMMAND;
                        break;
                    case SDF_DECODE_COMMAND_REPEAT_BYTE:
                        decoder->state = SDF_DECODE_FILL_BYTE;
                        break;
                    case SDF_DECODE_COMMAND_COPY_SHORT_BACKREFERENCE:
                        decoder->state = SDF_DECODE_COPY_SHORT_BACKREF;
                        break;
                    case SDF_DECODE_COMMAND_COPY_LONG_BACKREFERENCE:
                        decoder->state = SDF_DECODE_READ_BACKREF_LOW;
                        break;
                    case SDF_DECODE_COMMAND_EXPAND_BYTES:
                        decoder->state = SDF_DECODE_EXPAND_BYTES;
                        break;
                    }
                    break;
                case SDF_DECODE_COPY_LITERAL:
                    count = decoder->pendingBytes;
                    if (inputBytes < count) {
                        count = inputBytes;
                    }
                    memcpy(output, input, count);
                    output += count;
                    input += count;
                    inputBytes -= count;
                    decoder->pendingBytes -= count;
                    if (decoder->pendingBytes == 0) {
                        decoder->state = SDF_DECODE_READ_COMMAND;
                    }
                    break;
                case SDF_DECODE_FILL_BYTE:
                    count = decoder->pendingBytes;
                    memset(output, *input++, count);
                    output += count;
                    inputBytes--;
                    decoder->state = SDF_DECODE_READ_COMMAND;
                    break;
                case SDF_DECODE_COPY_SHORT_BACKREF:
                    sourceAddress = (u32)(output - *input++);
                    count = decoder->pendingBytes;
                    inputBytes--;
                    do {
                        *output++ = *(u8 *)sourceAddress++;
                    } while (--count != 0);
                    decoder->state = SDF_DECODE_READ_COMMAND;
                    break;
                case SDF_DECODE_READ_BACKREF_LOW:
                    decoder->backrefLow = *input++;
                    inputBytes--;
                    if (inputBytes == 0) {
                        decoder->state = SDF_DECODE_READ_BACKREF_HIGH;
                        break;
                    }
                    /* fallthrough */
                case SDF_DECODE_READ_BACKREF_HIGH: {
                    s32 combinedOffset;

                    offset = *input++ << 8;
                    sourceAddress = decoder->backrefLow;
                    inputBytes--;
                    combinedOffset = sourceAddress | offset;
                    sourceAddress = (u32)(output - combinedOffset);
                    count = decoder->pendingBytes;
                    do {
                        *output++ = *(u8 *)sourceAddress++;
                    } while (--count != 0);
                    decoder->state = SDF_DECODE_READ_COMMAND;
                    break;
                }
                case SDF_DECODE_EXPAND_BYTES:
                    count = decoder->pendingBytes;
                    if (inputBytes < count) {
                        count = inputBytes;
                        inputBytes = 0;
                        decoder->pendingBytes -= count;
                    } else {
                        decoder->pendingBytes = 0;
                        decoder->state = SDF_DECODE_READ_COMMAND;
                        inputBytes -= count;
                    }
                    do {
                        *output++ = *input++;
                        *output++ = 0;
                    } while (--count != 0);
                    break;
            }
        } while (inputBytes > 0);
    }
    decoder->input = input;
    decoder->inputBytes = inputBytes;
    decoder->output = output;
    return 0;
}

void sdfDecompInit(SdfPacDecoder *decoder, u8 *output) {
    decoder->output = output;
    decoder->state = 1;
}

SdfPacket *sdfDrawModelWithMatrix(const f32 vertices[][4], const u32 *colors, s32 vertexCount, u32 primitiveFlags) {
    SdfPacket *packet;
    f32 (*viewport)[4];
    u128 *cursor;
    s32 i;
    s32 emitted;
    s32 plane;
    u32 clipFlags;
    u32 firstClip;
    u32 secondClip;
    u32 quadwordCount;

    packet = (SdfPacket *)sdfGrOpenPacketBuff();
    cursor = (u128 *)(packet + 1);
    viewport = sdfDrawFaceListParam;
    VU0_LOAD_VF14(viewport);
    VU0_LOAD_VF15_AT16(viewport);
    emitted = 0;
    for (i = 0; i < vertexCount; i += 2) {
        VU0_LOAD_VF10(vertices[i]);
        EE_MMI_RGBA_TO_VF8(colors[i]);
        VU0_TRANSFORM_POINT_VF10();
        VU0_LOAD_VF11(vertices[i + 1]);
        EE_MMI_RGBA_TO_VF9(colors[i + 1]);
        VU0_TRANSFORM_POINT_VF11();
        VU0_CLIP_VF10();
        VU0_CLIP_VF11();
        VU0_READ_CLIP_FLAGS_WAIT5(clipFlags);
        firstClip = (clipFlags >> 6) & 0x3F;
        secondClip = clipFlags & 0x3F;
        if ((firstClip & secondClip) != 0 || (firstClip & 0x20) != 0 || (secondClip & 0x20) != 0) {
            continue;
        }
        if (firstClip != 0 || secondClip != 0) {
            for (plane = 4; plane != -1; plane--) {
                if (firstClip & (1 << plane)) {
                    if ((plane & 1) == 0) {
                        VU0_POSITIVE_CLIP_PLANE_VF2();
                    } else {
                        VU0_NEGATIVE_CLIP_PLANE_VF2();
                    }
                    VU0_CLIP_SEGMENT_PREP_FIRST();
                    switch (plane) {
                    case 0:
                    case 1:
                        VU0_CLIP_RATIO_X();
                        break;
                    case 2:
                    case 3:
                        VU0_CLIP_RATIO_Y();
                        break;
                    default:
                        VU0_CLIP_RATIO_Z();
                        break;
                    }
                    VU0_CLIP_SEGMENT_UPDATE_FIRST();
                    VU0_READ_CLIP_FLAGS_WAIT5(firstClip);
                    firstClip &= 0x3F;
                    if (firstClip & secondClip) {
                        break;
                    }
                }
                if (secondClip & (1 << plane)) {
                    if ((plane & 1) == 0) {
                        VU0_POSITIVE_CLIP_PLANE_VF2();
                    } else {
                        VU0_NEGATIVE_CLIP_PLANE_VF2();
                    }
                    VU0_CLIP_SEGMENT_PREP_SECOND();
                    switch (plane) {
                    case 0:
                    case 1:
                        VU0_CLIP_RATIO_X();
                        break;
                    case 2:
                    case 3:
                        VU0_CLIP_RATIO_Y();
                        break;
                    default:
                        VU0_CLIP_RATIO_Z();
                        break;
                    }
                    VU0_CLIP_SEGMENT_UPDATE_SECOND();
                    VU0_READ_CLIP_FLAGS_WAIT5(secondClip);
                    secondClip &= 0x3F;
                    if (firstClip & secondClip) {
                        break;
                    }
                }
            }
            if (firstClip & secondClip) {
                continue;
            }
        }
        VU0_PROJECT_COLORED_SEGMENT();
        VU0_STORE_VF8_AT0(cursor);
        VU0_STORE_VF10_AT16(cursor);
        VU0_STORE_VF9_AT32(cursor);
        VU0_STORE_VF11_AT48(cursor);
        cursor += 4;
        emitted += 2;
    }
    sdfGrClosePacketBuff((s32)cursor & 0x0FFFFFFF);
    quadwordCount = emitted * 2 + 1;
    packet->unk0 = (quadwordCount & 0xFFFF) | 0x20000000;
    packet->unk8 = ((u64)(quadwordCount | 0x50000000) << 32) | 0x10000000;
    packet->unk10 = emitted | ((u64)(primitiveFlags | 9) << 47) | 0x2000400000008000ULL;
    packet->unk18 = 0x41;
    return packet;
}

SdfPacket *effDebugDrawWireModel(const f32 vertices[][4], const u32 *colors, s32 vertexCount, u32 primitiveFlags) {
    VU0_LOAD_MATRIX(sdf3DScreenTm);
    return sdfDrawModelWithMatrix(vertices, colors, vertexCount, primitiveFlags);
}

SdfPacket *sdfDrawRotatedModel(const f32 vertices[][4], const u32 *colors, s32 vertexCount, u32 primitiveFlags) {
    MultMatrixRev(sdf3DScreenTm);
    return sdfDrawModelWithMatrix(vertices, colors, vertexCount, primitiveFlags);
}

void sdfTmxSetHeader(TMX_HEADER_t *header, s32 width, s32 height, s32 depth, s32 paletteCount, s32 clutFormat) {
    memset(header, 0, sizeof(TMX_HEADER_t));
    header->clutFormat = clutFormat;
    header->width = width;
    header->height = height;
    header->pixelFormat = depth;
    header->paletteCount = paletteCount;
    header->magic = SDF_TMX_MAGIC;
    header->unk00 = 2;
}

void sdfFileRpcContinueRead(void) {
    s32 chunkBytes = sdfFileRpcState.remainingBytes;

    if (chunkBytes > SDF_STREAM_CHUNK_BYTES) {
        chunkBytes = SDF_STREAM_CHUNK_BYTES;
    }
    sdfFileCmdRead(sdfFileRpcState.deviceState, D_00594940, chunkBytes);
}

s32 sdfFileRpcReadCallback(sdfFileCmdHandle_t *deviceState, s32 command, s32 sourceAddress,
                  s32 byteCount, s32 callbackArg) {
    SdfStreamState *stream = &sdfFileRpcState;
    s32 dmaId;
    s32 timer;

    switch (command) {
    case SDF_DEV_EVENT_SIZE_REPLY:
        stream->readResult = byteCount;
        SignalSema(stream->semaphore);
        break;
    case SDF_DEV_EVENT_READ_REPLY: {
        struct {
            s32 source;
            s32 destination;
            s32 size;
            s32 attributes;
        } transfer;

        FlushCache(0);
        transfer.source = sourceAddress;
        transfer.destination = (s32)stream->destination;
        transfer.size = byteCount;
        transfer.attributes = 0;
        dmaId = sceSifSetDma(&transfer, 1);
        while (sceSifDmaStat(dmaId) >= 0) {
            timer = sdfGetRunTimeCountBase();
            while ((s32)sdfGetRunTimeCount(timer) <= 0) {
            }
        }
        timer = sdfGetRunTimeCountBase();
        while ((s32)sdfGetRunTimeCount(timer) < 0x64) {
        }
        stream->destination += byteCount;
        stream->transferredBytes += byteCount;
        stream->remainingBytes -= byteCount;
        if (stream->remainingBytes > 0) {
            sdfFileRpcContinueRead();
        } else {
            SignalSema(stream->semaphore);
        }
        break;
    }
    case SDF_DEV_EVENT_OPENED:
    case SDF_DEV_EVENT_CLOSED:
        SignalSema(stream->semaphore);
        break;
    case SDF_DEV_EVENT_INACTIVE:
        SignalSema(stream->semaphore);
        break;
    }
    return 0;
}

s32 *sdfFileRpcHandler(u32 command, s32 request) {
    SdfStreamState *state = &sdfFileRpcState;
    sdfFileCmdHandle_t *deviceState;
    switch (command) {
    case SDF_STREAM_COMMAND_CREATE_STATE:
        state->deviceState = sdfFileCmdReadOpen((const char *)request, sdfFileRpcReadCallback, 0);
        if (state->deviceState != 0) {
            WaitSema(state->semaphore);
            deviceState = state->deviceState;
            if (deviceState->result != 0) {
                sdfFileCmdClearError(deviceState);
                sdfFileCmdFree(state->deviceState);
                state->deviceState = 0;
                deviceState = 0;
            }
        } else {
            deviceState = 0;
        }
        sdfFileRpcBuf[0] = (s32)deviceState;
        break;
    case SDF_STREAM_COMMAND_CONTROL:
        sdfFileCmdFileSize(state->deviceState, request, request);
        WaitSema(state->semaphore);
        sdfFileRpcBuf[0] = state->readResult;
        break;
    case SDF_STREAM_COMMAND_READ:
        state->remainingBytes = ((SdfStreamReadRequest *)request)->byteCount;
        state->transferredBytes = 0;
        state->destination = (u8 *)((SdfStreamReadRequest *)request)->destinationAddress;
        sdfFileRpcContinueRead();
        WaitSema(state->semaphore);
        sdfFileRpcBuf[0] = state->transferredBytes;
        break;
    case SDF_STREAM_COMMAND_RELEASE_STATE:
        sdfFileCmdClose(state->deviceState);
        WaitSema(state->semaphore);
        sdfFileCmdFree(state->deviceState);
        state->deviceState = 0;
        sdfFileRpcBuf[0] = 0;
        break;
    }
    return sdfFileRpcBuf;
}

typedef struct _reent _reent;
typedef __builtin_va_list va_list;
typedef struct __sbuf {
    u8 *_base;
    s32 _size;
} __sbuf;
typedef struct __sFILE {
    u8 *_p;
    s32 _r;
    s32 _w;
    s16 _flags;
    s16 _file;
    __sbuf _bf;
    s32 _lbfsize;
    void *_cookie;
    s32 (*_read)(void *cookie, char *buf, s32 n);
    s32 (*_write)(void *cookie, const char *buf, s32 n);
    long (*_seek)(void *cookie, s32 offset, s32 whence);
    s32 (*_close)(void *cookie);
    __sbuf _ub;
    u8 *_up;
    s32 _ur;
    u8 _ubuf[3];
    u8 _nbuf[1];
    __sbuf _lb;
    s32 _blksize;
    s32 _offset;
    _reent *_data;
} FILE;
typedef struct __siov {
    void *iov_base;
    u32 iov_len;
} __siov;
typedef struct __suio {
    __siov *uio_iov;
    s32 uio_iovcnt;
    s32 uio_resid;
} __suio;
typedef struct _Bigint {
    struct _Bigint *next;
    s32 k;
    s32 maxwds;
    s32 sign;
    s32 wds;
    u32 x[1];
} _Bigint;
typedef struct _glue {
    struct _glue *next;
    s32 niobs;
    FILE *iobs;
} _glue;
typedef struct AtExitEntry {
    void *unk00;
    void *unk04;
    void *fn;
    void *unk0C;
    void *unk10;
    struct AtExitEntry *next;
} AtExitEntry;
typedef struct AtExitList {
    u32 **entries;
    s32 count;
} AtExitList;
typedef struct _atexit {
    struct _atexit *_next;
    s32 _ind;
    void (*_fns[32])(void);
} _atexit;
struct _reent {
    s32 _errno;
    FILE *_stdin;
    FILE *_stdout;
    FILE *_stderr;
    s32 _inc;
    char _emergency[25];
    s32 _current_category;
    const char *_current_locale;
    s32 __sdidinit;
    void (*__cleanup)(struct _reent *r);
    _Bigint *_result;
    s32 _result_k;
    _Bigint *_p5s;
    _Bigint **_freelist;
    u8 _unused[0xF8];
    _atexit *_atexit;
    _atexit _atexit0;
    void (**_sig_func)(s32 sig);
    struct _glue __sglue;
    FILE __sf[3];
};
extern _reent *D_0052BE28 __attribute__((section(".data")));
extern s32 _puts_r(_reent *r, const char *s);
extern s32 _raise_r(_reent *r, s32 sig);
extern void *_signal_r(_reent *r, s32 sig, void *handler);
extern s32 _setlocale_r(_reent *r, s32 category, const char *locale);
extern void *func_002DA308(_reent *r);
extern void _cleanup_r(_reent *r);
extern s32 fflush(void *fp);
extern s32 _fwalk(_reent *r, s32 (*func)(void *));
extern s32 libc_2DC670(void);
extern long _strtol_r(_reent *r, const char *s, char **end, s32 base);
extern long strtol(const char *s, char **end, s32 base);
extern s32 raise(s32 sig);
extern void func_001001C8(s32 code) __attribute__((noreturn));

extern u8 D_0054B8D8[] __attribute__((section(".data")));
extern s32 D_00552178 __attribute__((section(".data")));
extern s32 D_0052C28C __attribute__((section(".data")));
extern s32 D_0052C288 __attribute__((section(".data")));
extern s32 D_0052C378 __attribute__((section(".data")));
extern s32 _init_signal_r(_reent *r);
extern void gcc2_compiled__2(FILE *fp, s32 flags, s32 file, _reent *data);
extern _glue *__sfmoreglue(_reent *r, s32 n);
extern s32 __sigtramp_r(_reent *r, s32 sig);
extern void *libc_2D2648(void *fn);
extern void free(void *ptr);
extern void *malloc(u32 size);
extern void __malloc_lock(_reent *r);
extern void __malloc_unlock(_reent *r);
extern void *_malloc_r(_reent *r, u32 size);
extern void _free_r(_reent *r, void *ptr);
extern s32 SignalSema(s32 sema);
extern void *sbrk(s32 incr);
extern s32 close(s32 fd);
extern _Bigint *_Balloc(_reent *r, s32 k);
extern s32 registerAtExit(void *fn, void *entry);
extern s32 libc_2D25B0(void *fn, void *entry);

extern s32 write(s32 fd, const void *buf, s32 count);
extern s32 read(s32 fd, void *buf, s32 count);
extern long lseek(s32 fd, s32 offset, s32 whence);
extern s32 fstat(s32 fd, void *buf);
extern s32 kill(s32 pid, s32 sig);
extern long _read_r(_reent *r, s32 fd, char *buf, s32 n);
extern long _write_r(_reent *r, s32 fd, const char *buf, s32 n);
extern long _lseek_r(_reent *r, s32 fd, s32 offset, s32 whence);
extern s32 __sfvwrite(FILE *fp, __suio *uio);
extern s32 __sread(void *cookie, char *buf, s32 n);
extern s32 __swrite(void *cookie, const char *buf, s32 n);
extern long __sseek(void *cookie, s32 offset, s32 whence);
extern s32 __sclose(void *cookie);
extern s32 GetThreadId(void);
extern s32 WaitSema(s32 sema);
extern s32 vfprintf(FILE *fp, const char *fmt, va_list ap);

extern void *memset(void *s, s32 c, u32 n);
extern void *_calloc_r(_reent *r, u32 n, u32 size);
extern const double D_0054B930[];

extern void abort(void);
extern s32 initAtExit(void);
extern s32 atexit(void (*fn)(void));
extern void (**D_0052BB28)(void) __attribute__((section(".data")));
extern s32 D_0052BB2C __attribute__((section(".data")));
extern s32 D_0052C37C __attribute__((section(".data")));
extern u8 D_0052D940[] __attribute__((section(".data")));
extern void (*D_002F5600[])(void) __attribute__((section(".data")));
extern s32 D_0059AD58 __attribute__((section(".data")));
extern AtExitEntry *D_0059AD60 __attribute__((section(".data")));
extern AtExitEntry D_0059AD40;
typedef struct SdfUnpacked {
    s32 kind;
    s32 sign;
    s32 exp;
    u32 mant;
} SdfUnpacked;
extern void sdfUnpackFloat(const void *x, SdfUnpacked *u);
extern f32 sdfPackFloat(const SdfUnpacked *u);
extern long dptoul(s32 kind, s32 sign, s32 exp, long mant);

#define va_start(ap, last) __builtin_stdarg_start((ap), last)
#define va_end(ap) ((void)0)
extern s32 func_002D6678(_reent *r, FILE *fp, const char *fmt, va_list ap);
typedef union f32_bits {
    f32 f;
    u32 u;
} f32_bits;
typedef struct SdfUnpackedD {
    s32 kind;
    s32 sign;
    s32 exp;
    long mant;
} SdfUnpackedD;
extern long libc_2D34A0(const SdfUnpackedD *u);
extern void sdfUnpackDouble(const double *x, SdfUnpackedD *u);
extern SdfUnpackedD *libc_2D1590(const SdfUnpackedD *a, const SdfUnpackedD *b, SdfUnpackedD *r);
extern long libc_2D3638(const SdfUnpackedD *a, const SdfUnpackedD *b);
extern const u8 D_0054B551[];
#define SDF_TOLOWER(c) ((D_0054B551[(c)] & 1) ? (c) + 0x20 : (c))

INCLUDE_ASM(const s32, "code_002C6F38", SdfFileRpcServerThreadProc);

INCLUDE_ASM(const s32, "code_002C6F38", sdf_2C7BC4);

void sdfStartFileRpcServer(void) {
    s32 workerId = sdfCreateThreadWithStack(SdfFileRpcServerThreadProc, 0x1000, 0x4C);

    StartThread(workerId, 0);
    sdfFileRpcThreadId = GetThreadId();
    SleepThread();
}

void LoadMcModules(const char *prefix, s32 moduleGroup) {
    char path[0x100];
    char **moduleNames = &mcModuleNames[moduleGroup * SDF_IOP_MODULES_PER_GROUP];
    s32 moduleIndex;

    for (moduleIndex = 0; moduleIndex != SDF_IOP_MODULES_PER_GROUP; moduleIndex++) {
        strcpy(path, prefix);
        strcat(path, moduleNames[moduleIndex]);
        sceSifLoadModule(path, 0, 0);
    }
    sceMcInit();
}

sdfSortOtNode_tag *sdfSortOtAvlRotateLR(sdfSortOtNode_tag *heavyChild, sdfSortOtNode_tag *parent) {
    sdfSortOtNode_tag *childRoot = heavyChild;
    sdfSortOtNode_tag *pivot;

    if (childRoot->balance < 0) {
        pivot = childRoot->left;
        parent->right = pivot->left;
        childRoot->left = pivot->right;
        pivot->left = parent;
        pivot->right = childRoot;
        if (pivot->balance > 0) {
            parent->balance = -1;
            childRoot->balance = 0;
        } else {
            parent->balance = 0;
            childRoot->balance = 1;
        }
        pivot->balance = 0;
    } else {
        parent->balance = 0;
        parent->right = childRoot->left;
        childRoot->balance = 0;
        childRoot->left = parent;
        pivot = childRoot;
    }
    return pivot;
}

sdfSortOtNode_tag *sdfSortOtAvlRotateRL(sdfSortOtNode_tag *heavyChild, sdfSortOtNode_tag *parent) {
    sdfSortOtNode_tag *childRoot = heavyChild;
    sdfSortOtNode_tag *pivot;

    if (childRoot->balance > 0) {
        pivot = childRoot->right;
        parent->left = pivot->right;
        childRoot->right = pivot->left;
        pivot->right = parent;
        pivot->left = childRoot;
        if (pivot->balance > 0) {
            parent->balance = -1;
            childRoot->balance = 0;
        } else {
            parent->balance = 0;
            childRoot->balance = 1;
        }
        pivot->balance = 0;
    } else {
        parent->balance = 0;
        parent->left = childRoot->right;
        childRoot->balance = 0;
        childRoot->right = parent;
        pivot = childRoot;
    }
    return pivot;
}

void sdfSortOtAvlCheckBalance(sdfSortOtNode_tag **path, s32 depth, sdfSortOtNode_tag *child,
                   sdfSortOtNode_tag **root) {
    sdfSortOtNode_tag *parent;
    sdfSortOtNode_tag *replacement = NULL;
    s32 balance;

    if (depth > 0) {
        do {
            parent = path[--depth];
            if (child == parent->right) {
                balance = ++parent->balance;
            } else {
                balance = --parent->balance;
            }
            if (balance == 0) {
                return;
            }
            if (balance > 1) {
                replacement = sdfSortOtAvlRotateLR(child, parent);
                break;
            }
            if (balance < -1) {
                replacement = sdfSortOtAvlRotateRL(child, parent);
                break;
            }
            child = parent;
        } while (depth > 0);
    }
    if (depth > 0) {
        sdfSortOtNode_tag *ancestor = path[--depth];
        if (ancestor->right == parent) {
            ancestor->right = replacement;
        } else {
            ancestor->left = replacement;
        }
    } else if (replacement != NULL) {
        *root = replacement;
    }
}

void sdfSortOtAvlRegist(sdfSortOtNode_tag **tree, SdfKeyTreeItem *item) {
    sdfSortOtNode_tag *path[SDF_KEY_TREE_PATH_LIMIT];
    s32 depth = 0;
    sdfSortOtNode_tag **link = tree;
    sdfSortOtNode_tag *cur = *tree;
    sdfSortOtNode_tag *node;
    f32 key = item->key;

    while (cur != NULL) {
        if (key == cur->item->key) {
            item->replaced = cur->item;
            cur->item = item;
            return;
        }
        path[depth] = cur;
        depth++;
        if (cur->item->key < key) {
            link = &cur->right;
        } else {
            link = &cur->left;
        }
        cur = *link;
    }
    item->replaced = NULL;
    node = sdfGrAllocPacketBuff(0x10);
    node->right = NULL;
    node->left = NULL;
    node->item = item;
    *link = node;
    node->balance = 0;
    if (depth != 0) {
        sdfSortOtAvlCheckBalance(path, depth, node, tree);
    }
}

void sdfGrSortOtReg(SdfPool *pool, SdfKeyPoolNode *node) {
    if (node->packetHeadAddress != 0) {
        if (node->kind == SDF_POOL_FREE_KIND) {
            node->next = pool->freeHead;
            pool->freeHead = node;
        } else {
            /* The key tree views the pool node's +0xC word as its float key. */
            sdfSortOtAvlRegist(pool->keyTree, (SdfKeyTreeItem *)node);
        }
    }
}

void sdfGrSortOtFixup(void *first, SdfKeyPoolNode **head, SdfKeyPoolNode **tail) {
    SdfKeyPoolNode *current;
    SdfKeyPoolNode *next;

    if (first != NULL) {
        if (*tail == NULL) {
            *head = first;
        } else {
            sdfGrJoinDrawTag(*tail, first);
        }
        current = first;
        while ((next = current->next) != NULL) {
            sdfGrJoinDrawTag(current, next);
            current = next;
        }
        *tail = current;
    }
}

void sdfGrSortOtTraverse(sdfSortOtNode_tag *node, SdfKeyPoolNode **head, SdfKeyPoolNode **tail) {
    sdfSortOtNode_tag *next;

    do {
        if (node->right != NULL) {
            sdfGrSortOtTraverse(node->right, head, tail);
        }
        sdfGrSortOtFixup(node->item, head, tail);
        next = node->left;
        node = next;
    } while (next != NULL);
}

void sdfGrSortOtSetup(SdfPool *pool) {
    SdfKeyPoolNode *head = NULL;
    SdfKeyPoolNode *tail = NULL;

    sdfGrSortOtFixup(pool->freeHead, &head, &tail);
    if (pool->keyTree[0] != NULL) {
        sdfGrSortOtTraverse(pool->keyTree[0], &head, &tail);
    }
    pool->head = head;
    pool->tail = tail;
}

void sdfGrSortOtCallback(SdfPool *pool, s32 mode, SdfKeyPoolNode *freeNode) {
    switch (mode) {
    case SDF_POOL_REBUILD_CHAIN:
        sdfGrSortOtSetup(pool);
        break;
    case SDF_POOL_PUSH_FREE:
        if (freeNode->packetHeadAddress != 0) {
            freeNode->next = pool->freeHead;
            pool->freeHead = freeNode;
        }
        break;
    }
}

void sdfModelSetOtCallbacks(SdfPool *pool) {
    pool->release = sdfGrSortOtReg;
    pool->update = sdfGrSortOtCallback;
}

INCLUDE_ASM(const s32, "code_002C6F38", sceGsResetGraph);

INCLUDE_ASM(const s32, "code_002C6F38", sceGsGetGParam);

INCLUDE_ASM(const s32, "code_002C6F38", sceGsResetPath);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054A3A0);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054A3B0);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054A3C0);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054A3D0);

INCLUDE_ASM(const s32, "code_002C6F38", checkModelVersion);

INCLUDE_ASM(const s32, "code_002C6F38", _GetGsDxDyOffset);

INCLUDE_ASM(const s32, "code_002C6F38", sceGsSetDefDispEnv);

INCLUDE_ASM(const s32, "code_002C6F38", sceGsPutDispEnv);

INCLUDE_ASM(const s32, "code_002C6F38", sceGsSyncPath);

INCLUDE_ASM(const s32, "code_002C6F38", sceGsSetDefLoadImage);

INCLUDE_ASM(const s32, "code_002C6F38", sceGsSetDefStoreImage);

INCLUDE_ASM(const s32, "code_002C6F38", sceGsExecLoadImage);

INCLUDE_ASM(const s32, "code_002C6F38", sceGsExecStoreImage);

INCLUDE_ASM(const s32, "code_002C6F38", sceGsPutIMR);

INCLUDE_ASM(const s32, "code_002C6F38", sceGsGetIMR);

INCLUDE_ASM(const s32, "code_002C6F38", WaitDma);

INCLUDE_ASM(const s32, "code_002C6F38", WatchDma);

INCLUDE_ASM(const s32, "code_002C6F38", CheckAddress);

INCLUDE_ASM(const s32, "code_002C6F38", memclr);

INCLUDE_ASM(const s32, "code_002C6F38", sceDmaGetChan);

INCLUDE_ASM(const s32, "code_002C6F38", sceDmaReset);

INCLUDE_ASM(const s32, "code_002C6F38", sceDmaPutEnv);

INCLUDE_ASM(const s32, "code_002C6F38", sceDmaGetEnv);

INCLUDE_ASM(const s32, "code_002C6F38", sceDmaPutStallAddr);

INCLUDE_ASM(const s32, "code_002C6F38", sceDmaSend);

INCLUDE_ASM(const s32, "code_002C6F38", sceDmaSendN);

INCLUDE_ASM(const s32, "code_002C6F38", sceDmaSendI);

INCLUDE_ASM(const s32, "code_002C6F38", sceDmaRecv);

INCLUDE_ASM(const s32, "code_002C6F38", sceDmaRecvN);

INCLUDE_ASM(const s32, "code_002C6F38", sceDmaRecvI);

INCLUDE_ASM(const s32, "code_002C6F38", sceDmaSync);

INCLUDE_ASM(const s32, "code_002C6F38", sceDmaWatch);

INCLUDE_ASM(const s32, "code_002C6F38", sceDmaPause);

INCLUDE_ASM(const s32, "code_002C6F38", sceDmaRestart);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054A8F0);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054A908);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054A930);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054A940);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054A950);

INCLUDE_ASM(const s32, "code_002C6F38", _send_to_iop);

INCLUDE_ASM(const s32, "code_002C6F38", scePadInit);

INCLUDE_ASM(const s32, "code_002C6F38", scePadInit2);

INCLUDE_ASM(const s32, "code_002C6F38", scePadEnd);

INCLUDE_ASM(const s32, "code_002C6F38", scePadPortOpen);

INCLUDE_ASM(const s32, "code_002C6F38", scePadPortClose);

INCLUDE_ASM(const s32, "code_002C6F38", scePadGetDmaStr);

INCLUDE_ASM(const s32, "code_002C6F38", scePadGetFrameCount);

INCLUDE_ASM(const s32, "code_002C6F38", scePadRead);

INCLUDE_ASM(const s32, "code_002C6F38", scePadGetState);

INCLUDE_ASM(const s32, "code_002C6F38", scePadStateIntToStr);

INCLUDE_ASM(const s32, "code_002C6F38", scePadSetReqState);

INCLUDE_ASM(const s32, "code_002C6F38", scePadGetReqState);

INCLUDE_ASM(const s32, "code_002C6F38", scePadReqIntToStr);

INCLUDE_ASM(const s32, "code_002C6F38", scePadInfoAct);

INCLUDE_ASM(const s32, "code_002C6F38", scePadInfoComb);

INCLUDE_ASM(const s32, "code_002C6F38", scePadInfoMode);

INCLUDE_ASM(const s32, "code_002C6F38", scePadSetMainMode);

INCLUDE_ASM(const s32, "code_002C6F38", scePadSetActDirect);

INCLUDE_ASM(const s32, "code_002C6F38", scePadSetActAlign);

INCLUDE_ASM(const s32, "code_002C6F38", scePadGetButtonMask);

INCLUDE_ASM(const s32, "code_002C6F38", scePadSetButtonInfo);

INCLUDE_ASM(const s32, "code_002C6F38", scePadInfoPressMode);

INCLUDE_ASM(const s32, "code_002C6F38", scePadEnterPressMode);

INCLUDE_ASM(const s32, "code_002C6F38", scePadExitPressMode);

INCLUDE_ASM(const s32, "code_002C6F38", scePadSetVrefParam);

INCLUDE_ASM(const s32, "code_002C6F38", scePadGetPortMax);

INCLUDE_ASM(const s32, "code_002C6F38", scePadGetSlotMax);

INCLUDE_ASM(const s32, "code_002C6F38", scePadGetModVersion);

INCLUDE_ASM(const s32, "code_002C6F38", scePadSetWarningLevel);

INCLUDE_ASM(const s32, "code_002C6F38", gcc2_compiled__6);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdDelayThread);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdCallback);

INCLUDE_ASM(const s32, "code_002C6F38", _sceCd_cd_callback);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054AA58);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054AA60);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054AA68);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054AA70);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054AA80);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054AA88);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054AA98);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054AAA0);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054AAA8);

INCLUDE_ASM(const s32, "code_002C6F38", _Cdvd_cbLoop);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdInitEeCB);

INCLUDE_ASM(const s32, "code_002C6F38", _sceCd_cd_read_intr);

INCLUDE_ASM(const s32, "code_002C6F38", cmd_sem_init);

INCLUDE_ASM(const s32, "code_002C6F38", cdvd_exit);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdPOffCallback);

INCLUDE_ASM(const s32, "code_002C6F38", _sceCd_Poff_Intr);

INCLUDE_ASM(const s32, "code_002C6F38", PowerOffCB);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdLayerSearchFile);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdSearchFile);

INCLUDE_ASM(const s32, "code_002C6F38", _sceCd_ncmd_prechk);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdNcmdDiskReady);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdSync);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdSyncS);

INCLUDE_ASM(const s32, "code_002C6F38", _sceCd_scmd_prechk);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdInit);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdDiskReady);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdMmode);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdGetDiskType);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdStatus);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdPowerOff);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdReadClock);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdStInit);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdStStart);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdStSeekF);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdStSeek);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdStStop);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdStRead);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdStPause);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdStResume);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdStStat);

INCLUDE_ASM(const s32, "code_002C6F38", sceCdStream);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifMInitRpc);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifMExitRpc);

INCLUDE_ASM(const s32, "code_002C6F38", _sceRpcGetPacket);

INCLUDE_ASM(const s32, "code_002C6F38", _sceRpcFreePacket);

INCLUDE_ASM(const s32, "code_002C6F38", _request_end);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifMBindRpc);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifMBindRpcParam);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifMUnBindRpc);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054ADD8);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifMCallRpc);

INCLUDE_ASM(const s32, "code_002C6F38", gcc2_compiled__7);

INCLUDE_ASM(const s32, "code_002C6F38", setD4_CHCR);

INCLUDE_ASM(const s32, "code_002C6F38", sceIpuStopDMA);

INCLUDE_ASM(const s32, "code_002C6F38", sceIpuRestartDMA);

INCLUDE_ASM(const s32, "code_002C6F38", sceIpuSync);

INCLUDE_ASM(const s32, "code_002C6F38", gcc2_compiled__8);

INCLUDE_ASM(const s32, "code_002C6F38", sceIpuInit);

INCLUDE_ASM(const s32, "code_002C6F38", sceMcInit);

INCLUDE_ASM(const s32, "code_002C6F38", sceMcEnd);

INCLUDE_ASM(const s32, "code_002C6F38", _lmcGetClientPtr);

INCLUDE_ASM(const s32, "code_002C6F38", sceMcChangeThreadPriority);

INCLUDE_ASM(const s32, "code_002C6F38", sceMcGetSlotMax);

INCLUDE_ASM(const s32, "code_002C6F38", sceMcOpen);

INCLUDE_ASM(const s32, "code_002C6F38", sceMcMkdir);

INCLUDE_ASM(const s32, "code_002C6F38", sceMcClose);

INCLUDE_ASM(const s32, "code_002C6F38", sceMcSeek);

INCLUDE_ASM(const s32, "code_002C6F38", mceIntrReadFixAlign);

INCLUDE_ASM(const s32, "code_002C6F38", sceMcRead);

INCLUDE_ASM(const s32, "code_002C6F38", sceMcWrite);

INCLUDE_ASM(const s32, "code_002C6F38", mcHearAlarm);

INCLUDE_ASM(const s32, "code_002C6F38", mcDelayThread);

INCLUDE_ASM(const s32, "code_002C6F38", sceMcSync);

INCLUDE_ASM(const s32, "code_002C6F38", mceGetInfoApdx);

INCLUDE_ASM(const s32, "code_002C6F38", sceMcGetInfo);

INCLUDE_ASM(const s32, "code_002C6F38", sceMcUdCheckNewCard);

INCLUDE_ASM(const s32, "code_002C6F38", sceMcGetDir);

INCLUDE_ASM(const s32, "code_002C6F38", mceStorePwd);

INCLUDE_ASM(const s32, "code_002C6F38", sceMcChdir);

INCLUDE_ASM(const s32, "code_002C6F38", sceMcFormat);

INCLUDE_ASM(const s32, "code_002C6F38", sceMcDelete);

INCLUDE_ASM(const s32, "code_002C6F38", sceMcFlush);

INCLUDE_ASM(const s32, "code_002C6F38", sceMcSetFileInfo);

INCLUDE_ASM(const s32, "code_002C6F38", sceMcRename);

INCLUDE_ASM(const s32, "code_002C6F38", sceMcUnformat);

INCLUDE_ASM(const s32, "code_002C6F38", sceMcGetEntSpace);

INCLUDE_ASM(const s32, "code_002C6F38", cosf);

INCLUDE_ASM(const s32, "code_002C6F38", sinf);

INCLUDE_ASM(const s32, "code_002C6F38", tanf);

INCLUDE_ASM(const s32, "code_002C6F38", __ieee754_acosf);

INCLUDE_ASM(const s32, "code_002C6F38", __ieee754_atan2f);

INCLUDE_ASM(const s32, "code_002C6F38", __ieee754_rem_pio2f);

INCLUDE_ASM(const s32, "code_002C6F38", __ieee754_sqrtf);

INCLUDE_ASM(const s32, "code_002C6F38", __kernel_cosf);

INCLUDE_ASM(const s32, "code_002C6F38", __kernel_rem_pio2f);

INCLUDE_ASM(const s32, "code_002C6F38", __kernel_sinf);

INCLUDE_ASM(const s32, "code_002C6F38", __kernel_tanf);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054AF60);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B274);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B348);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B358);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B3C0);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B3F8);

INCLUDE_ASM(const s32, "code_002C6F38", atanf);

INCLUDE_ASM(const s32, "code_002C6F38", fabsf);

INCLUDE_ASM(const s32, "code_002C6F38", floorf);

INCLUDE_ASM(const s32, "code_002C6F38", scalbnf);

INCLUDE_ASM(const s32, "code_002C6F38", copysignf);

void runAtExitList(void) {
    void (**p)(void);

    while (*D_0052BB28 != NULL) {
        p = D_0052BB28;
        D_0052BB28 = p + 1;
        (*p)();
    }
    if (D_0052BB2C == 0) {
        D_0052BB2C = 1;
        libc_2D2648(D_0052D940);
    }
}

INCLUDE_ASM(const s32, "code_002C6F38", initAtExit);

void initFloatOnce(void) {
    if (D_0059AD58 == 0) {
        D_0059AD58 = 1;
        initAtExit();
    }
}

long sdfFloatToULong(f32 x) {
    SdfUnpacked u;

    sdfUnpackFloat(&x, &u);
    return dptoul(u.kind, u.sign, u.exp, (long)u.mant << 30);
}

INCLUDE_ASM(const s32, "code_002C6F38", libc_2D1590);

long dpadd(double a, double b) {
    SdfUnpackedD ua;
    SdfUnpackedD ub;
    SdfUnpackedD r;

    sdfUnpackDouble(&a, &ua);
    sdfUnpackDouble(&b, &ub);
    return libc_2D34A0(libc_2D1590(&ua, &ub, &r));
}

long dpsub(double a, double b) {
    SdfUnpackedD ua;
    SdfUnpackedD ub;
    SdfUnpackedD r;

    sdfUnpackDouble(&a, &ua);
    sdfUnpackDouble(&b, &ub);
    ub.sign ^= 1;
    return libc_2D34A0(libc_2D1590(&ua, &ub, &r));
}

INCLUDE_ASM(const s32, "code_002C6F38", dpmul);

long dpdiv(double a, double b) {
    SdfUnpackedD ua;
    SdfUnpackedD ub;

    sdfUnpackDouble(&a, &ua);
    sdfUnpackDouble(&b, &ub);
    return libc_2D3638(&ua, &ub);
}

INCLUDE_ASM(const s32, "code_002C6F38", litodp);

INCLUDE_ASM(const s32, "code_002C6F38", dptoli);

long dptoul(s32 kind, s32 sign, s32 exp, long mant) {
    SdfUnpackedD u;

    u.kind = kind;
    u.sign = sign;
    u.exp = exp;
    u.mant = mant;
    return libc_2D34A0(&u);
}

u32 sdfDoubleToULong(double x) {
    SdfUnpackedD u;
    u32 m;

    sdfUnpackDouble(&x, &u);
    m = u.mant >> 30;
    if (u.mant & 0x3FFFFFFF) {
        m |= 1;
    }
    return sdfPackQuad(u.kind, u.sign, u.exp, m);
}

INCLUDE_ASM(const s32, "code_002C6F38", sdfDoubleToUInt);

u8 *libc_2D1E08(u8 *p, u32 *out) {
    u32 b;
    u32 v;
    s32 shift = 0;

    b = *p++;
    v = b & 0x7F;
    while (b & 0x80) {
        b = *p++;
        shift += 7;
        v |= (b & 0x7F) << shift;
    }
    *out = v;
    return p;
}

u8 *libc_2D1E50(u8 *p, s32 *out) {
    s32 shift = 0;
    u32 v = 0;
    u32 b;

    do {
        b = *p++;
        v |= (b & 0x7F) << shift;
        shift += 7;
    } while (b & 0x80);
    if ((u32)shift < 32 && (b & 0x40)) {
        v |= -1 << shift;
    }
    *out = v;
    return p;
}

INCLUDE_ASM(const s32, "code_002C6F38", libc_2D1EA8);

INCLUDE_ASM(const s32, "code_002C6F38", libc_2D1FA0);

s32 registerAtExit(void *fn, void *entry) {
    AtExitEntry *e = entry;

    e->fn = fn;
    e->unk04 = NULL;
    e->unk00 = NULL;
    e->unk0C = NULL;
    e->unk10 = NULL;
    WaitSema(D_0052C37C);
    e->next = D_0059AD60;
    D_0059AD60 = e;
    SignalSema(D_0052C37C);
    /* Empty asm: keeps the SignalSema call from becoming a sibcall. */
    __asm__("");
    return;
}

s32 libc_2D2580(void *fn) {
    return registerAtExit(fn, malloc(0x18));
}

s32 libc_2D25B0(void *fn, void *entry) {
    AtExitEntry *e = entry;

    e->fn = fn;
    e->unk0C = fn;
    e->unk04 = NULL;
    e->unk00 = NULL;
    e->unk10 = NULL;
    WaitSema(D_0052C37C);
    e->next = D_0059AD60;
    D_0059AD60 = e;
    SignalSema(D_0052C37C);
    /* Empty asm: keeps the SignalSema call from becoming a sibcall. */
    __asm__("");
    return;
}

s32 libc_2D2618(void *fn) {
    return libc_2D25B0(fn, malloc(0x18));
}

INCLUDE_ASM(const s32, "code_002C6F38", libc_2D2648);

void ctime_4(void *fn) {
    free(libc_2D2648(fn));
}

s32 libc_2D2728(u32 *p) {
    s32 n = 0;

    while (p[0] != 0) {
        if (p[1] != 0 && p[2] != 0) {
            n++;
        }
        p = (u32 *)((char *)p + p[0] + 4);
    }
    return n;
}

void libc_2D2768(u32 *p, AtExitList *list, u32 *pmin, u32 *pmax) {
    u32 lo = *pmin;
    u32 hi = *pmax;

    while (p[0] != 0) {
        if (p[1] != 0) {
            u32 a = p[2];

            if (a != 0) {
                if (list->entries != NULL) {
                    list->entries[list->count++] = p;
                }
                if (a < lo) {
                    lo = a;
                }
                if (hi < a + p[3]) {
                    hi = a + p[3];
                }
            }
        }
        p = (u32 *)((char *)p + p[0] + 4);
    }
    *pmin = lo;
    *pmax = hi;
}

void *libc_2D27F8(u32 *p, u32 addr) {
    while (p[0] != 0) {
        if (p[1] != 0 && p[2] != 0 && addr - p[2] < p[3]) {
            return p;
        }
        p = (u32 *)((char *)p + p[0] + 4);
    }
    return NULL;
}

INCLUDE_ASM(const s32, "code_002C6F38", libc_2D2850);

INCLUDE_ASM(const s32, "code_002C6F38", libc_2D29B8);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B408);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B414);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B418);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B460);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B480);

INCLUDE_ASM(const s32, "code_002C6F38", libc_2D2B60);

INCLUDE_ASM(const s32, "code_002C6F38", libc_2D2C70);

INCLUDE_ASM(const s32, "code_002C6F38", libc_2D2D90);

INCLUDE_ASM(const s32, "code_002C6F38", libc_2D31E0);

INCLUDE_ASM(const s32, "code_002C6F38", libc_2D3398);

INCLUDE_ASM(const s32, "code_002C6F38", sdfUnpackFloat);

f32 sdfPackQuad(s32 kind, s32 sign, s32 exp, u32 mant) {
    SdfUnpacked u;

    u.kind = kind;
    u.sign = sign;
    u.exp = exp;
    u.mant = mant;
    return sdfPackFloat(&u);
}

INCLUDE_ASM(const s32, "code_002C6F38", libc_2D34A0);

INCLUDE_ASM(const s32, "code_002C6F38", sdfUnpackDouble);

INCLUDE_ASM(const s32, "code_002C6F38", libc_2D3638);

INCLUDE_ASM(const s32, "code_002C6F38", sdfPackFloat);

void abort(void) {
    while (1) {
        raise(6);
        func_001001C8(1);
    }
}

s32 atexit(void (*fn)(void)) {
    _atexit *p;

    p = D_0052BE28->_atexit;
    if (p == NULL) {
        p = &D_0052BE28->_atexit0;
        D_0052BE28->_atexit = p;
    }
    if (p->_ind >= 32) {
        if ((p = malloc(sizeof(_atexit))) == NULL) {
            return -1;
        }
        p->_ind = 0;
        p->_next = D_0052BE28->_atexit;
        D_0052BE28->_atexit = p;
    }
    p->_fns[p->_ind++] = fn;
    return 0;
}

s32 atoi(const char *s) {
    return strtol(s, NULL, 10);
}

void *malloc(u32 size) {
    void *result;

    __malloc_lock(D_0052BE28);
    result = _malloc_r(D_0052BE28, size);
    __malloc_unlock(D_0052BE28);
    return result;
}

void free(void *ptr) {
    __malloc_lock(D_0052BE28);
    _free_r(D_0052BE28, ptr);
    __malloc_unlock(D_0052BE28);
}

INCLUDE_ASM(const s32, "code_002C6F38", gcc2_compiled_);

INCLUDE_ASM(const s32, "code_002C6F38", _malloc_r);

INCLUDE_ASM(const s32, "code_002C6F38", memcmp);

INCLUDE_ASM(const s32, "code_002C6F38", memcpy);

INCLUDE_ASM(const s32, "code_002C6F38", memmove);

INCLUDE_ASM(const s32, "code_002C6F38", memset);

void __malloc_lock(_reent *r) {
    s32 owner = GetThreadId();

    if (D_0052C288 == owner) {
        D_0052C28C++;
    } else {
        WaitSema(D_0052C378);
        D_0052C288 = owner;
        D_0052C28C++;
    }
}

void __malloc_unlock(_reent *r) {
    if (--D_0052C28C != 0) {
        return;
    }
    D_0052C288 = -1;
    SignalSema(D_0052C378);
}

s32 _printf_r(_reent *r, const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    return func_002D6678(r, r->_stdout, fmt, ap);
}

s32 printf(const char *fmt, ...) {
    _reent *r = D_0052BE28;
    va_list ap;

    va_start(ap, fmt);
    r->_stdout->_data = r;
    return vfprintf(r->_stdout, fmt, ap);
}

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B550);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B658);

INCLUDE_ASM(const s32, "code_002C6F38", _puts_r);

s32 puts(const char *s) {
    return _puts_r(D_0052BE28, s);
}

INCLUDE_ASM(const s32, "code_002C6F38", qsort);

void *_sbrk_r(_reent *r, s32 incr) {
    void *result;

    D_00552178 = 0;
    result = sbrk(incr);
    if (result == (void *)-1 && D_00552178 != 0) {
        r->_errno = D_00552178;
    }
    return result;
}

s32 _init_signal_r(_reent *r) {
    s32 i;

    if (r->_sig_func == NULL) {
        r->_sig_func = _malloc_r(r, 32 * sizeof(void *));
        if (r->_sig_func == NULL) {
            return -1;
        }
        for (i = 0; i < 32; i++) {
            r->_sig_func[i] = NULL;
        }
    }
    return 0;
}

void *_signal_r(_reent *r, s32 sig, void *handler) {
    void *old;

    if ((u32)sig >= 32) {
        r->_errno = 22;
        return (void *)-1;
    }
    if (r->_sig_func == NULL && _init_signal_r(r) != 0) {
        return (void *)-1;
    }
    old = (void *)r->_sig_func[sig];
    r->_sig_func[sig] = (void (*)(s32))handler;
    return old;
}

INCLUDE_ASM(const s32, "code_002C6F38", _raise_r);

s32 __sigtramp_r(_reent *r, s32 sig) {
    void (*handler)(s32 sig);

    if ((u32)sig >= 32) {
        return -1;
    }
    if (r->_sig_func == NULL && _init_signal_r(r) != 0) {
        return -1;
    }
    handler = r->_sig_func[sig];
    if (handler == NULL) {
        return 1;
    }
    if (handler == (void (*)(s32))-1) {
        return 2;
    }
    if (handler == (void (*)(s32))1) {
        return 3;
    }
    r->_sig_func[sig] = NULL;
    handler(sig);
    return 0;
}

s32 raise(s32 sig) {
    return _raise_r(D_0052BE28, sig);
}

void *signal(s32 sig, void *handler) {
    return _signal_r(D_0052BE28, sig, handler);
}

s32 _init_signal(void) {
    return _init_signal_r(D_0052BE28);
}

s32 __sigtramp(s32 sig) {
    return __sigtramp_r(D_0052BE28, sig);
}

s32 _kill_r(_reent *r, s32 pid, s32 sig) {
    s32 result;

    D_00552178 = 0;
    result = kill(pid, sig);
    if (result == -1 && D_00552178 != 0) {
        r->_errno = D_00552178;
    }
    return result;
}

s32 _getpid_r(_reent *r) {
    return libc_2DC670();
}

s32 _sprintf_r(_reent *r, char *str, const char *fmt, ...) {
    s32 result;
    FILE f;
    va_list ap;

    va_start(ap, fmt);
    f._flags = 0x208;
    f._bf._base = f._p = (u8 *)str;
    f._bf._size = f._w = 0x7FFFFFFF;
    f._data = r;
    result = vfprintf(&f, fmt, ap);
    *f._p = '\0';
    return result;
}

s32 sprintf(char *str, const char *fmt, ...) {
    s32 result;
    FILE f;
    va_list ap;

    va_start(ap, fmt);
    f._flags = 0x208;
    f._bf._base = f._p = (u8 *)str;
    f._bf._size = f._w = 0x7FFFFFFF;
    f._data = D_0052BE28;
    result = vfprintf(&f, fmt, ap);
    *f._p = '\0';
    return result;
}

INCLUDE_ASM(const s32, "code_002C6F38", strcasecmp);

INCLUDE_ASM(const s32, "code_002C6F38", strcat);

INCLUDE_ASM(const s32, "code_002C6F38", strchr);

INCLUDE_ASM(const s32, "code_002C6F38", strcmp);

INCLUDE_ASM(const s32, "code_002C6F38", strcpy);

INCLUDE_ASM(const s32, "code_002C6F38", strlen);

INCLUDE_ASM(const s32, "code_002C6F38", strncmp);

INCLUDE_ASM(const s32, "code_002C6F38", strncpy);

char *strrchr(const char *s, s32 c) {
    char ch = c;
    const char *last = NULL;

    while (*s != '\0') {
        if (*s == ch) {
            last = s;
        }
        s++;
    }
    return (char *)(*s != ch ? last : s);
}

INCLUDE_ASM(const s32, "code_002C6F38", strstr);

INCLUDE_ASM(const s32, "code_002C6F38", _strtol_r);

long strtol(const char *s, char **end, s32 base) {
    return _strtol_r(D_0052BE28, s, end, base);
}

s32 gcc2_compiled__0(FILE *fp, __suio *uio) {
    s32 err;

    if (uio->uio_resid == 0) {
        uio->uio_iovcnt = 0;
        return 0;
    }
    err = __sfvwrite(fp, uio);
    uio->uio_resid = 0;
    uio->uio_iovcnt = 0;
    return err;
}

INCLUDE_ASM(const s32, "code_002C6F38", __sbprintf);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B670);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B680);

INCLUDE_ASM(const s32, "code_002C6F38", vfprintf);

INCLUDE_ASM(const s32, "code_002C6F38", cvt);

INCLUDE_ASM(const s32, "code_002C6F38", exponent);

s32 vsprintf(char *str, const char *fmt, va_list ap) {
    s32 result;
    FILE f;

    f._flags = 0x208;
    f._bf._base = f._p = (u8 *)str;
    f._bf._size = f._w = 0x7FFFFFFF;
    f._data = D_0052BE28;
    result = vfprintf(&f, fmt, ap);
    *f._p = '\0';
    return result;
}

s32 vsprintf_r(_reent *r, char *str, const char *fmt, va_list ap) {
    s32 result;
    FILE f;

    f._flags = 0x208;
    f._bf._base = f._p = (u8 *)str;
    f._bf._size = f._w = 0x7FFFFFFF;
    f._data = r;
    result = vfprintf(&f, fmt, ap);
    *f._p = '\0';
    return result;
}

INCLUDE_ASM(const s32, "code_002C6F38", __swsetup);

INCLUDE_ASM(const s32, "code_002C6F38", gcc2_compiled__1);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B880);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B890);

INCLUDE_ASM(const s32, "code_002C6F38", _dtoa_r);

INCLUDE_ASM(const s32, "code_002C6F38", fflush);

void gcc2_compiled__2(FILE *fp, s32 flags, s32 file, _reent *data) {
    fp->_p = NULL;
    fp->_r = 0;
    fp->_w = 0;
    fp->_flags = flags;
    fp->_file = file;
    fp->_bf._base = NULL;
    fp->_bf._size = 0;
    fp->_lbfsize = 0;
    fp->_cookie = fp;
    fp->_read = __sread;
    fp->_write = __swrite;
    fp->_seek = __sseek;
    fp->_close = __sclose;
    fp->_data = data;
}

_glue *__sfmoreglue(_reent *r, s32 n) {
    _glue *g;
    FILE *p;

    g = _malloc_r(r, sizeof(_glue) + n * sizeof(FILE));
    if (g == NULL) {
        return NULL;
    }
    p = (FILE *)(g + 1);
    g->next = NULL;
    g->niobs = n;
    g->iobs = p;
    memset(p, 0, n * sizeof(FILE));
    return g;
}

INCLUDE_ASM(const s32, "code_002C6F38", __sfp);

void _cleanup_r(_reent *r) {
    _fwalk(r, fflush);
}

void _cleanup(void) {
    _cleanup_r(D_0052BE28);
}

void __sinit(_reent *r) {
    r->__cleanup = _cleanup_r;
    r->__sdidinit = 1;
    gcc2_compiled__2(&r->__sf[0], 4, 0, r);
    gcc2_compiled__2(&r->__sf[1], 9, 1, r);
    gcc2_compiled__2(&r->__sf[2], 0xA, 2, r);
    r->__sglue.next = NULL;
    r->__sglue.niobs = 3;
    r->__sglue.iobs = &r->__sf[0];
}

INCLUDE_ASM(const s32, "code_002C6F38", _free_r);

INCLUDE_ASM(const s32, "code_002C6F38", _malloc_trim_r);

INCLUDE_ASM(const s32, "code_002C6F38", __sfvwrite);

s32 _fwalk(_reent *r, s32 (*func)(void *)) {
    _glue *g;
    FILE *fp;
    s32 n;
    s32 ret = 0;

    for (g = &r->__sglue; g != NULL; g = g->next) {
        for (fp = g->iobs, n = g->niobs; --n >= 0; fp++) {
            if (fp->_flags != 0) {
                ret |= func(fp);
            }
        }
    }
    return ret;
}

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B8A0);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B8A8);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B8B0);

INCLUDE_RODATA(const s32, "code_002C6F38", jtbl_0054B8C0);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B8D8);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B908);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B910);

INCLUDE_ASM(const s32, "code_002C6F38", _setlocale_r);

void *func_002DA308(_reent *r) {
    return D_0054B8D8;
}

s32 setlocale(s32 category, const char *locale) {
    return _setlocale_r(D_0052BE28, category, locale);
}

void *localeconv(void) {
    return func_002DA308(D_0052BE28);
}

INCLUDE_ASM(const s32, "code_002C6F38", __smakebuf);

s32 _mbtowc_r(_reent *r, s32 *pwc, const char *s, u32 n, s32 *state) {
    s32 dummy;
    s32 *out = pwc;

    if (pwc == NULL) {
        out = &dummy;
    }
    if (s == NULL) {
        return 0;
    }
    if (n == 0) {
        return -1;
    }
    *out = *(u8 *)s;
    if (*(u8 *)s == 0) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_002C6F38", memchr);

_Bigint *_Balloc(_reent *r, s32 k) {
    _Bigint *rv;
    s32 x;

    if (r->_freelist == NULL) {
        r->_freelist = _calloc_r(r, 4, 16);
        if (r->_freelist == NULL) {
            return NULL;
        }
    }
    rv = r->_freelist[k];
    if (rv != NULL) {
        r->_freelist[k] = rv->next;
    } else {
        x = 1 << k;
        rv = _calloc_r(r, 1, 0x14 + x * sizeof(u32));
        if (rv == NULL) {
            return NULL;
        }
        rv->k = k;
        rv->maxwds = x;
    }
    rv->sign = rv->wds = 0;
    return rv;
}

void _Bfree(_reent *r, _Bigint *v) {
    if (v != NULL) {
        v->next = r->_freelist[v->k];
        r->_freelist[v->k] = v;
    }
}

INCLUDE_ASM(const s32, "code_002C6F38", _multadd);

INCLUDE_ASM(const s32, "code_002C6F38", _s2b);

s32 _hi0bits(u32 x) {
    s32 k = 0;

    if ((x & 0xFFFF0000) == 0) {
        k = 16;
        x <<= 16;
    }
    if ((x & 0xFF000000) == 0) {
        k += 8;
        x <<= 8;
    }
    if ((x & 0xF0000000) == 0) {
        k += 4;
        x <<= 4;
    }
    if ((x & 0xC0000000) == 0) {
        k += 2;
        x <<= 2;
    }
    if ((x & 0x80000000) == 0) {
        k++;
        if ((x & 0x40000000) == 0) {
            return 32;
        }
    }
    return k;
}

s32 _lo0bits(u32 *y) {
    s32 k;
    u32 x = *y;

    if (x & 7) {
        if (x & 1) {
            return 0;
        }
        if (x & 2) {
            *y = x >> 1;
            return 1;
        }
        *y = x >> 2;
        return 2;
    }
    k = 0;
    if ((x & 0xFFFF) == 0) {
        k = 16;
        x >>= 16;
    }
    if ((x & 0xFF) == 0) {
        k += 8;
        x >>= 8;
    }
    if ((x & 0xF) == 0) {
        k += 4;
        x >>= 4;
    }
    if ((x & 3) == 0) {
        k += 2;
        x >>= 2;
    }
    if ((x & 1) == 0) {
        k++;
        x >>= 1;
        if (x == 0) {
            return 32;
        }
    }
    *y = x;
    return k;
}

_Bigint *_i2b(_reent *r, s32 i) {
    _Bigint *b = _Balloc(r, 1);

    b->x[0] = i;
    b->wds = 1;
    return b;
}

INCLUDE_ASM(const s32, "code_002C6F38", _multiply);

INCLUDE_ASM(const s32, "code_002C6F38", _pow5mult);

INCLUDE_ASM(const s32, "code_002C6F38", _lshift);

s32 __mcmp(_Bigint *a, _Bigint *b) {
    u32 *xa, *xa0, *xb, *xb0;
    s32 i, j;

    i = a->wds;
    j = b->wds;
    if ((i -= j) != 0) {
        return i;
    }
    xa0 = a->x;
    xa = xa0 + j;
    xb0 = b->x;
    xb = xb0 + j;
    for (;;) {
        if (*--xa != *--xb) {
            return *xa < *xb ? -1 : 1;
        }
        if (xa <= xa0) {
            break;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_002C6F38", __mdiff);

INCLUDE_ASM(const s32, "code_002C6F38", _ulp);

INCLUDE_ASM(const s32, "code_002C6F38", _b2d);

INCLUDE_ASM(const s32, "code_002C6F38", _d2b);

INCLUDE_ASM(const s32, "code_002C6F38", _ratio);

double _mprec_log10(s32 dig) {
    double v = 1.0;

    if (dig < 24) {
        return D_0054B930[dig];
    }
    while (dig > 0) {
        v *= 10.0;
        dig--;
    }
    return v;
}

INCLUDE_ASM(const s32, "code_002C6F38", isinf);

INCLUDE_ASM(const s32, "code_002C6F38", isnan);

s32 __sread(void *cookie, char *buf, s32 n) {
    FILE *fp = cookie;
    s32 result;

    result = _read_r(fp->_data, fp->_file, buf, n);
    if (result >= 0) {
        fp->_offset += result;
    } else {
        fp->_flags &= ~0x1000;
    }
    return result;
}

s32 __swrite(void *cookie, const char *buf, s32 n) {
    FILE *fp = cookie;

    if (fp->_flags & 0x100) {
        _lseek_r(fp->_data, fp->_file, 0, 2);
    }
    fp->_flags &= ~0x1000;
    return _write_r(fp->_data, fp->_file, buf, n);
}

long __sseek(void *cookie, s32 offset, s32 whence) {
    FILE *fp = cookie;
    long result;

    if ((result = _lseek_r(fp->_data, fp->_file, offset, whence)) == -1) {
        fp->_flags &= ~0x1000;
    } else {
        fp->_flags |= 0x1000;
        fp->_offset = result;
    }
    return result;
}

s32 __sclose(void *cookie) {
    FILE *fp = cookie;

    return _close_r(fp->_data, fp->_file);
}

long _write_r(_reent *r, s32 fd, const char *buf, s32 n) {
    long result;

    D_00552178 = 0;
    if ((result = write(fd, buf, n)) == -1 && D_00552178 != 0) {
        r->_errno = D_00552178;
    }
    return result;
}

INCLUDE_ASM(const s32, "code_002C6F38", _calloc_r);

s32 _close_r(_reent *r, s32 fd) {
    s32 result;

    D_00552178 = 0;
    result = close(fd);
    if (result == -1 && D_00552178 != 0) {
        r->_errno = D_00552178;
    }
    return result;
}

s32 _fstat_r(_reent *r, s32 fd, void *buf) {
    s32 result;

    D_00552178 = 0;
    result = fstat(fd, buf);
    if (result == -1 && D_00552178 != 0) {
        r->_errno = D_00552178;
    }
    return result;
}

long _lseek_r(_reent *r, s32 fd, s32 offset, s32 whence) {
    long result;

    D_00552178 = 0;
    result = lseek(fd, offset, whence);
    if (result == -1 && D_00552178 != 0) {
        r->_errno = D_00552178;
    }
    return result;
}

long _read_r(_reent *r, s32 fd, char *buf, s32 n) {
    long result;

    D_00552178 = 0;
    if ((result = read(fd, buf, n)) == -1 && D_00552178 != 0) {
        r->_errno = D_00552178;
    }
    return result;
}

/* The handwritten syscall stubs below start 16 bytes past the end of the C
   code, so the padding has to come from here. */
__asm__("nop\nnop\nnop\nnop\n");

INCLUDE_ASM(const s32, "code_002C6F38", RFU000_FullReset);

INCLUDE_ASM(const s32, "code_002C6F38", ResetEE);

INCLUDE_ASM(const s32, "code_002C6F38", libc_2DBAA0);

INCLUDE_ASM(const s32, "code_002C6F38", RFU003);

INCLUDE_ASM(const s32, "code_002C6F38", libc_2DBAC0);

INCLUDE_ASM(const s32, "code_002C6F38", RFU005);

INCLUDE_ASM(const s32, "code_002C6F38", libc_2DBAE0);

INCLUDE_ASM(const s32, "code_002C6F38", _ExecPS2);

INCLUDE_ASM(const s32, "code_002C6F38", RFU008);

INCLUDE_ASM(const s32, "code_002C6F38", RFU009);

INCLUDE_ASM(const s32, "code_002C6F38", AddSbusIntcHandler);

INCLUDE_ASM(const s32, "code_002C6F38", RemoveSbusIntcHandler);

INCLUDE_ASM(const s32, "code_002C6F38", Interrupt2Iop);

INCLUDE_ASM(const s32, "code_002C6F38", SetVTLBRefillHandler);

INCLUDE_ASM(const s32, "code_002C6F38", SetVCommonHandler);

INCLUDE_ASM(const s32, "code_002C6F38", SetVInterruptHandler);

INCLUDE_ASM(const s32, "code_002C6F38", AddIntcHandler);

INCLUDE_ASM(const s32, "code_002C6F38", AddIntcHandler2);

INCLUDE_ASM(const s32, "code_002C6F38", RemoveIntcHandler);

INCLUDE_ASM(const s32, "code_002C6F38", AddDmacHandler);

INCLUDE_ASM(const s32, "code_002C6F38", AddDmacHandler2);

INCLUDE_ASM(const s32, "code_002C6F38", RemoveDmacHandler);

INCLUDE_ASM(const s32, "code_002C6F38", _EnableIntc);

INCLUDE_ASM(const s32, "code_002C6F38", _DisableIntc);

INCLUDE_ASM(const s32, "code_002C6F38", _EnableDmac);

INCLUDE_ASM(const s32, "code_002C6F38", _DisableDmac);

INCLUDE_ASM(const s32, "code_002C6F38", SetAlarm);

INCLUDE_ASM(const s32, "code_002C6F38", ReleaseAlarm);

INCLUDE_ASM(const s32, "code_002C6F38", _iEnableIntc);

INCLUDE_ASM(const s32, "code_002C6F38", _iDisableIntc);

INCLUDE_ASM(const s32, "code_002C6F38", _iEnableDmac);

INCLUDE_ASM(const s32, "code_002C6F38", _iDisableDmac);

INCLUDE_ASM(const s32, "code_002C6F38", iSetAlarm);

INCLUDE_ASM(const s32, "code_002C6F38", iReleaseAlarm);

INCLUDE_ASM(const s32, "code_002C6F38", CreateThread);

INCLUDE_ASM(const s32, "code_002C6F38", libc_2DBCB0);

INCLUDE_ASM(const s32, "code_002C6F38", StartThread);

INCLUDE_ASM(const s32, "code_002C6F38", ExitThread);

INCLUDE_ASM(const s32, "code_002C6F38", ExitDeleteThread);

INCLUDE_ASM(const s32, "code_002C6F38", TerminateThread);

INCLUDE_ASM(const s32, "code_002C6F38", iTerminateThread);

INCLUDE_ASM(const s32, "code_002C6F38", DisableDispatchThread);

INCLUDE_ASM(const s32, "code_002C6F38", EnableDispatchThread);

INCLUDE_ASM(const s32, "code_002C6F38", ChangeThreadPriority);

INCLUDE_ASM(const s32, "code_002C6F38", iChangeThreadPriority);

INCLUDE_ASM(const s32, "code_002C6F38", RotateThreadReadyQueue);

INCLUDE_ASM(const s32, "code_002C6F38", _iRotateThreadReadyQueue);

INCLUDE_ASM(const s32, "code_002C6F38", ReleaseWaitThread);

INCLUDE_ASM(const s32, "code_002C6F38", iReleaseWaitThread);

INCLUDE_ASM(const s32, "code_002C6F38", GetThreadId);

INCLUDE_ASM(const s32, "code_002C6F38", ReferThreadStatus);

INCLUDE_ASM(const s32, "code_002C6F38", iReferThreadStatus);

INCLUDE_ASM(const s32, "code_002C6F38", SleepThread);

INCLUDE_ASM(const s32, "code_002C6F38", WakeupThread);

INCLUDE_ASM(const s32, "code_002C6F38", _iWakeupThread);

INCLUDE_ASM(const s32, "code_002C6F38", CancelWakeupThread);

INCLUDE_ASM(const s32, "code_002C6F38", iCancelWakeupThread);

INCLUDE_ASM(const s32, "code_002C6F38", SuspendThread);

INCLUDE_ASM(const s32, "code_002C6F38", _iSuspendThread);

INCLUDE_ASM(const s32, "code_002C6F38", ResumeThread);

INCLUDE_ASM(const s32, "code_002C6F38", iResumeThread);

INCLUDE_ASM(const s32, "code_002C6F38", JoinThread);

INCLUDE_ASM(const s32, "code_002C6F38", RFU060);

INCLUDE_ASM(const s32, "code_002C6F38", RFU061);

INCLUDE_ASM(const s32, "code_002C6F38", EndOfHeap);

INCLUDE_ASM(const s32, "code_002C6F38", RFU063);

INCLUDE_ASM(const s32, "code_002C6F38", CreateSema);

INCLUDE_ASM(const s32, "code_002C6F38", DeleteSema);

INCLUDE_ASM(const s32, "code_002C6F38", SignalSema);

INCLUDE_ASM(const s32, "code_002C6F38", iSignalSema);

INCLUDE_ASM(const s32, "code_002C6F38", WaitSema);

INCLUDE_ASM(const s32, "code_002C6F38", PollSema);

INCLUDE_ASM(const s32, "code_002C6F38", iPollSema);

INCLUDE_ASM(const s32, "code_002C6F38", ReferSemaStatus);

INCLUDE_ASM(const s32, "code_002C6F38", iReferSemaStatus);

INCLUDE_ASM(const s32, "code_002C6F38", RFU073);

INCLUDE_ASM(const s32, "code_002C6F38", SetOsdConfigParam);

INCLUDE_ASM(const s32, "code_002C6F38", GetOsdConfigParam);

INCLUDE_ASM(const s32, "code_002C6F38", GetGsHParam);

INCLUDE_ASM(const s32, "code_002C6F38", GetGsVParam);

INCLUDE_ASM(const s32, "code_002C6F38", SetGsHParam);

INCLUDE_ASM(const s32, "code_002C6F38", SetGsVParam);

INCLUDE_ASM(const s32, "code_002C6F38", RFU080_CreateEventFlag);

INCLUDE_ASM(const s32, "code_002C6F38", RFU081_DeleteEventFlag);

INCLUDE_ASM(const s32, "code_002C6F38", RFU082_SetEventFlag);

INCLUDE_ASM(const s32, "code_002C6F38", RFU083_iSetEventFlag);

INCLUDE_ASM(const s32, "code_002C6F38", RFU084_ClearEventFlag);

INCLUDE_ASM(const s32, "code_002C6F38", RFU085_iClearEventFlag);

INCLUDE_ASM(const s32, "code_002C6F38", RFU086_WaitEvnetFlag);

INCLUDE_ASM(const s32, "code_002C6F38", RFU087_PollEvnetFlag);

INCLUDE_ASM(const s32, "code_002C6F38", RFU088_iPollEvnetFlag);

INCLUDE_ASM(const s32, "code_002C6F38", RFU089_ReferEventFlagStatus);

INCLUDE_ASM(const s32, "code_002C6F38", RFU090_iReferEventFlagStatus);

INCLUDE_ASM(const s32, "code_002C6F38", RFU091);

INCLUDE_ASM(const s32, "code_002C6F38", EnableIntcHandler);

INCLUDE_ASM(const s32, "code_002C6F38", iEnableIntcHandler);

INCLUDE_ASM(const s32, "code_002C6F38", DisableIntcHandler);

INCLUDE_ASM(const s32, "code_002C6F38", iDisableIntcHandler);

INCLUDE_ASM(const s32, "code_002C6F38", EnableDmacHandler);

INCLUDE_ASM(const s32, "code_002C6F38", iEnableDmacHandler);

INCLUDE_ASM(const s32, "code_002C6F38", DisableDmacHandler);

INCLUDE_ASM(const s32, "code_002C6F38", iDisableDmacHandler);

INCLUDE_ASM(const s32, "code_002C6F38", KSeg0);

INCLUDE_ASM(const s32, "code_002C6F38", EnableCache);

INCLUDE_ASM(const s32, "code_002C6F38", DisableCache);

INCLUDE_ASM(const s32, "code_002C6F38", GetCop0);

INCLUDE_ASM(const s32, "code_002C6F38", FlushCache);

INCLUDE_ASM(const s32, "code_002C6F38", CpuConfig);

INCLUDE_ASM(const s32, "code_002C6F38", iGetCop0);

INCLUDE_ASM(const s32, "code_002C6F38", iFlushCache);

INCLUDE_ASM(const s32, "code_002C6F38", iCpuConfig);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifStopDma);

INCLUDE_ASM(const s32, "code_002C6F38", SetCPUTimerHandler);

INCLUDE_ASM(const s32, "code_002C6F38", SetCPUTimer);

INCLUDE_ASM(const s32, "code_002C6F38", SetOsdConfigParam2);

INCLUDE_ASM(const s32, "code_002C6F38", GetOsdConfigParam2);

INCLUDE_ASM(const s32, "code_002C6F38", GsGetIMR);

INCLUDE_ASM(const s32, "code_002C6F38", iGsGetIMR);

INCLUDE_ASM(const s32, "code_002C6F38", GsPutIMR);

INCLUDE_ASM(const s32, "code_002C6F38", iGsPutIMR);

INCLUDE_ASM(const s32, "code_002C6F38", SetPgifHandler);

INCLUDE_ASM(const s32, "code_002C6F38", SetVSyncFlag);

INCLUDE_ASM(const s32, "code_002C6F38", RFU116);

INCLUDE_ASM(const s32, "code_002C6F38", _print);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifDmaStat);

INCLUDE_ASM(const s32, "code_002C6F38", isceSifDmaStat);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifSetDma);

INCLUDE_ASM(const s32, "code_002C6F38", isceSifSetDma);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifSetDChain);

INCLUDE_ASM(const s32, "code_002C6F38", isceSifSetDChain);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifSetReg);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifGetReg);

INCLUDE_ASM(const s32, "code_002C6F38", libc_2DC2C0);

INCLUDE_ASM(const s32, "code_002C6F38", Deci2Call);

INCLUDE_ASM(const s32, "code_002C6F38", PSMode);

INCLUDE_ASM(const s32, "code_002C6F38", MachineType);

INCLUDE_ASM(const s32, "code_002C6F38", GetMemorySize);

INCLUDE_ASM(const s32, "code_002C6F38", _InitTLB);

INCLUDE_ASM(const s32, "code_002C6F38", sceResetttyinit);

INCLUDE_ASM(const s32, "code_002C6F38", VSync);

INCLUDE_ASM(const s32, "code_002C6F38", VSync2);

INCLUDE_ASM(const s32, "code_002C6F38", write);

INCLUDE_ASM(const s32, "code_002C6F38", read);

INCLUDE_ASM(const s32, "code_002C6F38", open);

INCLUDE_ASM(const s32, "code_002C6F38", close);

INCLUDE_ASM(const s32, "code_002C6F38", ioctl);

INCLUDE_ASM(const s32, "code_002C6F38", lseek);

INCLUDE_ASM(const s32, "code_002C6F38", sbrk);

INCLUDE_ASM(const s32, "code_002C6F38", isatty);

INCLUDE_ASM(const s32, "code_002C6F38", fstat);

INCLUDE_ASM(const s32, "code_002C6F38", libc_2DC670);

INCLUDE_ASM(const s32, "code_002C6F38", kill);

INCLUDE_ASM(const s32, "code_002C6F38", stat);

INCLUDE_ASM(const s32, "code_002C6F38", unlink);

INCLUDE_ASM(const s32, "code_002C6F38", _sceSDC);

INCLUDE_ASM(const s32, "code_002C6F38", SyncDCache);

INCLUDE_ASM(const s32, "code_002C6F38", iSyncDCache);

INCLUDE_ASM(const s32, "code_002C6F38", _sceIDC);

INCLUDE_ASM(const s32, "code_002C6F38", InvalidDCache);

INCLUDE_ASM(const s32, "code_002C6F38", iInvalidDCache);

INCLUDE_ASM(const s32, "code_002C6F38", func_002DC970);

INCLUDE_ASM(const s32, "code_002C6F38", DisableIntc);

INCLUDE_ASM(const s32, "code_002C6F38", EnableIntc);

INCLUDE_ASM(const s32, "code_002C6F38", DisableDmac);

INCLUDE_ASM(const s32, "code_002C6F38", EnableDmac);

INCLUDE_ASM(const s32, "code_002C6F38", iEnableIntc);

INCLUDE_ASM(const s32, "code_002C6F38", iDisableIntc);

INCLUDE_ASM(const s32, "code_002C6F38", iEnableDmac);

INCLUDE_ASM(const s32, "code_002C6F38", iDisableDmac);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B91C);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B928);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B930);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054B9F8);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054BA18);

INCLUDE_ASM(const s32, "code_002C6F38", topThread);

INCLUDE_ASM(const s32, "code_002C6F38", InitThread);

INCLUDE_ASM(const s32, "code_002C6F38", iWakeupThread);

INCLUDE_ASM(const s32, "code_002C6F38", iRotateThreadReadyQueue);

INCLUDE_ASM(const s32, "code_002C6F38", iSuspendThread);

INCLUDE_ASM(const s32, "code_002C6F38", gcc2_compiled__10);

INCLUDE_ASM(const s32, "code_002C6F38", QueuePeekWriteDone);

INCLUDE_ASM(const s32, "code_002C6F38", QueuePeekReadDone);

INCLUDE_ASM(const s32, "code_002C6F38", sceTtyHandler);

INCLUDE_ASM(const s32, "code_002C6F38", sceTtyWrite);

INCLUDE_ASM(const s32, "code_002C6F38", sceTtyRead);

INCLUDE_ASM(const s32, "code_002C6F38", sceTtyInit);

INCLUDE_ASM(const s32, "code_002C6F38", kputchar);

INCLUDE_ASM(const s32, "code_002C6F38", deci2Putchar);

INCLUDE_ASM(const s32, "code_002C6F38", serialPutchar);

INCLUDE_ASM(const s32, "code_002C6F38", ftoi);

INCLUDE_ASM(const s32, "code_002C6F38", printfloat);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054BAF8);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054BB00);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054BB08);

INCLUDE_ASM(const s32, "code_002C6F38", _printf);

INCLUDE_ASM(const s32, "code_002C6F38", kprintf);

INCLUDE_ASM(const s32, "code_002C6F38", scePrintf);

INCLUDE_ASM(const s32, "code_002C6F38", gcc2_compiled__12);

INCLUDE_ASM(const s32, "code_002C6F38", _change_addr);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifGetSreg);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifSetSreg);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifGetDataTable);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifInitCmd);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifExitCmd);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifSetCmdBuffer);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifSetSysCmdBuffer);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifAddCmdHandler);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifRemoveCmdHandler);

INCLUDE_ASM(const s32, "code_002C6F38", _sceSifSendCmd);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifSendCmd);

INCLUDE_ASM(const s32, "code_002C6F38", isceSifSendCmd);

INCLUDE_ASM(const s32, "code_002C6F38", _sceSifCmdIntrHdlr);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifWriteBackDCache);

INCLUDE_ASM(const s32, "code_002C6F38", func_002DE504);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifInitRpc);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifExitRpc);

INCLUDE_ASM(const s32, "code_002C6F38", _sceRpcGetPacket_0);

INCLUDE_ASM(const s32, "code_002C6F38", _sceRpcFreePacket_0);

INCLUDE_ASM(const s32, "code_002C6F38", _sceRpcGetFPacket);

INCLUDE_ASM(const s32, "code_002C6F38", _sceRpcGetFPacket2);

INCLUDE_ASM(const s32, "code_002C6F38", _request_end_0);

INCLUDE_ASM(const s32, "code_002C6F38", _request_rdata);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifGetOtherData);

INCLUDE_ASM(const s32, "code_002C6F38", _search_svdata);

INCLUDE_ASM(const s32, "code_002C6F38", _request_bind);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifBindRpc);

INCLUDE_ASM(const s32, "code_002C6F38", _request_call);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifCallRpc);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifCheckStatRpc);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifSetRpcQueue);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifRegisterRpc);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifRemoveRpc);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifRemoveRpcQueue);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifGetNextRequest);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifExecRequest);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifRpcLoop);

INCLUDE_ASM(const s32, "code_002C6F38", gcc2_compiled__13);

INCLUDE_ASM(const s32, "code_002C6F38", new_iob);

INCLUDE_ASM(const s32, "code_002C6F38", get_iob);

INCLUDE_ASM(const s32, "code_002C6F38", _sceFs_Rcv_Intr);

INCLUDE_ASM(const s32, "code_002C6F38", _sceFsSemInit);

INCLUDE_ASM(const s32, "code_002C6F38", _sceFsWaitS);

INCLUDE_ASM(const s32, "code_002C6F38", _sceFsSigSema);

INCLUDE_ASM(const s32, "code_002C6F38", scePowerOffHandler);

INCLUDE_ASM(const s32, "code_002C6F38", _sceFs_Poff_Intr);

INCLUDE_ASM(const s32, "code_002C6F38", sceFsInit);

INCLUDE_ASM(const s32, "code_002C6F38", _fs_version);

INCLUDE_ASM(const s32, "code_002C6F38", sceFsReset);

INCLUDE_ASM(const s32, "code_002C6F38", sceOpen);

INCLUDE_ASM(const s32, "code_002C6F38", sceClose);

INCLUDE_ASM(const s32, "code_002C6F38", sceLseek);

INCLUDE_ASM(const s32, "code_002C6F38", sceRead);

INCLUDE_ASM(const s32, "code_002C6F38", sceWrite);

INCLUDE_ASM(const s32, "code_002C6F38", sceIoctl);

INCLUDE_ASM(const s32, "code_002C6F38", sceIoctl2);

INCLUDE_ASM(const s32, "code_002C6F38", _sceCallCode);

INCLUDE_ASM(const s32, "code_002C6F38", sceRemove);

INCLUDE_ASM(const s32, "code_002C6F38", sceMkdir);

INCLUDE_ASM(const s32, "code_002C6F38", sceRmdir);

INCLUDE_ASM(const s32, "code_002C6F38", sceFormat);

INCLUDE_ASM(const s32, "code_002C6F38", sceAddDrv);

INCLUDE_ASM(const s32, "code_002C6F38", sceDelDrv);

INCLUDE_ASM(const s32, "code_002C6F38", sceDopen);

INCLUDE_ASM(const s32, "code_002C6F38", sceDclose);

INCLUDE_ASM(const s32, "code_002C6F38", sceDread);

INCLUDE_ASM(const s32, "code_002C6F38", sceGetstat);

INCLUDE_ASM(const s32, "code_002C6F38", sceChstat);

INCLUDE_ASM(const s32, "code_002C6F38", sceRename);

INCLUDE_ASM(const s32, "code_002C6F38", sceChdir);

INCLUDE_ASM(const s32, "code_002C6F38", sceSync);

INCLUDE_ASM(const s32, "code_002C6F38", sceMount);

INCLUDE_ASM(const s32, "code_002C6F38", sceUmount);

INCLUDE_ASM(const s32, "code_002C6F38", sceLseek64);

INCLUDE_ASM(const s32, "code_002C6F38", sceDevctl);

INCLUDE_ASM(const s32, "code_002C6F38", sceSymlink);

INCLUDE_ASM(const s32, "code_002C6F38", sceReadlink);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifInitIopHeap);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifAllocIopHeap);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifAllocSysMemory);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifFreeSysMemory);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifFreeIopHeap);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifLoadIopHeap);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifQueryMemSize);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifQueryMaxFreeMemSize);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifQueryTotalFreeMemSize);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifQueryBlockTopAddress);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifQueryBlockSize);

INCLUDE_ASM(const s32, "code_002C6F38", _lf_bind);

INCLUDE_ASM(const s32, "code_002C6F38", _lf_version);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifLoadFileReset);

INCLUDE_ASM(const s32, "code_002C6F38", _sceSifLoadModuleBuffer);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifStopModule);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifUnloadModule);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifSearchModuleByName);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifSearchModuleByAddress);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifLoadModuleBuffer);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifLoadStartModuleBuffer);

INCLUDE_ASM(const s32, "code_002C6F38", _sceSifLoadModule);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifLoadModule);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifLoadStartModule);

INCLUDE_ASM(const s32, "code_002C6F38", _sceSifLoadElfPart);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifLoadElfPart);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054BCA8);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054BCB0);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifLoadElf);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifGetIopAddr);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifSetIopAddr);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifResetIop);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifIsAliveIop);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifSyncIop);

INCLUDE_ASM(const s32, "code_002C6F38", sceSifRebootIop);

INCLUDE_ASM(const s32, "code_002C6F38", DIntr);

INCLUDE_ASM(const s32, "code_002C6F38", EIntr);

INCLUDE_ASM(const s32, "code_002C6F38", supplement_crt0);

INCLUDE_ASM(const s32, "code_002C6F38", Copy);

INCLUDE_ASM(const s32, "code_002C6F38", kCopy);

INCLUDE_ASM(const s32, "code_002C6F38", kFindAddress);

INCLUDE_ASM(const s32, "code_002C6F38", FindAddress);

INCLUDE_ASM(const s32, "code_002C6F38", GetSystemCallEntry);

INCLUDE_ASM(const s32, "code_002C6F38", GetSystemCallTableEntry);

INCLUDE_ASM(const s32, "code_002C6F38", setup);

INCLUDE_ASM(const s32, "code_002C6F38", _InitSys);

INCLUDE_ASM(const s32, "code_002C6F38", gcc2_compiled__15);

INCLUDE_ASM(const s32, "code_002C6F38", Copy_0);

INCLUDE_ASM(const s32, "code_002C6F38", kCopy_0);

INCLUDE_ASM(const s32, "code_002C6F38", GetEntryAddress);

INCLUDE_ASM(const s32, "code_002C6F38", PatchIsNeeded_0);

INCLUDE_ASM(const s32, "code_002C6F38", InitExecPS2);

INCLUDE_ASM(const s32, "code_002C6F38", gcc2_compiled__16);

INCLUDE_ASM(const s32, "code_002C6F38", kCopy_1);

INCLUDE_ASM(const s32, "code_002C6F38", PatchIsNeeded);

INCLUDE_ASM(const s32, "code_002C6F38", SetArg);

INCLUDE_ASM(const s32, "code_002C6F38", TerminateLibrary);

INCLUDE_ASM(const s32, "code_002C6F38", ExecPS2);

INCLUDE_ASM(const s32, "code_002C6F38", LoadExecPS2);

INCLUDE_ASM(const s32, "code_002C6F38", Exit);

INCLUDE_ASM(const s32, "code_002C6F38", ExecOSD);

INCLUDE_ASM(const s32, "code_002C6F38", gcc2_compiled__17);

INCLUDE_ASM(const s32, "code_002C6F38", Copy_1);

INCLUDE_ASM(const s32, "code_002C6F38", kCopy_2);

INCLUDE_ASM(const s32, "code_002C6F38", GetEntryAddress_0);

INCLUDE_ASM(const s32, "code_002C6F38", InitAlarm);

INCLUDE_ASM(const s32, "code_002C6F38", sceDeci2Open);

INCLUDE_ASM(const s32, "code_002C6F38", sceDeci2Close);

INCLUDE_ASM(const s32, "code_002C6F38", sceDeci2ReqSend);

INCLUDE_ASM(const s32, "code_002C6F38", sceDeci2Poll);

INCLUDE_ASM(const s32, "code_002C6F38", sceDeci2ExRecv);

INCLUDE_ASM(const s32, "code_002C6F38", sceDeci2ExSend);

INCLUDE_ASM(const s32, "code_002C6F38", sceDeci2ExReqSend);

INCLUDE_ASM(const s32, "code_002C6F38", sceDeci2ExLock);

INCLUDE_ASM(const s32, "code_002C6F38", sceDeci2ExUnLock);

INCLUDE_ASM(const s32, "code_002C6F38", kputs);

INCLUDE_ASM(const s32, "code_002C6F38", SetTLBHandler);

INCLUDE_ASM(const s32, "code_002C6F38", SetDebugHandler);

INCLUDE_ASM(const s32, "code_002C6F38", Copy_2);

INCLUDE_ASM(const s32, "code_002C6F38", kCopy_3);

INCLUDE_ASM(const s32, "code_002C6F38", GetEntryAddress_1);

INCLUDE_ASM(const s32, "code_002C6F38", setup_0);

INCLUDE_ASM(const s32, "code_002C6F38", InitTLBFunctions);

INCLUDE_ASM(const s32, "code_002C6F38", PutTLBEntry);

INCLUDE_ASM(const s32, "code_002C6F38", iPutTLBEntry_0);

INCLUDE_ASM(const s32, "code_002C6F38", _SetTLBEntry);

INCLUDE_ASM(const s32, "code_002C6F38", iPutTLBEntry);

INCLUDE_ASM(const s32, "code_002C6F38", iSetTLBEntry);

INCLUDE_ASM(const s32, "code_002C6F38", GetTLBEntry);

INCLUDE_ASM(const s32, "code_002C6F38", iGetTLBEntry);

INCLUDE_ASM(const s32, "code_002C6F38", ProbeTLBEntry);

INCLUDE_ASM(const s32, "code_002C6F38", iProbeTLBEntry);

INCLUDE_ASM(const s32, "code_002C6F38", ExpandScratchPad);

INCLUDE_ASM(const s32, "code_002C6F38", InitTLB);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054BCF0);

INCLUDE_ASM(const s32, "code_002C6F38", InitTLB32MB);

INCLUDE_ASM(const s32, "code_002C6F38", _kTLBException);

INCLUDE_ASM(const s32, "code_002C6F38", _xlaunch);

INCLUDE_ASM(const s32, "code_002C6F38", _kDebugException);

INCLUDE_ASM(const s32, "code_002C6F38", libc_2E52FC);

INCLUDE_ASM(const s32, "code_002C6F38", libc_2E5320);

INCLUDE_ASM(const s32, "code_002C6F38", libc_2E59B8);

INCLUDE_ASM(const s32, "code_002C6F38", libc_2E6048);

INCLUDE_ASM(const s32, "code_002C6F38", libc_2E65C8);

INCLUDE_ASM(const s32, "code_002C6F38", libc_2E6B30);

INCLUDE_ASM(const s32, "code_002C6F38", libc_2E6C48);

INCLUDE_ASM(const s32, "code_002C6F38", libc_2E6D88);

INCLUDE_ASM(const s32, "code_002C6F38", __errno_8);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054BD78);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054BE78);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054BF78);

INCLUDE_RODATA(const s32, "code_002C6F38", D_0054C078);

