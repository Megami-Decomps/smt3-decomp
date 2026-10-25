#include "LIBSD.h"

/* Imports LIBSD needs that iop.h does not declare, and the two exports whose
 * callers sit above them in this file. */
s32 intrman_RegisterIntrHandler(s32 irq, s32 mode, void *handler, void *arg);
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
/* FUNC sceSdApplyEffectAttrMask */
s32 sceSdApplyEffectAttrMask(s32 core, sceSdEffectPreset_t *preset)
{
    u32 mask;
    s32 all;

    mask = preset->mask;
    all = mask < 1;

    if (all || (mask & 0x1))
        sceSdWriteAddrReg(core, 0x172, preset->apf1Size);
    if (all || (mask & 0x2))
        sceSdWriteAddrReg(core, 0x174, preset->apf2Size);
    if (all || (mask & 0x4))
        *(volatile u16 *)(core * 0x28 + sceSdRegBase + 0x774) = preset->iirVol;
    if (all || (mask & 0x8))
        *(volatile u16 *)(core * 0x28 + sceSdRegBase + 0x776) = preset->comb1Vol;
    if (all || (mask & 0x10))
        *(volatile u16 *)(core * 0x28 + sceSdRegBase + 0x778) = preset->comb2Vol;
    if (all || (mask & 0x20))
        *(volatile u16 *)(core * 0x28 + sceSdRegBase + 0x77A) = preset->comb3Vol;
    if (all || (mask & 0x40))
        *(volatile u16 *)(core * 0x28 + sceSdRegBase + 0x77C) = preset->comb4Vol;
    if (all || (mask & 0x80))
        *(volatile u16 *)(core * 0x28 + sceSdRegBase + 0x77E) = preset->wallVol;
    if (all || (mask & 0x100))
        *(volatile u16 *)(core * 0x28 + sceSdRegBase + 0x780) = preset->apf1Vol;
    if (all || (mask & 0x200))
        *(volatile u16 *)(core * 0x28 + sceSdRegBase + 0x782) = preset->apf2Vol;
    if (all || (mask & 0x400))
        sceSdWriteAddrReg(core, 0x176, preset->sameLDst);
    if (all || (mask & 0x800))
        sceSdWriteAddrReg(core, 0x178, preset->sameRDst);
    if (all || (mask & 0x1000))
        sceSdWriteAddrReg(core, 0x17A, preset->comb1LSrc);
    if (all || (mask & 0x2000))
        sceSdWriteAddrReg(core, 0x17C, preset->comb1RSrc);
    if (all || (mask & 0x4000))
        sceSdWriteAddrReg(core, 0x17E, preset->comb2LSrc);
    if (all || (mask & 0x8000))
        sceSdWriteAddrReg(core, 0x180, preset->comb2RSrc);
    if (all || (mask & 0x10000))
        sceSdWriteAddrReg(core, 0x182, preset->sameLSrc);
    if (all || (mask & 0x20000))
        sceSdWriteAddrReg(core, 0x184, preset->sameRSrc);
    if (all || (mask & 0x40000))
        sceSdWriteAddrReg(core, 0x186, preset->diffLDst);
    if (all || (mask & 0x80000))
        sceSdWriteAddrReg(core, 0x188, preset->diffRDst);
    if (all || (mask & 0x100000))
        sceSdWriteAddrReg(core, 0x18A, preset->comb3LSrc);
    if (all || (mask & 0x200000))
        sceSdWriteAddrReg(core, 0x18C, preset->comb3RSrc);
    if (all || (mask & 0x400000))
        sceSdWriteAddrReg(core, 0x18E, preset->comb4LSrc);
    if (all || (mask & 0x800000))
        sceSdWriteAddrReg(core, 0x190, preset->comb4RSrc);
    if (all || (mask & 0x1000000))
        sceSdWriteAddrReg(core, 0x192, preset->diffLSrc);
    if (all || (mask & 0x2000000))
        sceSdWriteAddrReg(core, 0x194, preset->diffRSrc);
    if (all || (mask & 0x4000000))
        sceSdWriteAddrReg(core, 0x196, preset->apf1LDst);
    if (all || (mask & 0x8000000))
        sceSdWriteAddrReg(core, 0x198, preset->apf1RDst);
    if (all || (mask & 0x10000000))
        sceSdWriteAddrReg(core, 0x19A, preset->apf2LDst);
    if (all || (mask & 0x20000000))
        sceSdWriteAddrReg(core, 0x19C, preset->apf2RDst);
    if (all || (mask & 0x40000000))
        *(volatile u16 *)(core * 0x28 + sceSdRegBase + 0x784) = preset->inCoefL;
    if (all || (mask & 0x80000000))
        *(volatile u16 *)(core * 0x28 + sceSdRegBase + 0x786) = preset->inCoefR;
}
/* ENDF sceSdApplyEffectAttrMask */
ALIGN_TEXT(4);
/* FUNC libsd_sceSdClearEffectWorkArea */
#ifdef NON_MATCHING
/* Retail's length and registers, but two independent register moves before the
 * first sceSdGetEffectAreaEnd call come out transposed: the scheduler puts
 * alignedAddr = 0 ahead of the argument move wherever the assignment sits. */
