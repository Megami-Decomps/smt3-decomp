# sdk/libmc

Twenty-one of the library's twenty-two `sceMc*` entry points match. Only
`sceMcSync` remains drafted. Most command-submission entry points follow this
template:

    if (PollSema(D_0052BB24) < 0) return -200;
    if (D_00599740.bound == 0) { SignalSema(D_0052BB24); return -100; }
    <fill the request at D_005997C0 or D_005997F0>
    ret = sceSifCallRpc(&D_00599740, <fno>, <mode>, &req, <size>, &D_0059AD00, 4, ...);
    if (ret == 0) D_0052BB20 = <fno>; else SignalSema(D_0052BB24);
    return ret;

Three details the template needs, each measured:

- **`SignalSema` is written out in both branches**, never hoisted above the
  `if`. `sceMcGetSlotMax` is 12 bytes shorter when it is hoisted.
- **The pointer-to-flag conversions are explicit `if`/`else`.** Writing
  `req.w[5] = type != NULL` costs `sceMcGetInfo` 52 bytes, because retail
  branches per flag.
- **The entry-info block is 4-byte aligned**, so the 0x40-byte copy in
  `sceMcSetFileInfo` is `ldl`/`ldr` pairs. Giving the struct `u64` members
  makes it 8-aligned and the copy becomes `ld`/`sd`, 76 bytes short.

`sceMcUdCheckNewCard`'s format string stays an extern rather than an inline
literal: the rodata chunk it opens also carries the float constants the next
unit needs, and inlining the literal orphans them, which shifts the image and
overflows a `$gp`-relative literal reference in a game unit.

## sceMcWrite

Matched: all 380 native instruction bytes, 95 of 95 words match. The following
four bytes are alignment and are not credited as function content.

The leading-byte arithmetic stays as two separate unsigned subtractions:

    addr_t top = (EE_ADDR(buf) - 1) & ~15;
    addr_t bot = EE_ADDR(buf) - 16;
    s32 head = top - bot;

This computes `(-address) mod 16`, from zero for an aligned buffer through 15.
Folding it into one expression changes the native instruction sequence. The
inline payload remains a named member rather than a cast base-plus-32 view,
which otherwise hoists its address out of the copy loop.

The exact reconstruction changes only ordinary assignment order. In the short
path, write inline size, zero bulk size, then null bulk buffer. In the long
path, write inline size, remaining bulk size, then bulk buffer. Qualified SDK
2.9 RTL and ready-list dumps show the latter scheduling bulk buffer, bulk size,
then inline size, with the native request-base allocation. Reversing only the
two long-path bulk assignments loses the complete-owner match. This explains
a compiler dependency, not historical source spelling; no hooked scheduler
comparator result is claimed. There is no constraint, binding, cached request
pointer, artificial dependency, new qualifier, type or compiler flag.

The 48-byte request has descriptor at +0, signed bulk size at +0xC, unsigned
inline size at +0x14, bulk pointer at +0x18 and sixteen prefix bytes at +0x20.
Other words stay untouched. For a valid nonnegative byte count, the short path
copies at most sixteen bytes; the long path's 0–15-byte prefix stays within
the supplied buffer. All header stores finish before source-byte reads, so no
invented source-disjointness assumption is needed, including aliases into the
request. The forward byte copy and its unsigned inline-size reload remain.
Negative counts retain native signed short-path selection and the unsigned
copy bound; ensuing out-of-bounds corruption is not defined C behavior or a
newly added safety guarantee. Zero count does not dereference the buffer.

Semaphore errors, the cache flush, all nine RPC arguments, success command
publication and failure release are unchanged. The direct `pbMCard.c` caller
declaration is aligned with the provider's `const void *` source parameter,
without changing the wrapper or its emitted code. This does not assert that
all higher-level memory-card wrapper declarations are consistent.

Both public and private complete-owner checks pass 27 functions with no
owned-data difference. Independent native/semantic review and strict
production, `SKIP_ASM`, and retained-C builds pass; ordinary versus dump output
agrees in complete assembly, the ELF header, allocated-section headers/data,
non-file symbols and relocations. All other emitted functions and owned data are preserved.

## sceMcSync

224 bytes, retail's size, with every instruction but two in place. The loop

    while (1) { done = 0; if (sceSifCheckStatRpc(&cd) == 0) break; mcDelayThread(60); }

puts `done = 0` in the branch delay slot where retail keeps a `nop`. Rewriting
it as `while (check() != 0) delay(60); done = 0;` is 12 bytes short, so the
`done = 0`-per-iteration shape is the right one and only its scheduling is
wrong.

## sceMcSetFileInfo (sdk/libmc, 0x1D8)

Matched: all 472 native instruction bytes. The 64-byte entry-info copy keeps
its truthful 4-byte alignment and native unaligned doubleword operations.

The incoming flags have a real lifetime from function entry, across the
semaphore poll and validation branches, to the masked request-field store.
A local binding retains that value in native callee-saved `$17`, separately
from the request cursor in `$16`. The ordinary draft coalesces those roles,
emits only 464 bytes and uses a 0x90-byte frame; this constraint restores the
native 0xA0-byte frame and complete instruction sequence. It is a localized
compiler reconstruction aid, not a claim about historical source spelling.
There is no empty asm operand, new memory access, helper or ABI change.

Ablating only the binding restores the 464-byte residual. Paired plain and
RTL-dump EE 2.9 builds have identical assembly, complete-owner text and
relocations for both baseline and final source. The final owner checks
26 functions exact with no differing owned data. The early -200, -100 and
-210 results, name validation, `flags & 7`, info copy, name termination,
cache flush, RPC arguments and semaphore release behavior are unchanged.
