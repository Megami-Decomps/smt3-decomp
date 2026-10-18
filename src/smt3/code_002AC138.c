#include "common.h"
#include "sdf_types.h"

extern u8 sdfFrameId;

#define SDF_PENDING_LAST_SLOT 1
#define SDF_DMA_TAG_REF_BYTE 0x30
#define SDF_DMA_CNT_ONE_WORD 0x10000001
#define SDF_VIF_DIRECT_ONE_FLUSHE 0x5000000110000000ULL
#define SDF_GIF_PACKED_AD_BITS 0x1000000000008000ULL
#define SDF_VIF_DIRECT_ONE_NOP 0x5000000100000000ULL
#define SDF_GS_DEFAULT_TEXA 0x4000000080ULL
#define SDF_GS_SCENE_TEST 0x517FB
#define SDF_TEXTURE_SCENE_PAYLOAD_QWORDS 0x15
#define SDF_GRAPH_DEPTH_BUFFER_INDEX 2
extern u8 D_0055188A;
extern u16 sdfReqDrawDelaySleepCount;
extern u32 sdfDrawFrameCount;
extern u8 sdfDrawImageTrans;
extern volatile u8 sdfDrawSyncFlag;
extern u32 sdfGetRunTimeCountBase(void);
extern u32 sdfGetRunTimeCount(u32 previousTick);
extern void sdfWaitVblank(s32 count);

struct SdfGsGouraudTexturedVertex;
struct SdfGsTwoVertexPayload;
struct SdfGsTexturedVertex;
struct SdfGsGouraudVertex;
struct sdfPrimF3Elem_t;
struct sdfPrimF4Elem_t;
struct sdfPrimBoxLineElem_t;
struct sdfPrimTransElem_t;
struct sdfPrimTexflushPacket_t;
union MovObjStream;
struct sdfGrRefPacketHeader_t;
struct sdfDelayedResrcCB_t;
struct SdfPacketPatchLink;
struct sdfGrDrawFramePacket_t;
struct sdfGrClearPacket_t;
struct SdfGsDrawDefaultsRegisters;
struct sdfPrimGt3Elem_t;
struct sdfPrimGt4Elem_t;
struct SdfGsTwoVertexPacket;
struct sdfPrimSpriteElem_t;
struct sdfPrimFt3Elem_t;
struct sdfPrimFt4Elem_t;
struct DevRequest;
struct sdfPrimG3Elem_t;
struct sdfPrimG4Elem_t;
struct SdfDmaReferenceChainPacket;
struct sdfPrimBitBltElem_t;
struct SdfPendingBuffer;
struct sdfPrimF3Packet_t;
struct sdfPrimF4Packet_t;
struct sdfPrimBoxLinePacket_t;
struct sdfPrimTransPacket_t;
struct sdfGrDrawFrame_t;
struct SdfGsSceneBlendRegisters;
struct sdfPrimGt3Packet_t;
struct sdfPrimGt4Packet_t;
struct sdfPrimSpritePacket_t;
struct sdfPrimFt3Packet_t;
struct sdfPrimFt4Packet_t;
struct sdfPrimG3Packet_t;
struct sdfPrimG4Packet_t;
struct sdfPrimBitBltPacket_t;
struct sdfLoadImagePacket_t;
struct SdfSceneDrawPacket;
struct sdfThCB_t;
struct sdfGsMemSlice_t;
struct IpuDmaState;
struct sdfIpuDecodeCb_t;
struct movieStream_t;
struct sdfGrFrameSet_t;
struct sdfDrawTag_t;
struct sdfGrOt_t;
struct sdfGrFramePacket_t;
struct sdfMemHandle_t;
struct SdfDmaTag;
struct SdfPacketSlot;
struct sdfStoreImagePacket_t;
struct SdfGraphCopyPacket;
struct SdfPatchableResourcePacket;
struct sdfPreDrawTag_t;
struct SdfPacketChain;
struct sdfFileCmdHandle_t;
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
/* Values written to the PRIM and TEST registers by the centered-bounds packet. */
enum {
    SDF_GS_PRIMITIVE_SPRITE = 0x06,
    SDF_GS_CENTERED_VIEW_BOUNDS_TEST = 0x30003
};
/* One 0x8C allocation owns the stream/sound links and IPU transfer state. */
struct SdfStreamInputDmaTag;
struct SoundFormat;
/* The +0x1C movie work pointer selects one of two separately allocated records.
 * isPac at +3 chooses the active arm; the variant payload types live in
 * sdf_movie_stream.h. */
struct MovLinearStream;
struct MovPacStream;
typedef struct sdfDelayedResrcChunk_t sdfDelayedResrcChunk_t;
/* Deferred handlers share one address-word argument at the native dispatch
 * boundary, while their C consumers interpret that word as different owners. */
typedef void (*SdfPendingCallback)();
/* The frame worker passes the node and queued buffer index. The registered
 * handlers project the node into their distinct complete packet owners. */
typedef void (*SdfPacketPatchCallback)();
/* Each GIF REGLIST vertex carries UV, its own RGBAQ, and packed XYZ2. */
typedef struct SdfGsGouraudTexturedVertex {
    u64 uv;
    u64 rgbaq;
    u64 xyz2;
} SdfGsGouraudTexturedVertex;
/* GIF REGLIST stream: PRIM, RGBAQ and two packed XYZ2 vertices. */
typedef struct SdfGsTwoVertexPayload {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    u64 rgbaq;
    u64 xyz2[2];
} SdfGsTwoVertexPayload;
/* GIF REGLIST stream: PRIM, RGBAQ, then UV/XYZ2 for each vertex. */
typedef struct SdfGsTexturedVertex {
    u64 uv;
    u64 xyz2;
} SdfGsTexturedVertex;
/* GIF REGLIST vertices carry their own RGBAQ and a packed XYZ2 position. */
typedef struct SdfGsGouraudVertex {
    u64 rgbaq;
    u64 xyz2;
} SdfGsGouraudVertex;
void sdfChipFreeDirect(void *memory);
#define SDF_PENDING_BUFFER_CAPACITY 0x3F
/* GIF stream for one flat-color triangle: tag/register list, PRIM, RGBAQ,
 * and the three XYZ2 values. */
typedef struct sdfPrimF3Elem_t {
    u64 gifTag;
    u64 gifRegisterList;
    u64 primitive;
    u64 rgbaq;
    u64 xyz2[3];
} sdfPrimF3Elem_t;
/* GIF stream for four flat-color vertices: PRIM, RGBAQ and four XYZ2 values. */
typedef struct sdfPrimF4Elem_t {
    u64 gifTag;
    u64 gifRegisterList;
    u64 primitive;
    u64 rgbaq;
    u64 xyz2[4];
} sdfPrimF4Elem_t;
/* GIF stream for a closed rectangle: its first XYZ2 value is repeated last. */
typedef struct sdfPrimBoxLineElem_t {
    u64 gifTag;
    u64 gifRegisterList;
    u64 primitive;
    u64 rgbaq;
    u64 xyz2[5];
} sdfPrimBoxLineElem_t;
#define SDF_DMA_TAG_NEXT_BYTE 0x20
#define SDF_DMA_TAG_CALL_BYTE 0x50
#define SDF_DMA_TAG_END_BYTE 0x70
#define SDF_DMA_QWC_MASK 0xFFFF
#define SDF_DMA_ADDRESS_MASK 0x0FFFFFFF
#define SDF_QWORD_BYTES 0x10
#define SDF_QWORD_ALIGNMENT_MASK 0xF
#define SDF_PACKET_BUFFER_ALIGNMENT_MASK 0x7F
#define SDF_PACKET_BUFFER_COUNT 2
#define SDF_PENDING_SLOT_COUNT 2
#define SDF_PATCHABLE_PACKET_BYTES 0x100
#define SDF_PATCHABLE_PACKET_TAIL_OFFSET 0xD0
#define SDF_DESCRIPTOR_PACKET_BYTES 0xB0
#define SDF_DESCRIPTOR_PACKET_TAIL_OFFSET 0x80
#define SDF_DMA_EXTENDED_TAIL_OFFSET 0x30
#define SDF_DMA_REFERENCE_NODE_BYTES 0x20
#define SDF_DMA_TAG_REF_WORD 0x30000000
#define SDF_DMA_TAG_NEXT_WORD 0x20000000
#define SDF_PACKET_LIST_BYTES 0x20
#define SDF_VIF_DIRECT_WORD 0x50000000
#define SDF_VIF_FLUSHE_WORD 0x10000000
#define SDF_GIF_ONE_REGISTER_WORD 0x10000000
#define SDF_GIF_EOP_BIT 0x8000
#define SDF_GS_CENTER_BIAS 0x1000
#define SDF_GS_FIELD_OFFSET_STEP 8
#define SDF_GS_DEFAULT_ALPHA 0x44
#define SDF_GS_ALPHA_TEST 0x717FB
#define SDF_SCENE_DRAW_PAYLOAD_QWORDS 5
#define SDF_ALPHA_DMA_QWORDS 3
#define SDF_VIF_DIRECT_THREE_WORD 0x50000003
#define SDF_VIF_FLUSHE_HALFWORD 0x1000
#define SDF_GIF_TWO_AD_LOOPS_EOP 0x8002
extern s8 sdfDelayedResrcFlag;
extern volatile s8 sdfReqDrawDelayCount;
/* Protocol IDs use distinct byte-sized storage codes in sdfFileCmdHandle_t. */
typedef u8 SdfDevOperationCode;
typedef s8 SdfDevStateCode;
/* TEST and ALPHA form the two A+D writes shared by the blend builders. */
typedef struct sdfPrimTransElem_t {
    sdfGrTagStdReg_t test;
    sdfGrTagStdReg_t alpha;
} sdfPrimTransElem_t;
/* A single A+D write follows the texture-flush DMA/VIF/GIF header. */
typedef struct sdfPrimTexflushPacket_t {
    sdfGrPacketHeader_t header;
    sdfGrTagStdReg_t textureFlush;
} sdfPrimTexflushPacket_t;
typedef union MovObjStream {
    struct MovLinearStream *linear;
    struct MovPacStream *pac;
} MovObjStream;
/* REF tag and VIF commands followed by the trailing chain tag (0x20).
 * The complete trailing quadword is cleared before its NEXT header is patched. */
typedef struct sdfGrRefPacketHeader_t {
    u64 dmaTag;
    u64 vifCommands;
    s128 nextTag;
} sdfGrRefPacketHeader_t;
/* Interrupt-synchronized deferred callback owner (0x08). */
typedef struct sdfDelayedResrcCB_t {
    SdfPendingCallback handler;
    sdfDelayedResrcChunk_t *pending;
} sdfDelayedResrcCB_t;
typedef struct SdfPacketPatchLink {
    struct SdfPacketPatchLink *next;
    SdfPacketPatchCallback patch;
} SdfPacketPatchLink;
/* Four A+D writes define one GS drawing context, without a DMA/GIF header. */
typedef struct sdfGrDrawFramePacket_t {
    sdfGrTagStdReg_t frame;
    sdfGrTagStdReg_t zbuf;
    sdfGrTagStdReg_t xyoffset;
    sdfGrTagStdReg_t scissor;
} sdfGrDrawFramePacket_t;
/* A+D writes that draw the centered viewport bounds as a sprite. */
typedef struct sdfGrClearPacket_t {
    sdfGrTagStdReg_t test;
    sdfGrTagStdReg_t primitive;
    sdfGrTagStdReg_t rgbaq;
    sdfGrTagStdReg_t xyz2[2];
} sdfGrClearPacket_t;
/* Shared A+D state for GS primitive modes, color clamping, dithering and TEXA. */
typedef struct SdfGsDrawDefaultsRegisters {
    sdfGrTagStdReg_t prmodecont;
    sdfGrTagStdReg_t colclamp;
    sdfGrTagStdReg_t dthe;
    sdfGrTagStdReg_t texa;
} SdfGsDrawDefaultsRegisters;
/* PRIM followed by 3 individually textured and colored vertices. */
typedef struct sdfPrimGt3Elem_t {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    SdfGsGouraudTexturedVertex vertices[3];
} sdfPrimGt3Elem_t;
/* PRIM followed by 4 individually textured and colored vertices. */
typedef struct sdfPrimGt4Elem_t {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    SdfGsGouraudTexturedVertex vertices[4];
} sdfPrimGt4Elem_t;
/* DMA/VIF header followed by the complete two-vertex GIF payload. */
typedef struct SdfGsTwoVertexPacket {
    u64 dmaTag;
    u64 vifCommands;
    SdfGsTwoVertexPayload drawing;
} SdfGsTwoVertexPacket;
typedef struct sdfPrimSpriteElem_t {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    u64 rgbaq;
    SdfGsTexturedVertex vertices[2];
} sdfPrimSpriteElem_t;
/* GIF REGLIST stream: PRIM, one RGBAQ, then UV/XYZ2 for each vertex. */
typedef struct sdfPrimFt3Elem_t {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    u64 rgbaq;
    SdfGsTexturedVertex vertices[3];
} sdfPrimFt3Elem_t;
/* GIF REGLIST stream: PRIM, one RGBAQ, then UV/XYZ2 for each vertex. */
typedef struct sdfPrimFt4Elem_t {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    u64 rgbaq;
    SdfGsTexturedVertex vertices[4];
} sdfPrimFt4Elem_t;
typedef struct DevRequest {
    struct sdfMemHandle_t *backingAllocation;
    s16 usedCount;
    u16 capacity;
    s16 stride;
    s16 growStep;
    void *buffer;
} DevRequest;
/* PRIM followed by three independently colored vertices. */
typedef struct sdfPrimG3Elem_t {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    SdfGsGouraudVertex vertices[3];
} sdfPrimG3Elem_t;
/* PRIM followed by four independently colored vertices. */
typedef struct sdfPrimG4Elem_t {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    SdfGsGouraudVertex vertices[4];
} sdfPrimG4Elem_t;
/* GIF tag followed by a REF transfer and the patchable NEXT link. */
typedef struct SdfDmaReferenceChainPacket {
    sdfGrPacketHeader_t header;
    u64 referenceTag;
    u64 referenceVifCommands;
    u64 nextTag;
    u64 zeroTailWord;
} SdfDmaReferenceChainPacket;
void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);
/* The four A+D writes controlling a GS image transfer. */
typedef struct sdfPrimBitBltElem_t {
    sdfGrTagStdReg_t bitbltbuf;
    sdfGrTagStdReg_t trxpos;
    sdfGrTagStdReg_t trxreg;
    sdfGrTagStdReg_t trxdir;
} sdfPrimBitBltElem_t;
/* A queued batch links its next chunk before 63 deferred owner addresses. */
typedef struct SdfPendingBuffer {
    struct SdfPendingBuffer *next;
    void *entry[SDF_PENDING_BUFFER_CAPACITY];
} SdfPendingBuffer;
/* The caller prepends DMA/VIF words to the triangle GIF stream. Allocation
 * reserves 0x50 bytes; the final qword is not written by these helpers. */
