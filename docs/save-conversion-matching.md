# Save revision conversion

`mc/slMain::slVersionConvert_Step3` at `0x001519A0` is exact C: **708 bytes**.
This is a native SMT3 reconstruction, not a DDS body transfer. The HD reference
contains an empty stub for this function. Existing project party and encyclopedia
record recovery supplies the field contracts; the retail instructions determine
the conversion. Credit the existing record recovery by **tge-was-taken**
(`e45754de0` encyclopedia record and `e352f9fb` unit layout), and the persistent
party view by **red thing** (`c7bc14dfc`). No credit is taken for those prior matches.

## Persistent storage and copy spans

`dds3GlobalWorkInit` allocates and clears `0x1A510` bytes and stores its allocation
handle at `+0x34`. Conversion saves that live handle before copying serialized
bytes, then restores it through the current `DDS3_GBWK` pointer. The local partial
view neither allocates storage nor changes the shared header.

The party view agrees with `newdata/datCalc.c`: sixteen `datUnitWork_t` records at
`+0xA54`, each `0xEC` bytes. The public record definition supplies `skill[24]` at
`+0x3A` and `keisyoskill[24]` at `+0x86`. The encyclopedia record is copied from
`facility/fclEncyc.c`'s recovered `fclencyceelem_t`: `0xB0` bytes, with the two
24-element arrays at `+0x1C` and `+0x58`. That provider places the table at
`+0x11E90` and enforces the current 184-record capacity. Unknown intervening
storage remains byte arrays, not invented scalar fields.

The earlier exact `slVersionConvert_GetSize` and load path establish the source
allocation lengths. Native conversion uses these spans:

| Revision | Operation |
|---|---|
| 1 | Copy `0x10` bytes to `+0`; next `0x1BF4` to `+0x30`; remaining `0xDD8C` to `+0x1CA4`. These consume exactly `0xF990` source bytes and preserve the native destination gaps. |
| 2 | Copy `0xFA30` bytes. |
| 3 | After the native diagnostic, clear `0x1A510`, copy `0x16DC0`, then remap skills. |
| 4 | After its distinct diagnostic, clear `0x1A510` and copy `0x16DC0`, without remapping. |
| Other | Copy the current `0x1A510`-byte image. |

Revision 3 visits all sixteen party slots and 168 encyclopedia slots, not a
runtime count. In each slot it visits both 24-element arrays. IDs in
`[0x100, 0x160)` increase by `0x20`; all others remain unchanged. The first loop's
`s32` temporary holds a zero-extended `u16` and produces the native full-width
range test. The other three tests read the `u16` fields directly and produce the
native narrowed subtraction. The increment cannot overflow a `u16` within that
range. No speculative validation or additional initialization is introduced.

## Compiler property and qualification

One ordinary `u32 bytes` variable carries the active byte extent through the
copy/clear phases. In revision 1 the first two values also advance the source
cursor; all later values are consumed by real memory operations before reuse.
This is same-kind live scalar reuse, not an artificial dependency or a false
library declaration. The existing canonical `memcpy` and `memset` prototypes and
normal compiler flags are unchanged.

The unhooked stock compiler trace identifies the deciding ordering: RTL expansion
emits real memory calls while `bytes` is pseudo 91. The `03.cse` pass propagates
its constants into the call arguments (for example instructions 53, 88 and 123
for the three revision-1 copies) but retains calls 54, 89 and 124. The final delay
slot pass retains the native calls and cursor arithmetic. A literal-size control
instead expands the copies inline. This agrees with the canonical DDS idiom for
reused byte extents; the source and measurement here are SMT3's own. Plain and
`-da` output for the frozen production source are identical after excluding only
the randomized `.file` path.

Direct current-owner lookups are retained where native reads `DDS3_GBWK`, rather
than imposing an entry snapshot across callbacks. The clear/copy pairs retain
their actual local destination and source snapshots. No selective reload, dummy
consumer, register binding, compiler modification or shared-header expansion is
needed.

On baseline `8d8c77514e25b620767d834d47914f4004be41a3`, the complete owner checks
**99 match, 0 differ**, including initialized data and existing C peers. Both
remaining guarded drafts compile. Force-split and `ninja` reproduce the complete
4,551,696-byte retail image with SHA-1
`5facfbf7916836613449c922e26a4ec412207b32`. The two padded diagnostic strings retain
their existing `INCLUDE_RODATA` owners. Focused unit-check, rodata, inline-string
and documentation-index suites pass 106 tests. Integration still runs the normal
combined repository gates.
