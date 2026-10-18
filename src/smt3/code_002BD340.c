#include "common.h"
#include "libsdf.h"

typedef struct
{
    u32 lsn;
    u32 unk4;
    s32 size;
} sdfCdImageEntry_t;

typedef struct
{
    u8 unk00[0x16];
    s8 stat;
    u8 unk17[0x11];
    s32 fd;
    s32 error;
} sdfFileCmd_t;


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

void FatalError(const char *aMessage) __attribute__((noreturn));
void sdfAddressResolver(void *aBase, void *aTarget, void *aTable, s32 aCount);
s32 sdfFileCmdUnlink(void *aCmd);
s32 sdfFileCmdDequeue(void *aCmd);
s32 sceClose(s32 aFd);
char *strcat(char *aDest, const char *aSrc);

INCLUDE_ASM(const s32, "code_002BD340", sdfPrintRpcFunc);

INCLUDE_ASM(const s32, "code_002BD340", sdfPrintRpcThread);

INCLUDE_ASM(const s32, "code_002BD340", sdf_2BD3E8);

INCLUDE_ASM(const s32, "code_002BD340", sdfPrintIopInit);

INCLUDE_ASM(const s32, "code_002BD340", sdfPuts);

INCLUDE_ASM(const s32, "code_002BD340", sdfPrintf);

void *sdfCdImageGetDirData(void)
{
    return sdfCdImageDirData;
}

INCLUDE_ASM(const s32, "code_002BD340", sdfCdWaitTrayClose);

INCLUDE_ASM(const s32, "code_002BD340", sdfCdStStart);

INCLUDE_ASM(const s32, "code_002BD340", sdfCdStRead);

INCLUDE_ASM(const s32, "code_002BD340", sdfCdImageSearchFileDirect);

INCLUDE_ASM(const s32, "code_002BD340", sdfCdImageSearchFile);

INCLUDE_ASM(const s32, "code_002BD340", sdfCdImageLoadStart);

INCLUDE_ASM(const s32, "code_002BD340", sdfCdImageRead);

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

INCLUDE_ASM(const s32, "code_002BD340", sdfCdImageLoadDir);

INCLUDE_ASM(const s32, "code_002BD340", sdfCdImageInit);

s32 sdfCdInit(void)
{
    sdfCdImageFd = sdfCreateSema(1, 0xFF, 0);
    sceCdInit(0);
    return sceCdMmode(sdfCdTrayMode);
}

INCLUDE_ASM(const s32, "code_002BD340", sdfFileSetCdFilename);

INCLUDE_ASM(const s32, "code_002BD340", sdfFileSetHostFilename);

INCLUDE_ASM(const s32, "code_002BD340", sdfFileSetHddFilename);

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

INCLUDE_ASM(const s32, "code_002BD340", sdfFileMakeFilename);

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

INCLUDE_ASM(const s32, "code_002BD340", sdfFileOpen);

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCheckExist);

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

INCLUDE_ASM(const s32, "code_002BD340", sdfFileSetHostRoot);

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

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCmdAllocHandle);

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

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCmdFinish);

INCLUDE_RODATA(const s32, "code_002BD340", D_00549640);

INCLUDE_RODATA(const s32, "code_002BD340", D_00549658);

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCmdThread);

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCdImageCmdThread);

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCmdMakeRealFileName);

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCmdAnalyzeFileName);

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCmdNewHandle);

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCmdReadOpen);

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCmdWriteOpen);

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCmdSeek);

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCmdFileSize);

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCmdRead);

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCmdWrite);

s32 sdfFileCmdClearError(sdfFileCmd_t *aCmd)
{
    if (aCmd->stat != 9) {
        return -1;
    }
    aCmd->error = 0;
    aCmd->stat = 7;
    return 0;
}

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCmdClose);

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCmdFree);

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCmdReadFile);

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCmdWriteFile);

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCmdThreadSetPrio);

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

INCLUDE_ASM(const s32, "code_002BD340", sdfFileCmdStartThread);

INCLUDE_ASM(const s32, "code_002BD340", sdfHddPowerOffTh);

void sdfHddPowerOffHandler(s32 aSemaId)
{
    iSignalSema(aSemaId);
}

INCLUDE_ASM(const s32, "code_002BD340", sdfHddInit);

INCLUDE_ASM(const s32, "code_002BD340", sdfDev9LoadModule);

INCLUDE_ASM(const s32, "code_002BD340", sdf_2BFC58);

INCLUDE_ASM(const s32, "code_002BD340", sdfZeroMemQuad);

INCLUDE_ASM(const s32, "code_002BD340", sdfStrdupChip);

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

INCLUDE_ASM(const s32, "code_002BD340", sdf_2BFE08);

INCLUDE_ASM(const s32, "code_002BD340", sdfItemArrayNew);

void sdfItemArrayFree(sdfItemArray_t *aArray)
{
    sdfMemFreeDirect(aArray->memHandle);
    sdfChipFreeDirect(aArray);
}

