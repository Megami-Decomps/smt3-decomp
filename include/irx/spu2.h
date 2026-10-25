#ifndef IRX_SPU2_H
#define IRX_SPU2_H

/* SPU2 and IOP DMA registers, as LIBSD writes them.
 *
 * The addresses are not guesses: LIBSD carries a table of all 41 of them in
 * .rodata (sceSdRegAddrTbl, which the entry word indexes), and these agree
 * with it entry for entry.
 *
 * SPU2_FOO(core) is the register, an lvalue with the volatile cast built in.
 * SPU2_FOO_HI(core) is its upper halfword, which is how the 24 bit masks and
 * the address pairs are written. SPU2_FOO_A(core) is the bare address.
 *
 * Which one to use is not a matter of taste. Where the retail code holds an
 * address in a register and loads at offset zero, the C has to say so, with
 * the _A form in a pointer or in arithmetic; inlining the address there lets
 * gcc fold the low half into the load offset and the function stops matching.
 * A pointer to a register is &SPU2_FOO(core). The volatile is load-bearing
 * for the same reason, which is why it lives in the macro.
 *
 * Two core strides: a core's 0x000..0x3FF block repeats every 0x400, the
 * volume coefficients at 0x760 repeat every 40 bytes, and the entry word's
 * bit 7 selects between them. The shift and the multiply are spelled the way
 * the retail code computes them.
 */

#define SPU2_BASE           0xBF900000
#define SPU2_CORE_STRIDE    0x400
#define SPU2_COEF_STRIDE    40

#define SPU2_VOICE_A(core, v) (SPU2_BASE + ((core) << 10) + (v) * 16)