typedef struct sdfPrimF3Packet_t {
    u64 dmaTag;
    u64 vifCommands;
    sdfPrimF3Elem_t drawing;
    u64 unwrittenAllocationWord;
} sdfPrimF3Packet_t;
/* The allocation contains the DMA/VIF prefix and the entire quad GIF stream. */
typedef struct sdfPrimF4Packet_t {
    u64 dmaTag;
    u64 vifCommands;
    sdfPrimF4Elem_t drawing;
} sdfPrimF4Packet_t;
/* The caller prepends DMA/VIF words. The final allocated qword is not written. */
typedef struct sdfPrimBoxLinePacket_t {
    u64 dmaTag;
    u64 vifCommands;
    sdfPrimBoxLineElem_t drawing;
    u64 unwrittenAllocationWord;
} sdfPrimBoxLinePacket_t;
extern sdfDelayedResrcChunk_t *sdfDelayedResrcLink;
/* vu0 routine: blend clone-source palette RGB toward intensity-scaled tint, preserving alpha. */


/* Find a texture by resource key, walking from the newest node to older ones. */


/* Transfer setup, deferred image metadata, and its linked primitive-reset tail. */


/* Allocate metadata plus a patchable DMA payload, registering both list views.
 * The drawing arguments remain opaque and are forwarded to the native builder. */


/* Allocate a descriptor packet, delegate its opaque options, and append its range. */


/* Register a completion callback, creating the shared semaphore on first use. */


/* Queue an entry on the owner's pending node, adding a node or buffer chunk as needed. */


/* Detach the pending chain and clear each callback owner's pending node. */


/* Flush the newest buffer's populated entries first, then older full buffers.
 * Invoke the owner's handler in entry order within each buffer before freeing it. */


/* Hand the oldest pending slot to the worker, shift the slot list down and
 * refill the last slot with the newly detached queue. */


/* Return one for an active rotation, a queued chain, or either delayed slot. */


extern struct sdfMemHandle_t *sdfGrPacketBuffHandle;
/* Fixed packets allocated by the paired blend and composite-quad renderers. */
typedef struct sdfPrimTransPacket_t {
    sdfGrPacketHeader_t header;
    sdfPrimTransElem_t registers;
} sdfPrimTransPacket_t;
/* Accessed output span of the common scene-context builder. */
typedef struct sdfGrDrawFrame_t {
    sdfGrPacketHeader_t header;
    sdfGrDrawFramePacket_t registers;
} sdfGrDrawFrame_t;
/* TEST/ALPHA A+D state for the two independent GS drawing contexts. */
typedef struct SdfGsSceneBlendRegisters {
    sdfPrimTransElem_t contextOne;
    sdfPrimTransElem_t contextTwo;
} SdfGsSceneBlendRegisters;
typedef struct sdfPrimGt3Packet_t {
    u64 dmaTag;
    u64 vifCommands;
    sdfPrimGt3Elem_t drawing;
} sdfPrimGt3Packet_t;
typedef struct sdfPrimGt4Packet_t {
    u64 dmaTag;
    u64 vifCommands;
    sdfPrimGt4Elem_t drawing;
    u64 unwrittenTail; /* Final allocated word is not written by the builder. */
} sdfPrimGt4Packet_t;
typedef struct sdfPrimSpritePacket_t {
    u64 dmaTag;
    u64 vifCommands;
    sdfPrimSpriteElem_t drawing;
} sdfPrimSpritePacket_t;
typedef struct sdfPrimFt3Packet_t {
    u64 dmaTag;
    u64 vifCommands;
    sdfPrimFt3Elem_t drawing;
} sdfPrimFt3Packet_t;
typedef struct sdfPrimFt4Packet_t {
    u64 dmaTag;
    u64 vifCommands;
    sdfPrimFt4Elem_t drawing;
} sdfPrimFt4Packet_t;
typedef struct sdfPrimG3Packet_t {
    u64 dmaTag;
    u64 vifCommands;
    sdfPrimG3Elem_t drawing;
    u64 unwrittenTail; /* Allocation rounds the REGLIST payload up to a qword. */
} sdfPrimG3Packet_t;
typedef struct sdfPrimG4Packet_t {
    u64 dmaTag;
    u64 vifCommands;
    sdfPrimG4Elem_t drawing;
    u64 unwrittenTail; /* Allocation rounds the REGLIST payload up to a qword. */
} sdfPrimG4Packet_t;
/* DMA/VIF and GIF header followed by the complete image-transfer register block. */
typedef struct sdfPrimBitBltPacket_t {
    sdfGrPacketHeader_t header;
    sdfPrimBitBltElem_t transfer;
} sdfPrimBitBltPacket_t;
/* Host-to-local image transfer packet followed by its flush terminal tag (0xB0). */
typedef struct sdfLoadImagePacket_t {
    u64 header[4];
    sdfPrimBitBltElem_t transfer;
    u64 imageReferenceTag;
    u64 imageReferencePad;
    u64 imageGifTag0;
    u64 imageGifTag1;
    u64 flushTag0;
    u64 flushTag1;
    u64 flushGifTag;
    u64 flushRegister;
    u64 zero;
    u64 finishRegister;
} sdfLoadImagePacket_t;
/* Complete DMA/GIF owner used by scene builders and held-texture draw banks. */
typedef struct SdfSceneDrawPacket {
    sdfGrPacketHeader_t header;
    SdfGsDrawDefaultsRegisters drawDefaults;
    sdfGrDrawFramePacket_t contextOne;
    sdfGrDrawFramePacket_t contextTwo;
    sdfGrClearPacket_t centeredBounds;
    SdfGsSceneBlendRegisters blendState;
} SdfSceneDrawPacket;
/* Linked registry node stored in the title's tracked-thread table (0x8). */
typedef struct sdfThCB_t {
    struct sdfThCB_t *next; /* 0x00 */
    s32 threadId;               /* 0x04 */
} sdfThCB_t;
/* Register a thread node, then start its entry function on the supplied stack. */
void sdfStartThreadAndWait(sdfThCB_t *node, void (*entry)(void),
                           void *stack, s64 stackSize, s32 priority, s32 arg);
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
/* Draw-surface pool entry (0x20); the SDK initializes and flushes this owner. */
typedef struct sdfGrOt_t {
    struct sdfGrOt_t *next; /* 0x00 */
    sdfDrawTag_t *first;       /* 0x04 */
    sdfDrawTag_t *last;        /* 0x08 */
    u32 unkC;
    void (*append)(struct sdfGrOt_t *, sdfDrawTag_t *);      /* 0x10 */
    s32 (*prepend)(struct sdfGrOt_t *, s32, sdfDrawTag_t *); /* 0x14 */
    u32 unk18;
    u32 unk1C;
} sdfGrOt_t;
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
/* General-heap allocation descriptor (0x10); links bound the represented span. */
typedef struct sdfMemHandle_t {
    struct sdfMemHandle_t *prev; /* 0x00 */
    struct sdfMemHandle_t *next; /* 0x04 */
    s32 address;              /* 0x08: start of represented span */
    u16 state;                /* 0x0C: free, used, or end sentinel */
    s16 referenceCount;       /* 0x0E: -1 for sentinels */
} sdfMemHandle_t;
void sdfBuildTextureScenePacket(SdfSceneDrawPacket *packet,
    struct sdfGrFrameSet_t *view, s32 bufferIndex);
/* Packet-list code views the same 16-byte row as QWC/ID/ADDR/VIF commands. */
typedef struct SdfDmaTag {
    u16 quadwordCount;
    u8 pad02;
    u8 kind;
    u32 address;
    u64 vifCommands;
} QWORD_ALIGNED SdfDmaTag;
DevRequest *sdfItemArrayNew(s32 count, s32 stride, s32 growStep);
void sdfDevResizeBufferedRequest(DevRequest *request, s32 count);
void sdfBuildDmaReferenceChain(SdfDmaReferenceChainPacket *packet,
    u32 sourceAddress, s32 qwordCount);
void sdfDrawTagLink2(struct sdfDrawTag_t *list, u32 packetAddress,
                          u32 rangeTailAddress);
void sdfDrawTagLinkRef(struct sdfDrawTag_t *list, u32 packetAddress);
/* One 0x10-byte entry in the three-slot frame packet submission ring. */
typedef struct SdfPacketSlot {
    struct sdfDrawTag_t *packetList;
    SdfPacketPatchLink *callbackHead;
    u8 slotUpdatePhase;
    u8 queuedBufferIndex;
    u8 bufferIndex;
    u8 bufferSlotIndex;
    u8 pad0C[4];
} SdfPacketSlot;
void *sdfChipAllocDirect(s32 size);
struct sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);
u32 sdfMemLock(struct sdfMemHandle_t *allocation);
/* Resource upload payload and its terminal primitive-reset tag (0xF0). */
typedef struct sdfStoreImagePacket_t {
    u64 header[14];
    sdfPrimBitBltElem_t transfer;
    s32 quadwordCount;
    u32 unkB4;
    u32 unkB8;
    u32 unkBC;
    u64 tail[6];
} sdfStoreImagePacket_t;
/* Metadata node plus the extended local-to-local image-copy payload (0x70). */
typedef struct SdfGraphCopyPacket {
    SdfPacketPatchLink link;
    u32 resourceIndexXor; /* Selects the source resource for the queued buffer. */
    u32 unk0C;
    sdfPrimBitBltPacket_t draw;
} SdfGraphCopyPacket;
/* Metadata node followed by the resource-transfer payload (0x100). */
typedef struct SdfPatchableResourcePacket {
    SdfPacketPatchLink link;
    u32 unk08;
    u32 unk0C;
    sdfStoreImagePacket_t resourcePacket;
} SdfPatchableResourcePacket;
/* Queue a retained resource for deferred release; otherwise destroy it now. */
void sdfMatListFreeResrc(void *resource, s32 retained);
/* Complete linked-packet list, distinct from the 0x20-byte DMA list head. */
typedef struct sdfPreDrawTag_t {
    u32 unk0;
    SdfPacketPatchLink *first;
    SdfPacketPatchLink *last;
    u32 unkC;
} sdfPreDrawTag_t;
/* Frame submission joins these 0x10-byte linked owners through head/tail. */
typedef struct SdfPacketChain {
    sdfPreDrawTag_t *head;
    sdfPreDrawTag_t *tail;
} SdfPacketChain;

/* Linked draw environment, including its two frame-mask writes and textures. */
typedef struct sdfSceneNode_t {
    SdfPacketPatchLink link;
    sdfGrFrameSet_t *view;
    u8 unk0C[4];
    sdfGrPacketHeader_t header;
    SdfGsDrawDefaultsRegisters drawDefaults;
    sdfGrDrawFramePacket_t contextOne;
    sdfGrDrawFramePacket_t contextTwo;
    sdfGrClearPacket_t centeredBounds;
    SdfGsSceneBlendRegisters blendState;
    sdfGrTagStdReg_t frameContextOne;
    sdfGrTagStdReg_t frameContextTwo;
    u8 texturePackets[0x80];
} sdfSceneNode_t;
void sdfGrStdDrawEnvPredraw(sdfSceneNode_t *node, s32 bufferIndex);
void sdfGrSetRefGifPacket(SdfDmaReferenceChainPacket *packet, u32 sourceAddress, s32 qwordCount);
void sdfGrSetDrawDefParmPacket(SdfGsDrawDefaultsRegisters *packet);
extern s32 EIntr(void);
extern s32 D_005520A8[2];
extern s32 sdfGrPacketBuffSize;
extern s32 D_00551870[2];
extern s32 sdfGrPacketBuffPtr;
extern s32 sdfGrPacketBuffLimit;
extern s32 DIntr(void);
extern s32 sdfDelayedResrcSemaId;
extern s32 sdfCreateSema(u32, u32, u32);
extern s32 sdfDrawReqQueueIndex;
extern SdfPacketSlot sdfGrDrawState[];
extern sdfGrFrameSet_t D_00529388;
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
s32 sdfGetTexelBitCount(u32 format);
typedef struct EffPacketParams {
    s16 parameterCount;
    s16 vertexCount;
    u16 primitive;
    u16 mask;
    u32 color;
    const u32 *parameters;
    u128 *positions;
    u128 *normals;
    f32 *texcoords;
    u32 *extraTexcoords;
    u32 *colors;
    void *(*allocate)(s32);
    f32 depth;
} EffPacketParams;
struct sdfDelayedResrcChunk_t {
    sdfDelayedResrcChunk_t *next;
    sdfDelayedResrcCB_t *owner;
    SdfPendingBuffer *buffer;
    s32 remaining;
};

