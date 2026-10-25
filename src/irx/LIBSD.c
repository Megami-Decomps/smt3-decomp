#include "LIBSD.h"

/* Imports LIBSD needs that iop.h does not declare, and the two exports whose
 * callers sit above them in this file. */
s32 intrman_RegisterIntrHandler(s32 irq, s32 mode, s32 (*handler)(void *), void *arg);
s32 intrman_ReleaseIntrHandler(s32 irq);
s32 intrman_EnableIntr(s32 irq);
s32 intrman_DisableIntr(s32 irq, s32 *res);
s32 intrman_QueryIntrContext(void);
s32 sifman_sceSifSetDma(sceSdSifDma_t *dma, s32 count);
s32 sifman_sceSifDmaStat(s32 id);
void sysclib_bzero(void *buf, s32 len);
u16 libsd_sceSdGetCoreAttr(u16 entry);
s32 libsd_sceSdCleanEffectWorkArea(s32 core, s32 channel, s32 effectMode);

/* The library export table: relocated function pointers, not code. */
extern u32 D_00000000[];

INCLUDE_ASM("LIBSD", D_00000000);
/* FUNC sceSdNullEntry */
s32 sceSdNullEntry()
{
}
/* ENDF sceSdNullEntry */
ALIGN_TEXT(4);
/* FUNC start */
#ifdef NON_MATCHING
/* One nop short: gas fills the jal's delay slot with the %lo half of the
 * export table's address, because the table is assembled into this same
 * translation unit. */