INCLUDE_ASM(const s32, "code_002BD340", sdfItemArrayGrow);

INCLUDE_ASM(const s32, "code_002BD340", sdfSin);

f32 sdfCos(f32 aRad)
{
    return sdfSin(aRad + SDF_PI_2);
}

INCLUDE_ASM(const s32, "code_002BD340", sdfTableInterpolation);

f32 sdfAtan(f32 aX)
{
    f32 x2 = aX * aX;
    f32 x3 = x2 * aX;
    f32 x5 = x2 * x3;

    return aX * 0.9999998f + x3 * -0.33325735f + x5 * 0.19388643f;
}

INCLUDE_ASM(const s32, "code_002BD340", sdfAtan2);

INCLUDE_ASM(const s32, "code_002BD340", sdfAsin);

INCLUDE_ASM(const s32, "code_002BD340", sdfAcos);

INCLUDE_ASM(const s32, "code_002BD340", sdfClipAngleValuePI);

INCLUDE_ASM(const s32, "code_002BD340", Mult);

INCLUDE_ASM(const s32, "code_002BD340", sdf_2C04B8);

INCLUDE_ASM(const s32, "code_002BD340", SDF_QUAT_INNERPRODUCT);

INCLUDE_ASM(const s32, "code_002BD340", sdf_2C0510);

INCLUDE_ASM(const s32, "code_002BD340", sdf_2C0538);

INCLUDE_ASM(const s32, "code_002BD340", FromRotation);

INCLUDE_ASM(const s32, "code_002BD340", FromRotation_B);

INCLUDE_ASM(const s32, "code_002BD340", ToMatrix);

INCLUDE_ASM(const s32, "code_002BD340", FromEulerXYZ);

INCLUDE_ASM(const s32, "code_002BD340", FromEuler);

INCLUDE_ASM(const s32, "code_002BD340", LookAt);

INCLUDE_ASM(const s32, "code_002BD340", Slerp);

INCLUDE_ASM(const s32, "code_002BD340", Lerp);

INCLUDE_ASM(const s32, "code_002BD340", EulerAngleAxisY);

INCLUDE_ASM(const s32, "code_002BD340", dds3GetRandA);

INCLUDE_ASM(const s32, "code_002BD340", dds3GetRandFloatA);

u32 dds3GetRandIntA(void *aState, u32 aRange)
{
    return dds3GetRandA(aState) % aRange;
}

INCLUDE_ASM(const s32, "code_002BD340", sdfRandomize);

INCLUDE_ASM(const s32, "code_002BD340", sdfRandomizeByTime);

INCLUDE_ASM(const s32, "code_002BD340", sdfGetCdClock);

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

INCLUDE_ASM(const s32, "code_002BD340", sdfSndSendCmd);

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

INCLUDE_ASM(const s32, "code_002BD340", p2sdPlayBGM);

s32 p2sdSendStopCmd(s32 aId)
{
    s32 arg = aId;

    return sdfSndSendCmd(0x30, 0, &arg, 0x10);
}

INCLUDE_ASM(const s32, "code_002BD340", p2sdResumeBgm);

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

INCLUDE_ASM(const s32, "code_002BD340", sdfSndLoadHDFile);

INCLUDE_ASM(const s32, "code_002BD340", sdfSndLoadBDFile);

INCLUDE_ASM(const s32, "code_002BD340", sdfSndLoadSQFile);

INCLUDE_ASM(const s32, "code_002BD340", p2sdPlaySE);

INCLUDE_ASM(const s32, "code_002BD340", p2sdPlaySeDeg);

INCLUDE_ASM(const s32, "code_002BD340", func_002C1990);

INCLUDE_ASM(const s32, "code_002BD340", p2sdPlaySe3DEx);

INCLUDE_ASM(const s32, "code_002BD340", p2sdSetStandPos);

INCLUDE_ASM(const s32, "code_002BD340", p2sdLoadCheckMidi);

INCLUDE_ASM(const s32, "code_002BD340", p2sdLoadSMGFile);

u64 p2sd_2C1D78(const char *aName)
{
    return sdfSndSendSyncCmd(0xA0, 0, (void *)aName, strlen(aName));
}

INCLUDE_ASM(const s32, "code_002BD340", p2sdSetPcmBuffer);

u64 p2sd_2C1E10(s32 aCh)
{
    return sdfSndSendSyncCmd((aCh & 0xF) | 0x1C0, 0, NULL, 0);
}

INCLUDE_ASM(const s32, "code_002BD340", p2sd_2C1E40);

INCLUDE_ASM(const s32, "code_002BD340", func_002C1E70);

INCLUDE_ASM(const s32, "code_002BD340", p2sd_2C1EA0);

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

INCLUDE_ASM(const s32, "code_002BD340", p2sd_2C1F90);

INCLUDE_ASM(const s32, "code_002BD340", p2sdPlayBGMFade);

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

