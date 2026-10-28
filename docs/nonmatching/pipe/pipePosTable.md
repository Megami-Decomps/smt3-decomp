# Non-matching drafts: pipe/pipePosTable

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## pipeFileLoad (pipe/pipePosTable, 424 bytes)

103 of 106 words match at the exact 424-byte size. The whole structure is
retail's: the cached `D_005517E0` handle with the `fileLoad` only on NULL, the
`pacFileCheckEnd` early return of 0, the three header groups (count, offset)
read at 0x0/0x4, 0x8/0xC and 0x10/0x14 with their `DEBUG_PRINT` formats, the
`count << 5` and `count * 9` allocation sizes, `sdfMemGetPtr` plus the
`data + count * 8` tail store, the two-word source record passed to
`LoadLinkData` through the stack, and the `fileGetMemHandle`/`sdfMemFreeDirect`
/`fileHandleFree` teardown.

The three remaining words are a rotation of the first three `lw`s that read the
16-byte result back off the stack: retail loads 0x18, 0x1C, 0x10, the draft
loads 0x10, 0x18, 0x1C, and the fourth load and all four stores agree.

Ruled out:

- All 24 orders of the four member copies. `unk_00`, `unk_08`, `unk_0C`,
  `unk_04` (the order kept here) is the best at three words; `unk_0C`,
  `unk_08`, `unk_00`, `unk_04` is four and natural order is six.
- `*link = table;` as a struct assignment, which becomes an `ldl`/`ldr` and
  `sdl`/`sdr` pair and needs an extra `addiu` for the stack address, two words
  over size.
- Naming the two high words in locals before the stores, which fixes the load
  order and loses the stores instead.
- A separate `src` local per header group, 15 words out. What closes the
  second group's five words is naming its offset in a local
  (`offset = header[3];` ahead of the count); the first and third groups match
  with the offset read inline.