void sdfSetStoreImage(sdfStoreImagePacket_t *packet, sdfGsMemSlice_t *source,
    s32 sourceX, s32 sourceY, s32 width, s32 height, u32 resourceAddress, u32 arg7, u32 arg8);
void sdfLinkStoreImage(sdfDrawTag_t *list, sdfGsMemSlice_t *source, s32 sourceX,
                   s32 sourceY, s32 width, s32 height, s32 resourceAddress, s32 arg7,
                   s32 arg8, s32 (*allocate)(s32));
void sdfLinkStoreFrameImagePredraw(SdfPatchableResourcePacket *packet, s32 entryIndex);
void sdfLinkStoreFrameImage(sdfDrawTag_t *list, sdfPreDrawTag_t *linkedList, s32 sourceX, s32 sourceY,
                   s32 width, s32 height, s32 resourceAddress, s32 arg7, s32 arg8,
                   s32 (*allocatePacket)(s32));
void sdfSetLoadImage(sdfLoadImagePacket_t *packet, sdfGsMemSlice_t *source,
                   s64 destinationX, s64 destinationY, s32 transferWidth,
                   s32 transferHeight, u32 sourceAddress);
void sdfLinkLoadImage(sdfDrawTag_t *list, sdfGsMemSlice_t *source,
                   s32 destinationX, s32 destinationY, s32 width, s32 height,
                   s32 sourceAddress, s32 (*allocatePacket)(s32));
void sdfDelayedResrcInitCB(sdfDelayedResrcCB_t *request, SdfPendingCallback callback);
void sdfDelayedResrcEntry(sdfDelayedResrcCB_t *owner, void *entry);
sdfDelayedResrcChunk_t *sdfDelayedResrcClose(void);
void sdfDelayedResrcRelease(sdfDelayedResrcChunk_t *pendingNode);
u64 sdfDelayedResrcCheckFunc(void);
u64 sdfDelayedResrcCheck(void);
void sdfGrMakePacketBuff(s32 bufferBytes);
void sdfGrSetupPacketBuff(s32 bufferIndex);
s32 sdfGrAllocPacketBuff(s32 size);
void sdfGrClosePacketBuff(s32 cursorAddress);
void sdfDrawTagInit(sdfDrawTag_t *list);
void sdfDrawTagLink(sdfDrawTag_t *list, u32 packet);
void sdfDrawTagLinkRefGif(sdfDrawTag_t *list, u32 packetAddress);
void sdfDrawTagLinkCall(sdfDrawTag_t *list, u32 packetAddress);
void sdfGrOtFuncLinkToHead(sdfGrOt_t *destinationList, sdfDrawTag_t *incomingList);
void sdfGrOtNormalReg(sdfGrOt_t *list, sdfDrawTag_t *item);
s32 sdfGrOtNormalCallback(sdfGrOt_t *list, s32 mode, sdfDrawTag_t *packet);
void sdfGrOtInit(sdfGrOt_t *nodes, s32 count);
sdfGrRefPacketHeader_t *sdfGrNewExPacket(SdfDmaTag *sourceTag);
SdfDmaTag *sdfGrAddExPacket(SdfDmaTag *previousTag, SdfDmaTag *sourceTag);
void sdfGrJoinDrawTag(sdfDrawTag_t *previousList, sdfDrawTag_t *incomingList);
void sdfGrInitExPacket(sdfDrawTag_t *list);
sdfDrawTag_t *sdfGrOtClose(sdfGrOt_t *node);
void sdfPreDrawTagLink(sdfPreDrawTag_t *list, SdfPacketPatchLink *node);
void sdfPreDrawChainLink(SdfPacketChain *chain, sdfPreDrawTag_t *node);
void sdfGrSetRegsPacketHeader(sdfGrPacketHeader_t *header, s32 payloadQwords);
void sdfGrSetDrawFramePacket(sdfGrDrawFramePacket_t *packet, s32 frameAddress, s32 width, s32 height,
                  s32 frameFormat, s32 depthAddress, s32 depthFormat,
                  s32 fieldOffset, s32 gsContext);
void sdfGrSetClearPacket(sdfGrClearPacket_t *packet, s32 width, s32 height, s32 unused0, s32 unused1);
void sdfGrSetDrawFrame(sdfGrDrawFrame_t *packet, s32 frameAddress, s32 width, s32 height,
                           s32 frameFormat, s32 depthAddress, s32 depthFormat, s32 gsContext);
void sdfGrLinkStdDrawEnv(sdfDrawTag_t *dmaList, sdfPreDrawTag_t *linkedList, SdfPacketPatchLink *linkedNode);
void sdfGrLinkDispFrameTex(sdfDrawTag_t *list, u32 source, sdfGrRefPacketHeader_t *node);
void sdfGrLinkDrawFrameTex(sdfDrawTag_t *list, u32 source, sdfGrRefPacketHeader_t *node);
void sdfGrFramePacketPredraw(sdfGrFramePacket_t *packet, s32 bufferIndex);
void sdfGrSetFramePacket(sdfGrFramePacket_t *packet, sdfGrFrameSet_t *source, u32 frameMask, s32 region, s32 mode);
void sdfGrReqDrawDelay(s32 value);
void sdfGrInitDraw(void);
sdfDrawTag_t *sdfPrimAllocDrawTag(s32 (*allocator)(s32));
void sdfPrimLinkSimplePacket(s32 listAddress, void (*initialize)(s32), s32 packetBytes, s32 (*allocator)(s32));
void sdfPrimSetOpaqueElem(sdfPrimTransElem_t *packet);
void sdfPrimSetOpaque2Elem(sdfPrimTransElem_t *packet);
void sdfPrimSetOpaquePacket(sdfPrimTransPacket_t *packet);
void sdfPrimSetOpaque2Packet(sdfPrimTransPacket_t *packet);
void sdfPrimSetSemiTransElem(sdfPrimTransElem_t *packet);
void sdfPrimSetSemiTrans2Elem(sdfPrimTransElem_t *packet);
void sdfPrimSetSemiTransPacket(sdfPrimTransPacket_t *packet);
void sdfPrimSetSemiTrans2Packet(sdfPrimTransPacket_t *packet);
void sdfPrimSetAddTransElem(sdfPrimTransElem_t *packet);
void sdfPrimSetAddTrans2Elem(sdfPrimTransElem_t *packet);
void sdfPrimSetAddTransPacket(sdfPrimTransPacket_t *packet);
void sdfPrimSetAddTrans2Packet(sdfPrimTransPacket_t *packet);
void sdfPrimSetSubTransElem(sdfPrimTransElem_t *packet);
void sdfPrimSetSubTrans2Elem(sdfPrimTransElem_t *packet);
void sdfPrimSetSubTransPacket(sdfPrimTransPacket_t *packet);
void sdfPrimSetSubTrans2Packet(sdfPrimTransPacket_t *packet);
void sdfPrimSetTexflushElem(sdfGrTagStdReg_t *write);
void sdfPrimSetTexflushPacket(sdfPrimTexflushPacket_t *packet);
void sdfPrimSetBitBltElem(sdfPrimBitBltElem_t *packet, u32 destinationBufferAddress, s32 destinationBufferWidth,
                  s64 destinationFormat, s64 destinationX, s64 destinationY,
                  u32 sourceBufferAddress, s32 sourceBufferWidth, s32 sourceFormat,
                  s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight, s32 transferDirection);
void sdfPrimSetBitBltPacket(sdfPrimBitBltPacket_t *packet, u32 destinationBufferAddress, s32 destinationBufferWidth,
                                    s64 destinationFormat, s64 destinationX, s64 destinationY,
                                    u32 sourceBufferAddress, s32 sourceBufferWidth, s32 sourceFormat,
                                    s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight, s32 transferDirection);
void sdfPrimLinkBitBltPacket(s32 packetList, u32 destinationBufferAddress, s32 destinationBufferWidth,
                             s64 destinationFormat, s64 destinationX, s64 destinationY,
                             u32 sourceBufferAddress, s32 sourceBufferWidth, s32 sourceFormat,
                             s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight,
                             s32 transferDirection, s32 (*allocator)(s32));
void sdfPrimFrameMovePredraw(SdfGraphCopyPacket *packet, s32 entryIndex);
void sdfPrimLinkFrameMovePacket(sdfDrawTag_t *drawList, sdfPreDrawTag_t *linkedList,
                   sdfGsMemSlice_t *destination, s32 destinationX, s32 destinationY,
                   s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight,
                   s32 resourceIndexXor, s32 (*allocPacket)(s32));
void sdfPrimSetSpriteElem(sdfPrimSpriteElem_t *packet, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 depth);
void sdfPrimLinkSprite(sdfDrawTag_t *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0,
                   s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 depth, s32 (*alloc)(s32));
void sdfPrimSetF3Elem(sdfPrimF3Elem_t *dst, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 depth);
void sdfPrimLinkF3(sdfDrawTag_t *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1,
                   s32 y1, s32 x2, s32 y2, s32 depth, s32 (*alloc)(s32));
void sdfPrimSetF4Elem(sdfPrimF4Elem_t *packet, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 x3, s32 y3, s32 depth);
void sdfPrimLinkF4(sdfDrawTag_t *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 x3, s32 y3, s32 depth, s32 (*alloc)(s32));
void sdfPrimSetG3Elem(sdfPrimG3Elem_t *packet, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1, s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 depth);
void sdfPrimLinkG3(sdfDrawTag_t *list, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1, s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 depth, s32 (*alloc)(s32));
void sdfPrimSetG4Elem(sdfPrimG4Elem_t *packet, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1, s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 x3, s32 y3, s32 color3, s32 depth);
void sdfPrimLinkG4(s32 list, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1,
                   s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 x3,
                   s32 y3, s32 color3, s32 depth, s32 (*alloc)(s32));
void sdfPrimSetFt3Elem(sdfPrimFt3Elem_t *packet, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2, s32 u2, s32 v2, s32 depth);
void sdfPrimLinkFt3(s32 list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0,
                   s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2,
                   s32 u2, s32 v2, s32 depth, s32 (*alloc)(s32));
void sdfPrimSetFt4Elem(sdfPrimFt4Elem_t *packet, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
                   s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2, s32 u2, s32 v2,
                   s32 x3, s32 y3, s32 u3, s32 v3, s32 depth);
void sdfPrimLinkFt4(sdfDrawTag_t *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0,
                   s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2,
                   s32 u2, s32 v2, s32 x3, s32 y3, s32 u3, s32 v3, s32 depth,
                   s32 (*alloc)(s32));
void sdfPrimSetGt3Elem(sdfPrimGt3Elem_t *packet, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 color0,
                   s32 x1, s32 y1, s32 u1, s32 v1, s32 color1, s32 x2, s32 y2,
                   s32 u2, s32 v2, s32 color2, s32 depth);
void sdfPrimLinkGt3(s32 list, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
                   s32 color0, s32 x1, s32 y1, s32 u1, s32 v1, s32 color1,
                   s32 x2, s32 y2, s32 u2, s32 v2, s32 color2, s32 depth,
                   s32 (*alloc)(s32));
void sdfPrimSetGt4Elem(sdfPrimGt4Elem_t *packet, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 color0,
                   s32 x1, s32 y1, s32 u1, s32 v1, s32 color1, s32 x2, s32 y2, s32 u2,
                   s32 v2, s32 color2, s32 x3, s32 y3, s32 u3, s32 v3, s32 color3, s32 depth);
void sdfPrimLinkGt4(sdfDrawTag_t *list, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
                   s32 color0, s32 x1, s32 y1, s32 u1, s32 v1, s32 color1,
                   s32 x2, s32 y2, s32 u2, s32 v2, s32 color2, s32 x3, s32 y3,
                   s32 u3, s32 v3, s32 color3, s32 depth, s32 (*alloc)(s32));
void sdfPrimSetRectElem(SdfGsTwoVertexPayload *packet, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth);
void sdfPrimLinkRect(sdfDrawTag_t *list, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth, s32 (*alloc)(s32));
void sdfPrimSetLineElem(SdfGsTwoVertexPayload *packet, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth);
void sdfPrimLinkLine(sdfDrawTag_t *list, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth, s32 (*alloc)(s32));
void sdfPrimSetBoxLineElem(sdfPrimBoxLineElem_t *dst, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth);
void sdfPrimLinkBoxLine(sdfDrawTag_t *list, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth, s32 (*alloc)(s32));

void sdfSetStoreImage(sdfStoreImagePacket_t *packet, sdfGsMemSlice_t *source,
    s32 sourceX, s32 sourceY, s32 width, s32 height, u32 resourceAddress, u32 arg7, u32 arg8) {
    s32 quadwordCount;

    packet->header[0] = 0x90000001ULL;
    packet->header[1] = 0x10000000ULL;
    packet->header[2] = 0;
    packet->header[3] = 0;
    packet->header[4] = 0x90000002ULL;
    packet->header[5] = 0x5000000206008000ULL;
    packet->header[6] = 0x1000000000000001ULL;
    packet->header[7] = 0xE;
    packet->header[8] = 0;
    packet->header[9] = 0x61;
    packet->header[10] = ((u64)((u32)packet->tail & SDF_DMA_ADDRESS_MASK) << 32) | 0xA0000005ULL;
    packet->header[11] = 0x5000000500000000ULL;
    packet->header[12] = 0x1000000000000004ULL;
    packet->header[13] = 0xE;
    sdfPrimSetBitBltElem(&packet->transfer, 0, 0, 0, 0, 0,
        source->word, source->width, source->format,
        sourceX, sourceY, width, height, 1);
    quadwordCount = (sdfGetTexelBitCount(source->format) * width * height) >> 7;
    packet->quadwordCount = quadwordCount;
    packet->unkB4 = resourceAddress;
    packet->unkB8 = arg7;
    packet->unkBC = arg8;
    packet->tail[0] = 0x20000002ULL;
    packet->tail[1] = 0x5000000206000000ULL;
    packet->tail[2] = 0x1000000000008001ULL;
    packet->tail[3] = 0xE;
    packet->tail[4] = 0x46;
    packet->tail[5] = 0;
}