s32 start()
{
    s32 state;
    s32 res;

    intrman_CpuSuspendIntr(&state);
    res = loadcore_RegisterLibraryEntries(D_00000000);
    intrman_CpuResumeIntr(state);
    if (res != 0)
        return 1;
    sceSdInitDma();
    sceSdClearGlobals();
    return 0;
}
#else
INCLUDE_ASM("LIBSD", start);
#endif
/* ENDF start */
ALIGN_TEXT(4);
/* FUNC sceSdWriteAddrReg */
u32 sceSdWriteAddrReg(s32 core, s32 regIdx, u32 value)
{
    volatile u16 *reg = (volatile u16 *)(((core << 9) + regIdx) * 2 + sceSdRegBase);

    value <<= 2;
    reg[0] = value >> 16;
    reg[1] = value;
}
/* ENDF sceSdWriteAddrReg */
INCLUDE_ASM("LIBSD", sceSdApplyEffectAttrMask);
ALIGN_TEXT(4);
INCLUDE_ASM("LIBSD", libsd_sceSdClearEffectWorkArea);
INCLUDE_ASM("LIBSD", sceSdCleanEffectStep);
INCLUDE_ASM("LIBSD", libsd_sceSdCleanEffectWorkArea);
ALIGN_TEXT(4);
/* FUNC sceSdMemcpy */
void sceSdMemcpy(u8 *dst, u8 *src, s32 len)
{
    s32 i;

    for (i = len - 1; i != -1; i--)
        *dst++ = *src++;
}
/* ENDF sceSdMemcpy */
/* FUNC libsd_sceSdGetEffectAttr */
void libsd_sceSdGetEffectAttr(s32 core, sceSdEffectAttr_t *attr)
{
    attr->core = core;
    attr->mode = sceSdEffectAttr[core].mode;
    attr->delay = sceSdEffectAttr[core].delay;
    attr->feedback = sceSdEffectAttr[core].feedback;
    attr->depth_L = *(volatile u16 *)(0xBF900764 + core * 0x28);
    attr->depth_R = *(volatile u16 *)(0xBF900766 + core * 0x28);
}
/* ENDF libsd_sceSdGetEffectAttr */
INCLUDE_ASM("LIBSD", libsd_sceSdSetEffectAttr);
/* FUNC sceSdGetEffectAreaEnd */
u32 sceSdGetEffectAreaEnd(s32 core)
{
    volatile u16 *reg = (volatile u16 *)(0xBF90033C + (core << 10));

    return (*reg << 17) | 0x1FFFF;
}
/* ENDF sceSdGetEffectAreaEnd */
INCLUDE_ASM("LIBSD", libsd_sceSdSetEffectMode);
INCLUDE_ASM("LIBSD", libsd_sceSdSetEffectModeParams);
ALIGN_TEXT(4);
/* FUNC sceSdInitDma */
s32 sceSdInitDma()
{
    s32 state;

    *(volatile u32 *)0xBF801404 = 0xBF900000;
    *(volatile u32 *)0xBF80140C = 0xBF900800;
    intrman_CpuSuspendIntr(&state);
    *(volatile u32 *)0xBF8010F0 |= 0x80000;
    *(volatile u32 *)0xBF801570 |= 0x8;
    intrman_CpuResumeIntr(state);
    *(volatile u32 *)0xBF801014 = 0x200B31E1;
    *(volatile u32 *)0xBF801414 = 0x200B31E1;
}
/* ENDF sceSdInitDma */
/* FUNC sceSdInitHardware */
s32 sceSdInitHardware()
{
    sceSdInitDma();
    *(volatile u16 *)0xBF9007C6 = 0x900;
    *(volatile u16 *)0xBF9007C8 = 0x200;
    *(volatile u16 *)0xBF9007CA = 0x8;
}
/* ENDF sceSdInitHardware */
INCLUDE_ASM("LIBSD", sceSdInitCoreMix);
INCLUDE_ASM("LIBSD", libsd_sceSdVoiceTrans);
ALIGN_TEXT(4);
INCLUDE_ASM("LIBSD", libsd_sceSdVoiceTransStatus);
INCLUDE_ASM("LIBSD", libsd_sceSdStopTrans);
ALIGN_TEXT(4);
INCLUDE_ASM("LIBSD", libsd_sceSdBlockTrans);
INCLUDE_ASM("LIBSD", libsd_sceSdBlockTransStatus);
/* FUNC sceSdStubNoOp */
void sceSdStubNoOp()
{
    volatile s32 sync;

    sync = 0;
    sync = 0;
    sync = 0;
    sync = 0;
}
/* ENDF sceSdStubNoOp */
INCLUDE_ASM("LIBSD", sceSdResetCores);
/* FUNC sceSdSetDmaModeWrite */
u32 sceSdSetDmaModeWrite(s32 core)
{
    volatile u32 *reg = (volatile u32 *)(0xBF801014 + (core << 10));

    *reg = *reg & 0xF0FFFFFF;
}
/* ENDF sceSdSetDmaModeWrite */
/* FUNC sceSdSetDmaModeRead */
u32 sceSdSetDmaModeRead(s32 core)
{
    volatile u32 *reg = (volatile u32 *)(0xBF801014 + (core << 10));

    *reg = (*reg & 0xF0FFFFFF) | 0x2000000;
}
/* ENDF sceSdSetDmaModeRead */
/* FUNC sceSdDelayUnit */
void sceSdDelayUnit()
{
    volatile s32 i;
    volatile s32 acc = 13;

    for (i = 0; i < 120; i++)
        acc = acc * 13;
}
/* ENDF sceSdDelayUnit */
/* FUNC sceSdDelay */
void sceSdDelay(s32 count)
{
    s32 i;

    for (i = 0; i < count; i++)
        sceSdDelayUnit(1);
}
/* ENDF sceSdDelay */
INCLUDE_ASM("LIBSD", sceSdTransferOp);
INCLUDE_ASM("LIBSD", sceSdVoiceTransIo);
INCLUDE_ASM("LIBSD", sceSdDmaIntrHandler);
INCLUDE_ASM("LIBSD", sceSdBlockTransPoll);
INCLUDE_ASM("LIBSD", sceSdBlockTransWrite);
INCLUDE_ASM("LIBSD", sceSdBlockTransRead);
INCLUDE_ASM("LIBSD", sceSdSifDmaToEe);
INCLUDE_ASM("LIBSD", libsd_sceSdProcBatch);
INCLUDE_ASM("LIBSD", libsd_sceSdProcBatchEx);
ALIGN_TEXT(4);
/* FUNC libsd_sceSdSetParam */
u16 libsd_sceSdSetParam(u16 entry, u16 value)
{
    s32 core = entry & 1;
    s32 attr = entry & 0x80;
    volatile u16 *reg;

    attr = attr != 0;
    reg = (volatile u16 *)(sceSdRegAddrTbl[entry >> 8] + core * (0x400 - attr * 0x3D8)
                           + ((entry & 0x3E) << 3));
    *reg = value;
}
/* ENDF libsd_sceSdSetParam */
/* FUNC libsd_sceSdGetParam */
u16 libsd_sceSdGetParam(u16 entry)
{
    s32 core = entry & 1;
    s32 attr = entry & 0x80;
    u32 base;
    volatile u16 *reg;

    attr = attr != 0;
    base = sceSdRegAddrTbl[entry >> 8] + core * (0x400 - attr * 0x3D8);
    reg = (volatile u16 *)(((entry & 0x3E) << 3) + base);
    return *reg;
}
/* ENDF libsd_sceSdGetParam */
/* FUNC libsd_sceSdSetSwitch */
void libsd_sceSdSetSwitch(u16 entry, u32 value)
{
    volatile u16 *reg = (volatile u16 *)(sceSdRegAddrTbl[entry >> 8] + ((entry & 1) << 10));

    *reg++ = value;
    *reg = (value >> 16) & 0xFF;
}
/* ENDF libsd_sceSdSetSwitch */
/* FUNC libsd_sceSdGetSwitch */
u32 libsd_sceSdGetSwitch(u16 entry)
{
    u32 base = sceSdRegAddrTbl[entry >> 8];
    volatile u16 *reg = (volatile u16 *)(((entry & 1) << 10) + base);

    return reg[0] | (reg[1] << 16);
}
/* ENDF libsd_sceSdGetSwitch */
/* FUNC libsd_sceSdSetAddr */
#ifdef NON_MATCHING
/* One instruction transposed: gcc's scheduler puts the halfword store ahead
 * of the entry & 0xFF00 the branch needs. */