s32 libsd_sceSdClearEffectWorkArea(s32 core, s32 channel, s32 effectMode)
{
    u32 mode = effectMode & 0xFF;
    void *volatile saved[2];
    u32 size;
    u32 addr;
    u32 alignedAddr;
    s32 res;
    s32 more;
    u32 len;

    if (mode >= 10)
        return -100;
    if (mode == 0)
        return 0;
    if (sceSdTransferOp((channel << 4) | 4) != 0)
        return -210;
    if (sceSdTransIdle[channel] != 1)
        return -201;
    if (intrman_QueryIntrContext() != 0)
        return -202;

    size = sceSdEffectAreaSize[mode] * 8;
    {
        u32 last = size - 1;

        alignedAddr = 0;
        addr = (sceSdGetEffectAreaEnd(core) - last) >> 1;
    }
    if ((size & 0x3F) != 0) {
        u32 last;
        size = (size >> 6) << 6;
        last = size - 1;
        alignedAddr = (sceSdGetEffectAreaEnd(core) - last) >> 1;
    }
    saved[1] = sceSdTransIntrHandler[channel];
    saved[0] = sceSdTransCallback[channel];
    sceSdTransIntrHandler[channel] = 0;
    sceSdTransCallback[channel] = 0;
    if (alignedAddr != 0) {
        res = libsd_sceSdVoiceTrans(channel, 0, sceSdZeroBuf, addr << 1, 0x40);
        if (res < 0)
            goto done;
        res = libsd_sceSdVoiceTransStatus(channel, 1);
        if (res < 0)
            goto done;
        addr = alignedAddr;
    }
    more = 1;
    do {
        len = 0x400;
        if (size <= 0x400) {
            len = size;
            more = 0;
        }
        res = libsd_sceSdVoiceTrans(channel, 0, sceSdZeroBuf, addr << 1, len);
        if (res < 0)
            goto done;
        res = libsd_sceSdVoiceTransStatus(channel, 1);
        if (res < 0)
            goto done;
        size -= 0x400;
        addr += 0x200;
    } while (more);
    res = 0;
done:
    sceSdTransIntrHandler[channel] = saved[1];
    sceSdTransCallback[channel] = saved[0];
    sceSdStubNoOp();
    return res;
}
#else
INCLUDE_ASM("LIBSD", libsd_sceSdClearEffectWorkArea);
#endif
/* ENDF libsd_sceSdClearEffectWorkArea */
/* FUNC sceSdCleanEffectStep */
s32 sceSdCleanEffectStep(s32 chan)
{
    sceSdCleanChunkIndex[chan]++;
    if (sceSdCleanChunkIndex[chan] >= sceSdCleanChunkCount[chan] - 1)
        sceSdCleanStepCallback[chan] = 0;
    sceSdTransferOp((chan << 4) | 2, sceSdZeroBuf);
    sceSdTransferOp((chan << 4) | 6,
                    sceSdCleanChunk[chan][sceSdCleanChunkIndex[chan]].addr,
                    sceSdCleanChunk[chan][sceSdCleanChunkIndex[chan]].size);
    return 0;
}
/* ENDF sceSdCleanEffectStep */
/* FUNC libsd_sceSdCleanEffectWorkArea */
s32 libsd_sceSdCleanEffectWorkArea(s32 core, s32 channel, s32 effectMode)
{
    u32 mode = effectMode & 0xFF;
    sceSdClearChunk_t *chunk;
    u32 size;
    u32 addr;
    u32 next;
    s32 rem;
    s32 res;

    if (mode >= 10)
        return -100;
    if (mode == 0)
        return 0;
    if (sceSdTransferOp((channel << 4) | 4) != 0)
        return -210;
    if (sceSdTransIdle[channel] != 1)
        return -201;

    size = sceSdEffectAreaSize[mode] * 8;
    {
        u32 last = size - 1;
        addr = sceSdGetEffectAreaEnd(core) - last;
    }
    if ((size & 0x3F) != 0) {
        u32 last;
        size = (size >> 6) << 6;
        res = libsd_sceSdVoiceTrans(channel, 8, sceSdZeroBuf, addr, 0x40);
        if (res < 0)
            goto done;
        last = size - 1;
        addr = sceSdGetEffectAreaEnd(core) - last;
    }
    chunk = (sceSdClearChunk_t *)sceSdCleanChunk[channel];
    next = addr + 0x400;
    rem = size - 0x400;
    sceSdCleanChunkCount[channel] = 0;
    while (rem > 0x400) {
        chunk->addr = next;
        chunk->size = 0x400;
        chunk++;
        next += 0x400;
        sceSdCleanChunkCount[channel]++;
        rem -= 0x400;
    }
    chunk->addr = next;
    chunk->size = rem;
    sceSdCleanChunkCount[channel]++;
    sceSdCleanStepCallback[channel] = (void *)sceSdCleanEffectStep;
    sceSdCleanChunkIndex[channel] = 0;
    res = libsd_sceSdVoiceTrans(channel, 0, sceSdZeroBuf, addr, 0x400);
    if (res < 0)
        goto done;
    res = 0;
done:
    sceSdStubNoOp();
    return res;
}
/* ENDF libsd_sceSdCleanEffectWorkArea */
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
    attr->depth_L = *(volatile u16 *)SPU2_EVOLL(core);
    attr->depth_R = *(volatile u16 *)SPU2_EVOLR(core);
}
/* ENDF libsd_sceSdGetEffectAttr */
/* FUNC libsd_sceSdSetEffectAttr */
s32 libsd_sceSdSetEffectAttr(s32 core, sceSdEffectAttr_t *attr)
{
    volatile u16 *statReg;
    sceSdEffectPreset_t preset;
    volatile u32 sync;
    s32 mode;
    s32 oldMode;
    u32 regOfs;
    s32 clean;
    s32 channel;
    s32 enabled;
    s32 copied;
    s32 recopied;
    s32 state;
    s32 res;
    s32 d;
    s32 t;

    clean = 0;
    channel = 0;
    enabled = 0;
    oldMode = 0;
    recopied = 0;
    statReg = (volatile u16 *)SPU2_ATTR(core);
    preset.mask = 0;
    mode = attr->mode;
    if ((mode & 0x100) != 0) {
        oldMode = sceSdEffectAttr[core].mode;
        clean = 1;
        t = mode & 0x200;
        channel = t != 0;
    }
    mode &= 0xFF;
    if ((u32)mode >= 10)
        return -100;
    sceSdEffectAttr[core].mode = mode;
    regOfs = core << 9;
    {
        u32 end = sceSdGetEffectAreaEnd(core);
        u32 last = sceSdEffectAreaSize[mode] * 8 - 1;
        sceSdEffectAreaStart[core] = end - last;
    }
    sceSdMemcpy((u8 *)&preset, (u8 *)&sceSdEffectPreset[sceSdEffectAttr[core].mode], 0x44);
    copied = 1;
    switch (mode) {
    case 7:
        sceSdEffectAttr[core].feedback = 0x80;
        sceSdEffectAttr[core].delay = 0x80;
        break;
    case 8:
        sceSdEffectAttr[core].feedback = 0;
        sceSdEffectAttr[core].delay = 0x80;
        break;
    default:
        sceSdEffectAttr[core].feedback = 0;
        sceSdEffectAttr[core].delay = 0;
        break;
    }
    if (mode < 9) {
      if (mode >= 7) {
        recopied = 1;
        if (copied == 0) {
            sceSdMemcpy((u8 *)&preset,
                        (u8 *)&sceSdEffectPreset[sceSdEffectAttr[core].mode], 0x44);
            preset.mask = 0xC011C00;
        }
        sceSdEffectAttr[core].delay = attr->delay;
        d = sceSdEffectAttr[core].delay + 1;
        preset.sameLDst = d * 64 - preset.apf1Size;
        preset.sameRDst = d * 32 - preset.apf2Size;
        preset.comb1LSrc = preset.comb1RSrc + d * 32;
        preset.sameLSrc = preset.sameRSrc + d * 32;
        preset.apf1LDst = preset.apf2LDst + d * 32;
        preset.apf1RDst = preset.apf2RDst + d * 32;
      }
    }
    if (mode < 9) {
      if (mode >= 7) {
        if (copied == 0) {
            if (recopied == 0) {
                sceSdMemcpy((u8 *)&preset,
                            (u8 *)&sceSdEffectPreset[sceSdEffectAttr[core].mode], 0x44);
                preset.mask = 0x80;
            } else {
                preset.mask |= 0x80;
            }
        }
        sceSdEffectAttr[core].feedback = attr->feedback;
        preset.wallVol = sceSdEffectAttr[core].feedback * 258;
      }
    }
    if (copied != 0) {
        enabled = (*statReg >> 7) & 1;
        if (enabled != 0) {
            intrman_CpuSuspendIntr(&state);
            *statReg &= 0xFF7F;
            intrman_CpuResumeIntr(state);
        }
    }
    if (enabled != 0 && clean != 0) {
        res = libsd_sceSdClearEffectWorkArea(core, channel, oldMode);
        if (res < 0)
            goto done;
    }
    *(volatile u16 *)SPU2_EVOLL(core) = attr->depth_L;
    *(volatile u16 *)SPU2_EVOLR(core) = attr->depth_R;
    sceSdApplyEffectAttrMask(core, &preset);
    if (copied != 0) {
        *(volatile u16 *)(regOfs * 2 + SPU2_ESA(0)) = sceSdEffectAreaStart[core] >> 17;
        *(volatile u16 *)(regOfs * 2 + (SPU2_ESA(0) + 2)) = sceSdEffectAreaStart[core] >> 1;
        sync = *(volatile u16 *)(regOfs * 2 + (SPU2_ESA(0) + 2));
    }
    res = 0;
    if (clean != 0)
        res = libsd_sceSdClearEffectWorkArea(core, channel, mode);
done:
    if (copied != 0 && enabled != 0) {
        intrman_CpuSuspendIntr(&state);
        *statReg |= 0x80;
        sync = *statReg;
        intrman_CpuResumeIntr(state);
    }
    sceSdStubNoOp();
    return res;
}
/* ENDF libsd_sceSdSetEffectAttr */
/* FUNC sceSdGetEffectAreaEnd */
u32 sceSdGetEffectAreaEnd(s32 core)
{
    volatile u16 *reg = (volatile u16 *)SPU2_EEA(core);

    return (*reg << 17) | 0x1FFFF;
}
/* ENDF sceSdGetEffectAreaEnd */
/* FUNC libsd_sceSdSetEffectMode */
s32 libsd_sceSdSetEffectMode(s32 core, sceSdEffectAttr_t *attr)
{
    volatile u16 *statReg;
    sceSdEffectPreset_t preset;
    volatile u32 sync;
    u32 mode;
    s32 clean;
    s32 channel;
    s32 enabled;
    s32 state;
    s32 res;
    s32 t;

    clean = 0;
    channel = 0;
    statReg = (volatile u16 *)SPU2_ATTR(core);
    preset.mask = 0;
    mode = attr->mode;
    if ((mode & 0x100) != 0) {
        clean = 1;
        t = mode & 0x200;
        channel = t != 0;
    }
    mode &= 0xFF;
    if (mode >= 10)
        return -100;
    sceSdEffectAttr[core].mode = mode;
    sceSdEffectAttr[core].delay = 0;
    sceSdEffectAttr[core].feedback = 0;
    {
        u32 end = sceSdGetEffectAreaEnd(core);
        u32 last = sceSdEffectAreaSize[mode] * 8 - 1;
        sceSdEffectAreaStart[core] = end - last;
    }
    preset = *(sceSdEffectPreset_t *)&sceSdEffectPreset[mode];
    enabled = (*statReg >> 7) & 1;
    if (enabled != 0) {
        intrman_CpuSuspendIntr(&state);
        *statReg &= 0xFF7F;
        intrman_CpuResumeIntr(state);
    }
    *(volatile u16 *)SPU2_EVOLL(core) = 0;
    *(volatile u16 *)SPU2_EVOLR(core) = 0;
    sceSdApplyEffectAttrMask(core, &preset);
    {
        volatile u16 *areaHi = (volatile u16 *)SPU2_ESA(core);
        volatile u16 *areaLo = (volatile u16 *)(SPU2_ESA(core) + 2);

        *areaHi = sceSdEffectAreaStart[core] >> 17;
        *areaLo = sceSdEffectAreaStart[core] >> 1;
    }
    if (enabled != 0) {
        intrman_CpuSuspendIntr(&state);
        *statReg |= 0x80;
        intrman_CpuResumeIntr(state);
        sync = *statReg;
    }
    res = 0;
    if (clean != 0)
        res = libsd_sceSdCleanEffectWorkArea(core, channel, mode);
    return res;
}
/* ENDF libsd_sceSdSetEffectMode */
/* FUNC libsd_sceSdSetEffectModeParams */
s32 libsd_sceSdSetEffectModeParams(s32 core, sceSdEffectAttr_t *attr)
{
    sceSdEffectPreset_t preset;
    u32 mode = *(u8 *)&attr->mode;
    s32 cur;
    s32 d;

    if (mode < 10) {
        cur = sceSdEffectAttr[core].mode;
        if (cur != mode)
            return -100;
    } else {
        return -100;
    }
    if (cur < 9) {
      if (cur >= 7) {
        preset = *(sceSdEffectPreset_t *)&sceSdEffectPreset[cur];
        preset.mask = 0xC011C80;
        sceSdEffectAttr[core].delay = attr->delay;
        sceSdEffectAttr[core].feedback = attr->feedback;
        d = sceSdEffectAttr[core].delay + 1;
        preset.sameLDst = d * 64 - preset.apf1Size;
        preset.sameRDst = d * 32 - preset.apf2Size;
        preset.comb1LSrc = preset.comb1RSrc + d * 32;
        preset.sameLSrc = preset.sameRSrc + d * 32;
        preset.apf1LDst = preset.apf2LDst + d * 32;
        preset.apf1RDst = preset.apf2RDst + d * 32;
        preset.wallVol = sceSdEffectAttr[core].feedback * 258;
        sceSdApplyEffectAttrMask(core, &preset);
      }
    }
    *(volatile u16 *)SPU2_EVOLL(core) = attr->depth_L;
    *(volatile u16 *)SPU2_EVOLR(core) = attr->depth_R;
    sceSdStubNoOp();
    return 0;
}
/* ENDF libsd_sceSdSetEffectModeParams */
ALIGN_TEXT(4);
/* FUNC sceSdInitDma */
s32 sceSdInitDma()
{
    s32 state;

    *(volatile u32 *)IOP_SPU2_DMA0_BANK = 0xBF900000;
    *(volatile u32 *)IOP_SPU2_DMA1_BANK = 0xBF900800;
    intrman_CpuSuspendIntr(&state);
    *(volatile u32 *)IOP_DPCR |= IOP_DMA_CH4_ENABLE;
    *(volatile u32 *)IOP_DPCR2 |= IOP_DMA_CH7_ENABLE;
    intrman_CpuResumeIntr(state);
    *(volatile u32 *)IOP_SPU_DELAY = 0x200B31E1;
    *(volatile u32 *)IOP_SPU2_DELAY = 0x200B31E1;
}
/* ENDF sceSdInitDma */
/* FUNC sceSdInitHardware */
s32 sceSdInitHardware()
{
    sceSdInitDma();
    *(volatile u16 *)SPU2_SPDIF_MODE = 0x900;
    *(volatile u16 *)SPU2_SPDIF_MEDIA = 0x200;
    *(volatile u16 *)SPU2_SPDIF_PROTECT = 0x8;
}
/* ENDF sceSdInitHardware */
/* FUNC sceSdInitCoreMix */
s32 sceSdInitCoreMix(s32 keepEffect)
{
    *(volatile u16 *)SPU2_SPDIF_OUT = 0xC032;
    if (keepEffect == 0) {
        *(volatile u16 *)SPU2_ATTR(0) = 0xC000;
        *(volatile u16 *)SPU2_ATTR(1) = 0xC001;
    } else {
        *(volatile u16 *)SPU2_ATTR(0) = 0xC080;
        *(volatile u16 *)SPU2_ATTR(1) = 0xC081;
    }
    *(volatile u16 *)SPU2_VMIXL(0) = 0xFFFF;
    *(volatile u16 *)(SPU2_VMIXL(0) + 2) = 0xFF;
    *(volatile u16 *)SPU2_VMIXR(0) = 0xFFFF;
    *(volatile u16 *)(SPU2_VMIXR(0) + 2) = 0xFF;
    *(volatile u16 *)SPU2_VMIXEL(0) = 0xFFFF;
    *(volatile u16 *)(SPU2_VMIXEL(0) + 2) = 0xFF;
    *(volatile u16 *)SPU2_VMIXER(0) = 0xFFFF;
    *(volatile u16 *)(SPU2_VMIXER(0) + 2) = 0xFF;
    *(volatile u16 *)SPU2_VMIXL(1) = 0xFFFF;
    *(volatile u16 *)(SPU2_VMIXL(1) + 2) = 0xFF;
    *(volatile u16 *)SPU2_VMIXR(1) = 0xFFFF;
    *(volatile u16 *)(SPU2_VMIXR(1) + 2) = 0xFF;
    *(volatile u16 *)SPU2_VMIXEL(1) = 0xFFFF;
    *(volatile u16 *)(SPU2_VMIXEL(1) + 2) = 0xFF;
    *(volatile u16 *)SPU2_VMIXER(1) = 0xFFFF;
    *(volatile u16 *)(SPU2_VMIXER(1) + 2) = 0xFF;
    *(volatile u16 *)SPU2_MMIX(0) = 0xFF0;
    *(volatile u16 *)SPU2_MMIX(1) = 0xFFC;
    if (keepEffect == 0) {
        *(volatile u16 *)SPU2_MVOLL(0) = 0;
        *(volatile u16 *)SPU2_MVOLR(0) = 0;
        *(volatile u16 *)SPU2_MVOLL(1) = 0;
        *(volatile u16 *)SPU2_MVOLR(1) = 0;
        *(volatile u16 *)SPU2_EVOLL(0) = 0;
        *(volatile u16 *)SPU2_EVOLR(0) = 0;
        *(volatile u16 *)SPU2_EVOLL(1) = 0;
        *(volatile u16 *)SPU2_EVOLR(1) = 0;
        *(volatile u16 *)SPU2_EEA(0) = 0xE;
        *(volatile u16 *)SPU2_EEA(1) = 0xF;
    }
    *(volatile u16 *)SPU2_AVOLL(0) = 0;
    *(volatile u16 *)SPU2_AVOLR(0) = 0;
    *(volatile u16 *)SPU2_AVOLL(1) = 0x7FFF;
    *(volatile u16 *)SPU2_AVOLR(1) = 0x7FFF;
    *(volatile u16 *)SPU2_BVOLL(0) = 0;
    *(volatile u16 *)SPU2_BVOLR(0) = 0;
    *(volatile u16 *)SPU2_BVOLL(1) = 0;
    *(volatile u16 *)SPU2_BVOLR(1) = 0;
}
/* ENDF sceSdInitCoreMix */
INCLUDE_ASM("LIBSD", libsd_sceSdVoiceTrans);
ALIGN_TEXT(4);
INCLUDE_ASM("LIBSD", libsd_sceSdVoiceTransStatus);
/* FUNC libsd_sceSdStopTrans */
s32 libsd_sceSdStopTrans(s32 chan)
{
    volatile s32 sync;
    s32 core = chan & 1;

    sceSdTransIntrArg[core].op = core;
    sync = sceSdTransIntrArg[core].op;
    return sceSdTransferOp((core << 4) | 0xA);
}
/* ENDF libsd_sceSdStopTrans */
ALIGN_TEXT(4);
INCLUDE_ASM("LIBSD", libsd_sceSdBlockTrans);
/* FUNC libsd_sceSdBlockTransStatus */
s32 libsd_sceSdBlockTransStatus(s16 chan, s16 flag)
{
    return sceSdBlockTransPoll(chan);
}
/* ENDF libsd_sceSdBlockTransStatus */
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
/* FUNC sceSdResetCores */
#ifdef NON_MATCHING
/* Two short in the timeout loop. Retail threads the latch increment into
 * both exit branches' delay slots and undoes it on the fall-through; gcc
 * keeps the increment in the body and fills the slots with the bound test
 * instead. */
