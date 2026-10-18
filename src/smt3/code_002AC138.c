#include "common.h"

struct SdfGsPacketHeader;
struct SdfGsRegisterWrite;
struct SdfGsGouraudTexturedVertex;
struct SdfGsTwoVertexPayload;
struct SdfGsTexturedVertex;
struct SdfGsGouraudVertex;
struct SdfGsFlatTrianglePayload;
struct SdfGsFlatQuadPayload;
struct SdfGsClosedRectanglePayload;
struct SdfGsBlendRegisters;
struct SdfGsTextureFlushPacket;
union MovObjStream;
struct SdfDmaNode;
struct SdfPendingRequest;
struct SdfPacketPatchLink;
struct SdfGsContextRegisters;
struct SdfGsCenteredBoundsRegisters;
struct SdfGsDrawDefaultsRegisters;
struct SdfGsGouraudTexturedTrianglePayload;
struct SdfGsGouraudTexturedQuadPayload;
struct SdfGsTwoVertexPacket;
struct SdfGsTexturedPairPayload;
struct SdfGsTexturedTrianglePayload;
struct SdfGsTexturedQuadPayload;
struct DevRequest;
struct SdfGsGouraudTrianglePayload;
struct SdfGsGouraudQuadPayload;
struct SdfDmaReferenceChainPacket;
struct SdfImageTransferRegisters;
struct SdfPendingBuffer;
struct SdfGsFlatTrianglePacket;
struct SdfGsFlatQuadPacket;
struct SdfGsClosedRectanglePacket;
struct SdfGsBlendPacket;
struct SdfGsContextPacket;
struct SdfGsSceneBlendRegisters;
struct SdfGsGouraudTexturedTrianglePacket;
struct SdfGsGouraudTexturedQuadPacket;
struct SdfGsTexturedPairPacket;
struct SdfGsTexturedTrianglePacket;
struct SdfGsTexturedQuadPacket;
struct SdfGsGouraudTrianglePacket;
struct SdfGsGouraudQuadPacket;
struct SdfImageTransferPacket;
struct SdfDescriptorPacket;
struct SdfSceneDrawPacket;
struct SdfThreadNode;
struct SdfTexResource;
struct IpuDmaState;
struct SdfStreamFrameNode;
struct MovObj;
struct SdfGraphObj;
struct SdfListHead;
struct SdfPoolNode;
struct SdfPacketBuilder;
struct SdfMemBlock;
struct SdfDmaTag;
struct SdfPacketSlot;
struct SdfResourcePacket;
struct SdfGraphCopyPacket;
struct SdfPatchableResourcePacket;
struct SdfLinkedPacketList;
struct SdfPacketChain;
struct DevState;
typedef int s128 __attribute__((mode(TI)));
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
/* Appending the packet patches the DMA tag's link; the VIF and GIF words
 * retain the renderer's original commands and register-list encoding. */
typedef struct SdfGsPacketHeader {
    u64 dmaTag;
    u64 vifCommands;
    u64 gifTag;
    u64 gifRegisters;
} QWORD_ALIGNED SdfGsPacketHeader;
typedef struct SdfGsRegisterWrite {
    u64 value;
    u64 registerId;
} SdfGsRegisterWrite;
/* One 0x8C allocation owns the stream/sound links and IPU transfer state. */
struct SdfStreamInputDmaTag;
struct SoundFormat;
/* The +0x1C movie work pointer selects one of two separately allocated records.
 * isPac at +3 chooses the active arm; the variant payload types live in
 * sdf_movie_stream.h. */
struct MovLinearStream;
struct MovPacStream;
typedef struct SdfPendingNode SdfPendingNode;
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
typedef struct SdfGsFlatTrianglePayload {
    u64 gifTag;
    u64 gifRegisterList;
    u64 primitive;
    u64 rgbaq;
    u64 xyz2[3];
} SdfGsFlatTrianglePayload;
/* GIF stream for four flat-color vertices: PRIM, RGBAQ and four XYZ2 values. */
typedef struct SdfGsFlatQuadPayload {
    u64 gifTag;
    u64 gifRegisterList;
    u64 primitive;
    u64 rgbaq;
    u64 xyz2[4];
} SdfGsFlatQuadPayload;
/* GIF stream for a closed rectangle: its first XYZ2 value is repeated last. */
typedef struct SdfGsClosedRectanglePayload {
    u64 gifTag;
    u64 gifRegisterList;
    u64 primitive;
    u64 rgbaq;
    u64 xyz2[5];
} SdfGsClosedRectanglePayload;
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
/* Protocol IDs use distinct byte-sized storage codes in DevState. */
typedef u8 SdfDevOperationCode;
typedef s8 SdfDevStateCode;
/* TEST and ALPHA form the two A+D writes shared by the blend builders. */
typedef struct SdfGsBlendRegisters {
    SdfGsRegisterWrite test;
    SdfGsRegisterWrite alpha;
} SdfGsBlendRegisters;
/* A single A+D write follows the texture-flush DMA/VIF/GIF header. */
typedef struct SdfGsTextureFlushPacket {
    SdfGsPacketHeader header;
    SdfGsRegisterWrite textureFlush;
} SdfGsTextureFlushPacket;
typedef union MovObjStream {
    struct MovLinearStream *linear;
    struct MovPacStream *pac;
} MovObjStream;
/* REF tag and VIF commands followed by the trailing chain tag (0x20).
 * The complete trailing quadword is cleared before its NEXT header is patched. */
typedef struct SdfDmaNode {
    u64 dmaTag;
    u64 vifCommands;
    int __attribute__((mode(TI))) nextTag;
} SdfDmaNode;
/* Interrupt-synchronized deferred callback owner (0x08). */
typedef struct SdfPendingRequest {
    SdfPendingCallback handler;
    SdfPendingNode *pending;
} SdfPendingRequest;
typedef struct SdfPacketPatchLink {
    struct SdfPacketPatchLink *next;
    SdfPacketPatchCallback patch;
} SdfPacketPatchLink;
/* Four A+D writes define one GS drawing context, without a DMA/GIF header. */
typedef struct SdfGsContextRegisters {
    SdfGsRegisterWrite frame;
    SdfGsRegisterWrite zbuf;
    SdfGsRegisterWrite xyoffset;
    SdfGsRegisterWrite scissor;
} SdfGsContextRegisters;
/* A+D writes that draw the centered viewport bounds as a sprite. */
typedef struct SdfGsCenteredBoundsRegisters {
    SdfGsRegisterWrite test;
    SdfGsRegisterWrite primitive;
    SdfGsRegisterWrite rgbaq;
    SdfGsRegisterWrite xyz2[2];
} SdfGsCenteredBoundsRegisters;
/* Shared A+D state for GS primitive modes, color clamping, dithering and TEXA. */
typedef struct SdfGsDrawDefaultsRegisters {
    SdfGsRegisterWrite prmodecont;
    SdfGsRegisterWrite colclamp;
    SdfGsRegisterWrite dthe;
    SdfGsRegisterWrite texa;
} SdfGsDrawDefaultsRegisters;
/* PRIM followed by 3 individually textured and colored vertices. */
typedef struct SdfGsGouraudTexturedTrianglePayload {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    SdfGsGouraudTexturedVertex vertices[3];
} SdfGsGouraudTexturedTrianglePayload;
/* PRIM followed by 4 individually textured and colored vertices. */
typedef struct SdfGsGouraudTexturedQuadPayload {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    SdfGsGouraudTexturedVertex vertices[4];
} SdfGsGouraudTexturedQuadPayload;
/* DMA/VIF header followed by the complete two-vertex GIF payload. */
typedef struct SdfGsTwoVertexPacket {
    u64 dmaTag;
    u64 vifCommands;
    SdfGsTwoVertexPayload drawing;
} SdfGsTwoVertexPacket;
typedef struct SdfGsTexturedPairPayload {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    u64 rgbaq;
    SdfGsTexturedVertex vertices[2];
} SdfGsTexturedPairPayload;
/* GIF REGLIST stream: PRIM, one RGBAQ, then UV/XYZ2 for each vertex. */
typedef struct SdfGsTexturedTrianglePayload {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    u64 rgbaq;
    SdfGsTexturedVertex vertices[3];
} SdfGsTexturedTrianglePayload;
/* GIF REGLIST stream: PRIM, one RGBAQ, then UV/XYZ2 for each vertex. */
typedef struct SdfGsTexturedQuadPayload {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    u64 rgbaq;
    SdfGsTexturedVertex vertices[4];
} SdfGsTexturedQuadPayload;
typedef struct DevRequest {
    struct SdfMemBlock *backingAllocation;
    s16 usedCount;
    u16 capacity;
    s16 stride;
    s16 growStep;
    void *buffer;
} DevRequest;
/* PRIM followed by three independently colored vertices. */
typedef struct SdfGsGouraudTrianglePayload {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    SdfGsGouraudVertex vertices[3];
} SdfGsGouraudTrianglePayload;
/* PRIM followed by four independently colored vertices. */
typedef struct SdfGsGouraudQuadPayload {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    SdfGsGouraudVertex vertices[4];
} SdfGsGouraudQuadPayload;
/* GIF tag followed by a REF transfer and the patchable NEXT link. */
typedef struct SdfDmaReferenceChainPacket {
    SdfGsPacketHeader header;
    u64 referenceTag;
    u64 referenceVifCommands;
    u64 nextTag;
    u64 zeroTailWord;
} SdfDmaReferenceChainPacket;
void sdfMemFreeDirect(struct SdfMemBlock *allocation);
/* The four A+D writes controlling a GS image transfer. */
typedef struct SdfImageTransferRegisters {
    SdfGsRegisterWrite bitbltbuf;
    SdfGsRegisterWrite trxpos;
    SdfGsRegisterWrite trxreg;
    SdfGsRegisterWrite trxdir;
} SdfImageTransferRegisters;
/* A queued batch links its next chunk before 63 deferred owner addresses. */
typedef struct SdfPendingBuffer {
    struct SdfPendingBuffer *next;
    void *entry[SDF_PENDING_BUFFER_CAPACITY];
} SdfPendingBuffer;
/* The caller prepends DMA/VIF words to the triangle GIF stream. Allocation
 * reserves 0x50 bytes; the final qword is not written by these helpers. */
