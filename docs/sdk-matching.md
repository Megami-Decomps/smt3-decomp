# Matching the SDK and C runtime

`.text` ends with 126,120 bytes of code that is not Atlus source: the Sony EE
libraries, newlib, and libgcc. It runs from `0x002C81A8` (`sceGsResetGraph`) to
the start of `.vutext` at `0x002E6E50` and holds 671 functions.

It is grouped under `src/smt3/sdk/` into library containers and recovered
compiler objects, so
[reading progress](progress.md) measures it as its own `sdk` category instead
of inside the game-code denominator. This page is the account of what it is,
function by function, and of the two toolchains it takes to rebuild.

## Where the names and the boundaries come from

`config/smt3/sdk_objects.tsv` maps each function to the archive object it was
compiled into. `tools/sdk_objects.py` writes it from installed SCE SDK trees:

```sh
python tools/sdk_objects.py smt3 --sdk .../sce/296 --sdk .../sce/204 --sdk .../sce/303
```

The archives are evidence, never a build input. Nothing from them is linked,
and no Sony source text or symbol is copied into this tree. Two rules produce a
row, and each row says which one did:

| Evidence | Rule | Strength |
|---|---|---|
| `bytes` | Same length, and the same instruction words once `j`/`jal` targets and the low half of every immediate form are cleared. Same normalisation as `tools/pair_dds1.py`. | identifies the object the code was compiled into |
| `name` | The recovered name is a function name in exactly one archive member. | says which object defines that name in the SDKs on hand, not that these bytes came from it |

`libg.a` and `libidt.a` are copies of `libc.a` and `libmx.a` of `libm.a`, so a
name found only there is reported against the original. The soft-float multilib
of `libgcc` is left out: the game links hard-float, and the two define the same
names.

Of the 126,120 bytes, 50,080 (39.7%) are identified by bytes, 65,508 (51.9%) by
name alone, and 10,532 (8.4%) not yet at all. 116 distinct archive members are
named, so the region is around 116 translation units rather than one.

## Assembler options for float constants

`config/smt3/asflags.txt` appends per-unit assembler options after the selected
compiler's defaults. Both the production and `SKIP_ASM` objects use them, as
does `tools/cc.sh` with a canonical source path or `SMT3_AS_UNIT`. Changes to
this file, `cflags.txt` or `compilers.txt` regenerate the Ninja graph.

The `sdk/libm -G0` entry keeps ee-gcc 2.96 and its optimization settings.
Its `li.s` pseudo-instructions must expand into immediate integer words and
`mtc1`, as in native `floorf` and `scalbnf`. Assembling identical compiler output
with `-G8` instead introduces unowned `.lit4` entries. The reviewed `floorf`
source then emits200 rather than216 bytes; `-G0` reproduces all216 native bytes
in an unmasked comparison. This is an assembler setting, independent of the
older compiler selected for other Sony-library units. Passing `-G0` only to
cc1 does not change the assembler's literal-pool expansion.

## Stream-buffer status ABI