INCLUDE_ASM(const s32, "code_002BD340", p2sdLoadPcmFile);

u32 p2sdGetPcmState(void)
{
    return p2sdPcmState;
}

s32 p2sdSyncSound(void)
{
    return sdfSndSendSyncCmd(0x100, 0, 0, 0);
}

INCLUDE_ASM(const s32, "code_002BD340", p2sd_2C21A8);

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

INCLUDE_ASM(const s32, "code_002BD340", p2sdPlayPcm);

u32 p2sd_2C2280(void)
{
    return p2sdPcmBusy;
}

s32 p2sdSyncMovie(void)
{
    return sdfSndSendSyncCmd(0x100, 0, 0, 0);
}

INCLUDE_ASM(const s32, "code_002BD340", p2sd_2C22B0);

INCLUDE_ASM(const s32, "code_002BD340", sdfFileRpcCallback);

INCLUDE_ASM(const s32, "code_002BD340", sdfFileRpcHandleCmd);

INCLUDE_ASM(const s32, "code_002BD340", SifRpcServerThreadProc);

INCLUDE_ASM(const s32, "code_002BD340", sdf_2C2608);

INCLUDE_ASM(const s32, "code_002BD340", sdf_2C2618);

INCLUDE_ASM(const s32, "code_002BD340", sdf_2C27F8);

INCLUDE_ASM(const s32, "code_002BD340", sdfDumpHexLine);

INCLUDE_ASM(const s32, "code_002BD340", sdfDumpDmaChain);

INCLUDE_RODATA(const s32, "code_002BD340", D_00549B70);

INCLUDE_RODATA(const s32, "code_002BD340", D_00549B88);

INCLUDE_RODATA(const s32, "code_002BD340", D_00549BB0);

INCLUDE_RODATA(const s32, "code_002BD340", D_00549BD0);

INCLUDE_RODATA(const s32, "code_002BD340", jtbl_00549BF0);

INCLUDE_ASM(const s32, "code_002BD340", sdfGrFlushOtChain);

INCLUDE_ASM(const s32, "code_002BD340", sdfDumpGsMemory);

INCLUDE_ASM(const s32, "code_002BD340", sdfDumpGsMemory2);

INCLUDE_RODATA(const s32, "code_002BD340", D_0054A088);

INCLUDE_RODATA(const s32, "code_002BD340", D_0054A100);

INCLUDE_ASM(const s32, "code_002BD340", sdfDumpMemory);

INCLUDE_ASM(const s32, "code_002BD340", sdfDumpChipMemory);

INCLUDE_ASM(const s32, "code_002BD340", sdfDevLoadFile);

void *sdfDevLoadFileEx(const char *aFileName, u32 *aOutData, u32 *aOutSize)
{
    return sdfDevLoadFile(aFileName, aOutData, aOutSize, 0);
}

INCLUDE_ASM(const s32, "code_002BD340", sdfDevLoadTex);

INCLUDE_ASM(const s32, "code_002BD340", sdfTexSetTB);

INCLUDE_ASM(const s32, "code_002BD340", sdfDevLoadTexList);

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

INCLUDE_ASM(const s32, "code_002BD340", sdfAddressResolver);

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuDetachQueue);

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuAttachQueue);

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

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuAllocBuff);

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuMakeToIpuDmaTag);

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuMakeBltPacket);

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuInitCb);

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuSetDecodeParams);

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuStrmDecodeInit);

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuAllocGsRect);

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuFree);

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuHandleDmaEnd);

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuSetToIpuDma);

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuSetFromIpuDma);

INCLUDE_ASM(const s32, "code_002BD340", IpuWriteCommand);

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuDecode);

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuExecQueue);

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuDecodeThread);

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuFromDmacHandler);

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuToDmacHandler);

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

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuFinishActiveFrame);

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuSetChainTag);

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuRunDecode);

void sdfIpuStartDecodeTask(sdfIpuDecodeCb_t *aNode, struct SoundFormat *aFormat, SdfStreamRead aRead, struct movieStream_t *aContext, s32 aResourceWord)
{
    sdfIpuSetDecodeParams(aNode, aFormat, aRead, aContext);
    aNode->resourceWord = aResourceWord;
    sdfIpuAttachTask(aNode);
}

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuSetOutputTex);

void movieAttachDecodeTask(sdfIpuDecodeCb_t *aNode, struct SoundFormat *aFormat, SdfStreamRead aRead, struct movieStream_t *aContext, s32 aResourceWord)
{
    sdfIpuStrmDecodeInit(aNode, aFormat, aRead, aContext);
    aNode->resourceWord = aResourceWord;
    sdfIpuAttachTask(aNode);
}

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuInit);

INCLUDE_ASM(const s32, "code_002BD340", sdfIpuClose);

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

INCLUDE_SDATA(const s32, "code_002BD340", D_005519B9);

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

INCLUDE_RODATA(const s32, "code_002BD340", D_0054A2D8);

