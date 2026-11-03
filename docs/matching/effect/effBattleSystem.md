# Matching notes: effect/effBattleSystem

## effBTLSys_Hansya_Draw (effect/effBattleSystem, 0x2D0)

9 of 180 words match in the retained guarded draft. It emits 688 bytes
against the native 720-byte extent, leaving 32 native bytes uncovered.

`effBTLSys_Hansya_Draw` remains guarded and nonmatching. No qualifier cleanup or branch-local initialization control is retained in source. The three light pointers are ordinary CPU stack objects consumed by `sdf3DMakeLightParallelPacket`; they are not hardware-polled data. Removing their `volatile` qualifiers preserves the native-style twelve-byte aggregate copy and all emitted object contracts, but does not close the match.

The unpublished ordinary control emits 688 bytes against 720 native bytes. The 32-byte deficit decomposes into an 8-byte entry-gate layout difference, 16 bytes in the first skill-selection/initialization region, and two 4-byte address rematerializations. Qualified plain-versus-dump builds show CSE merging each real matrix/position store address with its later call argument: RTL instructions 243 and 368 disappear between jump and CSE, redirecting uses to 123 and 143. Retail materializes separate addresses. This establishes a concrete remaining compiler dependency, not the original source spelling or permission to add dummy traffic.

Declaration-time aggregate initialization adds an extra copy; local declaration ordering, selector locals and an early return remain nonmatching. Those are retained negative controls, not landing candidates. No padding, false volatility, fake barrier or forced register grid is justified.

At the investigation baseline, the guarded owner checks 290 match / 0 differ. Strict retained compilation succeeds; strict normal and SKIP_ASM modes expose pre-existing conditional declaration gaps. This historical result predates the declaration-only follow-up below. There is no new matching credit.

A further ordinary control initializes all 15 real record fields inside each mutually exclusive skill-kind arm and calls the providers after the join. It emits 696 bytes, still nonmatching. Both `2` definitions (RTL 86/153) and diffuse-low definitions (694/688) survive through `25.sched2`; `27.jump2` shares their identical suffix at label 726, leaving 153/688 after the join. Retail shares stores but retains those definitions per arm. Plain/dump output parity qualifies this late tail-sharing observation. This control remains diagnostic, not accepted source.

All 18 calls retain native order, including two skill-ID queries. Formation and part fields are reloaded across callback-dispatching wrappers; the entry frame snapshot gates initialization and rendering, and frame is refreshed after drawing before increment. Packet layout and all float literals remain unchanged. No source/header change is proposed by this report.

### Declaration visibility follow-up

A separate minimal patch removes only four NON_MATCHING guard pairs around
existing memcpy, dds3SetQuake, mdlGetColor and memset declarations. Their
types agree with the actual providers and native call sites; no signature,
body, fallback, header or ownership changes. The later identical mdlGetColor
declaration remains. Hansya and NameText body controls are not retained.

Baseline strict production and SKIP_ASM fail, while strict retained-C passes.
The final declaration visibility makes all three modes pass; full paired
object contracts and normalized assembly remain identical in each mode.
This repairs a compiler-check/source-declaration blocker, not a demonstrated
runtime bug or a blanket warning-free guarantee. Final owners remain 290/0 and 13/0. Full retail byte comparison/checksum/no-op,
450 tests, 817 layout assertions, fresh stable 141-owner/481-draft checks,
seven IRX checks and reconciled unchanged reports pass.
