# Camp panel-effect initialization

`cmpRandomizePanelEff` (`code_0022C578`, `0x0023E9C0`) matches all 472
instruction bytes. It initializes sixteen 44-byte animation records. The
existing updater and drawer establish the horizontal position, endpoints,
width and lifetime fields. The constructor proves that `reflected` at
`+0x1C` is one byte: native uses `sb` and `lbu`, leaving `+0x1D..+0x1F`
untouched. Ordinary struct alignment supplies these three padding bytes; no
explicit padding member or padding initialization is introduced. Actual EE
compile-time assertions preserve size 44 and offsets 28, 32, 36 and 40 for
reflection, total lifetime, remaining lifetime and color.

Each record consumes six ordered calls to the actual unsigned RNG provider,
`dds3GetRandA(void *)` in `code_002C0BA8`:

1. Width is `(random & 7) + 8`, scaled by sixteen.
2. The reflection flag is `random & 1`.
3. Horizontal position uses an unsigned remainder after the signed
   `width / 6` quotient, then scales by sixteen.
4. Vertical position uses an unsigned remainder by `height`, then scales by
   eight. Both current coordinates are copied into their start fields.
5. The horizontal target adds a random displacement bounded by signed
   `width / 3 - (entry->width >> 4)`; target Y copies start Y.
6. Total and remaining lifetime receive `(random & 7) + 1`.

The mode switch preserves all four explicit colors and the default color.
Reflection negates only horizontal current/start/target positions and width.
No new zero-divisor guard, padding initialization or random call is added.
The provider mutates its four-word RNG state, not the animation records.

## Actual caller domain

The removed guarded draft contained synthetic zero-divisor fallbacks. They
are unreachable on the retail caller domain, not equivalent for arbitrary
invalid external inputs. The call chain is `cmpAddQuePanelEff` through
`cmpAddPanelEffEntry` to this helper. Stock recovery supplies widths 280,
310 or 340 with height 21; status effects supply width 490 / height 25 or
width 114 / height 19. The zero-sized heart event uses kind 4, which the
entry dispatcher excludes from randomization along with kind 3. Only kinds
0, 1, 2, 5 and 6 reach this helper.

Thus width is at least 114 and height at least 19 on the known retail paths.
The first horizontal divisor is at least 19; the second is at least
`38 - 15 = 23`, because `entry->width >> 4` is 8 through 15 before reflection.
Direct unsigned remainder preserves the native behavior without adding an
unreachable special case. No claim is made that division by zero is defined.

## Source shape and stock-compiler evidence

The advancing entry cursor is acquired after the color switch. This matches
native's delayed preservation of argument `$7` at the dispatch join; using
the parameter itself as the cursor changes the prologue and loop alignment.
The lifetime is initialized as `total`, then copied to `left`.

The index counts upward in ordinary source, `for (i = 0; i < 16; i++)`, and
is not used by the body. Stock ee-gcc 2.96 reverses it into native's countdown.
Writing the countdown directly produces the same 472-byte extent but rotates
its saved register with the two divisor constants, leaving nine words different.

The reconstruction experiment at base `ef68c967` used paired ordinary and
`-da` compilations through `tools/cc.sh`, under the unchanged canonical
`src/smt3/code_0022C578.c` path and flags, to qualify both source forms.
For each source form, ordinary and `-da` full-owner assembly is identical
after normalizing only the wrapper's temporary directory names; every
allocated section and all 3,144 relocations also agree. Compiler SHA-256 is
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
These are stock RTL observations, not debugger-hook traces.

In the matching form, `09.loop` records BIV 89 initialized to zero, then
`Can reverse loop` and `Reversed loop and added reg_nonneg`. It hoists divisor
constants 6 and 3 at UIDs 288/290, and introduces countdown initialization 15
at UID 293. `20.greg` assigns counter pseudo 89 to `$20`, constant pseudo 109
to `$22`, and constant pseudo 126 to `$21`. The direct-countdown control
instead assigns its counter to `$22` and constants to `$21`/`$20`.
This establishes the source/compiler distinction, not the original spelling.

## Published-snapshot qualification

Source commit `f7d17230214f2740b17727ffa0e420ab2ce04541` contains only this
function and its truthful record-byte correction. The published owner blob
is `5c3542d88d0d516aa94e6939c06116550bbec71b`. Independent review checked
the actual parent closure, provider, byte layout and source-only scope.
The complete owner and bounded data check reports 370 matches and zero
differences; all 83 remaining guarded drafts compile, and all 794 shared
header layout assertions pass. The restored exact published source builds
to retail SHA-1 `5facfbf7916836613449c922e26a4ec412207b32`; a second build
has no work.

The frozen snapshot inherits one docs-index freshness failure: 9 of the
10 docs-index tests pass, while `docs/matching/INDEX.tsv` is stale. The
source match does not modify the documentation indexes or their generator.
This standalone note likewise does not regenerate unrelated indexes.
