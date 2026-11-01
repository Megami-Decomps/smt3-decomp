# Matching notes: newbattle/NbNegoFlowFullMoon

## Flow2_6 (newbattle/NbNegoFlowFullMoon, 0x278)

158 of 158 words match at 632 bytes. The Kotowari lookup corrects the old
draft's `qindex` row to native `ekucyo`: retail loads +0x184 at
0x274810/0x274860/0x274890, not `qindex` at +0x154. Its address is
`72 * ekucyo + 2 * nowselect`, with signed-byte type1/type2/type3 fields at
+6/+12/+0. The initial normal-question lookup still uses both indices,
`(7 * ekucyo + qindex) * 4`. No table bounds or layout are invented.

Both candidate additions remain first and ordered. Normal questions read
unsigned halfword type, then higher and lower bytes before either parameter
call. Signed parameter and unsigned-halfword level comparisons, 30/70 or
50 defaults, selection inversion and RNG(NULL,100) retain native behavior.
Unknown question types do not read nowselect for inversion. qtype is reloaded
after RNG; only fresh types 1..3 consume the signed Kotowari point value.
The final branch reads only the selected nextkouho slot and returns zero.

One truthful scalar carries the lower rate and later the signed point type,
with a fresh definition before every feasible consumer. Actual compiler RTL
shows shared pseudo87 assigned hard20 by global allocation. A paired source
with separate locals creates reciprocal interference between lower87 and
bonus88, allocated hard20/hard21, adding a save and increasing the frame to
0x40 and function to 644 bytes. Each source separately passed plain/dump
normalized full assembly, allocated-section headers and bytes, relocation and
symbol-contract, and function-inventory parity. This qualifies the live reuse;
it does not establish original variable names or source spelling.

The initial `bonus = 0` is separately **semantically dead**, not justified by
that live reuse. Its native residue at 0x2746AC is retained under the narrow
dead-code exception: deleting it breaks exactness; same-owner Flow2_5,
Flow2_9 and Flow2_13 show the live zero-default qtype-switch shape whose
unknown-type path really passes zero to nbDispNegoMessage. Related Flow7_22
corroborates the Kotowari residue, but its own dead initializer alone would
not establish the exception. No historical copying or fabricated live
consumer is claimed. Independent review accepted this distinction and the
amended source after renewed owner and plain/dump checks.

Two local bindings retain real higher in `$18` and Boolean success in `$16`;
these are ABI-preserved across their actual calls. One empty `+r` higher
operand retains the measured higher-before-lower byte-load order, without
emitting instructions or claiming a memory barrier. The ordinary control
has 24 differing words; higher binding alone leaves four, resolved by this
load-order constraint and success binding. No lower/bonus register alias
remains. These are target-specific reconstruction aids, not original-source
claims or an excuse to weaken the native-access checks.

Independent complete-owner verification gives 17 match, 0 differ, including
all 16 peers and owned data. Integrated retail checksum, 427 tooling tests,
guarded EE/IRX and report checks passed. Source commit
`2fb484d41aeb9b9ff27d38f2e68d331cdd85cb7d` has green exact-source CI
[41035377823](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41035377823).