typedef struct SdfGsFlatTrianglePacket {
    u64 dmaTag;
    u64 vifCommands;
    SdfGsFlatTrianglePayload drawing;
    u64 unwrittenAllocationWord;
} SdfGsFlatTrianglePacket;
/* The allocation contains the DMA/VIF prefix and the entire quad GIF stream. */
typedef struct SdfGsFlatQuadPacket {
    u64 dmaTag;
    u64 vifCommands;
    SdfGsFlatQuadPayload drawing;
} SdfGsFlatQuadPacket;
/* The caller prepends DMA/VIF words. The final allocated qword is not written. */
typedef struct SdfGsClosedRectanglePacket {
    u64 dmaTag;
    u64 vifCommands;
    SdfGsClosedRectanglePayload drawing;
    u64 unwrittenAllocationWord;
} SdfGsClosedRectanglePacket;
extern SdfPendingNode *sdfDelayedResrcLink;
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




extern struct SdfMemBlock *sdfGrPacketBuffHandle;
/* Fixed packets allocated by the paired blend and composite-quad renderers. */
typedef struct SdfGsBlendPacket {
    SdfGsPacketHeader header;
    SdfGsBlendRegisters registers;
} SdfGsBlendPacket;
/* Accessed output span of the common scene-context builder. */
typedef struct SdfGsContextPacket {
    SdfGsPacketHeader header;
    SdfGsContextRegisters registers;
} SdfGsContextPacket;
/* TEST/ALPHA A+D state for the two independent GS drawing contexts. */
typedef struct SdfGsSceneBlendRegisters {
    SdfGsBlendRegisters contextOne;
    SdfGsBlendRegisters contextTwo;
} SdfGsSceneBlendRegisters;
typedef struct SdfGsGouraudTexturedTrianglePacket {
    u64 dmaTag;
    u64 vifCommands;
    SdfGsGouraudTexturedTrianglePayload drawing;
} SdfGsGouraudTexturedTrianglePacket;
typedef struct SdfGsGouraudTexturedQuadPacket {
    u64 dmaTag;
    u64 vifCommands;
    SdfGsGouraudTexturedQuadPayload drawing;
    u64 unwrittenTail; /* Final allocated word is not written by the builder. */
} SdfGsGouraudTexturedQuadPacket;
typedef struct SdfGsTexturedPairPacket {
    u64 dmaTag;
    u64 vifCommands;
    SdfGsTexturedPairPayload drawing;
} SdfGsTexturedPairPacket;
typedef struct SdfGsTexturedTrianglePacket {
    u64 dmaTag;
    u64 vifCommands;
    SdfGsTexturedTrianglePayload drawing;
} SdfGsTexturedTrianglePacket;
typedef struct SdfGsTexturedQuadPacket {
    u64 dmaTag;
    u64 vifCommands;
    SdfGsTexturedQuadPayload drawing;
} SdfGsTexturedQuadPacket;
typedef struct SdfGsGouraudTrianglePacket {
    u64 dmaTag;
    u64 vifCommands;
    SdfGsGouraudTrianglePayload drawing;
    u64 unwrittenTail; /* Allocation rounds the REGLIST payload up to a qword. */
} SdfGsGouraudTrianglePacket;
typedef struct SdfGsGouraudQuadPacket {
    u64 dmaTag;
    u64 vifCommands;
    SdfGsGouraudQuadPayload drawing;
    u64 unwrittenTail; /* Allocation rounds the REGLIST payload up to a qword. */
} SdfGsGouraudQuadPacket;
/* DMA/VIF and GIF header followed by the complete image-transfer register block. */
typedef struct SdfImageTransferPacket {
    SdfGsPacketHeader header;
    SdfImageTransferRegisters transfer;
} SdfImageTransferPacket;
/* Host-to-local image transfer packet followed by its flush terminal tag (0xB0). */
typedef struct SdfDescriptorPacket {
    u64 header[4];
    SdfImageTransferRegisters transfer;
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
} SdfDescriptorPacket;
/* Complete DMA/GIF owner used by scene builders and held-texture draw banks. */
typedef struct SdfSceneDrawPacket {
    SdfGsPacketHeader header;
    SdfGsDrawDefaultsRegisters drawDefaults;
    SdfGsContextRegisters contextOne;
    SdfGsContextRegisters contextTwo;
    SdfGsCenteredBoundsRegisters centeredBounds;
    SdfGsSceneBlendRegisters blendState;
} SdfSceneDrawPacket;
/* Linked registry node stored in the title's tracked-thread table (0x8). */
typedef struct SdfThreadNode {
    struct SdfThreadNode *next; /* 0x00 */
    s32 threadId;               /* 0x04 */
} SdfThreadNode;
/* Register a thread node, then start its entry function on the supplied stack. */
void sdfStartThreadAndWait(SdfThreadNode *node, void (*entry)(void),
                           void *stack, s64 stackSize, s32 priority, s32 arg);
/* Native VRAM range descriptor (0x1C), shared by textures, graph buffers and streams. */
typedef struct SdfTexResource {
    struct SdfTexResource *next; /* 0x00 */
    struct SdfTexResource *prev; /* 0x04 */
    u32 allocationMode;         /* 0x08: known categories above; unknown values remain valid */
    u32 word;                   /* 0x0C: VRAM offset in 32-bit words */
    s32 size;                   /* 0x10: range length in 32-bit words */
    s16 width;                  /* 0x14 */
    s16 height;                 /* 0x16 */
    s32 format;                 /* 0x18: GS pixel-storage mode */
} SdfTexResource;
/* Allocate a low-VRAM image block and record its logical dimensions and format.
 * The caller owns the returned block and releases it with the masked updater below. */