void sdfLinkStoreImage(sdfDrawTag_t *list, sdfGsMemSlice_t *source, s32 sourceX,
                   s32 sourceY, s32 width, s32 height, s32 resourceAddress, s32 arg7,
                   s32 arg8, s32 (*allocate)(s32)) {
    s32 packetAddress;

    if (allocate == NULL) {
        allocate = sdfGrAllocPacketBuff;
    }
    packetAddress = allocate(0xf0);
    sdfSetStoreImage((sdfStoreImagePacket_t *)packetAddress, source,
        sourceX, sourceY, width, height, resourceAddress, arg7, arg8);
    sdfDrawTagLink2(list, packetAddress, packetAddress + 0xc0);
}

void sdfLinkStoreFrameImagePredraw(SdfPatchableResourcePacket *packet, s32 entryIndex) {
    packet->resourcePacket.transfer.bitbltbuf.data =
        (packet->resourcePacket.transfer.bitbltbuf.data & ~0x3FFF) |
        (u64)(u32)(D_00529388.buffers[entryIndex]->word >> 6);
}

void sdfLinkStoreFrameImage(sdfDrawTag_t *list, sdfPreDrawTag_t *linkedList, s32 sourceX, s32 sourceY,
                   s32 width, s32 height, s32 resourceAddress, s32 arg7, s32 arg8,
                   s32 (*allocatePacket)(s32)) {
    SdfPatchableResourcePacket *packet;

    if (allocatePacket == NULL) {
        allocatePacket = sdfGrAllocPacketBuff;
    }
    packet = (SdfPatchableResourcePacket *)allocatePacket(SDF_PATCHABLE_PACKET_BYTES);
    packet->link.patch = sdfLinkStoreFrameImagePredraw;
    sdfSetStoreImage(&packet->resourcePacket,
        D_00529388.buffers[0], sourceX, sourceY, width, height,
        resourceAddress, arg7, arg8);
    sdfPreDrawTagLink(linkedList, &packet->link);
    sdfDrawTagLink2(list, (u32)&packet->resourcePacket,
        (u32)packet + SDF_PATCHABLE_PACKET_TAIL_OFFSET);
}

void sdfSetLoadImage(sdfLoadImagePacket_t *packet, sdfGsMemSlice_t *source,
                   s64 destinationX, s64 destinationY, s32 transferWidth,
                   s32 transferHeight, u32 sourceAddress) {
    s32 qwc;

    packet->header[0] = 0x10000006;
    packet->header[1] = 0x5000000611000000;
    packet->header[2] = 0x1000000000000004;
    packet->header[3] = 0xE;
    sdfPrimSetBitBltElem(&packet->transfer, source->word, source->width,
                  source->format, destinationX, destinationY, 0, 0, 0, 0, 0,
                  transferWidth, transferHeight, 0);

    qwc = (sdfGetTexelBitCount(source->format) * transferWidth * transferHeight) >> 7;
    packet->imageReferenceTag = 0x0800000000000000 | qwc;
    packet->imageReferencePad = 0;
    packet->imageGifTag0 = (u32)((qwc & SDF_DMA_QWC_MASK) | 0x30000000) |
                           ((u64)(sourceAddress & SDF_DMA_ADDRESS_MASK) << 32);
    packet->imageGifTag1 = (u64)(0x51000000 | qwc) << 32;
    packet->flushTag0 = 0x20000002;
    packet->flushTag1 = 0x5000000200000000;
    packet->flushGifTag = 0x1000000000008001;
    packet->flushRegister = 0xE;
    packet->zero = 0;
    packet->finishRegister = 0x3F;
}

void sdfLinkLoadImage(sdfDrawTag_t *list, sdfGsMemSlice_t *source,
                   s32 destinationX, s32 destinationY, s32 width, s32 height,
                   s32 sourceAddress, s32 (*allocatePacket)(s32)) {
    s32 packetAddress;
    if (allocatePacket == NULL) {
        allocatePacket = sdfGrAllocPacketBuff;
    }
    packetAddress = allocatePacket(SDF_DESCRIPTOR_PACKET_BYTES);
    sdfSetLoadImage((sdfLoadImagePacket_t *)packetAddress, source,
                  destinationX, destinationY, width, height, sourceAddress);
    sdfDrawTagLink2(list, packetAddress, packetAddress + SDF_DESCRIPTOR_PACKET_TAIL_OFFSET);
}

void sdfDelayedResrcInitCB(sdfDelayedResrcCB_t *request, SdfPendingCallback callback) {
    if (sdfDelayedResrcSemaId < 0) {
        sdfDelayedResrcSemaId = sdfCreateSema(1, 0x7f, 0);
    }
    request->handler = callback;
    request->pending = NULL;
}

void sdfDelayedResrcEntry(sdfDelayedResrcCB_t *owner, void *entry) {
    sdfDelayedResrcChunk_t *pendingNode;
    SdfPendingBuffer *entryBuffer;
    s32 freeEntries;

    if (entry != NULL) {
        WaitSema(sdfDelayedResrcSemaId);
        pendingNode = owner->pending;
        if (pendingNode == NULL) {
            pendingNode = (sdfDelayedResrcChunk_t *)sdfChipAllocDirect(sizeof(sdfDelayedResrcChunk_t));
            pendingNode->remaining = 0;
            pendingNode->next = sdfDelayedResrcLink;
            pendingNode->buffer = NULL;
            pendingNode->owner = owner;
            owner->pending = pendingNode;
            sdfDelayedResrcLink = pendingNode;
        }
        freeEntries = pendingNode->remaining;
        entryBuffer = pendingNode->buffer;
        if (freeEntries == 0) {
            SdfPendingBuffer *newBuffer = (SdfPendingBuffer *)sdfChipAllocDirect(sizeof(SdfPendingBuffer));
            newBuffer->next = entryBuffer;
            pendingNode->buffer = newBuffer;
            entryBuffer = newBuffer;
            freeEntries = SDF_PENDING_BUFFER_CAPACITY;
        }
        entryBuffer->entry[SDF_PENDING_BUFFER_CAPACITY - freeEntries] = entry;
        pendingNode->remaining = freeEntries - 1;
        SignalSema(sdfDelayedResrcSemaId);
    }
}

sdfDelayedResrcChunk_t *sdfDelayedResrcClose(void) {
    sdfDelayedResrcChunk_t *head = sdfDelayedResrcLink;
    sdfDelayedResrcChunk_t *node;

    sdfDelayedResrcLink = NULL;
    for (node = head; node != NULL; node = node->next) {
        node->owner->pending = NULL;
    }
    return head;
}

void sdfDelayedResrcRelease(sdfDelayedResrcChunk_t *pendingNode) {
    sdfDelayedResrcChunk_t *nextPendingNode;
    SdfPendingBuffer *entryBuffer;
    SdfPendingBuffer *nextEntryBuffer;
    SdfPendingCallback entryHandler;
    s32 entryCount;
    s32 entryIndex;

    if (pendingNode != NULL) {
        do {
            entryCount = SDF_PENDING_BUFFER_CAPACITY - pendingNode->remaining;
            entryHandler = pendingNode->owner->handler;
            entryBuffer = pendingNode->buffer;
            if (entryBuffer != NULL) {
                do {
                    entryIndex = 0;
                    do {
                        entryHandler(entryBuffer->entry[entryIndex]);
                        entryIndex++;
                    } while (entryIndex < entryCount);
                    nextEntryBuffer = entryBuffer->next;
                    sdfChipFreeDirect(entryBuffer);
                    entryBuffer = nextEntryBuffer;
                    entryCount = SDF_PENDING_BUFFER_CAPACITY;
                } while (entryBuffer != NULL);
            }
            nextPendingNode = pendingNode->next;
            sdfChipFreeDirect(pendingNode);
            pendingNode = nextPendingNode;
        } while (pendingNode != NULL);
    }
}

void sdfDelayedResrcProc(void) {
    u32 slotIndex;

    sdfDelayedResrcFlag = 1;
    WaitSema(sdfDelayedResrcSemaId);
    sdfDelayedResrcRelease((sdfDelayedResrcChunk_t *)D_005520A8[0]);
    for (slotIndex = 0; slotIndex < SDF_PENDING_LAST_SLOT; slotIndex++) {
        D_005520A8[slotIndex] = D_005520A8[slotIndex + 1];
    }
    D_005520A8[SDF_PENDING_LAST_SLOT] = (s32)sdfDelayedResrcClose();
    SignalSema(sdfDelayedResrcSemaId);
    sdfDelayedResrcFlag = 0;
}

u64 sdfDelayedResrcCheckFunc(void) {
    u32 slotIndex;
    s32 *slotCursor;
    if (sdfDelayedResrcFlag != 0) {
        return 1;
    }
    if (sdfDelayedResrcLink != NULL) {
        return 1;
    }
    slotIndex = 0;
    slotCursor = D_005520A8;
    do {
        if (*slotCursor != 0) {
            return 1;
        }
        slotCursor++;
        slotIndex++;
    } while (slotIndex < SDF_PENDING_SLOT_COUNT);
    return 0;
}

u64 sdfDelayedResrcCheck(void) {
    s64 interruptState;
    u64 pendingWork;

    interruptState = DIntr();
    pendingWork = sdfDelayedResrcCheckFunc();
    if (interruptState != 0) {
        EIntr();
    }
    return pendingWork;
}

void sdfGrMakePacketBuff(s32 bufferBytes) {
    s32 allocationAddress;
    if (sdfGrPacketBuffHandle != 0) {
        sdfMemFreeDirect(sdfGrPacketBuffHandle);
        sdfGrPacketBuffHandle = 0;
    }
    bufferBytes = (bufferBytes + SDF_PACKET_BUFFER_ALIGNMENT_MASK) & ~SDF_PACKET_BUFFER_ALIGNMENT_MASK;
    sdfGrPacketBuffSize = bufferBytes;
    sdfGrPacketBuffHandle = sdfMemAllocDirect(bufferBytes * SDF_PACKET_BUFFER_COUNT);
    allocationAddress = sdfMemLock(sdfGrPacketBuffHandle);
    D_00551870[0] = allocationAddress;
    D_00551870[1] = allocationAddress + bufferBytes;
}

void sdfGrSetupPacketBuff(s32 bufferIndex) {
    sdfGrPacketBuffPtr = D_00551870[bufferIndex];
    sdfGrPacketBuffLimit = D_00551870[bufferIndex] + sdfGrPacketBuffSize;
}

s32 sdfGrGetPacketBuffRemain(void) {
    return sdfGrPacketBuffLimit - sdfGrPacketBuffPtr;
}

s32 sdfGrAllocPacketBuff(s32 size) {
    s32 packet;

    packet = sdfGrPacketBuffPtr;
    sdfGrPacketBuffPtr = sdfGrPacketBuffPtr + ((size + 0xfU) & 0xfffffff0);
    return packet;
}

s32 sdfGrOpenPacketBuff(void) {
    return sdfGrPacketBuffPtr;
}

void sdfGrClosePacketBuff(s32 cursorAddress) {
    sdfGrPacketBuffPtr = (cursorAddress + SDF_QWORD_ALIGNMENT_MASK) & ~SDF_QWORD_ALIGNMENT_MASK;
}

void sdfDrawTagInit(sdfDrawTag_t *list) {
    list->unkC = 0xFFFF;
    list->nextList = NULL;
    list->first = 0;
    list->last = 0;
    list->firstReferenceSource = 0;
    list->secondReferenceSource = 0;
    list->unk18 = 0;
    list->unk1C = 0;
}

void sdfDrawTagLink(sdfDrawTag_t *list, u32 packet) {
    s32 last;

    last = list->last;
    if (last == 0) {
        list->first = packet;
    }
    else {
        ((SdfDmaTag *)last)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTag *)last)->address = packet & SDF_DMA_ADDRESS_MASK;
    }
    list->last = packet;
}

void sdfDrawTagLink2(sdfDrawTag_t *list, u32 packetAddress, u32 rangeTailAddress) {
    s32 previousTailAddress;

    previousTailAddress = list->last;
    if (previousTailAddress == 0) {
        list->first = packetAddress;
    }
    else {
        ((SdfDmaTag *)previousTailAddress)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTag *)previousTailAddress)->address = packetAddress & SDF_DMA_ADDRESS_MASK;
    }
    list->last = rangeTailAddress;
}

void sdfDrawTagLinkRef(sdfDrawTag_t *list, u32 packetAddress) {
    s32 previousTailAddress;

    ((SdfDmaTag *)packetAddress)->kind = SDF_DMA_TAG_REF_BYTE;
    previousTailAddress = list->last;
    if (previousTailAddress == 0) {
        list->first = packetAddress;
    }
    else {
        ((SdfDmaTag *)previousTailAddress)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTag *)previousTailAddress)->address = packetAddress & SDF_DMA_ADDRESS_MASK;
    }
    list->last = packetAddress + SDF_QWORD_BYTES;
}

