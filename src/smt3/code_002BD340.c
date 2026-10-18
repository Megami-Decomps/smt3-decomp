#include "fpu.h"
#include "common.h"
#include "libsdf.h"
#include "ps2asm.h"

typedef struct
{
    u32 lsn;
    u32 unk4;
    s32 size;
} sdfCdImageEntry_t;



struct movieStream_t;
struct SoundFormat;
struct sdfGsMemSlice_t;
struct SdfStreamInputDmaTag;

typedef struct IpuDmaState
{
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

typedef struct sdfIpuDecodeCb_t
{
    struct sdfIpuDecodeCb_t *streamPrev;
    struct sdfIpuDecodeCb_t *streamNext;
    struct sdfIpuDecodeCb_t *next;
    u8 active;
    u8 queued;
    u8 ipuCycleInitialized;
    u8 drained;
    u8 playbackPhase;
    u8 inputFeedEnabled;
    u8 inputDmaStartPending;
    u8 pendingPlaybackSubmissions;
    u8 audioMode;
    u8 loopMode;
    u8 playbackCadenceStep;
    u8 playbackCadenceRemainder;
    u8 bufferIndex;
    u8 transferPacketIndex;
    u8 completedBufferCount;
    u8 pad1B;
    s32 bufferSize;
    void *frameBuffers[2];
    void *transferPacketBuffers[2];
    u32 transferPacketBytes;
    s32 resourceWord;
    struct sdfGsMemSlice_t *textureHead;
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
    struct movieStream_t *readContext;
    u8 inputFeedDmaInFlight;
    u8 unk65;
    u8 pad66[2];
    IpuDmaState dma;
} sdfIpuDecodeCb_t;

typedef s32 (*SdfStreamRead)(sdfIpuDecodeCb_t *, struct movieStream_t *, s32, void *, s32);

void sdfIpuStrmDecodeInit(sdfIpuDecodeCb_t *aNode, struct SoundFormat *aFormat, SdfStreamRead aRead, struct movieStream_t *aContext);
void sdfIpuSetDecodeParams(sdfIpuDecodeCb_t *aNode, struct SoundFormat *aFormat, SdfStreamRead aRead, struct movieStream_t *aContext);
void sdfIpuAttachTask(sdfIpuDecodeCb_t *aNode);

u32 dds3GetRandA(void *aState);
s32 strlen(const char *aStr);
s32 sdfSndCmdTrans(void);
s32 SleepThread(void);
s32 sdfCreateSema(s32 aInit, s32 aMax, s32 aAttr);
s32 sceCdInit(s32 aMode);
s32 sceCdMmode(s32 aMode);
s32 sceCdStStop(void);

extern s8 sdfSndBurstReqMode;
extern sdfCdImageEntry_t *sdfCdImageCurrFile;
extern s32 sdfCdImageFd;
extern s32 sdfCdTrayMode;

s32 sdfFileCmdClose(void *aHandle);
s32 sdfFileCmdFree(sdfFileCmd_t *aCmd);
void sdfMemFreeDirect(sdfMemHandle_t *aHandle);
void sdfChipFreeDirect(void *aPtr);



typedef struct
{
    u32 unk0;
    u8 unk4;
    u8 unk5;
    u16 unk6;
} p2sdLoadEntry_t;

extern p2sdLoadEntry_t LastLoad[];
extern u32 sdfFileLengthWork;
extern u32 sdfFileSeekWork;

s32 sdfSndSendCmd(s32 aCmd, s32 aArg1, void *aData, s32 aSize);
sdfCdImageEntry_t *sdfCdImageSearchFile(const char *aFileName, s32 *aOutLsn, s32 *aOutSize);
s32 sdfFileCmdFileSize(void *aHandle);
s32 sdfFileCmdSeek(void *aHandle, s32 aOffset, s32 aOrigin);

extern s32 WaitSema(s32 aSemaId);
extern s32 SignalSema(s32 aSemaId);
extern s32 iSignalSema(s32 aSemaId);
extern s32 sdfFileSemaId;
extern u8 sdfFileDefaultSrcType;
extern u8 sdfFileCmdDefaultDevice;
extern u8 sdfFileCmdThreadPrioCheckCounter;
extern u32 p2sdPcmCallback;
extern u32 sdfSndMap[];
#define SDF_PI_2 1.5707963f


f32 sdfSin(f32 aRad);
void sdfFileCmdThreadSetPrio(s32 aPrio);
s32 sdfFileCmdRead(void *aHandle, void *aBuffer, s32 aSize);
void *sdfDevLoadFile(const char *aFileName, u32 *aOutData, u32 *aOutSize, s32 aAlign);

extern char sdfFileHostRootPath[];
extern void *sdfCdImageDirData;
extern u32 p2sdPcmState;
extern u32 p2sdPcmBusy;
extern u8 D_00593890[];

typedef struct
{
    u8 unk00[0x10];
    s32 resolveOffset;
    s32 resolveCount;
    u8 unk18[8];
} sdfBinaryHeader_t;

extern sdfIpuDecodeCb_t *sdfIpuTaskLink;
extern const char D_00549640[];

void FatalError(const char *aMessage, ...) __attribute__((noreturn));
void sdfAddressResolver(void *aBase, void *aTarget, void *aTable, s32 aCount);
s32 sdfFileCmdUnlink(void *aCmd);
s32 sdfFileCmdDequeue(void *aCmd);
s32 sceClose(s32 aFd);
char *strcat(char *aDest, const char *aSrc);

/* Volatile IPU and DMAC hardware registers. */
#define SDF_IPU_CMD (*(volatile u32 *)0x10002000)
#define SDF_IPU_CTRL (*(volatile u32 *)0x10002010)
#define SDF_FROM_IPU_CHCR (*(volatile u32 *)0x1000B000)
#define SDF_FROM_IPU_MADR (*(volatile u32 *)0x1000B010)
#define SDF_FROM_IPU_QWC (*(volatile u32 *)0x1000B020)
#define SDF_TO_IPU_CHCR (*(volatile u32 *)0x1000B400)
#define SDF_TO_IPU_MADR (*(volatile u32 *)0x1000B410)
#define SDF_TO_IPU_QWC (*(volatile u32 *)0x1000B420)
#define SDF_TO_IPU_TADR (*(volatile u32 *)0x1000B430)
#define SDF_DMAC_ENABLE (*(volatile u32 *)0x1000F520)

typedef struct sifRpcClientDataPrefix_t {
    u8 opaque00[0x24];
    void *server;
} sifRpcClientDataPrefix_t;

typedef struct sifRpcClientContextView_t {
    sifRpcClientDataPrefix_t client;
    u8 opaque28[8];
} sifRpcClientContextView_t;

typedef void (*sifMRpcEnd_t)(void *argument);

typedef struct sdfHostPrefix_t {
    s8 b[7];
} sdfHostPrefix_t;

typedef u8 sdfDevOperationCode_t;

typedef s8 sdfDevStateCode_t;

/* Complete 0x40-byte request owner. sdfFileCmd_t describes only its opaque
 * 0x30-byte prefix for consumers outside the device queue implementation. */
typedef struct sdfFileCmdHandle_t {
    struct sdfFileCmdHandle_t *next;
    struct sdfFileCmdHandle_t *previous;
    struct sdfFileCmdHandle_t *workerNext;
    struct sdfFileCmdHandle_t *workerPrev;
    char *resolvedPath;
    u8 workerIndex;
    sdfDevOperationCode_t operation;
    sdfDevStateCode_t state;
    u8 pad17;
    s32 operationArg;
    s32 requestExtra;
    void *requestData;
    s32 options;
    s32 resourceId;
    s32 result;
    s32 transferred;
    u8 pad34[4];
    void (*callback)(struct sdfFileCmdHandle_t *, s32, s32, s32, s32);
    s32 callbackContext;
} sdfFileCmdHandle_t;

typedef struct sdfDevicePrefix_t {
    s32 head;
    u16 tail;
} sdfDevicePrefix_t;

typedef struct sdfFileDeviceCB_t {
    s32 threadId;
    s32 semaphore;
    struct sdfFileCmdHandle_t *first;
    struct sdfFileCmdHandle_t *last;
    u8 opaque10[8];
} sdfFileDeviceCB_t;

enum sdf_dev_state {
    SDF_DEV_STATE_OPENING = 2,
    SDF_DEV_STATE_SEEKING = 3,
    SDF_DEV_STATE_READING = 4,
    SDF_DEV_STATE_WRITING = 5,
    SDF_DEV_STATE_CLOSING = 6,
    SDF_DEV_STATE_ACTIVE = 7,
    SDF_DEV_STATE_COMPLETE = 8,
    SDF_DEV_STATE_INACTIVE = 9
};

enum sdf_dev_operation {
    SDF_DEV_OPERATION_NONE = 0,
    SDF_DEV_OPERATION_OPEN_READ = 1,
    SDF_DEV_OPERATION_OPEN_WRITE = 2,
    SDF_DEV_OPERATION_SEEK = 3,
    SDF_DEV_OPERATION_GET_SIZE = 4,
    SDF_DEV_OPERATION_READ = 5,
    SDF_DEV_OPERATION_WRITE = 6,
    SDF_DEV_OPERATION_CLOSE = 7,
    SDF_DEV_OPERATION_READ_BUFFER = 8,
    SDF_DEV_OPERATION_WRITE_BUFFER = 9
};

typedef struct sdfRandCB_t {
    u32 words[4];
} sdfRandCB_t;

typedef struct sdfClock_t {
    u8 pad00;
    u8 sec;
    u8 min;
    u8 hour;
    u8 day;
    u8 month;
    u16 year;
} sdfClock_t;

typedef struct sdfCdClock_t {
    u8 status;
    u8 second;
    u8 minute;
    u8 hour;
    u8 pad04;
    u8 day;
    u8 month;
    u8 year;
} sdfCdClock_t;

typedef struct sndRingPacket_t {
    u32 transportHeader[3];
    u32 command;
    u32 sequence;
    u8 payload[108];
} sndRingPacket_t;

typedef struct sdfSndTrackPacket_t {
    u32 trackId;
    u32 unk4;
    u16 setting;
    u16 unkA;
    u32 unkC;
} sdfSndTrackPacket_t;

typedef struct sndSequenceVolumePanPacket_t {
    u32 trackId;
    u32 opaque04;
    u16 volume;
    u16 pan;
    u32 opaqueNotSent0C;
} sndSequenceVolumePanPacket_t;

typedef struct sndListenerState_t {
    s32 opaque00[2];
    s32 scaledValues[6];
} sndListenerState_t;

typedef struct sndFileRequestState_t {
    u8 path[0x100];
    s32 size;
    s32 pending;
    s32 data;
} sndFileRequestState_t;


typedef struct sndChannel_t {
    u8 index;
    u8 unk1;
    s16 unk2;
    u32 unk4;
    u32 iopWorkAddress;
    u32 unkC;
} sndChannel_t;

typedef struct sndIopBuffer_t {
    s32 size;
    u32 iopAddress;
} sndIopBuffer_t;

typedef struct {
    u8 values[24];
    u8 pad18[8];
} sndMidiStateGroup_t;

typedef struct sndMergedWork_t {
    s32 header;
    s32 bufferCount;
    u32 iopBufferAddress;
    u32 iopBufferBytes;
    sndChannel_t channels[16];
    sndIopBuffer_t buffers[16];
    p2sdLoadEntry_t slots[7];
    void *eeWorkAddress;
    u32 eeWorkByteCount;
    u32 opaque1D0;
    u32 opaque1D4;
    u32 opaque1D8;
    u32 opaque1DC;
    u8 unk1E0[0x380];
    sndMidiStateGroup_t stateGroups[2];
} sndMergedWork_t;


typedef struct sdfSoundRpcRequest_t {
    u8 opaque00[8];
    s32 destination;
    s32 byteCount;
} sdfSoundRpcRequest_t;

enum sdf_tex_resource {
    SDF_TEX_RESOURCE_FREE = 0,
    SDF_TEX_RESOURCE_FRAME = 1,
    SDF_TEX_RESOURCE_TEXTURE = 2,
    SDF_TEX_RESOURCE_CLUT = 3,
    SDF_TEX_RESOURCE_KNOWN_MODE_COUNT = 4
};

typedef struct sdfTextureOffsetListEntry_t {
    s32 textureHeaderOffset;
} sdfTextureOffsetListEntry_t;

typedef struct sdfTextureOffsetListHeader_t {
    u8 unknown00[0x10];
    s32 textureCount;
} sdfTextureOffsetListHeader_t;

typedef struct {
    u32 control;
    u32 address;
    u32 vifCode0;
    u32 vifCode1;
} sdfDmaDumpTag_t;

struct SdfStreamInputDmaTag {
    u64 control;
    u64 reserved;
};

enum sdf_stream_output {
    SDF_STREAM_OUTPUT_RGBA32 = 0,
    SDF_STREAM_OUTPUT_RGBA16 = 1,
    SDF_STREAM_OUTPUT_RGBA16_DITHERED = 2
};

struct SoundFormat {
    u8 output16Bit;
    u8 dither;
    u8 loopMode;
    u8 playbackCadenceStep;
};

typedef struct sdfStreamFrameHeader_t {
    u8 reserved[8];
    u16 width;
    u16 height;
    u32 cycleLength;
} sdfStreamFrameHeader_t;

enum {
    SDF_STREAM_READ_QUERY = 0,
    SDF_STREAM_READ_COPY = 1,
    SDF_STREAM_READ_RESUME = 2
};

enum sdf_stream_playback {
    SDF_STREAM_PLAYBACK_INITIAL = 0,
    SDF_STREAM_PLAYBACK_FIRST_COMPLETION = 1,
    SDF_STREAM_PLAYBACK_CADENCED = 2
};

typedef struct sdfTextureDmaTail_t {
    u64 tag;
    u64 vifCodes;
} sdfTextureDmaTail_t;

typedef u32 sdfDmaAddress_t;

extern s32 DIntr();

extern void StartThread();

typedef union { f32 quaternion[4]; u16 monthDays[20]; } sdfMathConstants_t;

extern sdfMathConstants_t D_00529BB0;

extern void sdfSndTransToIOP(s32 destination, void *source, s32 size);

extern void *memcpy(void *, const void *, unsigned int);

s32 sceSifMCallRpc(sifRpcClientDataPrefix_t *client, s32 function, s32 mode, void *send, s32 sendSize,
                   void *receive, s32 receiveSize, sifMRpcEnd_t end, void *endArgument);

extern sifRpcClientContextView_t D_0058F310;

extern s8 sdfPrintIopFlag;

extern char D_00551908[];

extern s32 scePrintf(const char *, ...);

extern s32 vsprintf(char *destination, const char *format, void *args);

extern void sdfDelayThread(s32 delay);

void sdfCdWaitTrayClose(void);

extern u8 D_00552110;

extern s32 D_0055210C;

extern s32 sceCdStStart(s32 arg, u8 *options);

extern s32 sceCdStatus(void);

extern s32 sdfCdImageCurrRemain;

extern s32 sdfCdImageCurrActive;

extern u8 *sdfCdImageOffset;

extern u8 D_0058F7C0[0x800];

extern char D_00549658[];

extern s32 sceOpen();

extern s32 sceLseek(s32 fileHandle, s32 offset, s32 origin);

extern void *malloc(u32 size);

extern s32 sceRead(s32 fileHandle, s32 buffer, s32 byteCount);

extern s32 dds3ImgPath;

extern s32 sceCdSearchFile(void *, s32);

extern u32 sdfCdImageFileName;

extern u32 dds3ImgLsn;

extern s32 sceSifAllocIopHeap(s32);

extern void sceCdStInit(s32, s32, s32);

extern char D_00551928[];

extern char * strcpy(char *, const char *);

extern sdfHostPrefix_t D_00551930[];

extern u8 D_00551938[];

void *sdfChipAllocDirect(s32 size);

extern char * strchr(const char *, s32);

extern char D_00551940[];

extern s32 strncmp(const char *, const char *, u32);

extern sdfFileDeviceCB_t sdfFileDeviceCB[];

extern void EIntr(void);

extern sdfFileCmdHandle_t *D_00551948;

extern sdfFileCmdHandle_t *sdfFileCmdFreeList;

extern s16 sdfFileCmdFreeCount;

extern s16 sdfFileCmdCount;

extern sdfFileCmdHandle_t *D_00551954;

extern sdfFileCmdHandle_t *D_00551958;

u32 sdfMemLock(sdfMemHandle_t *allocation);

extern s32 sceWrite(s32 fileHandle, s32 buffer, s32 byteCount);

extern char D_00551968[];

extern char D_00551970[];

extern char D_00551978[];

void *sdfChipAllocZeroDirect(s32 size);

extern s32 sdfFileCmdAnalyzeFileName(const char *path, char **outResolvedPath);

extern u16 sdfFileCmdDefaultCreateMode;

extern s32 sdfFileCmdThreadPrio;

extern s32 ChangeThreadPriority(s32 tid, s32 prio);

s32 sdfCreateThreadWithStack(void (*entryAddress)(), s32 stackBytes, s32 priority);

extern char D_00549878[];

extern u8 sdfDev9LoadFlag;

extern u8 sdfDev9Ready;

extern s32 sceSifLoadStartModule(const char *modulePath, s32 flags, void *moduleArguments, s32 *resident);

s32 printf(const char *format, ...);

extern f32 D_0052D944[];

extern f32 D_00529B90[4];

extern f32 D_00529BA0[4];

extern f32 D_00529620[4];

extern sdfRandCB_t sdfDefaultRandCB;

extern s32 sceCdReadClock(sdfCdClock_t *);

extern u8 sdfCdClock;

extern sdfClock_t D_005519B8;

extern sndRingPacket_t D_00590840[32];

extern u32 sdfSndSendCmd_cmd_id;

extern vu16 sdfSndCmdIndex;

extern vu16 sdfSndCmdTailIndex;

extern s32 sdfSndCmdBuf;

extern s32 WakeupThread(s32 thread);

extern void FlushCache(s32);

extern sndListenerState_t D_005907F0;

char *sdfSndMakeFilename(char *pathBuffer, const char *filename);

extern s32 p2sdLoadCheckMidi(s32 packedTrack);

extern s32 p2sdSmgHandle;

extern s32 sprintf();

extern sndFileRequestState_t D_00593E20;

extern s32 D_00552120;

extern char D_00529BE8[];

extern s32 sifRpcServerSema;

extern char D_00551A20[];

extern char D_00551A28[];

extern char D_00551A30[];

extern char D_00551A38[];

extern char D_00551A40[];

extern char sdfDumpLineBuf[];

extern char D_00551A50[];

extern char D_00549B50[];

extern char D_00549B70[];

extern char D_00549B88[];

extern char D_00549BB0[];

extern char D_00549BD0[];

extern char *sdfDmaTagNameTable[];

extern char D_00551A58[];

extern char D_00551A60[];

extern char D_00551B10[];

extern char D_00551B18[];

extern char D_0054A2D8[];

sdfGsMemSlice_t *sdfGsMemGetLinkHead(void);

extern char D_0054A088[];

extern char D_0054A100[];

extern char D_00551B40[];

extern char *D_00529CC8[];

sdfGsMemSlice_t *sdfGsMemGetLinkTail(void);

extern char *D_00529CD8[];

void sdfMemGetInfo(sdfMemInfo_t *stats);

void sdfChipGetInfo(sdfChipInfo_t *stats);

void sdfMemUnlock(sdfMemHandle_t *allocation);

void sdfTexListAdd(sdfItemArray_t *list, sdfTexHandle_t *texture);

sdfMemHandle_t *sdfMemAllocDirect(s32 size);

extern sdfItemArray_t *sdfTexListNew(u32 capacity);

extern sdfIpuDecodeCb_t *sdfIpuQueueHead;

extern sdfIpuDecodeCb_t *sdfIpuQueueTail;

void *sdfAllocAuto(s32 size);

extern void * memset(void *, s32, u32);

sdfTexHandle_t *sdfTexSet(const TMX_HEADER_t *header);

extern sdfGsMemSlice_t *sdfGsMemAllocRect(s32, s32, s32, s32, s32);

void sdfFreeAuto(void *data);

extern s32 sceIpuSync(s32, s32);

extern sdfIpuDecodeCb_t *sdfCurrIpuDecodeCb;

extern void sdfImageSetChainReleaseMem(s32);

extern void sdfGsMemFreeDelayed(sdfGsMemSlice_t *texture);

extern sdfIpuDecodeCb_t *D_00552158;

extern s32 sdfIpuDecode(sdfIpuDecodeCb_t *);

extern void sceIpuStopDMA(void *);

extern s32 sdfIpuDecodeThId;

extern s32 iWakeupThread(s32 threadId);

extern s32 D_00552144;

void sdfImageSetChainTag(sdfDmaAddress_t packetAddress, sdfTextureDmaTail_t *packetTail);

u32 sdfTexGetPixelAddr(sdfTexHandle_t *texture);

extern s32 sdfFromIpuDmacHandlerHandle;

extern s32 sdfToIpuDmacHandlerHandle;

extern void sceIpuInit(void);

extern s32 DmacAddHandler(s32, s32, void *, s32, s32);

extern void EnableDmac(s32);

extern s32 CreateThreadSimple(void *entryAddress, void *workspace, s32 stackBytes, s32 priority);

extern u8 sdfIpuDecodeThStack[];

extern void sdfIpuRemoveHandler(void *);

extern s32 DisableDmac(s32);

void sdfCdImageClose(void);

sdfFileCmdHandle_t *sdfFileCmdReadOpen(const char *path, void (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32),
                        s32 callbackContext);

s32 sdfFileCmdClearError(sdfFileCmd_t *state);

void sdfFileCmdStartThread(s32 index);
extern s32 (*p2sdSmgBuf)(void);

s32 sdfGetCdClock(sdfClock_t *now);

void p2sdLoadSMGFile(s32 packed);

void sdfIpuExecQueue(s32 skipInterruptGuard);

void sdfIpuMakeBltPacket(sdfIpuDecodeCb_t *node);

u32 *sdfFileRpcHandleCmd(u32 command, sdfSoundRpcRequest_t *request);

void sdfGrFlushOtChain(u32 *source);


/* Copy a positive source/end span into the RPC reply buffer; otherwise return NULL. */
void *sdfPrintRpcFunc(s32 unused, sifCommand_t *packet) {
    s32 copyBytes = packet->end - packet->source;
    if (copyBytes > 0) {
        memcpy(packet, (void *)packet->source, copyBytes);
        return packet;
    }
    return NULL;
}

INCLUDE_ASM(const s32, "code_002BD340", sdfPrintRpcThread);

INCLUDE_ASM(const s32, "code_002BD340", sdf_2BD3E8);

INCLUDE_ASM(const s32, "code_002BD340", sdfPrintIopInit);

/* Print text through the bound IOP dev client, or locally when it is unbound. */
void sdfPuts(const char *text) {
    u8 buffer[0x13F];
    u8 *alignedBuffer;
    s32 length;
    if (sdfPrintIopFlag != 0) {
        length = strlen(text);
        if (length != 0) {
            alignedBuffer = (u8 *)(((u32)&buffer[0x3F]) & ~0x3F);
            memcpy(alignedBuffer, text, length);
            sceSifMCallRpc(&D_0058F310.client, 0x7D1, 0, alignedBuffer, length, NULL, 0, NULL, NULL);
        }
    } else {
        scePrintf(D_00551908, text);
    }
}

s32 sdfPrintf(const char *fmt, ...) {
    char buffer[0x100];
    __builtin_va_list args;
    s32 length;
    __builtin_stdarg_start(args, fmt);
    length = vsprintf(buffer, fmt, args);
    sdfPuts(buffer);
    return length;
}

void *sdfCdImageGetDirData(void)
{
    return sdfCdImageDirData;
}

INCLUDE_ASM(const s32, "code_002BD340", sdfCdWaitTrayClose);

void sdfCdStStart(s32 request) {
    u8 options[4];
    s32 ready;
    options[0] = 0;
    if (D_00552110 == 1) {
        options[1] = 0;
    } else {
        options[1] = 1;
    }
    options[2] = 0;
    options[3] = 0;
    for (;;) {
        WaitSema(sdfCdImageFd);
        ready = sceCdStStart(request, options);
        SignalSema(sdfCdImageFd);
        if (ready != 0) {
            break;
        }
        if (sceCdStatus() == 1) {
            sdfCdWaitTrayClose();
        } else {
            sdfDelayThread(100);
        }
    }
    D_0055210C = request;
}

INCLUDE_ASM(const s32, "code_002BD340", sdfCdStRead);

INCLUDE_ASM(const s32, "code_002BD340", sdfCdImageSearchFileDirect);

INCLUDE_ASM(const s32, "code_002BD340", sdfCdImageSearchFile);

INCLUDE_ASM(const s32, "code_002BD340", sdfCdImageLoadStart);

/* Consume up to size disc-file bytes, retaining a partial sector; return the byte count. */
s32 sdfCdImageRead(void *buffer, s32 byteCount) {
    s32 available = sdfCdImageCurrRemain;
    u8 *destination = buffer;
    s32 bytes;
    s32 transferBytes;
    s32 t;
    if (available <= 0) {
        return 0;
    }
    if (byteCount > available) {
        byteCount = available;
    }
    if (byteCount == 0) {
        return 0;
    }
    bytes = byteCount;
    transferBytes = sdfCdImageCurrActive;
    if (transferBytes > 0) {
        if (transferBytes > byteCount) {
            transferBytes = byteCount;
        }
        memcpy(destination, sdfCdImageOffset, transferBytes);
        destination += transferBytes;
        sdfCdImageOffset += transferBytes;
        sdfCdImageCurrActive -= transferBytes;
        bytes -= transferBytes;
    }
    if (bytes > 0) {
        transferBytes = bytes;
        if (((u32)destination & 15) == 0) {
            t = transferBytes >> 11;
            t = transferBytes - t * 0x800;
            transferBytes -= t;
            if (transferBytes > 0) {
                sdfCdStRead(transferBytes >> 11, (s32)destination);
            }
            if (t > 0) {
                sdfCdStRead(1, (s32)D_0058F7C0);
                memcpy(destination + transferBytes, D_0058F7C0, t);
                sdfCdImageOffset = D_0058F7C0 + t;
                sdfCdImageCurrActive = 0x800 - t;
            }
        } else {
            while (transferBytes >= 0x800) {
                sdfCdStRead(1, (s32)D_0058F7C0);
                memcpy(destination, D_0058F7C0, 0x800);
                transferBytes -= 0x800;
                destination += 0x800;
            }
            if (transferBytes > 0) {
                sdfCdStRead(1, (s32)D_0058F7C0);
                memcpy(destination, D_0058F7C0, transferBytes);
                sdfCdImageOffset = D_0058F7C0 + transferBytes;
                sdfCdImageCurrActive = 0x800 - transferBytes;
            }
        }
    }
    sdfCdImageCurrRemain = available - byteCount;
    if (sdfCdImageCurrRemain == 0) {
        sdfCdImageClose();
    }
    return byteCount;
}

void sdfCdImageClose(void)
{
    if (sdfCdImageCurrFile != 0) {
        WaitSema(sdfCdImageFd);
        sceCdStStop();
        SignalSema(sdfCdImageFd);
        sdfCdImageCurrFile = NULL;
    }
}

s32 sdfCdImageCheckExist(const char *aFileName)
{
    s32 lsn;

    return sdfCdImageSearchFile(aFileName, &lsn, NULL) != 0;
}

s32 sdfCdImageGetFileSize(const char *aFileName)
{
    s32 lsn;
    sdfCdImageEntry_t *entry = sdfCdImageSearchFile(aFileName, &lsn, NULL);

    if (entry == NULL) {
        return -1;
    }
    return entry->size;
}

s32 sdfCdImageGetCurrFileSize(void)
{
    sdfCdImageEntry_t *file = sdfCdImageCurrFile;

    if (file == NULL) {
        FatalError(D_00549640);
    }
    return file->size;
}

/* Read a whole named file into the loader's allocated EE buffer, then close it. */
void sdfCdImageLoadDir(const char *fileName) {
    s32 fileHandle = sceOpen(fileName, 1);
    s32 fileBytes;
    void *buffer;
    if (fileHandle < 0) {
        FatalError(D_00549658, fileName);
    }
    fileBytes = sceLseek(fileHandle, 0, 2);
    sceLseek(fileHandle, 0, 0);
    buffer = malloc(fileBytes);
    sdfCdImageDirData = buffer;
    sceRead(fileHandle, (s32)buffer, fileBytes);
    sceClose(fileHandle);
}

/* Record the disc search path, preload a separate file, and initialize an aligned IOP buffer. */
void sdfCdImageInit(const char *discFileName, const char *preloadFileName) {
    u32 discFileRecord[12];
    s32 iopHeapAddress;
    dds3ImgPath = (u32)discFileName;
    sdfCdImageLoadDir(preloadFileName);
    if (sceCdSearchFile(discFileRecord, (s32)discFileName) == 0) {
        FatalError(D_00549658, discFileName);
    }
    dds3ImgLsn = discFileRecord[0];
    iopHeapAddress = sceSifAllocIopHeap(0x28010);
    sdfCdImageFileName = iopHeapAddress;
    sceCdStInit(0x50, 5, (iopHeapAddress + 15) & -16);
}

s32 sdfCdInit(void)
{
    sdfCdImageFd = sdfCreateSema(1, 0xFF, 0);
    sceCdInit(0);
    return sceCdMmode(sdfCdTrayMode);
}

/* Convert a relative disc path to the drive's uppercase backslash form with ;1 suffix. */
void sdfFileSetCdFilename(char *dst, char *src) {
    s32 c;
    memcpy(dst, D_00551928, 8);
    dst += 7;
    c = *src++;
    while (c != 0) {
        if (c == '/') {
            c = '\\';
        }
        if (c >= 'a' && c <= 'z') {
            c -= 0x20;
        }
        *dst++ = c;
        c = *src++;
    }
    dst[0] = ';';
    dst[1] = '1';
    dst[2] = 0;
}

char *sdfFileSetHostFilename(char *dst, char *src) {
    if (*src == 0x2F) {
        strcpy(dst, sdfFileHostRootPath);
        return strcat(dst, src);
    }
    *(sdfHostPrefix_t *)dst = D_00551930[0];
    return strcat(dst, src);
}

void sdfFileSetHddFilename(char *destination, char *path) {
    memcpy(destination, D_00551938, 6);
    strcat(destination, path);
}

s32 sdfFileChkCdFile(char *aFileName)
{
    return 0;
}

s32 sdfFileChkHostFile(char *aFileName)
{
    return 0;
}

s32 sdfFileChkHddFile(char *aFileName)
{
    return 0;
}

/* Return an allocated device path, normalizing disc-path case and separators. */
char *sdfFileMakeFilename(char *path) {
    s32 length = strlen(path);
    char *result = sdfChipAllocDirect(0x80);
    char *cursor;
    s32 character;
    if (strchr(path, ':') != NULL) {
        memcpy(result, path, length);
        result[length] = 0;
        if (strncmp(result, D_00551940, 5) == 0) {
            cursor = result + 5;
            character = *cursor;
            while (character != 0) {
                if (character == '/') *cursor = '\\';
                else if (character >= 'a' && character <= 'z') *cursor = character - 0x20;
                cursor++;
                character = *cursor;
            }
            *cursor++ = ';';
            *cursor++ = '1';
            *cursor = 0;
        }
    } else if (sdfFileChkCdFile(path)) {
        sdfFileSetCdFilename(result, path);
    } else if (sdfFileChkHostFile(path)) {
        sdfFileSetHostFilename(result, path);
    } else if (sdfFileChkHddFile(path)) {
        sdfFileSetHddFilename(result, path);
    } else {
        switch (sdfFileDefaultSrcType) {
        case 0: sdfFileSetCdFilename(result, path); break;
        case 2: sdfFileSetHddFilename(result, path); break;
        case 1: sdfFileSetHostFilename(result, path); break;
        default: strcpy(result, path); break;
        }
    }
    return result;
}

s32 sdfFileCallback(void *aCmd, s32 aOp, s32 aArg, s32 aResult)
{
    switch (aOp) {
    case 4:
        sdfFileLengthWork = aResult;
        break;
    case 3:
        sdfFileSeekWork = aResult;
        break;
    }
    return SignalSema(sdfFileSemaId);
}

/* Open a path using the shared reply callback and wait for its initial completion. */
sdfFileCmdHandle_t *sdfFileOpen(const char *path) {
    sdfFileCmdHandle_t *state;
    if (sdfFileSemaId < 0) {
        sdfFileSemaId = sdfCreateSema(0, 0x80, 0);
    }
    state = sdfFileCmdReadOpen(path, (void (*)(sdfFileCmdHandle_t *, s32, s32, s32, s32))sdfFileCallback, 0);
    WaitSema(sdfFileSemaId);
    sdfFileCmdClearError((sdfFileCmd_t *)state);
    return state;
}

s32 sdfFileCheckExist(char *path) {
    char *resolved = sdfFileMakeFilename(path);
    s32 fd;
    if (*resolved == '/') {
        return sdfCdImageCheckExist(resolved);
    }
    fd = sceOpen(resolved, 1);
    sceClose(fd);
    sdfChipFreeDirect(resolved);
    return fd >= 0;
}

void sdfFileClose(void *aHandle)
{
    sdfFileCmdClose(aHandle);
    WaitSema(sdfFileSemaId);
    sdfFileCmdFree(aHandle);
}

s32 sdfFileRead(void *aHandle, void *aBuffer, s32 aSize)
{
    sdfFileCmdRead(aHandle, aBuffer, aSize);
    return WaitSema(sdfFileSemaId);
}

u32 sdfFileLength(void *aHandle)
{
    sdfFileCmdFileSize(aHandle);
    WaitSema(sdfFileSemaId);
    return sdfFileLengthWork;
}

u32 sdfFileSeek(void *aHandle, s32 aOffset, s32 aOrigin)
{
    sdfFileCmdSeek(aHandle, aOffset, aOrigin);
    WaitSema(sdfFileSemaId);
    return sdfFileSeekWork;
}

/* Set the current device directory, ending in a slash; path is assumed nonempty. */
void sdfFileSetHostRoot(char *path) {
    s32 length = strlen(path);
    ((sdfDevicePrefix_t *)sdfFileHostRootPath)->head = ((sdfDevicePrefix_t *)D_00551930)->head;
    ((sdfDevicePrefix_t *)sdfFileHostRootPath)->tail = ((sdfDevicePrefix_t *)D_00551930)->tail;
    memcpy(sdfFileHostRootPath + 6, path, length);
    if (path[length - 1] != '/') {
        sdfFileHostRootPath[length + 6] = '/';
        sdfFileHostRootPath[length + 7] = 0;
    } else {
        sdfFileHostRootPath[length + 6] = 0;
    }
}

char *sdfFileGetHostRoot(void)
{
    return sdfFileHostRootPath;
}

static void sdfFileCmdSetDefaultDevice(s32 aDevice);

void sdfFileSetDefaultSrc(s32 aSrcType)
{
    sdfFileDefaultSrcType = aSrcType;
    sdfFileCmdSetDefaultDevice(aSrcType);
}

static void sdfFileCmdSetDefaultDevice(s32 aDevice)
{
    sdfFileCmdDefaultDevice = aDevice;
}

/* Queue a request on the global list and on its worker's list; wake the worker when its list was empty. */
void sdfFileCmdAllocHandle(sdfFileCmdHandle_t *state) {
    sdfFileDeviceCB_t *worker;
    s32 interrupts;
    s32 wake = 0;
    sdfFileCmdHandle_t *last;
    interrupts = DIntr(state);
    worker = &sdfFileDeviceCB[state->workerIndex];
    if (worker->threadId < 0) {
        sdfFileCmdStartThread(state->workerIndex);
    }
    state->previous = sdfFileCmdFreeList;
    if (sdfFileCmdFreeList != NULL) {
        sdfFileCmdFreeList->next = state;
    } else {
        D_00551948 = state;
    }
    last = worker->last;
    sdfFileCmdFreeList = state;
    state->workerPrev = last;
    if (last != NULL) {
        last->workerNext = state;
    } else {
        worker->first = state;
        wake = 1;
    }
    worker->last = state;
    sdfFileCmdFreeCount += 1;
    if (interrupts != 0) {
        EIntr();
    }
    if (wake != 0) {
        SignalSema(worker->semaphore);
    }
}

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCmdUnlink);

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCmdDequeue);