SdfTexResource *sdfGsAllocFrameBuff(s32 width, s32 height, s32 format);
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
typedef struct SdfStreamFrameNode {
    struct SdfStreamFrameNode *streamPrev;
    struct SdfStreamFrameNode *streamNext;
    struct SdfStreamFrameNode *next;
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
    SdfTexResource *textureHead;
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
    s32 (*read)(struct SdfStreamFrameNode *, struct MovObj *, s32, void *, s32);
    struct MovObj *readContext; /* Owner context passed to the read callback. */
    u8 inputFeedDmaInFlight;
    u8 unk65;
    u8 pad66[2];
    IpuDmaState dma;
} SdfStreamFrameNode;
typedef s32 (*SdfStreamRead)(SdfStreamFrameNode *, struct MovObj *, s32, void *, s32);
void sdfIpuStrmDecodeInit(SdfStreamFrameNode *node, struct SoundFormat *format, SdfStreamRead read, struct MovObj *readContext);
void sdfSoundInitFormattedAndAppendNode(SdfStreamFrameNode *node, struct SoundFormat *format, SdfStreamRead read, struct MovObj *readContext, s32 resource);
typedef struct MovObj {
    u8 active;
    u8 state;
    u8 stopRequested;
    u8 isPac;
    u16 unk04;
    u16 unk06;
    u32 unk08;
    u16 unk0C;
    u16 unk0E;
    struct DevState *deviceState;
    s32 totalBytes;
    s32 remainingBytes;
    MovObjStream stream;
    u8 pacEnabled;
    u8 pad21;
    u8 packetLimit;
    u8 pad23;
    SdfStreamFrameNode soundNode;
} MovObj;
/* Graph target: two color buffers followed by the auxiliary/depth buffer (0x14). */
typedef struct SdfGraphObj {
    s16 width;
    s16 unk2;
    s16 height;
    u8 bufferFormat;
    u8 auxiliaryFormat;
    SdfTexResource *buffers[3];
} SdfGraphObj;
/* Semaphore ID and attached work pointers (0x14); DDS1/2 game/code_002D10B0/00329F60.c. */


/* DMA packet list cursors and endpoints (0x20); DDS1/2 game/code_002D33C8/0032C278.c. */
typedef struct SdfListHead {
    struct SdfListHead *nextList; /* CPU link between lists; first/last are DMA addresses. */
    u32 first;
    u32 last;
    u32 unkC;
    u32 firstReferenceSource; /* Optional first DMA reference prefix. */
    u32 secondReferenceSource; /* Optional second DMA reference prefix. */
    u32 unk18;
    u32 unk1C;
} SdfListHead;
/* Draw-surface pool entry (0x20); the SDK initializes and flushes this owner. */
typedef struct SdfPoolNode {
    struct SdfPoolNode *next; /* 0x00 */
    SdfListHead *first;       /* 0x04 */
    SdfListHead *last;        /* 0x08 */
    u32 unkC;
    void (*append)(struct SdfPoolNode *, SdfListHead *);      /* 0x10 */
    s32 (*prepend)(struct SdfPoolNode *, s32, SdfListHead *); /* 0x14 */
    u32 unk18;
    u32 unk1C;
} SdfPoolNode;
/* Frame/depth builder state: one DMA/GIF header and two A+D register writes. */
typedef struct SdfPacketBuilder {
    u8 pad00[4];
    void (*prepare)(struct SdfPacketBuilder *, s32 bufferIndex);
    u8 pad08[8];
    SdfGsPacketHeader packetHeader;
    SdfGsRegisterWrite frame;
    SdfGsRegisterWrite zbuf;
    SdfGraphObj *source;
    u32 frameMask;
    s32 region;
    s32 mode;
} SdfPacketBuilder;
/* General-heap allocation descriptor (0x10); links bound the represented span. */
typedef struct SdfMemBlock {
    struct SdfMemBlock *prev; /* 0x00 */
    struct SdfMemBlock *next; /* 0x04 */
    s32 address;              /* 0x08: start of represented span */
    u16 state;                /* 0x0C: free, used, or end sentinel */
    s16 referenceCount;       /* 0x0E: -1 for sentinels */
} SdfMemBlock;
void sdfBuildTextureScenePacket(SdfSceneDrawPacket *packet,
    struct SdfGraphObj *view, s32 bufferIndex);
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
void sdfDrawTagLink2(struct SdfListHead *list, u32 packetAddress,
                          u32 rangeTailAddress);
void sdfDrawTagLinkRef(struct SdfListHead *list, u32 packetAddress);
/* One 0x10-byte entry in the three-slot frame packet submission ring. */
typedef struct SdfPacketSlot {
    struct SdfListHead *packetList;
    SdfPacketPatchLink *callbackHead;
    u8 slotUpdatePhase;
    u8 queuedBufferIndex;
    u8 bufferIndex;
    u8 bufferSlotIndex;
    u8 pad0C[4];
} SdfPacketSlot;
void *sdfChipAllocDirect(s32 size);
struct SdfMemBlock *sdfMemAllocDirect(s32 requestedBytes);
u32 sdfMemLock(struct SdfMemBlock *allocation);
/* Resource upload payload and its terminal primitive-reset tag (0xF0). */
typedef struct SdfResourcePacket {
    u64 header[14];
    SdfImageTransferRegisters transfer;
    s32 quadwordCount;
    u32 unkB4;
    u32 unkB8;
    u32 unkBC;
    u64 tail[6];
} SdfResourcePacket;
/* Metadata node plus the extended local-to-local image-copy payload (0x70). */
typedef struct SdfGraphCopyPacket {
    SdfPacketPatchLink link;
    u32 resourceIndexXor; /* Selects the source resource for the queued buffer. */
    u32 unk0C;
    SdfImageTransferPacket draw;
} SdfGraphCopyPacket;
/* Metadata node followed by the resource-transfer payload (0x100). */
typedef struct SdfPatchableResourcePacket {
    SdfPacketPatchLink link;
    u32 unk08;
    u32 unk0C;
    SdfResourcePacket resourcePacket;
} SdfPatchableResourcePacket;
/* Queue a retained resource for deferred release; otherwise destroy it now. */
void sdfMatListFreeResrc(void *resource, s32 retained);
/* Complete linked-packet list, distinct from the 0x20-byte DMA list head. */
typedef struct SdfLinkedPacketList {
    u32 unk0;
    SdfPacketPatchLink *first;
    SdfPacketPatchLink *last;
    u32 unkC;
} SdfLinkedPacketList;
/* Frame submission joins these 0x10-byte linked owners through head/tail. */
typedef struct SdfPacketChain {
    SdfLinkedPacketList *head;
    SdfLinkedPacketList *tail;
} SdfPacketChain;
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
extern SdfGraphObj D_00529388;
/* Complete 0x40-byte owner allocated by sdfFileCmdNewHandle. */
typedef struct DevState {
    struct DevState *next; /* 0x00 */
    struct DevState *previous; /* 0x04 */
    struct DevState *workerNext; /* 0x08 */
    struct DevState *workerPrev; /* 0x0C */
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
    void (*callback)(struct DevState *, s32, s32, s32, s32); /* 0x38 */
    s32 callbackContext; /* 0x3C: opaque callback-context word */
} DevState;
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
struct SdfPendingNode {
    SdfPendingNode *next;
    SdfPendingRequest *owner;
    SdfPendingBuffer *buffer;
    s32 remaining;
};

void sdfSetStoreImage(SdfResourcePacket *packet, SdfTexResource *source,
    s32 sourceX, s32 sourceY, s32 width, s32 height, u32 resourceAddress, u32 arg7, u32 arg8);
void sdfLinkStoreImage(SdfListHead *list, SdfTexResource *source, s32 sourceX,
                   s32 sourceY, s32 width, s32 height, s32 resourceAddress, s32 arg7,
                   s32 arg8, s32 (*allocate)(s32));
void sdfLinkStoreFrameImagePredraw(SdfPatchableResourcePacket *packet, s32 entryIndex);
void sdfLinkStoreFrameImage(SdfListHead *list, SdfLinkedPacketList *linkedList, s32 sourceX, s32 sourceY,
                   s32 width, s32 height, s32 resourceAddress, s32 arg7, s32 arg8,
                   s32 (*allocatePacket)(s32));
void sdfSetLoadImage(SdfDescriptorPacket *packet, SdfTexResource *source,
                   s64 destinationX, s64 destinationY, s32 transferWidth,
                   s32 transferHeight, u32 sourceAddress);
void sdfLinkLoadImage(SdfListHead *list, SdfTexResource *source,
                   s32 destinationX, s32 destinationY, s32 width, s32 height,
                   s32 sourceAddress, s32 (*allocatePacket)(s32));