void sdfDrawTagLinkRefGif(sdfDrawTag_t *list, u32 packetAddress) {
    s32 previousTailAddress;

    previousTailAddress = list->last;
    if (previousTailAddress == 0) {
        list->first = packetAddress;
    }
    else {
        ((SdfDmaTag *)previousTailAddress)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTag *)previousTailAddress)->address = packetAddress & SDF_DMA_ADDRESS_MASK;
    }
    list->last = packetAddress + SDF_DMA_EXTENDED_TAIL_OFFSET;
}

void sdfDrawTagLinkCall(sdfDrawTag_t *list, u32 packetAddress) {
    s32 previousTailAddress;

    ((SdfDmaTag *)packetAddress)->kind = SDF_DMA_TAG_CALL_BYTE;
    previousTailAddress = list->last;
    if (previousTailAddress == 0) {
        list->first = packetAddress;
    }
    else {
        ((SdfDmaTag *)previousTailAddress)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTag *)previousTailAddress)->address = packetAddress & SDF_DMA_ADDRESS_MASK;
    }
    list->last = packetAddress + SDF_QWORD_BYTES;
}

void sdfGrOtFuncLinkToHead(sdfGrOt_t *destinationList, sdfDrawTag_t *incomingList) {
    sdfDrawTag_t *firstList;

    if (incomingList->last == 0) {
        return;
    }
    firstList = destinationList->first;
    if (firstList == NULL) {
        destinationList->last = incomingList;
    } else {
        sdfGrJoinDrawTag(incomingList, firstList);
    }
    incomingList->nextList = firstList;
    destinationList->first = incomingList;
}

void sdfGrOtNormalReg(sdfGrOt_t *list, sdfDrawTag_t *item) {
    sdfDrawTag_t *last;

    if (item->first != 0) {
        last = list->last;
        if (last == NULL) {
            list->first = item;
        }
        else {
            last->nextList = item;
            sdfGrJoinDrawTag(last, item);
        }
        list->last = item;
    }
}

s32 sdfGrOtNormalCallback(sdfGrOt_t *list, s32 mode, sdfDrawTag_t *packet) {
    if (mode == 1) {
        sdfGrOtFuncLinkToHead(list, packet);
    }
}

void sdfGrOtInit(sdfGrOt_t *nodes, s32 count) {
    sdfGrOt_t *node = nodes;
    sdfGrOt_t *next;
    s32 remaining = count - 1;

    node->append = sdfGrOtNormalReg;
    node->prepend = sdfGrOtNormalCallback;
    node->first = 0;
    node->last = 0;
    node->unkC = 0;
    node->unk18 = 0;
    if (remaining == 0) {
        node->next = NULL;
        return;
    }
link:
    next = node + 1;
    remaining--;
    node->next = next;
    node = next;
    next->append = sdfGrOtNormalReg;
    next->prepend = sdfGrOtNormalCallback;
    next->first = 0;
    next->last = 0;
    next->unkC = 0;
    next->unk18 = 0;
    if (remaining != 0) {
        goto link;
    }
    node->next = NULL;
}

sdfGrRefPacketHeader_t *sdfGrNewExPacket(SdfDmaTag *sourceTag) {
    sdfGrRefPacketHeader_t *referenceNode = (sdfGrRefPacketHeader_t *)sdfGrAllocPacketBuff(SDF_DMA_REFERENCE_NODE_BYTES);
    u64 dmaHeader = sourceTag->quadwordCount;
    u32 payloadAddress = ((u32)sourceTag + SDF_QWORD_BYTES) & SDF_DMA_ADDRESS_MASK;
    s64 packedAddress = (s64)payloadAddress << 32;

    dmaHeader |= SDF_DMA_TAG_REF_WORD;
    dmaHeader |= packedAddress;
    referenceNode->dmaTag = dmaHeader;
    referenceNode->nextTag = 0;
    referenceNode->vifCommands = sourceTag->vifCommands;
    return referenceNode;
}

SdfDmaTag *sdfGrAddExPacket(SdfDmaTag *previousTag, SdfDmaTag *sourceTag) {
    sdfGrRefPacketHeader_t *referenceNode;

    referenceNode = sdfGrNewExPacket(sourceTag);
    previousTag->kind = SDF_DMA_TAG_NEXT_BYTE;
    previousTag->address = (u32)referenceNode & SDF_DMA_ADDRESS_MASK;
    return (SdfDmaTag *)((u32)referenceNode + SDF_QWORD_BYTES);
}

void sdfGrJoinDrawTag(sdfDrawTag_t *previousList, sdfDrawTag_t *incomingList) {
    SdfDmaTag *tailTag = (SdfDmaTag *)previousList->last;
    u32 previousSource;
    u32 incomingSource;

    previousSource = previousList->firstReferenceSource;
    incomingSource = incomingList->firstReferenceSource;
    if (previousSource != incomingSource) {
        if (incomingSource != 0) {
            tailTag = sdfGrAddExPacket(tailTag, (SdfDmaTag *)incomingSource);
        } else {
            incomingList->firstReferenceSource = previousSource;
        }
    }
    previousSource = previousList->secondReferenceSource;
    incomingSource = incomingList->secondReferenceSource;
    if (previousSource != incomingSource) {
        if (incomingSource != 0) {
            tailTag = sdfGrAddExPacket(tailTag, (SdfDmaTag *)incomingSource);
        } else {
            incomingList->secondReferenceSource = previousSource;
        }
    }
    tailTag->kind = SDF_DMA_TAG_NEXT_BYTE;
    tailTag->address = incomingList->first & SDF_DMA_ADDRESS_MASK;
}

void sdfGrInitExPacket(sdfDrawTag_t *list) {
    sdfGrRefPacketHeader_t *referenceNode;
    u32 priorHeadAddress;
    u32 sourceTagAddress;

    sourceTagAddress = list->secondReferenceSource;
    if (sourceTagAddress != 0) {
        referenceNode = sdfGrNewExPacket((SdfDmaTag *)sourceTagAddress);
        priorHeadAddress = list->first & SDF_DMA_ADDRESS_MASK;
        list->first = (u32)referenceNode;
        *(u64 *)&referenceNode->nextTag = ((s64)priorHeadAddress << 32) | SDF_DMA_TAG_NEXT_WORD;
    }
    sourceTagAddress = list->firstReferenceSource;
    if (sourceTagAddress != 0) {
        referenceNode = sdfGrNewExPacket((SdfDmaTag *)sourceTagAddress);
        priorHeadAddress = list->first & SDF_DMA_ADDRESS_MASK;
        list->first = (u32)referenceNode;
        *(u64 *)&referenceNode->nextTag = ((s64)priorHeadAddress << 32) | SDF_DMA_TAG_NEXT_WORD;
    }
}

sdfDrawTag_t *sdfGrOtClose(sdfGrOt_t *node) {
    sdfDrawTag_t *tail = NULL;
    sdfDrawTag_t *head = NULL;

    for (; node != NULL; node = node->next) {
        node->prepend(node, 0, NULL);
        if (node->first != 0) {
            if (head != 0) {
                sdfGrJoinDrawTag(tail, node->first);
            } else {
                head = node->first;
                sdfGrInitExPacket(head);
            }
            tail = node->last;
        }
    }
    if (tail != NULL) {
        ((SdfDmaTag *)tail->last)->kind = SDF_DMA_TAG_END_BYTE;
        ((SdfDmaTag *)tail->last)->address = 0;
    }
    return head;
}

void sdfPreDrawTagInit(sdfPreDrawTag_t *list) {
    list->unk0 = 0;
    list->first = 0;
    list->last = 0;
    list->unkC = 0;
}

void sdfPreDrawTagLink(sdfPreDrawTag_t *list, SdfPacketPatchLink *node) {
    if (list->last == 0) {
        list->first = node;
    }
    else {
        list->last->next = node;
    }
    list->last = node;
    node->next = NULL;
}

void sdfPreDrawChainInit(SdfPacketChain *chain) {
    chain->head = NULL;
    chain->tail = NULL;
}

void sdfPreDrawChainLink(SdfPacketChain *chain, sdfPreDrawTag_t *node) {
    if (chain->tail == NULL) {
        chain->head = node;
    }
    else {
        chain->tail->last->next = node->last;
    }
    chain->tail = node;
}

void sdfGrSetRegsPacketHeader(sdfGrPacketHeader_t *header, s32 payloadQwords) {
    s64 dmaQwords;

    dmaQwords = payloadQwords + 1;
    header->dmaWords.vif = (((dmaQwords | SDF_VIF_DIRECT_WORD) << 32) | SDF_VIF_FLUSHE_WORD);
    header->giftag.parm = (payloadQwords | (((s64)SDF_GIF_ONE_REGISTER_WORD << 32) | SDF_GIF_EOP_BIT));
    header->dmaWords.tag = dmaQwords;
    header->giftag.regs = SDF_GIF_REGISTER_AD;
}

void sdfGrSetRefGifPacket(SdfDmaReferenceChainPacket *packet, u32 sourceAddress, s32 qwordCount) {
    packet->header.dmaWords.tag = SDF_DMA_CNT_ONE_WORD;
    packet->header.dmaWords.vif = SDF_VIF_DIRECT_ONE_FLUSHE;
    packet->header.giftag.parm = (u64)qwordCount | SDF_GIF_PACKED_AD_BITS;
    packet->header.giftag.regs = SDF_GIF_REGISTER_AD;
    packet->referenceTag = (u32)((qwordCount & SDF_DMA_QWC_MASK) | SDF_DMA_TAG_REF_WORD) | ((u64)(sourceAddress & SDF_DMA_ADDRESS_MASK) << 32);
    packet->referenceVifCommands = SDF_VIF_DIRECT_ONE_NOP;
    packet->nextTag = SDF_DMA_TAG_NEXT_WORD;
    packet->zeroTailWord = 0;
}

void sdfGrSetDrawFramePacket(sdfGrDrawFramePacket_t *packet, s32 frameAddress, s32 width, s32 height,
                  s32 frameFormat, s32 depthAddress, s32 depthFormat,
                  s32 fieldOffset, s32 gsContext) {
    s64 frameRegisterId;
    s64 depthRegisterId;
    s64 offsetRegisterId;
    s64 scissorRegisterId;
    s32 xOffset;
    s32 yOffset;

    if (gsContext == 0) {
        frameRegisterId = SDF_GS_FRAME_1;
        depthRegisterId = SDF_GS_ZBUF_1;
        offsetRegisterId = SDF_GS_XYOFFSET_1;
        scissorRegisterId = SDF_GS_SCISSOR_1;
    } else {
        frameRegisterId = SDF_GS_FRAME_2;
        depthRegisterId = SDF_GS_ZBUF_2;
        offsetRegisterId = SDF_GS_XYOFFSET_2;
        scissorRegisterId = SDF_GS_SCISSOR_2;
    }
    packet->scissor.reg = scissorRegisterId;
    yOffset = (SDF_GS_CENTER_BIAS - height) << 3;
    xOffset = (SDF_GS_CENTER_BIAS - width) << 3;
    packet->frame.data = (s64)(frameFormat << 24) | (s64)(((width + 63) >> 6) << 16) | (s64)(frameAddress >> 11);
    if (fieldOffset != 0) {
        yOffset += SDF_GS_FIELD_OFFSET_STEP;
    }
    packet->frame.reg = frameRegisterId;
    packet->zbuf.data = (s64)(depthFormat << 24) | (s64)(depthAddress >> 11);
    packet->zbuf.reg = depthRegisterId;
    packet->xyoffset.reg = offsetRegisterId;
    packet->scissor.data = ((s64)(height - 1) << 48) | ((s64)(width - 1) << 16);
    packet->xyoffset.data = xOffset | ((s64)yOffset << 32);
}

void sdfGrSetClearPacket(sdfGrClearPacket_t *packet, s32 width, s32 height, s32 unused0, s32 unused1) {
    u32 lowerBounds = ((SDF_GS_CENTER_BIAS - height) << 19) | ((SDF_GS_CENTER_BIAS - width) << 3);
    u32 upperBounds = ((height + SDF_GS_CENTER_BIAS) << 19) | ((width + SDF_GS_CENTER_BIAS) << 3);

    packet->primitive.reg = SDF_GS_PRIM;
    packet->xyz2[0].reg = SDF_GS_XYZ2;
    packet->test.data = SDF_GS_CENTERED_VIEW_BOUNDS_TEST;
    packet->test.reg = SDF_GS_TEST_1;
    packet->primitive.data = SDF_GS_PRIMITIVE_SPRITE;
    packet->rgbaq.data = (u64)0xFE00 << 46;
    packet->rgbaq.reg = SDF_GS_RGBAQ;
    packet->xyz2[0].data = lowerBounds;
    packet->xyz2[1].data = upperBounds;
    packet->xyz2[1].reg = SDF_GS_XYZ2;
}

void sdfGrSetDrawDefParmPacket(SdfGsDrawDefaultsRegisters *packet) {
    packet->prmodecont.data = 1;
    packet->prmodecont.reg = SDF_GS_PRMODECONT;
    packet->colclamp.data = 1;
    packet->colclamp.reg = SDF_GS_COLCLAMP;
    packet->dthe.data = 0;
    packet->dthe.reg = SDF_GS_DTHE;
    packet->texa.data = SDF_GS_DEFAULT_TEXA;
    packet->texa.reg = SDF_GS_TEXA;
}

