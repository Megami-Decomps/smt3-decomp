# Compiling this tree's C for a 64-bit host

The decompilation is 32-bit MIPS C. A consumer that runs the engine natively on
an LP64 host has to get the same source past its own compiler first, and
`docs/pointer-assumptions.md` covers only the half of that problem that has a
cast in it. This document is the measured state of the whole problem, the hooks
added for it, and what is left.

`/home/wsl/projects/sdfport` is the worked precedent: it compiles 45 of 45
`src/smt3/sdf/` units for a **32-bit** host behind four force-included
replacement headers (`include/host/common.h`, `fpu.h`, `eeaddr.h`,
`ps2asm.h`) and `-DSKIP_ASM -DNON_MATCHING -w -Wno-int-conversion`. The
mechanism here is the same; only the target width differs, and the width is
what turns several of its silenced warnings into real defects.

## The probe

`tools/host64_probe.py` compiles every unit under `src/smt3/` on its own,
`-fsyntax-only`, no link. It is not part of the retail build and nothing in
`config/` references it. `tools/host64/{common,fpu,eeaddr}.h` are its host
views of the three headers that cannot survive a host compiler, force-included
ahead of the tree's own copies, because a quoted `#include "common.h"` from
inside `include/` finds its sibling before anything on `-I`.

Three modes, because they answer different questions.

`accept` asks whether an LP64 compiler takes the unit at all. gcc 14 makes the
two pointer/integer mismatch classes hard errors; this mode demotes exactly
those two and nothing else, since they are the decompilation's own typing and
`width` counts them on purpose.

```
python3 tools/host64_probe.py accept [--cc clang] [--list-failures]
```

`width` promotes the pointer-width diagnostics to errors. A unit can pass
`accept` and fail `width`: it compiles and is wrong.

```
python3 tools/host64_probe.py width
```

`trunc` asks what survives naming every cast. Once a site spells `EE_ADDR`
the cast no longer truncates, but the declaration the result lands in still
does, and only widening that declaration can fix it. The mode promotes
`-Wconversion` and counts the conversions whose source is an `EE_ADDR`
expansion.

```
python3 tools/host64_probe.py trunc
```

**A trap this measurement fell into once.** `-w` outranks
`-Werror=pointer-to-int-cast`. The first version of `width` carried `-w` from
sdfport's flag set and reported **zero** pointer-width casts in a tree that has
1470 of them. `width` now carries `-Wno-all -Wno-extra` instead, and the
classifier counts error lines only, not warnings, so a warning cannot be
attributed to a class that did not fail the unit.

**A second trap, in `trunc`.** The mode recognizes a truncated address by the
source type gcc names, so the host view's `ee_addr_t` spelling is part of the
instrument. A draft spelled it `unsigned long`, which is also how LP64 spells
`size_t` and the type of every `sizeof`, and reported 1120 sites where the
clean count is 1050: 70 were `sizeof` and `ps2Size_t` conversions with no
address in them. `unsigned long long` is the spelling that collides with
nothing, because gcc prints a cast's result as its underlying type and a
declared variable as its typedef name plus `{aka ...}`, so anchoring on
`from '<type>' to` also rejects `u64` variables. Both exclusions were checked
against a file holding all four cases at once, not reasoned about.

Each measuring run starts by pointing the instrument at a site confirmed to
have the property and requiring a non-zero answer, because the clean result is
the one nobody re-examines.

## Before

Measured on `9d0a5ce0`, the commit this work starts from, with the probe as it
stands now (a detached worktree at that commit with `tools/host64*` copied in,
so the command is identical).

| Command | Units compiling |
| --- | --- |
| `python3 tools/host64_probe.py accept` | 144 / 309 |
| `python3 tools/host64_probe.py accept --cc clang` | 101 / 309 |
| `python3 tools/host64_probe.py width` | 70 / 309 |

`accept` error distribution at that commit, 211 errors over 165 units, by
`grep -o 'error: .*'` over the per-unit stderr:

| Errors | Class |
| --- | --- |
| 158 | `size of array ... is negative`: a struct size assertion whose struct holds a pointer |
| 49 | implicit declaration of a function no header declares |
| 2 | `return` with no value in a non-void function (`sdk/libgcc.c`) |
| 1 | `redefinition of ffabsf` |
| 1 | conflicting types for `sdfPackQuad` |

Three assertions account for all 158: `dds3GlobalWork_t`, `sifRpcDataQueue_t`
and `sifRpcServerData_t`. The other seven in headers hold at LP64 as written.

## After

Current branch head, same commands.

| Command | Units compiling | At `018a0927` | Was |
| --- | --- | --- | --- |
| `python3 tools/host64_probe.py accept` | **307 / 309** | 307 | 144 |
| `python3 tools/host64_probe.py accept --cc clang` | **225 / 309** | 225 | 101 |
| `python3 tools/host64_probe.py width` | **252 / 309** | 141 | 70 |

`width` moved from 141 to 252 with the cast sweep below. `accept` did not move,
because a pointer-width cast is well-formed C: a unit that narrows an address
compiles, it is just wrong.

clang stays well behind gcc for one reason that is not this tree's C: clang
validates inline-asm register names under `-fsyntax-only` and gcc does not, so
70 units stop at `include/ps2asm.h`'s `$2`. sdfport's `include/host/ps2asm.h`
is the answer to that and no host view of it was written here.

## The hooks

Each is `#ifndef`-guarded where it is a macro, with a default expansion that
emits exactly what the open-coded code it replaced emitted.

### `PS2_SIZE_CHECK(name, type, size)`, `include/common.h`

```c
#define PS2_SIZE_CHECK(name, type, size) \
    typedef char name[(sizeof(void *) != 4 || sizeof(type) == (size)) ? 1 : -1]
```

The negative-array-size trick ee-gcc 2.96 accepts, with the check scoped to
targets whose pointer is four bytes. A wider pointer moves every member after
one, so the retail size is the wrong question there and the members' own types
stay the record of the layout. No host redefinition is needed: the first term
makes it vacuous at LP64, and on the EE the expansion is what was there before.
A typedef emits nothing, so this cannot move codegen.

23 sites: 10 in `include/` (`camp.h`, `facility.h` x3, `fclRag.h` x2,
`newdata.h` x2, `sif_rpc.h` x2) and 13 in `src/smt3/` (`code_0029B780.c`,
`interface/frFont.c`, `newbattle/NbNegoFlowInnen.c` x3, `sdf/sdfPrint.c` x2,
`sdf/sdfTmx.c`, `sdk/libgcc.c` x2, `sdk/libkernl.c` x3).

### `ps2Size_t`, `include/common.h`

```c
typedef __SIZE_TYPE__ ps2Size_t;
```

The EE's `size_t`. ee-gcc is invoked with `-D__SIZE_TYPE__=unsigned int`
(see the `cc` rule in `build.ninja`), so this is a 32-bit unsigned there and
the host's own `size_t` here. It is not a macro because it needs no override:
each compiler defines `__SIZE_TYPE__` for its own target.