void sdfDelayedResrcInitCB(SdfPendingRequest *request, SdfPendingCallback callback);
void sdfDelayedResrcEntry(SdfPendingRequest *owner, void *entry);
SdfPendingNode *sdfDelayedResrcClose(void);
void sdfDelayedResrcRelease(SdfPendingNode *pendingNode);
u64 sdfDelayedResrcCheckFunc(void);
u64 sdfDelayedResrcCheck(void);
void sdfGrMakePacketBuff(s32 bufferBytes);
void sdfGrSetupPacketBuff(s32 bufferIndex);
s32 sdfGrAllocPacketBuff(s32 size);
void sdfGrClosePacketBuff(s32 cursorAddress);
void sdfDrawTagInit(SdfListHead *list);
void sdfDrawTagLink(SdfListHead *list, u32 packet);
void sdfDrawTagLinkRefGif(SdfListHead *list, u32 packetAddress);
void sdfDrawTagLinkCall(SdfListHead *list, u32 packetAddress);
void sdfGrOtFuncLinkToHead(SdfPoolNode *destinationList, SdfListHead *incomingList);
void sdfGrOtNormalReg(SdfPoolNode *list, SdfListHead *item);
s32 sdfGrOtNormalCallback(SdfPoolNode *list, s32 mode, SdfListHead *packet);
void sdfGrOtInit(SdfPoolNode *nodes, s32 count);
SdfDmaNode *sdfGrNewExPacket(SdfDmaTag *sourceTag);
SdfDmaTag *sdfGrAddExPacket(SdfDmaTag *previousTag, SdfDmaTag *sourceTag);
void sdfGrJoinDrawTag(SdfListHead *previousList, SdfListHead *incomingList);
void sdfGrInitExPacket(SdfListHead *list);
SdfListHead *sdfGrOtClose(SdfPoolNode *node);
void sdfPreDrawTagLink(SdfLinkedPacketList *list, SdfPacketPatchLink *node);
void sdfPreDrawChainLink(SdfPacketChain *chain, SdfLinkedPacketList *node);
void sdfGrSetRegsPacketHeader(SdfGsPacketHeader *header, s32 payloadQwords);
void sdfGrSetDrawFramePacket(SdfGsContextRegisters *packet, s32 frameAddress, s32 width, s32 height,
                  s32 frameFormat, s32 depthAddress, s32 depthFormat,
                  s32 fieldOffset, s32 gsContext);
void sdfGrSetClearPacket(SdfGsCenteredBoundsRegisters *packet, s32 width, s32 height, s32 unused0, s32 unused1);
void sdfGrSetDrawFrame(SdfGsContextPacket *packet, s32 frameAddress, s32 width, s32 height,
                           s32 frameFormat, s32 depthAddress, s32 depthFormat, s32 gsContext);
void sdfGrLinkStdDrawEnv(SdfListHead *dmaList, SdfLinkedPacketList *linkedList, SdfPacketPatchLink *linkedNode);
void sdfGrLinkDispFrameTex(SdfListHead *list, u32 source, SdfDmaNode *node);
void sdfGrLinkDrawFrameTex(SdfListHead *list, u32 source, SdfDmaNode *node);
void sdfGrFramePacketPredraw(SdfPacketBuilder *packet, s32 bufferIndex);
void sdfGrSetFramePacket(SdfPacketBuilder *packet, SdfGraphObj *source, u32 frameMask, s32 region, s32 mode);
void sdfGrReqDrawDelay(s32 value);
void sdfGrInitDraw(void);
SdfListHead *sdfPrimAllocDrawTag(s32 (*allocator)(s32));
void sdfPrimLinkSimplePacket(s32 listAddress, void (*initialize)(s32), s32 packetBytes, s32 (*allocator)(s32));
void sdfPrimSetOpaqueElem(SdfGsBlendRegisters *packet);
void sdfPrimSetOpaque2Elem(SdfGsBlendRegisters *packet);
void sdfPrimSetOpaquePacket(SdfGsBlendPacket *packet);
void sdfPrimSetOpaque2Packet(SdfGsBlendPacket *packet);
void sdfPrimSetSemiTransElem(SdfGsBlendRegisters *packet);
void sdfPrimSetSemiTrans2Elem(SdfGsBlendRegisters *packet);
void sdfPrimSetSemiTransPacket(SdfGsBlendPacket *packet);
void sdfPrimSetSemiTrans2Packet(SdfGsBlendPacket *packet);
void sdfPrimSetAddTransElem(SdfGsBlendRegisters *packet);
void sdfPrimSetAddTrans2Elem(SdfGsBlendRegisters *packet);
void sdfPrimSetAddTransPacket(SdfGsBlendPacket *packet);
void sdfPrimSetAddTrans2Packet(SdfGsBlendPacket *packet);
void sdfPrimSetSubTransElem(SdfGsBlendRegisters *packet);
void sdfPrimSetSubTrans2Elem(SdfGsBlendRegisters *packet);
void sdfPrimSetSubTransPacket(SdfGsBlendPacket *packet);
void sdfPrimSetSubTrans2Packet(SdfGsBlendPacket *packet);
void sdfPrimSetTexflushElem(SdfGsRegisterWrite *write);
void sdfPrimSetTexflushPacket(SdfGsTextureFlushPacket *packet);
void sdfPrimSetBitBltElem(SdfImageTransferRegisters *packet, u32 destinationBufferAddress, s32 destinationBufferWidth,
                  s64 destinationFormat, s64 destinationX, s64 destinationY,
                  u32 sourceBufferAddress, s32 sourceBufferWidth, s32 sourceFormat,
                  s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight, s32 transferDirection);
void sdfPrimSetBitBltPacket(SdfImageTransferPacket *packet, u32 destinationBufferAddress, s32 destinationBufferWidth,
                                    s64 destinationFormat, s64 destinationX, s64 destinationY,
                                    u32 sourceBufferAddress, s32 sourceBufferWidth, s32 sourceFormat,
                                    s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight, s32 transferDirection);
void sdfPrimLinkBitBltPacket(s32 packetList, u32 destinationBufferAddress, s32 destinationBufferWidth,
                             s64 destinationFormat, s64 destinationX, s64 destinationY,
                             u32 sourceBufferAddress, s32 sourceBufferWidth, s32 sourceFormat,
                             s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight,
                             s32 transferDirection, s32 (*allocator)(s32));
void sdfPrimFrameMovePredraw(SdfGraphCopyPacket *packet, s32 entryIndex);
void sdfPrimLinkFrameMovePacket(SdfListHead *drawList, SdfLinkedPacketList *linkedList,
                   SdfTexResource *destination, s32 destinationX, s32 destinationY,
                   s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight,
                   s32 resourceIndexXor, s32 (*allocPacket)(s32));
void sdfPrimSetSpriteElem(SdfGsTexturedPairPayload *packet, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 depth);
void sdfPrimLinkSprite(SdfListHead *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0,
                   s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 depth, s32 (*alloc)(s32));
void sdfPrimSetF3Elem(SdfGsFlatTrianglePayload *dst, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 depth);
void sdfPrimLinkF3(SdfListHead *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1,
                   s32 y1, s32 x2, s32 y2, s32 depth, s32 (*alloc)(s32));
void sdfPrimSetF4Elem(SdfGsFlatQuadPayload *packet, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 x3, s32 y3, s32 depth);
void sdfPrimLinkF4(SdfListHead *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 x3, s32 y3, s32 depth, s32 (*alloc)(s32));
void sdfPrimSetG3Elem(SdfGsGouraudTrianglePayload *packet, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1, s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 depth);
void sdfPrimLinkG3(SdfListHead *list, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1, s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 depth, s32 (*alloc)(s32));
void sdfPrimSetG4Elem(SdfGsGouraudQuadPayload *packet, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1, s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 x3, s32 y3, s32 color3, s32 depth);
void sdfPrimLinkG4(s32 list, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1,
                   s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 x3,
                   s32 y3, s32 color3, s32 depth, s32 (*alloc)(s32));
void sdfPrimSetFt3Elem(SdfGsTexturedTrianglePayload *packet, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2, s32 u2, s32 v2, s32 depth);
void sdfPrimLinkFt3(s32 list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0,
                   s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2,
                   s32 u2, s32 v2, s32 depth, s32 (*alloc)(s32));
