# Matching the SDK and C runtime

`.text` ends with 126,120 bytes of code that is not Atlus source: the Sony EE
libraries, newlib, and libgcc. It runs from `0x002C81A8` (`sceGsResetGraph`) to
the start of `.vutext` at `0x002E6E50` and holds 671 functions.

It is now 12 units under `src/smt3/sdk/`, one per archive, so
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
| `libgcc` | 31 | 10,032 | 14 | fp-bit double emulation, dwarf2 frame unwinding, `__main` |
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
right shape and 132 bytes over. So Sony built this newlib with `-O3`-level
inlining, and the version between 1.8.2 and 1.9.0 makes no difference to this
file.

libm is further off: fdlibm 1.9.0's `sf_floor.c` gives 232 bytes against
retail's 216 and differs from the first instruction at both `-O2` and `-O3`,
building the mantissa mask with `lui`/`ori`/`and` where upstream shifts and
masks. That is a different source, not a different flag.

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
