#ifndef IRX_SPU2_H
#define IRX_SPU2_H

/* SPU2 and IOP DMA register addresses, as LIBSD writes them.
 *
 * The per-register addresses are not guesses: LIBSD carries its own table of
 * all 41 of them in .rodata (sceSdRegAddrTbl, which the entry word indexes),
 * and these agree with it entry for entry.
 *
 * Two different core strides, which is the thing to get right. Everything in
 * a core's 0x000..0x3FF block repeats every 0x400, while the volume
 * coefficient block at 0x760 repeats every 40 bytes. The entry word's bit 7
 * is what selects between them.
 *
 * These name addresses only. The volatile cast stays at the use site, because
 * it is load-bearing: without it gcc folds the address into a load offset and
 * the function stops matching.
 */

#define SPU2_BASE           0xBF900000
#define SPU2_CORE_STRIDE    0x400
#define SPU2_COEF_STRIDE    40

/* Per voice, 16 bytes each: VOLL VOLR PITCH ADSR1 ADSR2 ENVX VOLXL VOLXR. */
#define SPU2_VOICE(core, v) (SPU2_BASE + ((core) << 10) + (v) * 16)

/* The core block. */
#define SPU2_PMON(core)     (0xBF900180 + ((core) << 10))
#define SPU2_NON(core)      (0xBF900184 + ((core) << 10))
#define SPU2_VMIXL(core)    (0xBF900188 + ((core) << 10))
#define SPU2_VMIXEL(core)   (0xBF90018C + ((core) << 10))
#define SPU2_VMIXR(core)    (0xBF900190 + ((core) << 10))
#define SPU2_VMIXER(core)   (0xBF900194 + ((core) << 10))
#define SPU2_MMIX(core)     (0xBF900198 + ((core) << 10))
#define SPU2_ATTR(core)     (0xBF90019A + ((core) << 10))
#define SPU2_IRQA(core)     (0xBF90019C + ((core) << 10))
#define SPU2_KON(core)      (0xBF9001A0 + ((core) << 10))
#define SPU2_KOFF(core)     (0xBF9001A4 + ((core) << 10))
#define SPU2_TSA(core)      (0xBF9001A8 + ((core) << 10))
#define SPU2_DATA0(core)    (0xBF9001AC + ((core) << 10))
#define SPU2_DATA1(core)    (0xBF9001AE + ((core) << 10))
#define SPU2_ADMAS(core)    (0xBF9001B0 + ((core) << 10))
#define SPU2_SSA(core)      (0xBF9001C0 + ((core) << 10))
#define SPU2_LSAX(core)     (0xBF9001C4 + ((core) << 10))
#define SPU2_NAX(core)      (0xBF9001C8 + ((core) << 10))
#define SPU2_ESA(core)      (0xBF9002E0 + ((core) << 10))
#define SPU2_EEA(core)      (0xBF90033C + ((core) << 10))
#define SPU2_ENDX(core)     (0xBF900340 + ((core) << 10))
#define SPU2_STATX(core)    (0xBF900344 + ((core) << 10))

/* The volume coefficients, stride 40 rather than 0x400. */
#define SPU2_MVOLL(core)    (0xBF900760 + (core) * SPU2_COEF_STRIDE)
#define SPU2_MVOLR(core)    (0xBF900762 + (core) * SPU2_COEF_STRIDE)
#define SPU2_EVOLL(core)    (0xBF900764 + (core) * SPU2_COEF_STRIDE)
#define SPU2_EVOLR(core)    (0xBF900766 + (core) * SPU2_COEF_STRIDE)
#define SPU2_AVOLL(core)    (0xBF900768 + (core) * SPU2_COEF_STRIDE)
#define SPU2_AVOLR(core)    (0xBF90076A + (core) * SPU2_COEF_STRIDE)
#define SPU2_BVOLL(core)    (0xBF90076C + (core) * SPU2_COEF_STRIDE)
#define SPU2_BVOLR(core)    (0xBF90076E + (core) * SPU2_COEF_STRIDE)
#define SPU2_MVOLXL(core)   (0xBF900770 + (core) * SPU2_COEF_STRIDE)
#define SPU2_MVOLXR(core)   (0xBF900772 + (core) * SPU2_COEF_STRIDE)

/* SPDIF, not per core. */
#define SPU2_SPDIF_OUT      0xBF9007C0
#define SPU2_SPDIF_MODE     0xBF9007C6
#define SPU2_SPDIF_MEDIA    0xBF9007C8
#define SPU2_SPDIF_PROTECT  0xBF9007CA

/* IOP side. sceSdInitHardware writes the two delay registers and enables the
 * two DMA channels the SPU2 uses, channel 4 in DPCR and channel 7 in DPCR2. */
#define IOP_SPU_DELAY       0xBF801014
#define IOP_SPU2_DELAY      0xBF801414
#define IOP_DPCR            0xBF8010F0
#define IOP_DPCR2           0xBF801570
#define IOP_DMA_CH4_ENABLE  0x80000
#define IOP_DMA_CH7_ENABLE  0x8

/* sceSdInitDma points the two SPU2 DMA channels at a register bank each. The
 * values written are 0xBF900000 and 0xBF900800; note 0x800 rather than the
 * 0x400 core stride, so these are not simply the two cores' register bases.
 * What the hardware calls them is unconfirmed. */
#define IOP_SPU2_DMA0_BANK  0xBF801404
#define IOP_SPU2_DMA1_BANK  0xBF80140C

#endif /* IRX_SPU2_H */