s32 sdfFileCmdReleaseHandle(sdfFileCmd_t *aCmd)
{
    s32 fd = aCmd->fd;

    aCmd->fd = -1;
    if (fd >= 0) {
        sceClose(fd);
    }
    return sdfFileCmdDequeue(aCmd);
}

/* Store an error/result, release the request, and notify its callback of inactivity. */
void sdfFileCmdFinish(sdfFileCmdHandle_t *state, s32 result) {
    state->result = result;
    state->state = SDF_DEV_STATE_INACTIVE;
    sdfFileCmdReleaseHandle((sdfFileCmd_t *)state);
    if (state->callback != NULL) {
        state->callback(state, 0, 0, 0, state->callbackContext);
    }
}

INCLUDE_RODATA(const s32, "code_002BD340", D_00549640);

INCLUDE_RODATA(const s32, "code_002BD340", D_00549658);

void sdfFileCmdThread(sdfFileDeviceCB_t *worker) {
    sdfFileCmdHandle_t *state;
    void (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32);
    s32 fileHandle;
    s32 result;
    s32 position;
    s32 transferResult;
    s32 endOffset;
    void *buffer;
    u32 operationIndex;
    for (;;) {
        WaitSema(worker->semaphore);
        state = worker->first;
        if (state == NULL) {
            continue;
        }
        operationIndex = state->operation - SDF_DEV_OPERATION_OPEN_READ;
        state->operation = SDF_DEV_OPERATION_NONE;
        switch (operationIndex) {
        case SDF_DEV_OPERATION_OPEN_READ - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_OPENING;
            fileHandle = sceOpen(state->resolvedPath, 1, 0);
            if (fileHandle < 0) {
                sdfFileCmdFinish(state, fileHandle);
                continue;
            }
            state->state = SDF_DEV_STATE_ACTIVE;
            state->resourceId = fileHandle;
            state->transferred = 0;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 2, 0, 0, state->callbackContext);
            }
            continue;
        case SDF_DEV_OPERATION_OPEN_WRITE - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_OPENING;
            fileHandle = sceOpen(state->resolvedPath, 0x202, state->options);
            if (fileHandle < 0) {
                sdfFileCmdFinish(state, fileHandle);
                continue;
            }
            state->state = SDF_DEV_STATE_ACTIVE;
            state->resourceId = fileHandle;
            state->transferred = 0;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 2, 0, 0, state->callbackContext);
            }
            continue;
        case SDF_DEV_OPERATION_SEEK - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_SEEKING;
            endOffset = sceLseek(state->resourceId, state->operationArg, state->options);
            if (endOffset < 0) {
                sdfFileCmdFinish(state, endOffset);
                continue;
            }
            state->state = SDF_DEV_STATE_ACTIVE;
            state->transferred = endOffset;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 3, 0, endOffset, state->callbackContext);
            }
            continue;
        case SDF_DEV_OPERATION_GET_SIZE - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_SEEKING;
            position = sceLseek(state->resourceId, 0, 1);
            if (position < 0) {
                sdfFileCmdFinish(state, position);
                continue;
            }
            endOffset = sceLseek(state->resourceId, 0, 2);
            if (endOffset < 0) {
                sdfFileCmdFinish(state, endOffset);
                continue;
            }
            result = sceLseek(state->resourceId, position, 0);
            if (result < 0) {
                sdfFileCmdFinish(state, result);
                continue;
            }
            state->state = SDF_DEV_STATE_ACTIVE;
            state->transferred = endOffset;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 4, 0, endOffset, state->callbackContext);
            }
            continue;
        case SDF_DEV_OPERATION_READ - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_READING;
            result = sceRead(state->resourceId, (s32)state->requestData,
                           state->requestExtra);
            if (result < 0) {
                sdfFileCmdFinish(state, result);
                continue;
            }
            state->state = SDF_DEV_STATE_ACTIVE;
            state->transferred += result;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 5, (s32)state->requestData, result,
                                state->callbackContext);
            }
            continue;
        case SDF_DEV_OPERATION_WRITE - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_WRITING;
            result = sceWrite(state->resourceId, (s32)state->requestData,
                            state->requestExtra);
            if (result < 0) {
                sdfFileCmdFinish(state, result);
                continue;
            }
            state->state = SDF_DEV_STATE_ACTIVE;
            state->transferred += result;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 6, (s32)state->requestData, result,
                                state->callbackContext);
            }
            continue;
        case SDF_DEV_OPERATION_CLOSE - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_CLOSING;
            result = sceClose(state->resourceId);
            state->resourceId = -1;
            if (result < 0) {
                sdfFileCmdFinish(state, result);
                continue;
            }
            ((void (*)(void *))sdfFileCmdDequeue)(state);
            state->state = SDF_DEV_STATE_COMPLETE;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 7, 0, 0, state->callbackContext);
            }
            continue;
        case SDF_DEV_OPERATION_READ_BUFFER - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_OPENING;
            fileHandle = sceOpen(state->resolvedPath, 1, 0);
            if (fileHandle < 0) {
                sdfFileCmdFinish(state, fileHandle);
                continue;
            }
            state->resourceId = fileHandle;
            transferResult = state->requestExtra;
            /* A negative count means the whole file; NULL storage is allocated below. */
            if (transferResult < 0) {
                state->state = SDF_DEV_STATE_SEEKING;
                transferResult = sceLseek(fileHandle, 0, 2);
                if (transferResult < 0) {
                    sdfFileCmdFinish(state, transferResult);
                    continue;
                }
                result = sceLseek(state->resourceId, 0, 0);
                if (result < 0) {
                    sdfFileCmdFinish(state, result);
                    continue;
                }
            }
            if (transferResult != 0) {
                buffer = state->requestData;
                if (buffer == NULL) {
                    buffer = (void *)sdfMemLock((sdfMemHandle_t *)(sdfMemAllocDirect(transferResult)));
                }
                state->requestData = buffer;
                state->state = SDF_DEV_STATE_READING;
                transferResult = sceRead(state->resourceId, (s32)buffer, transferResult);
                if (transferResult < 0) {
                    sdfFileCmdFinish(state, transferResult);
                    continue;
                }
            }
            state->state = SDF_DEV_STATE_CLOSING;
            sceClose(state->resourceId);
            state->resourceId = -1;
            ((void (*)(void *))sdfFileCmdDequeue)(state);
            state->state = SDF_DEV_STATE_COMPLETE;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 8, (s32)state->requestData, transferResult,
                         state->callbackContext);
            }
            SignalSema(worker->semaphore);
            continue;
        case SDF_DEV_OPERATION_WRITE_BUFFER - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_OPENING;
            fileHandle = sceOpen(state->resolvedPath, 0x202, state->options);
            if (fileHandle < 0) {
                sdfFileCmdFinish(state, fileHandle);
                continue;
            }
            state->resourceId = fileHandle;
            transferResult = state->requestExtra;
            if (transferResult != 0) {
                state->state = SDF_DEV_STATE_WRITING;
                transferResult = sceWrite(fileHandle, (s32)state->requestData, transferResult);
                if (transferResult < 0) {
                    sdfFileCmdFinish(state, transferResult);
                    continue;
                }
            }
            break;
        default:
            continue;
        }
        state->state = SDF_DEV_STATE_CLOSING;
        sceClose(state->resourceId);
        state->resourceId = -1;
        ((void (*)(void *))sdfFileCmdDequeue)(state);
        state->state = SDF_DEV_STATE_COMPLETE;
        callback = state->callback;
        if (callback != NULL) {
            callback(state, 8, (s32)state->requestData, transferResult,
                            state->callbackContext);
        }
        SignalSema(worker->semaphore);
    }
}