/* The per core block, stride 0x400. */
#define SPU2_PMON_A(core)  (0xBF900180 + ((core) << 10))
#define SPU2_PMON(core)     (*(volatile u16 *)SPU2_PMON_A(core))
#define SPU2_PMON_HI(core)  (*(volatile u16 *)(SPU2_PMON_A(core) + 2))
#define SPU2_NON_A(core)  (0xBF900184 + ((core) << 10))
#define SPU2_NON(core)     (*(volatile u16 *)SPU2_NON_A(core))
#define SPU2_NON_HI(core)  (*(volatile u16 *)(SPU2_NON_A(core) + 2))
#define SPU2_VMIXL_A(core)  (0xBF900188 + ((core) << 10))
#define SPU2_VMIXL(core)     (*(volatile u16 *)SPU2_VMIXL_A(core))
#define SPU2_VMIXL_HI(core)  (*(volatile u16 *)(SPU2_VMIXL_A(core) + 2))
#define SPU2_VMIXEL_A(core)  (0xBF90018C + ((core) << 10))
#define SPU2_VMIXEL(core)     (*(volatile u16 *)SPU2_VMIXEL_A(core))
#define SPU2_VMIXEL_HI(core)  (*(volatile u16 *)(SPU2_VMIXEL_A(core) + 2))
#define SPU2_VMIXR_A(core)  (0xBF900190 + ((core) << 10))
#define SPU2_VMIXR(core)     (*(volatile u16 *)SPU2_VMIXR_A(core))
#define SPU2_VMIXR_HI(core)  (*(volatile u16 *)(SPU2_VMIXR_A(core) + 2))
#define SPU2_VMIXER_A(core)  (0xBF900194 + ((core) << 10))
#define SPU2_VMIXER(core)     (*(volatile u16 *)SPU2_VMIXER_A(core))
#define SPU2_VMIXER_HI(core)  (*(volatile u16 *)(SPU2_VMIXER_A(core) + 2))
#define SPU2_MMIX_A(core)  (0xBF900198 + ((core) << 10))
#define SPU2_MMIX(core)     (*(volatile u16 *)SPU2_MMIX_A(core))
#define SPU2_MMIX_HI(core)  (*(volatile u16 *)(SPU2_MMIX_A(core) + 2))
#define SPU2_ATTR_A(core)  (0xBF90019A + ((core) << 10))
#define SPU2_ATTR(core)     (*(volatile u16 *)SPU2_ATTR_A(core))
#define SPU2_ATTR_HI(core)  (*(volatile u16 *)(SPU2_ATTR_A(core) + 2))
#define SPU2_IRQA_A(core)  (0xBF90019C + ((core) << 10))
#define SPU2_IRQA(core)     (*(volatile u16 *)SPU2_IRQA_A(core))
#define SPU2_IRQA_HI(core)  (*(volatile u16 *)(SPU2_IRQA_A(core) + 2))
#define SPU2_KON_A(core)  (0xBF9001A0 + ((core) << 10))
#define SPU2_KON(core)     (*(volatile u16 *)SPU2_KON_A(core))
#define SPU2_KON_HI(core)  (*(volatile u16 *)(SPU2_KON_A(core) + 2))
#define SPU2_KOFF_A(core)  (0xBF9001A4 + ((core) << 10))
#define SPU2_KOFF(core)     (*(volatile u16 *)SPU2_KOFF_A(core))
#define SPU2_KOFF_HI(core)  (*(volatile u16 *)(SPU2_KOFF_A(core) + 2))
#define SPU2_TSA_A(core)  (0xBF9001A8 + ((core) << 10))
#define SPU2_TSA(core)     (*(volatile u16 *)SPU2_TSA_A(core))
#define SPU2_TSA_HI(core)  (*(volatile u16 *)(SPU2_TSA_A(core) + 2))
#define SPU2_DATA0_A(core)  (0xBF9001AC + ((core) << 10))
#define SPU2_DATA0(core)     (*(volatile u16 *)SPU2_DATA0_A(core))
#define SPU2_DATA0_HI(core)  (*(volatile u16 *)(SPU2_DATA0_A(core) + 2))
#define SPU2_DATA1_A(core)  (0xBF9001AE + ((core) << 10))
#define SPU2_DATA1(core)     (*(volatile u16 *)SPU2_DATA1_A(core))
#define SPU2_DATA1_HI(core)  (*(volatile u16 *)(SPU2_DATA1_A(core) + 2))
#define SPU2_ADMAS_A(core)  (0xBF9001B0 + ((core) << 10))
#define SPU2_ADMAS(core)     (*(volatile u16 *)SPU2_ADMAS_A(core))
#define SPU2_ADMAS_HI(core)  (*(volatile u16 *)(SPU2_ADMAS_A(core) + 2))
#define SPU2_SSA_A(core)  (0xBF9001C0 + ((core) << 10))
#define SPU2_SSA(core)     (*(volatile u16 *)SPU2_SSA_A(core))
#define SPU2_SSA_HI(core)  (*(volatile u16 *)(SPU2_SSA_A(core) + 2))
#define SPU2_LSAX_A(core)  (0xBF9001C4 + ((core) << 10))
#define SPU2_LSAX(core)     (*(volatile u16 *)SPU2_LSAX_A(core))
#define SPU2_LSAX_HI(core)  (*(volatile u16 *)(SPU2_LSAX_A(core) + 2))
#define SPU2_NAX_A(core)  (0xBF9001C8 + ((core) << 10))
#define SPU2_NAX(core)     (*(volatile u16 *)SPU2_NAX_A(core))
#define SPU2_NAX_HI(core)  (*(volatile u16 *)(SPU2_NAX_A(core) + 2))
#define SPU2_ESA_A(core)  (0xBF9002E0 + ((core) << 10))
#define SPU2_ESA(core)     (*(volatile u16 *)SPU2_ESA_A(core))
#define SPU2_ESA_HI(core)  (*(volatile u16 *)(SPU2_ESA_A(core) + 2))
#define SPU2_EEA_A(core)  (0xBF90033C + ((core) << 10))
#define SPU2_EEA(core)     (*(volatile u16 *)SPU2_EEA_A(core))
#define SPU2_EEA_HI(core)  (*(volatile u16 *)(SPU2_EEA_A(core) + 2))
#define SPU2_ENDX_A(core)  (0xBF900340 + ((core) << 10))
#define SPU2_ENDX(core)     (*(volatile u16 *)SPU2_ENDX_A(core))
#define SPU2_ENDX_HI(core)  (*(volatile u16 *)(SPU2_ENDX_A(core) + 2))
#define SPU2_STATX_A(core)  (0xBF900344 + ((core) << 10))
#define SPU2_STATX(core)     (*(volatile u16 *)SPU2_STATX_A(core))
#define SPU2_STATX_HI(core)  (*(volatile u16 *)(SPU2_STATX_A(core) + 2))