void sdfPrimSetFt4Elem(SdfGsTexturedQuadPayload *packet, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
                   s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2, s32 u2, s32 v2,
                   s32 x3, s32 y3, s32 u3, s32 v3, s32 depth);
void sdfPrimLinkFt4(SdfListHead *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0,
                   s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2,
                   s32 u2, s32 v2, s32 x3, s32 y3, s32 u3, s32 v3, s32 depth,
                   s32 (*alloc)(s32));
void sdfPrimSetGt3Elem(SdfGsGouraudTexturedTrianglePayload *packet, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 color0,
                   s32 x1, s32 y1, s32 u1, s32 v1, s32 color1, s32 x2, s32 y2,
                   s32 u2, s32 v2, s32 color2, s32 depth);
void sdfPrimLinkGt3(s32 list, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
                   s32 color0, s32 x1, s32 y1, s32 u1, s32 v1, s32 color1,
                   s32 x2, s32 y2, s32 u2, s32 v2, s32 color2, s32 depth,
                   s32 (*alloc)(s32));
void sdfPrimSetGt4Elem(SdfGsGouraudTexturedQuadPayload *packet, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 color0,
                   s32 x1, s32 y1, s32 u1, s32 v1, s32 color1, s32 x2, s32 y2, s32 u2,
                   s32 v2, s32 color2, s32 x3, s32 y3, s32 u3, s32 v3, s32 color3, s32 depth);
void sdfPrimLinkGt4(SdfListHead *list, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
                   s32 color0, s32 x1, s32 y1, s32 u1, s32 v1, s32 color1,
                   s32 x2, s32 y2, s32 u2, s32 v2, s32 color2, s32 x3, s32 y3,
                   s32 u3, s32 v3, s32 color3, s32 depth, s32 (*alloc)(s32));
void sdfPrimSetRectElem(SdfGsTwoVertexPayload *packet, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth);
void sdfPrimLinkRect(SdfListHead *list, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth, s32 (*alloc)(s32));
void sdfPrimSetLineElem(SdfGsTwoVertexPayload *packet, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth);
void sdfPrimLinkLine(SdfListHead *list, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth, s32 (*alloc)(s32));
void sdfPrimSetBoxLineElem(SdfGsClosedRectanglePayload *dst, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth);
void sdfPrimLinkBoxLine(SdfListHead *list, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth, s32 (*alloc)(s32));