/* Worker slot 3 services disc-backed requests through serialized disc operations. */
void sdfFileCdImageCmdThread(sdfFileDeviceCB_t *worker) {
    sdfFileCmdHandle_t *state;
    void (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32);
    s32 result;
    s32 transferResult;
    void *readBuffer;
    u32 operation;
    for (;;) {
        WaitSema(worker->semaphore);
        state = worker->first;
        if (state == NULL) {
            continue;
        }
        operation = state->operation - SDF_DEV_OPERATION_OPEN_READ;
        state->operation = SDF_DEV_OPERATION_NONE;
        switch (operation) {
        case SDF_DEV_OPERATION_OPEN_READ - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_OPENING;
            result = sdfCdImageLoadStart(state->resolvedPath);
            if (result < 0) {
                sdfFileCmdFinish(state, 0);
                continue;
            }
            state->state = SDF_DEV_STATE_ACTIVE;
            state->transferred = 0;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 2, 0, 0, state->callbackContext);
            }
            continue;
        case SDF_DEV_OPERATION_GET_SIZE - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_ACTIVE;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 4, 0, sdfCdImageGetCurrFileSize(),
                                state->callbackContext);
            }
            continue;
        case SDF_DEV_OPERATION_READ - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_READING;
            result = sdfCdImageRead(state->requestData, state->requestExtra);
            if (result < 0) {
                sdfFileCmdFinish(state, result);
                continue;
            }
            state->state = SDF_DEV_STATE_ACTIVE;
            state->transferred += result;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 5, (s32)state->requestData, result,
                                state->callbackContext);
            }
            continue;
        case SDF_DEV_OPERATION_CLOSE - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_CLOSING;
            sdfCdImageClose();
            ((void (*)(void *))sdfFileCmdDequeue)(state);
            state->state = SDF_DEV_STATE_COMPLETE;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 7, 0, 0, state->callbackContext);
            }
            continue;
        case SDF_DEV_OPERATION_READ_BUFFER - SDF_DEV_OPERATION_OPEN_READ:
            state->state = SDF_DEV_STATE_OPENING;
            result = sdfCdImageLoadStart(state->resolvedPath);
            if (result < 0) {
                sdfFileCmdFinish(state, 0);
                continue;
            }
            transferResult = state->requestExtra;
            if (transferResult < 0) {
                transferResult = sdfCdImageGetCurrFileSize();
            }
            if (transferResult != 0) {
                readBuffer = state->requestData;
                if (readBuffer == NULL) {
                    readBuffer = (void *)sdfMemLock((sdfMemHandle_t *)(sdfMemAllocDirect(transferResult)));
                }
                state->requestData = readBuffer;
                state->state = SDF_DEV_STATE_READING;
                transferResult = sdfCdImageRead(readBuffer, transferResult);
                if (transferResult < 0) {
                    sdfFileCmdFinish(state, transferResult);
                    continue;
                }
            }
            state->state = SDF_DEV_STATE_CLOSING;
            sdfCdImageClose();
            state->resourceId = -1;
            ((void (*)(void *))sdfFileCmdDequeue)(state);
            state->state = SDF_DEV_STATE_COMPLETE;
            callback = state->callback;
            if (callback != NULL) {
                callback(state, 8, (s32)state->requestData, transferResult,
                                state->callbackContext);
            }
            continue;
        case SDF_DEV_OPERATION_OPEN_WRITE - SDF_DEV_OPERATION_OPEN_READ:
        case SDF_DEV_OPERATION_SEEK - SDF_DEV_OPERATION_OPEN_READ:
        case SDF_DEV_OPERATION_WRITE - SDF_DEV_OPERATION_OPEN_READ:
        case SDF_DEV_OPERATION_WRITE_BUFFER - SDF_DEV_OPERATION_OPEN_READ:
            sdfFileCmdFinish(state, 0);
            continue;
        default:
            continue;
        }
    }
}

/* Allocate a worker-specific device path; an unsupported worker returns NULL. */
char *sdfFileCmdMakeRealFileName(char *path, s32 worker) {
    s32 length = strlen(path);
    char *result;
    s32 index;
    s32 character;
    char *base;
    s32 baseLength;
    switch (worker) {
    case 0:
        result = sdfChipAllocDirect(length + 10);
        memcpy(result, D_00551968, 7);
        index = 0;
        character = path[index];
        while (character != 0) {
            if (character == '/') character = '\\';
            else if (character >= 'a' && character <= 'z') character -= 0x20;
            result[index + 7] = character;
            index++;
            character = path[index];
        }
        result[index + 7] = ';';
        result[index + 8] = '1';
        result[index + 9] = 0;
        break;
    case 1:
        if (path[0] != '/') {
            result = sdfChipAllocDirect(length + 7);
            memcpy(result, D_00551970, 6);
            memcpy(result + 6, path, length);
            result[length + 6] = 0;
        } else {
            base = sdfFileGetHostRoot();
            baseLength = strlen(base);
            result = sdfChipAllocDirect(length + baseLength);
            memcpy(result, base, baseLength);
            memcpy(result + baseLength, path + 1, length);
        }
        break;
    case 2:
        result = sdfChipAllocDirect(length + 6);
        memcpy(result, D_00551978, 5);
        memcpy(result + 5, path, length);
        result[length + 5] = 0;
        break;
    case 3:
        result = sdfChipAllocDirect(length + 1);
        strcpy(result, path);
        break;
    default:
        return NULL;
    }
    return result;
}

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCmdAnalyzeFileName);

