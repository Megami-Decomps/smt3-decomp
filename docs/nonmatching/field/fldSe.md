# Non-matching drafts: field/fldSe

## fldBgmBitChk (field/fldSe, 0x7C)

30 of 31 words match at the exact 124-byte size. The only difference is
`beqz` versus native `beql` at +0x2C; both restore the return address in the
delay slot. The area-0x22 gate, opposite event-bit tests and two separate
fade-out/play paths are preserved. `fldGb` is a scalar in `.data`, as in
`fldGlobal`; `gFldAreaBgm` is a signed halfword in `.data`. The actual
`EventBit` provider takes and returns `s32`, so its argument uses ordinary
signed-halfword promotion.

A qualified `--reorg --no-dumps --func fldBgmBitChk` trace of this complete
owner passes hooked/unhooked/plain output parity (`verify: OK`). The first
bit-result branch UID59 has probability 3999 and `mostly_true_jump` returns
0 (no opinion). Reorg first finds no usable instruction on its fallthrough
call path, then accepts return-address restore UID174 from the return path
with flags 0x11. It fills the slot without annulling the branch. The second
bit-result branch UID124 has probability 6001, returns 1 (weakly taken), and
accepts the same restore with flags 0x5. This confirms a delay-slot decision,
not a missing field load or return-value conversion.

The source-side reason for the native first branch's annul remains unresolved.
Earlier inverted/early-return forms and callee visibility did not close it.
No local reset, artificial dependency or alternate provider signature is used.