void sdfSetStoreImage(SdfResourcePacket *packet, SdfTexResource *source,
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

void sdfLinkStoreImage(SdfListHead *list, SdfTexResource *source, s32 sourceX,
                   s32 sourceY, s32 width, s32 height, s32 resourceAddress, s32 arg7,
                   s32 arg8, s32 (*allocate)(s32)) {
    s32 packetAddress;

    if (allocate == NULL) {
        allocate = sdfGrAllocPacketBuff;
    }
    packetAddress = allocate(0xf0);
    sdfSetStoreImage((SdfResourcePacket *)packetAddress, source,
        sourceX, sourceY, width, height, resourceAddress, arg7, arg8);
    sdfDrawTagLink2(list, packetAddress, packetAddress + 0xc0);
}

void sdfLinkStoreFrameImagePredraw(SdfPatchableResourcePacket *packet, s32 entryIndex) {
    packet->resourcePacket.transfer.bitbltbuf.value =
        (packet->resourcePacket.transfer.bitbltbuf.value & ~0x3FFF) |
        (u64)(u32)(D_00529388.buffers[entryIndex]->word >> 6);
}

void sdfLinkStoreFrameImage(SdfListHead *list, SdfLinkedPacketList *linkedList, s32 sourceX, s32 sourceY,
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

void sdfSetLoadImage(SdfDescriptorPacket *packet, SdfTexResource *source,
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

void sdfLinkLoadImage(SdfListHead *list, SdfTexResource *source,
                   s32 destinationX, s32 destinationY, s32 width, s32 height,
                   s32 sourceAddress, s32 (*allocatePacket)(s32)) {
    s32 packetAddress;
    if (allocatePacket == NULL) {
        allocatePacket = sdfGrAllocPacketBuff;
    }
    packetAddress = allocatePacket(SDF_DESCRIPTOR_PACKET_BYTES);
    sdfSetLoadImage((SdfDescriptorPacket *)packetAddress, source,
                  destinationX, destinationY, width, height, sourceAddress);
    sdfDrawTagLink2(list, packetAddress, packetAddress + SDF_DESCRIPTOR_PACKET_TAIL_OFFSET);
}

void sdfDelayedResrcInitCB(SdfPendingRequest *request, SdfPendingCallback callback) {
    if (sdfDelayedResrcSemaId < 0) {
        sdfDelayedResrcSemaId = sdfCreateSema(1, 0x7f, 0);
    }
    request->handler = callback;
    request->pending = NULL;
}

void sdfDelayedResrcEntry(SdfPendingRequest *owner, void *entry) {
    SdfPendingNode *pendingNode;
    SdfPendingBuffer *entryBuffer;
    s32 freeEntries;

    if (entry != NULL) {
        WaitSema(sdfDelayedResrcSemaId);
        pendingNode = owner->pending;
        if (pendingNode == NULL) {
            pendingNode = (SdfPendingNode *)sdfChipAllocDirect(sizeof(SdfPendingNode));
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

SdfPendingNode *sdfDelayedResrcClose(void) {
    SdfPendingNode *head = sdfDelayedResrcLink;
    SdfPendingNode *node;

    sdfDelayedResrcLink = NULL;
    for (node = head; node != NULL; node = node->next) {
        node->owner->pending = NULL;
    }
    return head;
}

void sdfDelayedResrcRelease(SdfPendingNode *pendingNode) {
    SdfPendingNode *nextPendingNode;
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

INCLUDE_ASM(const s32, "code_002AC138", sdfDelayedResrcProc);

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

INCLUDE_ASM(const s32, "code_002AC138", sdfGrGetPacketBuffRemain);

s32 sdfGrAllocPacketBuff(s32 size) {
    s32 packet;

    packet = sdfGrPacketBuffPtr;
    sdfGrPacketBuffPtr = sdfGrPacketBuffPtr + ((size + 0xfU) & 0xfffffff0);
    return packet;
}

INCLUDE_ASM(const s32, "code_002AC138", sdfGrOpenPacketBuff);

void sdfGrClosePacketBuff(s32 cursorAddress) {
    sdfGrPacketBuffPtr = (cursorAddress + SDF_QWORD_ALIGNMENT_MASK) & ~SDF_QWORD_ALIGNMENT_MASK;
}

void sdfDrawTagInit(SdfListHead *list) {
    list->unkC = 0xFFFF;
    list->nextList = NULL;
    list->first = 0;
    list->last = 0;
    list->firstReferenceSource = 0;
    list->secondReferenceSource = 0;
    list->unk18 = 0;
    list->unk1C = 0;
}

void sdfDrawTagLink(SdfListHead *list, u32 packet) {
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

INCLUDE_ASM(const s32, "code_002AC138", sdfDrawTagLink2);

INCLUDE_ASM(const s32, "code_002AC138", sdfDrawTagLinkRef);

void sdfDrawTagLinkRefGif(SdfListHead *list, u32 packetAddress) {
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

void sdfDrawTagLinkCall(SdfListHead *list, u32 packetAddress) {
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

void sdfGrOtFuncLinkToHead(SdfPoolNode *destinationList, SdfListHead *incomingList) {
    SdfListHead *firstList;

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

void sdfGrOtNormalReg(SdfPoolNode *list, SdfListHead *item) {
    SdfListHead *last;

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

s32 sdfGrOtNormalCallback(SdfPoolNode *list, s32 mode, SdfListHead *packet) {
    if (mode == 1) {
        sdfGrOtFuncLinkToHead(list, packet);
    }
}

void sdfGrOtInit(SdfPoolNode *nodes, s32 count) {
    SdfPoolNode *node = nodes;
    SdfPoolNode *next;
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

SdfDmaNode *sdfGrNewExPacket(SdfDmaTag *sourceTag) {
    SdfDmaNode *referenceNode = (SdfDmaNode *)sdfGrAllocPacketBuff(SDF_DMA_REFERENCE_NODE_BYTES);
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
    SdfDmaNode *referenceNode;

    referenceNode = sdfGrNewExPacket(sourceTag);
    previousTag->kind = SDF_DMA_TAG_NEXT_BYTE;
    previousTag->address = (u32)referenceNode & SDF_DMA_ADDRESS_MASK;
    return (SdfDmaTag *)((u32)referenceNode + SDF_QWORD_BYTES);
}

void sdfGrJoinDrawTag(SdfListHead *previousList, SdfListHead *incomingList) {
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

void sdfGrInitExPacket(SdfListHead *list) {
    SdfDmaNode *referenceNode;
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

SdfListHead *sdfGrOtClose(SdfPoolNode *node) {
    SdfListHead *tail = NULL;
    SdfListHead *head = NULL;

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

INCLUDE_ASM(const s32, "code_002AC138", sdfPreDrawTagInit);

void sdfPreDrawTagLink(SdfLinkedPacketList *list, SdfPacketPatchLink *node) {
    if (list->last == 0) {
        list->first = node;
    }
    else {
        list->last->next = node;
    }
    list->last = node;
    node->next = NULL;
}

INCLUDE_ASM(const s32, "code_002AC138", sdfPreDrawChainInit);

void sdfPreDrawChainLink(SdfPacketChain *chain, SdfLinkedPacketList *node) {
    if (chain->tail == NULL) {
        chain->head = node;
    }
    else {
        chain->tail->last->next = node->last;
    }
    chain->tail = node;
}

void sdfGrSetRegsPacketHeader(SdfGsPacketHeader *header, s32 payloadQwords) {
    s64 dmaQwords;

    dmaQwords = payloadQwords + 1;
    header->vifCommands = (((dmaQwords | SDF_VIF_DIRECT_WORD) << 32) | SDF_VIF_FLUSHE_WORD);
    header->gifTag = (payloadQwords | (((s64)SDF_GIF_ONE_REGISTER_WORD << 32) | SDF_GIF_EOP_BIT));
    header->dmaTag = dmaQwords;
    header->gifRegisters = SDF_GIF_REGISTER_AD;
}

INCLUDE_ASM(const s32, "code_002AC138", sdfGrSetRefGifPacket);

void sdfGrSetDrawFramePacket(SdfGsContextRegisters *packet, s32 frameAddress, s32 width, s32 height,
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
    packet->scissor.registerId = scissorRegisterId;
    yOffset = (SDF_GS_CENTER_BIAS - height) << 3;
    xOffset = (SDF_GS_CENTER_BIAS - width) << 3;
    packet->frame.value = (s64)(frameFormat << 24) | (s64)(((width + 63) >> 6) << 16) | (s64)(frameAddress >> 11);
    if (fieldOffset != 0) {
        yOffset += SDF_GS_FIELD_OFFSET_STEP;
    }
    packet->frame.registerId = frameRegisterId;
    packet->zbuf.value = (s64)(depthFormat << 24) | (s64)(depthAddress >> 11);
    packet->zbuf.registerId = depthRegisterId;
    packet->xyoffset.registerId = offsetRegisterId;
    packet->scissor.value = ((s64)(height - 1) << 48) | ((s64)(width - 1) << 16);
    packet->xyoffset.value = xOffset | ((s64)yOffset << 32);
}

void sdfGrSetClearPacket(SdfGsCenteredBoundsRegisters *packet, s32 width, s32 height, s32 unused0, s32 unused1) {
    u32 lowerBounds = ((SDF_GS_CENTER_BIAS - height) << 19) | ((SDF_GS_CENTER_BIAS - width) << 3);
    u32 upperBounds = ((height + SDF_GS_CENTER_BIAS) << 19) | ((width + SDF_GS_CENTER_BIAS) << 3);

    packet->primitive.registerId = SDF_GS_PRIM;
    packet->xyz2[0].registerId = SDF_GS_XYZ2;
    packet->test.value = SDF_GS_CENTERED_VIEW_BOUNDS_TEST;
    packet->test.registerId = SDF_GS_TEST_1;
    packet->primitive.value = SDF_GS_PRIMITIVE_SPRITE;
    packet->rgbaq.value = (u64)0xFE00 << 46;
    packet->rgbaq.registerId = SDF_GS_RGBAQ;
    packet->xyz2[0].value = lowerBounds;
    packet->xyz2[1].value = upperBounds;
    packet->xyz2[1].registerId = SDF_GS_XYZ2;
}

INCLUDE_ASM(const s32, "code_002AC138", sdfGrSetDrawDefParmPacket);

void sdfGrSetDrawFrame(SdfGsContextPacket *packet, s32 frameAddress, s32 width, s32 height,
                           s32 frameFormat, s32 depthAddress, s32 depthFormat, s32 gsContext) {
    sdfGrSetRegsPacketHeader(&packet->header, SDF_SCENE_DRAW_PAYLOAD_QWORDS);
    sdfGrSetDrawFramePacket(&packet->registers, frameAddress, width, height, frameFormat, depthAddress, depthFormat, 0, gsContext);
}

INCLUDE_ASM(const s32, "code_002AC138", sdfGrSetDrawEnv);

INCLUDE_ASM(const s32, "code_002AC138", sdfGrStdDrawEnvPredraw);

INCLUDE_ASM(const s32, "code_002AC138", sdfGrSetStdDrawEnv);

void sdfGrLinkStdDrawEnv(SdfListHead *dmaList, SdfLinkedPacketList *linkedList, SdfPacketPatchLink *linkedNode) {
    sdfPreDrawTagLink(linkedList, linkedNode);
    sdfDrawTagLink(dmaList, (s32)linkedNode + SDF_QWORD_BYTES);
}

INCLUDE_ASM(const s32, "code_002AC138", sdfGrSetFrameMaskRGB);

INCLUDE_ASM(const s32, "code_002AC138", sdfGrSetFrameMaskReset);

void sdfGrLinkDispFrameTex(SdfListHead *list, u32 source, SdfDmaNode *node) {
    node->vifCommands = (((u64)0x50000004 << 16) | 0x1000) << 16;
    node->dmaTag = ((u64)((source + 0x1a0) & 0xfffffff) << 32) | 0x30000004;
    node->nextTag = 0;
    sdfDrawTagLinkRef(list, (u32)node);
}

void sdfGrLinkDrawFrameTex(SdfListHead *list, u32 source, SdfDmaNode *node) {
    node->vifCommands = (((u64)0x50000004 << 16) | 0x1000) << 16;
    node->dmaTag = ((u64)((source + 0x1e0) & 0xfffffff) << 32) | 0x30000004;
    node->nextTag = 0;
    sdfDrawTagLinkRef(list, (u32)node);
}

void sdfGrFramePacketPredraw(SdfPacketBuilder *packet, s32 bufferIndex) {
    SdfGraphObj *view = packet->source;
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
    packet->frame.value = ((u64)packet->frameMask << 32) |
        (u64)(view->buffers[bufferIndex]->word >> 11) |
        ((s64)(view->bufferFormat << 24) | ((s64)widthPages << 16));
    packet->frame.registerId = frameRegister;
    packet->zbuf.value = (s64)(view->auxiliaryFormat << 24) |
        (u64)(view->buffers[2]->word >> 11);
    packet->zbuf.registerId = depthRegister;
}

void sdfGrSetFramePacket(SdfPacketBuilder *packet, SdfGraphObj *source, u32 frameMask, s32 region, s32 mode) {
    sdfGrSetRegsPacketHeader(&packet->packetHeader, 2);
    packet->mode = mode;
    packet->source = source;
    packet->frameMask = frameMask;
    packet->region = region;
    packet->prepare = sdfGrFramePacketPredraw;
}

INCLUDE_ASM(const s32, "code_002AC138", sdfGrReqDraw);

void sdfGrReqDrawDelay(s32 value) {
    sdfReqDrawDelayCount = (value < 0) ? 0 : value;
}

void sdfGrInitDraw(void) {
    sdfReqDrawDelayCount = 0;
    sdfGrDrawState[0].packetList = NULL;
    sdfDrawReqQueueIndex = 0;
}

INCLUDE_ASM(const s32, "code_002AC138", sdfGrDrawFrame);

INCLUDE_ASM(const s32, "code_002AC138", sdfDrawSync);

SdfListHead *sdfPrimAllocDrawTag(s32 (*allocator)(s32)) {
    SdfListHead *list;

    if (allocator == NULL) {
        allocator = sdfGrAllocPacketBuff;
    }
    list = (SdfListHead *)(u32)allocator(SDF_PACKET_LIST_BYTES);
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

void sdfPrimSetOpaqueElem(SdfGsBlendRegisters *packet) {
    packet->test.value = SDF_GS_ALPHA_TEST;
    packet->test.registerId = SDF_GS_TEST_1;
    packet->alpha.value = SDF_GS_DEFAULT_ALPHA;
    packet->alpha.registerId = SDF_GS_ALPHA_1;
}

void sdfPrimSetOpaque2Elem(SdfGsBlendRegisters *packet) {
    packet->test.value = SDF_GS_ALPHA_TEST;
    packet->test.registerId = SDF_GS_TEST_2;
    packet->alpha.value = SDF_GS_DEFAULT_ALPHA;
    packet->alpha.registerId = SDF_GS_ALPHA_2;
}

void sdfPrimSetOpaquePacket(SdfGsBlendPacket *packet) {
    sdfPrimSetOpaqueElem(&packet->registers);
    packet->header.dmaTag = SDF_ALPHA_DMA_QWORDS;
    packet->header.vifCommands = (((u64)SDF_VIF_DIRECT_THREE_WORD << 16 | SDF_VIF_FLUSHE_HALFWORD) << 16);
    packet->header.gifTag = (((u64)SDF_GIF_ONE_REGISTER_WORD << 32) | SDF_GIF_TWO_AD_LOOPS_EOP);
    packet->header.gifRegisters = SDF_GIF_REGISTER_AD;
}

void sdfPrimSetOpaque2Packet(SdfGsBlendPacket *packet) {
    sdfPrimSetOpaque2Elem(&packet->registers);
    packet->header.dmaTag = SDF_ALPHA_DMA_QWORDS;
    packet->header.vifCommands = (((u64)SDF_VIF_DIRECT_THREE_WORD << 16 | SDF_VIF_FLUSHE_HALFWORD) << 16);
    packet->header.gifTag = (((u64)SDF_GIF_ONE_REGISTER_WORD << 32) | SDF_GIF_TWO_AD_LOOPS_EOP);
    packet->header.gifRegisters = SDF_GIF_REGISTER_AD;
}

void sdfPrimSetSemiTransElem(SdfGsBlendRegisters *packet) {
    packet->test.value = 0x717FB;
    packet->test.registerId = 0x47;
    packet->alpha.value = 0x44;
    packet->alpha.registerId = 0x42;
}

void sdfPrimSetSemiTrans2Elem(SdfGsBlendRegisters *packet) {
    packet->test.value = 0x717FB;
    packet->test.registerId = 0x48;
    packet->alpha.value = 0x44;
    packet->alpha.registerId = 0x43;
}

void sdfPrimSetSemiTransPacket(SdfGsBlendPacket *packet) {
    sdfPrimSetSemiTransElem(&packet->registers);
    packet->header.dmaTag = 3;
    packet->header.vifCommands = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->header.gifTag = (((u64)0x10000000 << 32) | 0x8002);
    packet->header.gifRegisters = 0xE;
}

void sdfPrimSetSemiTrans2Packet(SdfGsBlendPacket *packet) {
    sdfPrimSetSemiTrans2Elem(&packet->registers);
    packet->header.dmaTag = 3;
    packet->header.vifCommands = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->header.gifTag = (((u64)0x10000000 << 32) | 0x8002);
    packet->header.gifRegisters = 0xE;
}

void sdfPrimSetAddTransElem(SdfGsBlendRegisters *packet) {
    packet->test.value = 0x71801;
    packet->test.registerId = 0x47;
    packet->alpha.value = 0x48;
    packet->alpha.registerId = 0x42;
}

void sdfPrimSetAddTrans2Elem(SdfGsBlendRegisters *packet) {
    packet->test.value = 0x71801;
    packet->test.registerId = 0x48;
    packet->alpha.value = 0x48;
    packet->alpha.registerId = 0x43;
}

void sdfPrimSetAddTransPacket(SdfGsBlendPacket *packet) {
    sdfPrimSetAddTransElem(&packet->registers);
    packet->header.dmaTag = 3;
    packet->header.vifCommands = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->header.gifTag = (((u64)0x10000000 << 32) | 0x8002);
    packet->header.gifRegisters = 0xE;
}

void sdfPrimSetAddTrans2Packet(SdfGsBlendPacket *packet) {
    sdfPrimSetAddTrans2Elem(&packet->registers);
    packet->header.dmaTag = 3;
    packet->header.vifCommands = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->header.gifTag = (((u64)0x10000000 << 32) | 0x8002);
    packet->header.gifRegisters = 0xE;
}

void sdfPrimSetSubTransElem(SdfGsBlendRegisters *packet) {
    packet->test.value = 0x71801;
    packet->test.registerId = 0x47;
    packet->alpha.value = 0x42;
    packet->alpha.registerId = 0x42;
}

void sdfPrimSetSubTrans2Elem(SdfGsBlendRegisters *packet) {
    packet->test.value = 0x71801;
    packet->test.registerId = 0x48;
    packet->alpha.value = 0x42;
    packet->alpha.registerId = 0x43;
}

void sdfPrimSetSubTransPacket(SdfGsBlendPacket *packet) {
    sdfPrimSetSubTransElem(&packet->registers);
    packet->header.dmaTag = 3;
    packet->header.vifCommands = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->header.gifTag = (((u64)0x10000000 << 32) | 0x8002);
    packet->header.gifRegisters = 0xE;
}

void sdfPrimSetSubTrans2Packet(SdfGsBlendPacket *packet) {
    sdfPrimSetSubTrans2Elem(&packet->registers);
    packet->header.dmaTag = 3;
    packet->header.vifCommands = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->header.gifTag = (((u64)0x10000000 << 32) | 0x8002);
    packet->header.gifRegisters = 0xE;
}

void sdfPrimSetTexflushElem(SdfGsRegisterWrite *write) {
    write->value = 0;
    write->registerId = SDF_GS_TEXFLUSH;
}

void sdfPrimSetTexflushPacket(SdfGsTextureFlushPacket *packet) {
    sdfPrimSetTexflushElem(&packet->textureFlush);
    packet->header.dmaTag = 2;
    packet->header.vifCommands = (((u64)0x50000002 << 16 | 0x1000) << 16);
    packet->header.gifTag = (((u64)0x10000000 << 32) | 0x8001);
    packet->header.gifRegisters = 0xE;
}

void sdfPrimSetBitBltElem(SdfImageTransferRegisters *packet, u32 destinationBufferAddress, s32 destinationBufferWidth,
                  s64 destinationFormat, s64 destinationX, s64 destinationY,
                  u32 sourceBufferAddress, s32 sourceBufferWidth, s32 sourceFormat,
                  s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight, s32 transferDirection) {
    packet->bitbltbuf.value = (destinationFormat << 56) |
        ((u64)(destinationBufferWidth >> 6) << 48) |
        ((u64)(destinationBufferAddress >> 6) << 32) |
        (sourceFormat << 24) | ((sourceBufferWidth >> 6) << 16) |
        (sourceBufferAddress >> 6);
    packet->bitbltbuf.registerId = SDF_GS_BITBLTBUF;
    packet->trxpos.value = (destinationY << 48) | (destinationX << 32) |
        (sourceY << 16) | sourceX;
    packet->trxpos.registerId = SDF_GS_TRXPOS;
    packet->trxreg.value = transferWidth | ((u64)transferHeight << 32);
    packet->trxreg.registerId = SDF_GS_TRXREG;
    packet->trxdir.value = transferDirection;
    packet->trxdir.registerId = SDF_GS_TRXDIR;
}

void sdfPrimSetBitBltPacket(SdfImageTransferPacket *packet, u32 destinationBufferAddress, s32 destinationBufferWidth,
                                    s64 destinationFormat, s64 destinationX, s64 destinationY,
                                    u32 sourceBufferAddress, s32 sourceBufferWidth, s32 sourceFormat,
                                    s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight, s32 transferDirection) {
    packet->header.dmaTag = 5;
    packet->header.vifCommands = (((u64)0x50000005 << 16 | 0x1000) << 16);
    packet->header.gifTag = (((u64)0x10000000 << 32) | 0x8004);
    packet->header.gifRegisters = 0xE;
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
    sdfPrimSetBitBltPacket((SdfImageTransferPacket *)packetAddress, destinationBufferAddress, destinationBufferWidth,
                                    destinationFormat, destinationX, destinationY, sourceBufferAddress, sourceBufferWidth,
                                    sourceFormat, sourceX, sourceY, transferWidth, transferHeight, transferDirection);
    sdfDrawTagLink(packetList, packetAddress);
}

void sdfPrimFrameMovePredraw(SdfGraphCopyPacket *packet, s32 entryIndex) {
    packet->draw.transfer.bitbltbuf.value =
        (packet->draw.transfer.bitbltbuf.value & ~0x3FFF) |
        (u64)(u32)(D_00529388.buffers[entryIndex ^ packet->resourceIndexXor]->word >> 6);
}

void sdfPrimLinkFrameMovePacket(SdfListHead *drawList, SdfLinkedPacketList *linkedList,
                   SdfTexResource *destination, s32 destinationX, s32 destinationY,
                   s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight,
                   s32 resourceIndexXor, s32 (*allocPacket)(s32)) {
    SdfGraphCopyPacket *packet;
    SdfImageTransferPacket *drawPacket;

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

void sdfPrimSetSpriteElem(SdfGsTexturedPairPayload *packet, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 depth) {
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

void sdfPrimLinkSprite(SdfListHead *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0,
                   s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 depth, s32 (*alloc)(s32)) {
    SdfGsTexturedPairPacket *packet;
    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsTexturedPairPacket *)alloc(0x50);
    packet->dmaTag = 0x20000004;
    packet->vifCommands = (((u64)0x50000004 << 16) | 0x1000) << 16;
    sdfPrimSetSpriteElem(&packet->drawing, color, primitive, x0, y0, u0, v0, x1, y1, u1, v1, depth);
    sdfDrawTagLink(list, (s32)packet);
}

void sdfPrimSetF3Elem(SdfGsFlatTrianglePayload *dst, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 depth) {
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

void sdfPrimLinkF3(SdfListHead *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1,
                   s32 y1, s32 x2, s32 y2, s32 depth, s32 (*alloc)(s32)) {
    SdfGsFlatTrianglePacket *packet;
    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsFlatTrianglePacket *)alloc(0x50);
    packet->dmaTag = 0x20000004;
    packet->vifCommands = (((u64)0x50000004 << 16) | 0x1000) << 16;
    sdfPrimSetF3Elem(&packet->drawing, color, primitive, x0, y0, x1, y1, x2, y2, depth);
    sdfDrawTagLink(list, (s32)packet);
}

void sdfPrimSetF4Elem(SdfGsFlatQuadPayload *packet, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 x3, s32 y3, s32 depth) {
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

void sdfPrimLinkF4(SdfListHead *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 x3, s32 y3, s32 depth, s32 (*alloc)(s32)) {
    SdfGsFlatQuadPacket *packet;

    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsFlatQuadPacket *)alloc(0x50);
    packet->dmaTag = 0x20000004ULL;
    packet->vifCommands = 0x5000000410000000ULL;
    sdfPrimSetF4Elem(&packet->drawing, color, primitive, x0, y0, x1, y1, x2, y2, x3, y3, depth);
    sdfDrawTagLink(list, (s32)packet);
}

void sdfPrimSetG3Elem(SdfGsGouraudTrianglePayload *packet, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1, s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 depth) {
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

void sdfPrimLinkG3(SdfListHead *list, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1, s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 depth, s32 (*alloc)(s32)) {
    SdfGsGouraudTrianglePacket *packet;

    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsGouraudTrianglePacket *)alloc(0x60);
    packet->dmaTag = 0x20000005ULL;
    packet->vifCommands = 0x5000000510000000ULL;
    sdfPrimSetG3Elem(&packet->drawing, primitive, x0, y0, color0, x1, y1, color1, x2, y2, color2, depth);
    sdfDrawTagLink(list, (s32)packet);
}

void sdfPrimSetG4Elem(SdfGsGouraudQuadPayload *packet, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1, s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 x3, s32 y3, s32 color3, s32 depth) {
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
    SdfGsGouraudQuadPacket *packet;
    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsGouraudQuadPacket *)alloc(0x70);
    packet->dmaTag = 0x20000006;
    packet->vifCommands = (((u64)0x50000006 << 16) | 0x1000) << 16;
    sdfPrimSetG4Elem(&packet->drawing, primitive, x0, y0, color0, x1, y1, color1, x2, y2, color2, x3, y3, color3, depth);
    sdfDrawTagLink(list, (s32)packet);
}

void sdfPrimSetFt3Elem(SdfGsTexturedTrianglePayload *packet, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2, s32 u2, s32 v2, s32 depth) {
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
    SdfGsTexturedTrianglePacket *packet;
    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsTexturedTrianglePacket *)alloc(0x60);
    packet->dmaTag = 0x20000005;
    packet->vifCommands = (((u64)0x50000005 << 16) | 0x1000) << 16;
    sdfPrimSetFt3Elem(&packet->drawing, color, primitive, x0, y0, u0, v0, x1, y1, u1, v1, x2, y2, u2, v2, depth);
    sdfDrawTagLink(list, (s32)packet);
}

void sdfPrimSetFt4Elem(SdfGsTexturedQuadPayload *packet, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
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

void sdfPrimLinkFt4(SdfListHead *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0,
                   s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2,
                   s32 u2, s32 v2, s32 x3, s32 y3, s32 u3, s32 v3, s32 depth,
                   s32 (*alloc)(s32)) {
    SdfGsTexturedQuadPacket *packet;
    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsTexturedQuadPacket *)alloc(0x70);
    packet->dmaTag = 0x20000006;
    packet->vifCommands = (((u64)0x50000006 << 16) | 0x1000) << 16;
    sdfPrimSetFt4Elem(&packet->drawing, color, primitive, x0, y0, u0, v0, x1, y1, u1, v1, x2, y2, u2, v2, x3, y3, u3, v3, depth);
    sdfDrawTagLink(list, (s32)packet);
}

void sdfPrimSetGt3Elem(SdfGsGouraudTexturedTrianglePayload *packet, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 color0,
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
    SdfGsGouraudTexturedTrianglePacket *packet;
    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsGouraudTexturedTrianglePacket *)alloc(0x70);
    packet->dmaTag = 0x20000006;
    packet->vifCommands = (((u64)0x50000006 << 16) | 0x1000) << 16;
    sdfPrimSetGt3Elem(&packet->drawing, primitive, x0, y0, u0, v0, color0, x1, y1, u1, v1, color1, x2, y2, u2, v2, color2, depth);
    sdfDrawTagLink(list, (s32)packet);
}

void sdfPrimSetGt4Elem(SdfGsGouraudTexturedQuadPayload *packet, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 color0,
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

void sdfPrimLinkGt4(SdfListHead *list, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
                   s32 color0, s32 x1, s32 y1, s32 u1, s32 v1, s32 color1,
                   s32 x2, s32 y2, s32 u2, s32 v2, s32 color2, s32 x3, s32 y3,
                   s32 u3, s32 v3, s32 color3, s32 depth, s32 (*alloc)(s32)) {
    SdfGsGouraudTexturedQuadPacket *packet;
    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsGouraudTexturedQuadPacket *)alloc(0x90);
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

void sdfPrimLinkRect(SdfListHead *list, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth, s32 (*alloc)(s32)) {
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

void sdfPrimLinkLine(SdfListHead *list, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth, s32 (*alloc)(s32)) {
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

void sdfPrimSetBoxLineElem(SdfGsClosedRectanglePayload *dst, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth) {
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

void sdfPrimLinkBoxLine(SdfListHead *list, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth, s32 (*alloc)(s32)) {
    SdfGsClosedRectanglePacket *packet;

    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsClosedRectanglePacket *)alloc(0x60);
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

