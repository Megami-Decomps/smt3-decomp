# Null draw gate signature

`dds3NullDrawProc` (`0x001189D0`, 8 bytes) is already matched. This change
corrects only its reconstructed API to `u32 dds3NullDrawProc(void)`, the
convention used by every existing C caller. It earns **zero new matches**.

The entire native body is `jr $ra` with `daddu $v0,$zero,$zero` in its delay
slot. It reads no argument. The previous `dds3ProcessID_t *id` parameter was
unused and had no supporting caller or registration evidence. No dummy
argument, casted callback, alias, or compiler modification is needed.

An exact-symbol scan of source, headers, generated ASM and data finds only
direct gates: `dds3DrawProcess`, five functions in `effect/effBlur_Filter`,
`DrawMsgMng_Process`, and the assembly-backed `evtMoonProcess2`. There is no
address-taking, function-pointer registration or data reference that requires
the process-ID signature. All matched C consumers already declare `u32 (void)`;
only the provider declaration changes. Returning zero alone cannot distinguish
signedness, so the unsigned result convention comes from those existing users,
not from an assertion that native zero encodes a signedness.

The old IDA prototype export (`__int64 dds3NullDrawProc(int)`) remains an
unmodified historical input, not the authority for this source correction.
No IDB was updated.

On SMT3 base `e9255dbba166bb86f8cf8de9792c9ef785deb055`, **without** the new
DrawProcess implementation, whole-unit checks report:

| Owner | Match | Differ |
| --- | ---: | ---: |
| kernel/dds3DebugStuff | 11 | 0 |
| kernel/dds3KernelProcess | 3 | 0 |
| effect/effBlur_Filter | 30 | 0 |
| interface/itfMesManager | 118 | 0 |
| event/evtMoon | 15 | 0 |

`ninja` reproduces retail SHA-1 `5facfbf7916836613449c922e26a4ec412207b32`.
This is a separate prerequisite from the new 872-byte DrawProcess match.