void libsd_sceSdSetAddr(u16 entry, u32 value)
{
    volatile u16 *reg = (volatile u16 *)(sceSdRegAddrTbl[entry >> 8] + ((entry & 1) << 10)
                                         + (entry & 0x3E) * 6);
    u32 top = entry & 0xFF00;

    *reg++ = value >> 17;
    if (top != 0x1D00)
        *reg = (value >> 1) & 0xFFF8;
}
#else
INCLUDE_ASM("LIBSD", libsd_sceSdSetAddr);
#endif
/* ENDF libsd_sceSdSetAddr */
/* FUNC libsd_sceSdGetAddr */
u32 libsd_sceSdGetAddr(u16 entry)
{
    volatile u16 *reg = (volatile u16 *)(sceSdRegAddrTbl[entry >> 8] + ((entry & 1) << 10)
                                         + (entry & 0x3E) * 6);
    u32 hi = reg[0] << 17;
    u32 lo = 0x1FFFF;

    if ((entry & 0xFF00) != 0x1D00) {
        lo = reg[1] << 1;
        if ((entry & 0xFF00) == 0x2100 || (entry & 0xFF00) == 0x2200) {
            u32 hi2 = reg[0] << 17;
            u32 lo2 = reg[1] << 1;
            if (hi != hi2 || lo == lo2) {
                hi = hi2;
                lo = lo2;
            }
        }
    }
    return hi | lo;
}
/* ENDF libsd_sceSdGetAddr */
INCLUDE_ASM("LIBSD", libsd_sceSdNote2Pitch);
INCLUDE_ASM("LIBSD", libsd_sceSdPitch2Note);
INCLUDE_ASM("LIBSD", sceSdSetSpdifMode);
/* FUNC sceSdSetCoreAttrBit */
void sceSdSetCoreAttrBit(u16 entry, u16 value)
{
    s32 state;
    s16 shift = sceSdCoreAttrBit[(entry & 0xE) / 2 - 1];
    u16 reg;
    u16 v;

    intrman_CpuSuspendIntr(&state);
    reg = (entry & 1) | 0x2300;
    v = libsd_sceSdGetParam(reg);
    v = (v & ~(1 << shift)) | ((value & 1) << shift);
    libsd_sceSdSetParam(reg, v);
    intrman_CpuResumeIntr(state);
}
/* ENDF sceSdSetCoreAttrBit */
/* FUNC libsd_sceSdSetCoreAttr */
#ifdef NON_MATCHING
/* Two short: iop.h declares libsd_sceSdGetParam returning u16, so gcc knows
 * the high bits are clear and narrows the 32-bit mask to andi 0xC0FF. Retail's
 * own translation unit had no prototype in scope, so the call result is
 * int-width and the mask needs a register. */
