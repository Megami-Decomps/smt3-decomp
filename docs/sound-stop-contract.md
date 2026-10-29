# Sound stop command: payload and API contract

This is a **zero-credit prerequisite**, not a new function match. It preserves
`p2sdStopSe` and every existing caller's retail instructions while correcting
the reconstructed C object extent and making the visible declarations agree.

## Native payload and consumers

`p2sdStopSe` at `0x002C1730` reserves a `0x20`-byte frame. It writes the sound
identifier at `sp+0`, passes `sp` with length `0x10`, and saves the return address
at `sp+0x10`. The actual `sdfSndSendCmd` provider copies the requested length with
`memcpy`. The previous single-`s32` object passed with length 16 therefore did
not describe the object the native call reads.

The corrected source uses `u32 packet[4]`, initializes **only** `packet[0]`, and
passes `sizeof(packet)`. The three trailing words remain unspecified, as in
retail; zero-initializing them would add behavior and instructions.

The coherent interface is `void p2sdStopSe(u32 soundId)`. Fourteen direct call
or tail-call sites in ten native functions discard or overwrite the command
result. The tail wrappers `fldSeStop` and `fldMotionSeStop` and their script
callers are void. No address-taking reference was found. Declarations in
`field/fldSe`, `event/evtSound`, `result/rstcalc`, `newbattle/nbKoukaProcess` and
`facility/fclCombineInit` now agree with the provider.

This contract is corroborated by the qualified DDS implementation in
[code_002E87A8.c](https://github.com/Megami-Decomps/dds-decomp/blob/3419648e1c2ef3c9acbc55f149dd20c03a163d6c/src/dds1/game/code_002E87A8.c):
`void(u32)` with the same four-word payload and only word zero initialized.
All eleven native instructions correspond after masking only the call target.
Credit belongs to the Megami-Decomps DDS reconstruction / redthing1; matching
does not establish the original source spelling.

## Canonical field and rejected alternative

The field owner's two `D_002FC020[0]` accesses are the existing
`fldGlobalBase.sequenceCode` at `+0xA0`: a comparison against `0x80` and a clear
**after** the stop call. They now use that canonical member directly.

Retaining the previous reconstructed status-return signature caused the
40-byte `fldBgmStop` peer to use `$3` rather than native `$2` for its post-call
address. Ordinary compiler dumps with plain/`-da` output parity marked the
result `REG_UNUSED`; local allocation nevertheless assigned the adjacent
address pseudo to `$3`. Explicit discard and the canonical field did not
change that. No synthetic result consumer, reordered clear or match demotion
was used. The independently established payload/API correction resolves the
inconsistency instead.

## Qualification

With the official compiler and unchanged options, complete isolated owners
match: field **21/0**, sound **69/0**, event **10/0**, result **82/0**, kouka
**55/0**, combine **176/0** (match/differ). Affected guarded drafts compile.
`configure.py --force-split` followed by `ninja` passes the retail image check:
4,551,696 bytes, SHA-1 `5facfbf7916836613449c922e26a4ec412207b32`.

The spatial-sound function is qualified separately; this prerequisite adds no
matching bytes.