/* Allocate a request state and retain the callback's opaque context word. */
sdfFileCmdHandle_t *sdfFileCmdNewHandle(void *resource, s32 workerIndex, s32 operation,
                        void (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32), s32 callbackContext) {
    sdfFileCmdHandle_t *state = (sdfFileCmdHandle_t *)sdfChipAllocZeroDirect(sizeof(sdfFileCmdHandle_t));
    state->resolvedPath = resource;
    state->workerIndex = workerIndex;
    state->operation = operation;
    state->state = 0;
    state->resourceId = -1;
    state->callback = callback;
    state->callbackContext = callbackContext;
    return state;
}

/* Resolve a path to its worker and enqueue an asynchronous read-only open. */
sdfFileCmdHandle_t *sdfFileCmdReadOpen(const char *path, void (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32),
                        s32 callbackContext) {
    char *resolvedPath;
    s32 workerIndex = sdfFileCmdAnalyzeFileName(path, &resolvedPath);
    sdfFileCmdHandle_t *state;
    if (workerIndex < 0) {
        return NULL;
    }
    state = sdfFileCmdNewHandle(resolvedPath, workerIndex, SDF_DEV_OPERATION_OPEN_READ, callback, callbackContext);
    sdfFileCmdAllocHandle(state);
    return state;
}

/* Enqueue a write open; zero options select the device's default creation options. */
sdfFileCmdHandle_t *sdfFileCmdWriteOpen(const char *path, void (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32),
                        s32 callbackContext, s32 options) {
    char *resolvedPath;
    s32 workerIndex = sdfFileCmdAnalyzeFileName(path, &resolvedPath);
    sdfFileCmdHandle_t *state;
    if (workerIndex < 0) {
        return NULL;
    }
    state = sdfFileCmdNewHandle(resolvedPath, workerIndex, SDF_DEV_OPERATION_OPEN_WRITE, callback, callbackContext);
    state->options = options != 0 ? options : sdfFileCmdDefaultCreateMode;
    sdfFileCmdAllocHandle(state);
    return state;
}

/* Queue a seek with its offset and origin; return -1 unless the request is active. */
s32 sdfFileCmdSeek(void *handle, s32 seekOffset, s32 seekOrigin) {
    sdfFileCmdHandle_t *state = handle;
    if (state->state != SDF_DEV_STATE_ACTIVE) {
        return -1;
    }
    state->operationArg = seekOffset;
    state->options = seekOrigin;
    state->operation = SDF_DEV_OPERATION_SEEK;
    SignalSema(sdfFileDeviceCB[state->workerIndex].semaphore);
    return 0;
}

/* Query file size without changing the file position; return -1 unless active. */
s32 sdfFileCmdFileSize(void *handle) {
    sdfFileCmdHandle_t *state = handle;
    if (state->state != SDF_DEV_STATE_ACTIVE) {
        return -1;
    }
    state->operation = SDF_DEV_OPERATION_GET_SIZE;
    SignalSema(sdfFileDeviceCB[state->workerIndex].semaphore);
    return 0;
}

/* Queue a byte-counted read; return -1 without changing an inactive request. */
s32 sdfFileCmdRead(void *handle, void *readBuffer, s32 byteCount) {
    sdfFileCmdHandle_t *state = handle;
    if (state->state != SDF_DEV_STATE_ACTIVE) {
        return -1;
    }
    state->requestExtra = byteCount;
    state->operation = SDF_DEV_OPERATION_READ;
    state->requestData = readBuffer;
    SignalSema(sdfFileDeviceCB[state->workerIndex].semaphore);
    return 0;
}

/* Queue a byte-counted write; return -1 without changing an inactive request. */
s32 sdfFileCmdWrite(sdfFileCmdHandle_t *state, const void *buffer, s32 byteCount) {
    if (state->state != SDF_DEV_STATE_ACTIVE) {
        return -1;
    }
    state->requestExtra = byteCount;
    state->operation = SDF_DEV_OPERATION_WRITE;
    state->requestData = (void *)buffer;
    SignalSema(sdfFileDeviceCB[state->workerIndex].semaphore);
    return 0;
}

s32 sdfFileCmdClearError(sdfFileCmd_t *aCmd)
{
    if (aCmd->stat != 9) {
        return -1;
    }
    aCmd->error = 0;
    aCmd->stat = 7;
    return 0;
}

/* Active state and close operation share an ID; retain the original byte assignment. */
s32 sdfFileCmdClose(void *handle) {
    sdfFileCmdHandle_t *request = handle;
    sdfDevStateCode_t requestState = request->state;
    if (requestState != SDF_DEV_STATE_ACTIVE) {
        return -1;
    }
    request->operation = requestState;
    SignalSema(sdfFileDeviceCB[request->workerIndex].semaphore);
    return 0;
}

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCmdFree);

/* Enqueue a one-shot read: negative byte count requests the whole file. */
sdfFileCmdHandle_t *sdfFileCmdReadFile(const char *path, s32 buffer, s32 byteCount,
                        void (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32), s32 callbackContext) {
    char *resolvedPath;
    s32 workerIndex = sdfFileCmdAnalyzeFileName(path, &resolvedPath);
    sdfFileCmdHandle_t *state;
    if (workerIndex < 0) {
        return NULL;
    }
    state = sdfFileCmdNewHandle(resolvedPath, workerIndex, SDF_DEV_OPERATION_READ_BUFFER, callback, callbackContext);
    state->requestExtra = byteCount;
    state->requestData = (void *)buffer;
    state->operationArg = 0;
    sdfFileCmdAllocHandle(state);
    return state;
}

/* Enqueue a one-shot write using the supplied buffer, byte count and creation options. */
sdfFileCmdHandle_t *sdfFileCmdWriteFile(const char *path, s32 buffer, s32 byteCount,
                        void (*callback)(sdfFileCmdHandle_t *, s32, s32, s32, s32),
                        s32 callbackContext, s32 options) {
    char *resolvedPath;
    s32 workerIndex = sdfFileCmdAnalyzeFileName(path, &resolvedPath);
    sdfFileCmdHandle_t *state;
    if (workerIndex < 0) {
        return NULL;
    }
    state = sdfFileCmdNewHandle(resolvedPath, workerIndex, SDF_DEV_OPERATION_WRITE_BUFFER, callback, callbackContext);
    state->requestExtra = byteCount;
    state->requestData = (void *)buffer;
    state->options = options != 0 ? options : sdfFileCmdDefaultCreateMode;
    state->operationArg = 0;
    sdfFileCmdAllocHandle(state);
    return state;
}

/* Update non-negative worker IDs only; skip an unchanged numeric priority. */
void sdfFileCmdThreadSetPrio(s32 priority) {
    sdfFileDeviceCB_t *worker;
    u32 index;
    if (sdfFileCmdThreadPrio == priority) {
        return;
    }
    sdfFileCmdThreadPrio = priority;
    worker = sdfFileDeviceCB;
    index = 0;
    do {
        s32 threadId = worker->threadId;
        worker++;
        if (threadId >= 0) {
            ChangeThreadPriority(threadId, priority);
        }
        index++;
    } while (index < 4);
}

void sdfFileCmdThreadSetLoPrio(void)
{
    sdfFileCmdThreadPrioCheckCounter = 3;
    sdfFileCmdThreadSetPrio(0x78);
}

void sdfFileCmdThreadSetHiPrio(void)
{
    sdfFileCmdThreadSetPrio(0x48);
}

void sdfFileCmdThreadCheckPrio(void)
{
    if (sdfFileCmdThreadPrioCheckCounter != 0) {
        sdfFileCmdThreadPrioCheckCounter--;
        if (sdfFileCmdThreadPrioCheckCounter == 0) {
            sdfFileCmdThreadSetHiPrio();
        }
    }
}

/* Start worker thread `index` if it isn't running; slot 3 runs the alternate entry point. */
void sdfFileCmdStartThread(s32 index) {
    sdfFileDeviceCB_t *worker = &sdfFileDeviceCB[index];
    void (*entry)();
    s32 thread;
    thread = worker->threadId;
    if (thread < 0) {
        worker->semaphore = sdfCreateSema(0, 0xFF, 0);
        entry = sdfFileCdImageCmdThread;
        if (index != 3) {
            entry = sdfFileCmdThread;
        }
        sdfFileCmdThreadPrio = 0x48;
        thread = sdfCreateThreadWithStack(entry, 0x4000, 0x48);
        worker->threadId = thread;
        StartThread(thread, (s32)worker);
    }
}

INCLUDE_ASM(const s32, "code_002BD340", sdfHddPowerOffTh);

void sdfHddPowerOffHandler(s32 aSemaId)
{
    iSignalSema(aSemaId);
}

INCLUDE_ASM(const s32, "code_002BD340", sdfHddInit);

void sdfDev9LoadModule(void) {
    s32 resident;
    if (sdfDev9LoadFlag == 0) {
        sdfDev9Ready = 0;
        if (sceSifLoadStartModule("cdrom0:\\IRX\\DEV9.IRX;1", 0, NULL, &resident) < 0) {
            printf("cdrom0:\\IRX\\DEV9.IRX;1 could't load.\n");
            return;
        }
        if (resident != 0) {
            printf(D_00549878);
            return;
        }
        sdfDev9Ready = 1;
        sdfDev9LoadFlag = 1;
    }
}

INCLUDE_ASM(const s32, "code_002BD340", sdf_2BFC58);

/* Clear `count` quadwords at dst: four per pass, then a falling 3/2/1
 * remainder. The pass is a goto loop (no loop notes, so no loop alignment),
 * and its exit test is taken before the four stores. */
u128 *sdfZeroMemQuad(u128 *dst, s32 count) {
    u128 *p = dst;
    s32 done;
    if (count >= 4) {
loop:
        count -= 4;
        done = count < 4;
        PS2_STORE_ZERO_QUAD(p);
        PS2_STORE_ZERO_QUAD_AT16(p);
        PS2_STORE_ZERO_QUAD_AT32(p);
        PS2_STORE_ZERO_QUAD_AT48(p);
        p += 4;
        if (!done) {
            goto loop;
        }
    }
    switch (count) {
    case 3:
        PS2_STORE_ZERO_QUAD_AT32(p);
    case 2:
        PS2_STORE_ZERO_QUAD_AT16(p);
    case 1:
        PS2_STORE_ZERO_QUAD(p);
    }
    return dst;
}

char *sdfStrdupChip(const char *text) {
    u32 length;
    char *copy;
    if (text == NULL) {
        return NULL;
    }
    length = strlen(text);
    copy = sdfChipAllocDirect(length + 1);
    memcpy(copy, text, length);
    copy[length] = 0;
    return copy;
}

s32 sdfBcdToDecimal(s32 aBcd)
{
    s32 result = 0;
    s32 scale = 1;

    while (aBcd > 0) {
        result += (aBcd & 0xF) * scale;
        aBcd >>= 4;
        scale *= 10;
    }
    return result;
}

/* Pack positive decimal digits into nibbles; non-positive input returns zero.
 * The original signed shifts and lack of an overflow check are retained.
 */
s32 sdf_2BFE08(s32 number) {
    s32 digitShift = 0;
    s32 packedDigits = 0;
    while (number > 0) {
        s32 quotient = number / 10;
        packedDigits |= (number - quotient * 10) << digitShift;
        number = quotient;
        digitShift += 4;
    }
    return packedDigits;
}

/* Allocate element storage with a fixed element stride and growth increment. */
sdfItemArray_t *sdfItemArrayNew(s32 elementCount, s32 elementStride, s32 growthStep) {
    sdfItemArray_t *request = sdfChipAllocDirect(sizeof(*request));
    request->growStep = growthStep;
    request->count = 0;
    request->capacity = elementCount;
    request->elemSize = elementStride;
    if (elementCount != 0) {
        request->memHandle = sdfMemAllocDirect(elementStride * elementCount);
        request->items = (void *)sdfMemLock(request->memHandle);
    } else {
        request->memHandle = 0;
        request->items = 0;
    }
    return request;
}

void sdfItemArrayFree(sdfItemArray_t *aArray)
{
    sdfMemFreeDirect(aArray->memHandle);
    sdfChipFreeDirect(aArray);
}

INCLUDE_ASM(const s32, "code_002BD340", sdfItemArrayGrow);

/* Mirror the phase into a quarter turn, then evaluate an odd ninth-degree polynomial. */
f32 sdfSin(f32 angle) {
    f32 phase = angle * 0.15915494f;
    f32 polynomialInput;
    f32 inputSquared;
    f32 inputCubed;
    f32 inputFifthPower;
    f32 inputSeventhPower;
    f32 inputNinthPower;
    phase -= (s32)phase;
    if (phase > 0.5f) {
        phase -= 1.0f;
    } else if (phase < -0.5f) {
        phase += 1.0f;
    }
    if (phase > 0.25f) {
        phase = 0.5f - phase;
    } else if (phase < -0.25f) {
        phase = -0.5f - phase;
    }
    polynomialInput = phase * 3.9999996f;
    inputSquared = polynomialInput * polynomialInput;
    inputCubed = inputSquared * polynomialInput;
    inputFifthPower = inputCubed * inputSquared;
    inputSeventhPower = inputFifthPower * inputSquared;
    inputNinthPower = inputSeventhPower * inputSquared;
    return polynomialInput * 1.5707963f + inputCubed * -0.64596367f + inputFifthPower * 0.07968968f + inputSeventhPower * -0.0046737656f + inputNinthPower * 0.00015148419f;
}

f32 sdfCos(f32 aRad)
{
    return sdfSin(aRad + SDF_PI_2);
}

/* Binary-search samples, using zero as the implicit lower endpoint.
 * The step is integer 1/sampleCount converted to f32, not a float reciprocal.
 * The caller must provide valid table bounds for the final sample index.
 */
f32 sdfTableInterpolation(f32 lookupValue, const f32 *table, s32 sampleCount) {
    f32 positionStep = 1 / sampleCount;
    s32 lowerIndex = 0;
    s32 upperIndex = sampleCount;
    s32 sampleIndex;
    f32 lowerValue;
    f32 upperValue;
    do {
        sampleIndex = lowerIndex + upperIndex;
        sampleIndex >>= 1;
        upperValue = table[sampleIndex];
        if (lookupValue < upperValue) {
            upperIndex = sampleIndex;
        } else {
            sampleIndex++;
            lowerIndex = sampleIndex;
        }
    } while (lowerIndex < upperIndex);
    upperValue = table[sampleIndex];
    lowerValue = 0.0f;
    if (sampleIndex != 0) {
        lowerValue = table[sampleIndex - 1];
    }
    return sampleIndex * positionStep + (lookupValue - lowerValue) * positionStep / (upperValue - lowerValue);
}

f32 sdfAtan(f32 aX)
{
    f32 x2 = aX * aX;
    f32 x3 = x2 * aX;
    f32 x5 = x2 * x3;

    return aX * 0.9999998f + x3 * -0.33325735f + x5 * 0.19388643f;
}

f32 sdfAtan2(f32 y, f32 x) {
    s32 xNegative = 0;
    s32 yNegative;
    f32 angle;
    if (x < 0.0f) {
        x = -x;
        xNegative = 1;
    }
    yNegative = 0;
    if (y < 0.0f) {
        y = -y;
        yNegative = 1;
    }
    if (y < x) {
        angle = sdfAtan(y / x);
    } else {
        angle = 1.5707963f - sdfAtan(x / y);
    }
    if (xNegative != 0) {
        angle = 3.1415926f - angle;
    }
    if (yNegative != 0) {
        angle = -angle;
    }
    return angle;
}

/* Restore the input sign after sampling; magnitudes at or above one use half pi. */
f32 sdfAsin(f32 x) {
    f32 inputSign;
    f32 angleMagnitude;
    if (x < 0.0f) {
        x = -x;
        inputSign = -1.0f;
    } else {
        inputSign = 1.0f;
    }
    angleMagnitude = x >= 1.0f ? 1.5707963f : sdfTableInterpolation(x, D_0052D944, 128) * 1.5707963f;
    return angleMagnitude * inputSign;
}

/* Return the input-signed complement of the sampled angle, not standard acos(x).
 * Magnitudes at or above one leave the angle magnitude at zero.
 */
f32 sdfAcos(f32 x) {
    f32 inputSign;
    f32 angleMagnitude;
    if (x < 0.0f) {
        x = -x;
        inputSign = -1.0f;
    } else {
        inputSign = 1.0f;
    }
    angleMagnitude = 0.0f;
    if (!(x >= 1.0f)) {
        angleMagnitude = (1.0f - sdfTableInterpolation(x, D_0052D944, 128)) * 1.5707963f;
    }
    return angleMagnitude * inputSign;
}