u16 libsd_sceSdSetCoreAttr(u16 entry, u16 value)
{
    s32 state;

    if ((entry & 0xFFFE) == 0xA) {
        sceSdSetSpdifMode(value);
    } else if ((entry & 0xFFFE) == 8) {
        u16 reg;
        u32 v;

        intrman_CpuSuspendIntr(&state);
        reg = (entry & 1) | 0x2300;
        v = libsd_sceSdGetParam(reg);
        v &= ~0x3F00;
        v |= (value & 0x3F) << 8;
        libsd_sceSdSetParam(reg, v);
        intrman_CpuResumeIntr(state);
    } else {
        sceSdSetCoreAttrBit(entry, value);
    }
}
#else
INCLUDE_ASM("LIBSD", libsd_sceSdSetCoreAttr);
#endif
/* ENDF libsd_sceSdSetCoreAttr */
/* FUNC libsd_sceSdGetCoreAttr */
#ifdef NON_MATCHING
/* Cannot match while .rodata is assembly: the switch compiles to a jump table
 * whose case labels the assembly .rodata already defines, so a C body leaves
 * them undefined and the link fails. */
u16 libsd_sceSdGetCoreAttr(u16 entry)
{
    u16 reg = *(volatile u16 *)(0xBF90019A + ((entry & 1) << 10));

    switch (entry & 0xE) {
    case 2:
        return (reg >> 7) & 1;
    case 4:
        return (reg >> 6) & 1;
    case 6:
        return (reg >> 14) & 1;
    case 8:
        return (reg >> 8) & 0x3F;
    case 10:
        return sceSdSpdifMode;
    }
    return 0;
}
#else
INCLUDE_ASM("LIBSD", libsd_sceSdGetCoreAttr);
#endif
/* ENDF libsd_sceSdGetCoreAttr */
/* FUNC libsd_sceSdSetTransCallback */
#ifdef NON_MATCHING
/* Three short: gcc's CSE keeps the stored pointer live where retail reloads
 * sceSdTransCallback[chan] for the read-back. */
