# INF interaction dispatch

`field/fldInf::fldInfNext` is 1,376 bytes at `0x001405B0`. The implementation
adapts `fldDispatchInteractionAction` from
`Megami-Decomps/dds-decomp/src/dds1/game/code_00138C30.c`, credited to
**Megami-Decomps/dds-decomp / redthing1**. The donor complete-file SHA-256 is
`899b73e38d81072ea87e317ace30c4ce284126fc6d0e97bba1cec7fb28ef3d48`.

## Native contracts

The existing INF records already describe this serialization: four flag-selector
entries, 14-byte message rows with four signed-byte targets, 312-byte sets, 32-byte view records and
20-byte extra actions. The function reads the low signed halfword of each
32-bit action, as retail's `lh` does. No record or shared header is changed.

- IDs below ten select an event-flag branch; other IDs select a message target
  using the supplied slot. Branch targets and extra-action selectors retain
  native signed-byte interpretation.
- A target message clears and sets its two event flags, then obtains the
  active scene. Table indices are reread after the actual external calls,
  rather than incorrectly caching the pre-callback selected set.
- Extra actions call SMT3's actual healing, item and currency providers.
  In particular, action one calls `fldKaifukuTest`; it is not replaced by the
  DDS donor's differently named/no-op provider.
- SMT3 reads the full **signed** view byte at `0x00140844`. DDS masks its
  unsigned view byte with `0x7F`; that title-specific behavior is not ported.
  Native behavior is retained without adding a new bounds check.
- The existing basic payload's `hUnit` is at `+0x08`; its model owner is at
  event-unit `+0x8C`, with flags at model `+0x00`. Player visibility, optional
  position/camera names and both camera-reset switches follow the native
  branches. The player's basic and event-unit pointer are read again before
  final model-flag clearing, after callbacks that can change that state.
- Action four rereads its parameters after releasing scripts, calls
  `fldSetNextField(fldGlobalBase.area, command)` and `CallBattleSub2(command)`,
  conditionally writes the existing `wordCC`, and returns `command + 200`.
  The native field offsets are `+0x04` and `+0xCC`, not DDS's `+0x10`/`+0x24`.

Provider declarations follow their actual implementations: the active-scene,
name/ID search, current event-camera and follow-camera providers return basic
pointers; `dds3SetActiveCameraSceneBasic` returns the previous camera pointer.
The event-bit, item/currency, visibility, position, script and battle providers
retain their actual arities. The caller in `fldCommand_FLD_INF_NEXT` supplies
two integers and consumes the integer result.

## Existing camera-mode owner

Both camera switches read `fldGlobalBase.mode`, the already-recovered field
at `+0x18`. `fldGlobalBase` is at `0x002FBF80`, so this is precisely the native
`fldCamMode` address `0x002FBF98`. There is no new alias, fabricated array,
volatile qualifier or type-size change. A scalar-extern diagnostic emitted
GP-relative accesses and was rejected; reading the real existing record gives
the native HI/LO address construction and closes the complete function.

All 344 instruction words and both jump tables match. This required only the
native title differences and the canonical camera-mode owner, not an
allocation/scheduling grid or forced dependency. Plain and ordinary `-da`
compilations have identical assembly after removing only the temporary first
`.file` line; this is output-parity evidence, not a hooked compiler trace.

## Qualification

The source owner started from public `8293427a96375a2df1a2ef32cb77aac49e2074d7`.

- Complete `field/fldInf`: **7 match, 0 differ**, including generated local data.
- The unrelated `fldInfChkFastPanel` remains assembly-backed; its one guarded
  draft compiles. It is not credited as part of this change.
- Full retail build: SHA-1 **`5facfbf7916836613449c922e26a4ec412207b32`**.

No shared header, provider implementation, symbol definition, compiler option
or unrelated production fallback is changed.

## Fast-panel selector traversal (736 bytes)

`fldInfChkFastPanel` at `0x001402D0` completes the same owner. Its existing
reconstruction is credited to tge-was-taken (`76257a2f0`) and the later typed
record recovery (`3ef4ad06`); this match retains those records and operations.

The native loop carries the newly selected `value` into the next selector,
including when a message's kind is `-1`. Recursing on the previous `aline`
in the retained draft repeated the wrong selector. Returning the recursive
call with `value` expresses the real traversal; the compiler lowers it to
the native loop rather than adding a recursive call. The final ordinary
positive `(u16)kind < 2` block reproduces the native valid-kind dispatch and
branch-likely without an artificial dependency or a new type.

`fldDoor_GetHasigoTbl` uses its existing `s32(s32)` provider contract. The
selector table, signed action values, 14-byte message rows, callback-sensitive
global reloads and door-panel result codes remain unchanged. The combined
interaction owner has eight exact functions, including all three jump tables;
no guarded body remains in this owner.