/* Keep the cast-plus/minus-one turn adjustment; this is not general modulo.
 * Inputs already within the pi thresholds are returned unchanged.
 */
f32 sdfClipAngleValuePI(f32 angle) {
    s32 adjustedTurns;
    if (angle > 3.1415926f) {
        adjustedTurns = (s32)(angle / 6.2831852f) + 1;
        return angle - (f32)adjustedTurns * 6.2831852f;
    }
    if (angle < -3.1415926f) {
        adjustedTurns = (s32)(angle / 6.2831852f) - 1;
        return angle - (f32)adjustedTurns * 6.2831852f;
    }
    return angle;
}

/* vu0 routine: shared quaternion registers are the input/output interface. */
void Mult(void)
{
    __asm__ volatile (
        ".set noreorder\n"
        "\tvmul.xyzw vf2, vf10, vf11\n"
        "\tvopmula.xyz ACC, vf10, vf11\n"
        "\tvmaddaw.xyz ACC, vf11, vf10\n"
        "\tvmaddaw.xyz ACC, vf10, vf11\n"
        "\tvopmsub.xyz vf10, vf11, vf10\n"
        "\tvmulaw.w ACC, vf10, vf11\n"
        "\tvmsubax.w ACC, vf0, vf2\n"
        "\tvmsubay.w ACC, vf0, vf2\n"
        "\tvmsubz.w vf10, vf0, vf2\n"
        "\t.set reorder\n"
        : : : "memory");
}

/* vu0 routine: shared quaternion registers are the input/output interface. */
void sdf_2C04B8(void)
{
    __asm__ volatile (
        ".set noreorder\n"
        "\tvmul.xyzw vf2, vf10, vf10\n"
        "\tvaddax.w ACC, vf2, vf2x\n"
        "\tvmadday.w ACC, vf0, vf2y\n"
        "\tvmaddz.w vf3, vf0, vf2z\n"
        "\tvmove.w vf2, vf10\n"
        "\tvdiv Q, vf0w, vf3w\n"
        "\tvsub.xyz vf2, vf0, vf10\n"
        "\tvwaitq\n"
        "\tvmulq.xyzw vf10, vf2, Q\n"
        "\t.set reorder"
        );
}

/* vu0 routine: normalize all four components of vf10. */
void SDF_QUAT_INNERPRODUCT(void)
{
    __asm__ volatile (
        ".set noreorder\n"
        "\tvmul.xyzw vf2, vf10, vf10\n"
        "\tvaddax.w ACC, vf2, vf2\n"
        "\tvmadday.w ACC, vf0, vf2\n"
        "\tvmaddz.w vf3, vf0, vf2\n"
        "\tvrsqrt Q, vf0w, vf3w\n"
        "\tvwaitq\n"
        "\tvmulq.xyzw vf10, vf10, Q\n"
        "\t.set reorder"
        );
}

/* vu0 routine: retail reduces (x*x)*(y*y) + z*z + w*w, not squared length. */
f32 sdf_2C0510(void)
{
    f32 componentMetric;
    __asm__ volatile (
        ".set noreorder\n"
        "\tvaddw.xyz vf1, vf0, vf0w\n"
        "\tvmul.xyzw vf2, vf10, vf10\n"
        "\tvmulay.x ACC, vf2, vf2y\n"
        "\tvmaddaz.x ACC, vf1, vf2z\n"
        "\tvmaddw.x vf2, vf1, vf2w\n"
        "\tqmfc2.ni $2, vf2\n"
        "\tmtc1 $2, %0\n"
        "\t.set reorder"
        : "=f" (componentMetric));
    return componentMetric;
}

/* vu0 routine: return the component-wise dot product of vf10 and vf11. */
f32 sdf_2C0538(void)
{
    f32 dotProduct;
    VU0_QUAT_DOT_VF10_VF11(dotProduct);
    return dotProduct;
}

/* vf10 initially holds the axis; write the sine-scaled axis and cosine W. */
void FromRotation(f32 angle)
{
    f32 halfAngle = angle * 0.5f;
    f32 trigValue = sdfSin(halfAngle);
    __asm__ volatile (
        ".set noreorder\n"
        "\tmfc1 $2, %0\n"
        "\tqmtc2.ni $2, vf2\n"
        "\tvmulx.xyzw vf10, vf10, vf2x\n"
        "\t.set reorder"
        : : "f" (trigValue));
    trigValue = sdfCos(halfAngle);
    __asm__ volatile (
        ".set noreorder\n"
        "\tmfc1 $2, %0\n"
        "\tqmtc2.ni $2, vf2\n"
        "\tvmulx.w vf10, vf0, vf2x\n"
        "\t.set reorder"
        : : "f" (trigValue));
}

/* As above, but write the second VU quaternion register (vf11). */
void FromRotation_B(f32 angle)
{
    f32 halfAngle = angle * 0.5f;
    f32 trigValue = sdfSin(halfAngle);
    __asm__ volatile (
        ".set noreorder\n"
        "\tmfc1 $2, %0\n"
        "\tqmtc2.ni $2, vf2\n"
        "\tvmulx.xyzw vf11, vf11, vf2x\n"
        "\t.set reorder"
        : : "f" (trigValue));
    trigValue = sdfCos(halfAngle);
    __asm__ volatile (
        ".set noreorder\n"
        "\tmfc1 $2, %0\n"
        "\tqmtc2.ni $2, vf2\n"
        "\tvmulx.w vf11, vf0, vf2x\n"
        "\t.set reorder"
        : : "f" (trigValue));
}

/* vu0 routine: shared quaternion registers are the input/output interface. */
void ToMatrix(void)
{
    __asm__ volatile (
        ".set noreorder\n"
        "\tvaddw.xyz vf1, vf0, vf0w\n"
        "\tvadd.xyzw vf2, vf10, vf10\n"
        "\tvmulx.w vf28, vf0, vf0x\n"
        "\tvmulx.w vf29, vf0, vf0x\n"
        "\tvmulx.w vf30, vf0, vf0x\n"
        "\tvmul.xyzw vf3, vf10, vf2\n"
        "\tvmuly.xyzw vf4, vf10, vf2y\n"
        "\tvmulz.xyzw vf5, vf10, vf2z\n"
        "\tvmulx.xyzw vf6, vf10, vf2x\n"
        "\tvaddaw.xyz ACC, vf0, vf0w\n"
        "\tvmsubay.x ACC, vf1, vf3y\n"
        "\tvmsubz.x vf28, vf1, vf3z\n"
        "\tvmsubax.y ACC, vf1, vf3x\n"
        "\tvmsubz.y vf29, vf1, vf3z\n"
        "\tvmsubax.z ACC, vf1, vf3x\n"
        "\tvmsuby.z vf30, vf1, vf3y\n"
        "\tvmulax.y ACC, vf1, vf4x\n"
        "\tvmsubw.y vf28, vf1, vf5w\n"
        "\tvaddw.x vf29, vf4, vf5w\n"
        "\tvsubw.x vf30, vf5, vf4w\n"
        "\tvmulax.z ACC, vf1, vf5x\n"
        "\tvmaddw.z vf28, vf1, vf4w\n"
        "\tvmulay.z ACC, vf1, vf5y\n"
        "\tvmsubw.z vf29, vf1, vf6w\n"
        "\tvaddw.y vf30, vf5, vf6w\n"
        "\tvmove.xyzw vf31, vf0\n"
        "\t.set reorder"
        : : : "memory");
}

void FromEulerXYZ(f32 x, f32 y, f32 z);

INCLUDE_ASM(const s32, "code_002BD340", FromEulerXYZ);

void FromEuler(f32 x, f32 y, f32 z);

INCLUDE_ASM(const s32, "code_002BD340", FromEuler);

/* vu0 routine: compose pitch and offset-adjusted planar direction quaternions. */
void LookAt(f32 xOffset, f32 yOffset) {
    f32 input[4];
    f32 quaternion[4];
    f32 x;
    f32 y;
    f32 z;
    f32 horizontalSquared;
    f32 horizontalLength;
    f32 s;
    f32 t;
    f32 offsetSquared;
    f32 adjustedZ;
    SDF_VU0__STOREVECTOR(input);
    t = 0.0f;
    x = -input[0];
    y = input[1];
    if (yOffset < t) {
        y = -y;
    }
    z = input[2];
    horizontalSquared = x * x + z * z;
    if (horizontalSquared < 0.000001f) {
        if (y < t) {
            SDF_VU0__LOADVECTOR(D_00529B90);
        } else {
            SDF_VU0__LOADVECTOR(D_00529BA0);
        }
        return;
    }
    /* Pitch quaternion from the vertical and horizontal direction lengths. */
    s = y * y;
    horizontalLength = fsqrtf(horizontalSquared);
    quaternion[1] = t;
    quaternion[2] = t;
    t = fsqrtf(horizontalSquared + s) + horizontalLength;
    s = frsqrtf(1.0f, t * t + s);
    quaternion[0] = y * s;
    quaternion[3] = t * s;
    SDF_VU0__LOADVECTOR(quaternion);
    /* Yaw quaternion from the normalized, offset-adjusted XZ direction. */
    s = 1.0f / horizontalLength;
    x *= s;
    z *= s;
    adjustedZ = z + yOffset;
    t = x + xOffset;
    offsetSquared = t * t + adjustedZ * adjustedZ;
    if (offsetSquared < 0.000001f) {
        SDF_VU0__LOADVECTOR_B(D_00529BB0.quaternion);
    } else {
        s = frsqrtf(1.0f, offsetSquared);
        quaternion[0] = 0.0f;
        quaternion[1] = t * s;
        quaternion[2] = 0.0f;
        quaternion[3] = adjustedZ * s;
        SDF_VU0__LOADVECTOR_B(quaternion);
    }
    Mult();
}

/* vu0 routine: blend vf10/vf11 along the shorter quaternion arc into vf10.
 * blendAmount is used without clamping; the final step normalizes either blend. */
void Slerp(f32 blendAmount) {
    f32 dotProduct;
    f32 firstWeight;
    f32 secondWeight;
    f32 sineAngle;
    f32 angleBetween;
    VU0_QUAT_DOT_VF10_VF11(dotProduct);
    if (dotProduct < 0.0f) {
        dotProduct = -dotProduct;
        VU0_NEGATE_VF11_TO_VF12();
    } else {
        SDF_VU0__COPYVECTOR_B_TO_C();
    }
    firstWeight = 1.0f - blendAmount;
    secondWeight = blendAmount;
    /* Near-identical quaternions use linear weights; normalization still follows. */
    if (dotProduct < 0.99899996f) {
        angleBetween = sdfAcos(dotProduct);
        sineAngle = sdfSin(angleBetween);
        firstWeight = sdfSin(firstWeight * angleBetween) / sineAngle;
        secondWeight = sdfSin(secondWeight * angleBetween) / sineAngle;
    }
    VU0_SET_WEIGHTS_VF2X_VF3X(firstWeight, secondWeight);
    VU0_WEIGHTED_SUM_VF10_VF12();
    SDF_QUAT_INNERPRODUCT();
}

/* vu0 routine: shared quaternion registers are the input/output interface. */
void Lerp(f32 blendAmount)
{
    f32 dotProduct;
    VU0_QUAT_DOT_VF10_VF11(dotProduct);
    if (dotProduct < 0.0f) {
        VU0_NEGATE_VF11_TO_VF12();
    } else {
        SDF_VU0__COPYVECTOR_B_TO_C();
    }
    {
        f32 firstWeight = 1.0f - blendAmount;
        VU0_SET_WEIGHTS_VF2X_VF3X(firstWeight, blendAmount);
        VU0_WEIGHTED_SUM_VF10_VF12();
    }
    SDF_QUAT_INNERPRODUCT();
}

/* vu0 routine: shared quaternion registers are the input/output interface. */
f32 EulerAngleAxisY(void)
{
    f32 rotatedZ;
    f32 rotatedX;
    ToMatrix();
    /* PEXEW exchanges words 0 and 2: output 0 is Z, while output 1 is X. */
    VU0_LOAD_VF10_BARRIER(D_00529620);
    SDF_VU0__APPLYMATRIX_XYZ();
    __asm__ volatile (
        ".set noreorder\n"
        "\tqmfc2.ni $2, vf10\n"
        "\tpexew $2, $2\n"
        "\tmtc1 $2, %0\n"
        "\tqmfc2.ni $2, vf10\n"
        "\tmtc1 $2, %1\n"
        "\t.set reorder"
        : "=f" (rotatedZ), "=f" (rotatedX) : : "$2", "memory");
    return sdfAtan2(rotatedZ, rotatedX);
}

/* Four-word shift-register PRNG; NULL selects the shared effect RNG state. */
u32 dds3GetRandA(void *aState)
{
    sdfRandCB_t *state = aState;
    u32 x0;
    u32 x1;
    u32 x2;
    u32 x3;
    u32 rand;
    if (state == ((void*)0))
    {
        state = &sdfDefaultRandCB;
    }
    x0 = state->words[0];
    x1 = state->words[1];
    x2 = state->words[2];
    x3 = state->words[3];
    /* Mix the high two bits of x0 and high bit of x2 into the shifted taps. */
    rand = ((x1 << 0x02) | (((x0 >> 0x1e)) % 4)) ^ ((x3 << 0x01) | (((x2 >> 0x1f)) % 2));
    state->words[0] = rand;
    state->words[1] = x0;
    state->words[2] = x1;
    state->words[3] = x2;
    return rand;
}

/* Uniform float in [0, 1): 24 random bits scaled by 2^-24. */
f32 dds3GetRandFloatA(sdfRandCB_t *state) {
    u32 value = dds3GetRandA(state) & 0xFFFFFF;
    return (f32)value * 5.9604644775390625e-8f;
}

u32 dds3GetRandIntA(void *aState, u32 aRange)
{
    return dds3GetRandA(aState) % aRange;
}

/* Seed the generator state (the shared effect state when `state` is NULL); each word is the previous one rotated right 8 bits and xored with a constant. */
void sdfRandomize(sdfRandCB_t *state, u32 seed) {
    if (state == NULL) {
        state = &sdfDefaultRandCB;
    }
    seed ^= 0x0AED1A0C;
    state->words[0] = seed;
    seed = (seed << 24) | (seed >> 8);
    state->words[1] = seed;
    seed ^= 0xAA5A02FE;
    seed = (seed << 24) | (seed >> 8);
    state->words[2] = seed;
    seed ^= 0x11BE81C7;
    seed = (seed << 24) | (seed >> 8);
    state->words[3] = seed;
}

/* Seeds the generator at `state` from the current clock (time of day in seconds since an epoch). */
void sdfRandomizeByTime(sdfRandCB_t *state) {
    sdfClock_t now;
    s32 secondsSinceEpoch;
    sdfGetCdClock(&now);
    secondsSinceEpoch = now.year * 0x16E + now.day + D_00529BB0.monthDays[now.month + 7] + 0xFFF4FF83;
    secondsSinceEpoch = secondsSinceEpoch * 0x18 + now.hour;
    secondsSinceEpoch = secondsSinceEpoch * 0x3C + now.min;
    sdfRandomize(state, secondsSinceEpoch * 0x3C + now.sec);
}

INCLUDE_SDATA(const s32, "code_002BD340", sdfPrintIopFlag);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551901);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551908);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551910);

INCLUDE_SDATA(const s32, "code_002BD340", sdfCdTrayMode);

INCLUDE_SDATA(const s32, "code_002BD340", D_0055191C);

INCLUDE_SDATA(const s32, "code_002BD340", sdfFileDefaultSrcType);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551921);

INCLUDE_SDATA(const s32, "code_002BD340", sdfFileSemaId);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551928);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551930);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551938);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551940);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551948);

INCLUDE_SDATA(const s32, "code_002BD340", sdfFileCmdFreeList);

INCLUDE_SDATA(const s32, "code_002BD340", sdfFileCmdCount);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551952);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551954);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551958);

INCLUDE_SDATA(const s32, "code_002BD340", sdfFileCmdFreeCount);

INCLUDE_SDATA(const s32, "code_002BD340", sdfFileCmdDefaultDevice);

INCLUDE_SDATA(const s32, "code_002BD340", sdfFileCmdThreadPrioCheckCounter);

INCLUDE_SDATA(const s32, "code_002BD340", sdfFileCmdThreadPrio);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551962);