`include/newlib_stat.h` records the historical newlib status structure for the
EE ABI. The upstream [newlib 1.9.0 stat declaration](https://github.com/mirror/newlib-cygwin/blob/newlib-1_9_0/newlib/libc/include/sys/stat.h),
[system typedefs](https://github.com/mirror/newlib-cygwin/blob/newlib-1_9_0/newlib/libc/include/sys/types.h)
and [time typedef](https://github.com/mirror/newlib-cygwin/blob/newlib-1_9_0/newlib/libc/include/machine/types.h)
corroborate its fields and order. This tag supplies a historical layout reference;
it does not establish the exact newlib release bundled with the game.

The fixed-width prefix occupies 16 bytes. EE's 64-bit long supplies the remaining
eleven long fields, giving 104 bytes (0x68), with mode at +4 and block size at +0x48.
The header checks that size at compile time. Native `fstat` stores exactly those
two fields, while `__smakebuf` passes the record at stack+0 in its 0x90-byte frame,
with saved registers beginning at +0x70. The layout follows independently typed
fields rather than an invented frame-filling array.

`__smakebuf` matches all 340 instruction bytes. It preserves the negative-file
and failed-status defaults, regular-file/seek-provider test, 1024-byte allocation,
embedded-byte fallback, cleanup publication and terminal-only line-buffer flag.
It reads only the mode returned by the status call; unused status fields are not
silently initialized. The typed `_fstat_r` bridge preserves its existing bytes.

`fstat` matches all 20 bytes in its configured Sony-library compiler. It ignores
the descriptor, returns zero, sets character-device mode and clears the 64-bit
block-size field. It leaves every other byte untouched. Both the 2.9 provider
and 2.96 caller compile against the same checked status layout.

## Two different provenances, measured separately

The three SDK trees on hand are 2.0.4, the ee-gcc 2.96 toolchain package, and
3.0.3. Byte-exact coverage of this region, per tree, says the C runtime and the
Sony libraries came from different places:

| Tree | Byte-exact | Where it lands |
|---|---|---|
| ee-gcc `2.96-ee-001003-1` package | 23,640 bytes | `libc.a` 12,240, `libgcc.a` 8,992, `libm.a` 2,368 |
| SDK 2.0.4 | 38,132 bytes | `libkernl.a` 16,536, `libcdvd.a` 4,376, `libpad.a` 2,400, `libdma.a` 1,232, `libipu.a` 1,008, `libgraph.a` 984, `libmc.a` 480 |
| SDK 3.0.3 | 16,036 bytes | the same Sony libraries, less of each |

The C runtime is the 2.96 package's own: `_malloc_r`, `_dtoa_r`, `qsort`,
`_strtol_r` and `vfprintf` are word-for-word that `libc.a` and differ from both
2.0.4's and 3.0.3's by hundreds of words. That is the compiler package this
repo already pins for game code, so the runtime needs no version guess.

The Sony libraries are closest to 2.0.4 and identical to none of the three,
which fits a 2.5 or 2.6 release that is not on hand. Each library has to be
matched against the retail bytes, not against an archive.

## The Sony libraries were built by a different compiler

They were not compiled by the 2.96 the rest of the build uses. They were
compiled by **ee-gcc 2.9-991111-01** at `-O2 -G0 -fno-common`, and that is
measured, not inferred:

- SDK 2.0.4's `libgraph.a` member `graph020.o` holds `sceGsPutIMR` and is byte
  identical to the retail function. Only the two unrelocated `jal` targets
  differ, so retail's libgraph is that build of that object.
- 2.96 cannot produce it. Given the obvious C body, 2.96 and 2.95.3 both emit a
  32-byte frame with the three saved registers 8 bytes apart. Retail uses a
  48-byte frame with them 16 bytes apart, which is what 2.9 emits: it reserves
  16 bytes per saved register and reports them as `regs= 6/0`.
- 2.9 sibcalls only when the value is discarded. Retail's `sceGsGetIMR`
  returns what `GsGetIMR` gave it and keeps a frame with `jal`, where 2.96
  turns the same C into a `j` sibcall. Measured on a two-line probe: 2.9 gives
  `jal`/`jr` for `return f()` and `j` for a bare `f();`, 2.96 gives `j` for
  both.
- The flags come from SDK 3.0.3's own library makefile
  (`ee/src/lib/dma/Makefile`): `CFLAGS = -c -Wa,-al -fno-common -O2 -G0`, no
  `-g`. The ICO decompilation records the same `-G 0` without `-g` for the Sony
  libraries, against `-g -G 8 -fno-common` for game code.

`tools/download_tools.py --compiler ee-gcc2.9-991111-01` fetches that compiler
from the decomp.me archive, and its `cc1` output is identical to the 2.9 `cc1`
shipped in SDK 2.0.4, so nothing from the SDK has to be run.

`config/smt3/compilers.txt` assigns it per unit, the way `cflags.txt` assigns
flags, and every Sony library unit is on it.

Standalone checks must use the same unit selection. `tools/cc.sh` reads
`compilers.txt` for both source paths and `SMT3_AS_UNIT` overrides, including
GNU-version defines and assembler flags. For example, the IPU control writers
match under 2.9 but each differ by eleven words under 2.96. Run
`tools/test_cc.sh` to check wrapper selection and override handling.

Three of its traits decide how the C has to be written:

- Sibling calls only where the result is discarded. 2.9 emits `j func` for a
  tail call whose value the caller throws away, and `jal` plus `jr $31` as soon
  as the value is returned; 2.96 emits `j func` for both. So a `j` to another
  function at the end of a 2.9 unit says the original returned `void`, which is
  how `setD4_CHCR` was recovered: Hex-Rays reads its tail call to `EIntr` as
  `return EIntr()`, and that shape cannot assemble to a `j` under this
  compiler.
- 16 bytes reserved per callee-saved register, so three saved registers mean a
  48-byte frame with `sd` at 0x00, 0x10 and 0x20.
- A `volatile` access is never moved into a delay slot. Where retail returns
  with a store in the delay slot of `jr $31`, the original wrote the access
  through a plain pointer.

The first functions matched under it were `sceGsGetIMR`, `sceGsPutIMR`,
`sceDmaGetChan` and `sceDmaPutStallAddr`; libdma, libgraph and libpad have
since been worked through the probe loop.

### The probe loop

`tools/sdk_probe.py` compiles one candidate file with a unit's toolchain and
compares every function it defines against the retail bytes, without linking:

```sh
python tools/sdk_probe.py smt3 probe.c
```

It reports each function as a match or prints the differing words. Several can
run at once, which a full relink cannot, so this is the loop to work in; the
relink is the final check once a body lands in its unit.

## What is in the region

| Library | Funcs | Bytes | Members | Note |
|---|---|---|---|---|
| `libkernl.a` | 313 | 37,012 | 21 | threads, intc/dmac, sif cmd and rpc, tty, file I/O, loadfile, TLB, exec |
| `libc.a` | 85 | 32,640 | 46 | newlib: stdio, malloc, dtoa, qsort, strtol, locale, signal |
| `libgcc` | 31 | 10,032 | 14 | [fp-bit conversions](libgcc-conversion-matching.md) and [arithmetic](libgcc-arithmetic-matching.md), dwarf2 frame unwinding, `__main` |
| `libm.a` | 14 | 7,624 | 14 | fdlibm single-precision: sin, cos, tan, acos, atan2, sqrt, rem_pio2 |
| `libcdvd.a` | 30 | 6,976 | 5 | `cdvd000.o` and four numbered siblings |
| `libmc.a` | 28 | 6,248 | 1 | one object |
| `libpad.a` | 30 | 5,016 | 1 | one object |
| `libgraph.a` | 13 | 4,744 | 10 | `graph001.o` through `graph029.o` |
| `libdma.a` | 19 | 2,120 | 1 | one object |
| `libmrpc.a` | 7 | 1,832 | 1 | the multi-threaded RPC variant |
| `libipu.a` | 4 | 1,344 | 2 | `setD4_CHCR` matches; the three others spin on `IPU_CTRL` in wait loops that carry hazard `nop`s no plain C loop reproduces |
| unidentified | 66 | 10,532 | | mostly placeholder-named functions inside a known library |

Sony numbered the members of `libcdvd.a` and `libgraph.a`, so those names say
which object without saying what it was called in source.

## The split, and what it cost

The 12 units follow the library boundaries above. Three things had to be true
for the relinked image to stay byte identical, and each is a rule for the next
split in this region:

- **Each unit's `.rodata` needs its own subsegment.** A jump table refers to
  local labels in its own function, so it has to be assembled into the same
  object. The boundaries are the first rodata symbol each unit references,
  which came out monotone in text order with one shared constant.
- **A rodata block whose unit contains a 16-byte-aligned jump table must start
  16-byte aligned.** spimdisasm emits `.align 4` before such a table, and that
  directive aligns the offset within the section, so a section starting 8 bytes
  off puts the table 8 bytes wrong. libkernl's block starts at its first jump
  table for this reason, and the strings before it sit in libc's object.
- **An object aligned more strictly than its code needs has to say so.**
  libkernl's first object, `klib.o`, aligns `.text` to 64, and the retail link
  pads 16 bytes after libc's last function to reach it. `TEXT_ALIGN(6)` in the
  unit asks for that alignment; without it everything after libkernl moves 16
  bytes and the image is 1.5 MB different.

The already-matched C moved with the split: 72 functions (53 newlib, 19 libgcc)
that were sitting in `code_002C7CB0.c` are now in `sdk/libc.c` and
`sdk/libgcc.c`, and still match, which is a second witness that the C runtime
belongs to 2.96.

### 6,068 bytes of handwritten stubs

182 of libkernl's functions are `syscall` stubs of two or three instructions
and were never C: 5,980 bytes, plus one each in libcdvd, libgraph and libmc. A
sixth of libkernl's bytes can only ever be assembly, so the library's ceiling
is 32,612 of 38,592 bytes.

### 3,508 bytes that were never C either

Twelve newlib functions are hand-written R5900 assembly in newlib itself, which
the archive's debug paths name: `memchr`, `memcmp`, `memcpy`, `memmove`,
`memset`, `strcat`, `strchr`, `strcmp`, `strcpy`, `strlen`, `strncmp`,
`strncpy`, all from `libc/machine/r5900/*.S`. They belong with
`asm/smt3/hand/crt0` in the handwritten category, not in a C denominator.

## Free software, and everything else

The split that decides how each unit is worked is not Sony library against
Atlus code. It is whether the source is public:

**newlib, fdlibm and libgcc (50,296 bytes).** Free software, and the exact
build is pinned: the ee-gcc 2.96 package's archives. A matching unit is upstream
source compiled with the pinned `cc1` and the flags Sony built the archive with.
The archive objects are on disk, so the loop does not even need the retail image
to iterate: compile, compare to the member, then confirm against retail. The
debug paths in each member name the upstream file, down to
`src/newlib/libc/stdlib/mallocr.c` for `_malloc_r` and `src/gcc/libgcc2.c` for
the rest of libgcc.

Flags are the open question, and not a small one. Measured on `qsort`, whose
archive object is byte identical to retail: upstream newlib 1.8.2 and 1.9.0
compiled at `-O2 -G0 -fno-common -fno-builtin` give 1,340 bytes with `med3` and
`swapfunc` as separate functions, where retail is one 2,544-byte function with
both inlined. At `-O3` both versions give one function of 2,676 bytes, the
right shape and 132 bytes over. This establishes a different inlining result;
it does not prove a blanket `-O3` setting for the runtime. The two upstream
versions make no difference to that diagnostic.

The early unadapted fdlibm `sf_floor.c` diagnostic emitted232 bytes against
216 native bytes at both optimization levels. The current PS2 reconstruction
matches all216 bytes: native word gates, conversion boundaries and the
assembler's immediate-constant expansion all matter. See
[EE math matching](libm-matching.md) for the qualified source and library-call
profile. Source differences and build settings must be established separately.

So these units are ordinary matching work after all, with upstream source as a
strong hint rather than an answer. The archives still help: a member's bytes
say when a candidate is right without needing the retail image.

**The Sony libraries (65,292 bytes).** No public source. SDK 3.0.3 ships
sources for `libdma` and `libgraph` only (`ee/src/lib/dma/libdma.c`,
`ee/src/lib/graph/graphdev.c`), and those are confidential Sony text: they may
be read to settle what a function means, and nothing may be copied from them,
not source lines and not symbol names. Work on these units is clean room: the
implementer works from the retail disassembly and public API documentation, and
whoever has read the SDK source does not write the unit.

Public references that are not SDK text: the SDK's own public headers, the
ps2sdk reimplementation (`ps2dev/ps2sdk`), and the naming already recovered in
the IDB.

## Other projects that have matched this code

| Project | Compiler | What it has | Usable here |
|---|---|---|---|
| [nathanialf/ico](https://github.com/nathanialf/ico) | ee-gcc 2.9-991111-01 | MIT licensed. Reproduces the whole `sce/` tree plus newlib, libm, libgcc and `crt0.s`, and records per-library build flags. Fully matching on three targets. | the flag evidence above; its SDK objects are reproduced from assembly, not C |
| [OpenRAC/rac5-sm-decomp](https://github.com/OpenRAC/rac5-sm-decomp) | pinned Sony compiler, version not stated | GPL-3.0. Nine SDK bodies matched in C, including `sceGsResetGraph`, `sceDmaSend`, `sceDmaReset`, `sceDmaPause`, `InitAlarm` | copying from it makes this repo's derived files GPL-3.0, which is a project decision, not a technical one |
| [denzi-gh/crashwoc-decomp-ps2](https://github.com/denzi-gh/crashwoc-decomp-ps2) | ee-gcc | GPL-3.0. `sdk/sce/graphdev`, `sdk/gcc/libgcc2` and newlib `mallocr` decompiled, `sceGsResetGraph` at 97% | same |
| [entriphy/kl2_lv_decomp](https://github.com/entriphy/kl2_lv_decomp) | ee-gcc 2.9-991111-01 | the nearest compiler to ours among active PS2 projects | comparison, not code |
| [oohweee/sh2-proto-decomp](https://github.com/oohweee/sh2-proto-decomp) | PS2 era | SDK headers in tree | reference |

Neither of the two with matched C is a free import: both are GPL-3.0, and this
repo carries no license file, so taking their bodies is a licensing decision
about the whole tree rather than a shortcut. The one that is permissively
licensed reproduces the SDK as assembly, which this repo already does. Nothing
here is blocked on them: the clean-room loop above is what is producing
matches.

## Where it stands

`sdk` is 18,412 of 125,024 bytes matched (14.73%), and game code is measured
without it: 951,108 of 1,851,268 bytes (51.38%).

Done: the second compiler, selected per unit; the 12-unit split; the probe
loop; and six libraries worked, 85 functions in all.

| Unit | Matched | Left |
|---|---|---|
| `libpad` | 27 of 29 | `scePadPortOpen`, `_send_to_iop`, both drafted |
| `libmc` | 18 of 21 | `sceMcWrite`, `sceMcSync`, `sceMcSetFileInfo`, all drafted; seven internal helpers untouched |
| `libdma` | 18 of 19 | `sceDmaPutEnv`, drafted one instruction out |
| `libgraph` | 6 of 13 | `sceGsResetGraph`, `sceGsResetPath`, `sceGsSetDefDispEnv`, `sceGsSyncPath`, `sceGsExecStoreImage` |
| `libipu` | 1 of 4 | `sceIpuStopDMA`, `sceIpuRestartDMA`, `sceIpuSync` |
| `libmrpc` | 2 of 7 | the five that touch the packet queue |
| `libcdvd` | 14 of 30 | `sceCdPowerOff` and `sceCdReadClock` read a second reply word; `sceCdSync` and the bind-loop functions are untouched |
| `libkernl`, `libc`, `libm`, `libgcc` | none | untouched |

Three things the IDB's decompiler gets wrong here, each of which cost a pass
before it was understood:

- **It invents a return value for a void tail call.** `setD4_CHCR` ends in
  `j EIntr`, which Hex-Rays reads as `return EIntr()`. Under this compiler that
  shape cannot produce a `j` (see the sibcall rule above), so the `j` is itself
  the proof the function returns nothing. `sceSifMExitRpc` and
  `_sceRpcFreePacket` are the same story, where the "returned" value is just
  whatever was left in `$v0`.
- **A label that is not a function.** `libipu` calls `gcc2_compiled__7`, which
  is the `gcc2_compiled.` label at the head of the object that defines the
  channel-3 setter. The call is real; the name is a boundary artefact.
- **A placeholder name on a real symbol.** `_GetGsDxDyOffset` is a two
  instruction `syscall` stub, not C, so it belongs with the handwritten code
  rather than in any C denominator.

### How the near misses were closed

Five rules came out of taking `sceCdSyncS`, `sceCdMmode`, `sceCdGetDiskType`
and `sceCdStatus` from one instruction out to exact, and they apply to every
unit in this region:

1. **An access that must land in a delay slot cannot be `volatile`.** 2.9 never
   moves a volatile access into one, so where retail reads a word in the delay
   slot of a call, the original used a plain pointer. This was the whole of the
   recurring "one instruction long, `nop` in the delay slot" difference.
2. **A `volatile` access cannot fill a hazard pad either**, which is why
   libipu's `IPU_CTRL` polls carry four explicit slots between the load and the
   branch; see [its page](nonmatching/sdk/libipu.md).
3. **An sceSif client data symbol is the structure, not a pointer to it.**
   Retail passes `&D_0052BA48` with `addiu`; declaring it `void *` loads a word
   and costs a register.
4. **The fall-through arm is the one written first.** Branch polarity is
   evidence about source order, and inverting a test is a one-line experiment.
5. **One local pointer holds an address passed to two calls**, where naming the
   global twice recomputes it; the store through it still goes to the global.

Next, in the order the bytes are worth having:

1. **`libc`, `libm` and `libgcc` (50,296 bytes).** The upstream file each
   archive member names, compiled with the pinned `cc1`. The flag archaeology
   above (`-O3`-level inlining for newlib, a different fdlibm for libm) is
   where this starts, and the archives let a candidate be checked without the
   retail image.
2. **`libkernl` (37,012 bytes, 31,032 of them reachable).** 182 of its
   functions are `syscall` stubs that were never C.
3. **The rest of `libcdvd`.** Fourteen of thirty are done and the scmd
   template plus the five rules above are written down, so the remaining
   wrappers are mostly transcription. `_sceCd_scmd_prechk` and
   `sceCdDiskReady` carry the bind-retry loops and are the real work.
4. **The nine parked drafts.** Each is one register-allocation decision away;
   [the per-unit pages](nonmatching/) say which.
5. **Per-object units.** The 12 library units can become ~116 member units
   using the member column of `config/smt3/sdk_objects.tsv`.
## What the region says about the recovered names

Several names in this region are the IDB's placeholders or are wrong, and the
archives prove it. `initAtExit`, `runAtExitList` and `registerAtExit` sit in
libgcc's `__main.o`; `sdfFloatToULong`, `sdfDoubleToULong`, `sdfUnpackFloat`,
`sdfPackFloat`, `sdfUnpackDouble` and `sdfPackQuad` sit between libgcc's
`fp-bit` objects and are soft-float helpers, not `sdf` engine code. Correct these in the IDB, then re-export and rebuild. Record each in
[IDB corrections](idb-corrections.md) when it is done.

## Matched disc semaphore and power-off callbacks

### Semaphore delay and alarm callback

`sceCdDelayThread` matches all 104 bytes using the same six-word semaphore
record as the matched `sdfCreateSema` wrapper. It initializes maxCount to 1,
initCount to 0 and option to 0, retaining the native untouched fields. It
creates the semaphore, sets the 16-bit alarm delay with the semaphore ID as
callback argument, waits, then deletes it. The 36-byte callback forwards its
third argument to `iSignalSema`, then uses the existing `EE_SYNC_EI` hardware
primitive. The compiler supplies the argument move and complete epilogue.
At that qualification, the complete disc owner checked 21 match / 0 differ.

### Power-off callback registration and dispatch

The three power-off callback functions match 296 bytes. Registration returns
the prior callback pointer, publishes its user argument before the new function
pointer, and restores interrupts only when `DIntr` reported them enabled.
Installation sets the dispatch guard before registering command `0x80000012`,
then restores interrupt state before clearing that guard and marking the handler
installed. Dispatch requires a nonnull callback and a clear guard.

The HDD consumer uses that same function-pointer/user-pointer interface, with
its semaphore ID carried as the user argument. Updating its placeholder
declaration and handler parameter preserves all three existing HDD functions.

## Matched RPC request execution

`sceSifExecRequest` matches all 452 instruction bytes. It calls the registered
handler, retains a reply length only for a nonnull result, writes back positive
request/reply spans, and selects a forwarding packet under disabled interrupts.
Command mode retries the send immediately. DMA mode clears the native packet
fields and submits an optional reply descriptor followed by the packet descriptor.
Only a failed DMA submission runs the native countdown before retrying.

Each DMA descriptor is four wire words: source address, destination address,
byte count and attributes. A two-element array of four-word records reproduces
the native field-address calculations. A C structure of four named fields has
the same 16-byte extent but changes scheduling and saved-register allocation
(77 of 113 words match); a flattened eight-word array emits 468 bytes instead
of 452. The ordinary empty signed countdown from `0x100000` to `-1` naturally
emits the four NOPs between decrement and comparison with the configured 2.9
compiler. No inline assembly or volatile counter is needed. The complete
kernel unit checks 25 match / 0 differ at this qualification.

## Matched RPC read-data reply and server lookup

`_request_rdata` matches 96 bytes. It obtains a forwarding packet, captures
the incoming packet-address and client routing words before writing either,
sets the read-data response command, and sends the incoming source,
destination and byte count through the interrupt-safe command sender. The
command dispatcher discards the callback result; this handler returns void.

`_search_svdata` matches 76 bytes. It walks the global server-queue list and
each queue's registered-server list, returning the first service-ID match or
null. Registration links are distinct from pending-request links. The complete
kernel owner checks 32 match / 0 differ at this qualification.

## Matched synchronous and asynchronous RPC transactions

`sceSifBindRpc` (320 bytes) and `sceSifGetOtherData` (344 bytes) match exactly.
Both allocate a request packet, capture its sequence in the transaction record,
and select synchronous or asynchronous completion using only mode bit zero.
Synchronous calls create a semaphore, send, wait and delete it; asynchronous
calls store semaphore ID `-1` and send without waiting. Packet exhaustion returns
`-1`, semaphore creation failure returns `-3`, and send failure returns `-2`.
The native packet release and semaphore deletion order is preserved on failures.

Unlike the multi-threaded bind variant, these routines initialize only the
semaphore's maximum count and initial count. They leave the other four words
untouched. Bind clears only the client service-ID and server-pointer fields
before allocation; neither routine adds client-field cleanup after failures or
after deleting a semaphore. Read-data uses just the packet/sequence/semaphore
transaction prefix. The complete kernel owner checks 34 match / 0 differ at
this qualification.

## Ordinary empty bind-retry loops

`sceMcInit` matches all 436 bytes with ordinary C under the configured 2.9
compiler. Its retry delay is:

```c
for (delay = 0x100000; --delay != 0;) {
}
```

The compiler retains the countdown, loads `0x100000`, and emits the native
four-NOP loop body between the decrement and branch. The empty infinite loop
after the bind-error diagnostic also receives its native padded branch block.
Neither loop needs an inline-assembly primitive or a volatile counter.

The initializer writes only the semaphore's initial count, maximum count and
option fields. It signals the semaphore before checking the RPC result; a
negative result returns that result minus 100. The server and manager version
checks read the shared reply after preceding callbacks. All three diagnostics
are ordinary string literals, with their complete read-only-data placement
verified against retail.

`scePadInit` independently matches all 320 bytes with the same ordinary
countdown mechanism, using `for (delay = 0x10000; --delay != -1;) {}` for
both client retries. The native `lui 1` initializes 0x10000; the first
comparison sees 0xFFFF and the final one sees -1. The two RPC clients are
consecutive 0x28-byte records with server pointers at +0x24. Bind results
are ignored; each retry waits for its server pointer before continuing.
The version gate forwards the original mode only when the signed major
version is 4, otherwise optionally prints the mismatch and returns zero.

## Matched heap allocation and release wrappers

`sceSifAllocSysMemory` (128 bytes), `sceSifFreeSysMemory` (120 bytes) and
`sceSifFreeIopHeap` (28 bytes) match 276 instruction bytes. Allocation sends
three words in size/mode/address order using command four and returns the
reply address, or null on a negative RPC status. Free sends one address word
using command two and returns the reply status, or `-1` on RPC failure. Both
return zero while the heap initialization flag is negative. The small free
wrapper forwards the result through the native call/return sequence.

The BSS map bounds the shared request buffer to 64 bytes. Its word-array view
supports the three-word allocation request and the existing one-word requests
without inventing neighboring storage. The complete kernel owner checks 38
match / 0 differ; heap initialization remains a guarded draft with its native
assembly fallback.

## Matched load-file module management

`sceSifUnloadModule` (144 bytes), `sceSifSearchModuleByName` (160 bytes) and
`sceSifSearchModuleByAddress` (144 bytes) match 448 instruction bytes. All
bind first and require the supported service version. They distinguish bind
failure (`-65536`), unsupported version (`-65540`) and RPC failure (`-65537`),
returning the shared reply word after a successful transaction.

The name-search RPC sends the complete 512-byte request region: the BSS map's
two-word header followed by 252 name bytes and 252 argument bytes. The loader
that shares this buffer reads the second header word as its module result.
Name search copies at most 252 name bytes and forces name byte 251 to zero,
leaving the two header words and argument storage untouched. Unload and address search send only four bytes.
All three receive their result into the first request word.

Writing the RPC failure return first emits the native branch-likely result
load with the configured 2.9 compiler. A nonnegative-result-first form leaves
one branch-word difference in each four-byte transaction and adds eight bytes
to name search. The complete kernel owner checks 41 match / 0 differ at this
qualification.

## Matched load-file initialization and loader wrappers

`_lf_bind` (256 bytes), `_lf_version` (140 bytes), `sceSifLoadModule` (32 bytes)
and `sceSifLoadStartModule` (28 bytes) match 456 instruction bytes. Binding is
skipped once the initialization flag is nonnegative. Otherwise it retries the
synchronous bind with the native empty countdown until a server is available.
It clears the flag before querying command `0xFF`; a failed query returns
`-65537` without resetting that flag. A four-byte `memcpy` preserves the native
unaligned copy into the version-byte buffer.

Version validation compares the detected, built-in and configured four-byte
values. It returns one only when all three are pairwise different. The basic
loader wrapper supplies a real local result slot; the start wrapper forwards
its caller's result pointer. Both preserve the core's return value and command
zero, including the configured compiler's call/return sequence.

Keeping bind work inside the negative-initialization guard matches the native
entry branch. An early nonnegative return instead emits 260 bytes. Constructing
the built-in address before the detected address reproduces the four native
address-setup words in the version helper; the reversed declaration order is
the same 140-byte size but differs in those words. The complete kernel owner
checks 45 match / 0 differ at this qualification.

## Matched ELF loading

`_sceSifLoadElfPart` (260 bytes), `sceSifLoadElfPart` (28 bytes) and
`sceSifLoadElf` (40 bytes) match 328 instruction bytes. The core copies and
terminates both the 252-byte path and section fields, sends the complete
512-byte request and receives 16 bytes. It consumes only the first two reply
words. Bind, version and RPC failures leave caller output untouched; a zero
entry word returns `-65539` and also leaves output untouched. Otherwise it
copies the entry and global-pointer words and returns zero.

The wrappers supply command one; the whole-file wrapper supplies the ordinary
string literal `"all"`. No larger caller-output record is inferred from the
16-byte receive length. The complete kernel owner checks 48 match / 0 differ
at this qualification.

## Matched module-loading core

`_sceSifLoadModule` matches all 548 instruction bytes. It copies and terminates
the path, then copies at most 252 argument bytes. A null argument pointer
writes only the first argument byte and sets the transmitted count to zero;
other unused argument bytes retain their native shared-buffer state. The
size test is signed, and the short-copy path forwards the original size.

The ordinary constant-size `memcpy` expands into the native aligned/unaligned
252-byte copy paths with the configured compiler. The RPC sends 512 bytes and
receives eight. Failure leaves the caller's result untouched. Success captures
the first reply word before writing the second through the caller's result
pointer, preserving the native behavior even when that pointer aliases the
shared reply. A write-then-read form has the same 548-byte size but differs in
three result-path words. The complete kernel owner checks 49 match / 0 differ.

## Heap query result words

The five heap queries reproduce their complete configured 2.9 bodies with
32-bit result words. Block-top and block-size queries send one block key
word, using selectors 8 and 9; memory-size, maximum-free and total-free
queries send no request, using selectors 5, 6 and 7. Each receives one word.
An unavailable heap returns zero before touching the request or RPC. A
negative RPC result returns the all-ones word `0xFFFFFFFF`; success returns
the received word. This describes the reconstructed word contract rather
than asserting the spelling of the original SDK declaration.

Writing the failure case first (`if (call(...) < 0) return 0xFFFFFFFF;`
then the reply return) reproduces native `bgezl` with the reply load in its
delay slot. The equivalent success-first block-query form instead emits
ordinary `bgez`, differing by one word at the same 124-byte size. The native
query failure uses `lui`/`ori` for the all-ones result; the signed free-memory
wrapper uses `addiu -1`. The three request-free query bodies each match
112 bytes; the two block queries each match 124. Complete-owner checks
qualify the shared signed reply buffer and unchanged neighboring functions.

## Matched IOP address reads and status polling

`sceSifGetIopAddr` (236 bytes), `sceSifIsAliveIop` (40 bytes) and
`sceSifSyncIop` (56 bytes) match 332 instruction bytes. Address reads bind
before checking the unsigned width kind, reject kinds outside zero through
two before writing the request, and send/receive 32 bytes. On success they
copy only the selected unsigned byte, halfword or word from the reply.
Binding, invalid-width and RPC failures leave caller output untouched.

The live query returns whether register four has bit `0x10000` set. The sync
query checks bit `0x40000`, resets TTY initialization only when it is set, and
returns zero or one accordingly. A boolean result local reproduces the native
mask/unsigned-compare sequence; a direct comparison instead emits a shorter
shift/low-bit sequence. Explicit width branches reproduce the native address
reader, while a switch emits 232 bytes instead of 236. The complete kernel
owner checks 53 match / 0 differ; the address writer remains guarded.

## Matched IOP reset transaction

`sceSifResetIop` matches all 316 instruction bytes. Its 104-byte packet has a
16-byte command header, argument length/mode words and an 80-byte argument
area. The native 64-bit header-field update is represented by 8-bit packet
size and 24-bit data size fields with a 64-bit allocation type; the destination
remains at offset four. A compile-time size check verifies the wire extent.
A 32-bit allocation type instead emits a shorter word update and does not match.

The routine stops DMA, captures the destination register, and copies nonzero
argument bytes without appending a terminator or adding a length bound. It
leaves the option word and unused argument bytes untouched. After packet
writeback it sets register four to `0x40000` and submits one descriptor with
size 104 and attributes `0x44`. Only successful submission performs the four
native register updates and returns one; failure returns zero.

Initializing the payload length and command before clearing the header data
size reproduces the native field scheduling. Keeping the success-side register
updates inside a nonzero-DMA-result block reproduces the final branch. The
complete kernel owner checks 59 match / 0 differ at this qualification; reboot
remains assembly-backed.

## Matched module-buffer loading and stopping

`_sceSifLoadModuleBuffer` and `sceSifStopModule` each match 520 bytes. The
load-buffer and load-start-buffer wrappers each match 28 bytes, for 1,096
instruction bytes across the family. The cores write the buffer address or
module ID into request word zero and the argument byte count into word one.
They use commands six and seven respectively, send 512 bytes and receive eight.

Nonnull arguments use the same signed size test and ordinary constant-size
252-byte `memcpy` expansion as the matched path loader. A null argument pointer
sets only the size word to zero; it does not clear any argument byte. The name
area is also untouched. Errors leave caller output untouched. Success captures
the first reply word before writing the second through the caller's pointer,
retaining native behavior when that pointer aliases the reply buffer.

The basic buffer wrapper supplies a real local result slot and ignores its
contents; the start wrapper forwards the caller's result pointer. Both return
the core's result. All four match on the first complete-owner compilation,
which checks 77 match / 0 differ at this qualification.

## Matched directory open and path-command wrappers

`sceDopen` (200 bytes), `sceRemove`, `sceRmdir` and `sceDelDrv` (28 bytes each)
match 284 instruction bytes. Directory open waits for filesystem access,
initializes it when necessary, releases that semaphore, then reserves a request
slot. Exhaustion returns `-19`. A failed remote open clears only the reservation
word under the table semaphore; success stores the remote handle and returns
the slot index. Both completion paths signal the table semaphore.

Each completion branch captures the semaphore ID after its wait and slot write,
before the common signal call. This reproduces the native separate loads and
register allocation; reading the global only at that common call instead emits
192 bytes. The three siblings return the path-command result for commands six,
eight and sixteen, preserving the native call/return sequence. The complete
kernel owner checks 83 match / 0 differ at this qualification.

## Matched driver registration

`sceAddDrv` matches all 288 instruction bytes using the existing filesystem
completion-request prefix. It publishes the driver address in request word
three, initializes only the semaphore maximum/initial/option fields, and sends
command fifteen with a 16-byte request and four-byte acceptance reply. It adds
no semaphore-creation failure check absent from native code.

RPC failure deletes the semaphore before signaling filesystem access and
returns `-1`. Success snapshots the acceptance word through the uncached alias
before signaling. A zero acceptance deletes the semaphore and returns `-1`;
nonzero acceptance waits for completion, deletes the semaphore and returns the
callback's stack result. The complete kernel owner checks 88 match / 0 differ
at this qualification; no competing request-buffer type is introduced.

## Matched filesystem sync transaction

`sceSync` matches all 408 instruction bytes. Its request view extends the
shared union with mode at offset `0x10` and a 1024-byte path at `0x14`; the
shared descriptor word at `0x0C` is left untouched. The bounded copy includes
the terminator when found and forces path byte 1023 to zero at the limit.
Unlike the variable-length path commands, sync always sends 1044 bytes.

The routine retains the native partial semaphore initialization and unchecked
creation result. Command nineteen receives a four-byte acceptance word, which
is captured through the uncached alias before releasing filesystem access.
RPC failure and zero acceptance delete the completion semaphore and return
`-11` in their distinct native order; acceptance waits, deletes and returns
the callback's stack result. The complete kernel owner checks 93 match / 0
differ at this qualification.

## Matched symbolic-link transaction

`sceSymlink` matches all 480 instruction bytes using the published two-path
request view. It independently copies and terminates each path within its
1024-byte field, retaining the bytes after the first NUL. It preserves the
native wait-helper argument seventeen and RPC command twenty-four. Unlike
rename, it has no separate cache-writeback call before sending the complete
`0x80C`-byte request.

The result-pointer/semaphore/result-size setup order reproduces native
publication. Partial semaphore initialization, uncached acceptance capture
before releasing filesystem access, failure cleanup and callback result
lifetime remain the same audited completion contract. The complete kernel
owner checks 96 match / 0 differ at this qualification.

## Kernel patch copy helpers

`kCopy_1` copies an unsigned byte count forward with byte loads/stores.
`kCopy_0`, `kCopy_2` and `kCopy_3` first shift that unsigned byte count right
by two, then copy words forward; trailing one to three bytes are ignored.
All four explicitly return zero, including for an empty copy. The three
word-copy bodies are byte-identical native siblings.

The byte helper uses incrementing pointers and a separate unsigned loop
index. The word helpers use ordinary indexed `u32` copies; the configured
EE 2.9 compiler supplies their native pointer increments and loop padding.
No copy intrinsic, assembly, extra alignment or compiler flag is needed.
Together these are four exact SDK EE functions / 216 instruction bytes.

## Alarm and TLB patch initialization

`InitAlarm` (212 bytes) and `InitTLBFunctions` (196) match as ordinary C.
The existing data at `D_0052D2B0` and `D_0052D670` each spans eight mutable
pairs: a 32-bit syscall index and an entry address. Their kernel patch
blocks remain external data; no embedded code or syscall stub is translated
into a C byte array.

The alarm initializer does nothing when timer register `0x10001810` has bit
`0x100` set. Otherwise it installs the copy helper, copies the native
`0x740`- and `0x28`-byte blocks, flushes cache modes 0 then 2, installs the
entry lookup and patches entries 2 through 7. The TLB initializer similarly
copies its `0x330`-byte block, flushes both caches, installs entries 1 and 2,
then patches entries 3 through 7. Each loop reloads the syscall index from
the mutable table after the entry-lookup call before installing its result.

The TLB initializer publishes `GetEntryAddress_1(3)` to the pointer at
`D_0052D668`; consumers use that address as their argument-storage base.
Only that pointer is accessed; the following word does not establish a
two-element C array. Both routines
have void interfaces: the startup caller discards the alarm result and
tail-calls TLB setup; neither initializer manufactures a return value.
The installed copy implementations have explicit zero results, so their
copy syscall declarations retain that integer return. Cache flushing and
setup calls discard their results.


## Signed long conversion

`_strtol_r` matches all 568 instruction bytes with 64-bit `long` and
`unsigned long`, as required by the EE ABI. Signed character loads feed
`_ctype_ + 1`; the routine consumes whitespace and one optional sign, selects
hexadecimal after an eligible `0x` prefix, and otherwise selects octal or
decimal when the base is zero. It adds no base-range validation.

The unsigned cutoff magnitude is `0x8000000000000000` for negative input
and `0x7fffffffffffffff` otherwise. Remainder is computed before quotient;
the corresponding providers are the existing `__umoddi3` and the inferred
`__udivdi3` at `0x002E6048`. Digit accumulation uses the inferred `__muldi3`
at `0x002D3398`; both newly named providers keep their assembly bodies.

An unclassified character breaks the scan; a classified digit outside the
base goes directly to the shared completion label. This preserves the
native loop layout. An overflowing magnitude stops updating the accumulator
but continues consuming valid digits, then saturates and writes error 34.
Other paths leave the error unchanged. The end pointer is the original
input if no digit was accepted, including a sign or hexadecimal prefix
without digits; otherwise it identifies the first unconsumed character.

## Kernel startup sequence

`_InitSys` matches all 60 bytes as a no-argument void initializer. Its sole
caller is startup assembly and discards its result. It calls semaphore
setup, syscall-table discovery, alarm setup, thread initialization and
executable-patch setup in native order, then tail-calls TLB setup.
The configured EE 2.9 compiler supplies that tail handoff directly.

`InitThread` explicitly returns -1 on the early/failure paths or its stored
thread ID on success. Both the startup and SDF callers discard that result;
their declarations retain its integer interface. This declaration correction
earns no matching credit. The neighboring semaphore setup remains guarded
with a [four-store residual](nonmatching/sdk/libkernl.md#supplement_crt0-sdklibkernl-0x48).

## Executable patch installation

`PatchIsNeeded_0` (104 bytes) saves one configuration word, clears bits
13 through 15 and sets that field to one in a second word. It writes and
rereads the second word, restores the original, then tests whether the
reread field is zero. Two actual `u32` locals reproduce the native frame
and unsigned shifts without an invented configuration record or padding.

`InitExecPS2` (172 bytes) uses that predicate to gate installation. It reuses
`kernelEntryPair_t` for the existing three-entry table at `D_0052CB28`,
installs the word-copy entry, copies exactly `0x7A8` bytes to `0x80074000`,
flushes cache modes 0 then 2, installs the lookup entry, and resolves the
remaining table entry. Its raw selector `0xFFFFC402` is passed unchanged.
The table index is reloaded after the lookup callback before setup.

The patch code stays external data and all syscall stubs stay in assembly.
The shared pair declaration and cache-flush prototype precede all of their
C consumers. Both complete helper bodies match with the configured compiler;
no flags, copy shims or artificial lifetimes are added.

## Signed 64-bit integer conversion

`__floatdidf` at `0x002E6D88` matches all 168 instruction bytes with a real
`s64` input and `f64` result. This EE double ABI carries the value in GPRs;
the reconstruction uses ordinary double arithmetic and the existing `litodp`,
`dpmul` and `dpadd` compiler providers.

The signed high word is converted first and multiplied by 65536 twice. The
low word is then isolated in the 64-bit working integer, narrowed to signed
32 bits, and converted. A negative low word receives a double correction of
4294967296 before the final addition. Preserve both high-word multiplies and
the final addition order rather than reassociating the expression.

The explicit `0xFFFFFFFFULL` mask is a half-width extraction of the genuine
64-bit argument. A qualified CSE/allocation trace (`--expand --cse --alloc
--no-dumps`, with plain and hooked output parity) records the DI-mode AND at
instruction 48 and its subsequent SI-mode narrowing at instruction 50. A
cast-only low-word extraction emits 156 bytes and hoists the narrowing ahead
of the high-word calls; retaining the working integer's low-word mask gives
the native order and exact bytes. No float value is carried through an
integer substitute type.

`config/smt3/sdk_objects.tsv` identifies the entire 168-byte helper as
`_floatdidf.o` by bytes. Its recovered C unit starts at `0x002E6D88` and ends
at `0x002E6E30`; the remaining errno accessor and native padding keep a
separate assembly-backed tail. The earlier combined archive unit starts at
`0x002E52FC`, which is only four-byte aligned. Keeping the new C function in
that aggregate raises its alignment and adds padding, shifting the linked
image despite a standalone instruction match. The object-backed split
preserves both the earlier archive code and the helper's real placement
without compiler-flag or instruction changes.

## Double to unsigned 64-bit integer conversion

`__fixunsdfdi` matches all 280 instruction bytes. Its real interface is
`f64` to `u64`, using the EE double ABI in GPRs. Ordered negative inputs
return zero. The high chunk is an unsigned 32-bit conversion of the input
scaled by `2^-32`, widened and shifted left by 32. The routine subtracts the
double representation of that chunk from the input, then adds or subtracts
the unsigned 32-bit remainder magnitude. The existing conversion providers
govern NaN and out-of-range inputs; no new guard or saturation is introduced.

The cast of the high chunk back to `f64` is genuinely unsigned. The compiler
lowers it to the native signed-negative test, sticky-bit halving before
`__floatdidf`, and doubling of that result. The negative-remainder arm uses
`0.0 - value`, preserving the software-double subtraction operation.

Updating the floating working value to its remainder gives the native
allocation. Qualified before/after allocation traces (`--expand --alloc
--no-dumps`, both with output parity) show that separate input and remainder
locals occupy GPR16 and leave the integer accumulator in GPR17, differing in
17 of 70 words. With `value -= (f64)result`, the accumulator remains the same
DI pseudo (10 references, live length 41, eight calls) but takes GPR16. The
DF working value has seven references, live length 31 and seven calls, and
takes GPR17. Both uses are real floating arithmetic values; neither is an
integer carrier or an artificial dependency.

The byte-identified `_fixunsdfdi.o` owns `0x002E6B30` through `0x002E6C48`.
It has a separate C unit to preserve the original alignment of the preceding
assembly container. The following byte-identified 320-byte `_div_df.o`
remains assembly-backed in its own container. The recovered boundaries do
not change instructions, data ownership, compiler flags or matching credit
for those adjacent routines.


## Frame lookup after allocation failure

`sdk/libgcc::libc_2D29B8` matches all 424 instruction bytes. It holds the
registered-object semaphore while lazily initializing PC ranges and finding
the containing object. A missing sorted array, or an array still equal to
the original table, triggers another initialization attempt. Allocation
failure then falls back to the original single frame sequence or pointer
table, keeping the semaphore held through those searches. The table search
stops before advancing its cursor when a frame is found.

A genuinely sorted array takes the other path: release the semaphore first,
then load the current count and array for unsigned binary search. Do not
capture these fields before unlocking. The midpoint and frame-end arithmetic
use native 32-bit wrapping operations; containment is begin-inclusive and
end-exclusive. Every path releases the semaphore exactly once. The caller
passes a 32-bit PC and consumes the returned descriptor pointer or null.


## CIE metadata and unaligned EH augmentation

`sdk/libgcc::libc_2D2B60` matches all 272 instruction bytes. Its caller passes
a 20-byte metadata record at stack offset zero; the separate 0x480-byte frame
state starts at offset 0x20 and is initialized afterward. The five metadata
words are augmentation pointer, EH pointer, unsigned code alignment, signed
data alignment and return-register number.

The parser accepts empty augmentation, `eh`, or any leading `z`. Rejection
writes only the augmentation pointer. The optional EH pointer follows a
variable-length string, so its packed encoded view permits the native
unaligned four-byte load. Code/data alignment use the existing ULEB/SLEB
providers. The return-register byte is zero-extended; `z` consumes a payload
size and skips those bytes. Both existing string blocks retain their original
assembly data ownership, including the unused trailing bytes.

## Binary64 division

`dpdiv` at `0x002E6C48` matches all 320 instruction bytes. It uses the same
unpacked binary64 record and real `sdfUnpackDouble` / `__pack_df` interfaces
as the matched addition, subtraction and comparison wrappers.

The parts-level division helper and classification primitives are established
in [GCC's historical fp-bit.c](https://github.com/gcc-mirror/gcc/blob/releases/gcc-2.95/gcc/config/fp-bit.c#L944).
They describe real arithmetic operations, rather than wrappers introduced only
to constrain registers. Retail independently establishes the selected class
paths and quotient loop; retain its behavior rather than importing upstream
rounding or subnormal changes.

NaNs propagate numerator-first before sign XOR. Equal zero classes and equal
infinity classes select the existing shared NaN record at `D_0054B460`.
Finite divided by infinity clears the numerator fraction and exponent;
nonzero divided by zero makes it infinity. Exponent-zero inputs follow the
unpacker's zero classification, including subnormals.

The finite path subtracts exponents, doubles a smaller numerator fraction and
decrements its exponent, then performs 61 unsigned restoring-division steps
from bit 60 through bit zero. The loop computes a real `nextRemainder` after
the optional subtraction, then advances the quotient bit and current
remainder. Qualified allocation, PRE, jump and delay-slot traces with output
parity confirm the native shape: current remainder in GPR4, next remainder in
GPR2, quotient in GPR7, quotient bit in GPR3, and the result-record pointer in
GPR8. A direct in-place update or a speculative next-value initialization
leaves an extra loop-entry transfer and emits 328 bytes; the single
next-state calculation gives the native 320-byte body without extra uses,
flags, constraints or instruction padding.

## CFA expression state and decoder

`sdk/libgcc::libc_2D2C70` matches all 288 instruction bytes. The native
register-array stride and boundaries establish 123 eight-byte register
slots. Their saved-rule bytes end at `0x476`, followed by the indirect flag
at `0x477`. The public state is `0x478` bytes; internal remember/restore
snapshots add a saved-state link at `0x478` and occupy `0x480` bytes.
The caller copies the public record as `0x460` bytes plus a `0x18` tail.

The [June 2000 GCC CFA-expression patch](https://gcc.gnu.org/legacy-ml/gcc-patches/2000-06/msg00872.html)
provides the base-offset and indirect-field context missing from the older
header. Native field widths and copy extents determine this target's layout.
The expression decoder preserves its `0x90` fallthrough: the first signed
register operand is stored, then replaced with register 32 before reading
a second signed base offset. The unsigned operand of `0x23` is interpreted
through its signed 32-bit representation before the native 64-bit store.
Unsupported operations call abort; no bounds checks or DWARF-spec corrections
are introduced.


## Building the public frame state

`sdk/libgcc::libc_2D31E0` matches all 440 instruction bytes. It uses separate
CIE metadata and internal-state locals, with natural sizes `0x14` and `0x480`.
Failed frame lookup or CIE parsing returns null before writing the output.
The CIE instruction program runs with no location pointer; the FDE program
uses a mutable location and stops after crossing the queried PC or reaching
the instruction end. The final structure assignment copies only the `0x478`
public bytes and returns the caller's output pointer. The internal saved-state
link is excluded. The compiler generates the native frame and copy loops
from these actual records without manual padding or copy assembly.


## CFI interpreter and dispatch table

`sdk/libgcc::libc_2D2D90` matches all 1100 instruction bytes and its 48-entry
jump table. Opcodes zero, seven and eight are native no-ops. The primary
bit tests retain their original order, including the emitted third restore
arm; they are not rewritten into standard class comparisons. Ordinary
offset rules skip the CFA register at `0x3F8`, not the return column.

Compact advances multiply by code alignment; the explicit one-, two- and
four-byte advances add the raw decoded value. Packed encoded values preserve
unaligned reads. Register-to-register rules zero-extend their operand, while
offset products and the arguments-size operand keep the native signed
32-to-64-bit widening. Negative offsets do not inherit the ordinary CFA
register exclusion.

Remember/restore uses the real `0x480`-byte internal record and linked saved
state, with the native unchecked allocation. Structure assignments generate
the copy loops. Expression blocks reset CFA fields, then call the matched
expression decoder through the encoded end. Window-save rules cover registers
16 through 31 with four-byte offsets. No instruction or data padding is added.

## SIF command DMA submission

`_sceSifSendCmd` matches all 308 instruction bytes. The 16-byte command
header uses a 32-bit storage word for its 8-bit packet size and 24-bit payload
size. The two DMA descriptors now have named bus-address, signed-size and
attribute fields; both existing descriptor consumers remain exact.

The unsigned packet-size check accepts 16 through 112 bytes. A positive payload
adds its descriptor, optionally flushes its source for mode bit 4, and preserves
the full signed payload length in that descriptor despite the header's 24-bit
length. The remote packet address is read after that callback. The packet
descriptor carries attribute `0x44`; the command and size are published before
the mandatory packet flush. Mode bit 1 selects interrupt-context submission.
The option word is untouched. Initializing the payload descriptor before the
header gives the native store schedule using ordinary field assignments.

## Thread-command publication and exception registration

`iRotateThreadReadyQueue` accepts priorities 0 through 127 only while the manager
thread ID is nonzero. It masks the current write index to 511, publishes the
incremented index, writes command 1 and the priority byte, then signals the
manager semaphore and returns the original priority. No queue-full check is added.

`SetTLBHandler` publishes the opaque handler address and installs the native TLB
entry for selectors 1, 2 and 3, returning the supplied address. `SetDebugHandler`
accepts selectors 1 through 13 and indexes the existing 16-entry address table
directly; it returns the previous address or the pointer sentinel `-1`. Selectors
1 through 3 register the native debug entry through the TLB service, and the
remaining selectors use the common-exception service. The hardware entries remain
assembly. Their user callbacks have different argument sets, so the registration
functions preserve opaque code addresses instead of imposing one callback type.

## Process and filesystem compatibility shims

The kernel runtime's `isatty` and the raw provider `libc_2DC670` both return 1;
the latter is called by `_getpid_r` and keeps its existing symbol name. `kill`
calls `Exit(signal)` only for process ID 1 and returns zero. `stat` and `unlink`
leave their supplied objects untouched, set the current errno to 5 through
`__errno_8`, and return -1. These are the native compatibility behaviors.

The raw `close` and `lseek` compatibility providers also return -1 unconditionally
and do not update errno. The existing `_lseek_r` interface supplies signed 32-bit
descriptor, offset and origin values and consumes a 64-bit `long` result; the
native signed immediate return sign-extends to that same full-width -1. These
providers are distinct from the `sceClose` and `sceLseek` RPC interfaces.

## Inverse-cosine entry boundaries

The region formerly covered by the single `__ieee754_acosf` symbol contains
three functions. The entry at0x002CF730 has a complete20-byte forwarding
body, followed by four alignment bytes. The separate20-byte entry
`func_002CF748` forwards two float arguments to `__ieee754_atan2f`, also
followed by four alignment bytes. Both save and restore the return address
before their terminal jumps. The arithmetic inverse-cosine body begins at
`func_002CF760`, owns its own frame and returns independently after1060 bytes.
Its next four bytes are alignment before0x002CFB88.

Keep the established entry names: caller behavior establishes their interfaces,
not their original source spellings. The archive map's old1112-byte name-only
`ef_acos.o` attribution covers this aggregate region and does not prove that
both forwarding entries belong to that object. Function boundaries now follow
the independent native entries, without changing the translation-unit boundary
or compiler profile. The forwarding functions remain assembly.

## Formatted-output entry boundary

The former 0x16E8-byte `vfprintf` inventory span contains two functions.
The public wrapper is 116 instruction bytes at 0x002D6600, followed by four
alignment bytes. It initializes FILE reentrancy/stdio, restores its 0x20-byte
frame, and tail-jumps to `func_002D6678`. The latter has an independent
0x290-byte frame, direct reentrant printf/sprintf callers, and 0x1670 bytes
of formatter code ending at 0x002D7CE8. No source-object boundary changes.

Curated function extents now expose both entries and retain the core's
existing generic identity. The original `sdk_objects.tsv` name-only row
for `vfprintf` describes the former aggregate span; it is not evidence that
the wrapper alone has that extent or a byte-identified producer. All strings
and the dispatch table remain with the assembly-backed core. Neither boundary
recovery nor the guarded wrapper adds exact matching credit.

