# Matching notes: sdk/libgraph

## sceGsResetPath (sdk/libgraph, 0x68)

**Assembly remains production.** The truthful C draft emits 100 bytes against
retail's 104; the earlier match claim is withdrawn.

`EE_FBRST_SET` reads COP2 control register 28 into its scratch, ORs the reset
bits and writes the result back. Its scratch is an output, not an input:
the previous input-only constraint hid an actual register write from the
compiler. The macro now uses `"=r"`, and its caller does not initialize the
output. A paired `"=&r"` control also emits 100 bytes. Keeping a read-write
operand merely to retain an otherwise unused zero setup has no justified
input consumer and was rejected. No invalid contract is retained for credit.

The pointer-valued volatile register accesses, actual two-quadword VIF packet,
register operations and return are retained in the guarded reference. The
native fallback preserves all 104 bytes, including its dead zero setup;
recovering that extra instruction needs an independently justified source
premise. The SDK's 2.10 assembler selection remains in effect, but is not by
itself evidence that an input-only overwritten operand is sound.

## sceGsSyncPath (sdk/libgraph, 0x314)

The C matches all 788 bytes under the SDK toolchain's 2.10 assembler. The
native function span also contains four bytes of alignment fill.

The packaging blocker was rodata ownership discovery, not missing native
strings. The ten register-dump format symbols are named inside the source-local
`DUMP_REGS()` macro, rather than directly in `sceGsSyncPath`. Previously,
`include_rodata.py` followed private C helpers but not macros, so it incorrectly
treated those ten external symbols as compiler-emitted literals and omitted
their native pieces. Following referenced source-local macro bodies preserves
all ten original symbol blocks, including their native alignment. No C padding,
forced string alignment, or new compiler constraint is needed.

An isolated native-file reproduction emits zero of the ten dump-string includes
with the old generator and all ten with macro traversal; both outputs are
stable on a second pass. The regression suite covers continued and nested
function-like/object-like macros, cycles, unused macros, and non-expansion in
comments, literals and bare function-like macro names. The ordinary C body is
unchanged by this packaging repair. Full-owner and retail gates must accompany
the promotion; exact function bytes alone do not verify rodata placement.

The body itself needed: the counter in the unused timeout parameter's register,
five spin loops with their rotated do-while shape and top tests, a
post-increment counter test against 0x1000000, two COP2 control-register-29
reads around the VU1 loop, the eleven-call diagnostic block written out at each
of the five timeout sites so cross-jumping merges the tails, a literal zero
return on the blocking path with the result local confined to the non-blocking
section, and five single-statement ifs that `-O2` turns into four conditional
moves.

## sceGsSetDefDispEnv (0x374)

892 bytes against retail's 884 instruction bytes, with the first 64 words exact.
The four-byte nop after the native extent is alignment, not target code. The assembler
pin does not touch this one: it has no unfilled delay slot. Matched: the
`sceGsGetGParam` call kept external, the `mode == 2 || mode == 3` range test,
the `checkModelVersion` path, the `_GetGsDxDyOffset` call with the four frame
addresses formed by OR-ing into the stack pointer, the zeroing block with its
descending stores and the mode halfword reloaded into both predecessors by
delay-slot filling, `pmode`, the three `smode2` stores that cross-jumping
merges into one, and `dispfb`.

Two findings closed most of it. The display-mode field is read unsigned for
the range test and sign-extended (`sll` 16, `sra` 16) for the dispatch and the
diagnostic argument, so it is not cached in a local; reading `param->mode` at
each use reproduces retail's reloads exactly. And `smode2` is written in each
arm of the nested if rather than selected into a local, which is what keeps
the three constants in branch delay slots: selecting a value gives a
conditional move instead.

The residual is in the NTSC and PAL display term. Retail adds the horizontal
constant to the frame word with a 64-bit `daddiu` and only then the
offset-times-magnification product, where plain C keeps that sum 32-bit.
Giving the term a 64-bit operand does produce the `daddiu`, but gcc then folds
the constant into the product instead and seven words are lost elsewhere, so
the draft keeps the 32-bit form. Retail also decrements the magnification in
the `mflo` register after both multiplies, where this shape needs one register
copy first, and that one extra instruction shifts the remainder of the
function.

The vertical terms are written offset-plus-frame-word-plus-constant and the
two multiplies in the order the horizontal term needs; both were read off
retail's operand order rather than guessed.

## sceGsExecStoreImage (sdk/libgraph, 0x68C)

419 of 419 words match at the 1,676-byte native extent. The following
alignment word is outside the function. The 59-entry switch table and its
four preceding diagnostic strings retain their retail offsets, including
the strings' padding. The entire graph owner and linked retail image match.

The store-image constructor and SDF capture caller establish the two-argument
interface and the 112-byte packet. The 12-bit dimensions are signed 32-bit
locals after extraction; even the largest rounded pixel product and packed
byte count fit. The 32-, 24-, 16-, 8- and 4-bit cases split the actual byte
count into eight-quadword DMA blocks, a quadword tail, and a byte tail.
Only a byte tail requires an uncached TRXREG update and a later signed
discard count. The 24-bit case computes that count separately after its
multiply by three. A negative discard count executes no iterations.

