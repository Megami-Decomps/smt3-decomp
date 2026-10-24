# The SDF IRX modules

Three IOP modules ship in `USERIRX/` beside the ELF. They are not part of the matching
build (nothing under `orig/` is tracked), but they define the EE side of the sound and
file interface, so the names and protocols here are what the EE code in
`SLUS_209.11` talks to.

| Module | Size | Functions | Role |
| --- | --- | --- | --- |
| `SDFIOP.IRX` | 0x670 | 22 (5 real) | RPC server that loads and starts further IRX modules |
| `SDFIOPFL.IRX` | 0x890 | 20 (5 real) | IRX library exporting four file calls that forward to the EE |
| `SDFSDMAN.IRX` | 0x1a8b8 | 140 | The sound driver: sequencer, SE, 3D panning and ADPCM streaming |

All three build byte-identical from source; how, and the container format, is
in [Building the IRX modules](irx-modules.md).

Each IDB is addressed by module name, as everywhere else in this project:
`instance=SDFIOP.IRX`, `instance=SDFIOPFL.IRX`, `instance=SDFSDMAN.IRX`. Ports move
between sessions.

All three are built against the same IOP library set and all three start the same way:
`if (!sceSifSetIOPrcvaddr()) sceSifSIF2Init();` then `sceSifInitRpc(0)`.

## SDFIOP.IRX, the module loader

Module name `sdf_iop_manager`. Entry creates one thread at priority 66 which registers
an RPC server on SID `0x6F496453` ("SdIo") and never returns.

The server has one function, number 1. Its request block is

| Offset | Field |
| --- | --- |
| +0x00 | length of the path string |
| +0x04 | argument count |
| +0x08 | NUL-terminated path, then the argument block |

`sdfLoadStartModule` opens the path through `ioman`, seeks to the end for the size,
allocates that much with `AllocSysMemory`, reads the whole file, closes it, hands the
buffer to `LoadModuleBuffer`, frees the buffer and calls `StartModule`. It reads the
file itself rather than using `LoadStartModule` because the path lives on a device
`modload` cannot reach on its own.

Its error codes are returned to the EE in the reply word: -203 open failed, -204 seek
or read failed, -201 zero-length file, -400 out of memory. A `LoadModuleBuffer` failure
is passed through as whatever `modload` returned.

`sdfIopPrintf` (0x0) is `vsprintf` into a 1 KiB stack buffer followed by `printf`. The
same function appears in `SDFIOPFL.IRX`.

## SDFIOPFL.IRX, file I/O forwarded to the EE

Module name `sdf_iop_file_rpc`. This one is a client, not a server: it binds to RPC
server `0x66504F49` ("IOPf"), which lives on the EE, and polls in a spin loop until the
bind completes. It then registers a library named `sdfiopfl` so other IRX modules can
call it.

The library exports four entries, at the usual indices 4 through 7:

| Ordinal | Export | RPC func | Request | Reply |
| --- | --- | --- | --- | --- |
| 4 | `sdfIopFileReadOpen` | 32 | path, padded to a multiple of 4 | fd |
| 5 | `sdfIopFileGetFileSize` | 33 | fd | size |
| 6 | `sdfIopFileRead` | 34 | fd, buffer, length | bytes read |
| 7 | `sdfIopFileClose` | 35 | fd | none |

Every call takes a semaphore first, so the four are serialised against each other. The
send and receive buffers are fixed 256-byte blocks in `.bss` and the reply is always
read as one word.

The direction is worth stating plainly: the file system lives on the EE. The IOP asks
the EE to open and read files, not the other way round.

## SDFSDMAN.IRX, the sound driver

Module name `sdf_sound_driver`. It exports nothing and has no entry in the section
header's entrypoint field that IDA recognises; the real entry is `sdfSdManInit` at 0x0,
which IDA had split into a stray 8-byte function plus the body at 0x8.

`sdfSdManInit` prints ` sdfSdMan version 0.0` and starts three threads:

| Priority | Thread | Purpose |
| --- | --- | --- |
| 40 | `sdfSndDrvRpcInit` | command dispatcher, never returns |
| 41 | `sdfSndSmgLoadThread` | loads a sound bank off a semaphore |
| 42 | `sdfAdpcmStreamInit` | ADPCM streaming |

A fourth thread, priority 30, is created later by `sdfSndInitSynth`: `sdfSndSeqThread`,
the tick thread. A fifth at priority 1 (`sdfSndKeepAliveThread`) does nothing but
`JoinThread(10000)` in a loop.

### How the EE drives it

Two channels run in opposite directions.

