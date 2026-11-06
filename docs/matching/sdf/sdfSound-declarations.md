# Matching notes: sound provider declarations

## Provider declarations (sdf/sdfSound, byte-neutral)

The sound owner declares `WaitSema(s32)` before its RPC-handler callers and
exposes `sdfSndVoiceMapInit(char *, s32)` outside the retained-C guard. The latter
declaration matches the retained provider: retail stores the signed voice count,
does not read the information pointer, and its caller does not consume a result.
`WaitSema` keeps the SDK's signed semaphore/result contract.

`sdfDelayThread` takes `u32`, matching its canonical definition in `sdfSystem`.
The sound owner's only call supplies the constant 2. These declaration repairs
do not alter bodies, native calls, argument values, data, or assembly fallbacks.
Complete builds in production, SKIP_ASM, and NON_MATCHING contexts have identical
assembly and object output after filename-only metadata normalization. The
repaired source also passes strict declaration checks in these contexts. The
strict production diagnostic now sees both formerly implicit declarations;
the repository's required retained-C compilation gate already passed before
these edits and still passes afterward.
No additional function receives matching credit.