The target-specific operands keep the true rounded pixel products, shifted
quadword counts, and 24-bit remainders separate while they are still live.
In the 24-bit path, the raw pixel count and quadword count feed the byte and
DMA splits, while the tripled rounded count and the post-DMA count feed the
signed discard loop. The signed-product operands retain the arithmetic
shifts; they are not storage or integer-width changes. The only fixed
operand outside the dispatcher is the initial VIF1 fullness read in the byte
tail. That value goes straight to the real status mask, not across a call.
Ablating each retained aid changed the complete compiled owner; redundant
bindings and operands found by repeated ablation were removed. These are
compiler-scheduling aids, not claims about the original source.

The initial quadword index precedes the zero-tail guard. The aligned
quadword copy and byte copy use the same index for disjoint phases; the
discard loop has its own signed counter. The FIFO is read as a quadword,
with an aligned union providing its 16 byte values to the partial copy.
The DMA packet and destination writes each retain their native scratchpad
address handling.

All wait paths share an unsigned postfix timeout counter. The saved
interrupt mask is the return from `GsPutIMR(GsGetIMR() | 0x200)`.
Success clears VIF1 status and GS BUSDIR, restores the mask, acknowledges
FINISH, then writes the existing FIFO command. The early DMA exits only
print, FINISH timeout prints and writes the command, and later DMA/FIFO
timeouts print and reset CSR, BUSDIR, GIF control, and VIF1 FBRST. None of
those errors restores the mask. MMIO widths, order, and error exits were
checked in a bounded execution model of the emitted instructions, covering
all 64 format selectors, both DMA address classes, and seven seeded timeout
sites. This is not a hardware timing test.

### Display product/base lifetime controls (closed negative)

The native constructor truncates format, width, height and both offsets to
signed 16 bits on entry. Its width quotient feeds two real signed 32-bit
products before becoming magnification minus one. Separately, the signed
32-bit syscall output is sign-extended before the native 64-bit horizontal
base addition. The four syscall output addresses still name four-byte words;
this is not a widened memory-output ABI.

Explicit product phases emit 916 bytes; a constraint on the initialized
wide base emits 892. Binding the real quotient within its no-call branch
emits 900. The best tested corrected-base binding emits 892 bytes with
84 of 221 native-position words equal. All ten matching peers remain exact.
The diagnostic string must be restored as its native literal when the target
is activated, because the removed fallback had owned it; corrected complete
owner checks have no data diagnostic. These controls are private negatives.

The actual DISPLAY consumer transfers the entire 64-bit word to the GS.
Native assembly combines pack terms with whole-word OR, without masking each
hardware field individually. C bitfields would lose native spill bits and
are not a truthful replacement. An arithmetic model covers all 65,535 nonzero
signed-16 widths and 3,932,100 horizontal edge cases, including full signed-32
syscall offsets. It proves the tested horizontal low-12-bit equivalence, not
actual syscall outputs or the whole constructor. Width zero remains native's
division-trap case.

The vertical and mode-80 overflow audit is resolved below. No provider, flags
or ownership changes are promoted. A new matching control still needs evidence
beyond this product/base/quotient family.

### Defined masked coordinate addition

The syscall outputs have no established signed-range bound. The retained draft
now uses unsigned modulo-2^32 addition at each masked coordinate offset. This
repairs undefined signed overflow rather than improving a matching score:
mode 80 with `dy = 32767` and `offsetY = INT_MAX` previously overflowed before
the mask; the horizontal offset has the same issue. Native vertical `addu` /
`addiu` wraps, and reducing the horizontal wide sum modulo 2^32 before taking
its low 12 bits is equivalent. Products and sums before the offsets fit signed
32 bits for all signed-16 inputs and nonzero widths. Packing still ORs complete
64-bit terms without masking away native spill bits, and width zero retains
the native division-trap case rather than receiving an invented result.

The actual repaired expressions pass 47,775,096 whole-DISPLAY comparisons
against an independent native instruction-dataflow model under host UBSan.
Coverage includes every signed-16 width, all three supported modes, both
interlace/field classes, signed-16 edge tuples and signed-32 syscall-offset
extremes; this is not exhaustive over all combinations. This is an arithmetic
model, not PS2 hardware execution. Plain/dump assembly and object output agree
in all three compilation modes. The active draft remains 892 bytes against
884 native instruction bytes, with 80/221 native-position words equal (the
unsafe draft had 83/221). All ten matched peers remain exact and the production
guard retains the assembly fallback. No exact matching credit is claimed.

## sdfGrDispEnv (SDK display environment)

C owns the native 40-byte BSS object at `0x580FF0`, immediately before
the image-set callback state at `0x581018`. The common definition has
five 64-bit GS registers and the SDK's actual signed 16-bit scalar
parameters. A union exposes the same DISPFB register as a full 64-bit
value for the SDK and as low/high words for the game. The game's
frame-buffer-base patch touches only the low word, preserving the high
word. The uncached alias passed to the SDK constructor is unchanged.

Both reader objects are byte-identical. The section placement keeps
the object in its existing BSS input and prevents a COMMON allocation.
This corrects the cross-unit contract and gives C ownership of storage;
it adds no matching executable bytes.