**EE to IOP, by SIF command.** `sdfSndSifCmdHandler` is registered on command 0. Each
arriving packet is memcpy'd into a 128-byte slot of a ring at `sdfSndCmdRing`, indexed
by `sdfSndCmdHead` modulo 256, and a semaphore is signalled. The copy length comes from
the packet itself: `16 * ((word[3] >> 28) + 1)`.

The dispatcher reads `word[3]` of the slot and splits it as

| Bits | Meaning |
| --- | --- |
| 27..20 | opcode |
| 19..16 | 4-bit parameter (volume, slot count, flag) |
| 15..0 | 16-bit argument |

and acknowledges each command by sending SIF command `0x80000001` back with a 24-byte
packet whose two payload words are a status code and the command's own tag from
`word[4]`. Code 31 is sent once at startup to say the driver is up; code 30 means
"command finished".

The command table:

| Opcode | Handler | Notes |
| --- | --- | --- |
| 0x00 | `sdfSndInitSynth` | argument is the address of the shared work area |
| 0x01 | `sdfSndQuitSynth` | the dispatcher returns, ending the thread |
| 0x02 | `sdfSndBgmPlay` | |
| 0x03 | `sdfSndBgmStop` | |
| 0x04 | `sdfSndStopAll(0)` | all 16 slots |
| 0x09 | `sdfSndSePlay` | |
| 0x0A | `sdfSndLoadSmg(path, 0, id)` | path comes in the packet |
| 0x0B | `sdfSndUnloadBank` | via `sdfSndFindSlotById` |
| 0x0D | `sdfSndFadeOutById` | |
| 0x0E | `sdfSndRequestBgmSwitch` | |
| 0x0F | `sdfAdpcmStreamPlay(path, vol, 0)` | from file |
| 0x10 | `sdfAdpcmStop` | |
| 0x11 | `sdfAdpcmSetVolume` | |
| 0x12 | `sdfAdpcmStreamPlay(path, vol, 1)` | from memory |
| 0x13 | `sdfSndBgmResume` | |
| 0x14 | SPU2 master volume | `sceSdSetParam` 0x980/0x981 core 0, 0xA80/0xA81 core 1 |
| 0x15 | `sdfSndSetSeqVolume` | |
| 0x16 | `sdfSndSetListener` | |
| 0x17 | `sdfSndSePlay3d` | |
| 0x18 | `sdfSndFadeOutAll` | |
| 0x19 | `sdfSndFadeInAll` | |
| 0x1A | `sdfSndStopAll(1)` | slots 3..15, leaves the three BGM slots |
| 0x1B | `sdfSndLoadSmg(saved path, 1, id)` | reload through the EE |
| 0x1C | `sceHSyn` enable/disable | |
| 0x1D | `sdfAdpcmFadeOut` | |
| 0x1F | `sdfSndLoadVolTables` | copies the work area's tables into IOP memory |
| 0x20 | `sdfSndStoreVolTables` | the reverse, then DMAs the work area back |
| 0x21 | `sdfSndStopAll(2)` | |
| 0xC0 | nothing but `p2sdIrxSpuIopMap` | a status refresh |

Anything else prints `unknown command %04X - %04X` and posts error 14.

**IOP to EE, by RPC.** The driver binds as a client to EE server `0x54524E53` ("SNRT")
and calls it through `sdfSndCallEeRpc(arg0, buffer, length, func, reply)`, which sends a
16-byte request and copies a 16-byte reply back. The functions:

| Func | Meaning |
| --- | --- |
| 0, 1, 2 | ADPCM stream open, read, close |
| 3, 4, 5 | sound-bank file open, read, close |
| 6, 7, 8 | the same three against an in-memory ADPCM image |

So a bank load is either local (through the `sdfiopfl` library, which itself goes to the
EE) or driven directly over this RPC, selected by the `mode` argument threaded through
`sdfSndFileOpen`, `sdfSndFileRead` and `sdfSndLoadSmg`.

**The shared work area.** `sdfSndWorkArea` points at a block in EE-visible memory whose
EE address and size sit at +456 and +460. `p2sdIrxSpuIopMap` flushes the IOP D-cache and
DMAs the whole block back to the EE with `sceSifSetDma`, polling `sceSifDmaStat` with a
roughly 0.9 second complaint timeout. Every state change the EE needs to observe ends in
a call to it. Known offsets:

| Offset | Contents |
| --- | --- |
| +0x00 | bank count |
| +0x08, +0x0C | staging buffer and its size, used for the voice-data transfer |
| +0x10 | seven 16-byte bank records: size at +2, SPU address at +8, program data at +12 |
| +0x82 | ADPCM block size, 512 |
| +0x88 | IOP-side ADPCM buffer address |
| +0x190 | seven 8-byte bank status records: loaded id at +0, kind at +4 |
| +0x1C8 | error count, bumped on every `sdfSndPostError` |
| +0x1CC | last error code |
| +0x1E0 | 32-byte sequencer volume table |
| +0x260, +0x2E0, +0x360 | three 128-byte volume tables |
| +0x460, +0x4E0 | two per-core bytes |
| +0x560 | 2 by 32 voice map, rebuilt every tick from the sceHSyn voice table |

Error codes posted through `sdfSndPostError`: 1 file open, 2 bad header, 5 data size,
7 `sceMidi_SelectMidi`, 8 `sceMidi_MidiSetLocation`, 9 `sceHSyn_Init`, 10 `sceMidi_Init`,
14 unknown command, 17 `sceMidi_Load`.

### The sequencer

`sdfSndInitSynth` sets up SPU2 (both cores, reverb area at 0x1FFFFF and 0x1FFFF, master
volume 0x3FFF), builds the `sceMidi` and `sceHSyn` work areas, creates `sdfSndSeqThread`
and arms a hardware timer whose handler simply wakes that thread. Everything periodic
happens in the thread, not in the interrupt.

Each tick the thread calls the `sceMidi` and `sceHSyn` service entries, refreshes the
voice map, and then walks all 16 slots doing volume ramps, 3D re-panning for slots 3 and
up, and the deferred BGM switch. It finishes with the ADPCM fade step.

Slot state is held as 16 parallel arrays of stride 52 bytes starting at `sdfSndSlotId`:
id, group, volume, X, Y, Z, a 3D flag, a radius, a per-tick volume step, a fade-in flag,
an allocation serial and a resume location. Slots 0 to 2 are BGM, 3 to 15 are SE;
`sdfSndAllocBgmSlot` and `sdfSndAllocSeSlot` pick a free one, falling back to the
lowest-serial non-3D slot and stopping it.

Positional audio is integer-only: `sdfSndIsqrt` is Newton's method and `sdfSndAtan2Deg`
reads a 32-entry table at `sdfSndAtanTbl` covering 0 to 45 degrees and reflects it into
the other octants. `sdfSndDistanceAtten` clamps the result to 0..100 and
`sdfSndUpdate3dVoice` turns angle and distance into a pan sent as a MIDI controller
through `sceMSIn_PutMsg` plus a volume through `sceMidi_MidiSetVolume`.

### The .SMG bank format

`sdfSndLoadSmg` reads a 176-byte header and rejects anything shorter.

| Offset | Field |
| --- | --- |
| +0x14 | sequence data size |
| +0x1C | waveform data size |
| +0x24 | program data size |
| +0x2A | bank id, the `%04X` in `MIDI%04X.SMG` |
| +0x2F | bank kind, 0 to 6, selecting the target slot |
| +0x30 | 128-byte volume table, copied into the table for that kind |

Sequence and program data are read straight into the bank's work-area buffer. Waveform
data goes up in chunks of the staging buffer's size through
`sceHSyn_VoiceTrans`. Kind 4 has no program block. Kinds 1, 2 and 4 carry size limits
(0x69800 and 0x65000 of waveform) above which the driver evicts a neighbouring bank
first.

### ADPCM streaming

`sdfAdpcmStreamInit` waits on a semaphore, then reads a 16-byte header: byte 0 must be
0x0B and the word at +8 must be `APCM`. A second 16-byte block gives the sample length
in halfwords, a stereo flag, a frequency index and a reverb flag. The frequency index
selects a pitch from `sdfAdpcmPitchTbl`:

| Index | Rate | Pitch |
| --- | --- | --- |
| 0 | 48 kHz | 0x1000 |
| 1 | 44.1 kHz | 0xEB4 |
| 2 | 32 kHz | 0xAAA |
| 3 | 24 kHz | 0x800 |
| 4 | 22 kHz | 0x75A |
| 5 | 16 kHz | 0x555 |

Playback is triple-buffered in SPU2 RAM at 0x1E0000 with a 512-byte IOP staging buffer
and two voices for stereo. `sdfAdpcmFillBuffer` pulls the next 512 bytes over the EE RPC
and patches the ADPCM block flags by hand: the first block of a stream gets flag 6, the
body gets 2, and the last gets 3. `sdfAdpcmUploadBlock` DMAs a block to SPU2 with
`sceSdVoiceTrans` and moves the loop address on. The SPU2 interrupt handler
(`sdfAdpcmSpu2IntrHandler`) just signals a semaphore; the work happens on the thread.