This is the one width in the tree that differs between targets with no cast
anywhere to mark it. 152 files declared `memset`, `memcpy`, `memmove`,
`memcmp`, `strncpy`, `strncmp` or `malloc` with a `u32` length, 243 sites, and
gcc reports every one as `-Wbuiltin-declaration-mismatch` against the builtin's
`unsigned long`. 231 sites now spell `ps2Size_t` (230 declarations plus
`sdk/libc.c`'s `malloc` definition); the remaining 30 are listed under open
items.

Note that `unsigned long` would have been the wrong fix: ee-gcc is invoked with
`-D__LONG_MAX__=9223372036854775807L`, so `long` is 64-bit on this EE target
and 64-bit on the host, which is exactly backwards from what `size_t` needs.

### `EE_ADDR` and `EE_ADDR_PTR` at sites with no cast

Already in `include/common.h` from the earlier sweep. 33 further sites now
carry them, in `code_0022C578.c` (17), `sdf/sdfPrim.c` (5), `sdf/sdfVu1.c` (5),
`facility/fclShare.c` (2), `kernel/dds3GlobalWork.c` (2),
`effect/billManager.c` (1) and `sdf/sdfSortOt.c` (1).

The earlier sweep drove off a cast-operand extractor, so by construction it
could only find sites that already spelled a cast. These spell none: a `void *`
assigned the `s32` that `sdfGrAllocPacketBuff` returns, a bare pointer passed to
`sdfDrawTagLink`'s `u32` second argument, a `u32` passed as its
`sdfDrawTag_t *` first argument. The implicit conversion is already what the
macro's default expansion does, so the target output does not move; the
difference is that a host now has a name to redefine. These are the five units
sdfport's `docs/reference/engine-compile.md` filed as upstream reports
(`sdf3DPrim.c`, `sdfPacketBuff.c`, `sdfPrim.c`, `sdfSortOt.c`, `sdfVu1.c`).

### `EE_ADDR_AS(type, addr)`, `include/common.h`

```c
#define EE_ADDR_AS(type, addr) ((type)(addr))
```

An EE address widened back to a typed pointer, which is what the widening half
of the cast population is. The default expansion is the cast that was there,
with parentheses around the operand and around the whole expression, so the
preprocessed token stream differs from the original only in parentheses.

That it is byte-identical at **every** covered site, rather than at the ones
somebody read, rests on two mechanical checks rather than on inspection.
`tools/host64_casts.py verify` un-expands every `EE_ADDR*` call in the working
tree and in `HEAD` to a fixed point and compares the whitespace-stripped
results, reporting each file as token-identical, differing in parentheses only,
or differing. Only the last is a defect: a parenthesis around a complete
unary-expression operand, which is all a cast binds to, and around a complete
cast expression cannot change the parse. The checksum then confirms the output
over the whole image. The converter is the reason the scope is right in the
first place: it finds the cast's operand by walking the unary-expression
grammar, prefix operators, calls, subscripts and member selection included, and
refuses the site rather than guessing when the walk does not settle. Wrapping
one token too many would turn a byte offset into pointer arithmetic, which the
checksum would catch but a reader would not.

### The cast sweep

`tools/host64_casts.py` finds the sites and rewrites them. It keys on gcc's
`-Werror=pointer-to-int-cast` and `-Werror=int-to-pointer-cast` diagnostics,
read as JSON, so the site is identified by its operand's type and never by the
cast's text: 412 of the narrowing sites are spelled `(s32)`, which is exactly
why the earlier `(u32)`-matching sweep reported none of them.

1470 sites at `9d0a5ce0`, 14 now. Narrowing casts became `EE_ADDR` or
`EE_ADDR_S`, keeping the signedness because the two differ on a host; widening
casts became `EE_ADDR_AS`. The converted sites are spread over the eleven
commits from `5a21da68`, grouped by directory so each batch could be gated on
its own, with `src/smt3/sdk/` last.

The 14 that remain are the two classes with no macro to go to, and they are
listed rather than counted because they are not a backlog:

- **Nine casts to a function or array pointer**: `(s32 (*)())` on a stored
  function address in `mc/slMain.c` (3), `(void (*)(void))`, `(u8 *(*)(s32))`
  and `(s32 (*)(s32, void *, void *))` on callbacks in `interface/itfMesManager.c`,
  `sdf/sdfSound.c`, `sdf/sdfGraph.c` and `sdf/sdfSystem.c`, and `(f32 (*)[4])`
  on a row-of-four array in `effect/effMagatuhi.c` (2). A declarator is not a
  type name followed by stars, so it cannot be an `EE_ADDR_AS` argument at all.
  A host needs a typedef for each, which is per-site work, not a macro.
- **Five `(u32)&((T *)0)->field` offsets** in `model/mdlManager.c`,
  `newbattle/NbNegoFlowOne.c` (2) and `sdf/sdfModel.c` (2). The operand is a
  field address at a null base, so the value is an offset and not an address,
  and the cast truncates nothing. The converter skips these by name. What does
  change at LP64 is the offset itself, once a pointer member ahead of the field
  grows, which is open item 2 and not something a cast spelling can reach.

`sdk/libcdvd.c` contributed nine sites through one macro, `UNCACHED`, whose
body held the cast. Naming the address conversion in the definition closed all
nine. It composes the way `include/eeregs.h`'s `EE_UNCACHED` hook does, but
spells the mirror bit as the literal it already was, because `libcdvd.c` does
not include that header and adding one to an SDK unit buys less than it risks.

### Declarations, not hooks

38 `extern` prototypes in 22 units, for functions the unit calls with no
declaration in scope. The spellings come from each function's definition
elsewhere in the tree, or from another unit that already declares it; the
tree's own convention is a block of unit-local `extern`s near the top, as
`sdf/sdfTmx.c` has had all along. Two units needed a type for a parameter:
`field/fldDraw.c` gained `#include "font.h"` and `facility/fclShopInit.c`
gained `#include "interface.h"`, both header-only and both verified.

`script/scrCommonCommand.c`'s local `static inline ffabsf` is now `scrFabsf`.
It collided with `include/fpu.h`'s tree-wide `ffabsf`, which the unit does not
include, so on target there was no conflict and on a host there is. A
`static inline` nobody calls under the old name emits nothing either way.

## The gate

```
$ date +%s > stamp && touch include/common.h && ninja
[317/318] check config/smt3/checksum.sha1          # ninja exit 0
$ sha1sum build/smt3/SLUS_209.11
5facfbf7916836613449c922e26a4ec412207b32  build/smt3/SLUS_209.11
$ stat -c '%s %Y' build/smt3/SLUS_209.11
4551696 1793724850
$ cat stamp
1793724843
```

Roughly 312 steps for a `touch include/common.h`, so the build did the work
rather than reporting nothing to do, and the image's mtime is later than the
stamp taken before it, so it is this build's image and not the previous one
left in place by a failure. A "no work to do" ninja is not a verification.
Every commit batch on the branch was gated this way, and
`tools/check_unit.py` was run per unit on each batch before the link.

Four changes were reverted because they broke it, below.

## Open items

Ordered by how much a host consumer would gain from closing them.

1. **14 pointer-width cast sites remain**, down from 1470 at `9d0a5ce0`:
   5 narrowing and 9 widening, over 9 files. Measured by
   `python3 tools/host64_casts.py list`, which reports distinct
   `file:line:col` triples. Both remaining classes are described under the
   cast sweep above and neither is closed by a macro: casts to a function or
   array pointer need a per-site typedef, and `(u32)&((T *)0)->field` offsets
   are already correct on a host as written. Nothing here is pending work;
   the next thing a host consumer gains is open item 2.
2. **The `u32` signatures themselves are unchanged.** `aPacket` is still `u32`
   in all five `sdfDrawTagLink` declarations, and `lightPacket`, `tagHead`,
   `corePktPointBuff` and their kind are still `u32` fields. This is the one
   place where 64-bit correctness and byte-identical output genuinely conflict:
   widening the field or the parameter changes the struct layout or the ABI and
   therefore the codegen, in a tree whose gate is the retail checksum. The
   macros mark where the narrowing happens and a host redefines them, but the
   host still has to widen these declarations in its own staged copy. Nothing
   upstream can close this.

   This is now the largest open item and it has a measured size.
   `python3 tools/host64_probe.py trunc` reports **1074 sites over 120 units**
   where an `EE_ADDR` result is stored into a 32-bit declaration. That is what
   the cast sweep could not fix and by construction could not: naming the cast
   stops the cast truncating, and then the declaration truncates instead. Every
   one of the 1074 is a `u32` field, parameter, local or return type that would
   have to widen, and each widening moves a struct layout or an ABI. So the
   number is not a backlog to work through here; it is the size of the staged
   copy a host consumer has to maintain, and the reason the hooks are macros
   rather than edits.
3. **A host view of `include/ps2asm.h` is missing**, which is what keeps clang
   at 225 rather than near 307, and what would be needed for `-c` rather than
   `-fsyntax-only` on either compiler. sdfport has one
   (`include/host/ps2asm.h`, 445 lines) and it was deliberately not copied: it
   is that project's work and it belongs in the consumer, not here.
4. **142 incompatible-pointer-type sites across 42 files.** `fldVecDot` alone
   accounts for 46 of the error lines. These are type-recovery gaps rather
   than width problems, the same class sdfport reports, and each needs its
   struct identified rather than a macro.
5. **39 remaining int-conversion sites across 9 files**, dominated by
   `dds3GetProcessData` (13) and `dds3KillProcess`: callers pass an `s32`
   process id where the declaration says `dds3ProcessID_t *`. Whether the id
   is an address at all is a type question for the IDB, not a conversion to
   name, so none were touched.
6. **Four declarations are load-bearing for the match and had to be
   reverted.** Each compiled fine and moved codegen:
   - `evtCommand.c`, `datAddKotowariPoint`. `evt_1E91A8` reads two `u64`s from
     `scrGetIntPara` and passes them to a function whose real parameters are
     `s32`. With no prototype ee-gcc passes the 64-bit registers through; with
     one it truncates, which is 8 of that function's 16 words. The EE's 64-bit
     registers make this a genuinely 64-bit finding, not a hygiene one.
   - `evtCommand.c`, `evtPlayEventBgm`, `evtStopEventBgm`, `evtTransEventBgm`,
     `evtPlaySe`, `func_001FA158`. Declaring these changed argument conversion
     in five callers, 6 functions differing by `tools/check_unit.py`.
   - `libgcc.c`, `sdfPackQuad`. It is called at `:748` and declared at `:1364`;
     moving the declaration above the call changes the call's result handling,
     because the implicit declaration retail was compiled under returns `int`
     and the real function returns `f32`.
   This leaves `evtCommand.c` and `libgcc.c` as the two units that still do not
   compile. `evtCommand.c` also calls four functions still in assembly with no
   declaration anywhere (`evtSetUnitMove3`, `evtSetUnitRot2`,
   `fldCamSyukanSuriawase`, `func_001FA310`); their signatures would have to
   come from the IDB, and given the reverts above each one is as likely
   to be load-bearing as not.

   The `datAddKotowariPoint` revert was re-measured rather than taken on
   trust: adding the `(s32, s32)` prototype still gives
   `DIFF evt_1E91A8 @ 0x001E91A8: 8 of 16 words differ`, the same figure
   recorded above.

   The obvious way around it does not survive the policy. Declaring
   `extern void datAddKotowariPoint(u64 type, u64 point);` in `evtCommand.c`
   restores the match exactly, 162 functions matching and 0 differing, because
   a `u64` parameter needs no promotion and ee-gcc passes the register whole.
   But the provider in `newdata/datCalc.c` is `(s32, s32)`, and
   `docs/CONTRIBUTING.md`'s same-symbol ABI-view allowance requires a
   declaration to match the provider's canonical type exactly and closes by
   excluding false types. A declaration that matches the bytes by misdescribing
   the function is the thing that allowance forbids, so it was reverted too.
   The same objection kills the equivalent move on `libgcc.c`'s `sdfPackQuad`,
   where the false type would be an `int` return on a function returning `f32`.
   These two units stay uncompiled, which is the cheaper price.
7. **`libgcc.c` has two `return` with no value in non-void functions**
   (`:856`, `:877`). A real defect in the decompiled source rather than a
   portability one; left for whoever owns that unit.
8. **30 `-Wbuiltin-declaration-mismatch` sites remain**: `strlen` (25),
   `vsprintf` (2), `puts`, `memchr` and one `memset` the sweep's pattern
   missed. `strlen` is the interesting one: the tree declares it returning
   both `u32` and `s32` in different units, so correcting the return type is a
   signedness change at every call site and wants its own gated pass.
9. **Three struct sizes genuinely differ at LP64** and cannot be made to
   agree: `dds3GlobalWork_t` (0x1A510), `sifRpcDataQueue_t` (0x18) and
   `sifRpcServerData_t` (0x44). `PS2_SIZE_CHECK` stops asserting them off the
   EE rather than pretending otherwise. A consumer that needs these layouts to
   match the EE's has to change the member types, which is item 2.
10. **The probe compiles, it does not link or run.** sdfport's
    `docs/reference/engine-compile.md` already measured the link for its
    allocator set and found 35 undefined symbols, 20 of them bss and sbss
    globals no `.c` defines. Nothing here changes that.