s32 sceSdResetCores()
{
    u32 i;

    *(volatile u16 *)SPU2_SPDIF_OUT = 0;
    sceSdDelay(2);
    *(volatile u16 *)SPU2_SPDIF_OUT = 0x8000;
    sceSdDelay(1);
    *(volatile u16 *)SPU2_MVOLL(0) = 0;
    *(volatile u16 *)SPU2_MVOLR(0) = 0;
    *(volatile u16 *)SPU2_MVOLL(1) = 0;
    *(volatile u16 *)SPU2_MVOLR(1) = 0;
    *(volatile u16 *)SPU2_ADMAS(0) = 0;
    *(volatile u16 *)SPU2_ADMAS(1) = 0;
    *(volatile u16 *)SPU2_ATTR(0) = 0;
    *(volatile u16 *)SPU2_ATTR(1) = 0;
    sceSdDelay(1);
    *(volatile u16 *)SPU2_ATTR(0) = 0x8000;
    *(volatile u16 *)SPU2_ATTR(1) = 0x8000;
    *(volatile u16 *)SPU2_MVOLL(0) = 0;
    *(volatile u16 *)SPU2_MVOLR(0) = 0;
    *(volatile u16 *)SPU2_MVOLL(1) = 0;
    *(volatile u16 *)SPU2_MVOLR(1) = 0;
    i = 0;
    while ((*(volatile u16 *)SPU2_STATX(0) & 0x7FF) != 0
           || (*(volatile u16 *)SPU2_STATX(1) & 0x7FF) != 0) {
        if (i >= 0xF01)
            break;
        sceSdDelay(1);
        i++;
    }
    *(volatile u16 *)SPU2_KOFF(0) = 0xFFFF;
    *(volatile u16 *)(SPU2_KOFF(0) + 2) = 0xFF;
    *(volatile u16 *)SPU2_KOFF(1) = 0xFFFF;
    *(volatile u16 *)(SPU2_KOFF(1) + 2) = 0xFF;
    *(volatile u16 *)SPU2_PMON(0) = 0;
    *(volatile u16 *)(SPU2_PMON(0) + 2) = 0;
    *(volatile u16 *)SPU2_PMON(1) = 0;
    *(volatile u16 *)(SPU2_PMON(1) + 2) = 0;
    *(volatile u16 *)SPU2_NON(0) = 0;
    *(volatile u16 *)(SPU2_NON(0) + 2) = 0;
    *(volatile u16 *)SPU2_NON(1) = 0;
    *(volatile u16 *)(SPU2_NON(1) + 2) = 0;
    return 0;
}
#else
INCLUDE_ASM("LIBSD", sceSdResetCores);
#endif
/* ENDF sceSdResetCores */
/* FUNC sceSdSetDmaModeWrite */
u32 sceSdSetDmaModeWrite(s32 core)
{
    volatile u32 *reg = (volatile u32 *)(IOP_SPU_DELAY + (core << 10));

    *reg = *reg & 0xF0FFFFFF;
}
/* ENDF sceSdSetDmaModeWrite */
/* FUNC sceSdSetDmaModeRead */
u32 sceSdSetDmaModeRead(s32 core)
{
    volatile u32 *reg = (volatile u32 *)(IOP_SPU_DELAY + (core << 10));

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
/* FUNC sceSdTransferOp */
#ifdef NON_MATCHING
/* Cannot match while .rodata is assembly: the 20-way dispatch compiles to a
 * jump table whose case labels the assembly .rodata already defines, so a C
 * body leaves them undefined and the link fails. */
/* Old-style on purpose: the op word says how many arguments are really
 * passed, and a prototyped definition would reject the one-argument calls. */
s32 sceSdTransferOp(op, addr, size)
s32 op;
u32 addr;
u32 size;
{
    s32 core = op >> 4;
    s32 v = core << 9;
    volatile u32 *chcr = (volatile u32 *)(core * 0x440 + 0xBF8010C8);
    volatile u16 *attr = (volatile u16 *)(SPU2_ATTR(0) + (v << 1));
    volatile u16 *admaStat = (volatile u16 *)(SPU2_ADMAS(0) + (v << 1));
    volatile u16 sync;
    s32 ret = 0;
    s32 state;
    u32 i;

    switch (op & 0xF) {
    case 2:
        addr = (addr >> 1) & ~7;
        *(volatile u16 *)((SPU2_TSA(0) + 2) + (v << 1)) = addr;
        *(volatile u16 *)(SPU2_TSA(0) + (v << 1)) = addr >> 16;
        sync = *(volatile u16 *)(SPU2_TSA(0) + (v << 1));
        break;
    case 4:
        if ((*attr & 0x30) != 0 || (*chcr & 0x1000000) != 0 || *admaStat != 0)
            ret = -1;
        break;
    case 5:
    case 6: {
        s32 blocks;

        intrman_CpuSuspendIntr(&state);
        if ((op & 0xF) == 6)
            *attr = (*attr & 0xFFCF) | 0x20;
        else
            *attr = *attr | 0x30;
        intrman_CpuResumeIntr(state);
        if ((op & 0xF) == 6)
            sceSdSetDmaModeWrite(core);
        else
            sceSdSetDmaModeRead(core);
        blocks = (size >> 6) + ((size & 0x3F) != 0);
        *(volatile u32 *)(core * 0x440 + 0xBF8010C0) = addr;
        *(volatile u16 *)(core * 0x440 + 0xBF8010C4) = 0x10;
        *(volatile u16 *)(core * 0x440 + 0xBF8010C6) = blocks;
        *chcr = ((op & 0xF) == 6) ? 0x1000201 : 0x1000200;
        sync = *chcr;
        ret = blocks << 6;
        break;
    }
    case 10: {
        u32 pos = 0;
        s32 half = 0;
        s32 stopped;

        if ((*chcr & 0x1000000) != 0) {
            volatile u32 *madr = (volatile u32 *)(core * 0x440 + 0xBF8010C0);
            s32 half1 = sceSdBlockHalf[core];
            u32 pos1 = *madr;

            half = sceSdBlockHalf[core];
            pos = *madr;
            if (half1 == half && pos1 != pos)
                pos = pos1;
            *chcr = *chcr & 0xFEFFFFFF;
            if ((*attr & 0x30) != 0) {
                volatile u16 *dmaStat = (volatile u16 *)(SPU2_STATX(0) + (v << 1));

                i = 0;
                while ((*dmaStat & 0x80) == 0) {
                    if (++i > 0x1000000)
                        break;
                }
            }
        }
        if ((*attr & 0x30) != 0) {
            intrman_CpuSuspendIntr(&state);
            *attr = *attr & 0xFFCF;
            intrman_CpuResumeIntr(state);
            i = 0;
            while ((*attr & 0x30) != 0) {
                if (++i > 0xF00)
                    break;
            }
        }
        stopped = 0;
        if ((*admaStat & 7) != 0) {
            stopped = 1;
            *admaStat = 0;
        }
        if (intrman_QueryIntrContext() != 0)
            thevent_iSetEventFlag(sceSdTransEventFlag[core], 1);
        else
            thevent_SetEventFlag(sceSdTransEventFlag[core], 1);
        sceSdTransDone[core] = 0;
        sceSdTransIdle[core] = 1;
        sceSdCleanStepCallback[core] = 0;
        sync = sceSdTransIdle[core];
        if (pos != 0 && stopped != 0)
            ret = (pos & 0xFFFFFF) | (half << 24);
        break;
    }
    }
    return ret;
}
#else
INCLUDE_ASM("LIBSD", sceSdTransferOp);
#endif
/* ENDF sceSdTransferOp */
INCLUDE_ASM("LIBSD", sceSdVoiceTransIo);
INCLUDE_ASM("LIBSD", sceSdDmaIntrHandler);
INCLUDE_ASM("LIBSD", sceSdBlockTransPoll);
INCLUDE_ASM("LIBSD", sceSdBlockTransWrite);
INCLUDE_ASM("LIBSD", sceSdBlockTransRead);
INCLUDE_ASM("LIBSD", sceSdSifDmaToEe);
/* FUNC libsd_sceSdProcBatch */
#ifdef NON_MATCHING
/* The dispatch compiles to a jump table whose case labels the assembly
 * .rodata already defines, so a C body breaks the link. */
s32 libsd_sceSdProcBatch(u32 *cmds, u32 *rets, u32 count)
{
    volatile s32 flush;
    sceSdBatch_t *b = (sceSdBatch_t *)cmds;
    u32 *out = rets;
    s32 err = -1;
    u32 i;

    for (i = 0; i < count; i++) {
        u32 value = 0;

        switch (b->func) {
        case 1:
            libsd_sceSdSetParam(b->entry, b->value);
            break;
        case 2:
            libsd_sceSdSetSwitch(b->entry, b->value);
            break;
        case 3:
            libsd_sceSdSetAddr(b->entry, b->value);
            break;
        case 4:
            libsd_sceSdSetCoreAttr(b->entry, b->value);
            break;
        case 5:
            *(u32 *)b->value = b->entry;
            break;
        case 6:
            sceSdBatchSifWord = b->entry;
            value = sceSdSifDmaToEe((void *)b->value, &sceSdBatchSifWord, 4);
            break;
        case 7:
            value = sceSdSifDmaToEe((void *)b->value, rets, b->entry);
            break;
        case 0x11:
            value = libsd_sceSdGetParam(b->entry);
            break;
        case 0x12:
            value = libsd_sceSdGetSwitch(b->entry);
            break;
        case 0x13:
            value = libsd_sceSdGetAddr(b->entry);
            break;
        case 0x14:
            value = libsd_sceSdGetCoreAttr(b->entry);
            break;
        default:
            flush = 0;
            return err;
        }
        if (rets != 0)
            *out = value;
        out++;
        err--;
        b++;
    }
    flush = 0;
    return i;
}
#else
INCLUDE_ASM("LIBSD", libsd_sceSdProcBatch);
#endif
/* ENDF libsd_sceSdProcBatch */
/* FUNC libsd_sceSdProcBatchEx */
#ifdef NON_MATCHING
/* The dispatch compiles to a jump table whose case labels the assembly
 * .rodata already defines, so a C body breaks the link. */
s32 libsd_sceSdProcBatchEx(u32 *cmds, u32 *rets, u32 count, u32 voices)
{
    volatile s32 flush;
    sceSdBatch_t *b = (sceSdBatch_t *)cmds;
    u32 written = 0;
    u32 i;
    s32 v;

    for (i = 0; i < count; i++) {
        u32 value = 0;
        u32 *out;

        switch (b->func) {
        case 1:
            if ((b->entry & 0x3E) != 0x3E) {
                libsd_sceSdSetParam(b->entry, b->value);
                break;
            }
            for (v = 0; v < 24; v++)
                if (voices & (1 << v)) {
                    written++;
                    libsd_sceSdSetParam((b->entry & 0xFFC1) | (v << 1), b->value);
                }
            written--;
            break;
        case 2:
            libsd_sceSdSetSwitch(b->entry, b->value);
            break;
        case 3:
            /* The address setters hold the voice in six bits, not five. */
            if ((*(u32 *)b & 0x7E0000) != 0x7E0000) {
                libsd_sceSdSetAddr(b->entry, b->value);
                break;
            }
            for (v = 0; v < 24; v++)
                if (voices & (1 << v)) {
                    written++;
                    libsd_sceSdSetAddr((b->entry & 0xFFC1) | (v << 1), b->value);
                }
            written--;
            break;
        case 4:
            libsd_sceSdSetCoreAttr(b->entry, b->value);
            break;
        case 5:
            *(u32 *)b->value = b->entry;
            break;
        case 6:
            sceSdBatchSifWord = b->entry;
            value = sceSdSifDmaToEe((void *)b->value, &sceSdBatchSifWord, 4);
            break;
        case 7:
            value = sceSdSifDmaToEe((void *)b->value, rets, b->entry);
            break;
        case 0x11:
            if ((b->entry & 0x3E) != 0x3E) {
                value = libsd_sceSdGetParam(b->entry);
                break;
            }
            /* One word per voice whatever the mask: an unselected slot keeps
             * the previous selected voice's value. */
            out = rets + written;
            for (v = 0; v < 24; v++) {
                if (voices & (1 << v))
                    value = libsd_sceSdGetParam((b->entry & 0xFFC1) | (v << 1));
                if (rets != 0)
                    *out = value;
                out++;
                written++;
            }
            written--;
            break;
        case 0x12:
            value = libsd_sceSdGetSwitch(b->entry);
            break;
        case 0x13:
            if ((*(u32 *)b & 0x7E0000) != 0x7E0000) {
                value = libsd_sceSdGetAddr(b->entry);
                break;
            }
            /* Unlike the wildcard GetParam, this one writes only the
             * selected voices. */
            out = rets + written;
            for (v = 0; v < 24; v++)
                if (voices & (1 << v)) {
                    value = libsd_sceSdGetAddr((b->entry & 0xFFC1) | (v << 1));
                    if (rets != 0)
                        *out = value;
                    out++;
                    written++;
                }
            written--;
            break;
        case 0x14:
            value = libsd_sceSdGetCoreAttr(b->entry);
            break;
        default:
            flush = 0;
            return ~i;
        }
        if (rets != 0)
            rets[written] = value;
        b++;
        written++;
    }
    flush = 0;
    return written;
}
#else
INCLUDE_ASM("LIBSD", libsd_sceSdProcBatchEx);
#endif
/* ENDF libsd_sceSdProcBatchEx */
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
/* FUNC libsd_sceSdNote2Pitch */
#ifdef NON_MATCHING
/* Two short, both register copies the retail allocator made and gcc does
 * not: center out of $4 to free it for the division magic, and a second
 * pseudo for the quotient. */
u16 libsd_sceSdNote2Pitch(u16 center, u16 fine, u16 note, s16 bend)
{
    s32 b;
    s32 q;
    s32 r;
    s32 x;
    s32 oct;
    s32 p;
    s16 st;
    s16 sh;

    b = bend + fine;
    q = b / 128;
    x = (s16)(note + q - center);
    oct = x / 12;
    sh = oct - 2;
    r = b % 128;
    st = x - oct * 12;
    if (st < 0 || (st == 0 && r < 0)) {
        st += 12;
        sh = oct - 3;
    }
    if (r < 0) {
        st--;
        st += q;
        r += (q + 1) << 7;
    }
    p = (sceSdPitchSemitone[st] * sceSdPitchFine[r]) >> 16;
    if (sh < 0) {
        s32 n = -sh;

        p += 1 << (n - 1);
        p = (u32)p >> n;
    }
    return p;
}
#else
INCLUDE_ASM("LIBSD", libsd_sceSdNote2Pitch);
#endif
/* ENDF libsd_sceSdNote2Pitch */
/* FUNC libsd_sceSdPitch2Note */
#ifdef NON_MATCHING
/* Two short. iop.h declares center and fine u16, so gcc masks center on its
 * first use and spills fine to a second register; retail's own translation
 * unit had them int-width and needs neither. */
u16 libsd_sceSdPitch2Note(u16 center, u16 fine, u16 pitch)
{
    u16 norm;
    u16 frac;
    u16 f;
    s32 note;
    u16 res;
    s32 k;
    s32 fi = 0;
    s32 semi = 0;
    s32 msb = 0;

    if (pitch >= 0x4000)
        pitch = 0x3FFF;
    for (k = 0; k < 14; k++)
        if ((pitch >> k) & 1)
            msb = k;
    norm = pitch << (15 - msb);
    for (k = 11; k >= 0; k--)
        if (norm >= sceSdPitchSemitone[k]) {
            semi = k;
            break;
        }
    frac = ((u32)norm << 15) / sceSdPitchSemitone[(u16)semi];
    for (k = 127; k >= 0; k--)
        if (frac >= sceSdPitchFine[k]) {
            fi = k;
            break;
        }
    f = fine + (fi + 1);
    note = center + (msb - 12) * 12 + semi + (f >> 7);
    res = (f & 0x7E) + (note << 8);
    return res & 0xFFFE;
}
#else
INCLUDE_ASM("LIBSD", libsd_sceSdPitch2Note);
#endif
/* ENDF libsd_sceSdPitch2Note */
/* FUNC sceSdSetSpdifMode */
#ifdef NON_MATCHING
/* Retail's length and instructions, but the prologue block comes out
 * permuted: gcc's scheduler pairs each register address with its own load
 * where retail builds both addresses first. */
s32 sceSdSetSpdifMode(s32 mode)
{
    volatile s32 sync;
    volatile u16 *attrReg = (volatile u16 *)SPU2_SPDIF_OUT;
    volatile u16 *ctrlReg = (volatile u16 *)SPU2_SPDIF_MODE;
    u16 reg0 = *attrReg;
    u16 reg1 = *ctrlReg;
    u16 first = reg0 & 0xFE57;
    u16 second = reg1 & 0xFFFD;
    u16 a = first;
    u16 b = second;

    switch (mode & 0xF) {
    case 0:
        a = first | 0x20;
        break;
    case 1:
        a = first | 0x100;
        b = second | 2;
        break;
    case 2:
        break;
    case 3:
        a = first | 0x100;
        break;
    default:
        return -100;
    }
    if (mode & 0x80)
        b |= 0x8000;
    else
        b &= 0x7FFF;
    if ((mode & 0xF00) == 0x800) {
        *(volatile u16 *)SPU2_SPDIF_MEDIA = 0x200;
        b = (b & 0xC0FF) | 0x1900;
    } else if ((mode & 0xF00) == 0x400) {
        *(volatile u16 *)SPU2_SPDIF_MEDIA = 0;
        b = (b & 0xC0FF) | 0x100;
    } else {
        *(volatile u16 *)SPU2_SPDIF_MEDIA = 0x200;
        b = (b & 0xC0FF) | 0x900;
    }
    *(volatile u16 *)SPU2_SPDIF_OUT = a;
    *(volatile u16 *)SPU2_SPDIF_MODE = b;
    sceSdSpdifMode = mode;
    sync = sceSdSpdifMode;
    return 0;
}
#else
INCLUDE_ASM("LIBSD", sceSdSetSpdifMode);
#endif
/* ENDF sceSdSetSpdifMode */
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
    u16 reg = *(volatile u16 *)SPU2_ATTR(entry & 1);

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
void *libsd_sceSdSetTransCallback(u16 chan, void *func)
{
    void *volatile sync;
    void *old = sceSdTransCallback[chan];

    sceSdTransCallback[chan] = func;
    sync = sceSdTransCallback[chan];
    return old;
}
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
/* FUNC sceSdIrqHandler */
#ifdef NON_MATCHING
/* Two short. Retail reloads sceSdSpu2IntrHandler for the call after testing
 * it; gcc's CSE keeps the tested value live. */
s32 sceSdIrqHandler()
{
    s32 flags;

    if (sceSdSpu2IntrHandler != 0 || sceSdIrqCallback != 0) {
        flags = (*(volatile u16 *)0xBF9007C2 & 0xC) >> 2;
        while (flags != 0) {
            if (flags & 1)
                *(volatile u16 *)SPU2_ATTR(0) &= 0xFFBF;
            if (flags & 2)
                *(volatile u16 *)SPU2_ATTR(1) &= 0xFFBF;
            if (sceSdSpu2IntrHandler != 0)
                ((void (*)(s32, void *))sceSdSpu2IntrHandler)(flags, sceSdSpu2IntrArg);
            else if (sceSdIrqCallback != 0)
                ((void (*)(s32))sceSdIrqCallback)(0);
            flags = (*(volatile u16 *)0xBF9007C2 & 0xC) >> 2;
        }
    }
    return 1;
}
#else
INCLUDE_ASM("LIBSD", sceSdIrqHandler);
#endif
/* ENDF sceSdIrqHandler */
/* FUNC sceSdResetVoices */
#ifdef NON_MATCHING
/* Retail's length and instructions, but two address pseudos land in other
 * registers: the lui half of SPU2_ATTR(0) survives the timeout loop in retail
 * and gcc rematerialises it in the loop's last delay slot. */
s32 sceSdResetVoices()
{
    volatile s32 i;
    volatile u16 *src = sceSdDummyBlock;
    volatile u16 *v0 = (volatile u16 *)0xBF900000;
    volatile u16 *v1 = (volatile u16 *)0xBF900400;
    volatile u16 *a0 = (volatile u16 *)SPU2_SSA(0);
    volatile u16 *a1 = (volatile u16 *)SPU2_SSA(1);
    volatile u16 *lo;
    volatile u16 *hi;
    volatile u16 *lo2;
    volatile u16 *hi2;

    *(volatile u16 *)SPU2_TSA(0) = 0;
    *(volatile u16 *)(SPU2_TSA(0) + 2) = 0x2800;
    for (i = 0; i < 8; i++)
        *(volatile u16 *)SPU2_DATA0(0) = *src++;
    *(volatile u16 *)SPU2_ATTR(0) = (*(volatile u16 *)SPU2_ATTR(0) & 0xFFCF) | 0x10;
    i = 0;
    while ((*(volatile u16 *)SPU2_STATX(0) & 0x400) != 0) {
        sceSdDelay(1);
        i++;
        if (i > 0x1000000)
            break;
    }
    *(volatile u16 *)SPU2_ATTR(0) = *(volatile u16 *)SPU2_ATTR(0) & 0xFFCF;
    for (i = 0; i < 0x18; i++) {
        *v1 = 0;
        *v0++ = *v1++;
        *v1 = 0;
        *v0++ = *v1++;
        *v1 = 0x3FFF;
        *v0++ = *v1++;
        *v1 = 0;
        *v0++ = *v1++;
        *v1 = 0;
        *v0 = *v1;
        v1 += 4;
        v0 += 4;
        *a1 = 0;
        *a0 = *a1++;
        *a1 = 0x2800;
        a0++;
        *a0 = *a1;
        a1 += 5;
        a0 += 5;
    }
    lo = (volatile u16 *)SPU2_KON(0);
    hi = (volatile u16 *)SPU2_KON(1);
    lo2 = (volatile u16 *)(SPU2_KON(0) + 2);
    hi2 = (volatile u16 *)(SPU2_KON(1) + 2);
    *hi = 0xFFFF;
    *lo = *hi;
    *hi2 = 0xFF;
    *lo2 = *hi2;
    sceSdDelay(3);
    lo = (volatile u16 *)SPU2_KOFF(0);
    hi = (volatile u16 *)SPU2_KOFF(1);
    lo2 = (volatile u16 *)(SPU2_KOFF(0) + 2);
    hi2 = (volatile u16 *)(SPU2_KOFF(1) + 2);
    *hi = 0xFFFF;
    *lo = *hi;
    *hi2 = 0xFF;
    *lo2 = *hi2;
    sceSdDelay(3);
    lo = (volatile u16 *)SPU2_ENDX(0);
    hi = (volatile u16 *)(SPU2_ENDX(0) + 2);
    *hi = 0;
    *lo = *hi;
}
#else
INCLUDE_ASM("LIBSD", sceSdResetVoices);
#endif
/* ENDF sceSdResetVoices */
/* FUNC sceSdInitState */
#ifdef NON_MATCHING
/* Retail's length, but three scheduling differences in the one long block of
 * global stores: gcc hoists the flag mask into a call delay slot, sinks the
 * sceSdSpu2IntrHandler store, and keeps one copy of the trailing flag = 0
 * where retail has it in both arms. */
s32 sceSdInitState(s32 flag)
{
    s32 res;
    s32 param[3];
    s32 *ev;

    intrman_DisableIntr(0x24, &res);
    intrman_DisableIntr(0x28, &res);
    intrman_DisableIntr(9, &res);
    intrman_ReleaseIntrHandler(0x24);
    intrman_ReleaseIntrHandler(0x28);
    intrman_ReleaseIntrHandler(9);
    sceSdTransByIo[0] = 0;
    sceSdTransByIo[1] = 0;
    sceSdTransIdle[0] = 1;
    sceSdTransIdle[1] = 1;
    sceSdTransDone[0] = 0;
    sceSdTransDone[1] = 0;
    sceSdSpu2IntrHandler = 0;
    sceSdTransIntrHandler[0] = 0;
    sceSdTransIntrHandler[1] = 0;
    sceSdCleanStepCallback[0] = 0;
    sceSdCleanStepCallback[1] = 0;
    sceSdTransIntrArg[0].op = 0;
    sceSdTransIntrArg[1].op = 1;
    sceSdTransIntrArg[0].arg = 0;
    sceSdTransIntrArg[1].arg = 0;
    sceSdSpu2IntrArg = 0;
    sceSdIrqCallback = 0;
    sceSdTransCallback[0] = 0;
    sceSdTransCallback[1] = 0;
    flag &= 0xF;
    if (flag == 0) {
        sysclib_bzero(sceSdEffectAttr, 0x28);
        sceSdEffectAreaStart[0] = 0x1DFFF0;
        sceSdEffectAreaStart[1] = 0x1FFFF0;
        *(volatile u16 *)SPU2_ESA(0) = 0xE;
        *(volatile u16 *)(SPU2_ESA(0) + 2) = 0xFFF8;
        *(volatile u16 *)SPU2_ESA(1) = 0xF;
        *(volatile u16 *)(SPU2_ESA(1) + 2) = 0xFFF8;
    }
    flag = 0;
    param[0] = 2;
    param[1] = 0;
    param[2] = 1;
    ev = &sceSdTransEventFlag[0];
    if (*ev > 0) {
        if (intrman_QueryIntrContext() != 0)
            thevent_iSetEventFlag(*ev, 1);
        else
            thevent_SetEventFlag(*ev, 1);
    } else {
        *ev = thevent_CreateEventFlag(param);
        if (*(volatile s32 *)ev <= 0)
            flag = -301;
    }
    ev = &sceSdTransEventFlag[1];
    if (*ev > 0) {
        if (intrman_QueryIntrContext() != 0)
            thevent_iSetEventFlag(*ev, 1);
        else
            thevent_SetEventFlag(*ev, 1);
    } else {
        *ev = thevent_CreateEventFlag(param);
        if (*(volatile s32 *)ev <= 0)
            flag = -301;
    }
    return flag;
}
#else
INCLUDE_ASM("LIBSD", sceSdInitState);
#endif
/* ENDF sceSdInitState */
/* FUNC sceSdClearGlobals */
void sceSdClearGlobals()
{
    sceSdInitialized = 0;
    sceSdTransEventFlag[0] = 0;
    sceSdTransEventFlag[1] = 0;
}
/* ENDF sceSdClearGlobals */
/* FUNC libsd_sceSdInit */
#ifdef NON_MATCHING
/* One long. gas will not move the second half of the sw sceSdInitialized
 * macro into the following jal's delay slot, where retail has it. */
s32 libsd_sceSdInit(s32 flag)
{
    s32 keep;
    s32 res;

    sceSdInitHardware();
    keep = flag & 0xF;
    if (keep == 0)
        sceSdResetCores(0);
    res = sceSdInitState(flag);
    sceSdResetVoices();
    sceSdInitCoreMix(keep);
    intrman_EnableIntr(0x24);
    intrman_EnableIntr(0x28);
    intrman_EnableIntr(9);
    intrman_RegisterIntrHandler(0x24, 1, sceSdDmaIntrHandler,
                                sceSdTransIntrArg);
    intrman_RegisterIntrHandler(0x28, 1, sceSdDmaIntrHandler,
                                &sceSdTransIntrArg[1]);
    intrman_RegisterIntrHandler(9, 1, sceSdIrqHandler,
                                sceSdSpu2IntrArg);
    sceSdInitialized = 1;
    sceSdStubNoOp();
    return res;
}
#else
INCLUDE_ASM("LIBSD", libsd_sceSdInit);
#endif
/* ENDF libsd_sceSdInit */
/* FUNC libsd_sceSdQuit */
s32 libsd_sceSdQuit()
{
    s32 res;
    s32 *ev = sceSdTransEventFlag;

    sceSdTransferOp(0xA);
    sceSdTransferOp(0x1A);
    if (ev[0] > 0)
        thevent_DeleteEventFlag(*(volatile s32 *)&ev[0]);
    if (ev[1] > 0)
        thevent_DeleteEventFlag(*(volatile s32 *)&ev[1]);
    intrman_DisableIntr(0x28, &res);
    intrman_DisableIntr(0x24, &res);
    intrman_DisableIntr(9, &res);
    intrman_ReleaseIntrHandler(0x28);
    intrman_ReleaseIntrHandler(0x24);
    intrman_ReleaseIntrHandler(9);
    sceSdStubNoOp();
    return 0;
}
/* ENDF libsd_sceSdQuit */
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