Fades run on the sequencer tick through `sdfAdpcmFadeTick`, which steps a 4.4 fixed-point
accumulator and stops the stream when the volume reaches zero.

## Recovered types

The three databases carry no type library of their own, so every struct below was
reconstructed from the code and then checked against the stride the code walks the
object with. Each one's size matching its stride is the check that it is right, and six
of them matched on the first try:

| Type | Size | Stride it has to match |
| --- | --- | --- |
| `sdfSndWorkArea_t` | 1440 | the length the driver DMAs to the EE |
| `sdfSndSlot_t` | 52 | `sdfSndSlots` walked 13 ints at a time |
| `sceMidiSlot_t` | 564 | `sdfSndMidiSlotWork` walked 141 dwords at a time |
| `sceHSynSlot_t` | 1368 | `sdfSndHsynSlotWork` in `sdfSndInitWorkAreas` |
| `sceHSynSeq_t` | 1032 | `sdfSndHsynSeqWork`, and the module writes 1032 into its first word |
| `sdfSmgHeader_t` | 176 | the header read length |
| `sdfSndCmd_t` | 128 | the command ring slot |
| `SifRpcClientData_t` | 40 | `server` lands at +0x24, which is what the bind loop polls |

The IOP SDK types (`iop_thread_t`, `iop_sema_t`, `SifCmdHeader_t`,
`SifRpcClientData_t`, `SifDmaTransfer_t`, `SifCmdHandlerData_t`,
`sceSdEffectAttr_t`) are the standard layouts and are declared in all three databases.

Two findings fell out of typing rather than reading.

**The bank array has eight entries, not seven, and the eighth is the ADPCM buffer.**
`sdfSndSetBankAddr` initialises seven entries in one loop and eight in the next, and the
two loose offsets the ADPCM code uses, `workArea + 130` for a block size of 512 and
`workArea + 136` for the IOP staging buffer, are exactly `banks[7].size` and
`banks[7].data`. The stream buffer is a bank slot like any other.

**The bank-space limits read as numbers once the operand stops being an address.** IDA
had turned the constant 0x11C20 into `sdfSndMidiSlotWork[11].rest`, because it happens to
fall inside that array. The two checks in `sdfSndLoadSmg` are: bank kind 1 or 4 needs
waveform data no larger than 0x69800 and sequence plus program data under 0x5401; kind 2
needs waveform no larger than 0x65000 and sequence plus program no larger than 0x11C20.
Over either limit, the driver evicts a neighbouring bank and loads anyway.

### The command payloads

The 4-bit field and the 16-bit argument in `opt` carry the small commands. The ones with
more to say put a struct in the packet body at +0x14:

| Request | Layout |
| --- | --- |
| `sdfSndBgmReq_t` | `u32 id; u32 location; u16 vol;` (opcodes 0x02 and 0x13) |
| `sdfSndSeReq_t` | `u32 id; u32 unk04; u16 vol; u16 pan;` (opcode 0x09) |
| `sdfSndSe3dReq_t` | `u32 id; s32 radius; s32 x, y, z;` (opcode 0x17) |
| `sdfSndListenerReq_t` | eight words: the look-at point at +0x08 and the listener position at +0x14 (opcode 0x16) |
| `sdfIopLoadRequest_t` | `u32 pathLen; u32 argCount; char path[];`, SDFIOP's only RPC function |
| `sdfIopFileReq_t` | `s32 fd; void *buf; s32 len;`, SDFIOPFL functions 33 to 35 |

`vol` is read as a 16-bit value with two flags above the 7-bit level: 0x100 means start
silent and fade in at 64 per tick, and anything else at or above 0x80 clamps to 127.
`sdfSndBgmResume` builds one of these with `vol = 0x100`, which is how a resumed track
fades in.

### What is still untyped

`sdfSndUnloadBank` and part of `sdfSndLoadSmg` still read the work area through
`(char *)workArea + 16 * kind` and `+ 8 * kind`. The compiler folded the array's base
offset into the index, so the expressions are four and five words past a base that is not
the array, and no struct can make them resolve. The offsets are `banks[kind].size`,
`banks[kind].waveSize`, `banks[kind].data` and `progData[kind]`.

`thbase` ordinal 43 keeps its ordinal name. It returns a free-running counter that
`p2sdIrxSpuIopMap` differences against 33000000, about 0.9 s at the IOP's 36.864 MHz, and
that behaviour does not identify which clock reader it is.