void sdfGrSetDrawFrame(sdfGrDrawFrame_t *packet, s32 frameAddress, s32 width, s32 height,
                           s32 frameFormat, s32 depthAddress, s32 depthFormat, s32 gsContext) {
    sdfGrSetRegsPacketHeader(&packet->header, SDF_SCENE_DRAW_PAYLOAD_QWORDS);
    sdfGrSetDrawFramePacket(&packet->registers, frameAddress, width, height, frameFormat, depthAddress, depthFormat, 0, gsContext);
}

void sdfGrSetDrawEnv(SdfSceneDrawPacket *packet, sdfGrFrameSet_t *view, s32 bufferIndex) {
    s32 frameAddress;
    s32 depthAddress;
    s32 width;
    s32 height;
    s32 frameFormat;
    s32 depthFormat;

    sdfGrSetRegsPacketHeader(&packet->header, SDF_TEXTURE_SCENE_PAYLOAD_QWORDS);
    frameAddress = view->buffers[bufferIndex]->word;
    depthAddress = view->buffers[SDF_GRAPH_DEPTH_BUFFER_INDEX]->word;
    width = view->width;
    frameFormat = view->bufferFormat;
    height = view->height;
    depthFormat = view->auxiliaryFormat;
    sdfGrSetDrawFramePacket(&packet->contextOne, frameAddress, width, height, frameFormat, depthAddress, depthFormat, D_0055188A, 0);
    sdfGrSetDrawFramePacket(&packet->contextTwo, frameAddress, width, height, frameFormat, depthAddress, depthFormat, D_0055188A, 1);
    sdfGrSetClearPacket(&packet->centeredBounds, view->width, view->height, view->bufferFormat, view->auxiliaryFormat);
    packet->blendState.contextOne.test.data = SDF_GS_SCENE_TEST;
    packet->blendState.contextOne.test.reg = SDF_GS_TEST_1;
    packet->blendState.contextOne.alpha.data = SDF_GS_DEFAULT_ALPHA;
    packet->blendState.contextOne.alpha.reg = SDF_GS_ALPHA_1;
    packet->blendState.contextTwo.test.data = SDF_GS_SCENE_TEST;
    packet->blendState.contextTwo.test.reg = SDF_GS_TEST_2;
    packet->blendState.contextTwo.alpha.data = SDF_GS_DEFAULT_ALPHA;
    packet->blendState.contextTwo.alpha.reg = SDF_GS_ALPHA_2;
    sdfGrSetDrawDefParmPacket(&packet->drawDefaults);
}

INCLUDE_ASM(const s32, "code_002AC138", sdfGrStdDrawEnvPredraw);

void sdfGrSetStdDrawEnv(sdfSceneNode_t *node, sdfGrFrameSet_t *view) {
    sdfGrSetRegsPacketHeader(&node->header, SDF_TEXTURE_SCENE_PAYLOAD_QWORDS);
    node->view = view;
    node->link.patch = sdfGrStdDrawEnvPredraw;
    sdfGrSetClearPacket(&node->centeredBounds, view->width, view->height, view->bufferFormat, view->auxiliaryFormat);
    node->blendState.contextOne.test.data = SDF_GS_SCENE_TEST;
    node->blendState.contextOne.test.reg = SDF_GS_TEST_1;
    node->blendState.contextOne.alpha.data = SDF_GS_DEFAULT_ALPHA;
    node->blendState.contextOne.alpha.reg = SDF_GS_ALPHA_1;
    node->blendState.contextTwo.test.data = SDF_GS_SCENE_TEST;
    node->blendState.contextTwo.test.reg = SDF_GS_TEST_2;
    node->blendState.contextTwo.alpha.data = SDF_GS_DEFAULT_ALPHA;
    node->blendState.contextTwo.alpha.reg = SDF_GS_ALPHA_2;
    sdfGrSetDrawDefParmPacket(&node->drawDefaults);
}

void sdfGrLinkStdDrawEnv(sdfDrawTag_t *dmaList, sdfPreDrawTag_t *linkedList, SdfPacketPatchLink *linkedNode) {
    sdfPreDrawTagLink(linkedList, linkedNode);
    sdfDrawTagLink(dmaList, (s32)linkedNode + SDF_QWORD_BYTES);
}

void sdfGrSetFrameMaskRGB(const u8 *source, SdfDmaReferenceChainPacket *packet, s32 gsContext) {
    if (gsContext == 0) {
        sdfGrSetRefGifPacket(packet, (u32)(source + 0x180), 1);
        return;
    }
    sdfGrSetRefGifPacket(packet, (u32)(source + 400), 1);
}

void sdfGrSetFrameMaskReset(const u8 *source, SdfDmaReferenceChainPacket *packet, s32 gsContext) {
    if (gsContext == 0) {
        sdfGrSetRefGifPacket(packet, (u32)(source + 0x70), 1);
        return;
    }
    sdfGrSetRefGifPacket(packet, (u32)(source + 0xb0), 1);
}

void sdfGrLinkDispFrameTex(sdfDrawTag_t *list, u32 source, sdfGrRefPacketHeader_t *node) {
    node->vifCommands = (((u64)0x50000004 << 16) | 0x1000) << 16;
    node->dmaTag = ((u64)((source + 0x1a0) & 0xfffffff) << 32) | 0x30000004;
    node->nextTag = 0;
    sdfDrawTagLinkRef(list, (u32)node);
}

void sdfGrLinkDrawFrameTex(sdfDrawTag_t *list, u32 source, sdfGrRefPacketHeader_t *node) {
    node->vifCommands = (((u64)0x50000004 << 16) | 0x1000) << 16;
    node->dmaTag = ((u64)((source + 0x1e0) & 0xfffffff) << 32) | 0x30000004;
    node->nextTag = 0;
    sdfDrawTagLinkRef(list, (u32)node);
}

void sdfGrFramePacketPredraw(sdfGrFramePacket_t *packet, s32 bufferIndex) {
    sdfGrFrameSet_t *view = packet->source;
    u64 frameRegister;
    u64 depthRegister;
    s16 widthPages;

    if (packet->mode == 0) {
        frameRegister = 0x4C;
        depthRegister = 0x4E;
    } else {
        frameRegister = 0x4D;
        depthRegister = 0x4F;
    }
    widthPages = (view->width + 63) >> 6;
    packet->frame.data = ((u64)packet->frameMask << 32) |
        (u64)(view->buffers[bufferIndex]->word >> 11) |
        ((s64)(view->bufferFormat << 24) | ((s64)widthPages << 16));
    packet->frame.reg = frameRegister;
    packet->zbuf.data = (s64)(view->auxiliaryFormat << 24) |
        (u64)(view->buffers[2]->word >> 11);
    packet->zbuf.reg = depthRegister;
}

void sdfGrSetFramePacket(sdfGrFramePacket_t *packet, sdfGrFrameSet_t *source, u32 frameMask, s32 region, s32 mode) {
    sdfGrSetRegsPacketHeader(&packet->packetHeader, 2);
    packet->mode = mode;
    packet->source = source;
    packet->frameMask = frameMask;
    packet->region = region;
    packet->prepare = sdfGrFramePacketPredraw;
}

void sdfGrReqDraw(sdfDrawTag_t *list, SdfPacketChain *chain) {
    u32 start = sdfGetRunTimeCountBase();
    s32 interrupts;
    s32 index;
    SdfPacketSlot *slot;

    while (sdfReqDrawDelayCount != 0) {
    }
    sdfReqDrawDelaySleepCount = sdfGetRunTimeCount(start);
    interrupts = DIntr();
    index = sdfDrawReqQueueIndex + 1;
    if (index == 3) {
        index = 0;
    }
    slot = &sdfGrDrawState[index];
    slot->packetList = NULL;
    slot->callbackHead = NULL;
    slot->slotUpdatePhase = 0;
    if (list != NULL && list->last != 0) {
        slot->packetList = list;
        if (chain != NULL) {
            slot->callbackHead = chain->head->first;
            if (slot->callbackHead != NULL) {
                chain->tail->last->next = NULL;
            }
        }
        slot->queuedBufferIndex = sdfFrameId;
    }
    sdfDrawReqQueueIndex = index;
    sdfDrawImageTrans = 1;
    sdfDrawFrameCount++;
    if (interrupts != 0) {
        EIntr();
    }
}

void sdfGrReqDrawDelay(s32 value) {
    sdfReqDrawDelayCount = (value < 0) ? 0 : value;
}

void sdfGrInitDraw(void) {
    sdfReqDrawDelayCount = 0;
    sdfGrDrawState[0].packetList = NULL;
    sdfDrawReqQueueIndex = 0;
}

INCLUDE_ASM(const s32, "code_002AC138", sdfGrDrawFrame);

void sdfDrawSync(void) {
    SdfPacketSlot *table = sdfGrDrawState;

    for (;;) {
        while (sdfDrawSyncFlag != 0) {
        }
        if (table[sdfDrawReqQueueIndex].packetList == NULL) {
            break;
        }
        if (table[sdfDrawReqQueueIndex].slotUpdatePhase >= 2) {
            break;
        }
        sdfWaitVblank(0);
    }
}

sdfDrawTag_t *sdfPrimAllocDrawTag(s32 (*allocator)(s32)) {
    sdfDrawTag_t *list;

    if (allocator == NULL) {
        allocator = sdfGrAllocPacketBuff;
    }
    list = (sdfDrawTag_t *)(u32)allocator(SDF_PACKET_LIST_BYTES);
    sdfDrawTagInit(list);
    return list;
}

void sdfPrimLinkSimplePacket(s32 listAddress, void (*initialize)(s32), s32 packetBytes, s32 (*allocator)(s32)) {
    s32 packetAddress;

    if (allocator == NULL) {
        allocator = sdfGrAllocPacketBuff;
    }
    packetAddress = allocator(packetBytes);
    initialize(packetAddress);
    sdfDrawTagLink(listAddress, packetAddress);
}

void sdfPrimSetOpaqueElem(sdfPrimTransElem_t *packet) {
    packet->test.data = SDF_GS_ALPHA_TEST;
    packet->test.reg = SDF_GS_TEST_1;
    packet->alpha.data = SDF_GS_DEFAULT_ALPHA;
    packet->alpha.reg = SDF_GS_ALPHA_1;
}

void sdfPrimSetOpaque2Elem(sdfPrimTransElem_t *packet) {
    packet->test.data = SDF_GS_ALPHA_TEST;
    packet->test.reg = SDF_GS_TEST_2;
    packet->alpha.data = SDF_GS_DEFAULT_ALPHA;
    packet->alpha.reg = SDF_GS_ALPHA_2;
}

void sdfPrimSetOpaquePacket(sdfPrimTransPacket_t *packet) {
    sdfPrimSetOpaqueElem(&packet->registers);
    packet->header.dmaWords.tag = SDF_ALPHA_DMA_QWORDS;
    packet->header.dmaWords.vif = (((u64)SDF_VIF_DIRECT_THREE_WORD << 16 | SDF_VIF_FLUSHE_HALFWORD) << 16);
    packet->header.giftag.parm = (((u64)SDF_GIF_ONE_REGISTER_WORD << 32) | SDF_GIF_TWO_AD_LOOPS_EOP);
    packet->header.giftag.regs = SDF_GIF_REGISTER_AD;
}

void sdfPrimSetOpaque2Packet(sdfPrimTransPacket_t *packet) {
    sdfPrimSetOpaque2Elem(&packet->registers);
    packet->header.dmaWords.tag = SDF_ALPHA_DMA_QWORDS;
    packet->header.dmaWords.vif = (((u64)SDF_VIF_DIRECT_THREE_WORD << 16 | SDF_VIF_FLUSHE_HALFWORD) << 16);
    packet->header.giftag.parm = (((u64)SDF_GIF_ONE_REGISTER_WORD << 32) | SDF_GIF_TWO_AD_LOOPS_EOP);
    packet->header.giftag.regs = SDF_GIF_REGISTER_AD;
}

void sdfPrimSetSemiTransElem(sdfPrimTransElem_t *packet) {
    packet->test.data = 0x717FB;
    packet->test.reg = 0x47;
    packet->alpha.data = 0x44;
    packet->alpha.reg = 0x42;
}

void sdfPrimSetSemiTrans2Elem(sdfPrimTransElem_t *packet) {
    packet->test.data = 0x717FB;
    packet->test.reg = 0x48;
    packet->alpha.data = 0x44;
    packet->alpha.reg = 0x43;
}

void sdfPrimSetSemiTransPacket(sdfPrimTransPacket_t *packet) {
    sdfPrimSetSemiTransElem(&packet->registers);
    packet->header.dmaWords.tag = 3;
    packet->header.dmaWords.vif = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->header.giftag.parm = (((u64)0x10000000 << 32) | 0x8002);
    packet->header.giftag.regs = 0xE;
}

void sdfPrimSetSemiTrans2Packet(sdfPrimTransPacket_t *packet) {
    sdfPrimSetSemiTrans2Elem(&packet->registers);
    packet->header.dmaWords.tag = 3;
    packet->header.dmaWords.vif = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->header.giftag.parm = (((u64)0x10000000 << 32) | 0x8002);
    packet->header.giftag.regs = 0xE;
}

void sdfPrimSetAddTransElem(sdfPrimTransElem_t *packet) {
    packet->test.data = 0x71801;
    packet->test.reg = 0x47;
    packet->alpha.data = 0x48;
    packet->alpha.reg = 0x42;
}

void sdfPrimSetAddTrans2Elem(sdfPrimTransElem_t *packet) {
    packet->test.data = 0x71801;
    packet->test.reg = 0x48;
    packet->alpha.data = 0x48;
    packet->alpha.reg = 0x43;
}