INCLUDE_SDATA(const s32, "code_002BD340", sdfFileCmdDefaultCreateMode);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551966);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551968);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551970);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551978);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551980);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551988);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551990);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551998);

INCLUDE_SDATA(const s32, "code_002BD340", D_005519A0);

INCLUDE_SDATA(const s32, "code_002BD340", sdfDev9Ready);

INCLUDE_SDATA(const s32, "code_002BD340", D_005519A9);

INCLUDE_SDATA(const s32, "code_002BD340", sdfCdClock);

INCLUDE_SDATA(const s32, "code_002BD340", D_005519B1);

INCLUDE_SDATA(const s32, "code_002BD340", D_005519B8);

s32 sdfGetCdClock(sdfClock_t *now) {
    sdfCdClock_t clock;
    s32 result = 1;
    s32 year;
    if (sdfCdClock == 0) {
        sceCdReadClock(&clock);
        result = -1;
        if ((clock.status & 0x103) == 0) {
            D_005519B8.sec = sdfBcdToDecimal(clock.second);
            D_005519B8.min = sdfBcdToDecimal(clock.minute);
            D_005519B8.hour = sdfBcdToDecimal(clock.hour);
            D_005519B8.day = sdfBcdToDecimal(clock.day);
            D_005519B8.month = sdfBcdToDecimal(clock.month);
            /* Retail reuses the month byte as the two-digit year input. */
            year = sdfBcdToDecimal(clock.month);
            result = 0;
            D_005519B8.year = year + (year < 70 ? 2000 : 1900);
        }
        sdfCdClock = 20;
    }
    *now = D_005519B8;
    return result;
}

INCLUDE_RODATA(const s32, "code_002BD340", D_00549878);

INCLUDE_ASM(const s32, "code_002BD340", sdfSndIopAlloc);

INCLUDE_ASM(const s32, "code_002BD340", sdfSndCmdTrans);

void sdfSndCmdIFThread(void)
{
    for (;;) {
        SleepThread();
        if (sdfSndBurstReqMode == 0) {
            while (sdfSndCmdTrans() == 0) {
                ;
            }
        }
    }
}

INCLUDE_ASM(const s32, "code_002BD340", sdfSndInitCmdIF);

s32 sdfSndSendCmd(s32 command, s32 channel, void *packet, s32 payloadBytes) {
    s32 nextWriteIndex;
    s32 wakeAttemptsRemaining;
    sndRingPacket_t *entry;
    u32 transferQuadwords;
    if (++sdfSndSendCmd_cmd_id == 0) {
        sdfSndSendCmd_cmd_id = 1;
    }
    nextWriteIndex = ((s16)sdfSndCmdTailIndex + 1) & (32 - 1);
    if (nextWriteIndex == (s16)sdfSndCmdIndex) {
        for (wakeAttemptsRemaining = 8; wakeAttemptsRemaining != 0; wakeAttemptsRemaining--) {
            do {
                WakeupThread(sdfSndCmdBuf);
            } while ((s16)sdfSndCmdIndex != nextWriteIndex);
        }
        return 0;
    }
    entry = &D_00590840[(s16)sdfSndCmdTailIndex];
    entry->sequence = sdfSndSendCmd_cmd_id;
    if (payloadBytes != 0) {
        memcpy(entry->payload, packet, payloadBytes);
    }
    /* The SIF length includes the transport/command header and rounds up to quadwords. */
    transferQuadwords = (payloadBytes + 0x14 +
                        (16 - 1)) >> 4;
    entry->command = (command << 16) |
                     (channel & 0xFFFF) |
                     (transferQuadwords << 28);
    sdfSndCmdTailIndex = nextWriteIndex;
    if (sdfSndBurstReqMode != 0) {
        while (sdfSndCmdTrans() != 0) {
        }
    } else {
        WakeupThread(sdfSndCmdBuf);
    }
    return sdfSndSendCmd_cmd_id;
}

void sdfSndSyncCmd(s32 aHandle)
{
}

s32 sdfSndSendSyncCmd(s32 aCmd, s32 aArg1, void *aData, s32 aSize)
{
    s32 handle = sdfSndSendCmd(aCmd, aArg1, aData, aSize);

    sdfSndSyncCmd(handle);
    return handle;
}

INCLUDE_ASM(const s32, "code_002BD340", sdfSndTransToIOP);

INCLUDE_ASM(const s32, "code_002BD340", sdfSndLoadIRXModules);

INCLUDE_ASM(const s32, "code_002BD340", sdfSndVoiceMapInit);

INCLUDE_ASM(const s32, "code_002BD340", sdfSndSeqMapInit);

INCLUDE_ASM(const s32, "code_002BD340", sdfSndInit);

/* Starts a track with the default 0x7f setting after preparing its identifier. */
void p2sdPlayBGM(s32 trackId) {
    sdfSndTrackPacket_t packet;
    p2sdLoadSMGFile(trackId);
    packet.trackId = trackId;
    packet.unk4 = 0;
    packet.setting = 0x7F;
    sdfSndSendCmd(0x20, 0, &packet, 0x10);
}

s32 p2sdSendStopCmd(s32 aId)
{
    s32 arg = aId;

    return sdfSndSendCmd(0x30, 0, &arg, 0x10);
}

/* Starts the alternate track command, leaving the other packet fields untouched. */
void p2sdResumeBgm(s32 trackId) {
    sdfSndTrackPacket_t packet;
    p2sdLoadSMGFile(trackId);
    packet.trackId = trackId;
    packet.setting = 0x7F;
    sdfSndSendCmd(0x130, 0, &packet, 0x10);
}

s32 p2sdStopSe(s32 aId)
{
    s32 arg = aId;

    return sdfSndSendCmd(0x30, 0, &arg, 0x10);
}

s32 p2sdStopAllBgm(void)
{
    return sdfSndSendCmd(0x1A0, 0, 0, 0);
}

s32 p2sd_2C1788(void)
{
    return sdfSndSendCmd(0x210, 0, 0, 0);
}

s32 p2sdStopAllSe(void)
{
    return sdfSndSendCmd(0x40, 0, 0, 0);
}

s32 p2sd_2C17D8(s32 aSeId)
{
    return sdfSndSendCmd(aSeId | 0x50, 0, 0, 0);
}

/* Send a NUL-terminated command string; the transport excludes the terminator. */
u32 sdfSndLoadHDFile(s32 command, char *text) {
    u32 length = strlen(text);
    return sdfSndSendSyncCmd(command | 0x70, 0, text, length);
}

u32 sdfSndLoadBDFile(s32 command, char *text) {
    u32 length = strlen(text);
    return sdfSndSendSyncCmd(command | 0x60, 0, text, length);
}

/* Send text with command bit 0x80; payload length excludes its NUL terminator. */
u32 sdfSndLoadSQFile(s32 command, char *text) {
    u32 length = strlen(text);
    return sdfSndSendSyncCmd(command | 0x80, 0, text, length);
}

/* Send a 12-byte sequence command; the trailing struct word is not transmitted. */
void p2sdPlaySE(s32 trackId, s32 volume, s32 pan) {
    sndSequenceVolumePanPacket_t packet;
    p2sdLoadSMGFile(trackId);
    packet.trackId = trackId;
    packet.volume = volume;
    packet.pan = (u8)pan;
    sdfSndSendCmd(0x90, 0, &packet, 0xC);
}

INCLUDE_ASM(const s32, "code_002BD340", p2sdPlaySeDeg);

INCLUDE_ASM(const s32, "code_002BD340", func_002C1990);

/* Converts world coordinates to the sound engine's one-tenth scale. */
void p2sdPlaySe3DEx(s32 trackId, s32 radiusInHundreds, f32 worldX, f32 worldY, f32 worldZ) {
    u32 packet[8];
    packet[0] = trackId;
    packet[1] = radiusInHundreds;
    packet[2] = (s32)(worldX * 0.1f);
    packet[3] = (s32)(worldY * 0.1f);
    packet[4] = (s32)(worldZ * 0.1f);
    sdfSndSendSyncCmd(0x170, 0, packet, 0x20);
}

/* Scale six world-space values and send them to the sound engine when they changed. */
void p2sdSetStandPos(f32 firstPointX, f32 firstPointY, f32 firstPointZ,
                                 f32 secondPointX, f32 secondPointY, f32 secondPointZ) {
    sndListenerState_t state;
    state.scaledValues[0] = firstPointX * 0.1f;
    state.scaledValues[1] = firstPointY * 0.1f;
    state.scaledValues[2] = firstPointZ * 0.1f;
    state.scaledValues[3] = secondPointX * 0.1f;
    state.scaledValues[4] = secondPointY * 0.1f;
    state.scaledValues[5] = secondPointZ * 0.1f;
    if (D_005907F0.scaledValues[0] != state.scaledValues[0] || D_005907F0.scaledValues[1] != state.scaledValues[1] ||
        D_005907F0.scaledValues[2] != state.scaledValues[2] || D_005907F0.scaledValues[3] != state.scaledValues[3] ||
        D_005907F0.scaledValues[4] != state.scaledValues[4] || D_005907F0.scaledValues[5] != state.scaledValues[5]) {
        sdfSndSendSyncCmd(0x160, 0, &state, 0x20);
        D_005907F0 = state;
    }
}

INCLUDE_ASM(const s32, "code_002BD340", p2sdLoadCheckMidi);

/* Make sure the MIDI bank named by the packed track id is resident, loading it if not. */
void p2sdLoadSMGFile(s32 packed) {
    char name[0x10];
    char path[0x100];
    s32 status = p2sdLoadCheckMidi(packed);
    s32 id;
    switch (status) {
    case 0:
        id = packed >> 16;
        sprintf(name, "MIDI%04X.SMG", id);
        sdfSndMakeFilename(path, name);
        sdfSndSendSyncCmd(0xA0, 0, path, strlen(path) + 1);
        p2sdSmgHandle = id;
        break;
    case 1:
        break;
    case 2:
        do {
            sdfDelayThread(2);
            if (p2sdSmgBuf != NULL) {
                p2sdSmgBuf();
            }
        } while (p2sdLoadCheckMidi(packed) != 1);
        break;
    }
}

u64 p2sd_2C1D78(const char *aName)
{
    return sdfSndSendSyncCmd(0xA0, 0, (void *)aName, strlen(aName));
}

void p2sdSetPcmBuffer(s32 data, s32 size) {
    s32 *packet;
    D_00593E20.size = size;
    D_00593E20.pending = 1;
    D_00593E20.data = data;
    if (data != 0 && size > 0) {
        packet = &D_00552120;
        *packet = data;
        sdfSndSendSyncCmd(0x1B0, 0, packet, 4);
    }
    p2sdSmgHandle = -1;
}

u64 p2sd_2C1E10(s32 aCh)
{
    return sdfSndSendSyncCmd((aCh & 0xF) | 0x1C0, 0, NULL, 0);
}

INCLUDE_ASM(const s32, "code_002BD340", p2sd_2C1E40);

INCLUDE_ASM(const s32, "code_002BD340", func_002C1E70);

void p2sd_2C1EA0(s32 id) {
    u32 packet[4];
    if (p2sdLoadCheckMidi(id) != 0) {
        packet[0] = id;
        sdfSndSendSyncCmd(0xB0, 0, packet, 0x10);
        id >>= 16;
        if (p2sdSmgHandle == id) {
            p2sdSmgHandle = -1;
        }
    }
}

u8 p2sd_2C1EF8(s32 aIndex)
{
    return LastLoad[aIndex].unk4;
}

u8 p2sd_2C1F10(s32 aIndex)
{
    return LastLoad[aIndex].unk5;
}

s32 p2sd_2C1F28(s32 aIndex)
{
    s32 remaining = LastLoad[aIndex].unk4 - LastLoad[aIndex].unk5;

    if (remaining > 0) {
        return remaining;
    }
    return 0;
}

u8 *p2sd_2C1F50(void)
{
    return D_00593890;
}

p2sdLoadEntry_t *p2sd_2C1F60(void)
{
    return LastLoad;
}

u32 p2sd_2C1F70(u32 *aOutValue)
{
    if (aOutValue != NULL) {
        *aOutValue = sdfSndMap[0x77];
    }
    return sdfSndMap[0x76];
}

/* Pack two 24-byte MIDI state groups into out; NULL leaves the state untouched. */
void p2sd_2C1F90(u8 *out) {
    s32 group;
    if (out == NULL) {
        return;
    }
    for (group = 0; group < 2; group++) {
        s32 byteIndex;
        for (byteIndex = 0; byteIndex < 24; byteIndex++) {
            *out++ = ((sndMergedWork_t *)sdfSndMap)->stateGroups[group].values[byteIndex];
        }
    }
}

/* Sends a prepared track identifier with the 0x17f setting. */
void p2sdPlayBGMFade(s32 trackId) {
    sdfSndTrackPacket_t packet;
    p2sdLoadSMGFile(trackId);
    packet.trackId = trackId;
    packet.unk4 = 0;
    packet.setting = 0x17F;
    sdfSndSendCmd(0x20, 0, &packet, 0x10);
}

s32 p2sdFadeOutBgm(s32 aId)
{
    struct
    {
        s32 id;
        s32 unk4;
        s16 fade;
    } arg;

    arg.id = aId;
    arg.unk4 = 0;
    arg.fade = 0x17F;
    return sdfSndSendCmd(0x20, 0, &arg, 0x10);
}

s32 p2sdStopBGM(s32 aId)
{
    s32 arg = aId;

    return sdfSndSendCmd(0xD0, 0, &arg, 0x10);
}

s32 p2sdBgmVolDown(void)
{
    return sdfSndSendCmd(0x180, 0, 0, 0);
}

s32 p2sdBgmVolUp(void)
{
    return sdfSndSendCmd(0x190, 0, 0, 0);
}

void p2sdSetBgmTrack(s32 aTrack)
{
    sdfSndSendCmd(0xE0 | ((aTrack + 1) & 0xF), 0, NULL, 0);
}

s32 p2sdLoadPcmFile(const char *name, u8 channel) {
    if (p2sdPcmState != 0) {
        return 1;
    }
    strcpy(D_00529BE8, name);
    sdfSndSendSyncCmd((channel >> 3) | 0xF0, 0, 0, 0);
    return 0;
}

u32 p2sdGetPcmState(void)
{
    return p2sdPcmState;
}

s32 p2sdSyncSound(void)
{
    return sdfSndSendSyncCmd(0x100, 0, 0, 0);
}

void p2sd_2C21A8(u8 channel) {
    sdfSndSendSyncCmd(((channel >> 3) & 0xF) | 0x110, 0, 0, 0);
}

s32 p2sd_2C21D8(u32 aCh)
{
    if (aCh > 16) {
        aCh = 16;
    }
    if (aCh == 0) {
        aCh = 1;
    }
    return sdfSndSendSyncCmd((aCh - 1) | 0x1D0, 0, NULL, 0);
}

u32 p2sdSetPcmCallback(u32 aCallback)
{
    if (p2sdPcmBusy != 0) {
        return 0;
    }
    p2sdPcmCallback = aCallback;
    return aCallback;
}

/* Send opcode 0x120 with the channel's group (bits 3-6) unless a command is
 * pending; returns nonzero when busy. */
s32 p2sdPlayPcm(u8 channel) {
    if (p2sdPcmBusy) {
        return 1;
    }
    channel = (channel >> 3) & 0xF;
    sdfSndSendSyncCmd(0x120 | channel, 0, 0, 0);
    return 0;
}

u32 p2sd_2C2280(void)
{
    return p2sdPcmBusy;
}

s32 p2sdSyncMovie(void)
{
    return sdfSndSendSyncCmd(0x100, 0, 0, 0);
}

/* Separate native copy of the command-0x110 channel-group sender. */
void p2sd_2C22B0(u8 channel) {
    sdfSndSendSyncCmd(((channel >> 3) & 0xF) | 0x110, 0, 0, 0);
}