/* The volume coefficients, stride 40. */
#define SPU2_MVOLL_A(core)  (0xBF900760 + (core) * SPU2_COEF_STRIDE)
#define SPU2_MVOLL(core)     (*(volatile u16 *)SPU2_MVOLL_A(core))
#define SPU2_MVOLL_HI(core)  (*(volatile u16 *)(SPU2_MVOLL_A(core) + 2))
#define SPU2_MVOLR_A(core)  (0xBF900762 + (core) * SPU2_COEF_STRIDE)
#define SPU2_MVOLR(core)     (*(volatile u16 *)SPU2_MVOLR_A(core))
#define SPU2_MVOLR_HI(core)  (*(volatile u16 *)(SPU2_MVOLR_A(core) + 2))
#define SPU2_EVOLL_A(core)  (0xBF900764 + (core) * SPU2_COEF_STRIDE)
#define SPU2_EVOLL(core)     (*(volatile u16 *)SPU2_EVOLL_A(core))
#define SPU2_EVOLL_HI(core)  (*(volatile u16 *)(SPU2_EVOLL_A(core) + 2))
#define SPU2_EVOLR_A(core)  (0xBF900766 + (core) * SPU2_COEF_STRIDE)
#define SPU2_EVOLR(core)     (*(volatile u16 *)SPU2_EVOLR_A(core))
#define SPU2_EVOLR_HI(core)  (*(volatile u16 *)(SPU2_EVOLR_A(core) + 2))
#define SPU2_AVOLL_A(core)  (0xBF900768 + (core) * SPU2_COEF_STRIDE)
#define SPU2_AVOLL(core)     (*(volatile u16 *)SPU2_AVOLL_A(core))
#define SPU2_AVOLL_HI(core)  (*(volatile u16 *)(SPU2_AVOLL_A(core) + 2))
#define SPU2_AVOLR_A(core)  (0xBF90076A + (core) * SPU2_COEF_STRIDE)
#define SPU2_AVOLR(core)     (*(volatile u16 *)SPU2_AVOLR_A(core))
#define SPU2_AVOLR_HI(core)  (*(volatile u16 *)(SPU2_AVOLR_A(core) + 2))
#define SPU2_BVOLL_A(core)  (0xBF90076C + (core) * SPU2_COEF_STRIDE)
#define SPU2_BVOLL(core)     (*(volatile u16 *)SPU2_BVOLL_A(core))
#define SPU2_BVOLL_HI(core)  (*(volatile u16 *)(SPU2_BVOLL_A(core) + 2))
#define SPU2_BVOLR_A(core)  (0xBF90076E + (core) * SPU2_COEF_STRIDE)
#define SPU2_BVOLR(core)     (*(volatile u16 *)SPU2_BVOLR_A(core))
#define SPU2_BVOLR_HI(core)  (*(volatile u16 *)(SPU2_BVOLR_A(core) + 2))
#define SPU2_MVOLXL_A(core)  (0xBF900770 + (core) * SPU2_COEF_STRIDE)
#define SPU2_MVOLXL(core)     (*(volatile u16 *)SPU2_MVOLXL_A(core))
#define SPU2_MVOLXL_HI(core)  (*(volatile u16 *)(SPU2_MVOLXL_A(core) + 2))
#define SPU2_MVOLXR_A(core)  (0xBF900772 + (core) * SPU2_COEF_STRIDE)
#define SPU2_MVOLXR(core)     (*(volatile u16 *)SPU2_MVOLXR_A(core))
#define SPU2_MVOLXR_HI(core)  (*(volatile u16 *)(SPU2_MVOLXR_A(core) + 2))

/* Not per core. */
#define SPU2_SPDIF_OUT_A        0xBF9007C0
#define SPU2_SPDIF_OUT          (*(volatile u16 *)SPU2_SPDIF_OUT_A)
/* Read for two flag bits in bits 2..3; the name is from that use, not from
 * the register table, which does not cover the SPDIF block. */
#define SPU2_SPDIF_STAT_A        0xBF9007C2
#define SPU2_SPDIF_STAT          (*(volatile u16 *)SPU2_SPDIF_STAT_A)
#define SPU2_SPDIF_MODE_A        0xBF9007C6
#define SPU2_SPDIF_MODE          (*(volatile u16 *)SPU2_SPDIF_MODE_A)
#define SPU2_SPDIF_MEDIA_A        0xBF9007C8
#define SPU2_SPDIF_MEDIA          (*(volatile u16 *)SPU2_SPDIF_MEDIA_A)
#define SPU2_SPDIF_PROTECT_A        0xBF9007CA
#define SPU2_SPDIF_PROTECT          (*(volatile u16 *)SPU2_SPDIF_PROTECT_A)
#define IOP_SPU_DELAY_A        0xBF801014
#define IOP_SPU_DELAY          (*(volatile u32 *)IOP_SPU_DELAY_A)
#define IOP_SPU2_DELAY_A        0xBF801414
#define IOP_SPU2_DELAY          (*(volatile u32 *)IOP_SPU2_DELAY_A)
#define IOP_DPCR_A        0xBF8010F0
#define IOP_DPCR          (*(volatile u32 *)IOP_DPCR_A)
#define IOP_DPCR2_A        0xBF801570
#define IOP_DPCR2          (*(volatile u32 *)IOP_DPCR2_A)
#define IOP_SPU2_DMA0_BANK_A        0xBF801404
#define IOP_SPU2_DMA0_BANK          (*(volatile u32 *)IOP_SPU2_DMA0_BANK_A)
#define IOP_SPU2_DMA1_BANK_A        0xBF80140C
#define IOP_SPU2_DMA1_BANK          (*(volatile u32 *)IOP_SPU2_DMA1_BANK_A)
#define IOP_DMA_CH4_ENABLE  0x80000
#define IOP_DMA_CH7_ENABLE  0x8

#endif /* IRX_SPU2_H */