void *libsd_sceSdSetTransCallback(u16 chan, void *func)
{
    void *volatile sync;
    void *old = sceSdTransCallback[chan];

    sceSdTransCallback[chan] = func;
    sync = sceSdTransCallback[chan];
    return old;
}
#else
INCLUDE_ASM("LIBSD", libsd_sceSdSetTransCallback);
#endif
/* ENDF libsd_sceSdSetTransCallback */
/* FUNC libsd_sceSdSetTransIntrHandler */
void *libsd_sceSdSetTransIntrHandler(s32 chan, void *func, void *arg)
{
    void *volatile sync;
    void *old = sceSdTransIntrHandler[chan];

    sceSdTransIntrHandler[chan] = func;
    sceSdTransIntrArg[chan].arg = arg;
    sync = sceSdTransIntrArg[chan].arg;
    return old;
}
/* ENDF libsd_sceSdSetTransIntrHandler */
/* FUNC libsd_sceSdGetTransIntrHandlerArgument */
void *libsd_sceSdGetTransIntrHandlerArgument(s32 chan)
{
    return sceSdTransIntrArg[chan].arg;
}
/* ENDF libsd_sceSdGetTransIntrHandlerArgument */
/* FUNC libsd_sceSdSetIRQCallback */
void *libsd_sceSdSetIRQCallback(void *func)
{
    void *volatile sync;
    void *old = sceSdIrqCallback;

    sceSdIrqCallback = func;
    sync = sceSdIrqCallback;
    return old;
}
/* ENDF libsd_sceSdSetIRQCallback */
/* FUNC libsd_sceSdSetSpu2IntrHandler */
void *libsd_sceSdSetSpu2IntrHandler(void *func, void *arg)
{
    void *volatile sync;
    void *old = sceSdSpu2IntrHandler;

    sceSdSpu2IntrHandler = func;
    sceSdSpu2IntrArg = arg;
    sync = sceSdSpu2IntrArg;
    return old;
}
/* ENDF libsd_sceSdSetSpu2IntrHandler */
/* FUNC libsd_sceSdGetSpu2IntrHandlerArgument */
void *libsd_sceSdGetSpu2IntrHandlerArgument()
{
    return sceSdSpu2IntrArg;
}
/* ENDF libsd_sceSdGetSpu2IntrHandlerArgument */
ALIGN_TEXT(4);
INCLUDE_ASM("LIBSD", sceSdIrqHandler);
INCLUDE_ASM("LIBSD", sceSdResetVoices);
INCLUDE_ASM("LIBSD", sceSdInitState);
/* FUNC sceSdClearGlobals */
void sceSdClearGlobals()
{
    sceSdInitialized = 0;
    sceSdTransEventFlag[0] = 0;
    sceSdTransEventFlag[1] = 0;
}
/* ENDF sceSdClearGlobals */
INCLUDE_ASM("LIBSD", libsd_sceSdInit);
INCLUDE_ASM("LIBSD", libsd_sceSdQuit);
ALIGN_TEXT(4);
IRX_IMPORT_TABLE(intrman, 1, 2);
IRX_IMPORT(intrman, RegisterIntrHandler, 0x4);
IRX_IMPORT(intrman, ReleaseIntrHandler, 0x5);
IRX_IMPORT(intrman, EnableIntr, 0x6);
IRX_IMPORT(intrman, DisableIntr, 0x7);
IRX_IMPORT(intrman, CpuSuspendIntr, 0x11);
IRX_IMPORT(intrman, CpuResumeIntr, 0x12);
IRX_IMPORT(intrman, QueryIntrContext, 0x17);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(loadcore, 1, 3);
IRX_IMPORT(loadcore, FlushDcache, 0x5);
IRX_IMPORT(loadcore, RegisterLibraryEntries, 0x6);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(sifman, 1, 1);
IRX_IMPORT(sifman, sceSifSetDma, 0x7);
IRX_IMPORT(sifman, sceSifDmaStat, 0x8);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(sysclib, 1, 3);
IRX_IMPORT(sysclib, bzero, 0x11);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(thevent, 1, 1);
IRX_IMPORT(thevent, CreateEventFlag, 0x4);
IRX_IMPORT(thevent, DeleteEventFlag, 0x5);
IRX_IMPORT(thevent, SetEventFlag, 0x6);
IRX_IMPORT(thevent, iSetEventFlag, 0x7);
IRX_IMPORT(thevent, ClearEventFlag, 0x8);
IRX_IMPORT(thevent, iClearEventFlag, 0x9);
IRX_IMPORT(thevent, WaitEventFlag, 0xa);
IRX_IMPORT_TABLE_END;
ALIGN_TEXT(4);