s32 sdfFileRpcCallback(s32 unused, u32 event) {
    switch (event) {
    case 1:
        break;
    case 3:
        break;
    case 6:
        break;
    case 5:
        FlushCache(0);
        /* Fall through: flush and wake the waiting thread. */
    case 4:
        SignalSema(sifRpcServerSema);
        break;
    case 0:
    case 2:
    case 7:
        SignalSema(sifRpcServerSema);
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_002BD340", sdfFileRpcHandleCmd);

INCLUDE_ASM(const s32, "code_002BD340", SifRpcServerThreadProc);

INCLUDE_ASM(const s32, "code_002BD340", sdf_2C2608);

INCLUDE_ASM(const s32, "code_002BD340", sdf_2C2618);

INCLUDE_ASM(const s32, "code_002BD340", sdf_2C27F8);

/* Print size bytes in address-aligned, sixteen-column hexadecimal rows. */
void sdfDumpHexLine(const u8 *data, s32 size) {
    s32 column = 0;
    s32 i;
    while (size > 0) {
        if (column == 0) {
            sdfPrintf(D_00551A20, data);
            column = (u32)data & 0xF;
            for (i = column; i > 0; i--) {
                sdfPrintf(D_00551A28);
            }
        } else if (column == 8) {
            sdfPrintf(D_00551A30);
        } else {
            sdfPrintf(D_00551A38);
        }
        sdfPrintf(D_00551A40, *data++);
        column++;
        if (column == 0x10) {
            column = 0;
            sdfPrintf(sdfDumpLineBuf);
        }
        size--;
    }
    if (column == 0) {
        sdfPrintf(sdfDumpLineBuf);
    } else {
        sdfPrintf(D_00551A50);
    }
}

INCLUDE_RODATA(const s32, "code_002BD340", D_00549B50);

INCLUDE_RODATA(const s32, "code_002BD340", D_00549B70);

INCLUDE_RODATA(const s32, "code_002BD340", D_00549B88);

INCLUDE_RODATA(const s32, "code_002BD340", D_00549BB0);

INCLUDE_RODATA(const s32, "code_002BD340", D_00549BD0);

void sdfDumpDmaChain(sdfDmaDumpTag_t *source) {
    sdfDmaDumpTag_t *callStack[2];
    sdfDmaDumpTag_t *cursor;
    u32 tag;
    u32 tagAddress;
    s32 tagType;
    s32 tagCount;
    s32 depth = 0;
    s32 done = 0;
    sdfPrintf(D_00549B50, source);
    cursor = source;
    for (;;) {
        if ((u32)cursor & 0xF) {
            sdfPrintf(D_00549B70, cursor);
            return;
        }
        tag = cursor->control;
        tagAddress = cursor->address;
        tagType = (tag >> 28) & 7;
        tagCount = tag & 0xFFFF;
        sdfPrintf(D_00549B88, cursor, sdfDmaTagNameTable[tagType], tag, tagAddress, cursor->vifCode0, cursor->vifCode1);
        cursor++;
        switch (tagType) {
        case 0:
            done = 1;
            break;
        case 1:
            cursor += tagCount;
            break;
        case 2:
            cursor = (sdfDmaDumpTag_t *)tagAddress;
            break;
        case 3:
        case 4:
            break;
        case 5:
            if (depth == 2) {
                sdfPrintf(D_00549BB0);
                return;
            }
            callStack[depth++] = cursor;
            cursor = (sdfDmaDumpTag_t *)tagAddress;
            break;
        case 6:
            if (depth == 0) {
                done = 1;
            } else {
                cursor = callStack[--depth];
            }
            break;
        case 7:
            cursor += tagCount;
            done = 1;
            break;
        }
        if (done != 0) {
            sdfPrintf(D_00549BD0, cursor);
            return;
        }
    }
}

INCLUDE_ASM(const s32, "code_002BD340", sdfGrFlushOtChain);

void sdfDumpGsMemory(void) {
    char buf[8];
    sdfGsMemSlice_t *head;
    sdfGsMemSlice_t *node;
    sdfGsMemSlice_t *walk;
    char *name;
    sdfPrintf(D_0054A088);
    head = sdfGsMemGetLinkHead();
    node = head;
    while (node != 0) {
        if ((u32)node->type < SDF_TEX_RESOURCE_KNOWN_MODE_COUNT) {
            name = D_00529CC8[node->type];
        } else {
            sdfPrintf(buf, D_00551B40, node->type);
            name = buf;
        }
        sdfPrintf(D_0054A100, node, node->prev, node->next, name, node->addr, node->size);
        walk = head;
        while (walk != node) {
            walk = walk->next;
        }
        node = node->next;
    }
}

void sdfDumpGsMemory2(void) {
    char buf[8];
    sdfGsMemSlice_t *head;
    sdfGsMemSlice_t *node;
    sdfGsMemSlice_t *walk;
    char *name;
    sdfPrintf(D_0054A088);
    head = sdfGsMemGetLinkTail();
    node = head;
    while (node != 0) {
        if ((u32)node->type < SDF_TEX_RESOURCE_KNOWN_MODE_COUNT) {
            name = D_00529CD8[node->type];
        } else {
            sdfPrintf(buf, D_00551B40, node->type);
            name = buf;
        }
        sdfPrintf(D_0054A100, node, node->prev, node->next, name, node->addr, node->size);
        walk = head;
        while (walk != node) {
            walk = walk->prev;
        }
        node = node->prev;
    }
}

INCLUDE_RODATA(const s32, "code_002BD340", D_0054A088);

INCLUDE_RODATA(const s32, "code_002BD340", D_0054A100);

void sdfDumpMemory(void) {
    sdfMemInfo_t info;
    sdfMemGetInfo(&info);
    sdfPrintf(" <<< memory information >>>\n             total : 0x%06X\n        free total : 0x%06X\n     max free size : 0x%06X\n     min free size : 0x%06X\n      handle total : %d\n free handle count : %d\n\n",
                    info.total, info.freeSize,
                    info.maxFreeSize, info.minFreeSize,
                    info.handleCount, info.freeHandleCount);
}

/* Print the chip heap totals and how many cells are in use per size class (1..16, 17..32, ...). */
void sdfDumpChipMemory(void) {
    sdfChipInfo_t stats;
    u32 limit = 16;
    s32 i = 0;
    u32 first;
    u32 *used;
    sdfChipGetInfo(&stats);
    sdfPrintf(" <<< chip memory information >>>\n                 total : 0x%06X\n            free total : 0x%06X\n            page count : %d\n       free page count : %d\n fragmented page count : %d\n", stats.total, stats.freeSize, stats.pageCount, stats.freePageCount, stats.fragmentedPageCount);
    sdfPrintf("\n several size alloc count...\n");
    first = 0;
    used = stats.severalAllocCount;
    do {
        sdfPrintf("       for %3d .. %4d : %d\n", first, limit, *used++);
        i++;
        first = limit + 1;
        limit *= 2;
    } while (i != 7);
    sdfPrintf(sdfDumpLineBuf, first);
}

/* Read a named file through the dev RPC into a freshly allocated block; returns the block's handle.
 * outData receives the block address, outSize the file size; without outData the block is released. */
void *sdfDevLoadFile(const char *name, u32 *outData, u32 *outSize, s32 extra) {
    sdfFileCmdHandle_t *state = sdfFileOpen(name);
    s32 size = sdfFileLength(state);
    sdfMemHandle_t *handle = sdfMemAllocDirect(size + extra);
    u32 address = sdfMemLock(handle);
    sdfFileRead(state, (void *)address, size);
    sdfFileClose(state);
    if (outData != NULL) {
        *outData = address;
    } else {
        sdfMemUnlock(handle);
    }
    if (outSize != NULL) {
        *outSize = size;
    }
    return handle;
}

void *sdfDevLoadFileEx(const char *aFileName, u32 *aOutData, u32 *aOutSize)
{
    return sdfDevLoadFile(aFileName, aOutData, aOutSize, 0);
}

sdfTexHandle_t *sdfDevLoadTex(const char *filename) {
    u32 dataAddress;
    sdfMemHandle_t *allocation;
    sdfTexHandle_t *texture;

    printf(D_0054A2D8, filename);
    allocation = sdfDevLoadFileEx(filename, &dataAddress, 0);
    texture = sdfTexSet((void *)dataAddress);
    sdfMemFreeDirect(allocation);
    return texture;
}

sdfItemArray_t *sdfTexSetTB(const sdfTextureOffsetListHeader_t *resource) {
    s32 i = 0;
    s32 count = resource->textureCount;
    sdfItemArray_t *handle = sdfTexListNew(count);
    const sdfTextureOffsetListEntry_t *entry;
    if (count != i) {
        entry = (const sdfTextureOffsetListEntry_t *)((const u8 *)resource + sizeof(*resource));
        do {
            i++;
            sdfTexListAdd(handle, sdfTexSet((TMX_HEADER_t *)(
                (void *)((const u8 *)resource + entry->textureHeaderOffset))));
            entry++;
        } while (i != count);
    }
    return handle;
}

sdfItemArray_t *sdfDevLoadTexList(const char *filename) {
    u32 dataAddress;
    sdfMemHandle_t *allocation;
    sdfItemArray_t *texture;

    printf(D_0054A2D8, filename);
    allocation = sdfDevLoadFileEx(filename, &dataAddress, 0);
    texture = sdfTexSetTB((void *)dataAddress);
    sdfMemFreeDirect(allocation);
    return texture;
}

void *sdfFixupModelBinary(sdfBinaryHeader_t *aHeader)
{
    void *body = aHeader + 1;

    sdfAddressResolver(body, body, (u8 *)body + aHeader->resolveOffset, aHeader->resolveCount);
    return body;
}

INCLUDE_ASM(const s32, "code_002BD340", sdfDevLoadModel);

void *sdfFixupMotionBinary(sdfBinaryHeader_t *aHeader)
{
    void *body = aHeader + 1;

    sdfAddressResolver(body, body, (u8 *)body + aHeader->resolveOffset, aHeader->resolveCount);
    return body;
}

INCLUDE_ASM(const s32, "code_002BD340", sdfDevLoadMotion);

/* Byte-coded commands advance by word deltas, or relocate a run of following words.
 * Low tag bits select one-, two-, or three-byte deltas; tag 7 encodes a run.
 * tableBytes bounds the encoded byte stream, not the number of relocated words.
 */
void sdfAddressResolver(void *base, void *target, void *encodedTable, s32 tableBytes) {
    s32 *words = base;
    s32 relocationBase = (s32)target;
    const u8 *table = encodedTable;
    const u8 *commandCursor = table;
    s32 value;
    s32 runIndex;
    while (commandCursor - table < tableBytes) {
        value = *commandCursor++;
        if ((value & 1) == 0) {
            value >>= 1;
        } else if ((value & 2) == 0) {
            value = (value | *commandCursor++ << 8) >> 2;
        } else if ((value & 4) == 0) {
            value = (value | commandCursor[0] << 8 | commandCursor[1] << 16) >> 3;
            commandCursor += 2;
        } else {
            value = (value >> 3) + 2;
            for (runIndex = 0; runIndex < value; runIndex++) {
                words++;
                *words += relocationBase;
            }
            continue;
        }
        words += value;
        *words += relocationBase;
    }
}

/* Remove a queued node without clearing its old links; the caller may own the interrupt guard. */
void sdfIpuDetachQueue(sdfIpuDecodeCb_t *node, s32 skipInterruptGuard) {
    s32 interruptsEnabled = 0;
    sdfIpuDecodeCb_t *previousNode;
    sdfIpuDecodeCb_t *nextNode;
    if (skipInterruptGuard == 0) {
        interruptsEnabled = DIntr();
    }
    if (node->queued != 0) {
        previousNode = node->streamPrev;
        nextNode = node->streamNext;
        if (previousNode == 0) {
            sdfIpuQueueHead = nextNode;
        } else {
            previousNode->streamNext = nextNode;
        }
        if (nextNode == 0) {
            sdfIpuQueueTail = previousNode;
        } else {
            nextNode->streamPrev = previousNode;
        }
        node->queued = 0;
    }
    if (skipInterruptGuard == 0 && interruptsEnabled != 0) {
        EIntr();
    }
}

/* Move a node to the queue tail, using one interrupt guard for unlink plus append. */
void sdfIpuAttachQueue(sdfIpuDecodeCb_t *node, s32 skipInterruptGuard) {
    s32 interruptsEnabled = 0;
    if (skipInterruptGuard == 0) {
        interruptsEnabled = DIntr();
    }
    if (node->queued != 0) {
        sdfIpuDetachQueue(node, 1);
    }
    node->queued = 1;
    if (sdfIpuQueueTail == 0) {
        sdfIpuQueueHead = node;
    } else {
        sdfIpuQueueTail->streamNext = node;
    }
    node->streamPrev = sdfIpuQueueTail;
    node->streamNext = 0;
    sdfIpuQueueTail = node;
    if (skipInterruptGuard == 0 && interruptsEnabled != 0) {
        EIntr();
    }
}

void sdfIpuAttachTask(sdfIpuDecodeCb_t *aNode)
{
    sdfIpuDecodeCb_t **link = &sdfIpuTaskLink;
    sdfIpuDecodeCb_t *cur;

    for (cur = *link; cur != NULL; cur = *link) {
        link = &cur->next;
    }
    *link = aNode;
    aNode->next = NULL;
}

void sdfIpuDetachTask(sdfIpuDecodeCb_t *aNode)
{
    sdfIpuDecodeCb_t **link = &sdfIpuTaskLink;
    sdfIpuDecodeCb_t *cur;

    for (cur = *link; cur != NULL; cur = *link) {
        if (cur == aNode) {
            *link = cur->next;
            return;
        }
        link = &cur->next;
    }
}

/* Allocate two width-by-height frames, using four bytes per pixel for mode zero, two otherwise. */
void sdfIpuAllocBuff(sdfIpuDecodeCb_t *node) {
    s32 bytesPerPixel = 4;
    s32 size;
    if (node->audioMode != SDF_STREAM_OUTPUT_RGBA32) {
        bytesPerPixel = 2;
    }
    size = node->width * node->height;
    size *= bytesPerPixel;
    node->bufferSize = size;
    node->frameBuffers[0] = sdfAllocAuto(size);
    node->frameBuffers[1] = sdfAllocAuto(size);
}

/* Each 16-byte DMA tag holds control/address bits and a zero reserved half. */
void sdfIpuMakeToIpuDmaTag(sdfIpuDecodeCb_t *node, u8 *source, s32 bytes) {
    s32 remaining = (bytes + 15) & ~15;
    struct SdfStreamInputDmaTag *tag;
    u32 address;
    tag = sdfAllocAuto(((remaining + 0xFFEFF) / 0xFFF00) * 16);
    node->inputDmaChain = tag;
    address = (u32)source & 0x0FFFFFFF;
    do {
        s32 chunk = remaining > 0xFFF00 ? 0xFFF00 : remaining;
        u32 id;
        remaining -= chunk;
        id = remaining != 0 ? 3 : 0;
        tag->control = ((u16)(chunk >> 4) | (id << 28)) | ((u64)address << 32);
        tag->reserved = 0;
        address += chunk;
        tag++;
    } while (remaining > 0);
    FlushCache(0);
}

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuMakeBltPacket);

/* Clear the decoder, choose 32-bit/16-bit/dithered output and copy playback options. */
void sdfIpuInitCb(sdfIpuDecodeCb_t *node, struct SoundFormat *format) {
    memset(node, 0, 0x8C);
    if (format->output16Bit == 0) {
        node->audioMode = SDF_STREAM_OUTPUT_RGBA32;
    } else if (format->dither == 0) {
        node->audioMode = SDF_STREAM_OUTPUT_RGBA16;
    } else {
        node->audioMode = SDF_STREAM_OUTPUT_RGBA16_DITHERED;
    }
    node->loopMode = format->loopMode;
    node->playbackCadenceStep = format->playbackCadenceStep;
}

/* Decode the frame header, allocate its buffers, then queue the bytes after that header. */
void sdfIpuSetDecodeParams(sdfIpuDecodeCb_t *node, struct SoundFormat *format, SdfStreamRead sourceAddress, struct movieStream_t *sourceContext) {
    s32 sourceSize = (s32)sourceContext;
    s32 interruptsEnabled;
    u8 *frameBytes = (u8 *)sourceAddress;
    sdfIpuInitCb(node, format);
    node->width = ((sdfStreamFrameHeader_t *)frameBytes)->width;
    node->cycleLength = ((sdfStreamFrameHeader_t *)frameBytes)->cycleLength;
    node->height = ((sdfStreamFrameHeader_t *)frameBytes)->height;
    sdfIpuAllocBuff(node);
    sdfIpuMakeToIpuDmaTag(node, frameBytes + 0x10, sourceSize - 0x10);
    interruptsEnabled = DIntr();
    sdfIpuAttachQueue(node, 0);
    if (interruptsEnabled != 0) {
        EIntr();
    }
    sdfIpuExecQueue(0);
}

/* Store the callback and movie context, then allocate the feed ring and leading mirror bytes. */
void sdfIpuStrmDecodeInit(sdfIpuDecodeCb_t *node, struct SoundFormat *format, SdfStreamRead readSource, struct movieStream_t *readContext) {
    sdfIpuInitCb(node, format);
    node->read = readSource;
    node->readContext = readContext;
    node->active = 1;
    node->scratchBuffer = (u8 *)sdfAllocAuto(0x10100) + 0x100;
}

/* Query for a complete header before reading it. Texture dimensions are rounded
 * only when larger than one GS page; smaller dimensions remain unchanged.
 */
void sdfIpuAllocGsRect(sdfIpuDecodeCb_t *node) {
    sdfStreamFrameHeader_t header;
    s32 readStatus;
    s32 interruptsEnabled;
    if (node->headerReady != 0) {
        return;
    }
    if (node->read(node, node->readContext, SDF_STREAM_READ_QUERY, &readStatus, 0) < sizeof(header)) {
        return;
    }
    node->headerReady = 1;
    node->read(node, node->readContext, SDF_STREAM_READ_COPY, &header, sizeof(header));
    node->width = header.width;
    node->height = header.height;
    node->cycleLength = header.cycleLength;
    if (node->resourceWord == 0) {
        s32 textureWidth = node->width;
        s32 textureHeight = node->height;
        s32 pixelFormat;
        sdfGsMemSlice_t *texture;
        if (textureWidth > 64) {
            textureWidth = (textureWidth + 63) & ~63;
        }
        if (node->audioMode == SDF_STREAM_OUTPUT_RGBA32) {
            pixelFormat = 0;
            if (textureHeight > 32) {
                textureHeight = (textureHeight + 31) & ~31;
            }
        } else {
            pixelFormat = 2;
            if (textureHeight > 64) {
                textureHeight = (textureHeight + 63) & ~63;
            }
        }
        texture = sdfGsMemAllocRect(textureWidth, textureHeight, pixelFormat, SDF_TEX_RESOURCE_TEXTURE, 0);
        node->textureHead = texture;
        node->resourceWord = texture->addr;
    }
    sdfIpuAllocBuff(node);
    interruptsEnabled = DIntr();
    sdfIpuAttachQueue(node, 0);
    if (interruptsEnabled != 0) {
        EIntr();
    }
}

void sdfIpuFree(sdfIpuDecodeCb_t *node) {
    s32 interruptsEnabled;
    s32 wasActive;
    u32 dmaEnable;
    interruptsEnabled = DIntr();
    wasActive = 0;
    node->drained = 1;
    sdfIpuDetachTask(node);
    sdfIpuDetachQueue(node, 1);
    if (sdfCurrIpuDecodeCb == node) {
        SDF_IPU_CTRL = 0x40000000;
        sceIpuSync(0, 0);
        dmaEnable = SDF_DMAC_ENABLE;
        SDF_DMAC_ENABLE = 0x10000;
        SDF_TO_IPU_CHCR = 1;
        SDF_FROM_IPU_CHCR = 0;
        SDF_DMAC_ENABLE = dmaEnable;
        sdfCurrIpuDecodeCb = NULL;
        wasActive = 1;
    }
    if (interruptsEnabled != 0) {
        EIntr();
    }
    sdfFreeAuto(node->inputDmaChain);
    sdfFreeAuto(node->frameBuffers[0]);
    sdfFreeAuto(node->frameBuffers[1]);
    sdfImageSetChainReleaseMem((s32)node->transferPacketBuffers[0]);
    sdfImageSetChainReleaseMem((s32)node->transferPacketBuffers[1]);
    if (node->scratchBuffer != 0) {
        sdfFreeAuto(node->scratchBuffer - 0x100);
    }
    if (node->textureHead != 0) {
        sdfGsMemFreeDelayed(node->textureHead);
    }
    if (wasActive != 0) {
        sdfIpuExecQueue(0);
    }
}

/* Fill at most seven of eight 8-KiB slots. A short non-EOF query waits for more
 * data; EOF permits a partial final read. Copy the final slot's trailing bytes
 * into the prefix mirror, retaining the original copy even for a partial read.
 */
void sdfIpuHandleDmaEnd(sdfIpuDecodeCb_t *feed) {
    s32 writeSlot;
    s32 filledSlots;
    s32 availableBytes;
    u32 destinationAddress;
    s32 endOfStream;
    filledSlots = feed->filledSlots;
    if (feed->done == 0 && filledSlots < 7) {
        do {
            writeSlot = feed->firstSlot;
            endOfStream = 0;
            destinationAddress = (u32)feed->scratchBuffer;
            writeSlot += filledSlots;
            if (writeSlot >= 8) {
                writeSlot -= 8;
            }
            destinationAddress += writeSlot << 13;
            destinationAddress = (destinationAddress & 0x0FFFFFFF) | 0x20000000;
            availableBytes = feed->read(feed, feed->readContext, SDF_STREAM_READ_QUERY, &endOfStream, 0);
            if (availableBytes < 0x2000) {
                if (endOfStream == 0) {
                    return;
                }
                if (availableBytes > 0) {
                    feed->read(feed, feed->readContext, SDF_STREAM_READ_COPY, (void *)destinationAddress, availableBytes);
                    filledSlots++;
                }
                feed->done = 1;
            } else {
                if (availableBytes == 0x2000) {
                    if (endOfStream != 0) {
                        feed->done = 1;
                    }
                }
                feed->read(feed, feed->readContext, SDF_STREAM_READ_COPY, (void *)destinationAddress, 0x2000);
                filledSlots++;
            }
            if (writeSlot == 7) {
                memcpy((void *)(((u32)(feed->scratchBuffer - 0x100) & 0x0FFFFFFF) | 0x20000000), (void *)(destinationAddress + 0x1F00), 0x100);
            }
            feed->filledSlots = filledSlots;
            if (feed->done != 0) {
                break;
            }
        } while (filledSlots < 7);
    }
}

/* Start IPU input from the initial chain or a feed slot; defer an empty ring. */
void sdfIpuSetToIpuDma(sdfIpuDecodeCb_t *stream) {
    if (stream->active == 0) {
        D_00552158 = stream;
        SDF_TO_IPU_TADR = (u32)stream->inputDmaChain & 0x0FFFFFFF;
        SDF_TO_IPU_QWC = 0;
        SDF_TO_IPU_CHCR = 0x105;
    } else {
        if (stream->filledSlots == 0) {
            stream->inputDmaStartPending = 1;
            return;
        }
        D_00552158 = stream;
        stream->inputFeedDmaInFlight = 1;
        SDF_TO_IPU_MADR = ((u32)stream->scratchBuffer + (stream->firstSlot << 13)) & 0x0FFFFFFF;
        SDF_TO_IPU_QWC = 0x2000 / 16;
        SDF_TO_IPU_CHCR = 0x101;
    }
    stream->inputDmaStartPending = 0;
}

/* Start DMA from the IPU into the selected buffer, then toggle the byte index. */
void sdfIpuSetFromIpuDma(sdfIpuDecodeCb_t *stream) {
    u8 bufferIndex = stream->bufferIndex;
    SDF_FROM_IPU_MADR = (u32)stream->frameBuffers[bufferIndex] & 0x0FFFFFFF;
    SDF_FROM_IPU_QWC = stream->bufferSize / 16;
    SDF_FROM_IPU_CHCR = 0x100;
    stream->bufferIndex = bufferIndex ^ 1;
}

/* Read the FDEC result, then issue FDEC with an eight-bit advance; return the earlier result. */
s32 IpuWriteCommand(void) {
    s32 decodedBits;
    SDF_IPU_CMD = 0x40000000;
    sceIpuSync(0, 0);
    decodedBits = SDF_IPU_CMD;
    sceIpuSync(0, 0);
    SDF_IPU_CMD = 0x40000008;
    sceIpuSync(0, 0);
    return decodedBits;
}

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuDecode);