void sdfPrimSetAddTransPacket(sdfPrimTransPacket_t *packet) {
    sdfPrimSetAddTransElem(&packet->registers);
    packet->header.dmaWords.tag = 3;
    packet->header.dmaWords.vif = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->header.giftag.parm = (((u64)0x10000000 << 32) | 0x8002);
    packet->header.giftag.regs = 0xE;
}

void sdfPrimSetAddTrans2Packet(sdfPrimTransPacket_t *packet) {
    sdfPrimSetAddTrans2Elem(&packet->registers);
    packet->header.dmaWords.tag = 3;
    packet->header.dmaWords.vif = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->header.giftag.parm = (((u64)0x10000000 << 32) | 0x8002);
    packet->header.giftag.regs = 0xE;
}

void sdfPrimSetSubTransElem(sdfPrimTransElem_t *packet) {
    packet->test.data = 0x71801;
    packet->test.reg = 0x47;
    packet->alpha.data = 0x42;
    packet->alpha.reg = 0x42;
}

void sdfPrimSetSubTrans2Elem(sdfPrimTransElem_t *packet) {
    packet->test.data = 0x71801;
    packet->test.reg = 0x48;
    packet->alpha.data = 0x42;
    packet->alpha.reg = 0x43;
}

void sdfPrimSetSubTransPacket(sdfPrimTransPacket_t *packet) {
    sdfPrimSetSubTransElem(&packet->registers);
    packet->header.dmaWords.tag = 3;
    packet->header.dmaWords.vif = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->header.giftag.parm = (((u64)0x10000000 << 32) | 0x8002);
    packet->header.giftag.regs = 0xE;
}

void sdfPrimSetSubTrans2Packet(sdfPrimTransPacket_t *packet) {
    sdfPrimSetSubTrans2Elem(&packet->registers);
    packet->header.dmaWords.tag = 3;
    packet->header.dmaWords.vif = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->header.giftag.parm = (((u64)0x10000000 << 32) | 0x8002);
    packet->header.giftag.regs = 0xE;
}

void sdfPrimSetTexflushElem(sdfGrTagStdReg_t *write) {
    write->data = 0;
    write->reg = SDF_GS_TEXFLUSH;
}

void sdfPrimSetTexflushPacket(sdfPrimTexflushPacket_t *packet) {
    sdfPrimSetTexflushElem(&packet->textureFlush);
    packet->header.dmaWords.tag = 2;
    packet->header.dmaWords.vif = (((u64)0x50000002 << 16 | 0x1000) << 16);
    packet->header.giftag.parm = (((u64)0x10000000 << 32) | 0x8001);
    packet->header.giftag.regs = 0xE;
}

void sdfPrimSetBitBltElem(sdfPrimBitBltElem_t *packet, u32 destinationBufferAddress, s32 destinationBufferWidth,
                  s64 destinationFormat, s64 destinationX, s64 destinationY,
                  u32 sourceBufferAddress, s32 sourceBufferWidth, s32 sourceFormat,
                  s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight, s32 transferDirection) {
    packet->bitbltbuf.data = (destinationFormat << 56) |
        ((u64)(destinationBufferWidth >> 6) << 48) |
        ((u64)(destinationBufferAddress >> 6) << 32) |
        (sourceFormat << 24) | ((sourceBufferWidth >> 6) << 16) |
        (sourceBufferAddress >> 6);
    packet->bitbltbuf.reg = SDF_GS_BITBLTBUF;
    packet->trxpos.data = (destinationY << 48) | (destinationX << 32) |
        (sourceY << 16) | sourceX;
    packet->trxpos.reg = SDF_GS_TRXPOS;
    packet->trxreg.data = transferWidth | ((u64)transferHeight << 32);
    packet->trxreg.reg = SDF_GS_TRXREG;
    packet->trxdir.data = transferDirection;
    packet->trxdir.reg = SDF_GS_TRXDIR;
}

void sdfPrimSetBitBltPacket(sdfPrimBitBltPacket_t *packet, u32 destinationBufferAddress, s32 destinationBufferWidth,
                                    s64 destinationFormat, s64 destinationX, s64 destinationY,
                                    u32 sourceBufferAddress, s32 sourceBufferWidth, s32 sourceFormat,
                                    s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight, s32 transferDirection) {
    packet->header.dmaWords.tag = 5;
    packet->header.dmaWords.vif = (((u64)0x50000005 << 16 | 0x1000) << 16);
    packet->header.giftag.parm = (((u64)0x10000000 << 32) | 0x8004);
    packet->header.giftag.regs = 0xE;
    sdfPrimSetBitBltElem(&packet->transfer, destinationBufferAddress, destinationBufferWidth,
                  destinationFormat, destinationX, destinationY, sourceBufferAddress, sourceBufferWidth,
                  sourceFormat, sourceX, sourceY, transferWidth, transferHeight, transferDirection);
}

void sdfPrimLinkBitBltPacket(s32 packetList, u32 destinationBufferAddress, s32 destinationBufferWidth,
                             s64 destinationFormat, s64 destinationX, s64 destinationY,
                             u32 sourceBufferAddress, s32 sourceBufferWidth, s32 sourceFormat,
                             s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight,
                             s32 transferDirection, s32 (*allocator)(s32)) {
    s32 packetAddress;

    if (allocator == NULL) {
        allocator = sdfGrAllocPacketBuff;
    }
    packetAddress = allocator(0x60);
    sdfPrimSetBitBltPacket((sdfPrimBitBltPacket_t *)packetAddress, destinationBufferAddress, destinationBufferWidth,
                                    destinationFormat, destinationX, destinationY, sourceBufferAddress, sourceBufferWidth,
                                    sourceFormat, sourceX, sourceY, transferWidth, transferHeight, transferDirection);
    sdfDrawTagLink(packetList, packetAddress);
}

void sdfPrimFrameMovePredraw(SdfGraphCopyPacket *packet, s32 entryIndex) {
    packet->draw.transfer.bitbltbuf.data =
        (packet->draw.transfer.bitbltbuf.data & ~0x3FFF) |
        (u64)(u32)(D_00529388.buffers[entryIndex ^ packet->resourceIndexXor]->word >> 6);
}

void sdfPrimLinkFrameMovePacket(sdfDrawTag_t *drawList, sdfPreDrawTag_t *linkedList,
                   sdfGsMemSlice_t *destination, s32 destinationX, s32 destinationY,
                   s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight,
                   s32 resourceIndexXor, s32 (*allocPacket)(s32)) {
    SdfGraphCopyPacket *packet;
    sdfPrimBitBltPacket_t *drawPacket;

    if (allocPacket == NULL) {
        allocPacket = sdfGrAllocPacketBuff;
    }
    packet = (SdfGraphCopyPacket *)allocPacket(0x70);
    packet->resourceIndexXor = resourceIndexXor;
    packet->link.patch = sdfPrimFrameMovePredraw;
    drawPacket = &packet->draw;

    sdfPrimSetBitBltPacket(
        drawPacket, destination->word, destination->width,
        destination->format, destinationX, destinationY, D_00529388.buffers[0]->word,
        D_00529388.width, D_00529388.bufferFormat, sourceX, sourceY, transferWidth,
        transferHeight, 2);
    sdfPreDrawTagLink(linkedList, &packet->link);
    sdfDrawTagLink(drawList, (s32)drawPacket);
}

void sdfPrimSetSpriteElem(sdfPrimSpriteElem_t *packet, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet->gifTag = 0x6400000000008001ULL;
    packet->gifRegisters = 0x535310;
    packet->primitive = (u32)(primitive | 0x116);
    packet->rgbaq = (u32)color | ((u64)0xFE00 << 46);
    packet->vertices[0].uv = (u0 & 0xFFFF) | (v0 << 16);
    packet->vertices[0].xyz2 = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet->vertices[1].uv = (u1 & 0xFFFF) | (v1 << 16);
    packet->vertices[1].xyz2 = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
}

void sdfPrimLinkSprite(sdfDrawTag_t *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0,
                   s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 depth, s32 (*alloc)(s32)) {
    sdfPrimSpritePacket_t *packet;
    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (sdfPrimSpritePacket_t *)alloc(0x50);
    packet->dmaTag = 0x20000004;
    packet->vifCommands = (((u64)0x50000004 << 16) | 0x1000) << 16;
    sdfPrimSetSpriteElem(&packet->drawing, color, primitive, x0, y0, u0, v0, x1, y1, u1, v1, depth);
    sdfDrawTagLink(list, (s32)packet);
}

void sdfPrimSetF3Elem(sdfPrimF3Elem_t *dst, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 depth) {
    u64 depthHigh = (u64)depth << 32;
    u32 first = ((u32)x0 & 0xFFFF) | ((u32)y0 << 16);
    u32 second = ((u32)x1 & 0xFFFF) | ((u32)y1 << 16);
    u32 third = ((u32)x2 & 0xFFFF) | ((u32)y2 << 16);

    dst->gifTag = 0x6400000000008001ULL;
    dst->gifRegisterList = 0xF55510;
    dst->primitive = (u32)primitive | 0x104;
    dst->rgbaq = (u32)color | 0x3F80000000000000ULL;
    dst->xyz2[0] = first | depthHigh;
    dst->xyz2[1] = second | depthHigh;
    dst->xyz2[2] = third | depthHigh;
}

void sdfPrimLinkF3(sdfDrawTag_t *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1,
                   s32 y1, s32 x2, s32 y2, s32 depth, s32 (*alloc)(s32)) {
    sdfPrimF3Packet_t *packet;
    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (sdfPrimF3Packet_t *)alloc(0x50);
    packet->dmaTag = 0x20000004;
    packet->vifCommands = (((u64)0x50000004 << 16) | 0x1000) << 16;
    sdfPrimSetF3Elem(&packet->drawing, color, primitive, x0, y0, x1, y1, x2, y2, depth);
    sdfDrawTagLink(list, (s32)packet);
}

void sdfPrimSetF4Elem(sdfPrimF4Elem_t *packet, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 x3, s32 y3, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet->gifTag = 0x6400000000008001ULL;
    packet->gifRegisterList = 0x555510;
    packet->primitive = (u32)(primitive | 0x104);
    packet->rgbaq = (u32)color | ((u64)0xFE00 << 46);
    packet->xyz2[0] = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet->xyz2[1] = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    packet->xyz2[2] = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
    packet->xyz2[3] = (u32)((x3 & 0xFFFF) | (y3 << 16)) | depthHigh;
}

void sdfPrimLinkF4(sdfDrawTag_t *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 x3, s32 y3, s32 depth, s32 (*alloc)(s32)) {
    sdfPrimF4Packet_t *packet;

    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (sdfPrimF4Packet_t *)alloc(0x50);
    packet->dmaTag = 0x20000004ULL;
    packet->vifCommands = 0x5000000410000000ULL;
    sdfPrimSetF4Elem(&packet->drawing, color, primitive, x0, y0, x1, y1, x2, y2, x3, y3, depth);
    sdfDrawTagLink(list, (s32)packet);
}

void sdfPrimSetG3Elem(sdfPrimG3Elem_t *packet, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1, s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet->gifTag = 0x8400000000008001ULL;
    packet->gifRegisters = 0xFFFFFFFFF5151510ULL;
    packet->primitive = (u32)(primitive | 0x10C);
    packet->vertices[0].rgbaq = (u32)color0 | ((u64)0xFE00 << 46);
    packet->vertices[0].xyz2 = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet->vertices[1].rgbaq = (u32)color1 | ((u64)0xFE00 << 46);
    packet->vertices[1].xyz2 = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    packet->vertices[2].rgbaq = (u32)color2 | ((u64)0xFE00 << 46);
    packet->vertices[2].xyz2 = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
}

void sdfPrimLinkG3(sdfDrawTag_t *list, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1, s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 depth, s32 (*alloc)(s32)) {
    sdfPrimG3Packet_t *packet;

    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (sdfPrimG3Packet_t *)alloc(0x60);
    packet->dmaTag = 0x20000005ULL;
    packet->vifCommands = 0x5000000510000000ULL;
    sdfPrimSetG3Elem(&packet->drawing, primitive, x0, y0, color0, x1, y1, color1, x2, y2, color2, depth);
    sdfDrawTagLink(list, (s32)packet);
}

void sdfPrimSetG4Elem(sdfPrimG4Elem_t *packet, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1, s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 x3, s32 y3, s32 color3, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet->gifTag = 0xA400000000008001ULL;
    packet->gifRegisters = 0xF515151510ULL;
    packet->primitive = (u32)(primitive | 0x10C);
    packet->vertices[0].rgbaq = (u32)color0 | ((u64)0xFE00 << 46);
    packet->vertices[0].xyz2 = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet->vertices[1].rgbaq = (u32)color1 | ((u64)0xFE00 << 46);
    packet->vertices[1].xyz2 = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    packet->vertices[2].rgbaq = (u32)color2 | ((u64)0xFE00 << 46);
    packet->vertices[2].xyz2 = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
    packet->vertices[3].rgbaq = (u32)color3 | ((u64)0xFE00 << 46);
    packet->vertices[3].xyz2 = (u32)((x3 & 0xFFFF) | (y3 << 16)) | depthHigh;
}

void sdfPrimLinkG4(s32 list, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1,
                   s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 x3,
                   s32 y3, s32 color3, s32 depth, s32 (*alloc)(s32)) {
    sdfPrimG4Packet_t *packet;
    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (sdfPrimG4Packet_t *)alloc(0x70);
    packet->dmaTag = 0x20000006;
    packet->vifCommands = (((u64)0x50000006 << 16) | 0x1000) << 16;
    sdfPrimSetG4Elem(&packet->drawing, primitive, x0, y0, color0, x1, y1, color1, x2, y2, color2, x3, y3, color3, depth);
    sdfDrawTagLink(list, (s32)packet);
}

