# Shared movie records and player API

This is a zero-new-match prerequisite: the already recovered movie and decoder
records now live in `include/sdf_movie_types.h`, rather than two divergent local
copies and a padded viewer alias. No allocation, boundary, data or decode body
is added. Layout assertions belong in `tools/header_layout.c`, not built headers.

## Native storage and actual providers

`movieOpenStream` clears `0xB0` bytes at its first argument. The existing
`sdfMovieCb_t` has its `0x8C`-byte `decodeCb` at `+0x24`; the viewer cadence byte
at `0x00343E7A` is `D_00343E40.decodeCb.fps`, not an independent padded object.
The displayed frame count and frame are the same decoder's whole-object
`+0x64` and `+0x6C` fields. All four owners use that one record identity.

The native indexed table at `0x00343BE8` advances by 24 bytes. Its first word
is the movie path pointer, followed by a 20-byte `sdfMovieInitParm_t` descriptor;
`dds3PlayMovieByIndex` at `0x0022B768` loads the former and takes the address of
the latter. For example, the first pointer is `0x0053F3B0`, the string
`cdrom0:/movie/dds.IPS`, not an integer movie number. Its following descriptor
bytes are `007000798000ff000020000e0000000000001e00`. The separate viewer
initialization descriptor at `0x00343EF0` uses the same actual provider type.
`movieOpenStream` consumes `(sdfMovieCb_t *, sdfMovieInitParm_t *, const char *)`.

`sdfIpuStrmDecodeInit` stores a decoder read callback and its owning movie
context; its two real read calls supply that `sdfMovieCb_t *`. Thus the old
`sdfMovie.c` duplicate's `movieStream_t *` is corrected to the decoder owner's
existing type. The live decoder byte at `+0x65` retains the existing movie
name `strmToIpuDmaStep`, including the decoder's loop restart store. No fields
or extents are invented. `SdfStreamRead` describes only this decoder callback;
the legacy casts passed to `sdfFileCmdReadOpen` have a different file-command
ABI and are deliberately not redesigned or exported as a canonical contract.

## The player and index wrapper return no value

The canonical signatures are
`void moviePlayFile_27041744(const char *, sdfMovieInitParm_t *)` and
`void dds3PlayMovieByIndex(s32)`. This is source/consumer evidence, not a way to
control register allocation:

- Recovered IDA prototypes, `config/smt3/ida_protos.txt` entries 5687–5688,
  identify both as void. The HD reconstruction also declares the player void.
  Its extra Layer/transport arguments are not imported: native reads exactly
  two player arguments and one index argument.
- The exact DDS1 equivalents, `mnuStartMovieDrawTaskForResource` and
  `dds3PlayMovieByIndex`, are both void in `game/code_0026EC90.c:515–525` at
  DDS commit `f79f86dc00f7fbd266fd5144e71faa86bef12aa4`.
- All 13 direct/tail sites across six native consumers discard the result:
  facility `0x00211A14`; sound `0x00227894`; title `0x00227DE8`, `0x00227E00`,
  `0x00227FA8`, `0x0022820C`, `0x00228414`, `0x002284C4`, `0x00228610`;
  manager `0x0022B794`, `0x0022B890`; viewer `0x0022BE98`, `0x0022BEB8`.
  The title joins generate their own zero result; there are no address/data
  references registering either leaf as a value-returning callback.

The player happens to leave `PlayingNo` in `v0` on its busy path and the new
process pointer there on its start path (`0x0022B710`, `0x0022B758`). Those are
residual register bits, not evidence of an intentional return-value API. The
old non-void definition had no return statement; an investigated explicit
`return PlayingNo` reconstruction was rejected and is not part of this patch.
The sole extra source consumer, `facility/fclTerminalUpdate.c`, receives only
the truthful void declaration change; its body is unchanged.

The viewer's `movieViewerStep` declaration follows its actual `void *(void)`
provider. The process callback takes the real `dds3ProcessID_t *` passed by
`RunProcess` (`processfunc(pro)`); it need not use that argument. The local
StartProcess/KillProcess declarations follow their existing kernel providers,
not integer/function-pointer approximations.

The guarded `titleAllocWork` consumer in `sound/sndManager.c` also declares the
index wrapper `void`. Its native call at `0x00227894` passes index 13, then the
first following instruction at `0x0022789C` overwrites `$2/v0`; no result is used.
This declaration-only reconciliation does not alter that guarded body or earn
matching credit.

## Qualification and credit

The prerequisite leaves `MovieViewerProc` behind its original ASM fallback.
All five complete owners and their native data match: viewer 7, manager 16,
movie 11, IPU 30, terminal 14. The retail image retains SHA-1
`5facfbf7916836613449c922e26a4ec412207b32`.

Existing record recovery and previously matched bodies remain credited to their
original contributors. PiM's changes unify those records and correct the narrow
API declarations; they earn no new function/byte credit. The additive viewer
promotion is documented separately in `movie-viewer-matching.md`.