/* Remove and advance the first list item matching the IPU cleanup test. */
void sdfIpuExecQueue(s32 skipInterruptGuard) {
    sdfIpuDecodeCb_t *node;
    s32 interruptsEnabled = 0;
    if (sdfCurrIpuDecodeCb != NULL) {
        return;
    }
    if (skipInterruptGuard == 0) {
        interruptsEnabled = DIntr();
    }
    node = sdfIpuQueueHead;
    while (node != 0) {
        if (node->completedBufferCount + node->pendingPlaybackSubmissions < 2 &&
            (node->active != 1 || node->filledSlots != 0)) {
            sdfIpuDetachQueue(node, 1);
            sdfIpuDecode(node);
            break;
        }
        node = node->streamNext;
    }
    if (skipInterruptGuard == 0 && interruptsEnabled != 0) {
        EIntr();
    }
}

/* Sleep for IPU completion, advance cycle state and requeue non-drained nodes;
 * the DMA worker runs indefinitely and retains the native completion-byte gates. */
void sdfIpuDecodeThread(void) {
    sdfIpuDecodeCb_t *work;
    s32 interruptsEnabled;
    for (;;) {
        SleepThread();
        work = sdfCurrIpuDecodeCb;
        if (work == NULL) {
            continue;
        }
        sceIpuStopDMA(&work->dma);
        if (work->dma.inputQwords == 0) {
            work->inputFeedEnabled = 0;
        }
        if (work->playbackPhase == SDF_STREAM_PLAYBACK_INITIAL) {
            work->playbackPhase = SDF_STREAM_PLAYBACK_FIRST_COMPLETION;
        }
        work->completedBufferCount++;
        sdfCurrIpuDecodeCb = NULL;
        work->tickCount++;
        if (work->tickCount == work->cycleLength) {
            if (work->loopMode != 0) {
                work->ipuCycleInitialized = 0;
                work->tickCount = 0;
                work->unk65 = 0;
            } else {
                work->drained = 1;
            }
        }
        if (work->drained == 0) {
            interruptsEnabled = DIntr();
            sdfIpuAttachQueue(work, 0);
            if (interruptsEnabled != 0) {
                EIntr();
            }
        }
        sdfIpuExecQueue(0);
    }
}

s32 sdfIpuFromDmacHandler(void) {
    iWakeupThread(sdfIpuDecodeThId);
    EE_SYNC_EI();
    return 0;
}

s32 sdfIpuToDmacHandler(void) {
    sdfIpuDecodeCb_t *stream = D_00552158;
    if (stream != NULL) {
        D_00552144 = stream->playbackFrameIndex;
        D_00552158 = NULL;
        if (stream->inputFeedEnabled != 0 && stream->active == 1) {
            if (stream->inputFeedDmaInFlight != 0) {
                stream->inputFeedDmaInFlight = 0;
                stream->firstSlot++;
                if (stream->firstSlot == 8) {
                    stream->firstSlot = 0;
                }
                stream->filledSlots--;
            }
            sdfIpuHandleDmaEnd(stream);
            sdfIpuSetToIpuDma(stream);
        }
    }
    EE_SYNC_EI();
    return 0;
}

void *sdfIpuGetActivePacket(sdfIpuDecodeCb_t *aNode)
{
    void *packet = NULL;

    if (aNode->completedBufferCount != 0) {
        packet = aNode->frameBuffers[aNode->transferPacketIndex];
    }
    return packet;
}

void *sdfIpuGetActiveFrame(sdfIpuDecodeCb_t *aNode)
{
    void *frame = NULL;

    if (aNode->completedBufferCount != 0) {
        frame = aNode->transferPacketBuffers[aNode->transferPacketIndex];
    }
    return frame;
}

/* Consume a pending buffer when present, but always advance processed; loop only on exact equality. */
void sdfIpuFinishActiveFrame(sdfIpuDecodeCb_t *node) {
    s32 interruptsEnabled = DIntr();
    s32 pendingBuffers = node->completedBufferCount;
    s32 remainingBuffers = pendingBuffers - 1;
    if (pendingBuffers > 0) {
        node->completedBufferCount = remainingBuffers;
        node->transferPacketIndex ^= 1;
        node->pendingPlaybackSubmissions++;
    }
    node->playbackFrameIndex++;
    if (node->playbackFrameIndex == node->cycleLength && node->loopMode != 0) {
        node->playbackFrameIndex = 0;
    }
    if (interruptsEnabled != 0) {
        EIntr();
    }
    sdfIpuExecQueue(0);
}

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuSetChainTag);

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuRunDecode);

void sdfIpuStartDecodeTask(sdfIpuDecodeCb_t *aNode, struct SoundFormat *aFormat, SdfStreamRead aRead, struct movieStream_t *aContext, s32 aResourceWord)
{
    sdfIpuSetDecodeParams(aNode, aFormat, aRead, aContext);
    aNode->resourceWord = aResourceWord;
    sdfIpuAttachTask(aNode);
}

void sdfIpuSetOutputTex(sdfIpuDecodeCb_t *node, struct SoundFormat *params, u8 *sourceData, s32 sourceSize, sdfTexHandle_t *source) {
    struct SoundFormat streamParams = *params;
    switch (source->pixelFormat) {
    case 0:
        streamParams.output16Bit = 0;
        break;
    case 2:
        streamParams.output16Bit = 1;
        break;
    }
    sdfIpuStartDecodeTask(node, &streamParams, (SdfStreamRead)sourceData, (struct movieStream_t *)sourceSize, sdfTexGetPixelAddr(source));
}

void movieAttachDecodeTask(sdfIpuDecodeCb_t *aNode, struct SoundFormat *aFormat, SdfStreamRead aRead, struct movieStream_t *aContext, s32 aResourceWord)
{
    sdfIpuStrmDecodeInit(aNode, aFormat, aRead, aContext);
    aNode->resourceWord = aResourceWord;
    sdfIpuAttachTask(aNode);
}

void sdfIpuInit(void) {
    s32 thread;
    sceIpuInit();
    SDF_IPU_CMD = 0x90000000;
    sdfIpuTaskLink = 0;
    sdfCurrIpuDecodeCb = NULL;
    sdfFromIpuDmacHandlerHandle = DmacAddHandler(1, 3, sdfIpuFromDmacHandler, -1, 0);
    EnableDmac(3);
    sdfToIpuDmacHandlerHandle = DmacAddHandler(1, 4, sdfIpuToDmacHandler, -1, 0);
    EnableDmac(4);
    thread = CreateThreadSimple((void *)sdfIpuDecodeThread, sdfIpuDecodeThStack, 0x800, 0x46);
    sdfIpuDecodeThId = thread;
    StartThread(thread, 0);
}

s32 sdfIpuClose(void) {
    if (sdfFromIpuDmacHandlerHandle != 0) {
        sdfIpuRemoveHandler((void *)sdfFromIpuDmacHandlerHandle);
        sdfFromIpuDmacHandlerHandle = 0;
    }
    DisableDmac(3);
    if (sdfToIpuDmacHandlerHandle != 0) {
        sdfIpuRemoveHandler((void *)sdfToIpuDmacHandlerHandle);
        sdfToIpuDmacHandlerHandle = 0;
    }
    return DisableDmac(4);
}

INCLUDE_RODATA(const s32, "code_002BD340", D_0054A2D8);

INCLUDE_SDATA(const s32, "code_002BD340", p2sdSmgHandle);

INCLUDE_SDATA(const s32, "code_002BD340", p2sdPcmCallback);

INCLUDE_SDATA(const s32, "code_002BD340", p2sdPcmState);

INCLUDE_SDATA(const s32, "code_002BD340", p2sdPcmBusy);

INCLUDE_SDATA(const s32, "code_002BD340", sdfSndSendCmd_cmd_id);

INCLUDE_SDATA(const s32, "code_002BD340", D_005519D4);

INCLUDE_SDATA(const s32, "code_002BD340", p2sdSmgBuf);

INCLUDE_SDATA(const s32, "code_002BD340", D_005519DC);

INCLUDE_SDATA(const s32, "code_002BD340", D_005519E0);

INCLUDE_SDATA(const s32, "code_002BD340", D_005519E8);

INCLUDE_SDATA(const s32, "code_002BD340", D_005519F0);

INCLUDE_SDATA(const s32, "code_002BD340", D_005519F8);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551A00);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551A08);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551A10);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551A18);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551A20);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551A28);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551A30);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551A38);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551A40);

INCLUDE_SDATA(const s32, "code_002BD340", sdfDumpLineBuf);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551A4A);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551A50);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551A58);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551A60);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551A68);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551A70);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551A78);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551A80);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551A88);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551A90);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551A98);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551AA0);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551AA8);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551AB0);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551AB8);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551AC0);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551AC8);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551AD0);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551AD8);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551AE0);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551AE8);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551AF0);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551AF8);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551B00);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551B08);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551B10);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551B18);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551B20);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551B28);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551B30);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551B38);

INCLUDE_SDATA(const s32, "code_002BD340", D_00551B40);