void sdfPrimSetFt3Elem(sdfPrimFt3Elem_t *packet, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2, s32 u2, s32 v2, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet->gifTag = 0x8400000000008001ULL;
    packet->gifRegisters = 0x53535310;
    packet->primitive = (u32)(primitive | 0x114);
    packet->rgbaq = (u32)color | ((u64)0xFE00 << 46);
    packet->vertices[0].uv = (u0 & 0xFFFF) | (v0 << 16);
    packet->vertices[0].xyz2 = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet->vertices[1].uv = (u1 & 0xFFFF) | (v1 << 16);
    packet->vertices[1].xyz2 = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    packet->vertices[2].uv = (u2 & 0xFFFF) | (v2 << 16);
    packet->vertices[2].xyz2 = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
}

void sdfPrimLinkFt3(s32 list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0,
                   s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2,
                   s32 u2, s32 v2, s32 depth, s32 (*alloc)(s32)) {
    sdfPrimFt3Packet_t *packet;
    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (sdfPrimFt3Packet_t *)alloc(0x60);
    packet->dmaTag = 0x20000005;
    packet->vifCommands = (((u64)0x50000005 << 16) | 0x1000) << 16;
    sdfPrimSetFt3Elem(&packet->drawing, color, primitive, x0, y0, u0, v0, x1, y1, u1, v1, x2, y2, u2, v2, depth);
    sdfDrawTagLink(list, (s32)packet);
}

void sdfPrimSetFt4Elem(sdfPrimFt4Elem_t *packet, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
                   s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2, s32 u2, s32 v2,
                   s32 x3, s32 y3, s32 u3, s32 v3, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet->gifTag = 0xA400000000008001ULL;
    packet->gifRegisters = 0x5353535310ULL;
    packet->primitive = (u32)(primitive | 0x114);
    packet->rgbaq = (u32)color | ((u64)0xFE00 << 46);
    packet->vertices[0].uv = (u0 & 0xFFFF) | (v0 << 16);
    packet->vertices[0].xyz2 = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet->vertices[1].uv = (u1 & 0xFFFF) | (v1 << 16);
    packet->vertices[1].xyz2 = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    packet->vertices[2].uv = (u2 & 0xFFFF) | (v2 << 16);
    packet->vertices[2].xyz2 = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
    packet->vertices[3].uv = (u3 & 0xFFFF) | (v3 << 16);
    packet->vertices[3].xyz2 = (u32)((x3 & 0xFFFF) | (y3 << 16)) | depthHigh;
}

void sdfPrimLinkFt4(sdfDrawTag_t *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0,
                   s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2,
                   s32 u2, s32 v2, s32 x3, s32 y3, s32 u3, s32 v3, s32 depth,
                   s32 (*alloc)(s32)) {
    sdfPrimFt4Packet_t *packet;
    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (sdfPrimFt4Packet_t *)alloc(0x70);
    packet->dmaTag = 0x20000006;
    packet->vifCommands = (((u64)0x50000006 << 16) | 0x1000) << 16;
    sdfPrimSetFt4Elem(&packet->drawing, color, primitive, x0, y0, u0, v0, x1, y1, u1, v1, x2, y2, u2, v2, x3, y3, u3, v3, depth);
    sdfDrawTagLink(list, (s32)packet);
}

void sdfPrimSetGt3Elem(sdfPrimGt3Elem_t *packet, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 color0,
                   s32 x1, s32 y1, s32 u1, s32 v1, s32 color1, s32 x2, s32 y2,
                   s32 u2, s32 v2, s32 color2, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet->gifTag = 0xA400000000008001ULL;
    packet->gifRegisters = 0x5135135130ULL;
    packet->primitive = (u32)(primitive | 0x11C);
    packet->vertices[0].uv = (u0 & 0xFFFF) | (v0 << 16);
    packet->vertices[0].rgbaq = (u32)color0 | ((u64)0xFE00 << 46);
    packet->vertices[0].xyz2 = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet->vertices[1].uv = (u1 & 0xFFFF) | (v1 << 16);
    packet->vertices[1].rgbaq = (u32)color1 | ((u64)0xFE00 << 46);
    packet->vertices[1].xyz2 = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    packet->vertices[2].uv = (u2 & 0xFFFF) | (v2 << 16);
    packet->vertices[2].rgbaq = (u32)color2 | ((u64)0xFE00 << 46);
    packet->vertices[2].xyz2 = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
}

void sdfPrimLinkGt3(s32 list, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
                   s32 color0, s32 x1, s32 y1, s32 u1, s32 v1, s32 color1,
                   s32 x2, s32 y2, s32 u2, s32 v2, s32 color2, s32 depth,
                   s32 (*alloc)(s32)) {
    sdfPrimGt3Packet_t *packet;
    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (sdfPrimGt3Packet_t *)alloc(0x70);
    packet->dmaTag = 0x20000006;
    packet->vifCommands = (((u64)0x50000006 << 16) | 0x1000) << 16;
    sdfPrimSetGt3Elem(&packet->drawing, primitive, x0, y0, u0, v0, color0, x1, y1, u1, v1, color1, x2, y2, u2, v2, color2, depth);
    sdfDrawTagLink(list, (s32)packet);
}

void sdfPrimSetGt4Elem(sdfPrimGt4Elem_t *packet, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 color0,
                   s32 x1, s32 y1, s32 u1, s32 v1, s32 color1, s32 x2, s32 y2, s32 u2,
                   s32 v2, s32 color2, s32 x3, s32 y3, s32 u3, s32 v3, s32 color3, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet->gifTag = 0xE400000000008001ULL;
    packet->gifRegisters = 0xF5135135135130ULL;
    packet->primitive = (u32)(primitive | 0x11C);
    packet->vertices[0].uv = (u0 & 0xFFFF) | (v0 << 16);
    packet->vertices[0].rgbaq = (u32)color0 | ((u64)0xFE00 << 46);
    packet->vertices[0].xyz2 = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet->vertices[1].uv = (u1 & 0xFFFF) | (v1 << 16);
    packet->vertices[1].rgbaq = (u32)color1 | ((u64)0xFE00 << 46);
    packet->vertices[1].xyz2 = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    packet->vertices[2].uv = (u2 & 0xFFFF) | (v2 << 16);
    packet->vertices[2].rgbaq = (u32)color2 | ((u64)0xFE00 << 46);
    packet->vertices[2].xyz2 = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
    packet->vertices[3].uv = (u3 & 0xFFFF) | (v3 << 16);
    packet->vertices[3].rgbaq = (u32)color3 | ((u64)0xFE00 << 46);
    packet->vertices[3].xyz2 = (u32)((x3 & 0xFFFF) | (y3 << 16)) | depthHigh;
}

void sdfPrimLinkGt4(sdfDrawTag_t *list, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
                   s32 color0, s32 x1, s32 y1, s32 u1, s32 v1, s32 color1,
                   s32 x2, s32 y2, s32 u2, s32 v2, s32 color2, s32 x3, s32 y3,
                   s32 u3, s32 v3, s32 color3, s32 depth, s32 (*alloc)(s32)) {
    sdfPrimGt4Packet_t *packet;
    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (sdfPrimGt4Packet_t *)alloc(0x90);
    packet->dmaTag = 0x20000008;
    packet->vifCommands = (((u64)0x50000008 << 16) | 0x1000) << 16;
    sdfPrimSetGt4Elem(&packet->drawing, primitive, x0, y0, u0, v0, color0, x1, y1, u1, v1, color1, x2, y2, u2, v2, color2, x3, y3, u3, v3, color3, depth);
    sdfDrawTagLink(list, (s32)packet);
}

void sdfPrimSetRectElem(SdfGsTwoVertexPayload *packet, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth) {
    u64 depthHigh = (u64)(u32)depth << 32;
    u32 first = ((u32)left & 0xFFFF) | ((u32)top << 16);
    u32 second = ((u32)right & 0xFFFF) | ((u32)bottom << 16);

    packet->gifTag = 0x4400000000008001ULL;
    packet->gifRegisters = 0x5510;
    packet->primitive = (u32)primitive | 0x106;
    packet->rgbaq = (u32)color | 0x3F80000000000000ULL;
    packet->xyz2[0] = first | depthHigh;
    packet->xyz2[1] = second | depthHigh;
}

void sdfPrimLinkRect(sdfDrawTag_t *list, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth, s32 (*alloc)(s32)) {
    SdfGsTwoVertexPacket *packet;

    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsTwoVertexPacket *)alloc(0x40);
    packet->dmaTag = 0x20000003ULL;
    packet->vifCommands = 0x5000000310000000ULL;
    sdfPrimSetRectElem(&packet->drawing, color, primitive, left, top, right, bottom, depth);
    sdfDrawTagLink(list, (u32)packet);
}

void sdfPrimSetLineElem(SdfGsTwoVertexPayload *packet, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth) {
    u64 depthHigh = (u64)(u32)depth << 32;
    u32 first = ((u32)left & 0xFFFF) | ((u32)top << 16);
    u32 second = ((u32)right & 0xFFFF) | ((u32)bottom << 16);

    packet->gifTag = 0x4400000000008001ULL;
    packet->gifRegisters = 0x5510;
    packet->primitive = (u32)primitive | 0x101;
    packet->rgbaq = (u32)color | 0x3F80000000000000ULL;
    packet->xyz2[0] = first | depthHigh;
    packet->xyz2[1] = second | depthHigh;
}

void sdfPrimLinkLine(sdfDrawTag_t *list, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth, s32 (*alloc)(s32)) {
    SdfGsTwoVertexPacket *packet;

    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsTwoVertexPacket *)alloc(0x40);
    packet->dmaTag = 0x20000003ULL;
    packet->vifCommands = 0x5000000310000000ULL;
    sdfPrimSetLineElem(&packet->drawing, color, primitive, left, top, right, bottom, depth);
    sdfDrawTagLink(list, (u32)packet);
}

void sdfPrimSetBoxLineElem(sdfPrimBoxLineElem_t *dst, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth) {
    u64 depthHigh = (u64)(u32)depth << 32;
    u32 x0 = (u32)left & 0xFFFF;
    u32 x1 = (u32)right & 0xFFFF;
    u32 y0 = (u32)top << 16;
    u32 y1 = (u32)bottom << 16;

    dst->gifTag = 0x8400000000008001ULL;
    dst->gifRegisterList = (s32)0xF5555510;
    dst->primitive = (u32)primitive | 0x102;
    dst->rgbaq = (u32)color | 0x3F80000000000000ULL;
    dst->xyz2[0] = (x0 | y0) | depthHigh;
    dst->xyz2[1] = (x1 | y0) | depthHigh;
    dst->xyz2[2] = (x1 | y1) | depthHigh;
    dst->xyz2[3] = (x0 | y1) | depthHigh;
    dst->xyz2[4] = (x0 | y0) | depthHigh;
}

void sdfPrimLinkBoxLine(sdfDrawTag_t *list, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth, s32 (*alloc)(s32)) {
    sdfPrimBoxLinePacket_t *packet;

    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (sdfPrimBoxLinePacket_t *)alloc(0x60);
    packet->dmaTag = 0x20000005ULL;
    packet->vifCommands = 0x5000000510000000ULL;
    sdfPrimSetBoxLineElem(&packet->drawing, color, primitive, left, top, right, bottom, depth);
    sdfDrawTagLink(list, (s32)packet);
}
INCLUDE_SDATA(const s32, "code_002AC138", sdfDelayedResrcSemaId);

INCLUDE_SDATA(const s32, "code_002AC138", sdfGrPacketBuffHandle);

INCLUDE_SDATA(const s32, "code_002AC138", sdfGrPacketBuffSize);

INCLUDE_SDATA(const s32, "code_002AC138", D_00551870);

INCLUDE_SDATA(const s32, "code_002AC138", sdfGrPacketBuffPtr);

INCLUDE_SDATA(const s32, "code_002AC138", sdfGrPacketBuffLimit);

INCLUDE_SDATA(const s32, "code_002AC138", D_00551880);

INCLUDE_SDATA(const s32, "code_002AC138", sdfDrawFrameFlag);

INCLUDE_SDATA(const s32, "code_002AC138", sdfDrawSyncFlag);

INCLUDE_SDATA(const s32, "code_002AC138", sdfDrawFrameCounter);

INCLUDE_SDATA(const s32, "code_002AC138", D_00551888);

INCLUDE_SDATA(const s32, "code_002AC138", sdfGrDrawFrameId);

INCLUDE_SDATA(const s32, "code_002AC138", D_0055188A);

INCLUDE_SDATA(const s32, "code_002AC138", sdfReqDrawDelayCount);

INCLUDE_SDATA(const s32, "code_002AC138", sdfReqDrawDelaySleepCount);

INCLUDE_SDATA(const s32, "code_002AC138", D_0055188E);

INCLUDE_SDATA(const s32, "code_002AC138", sdfDrawReqQueueIndex);

INCLUDE_SDATA(const s32, "code_002AC138", sdfGrDrawFrameTagHead);

INCLUDE_SDATA(const s32, "code_002AC138", sdfDrawFrameCount);

INCLUDE_SDATA(const s32, "code_002AC138", corePktSoftenBias);

INCLUDE_SDATA(const s32, "code_002AC138", D_0055189D);

INCLUDE_SDATA(const s32, "code_002AC138", sdfMatAllocCount);

INCLUDE_SDATA(const s32, "code_002AC138", sdfCoreDrawBuffHandle);

INCLUDE_SDATA(const s32, "code_002AC138", sdfFaceDivSoften);

INCLUDE_SDATA(const s32, "code_002AC138", sdfFaceDivBias);

