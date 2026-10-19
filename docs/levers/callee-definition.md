# A caller can need its callee's definition, not just a prototype


Measured in `mc/slMain`: `slDrawInfo` at 183 of 189 words and `slDrawError`
at 293 of 299 both become exact the moment `slDrawWindow` is a C definition
above them, and while it is still `INCLUDE_ASM` they each emit six `bnel`
where retail has `bne`, on exactly the tests whose arms call it. Cracking one
callee therefore banked 801 words across three functions.

So when a caller sits within a couple of branch-annul words of exact and
calls something still in assembly, decompile the callee first rather than
permuting the caller.

It is not universal. An agent on `field/fldDraw` tested it directly, reverting
a converted callee to `INCLUDE_ASM` while changing nothing else, and its
caller still compiled exact; no near miss in that unit emitted a `bnel` for a
`bne` that a C callee fixed. Try it, but measure rather than assume.

It also runs the other way, which is worth knowing before you convert
something. `nbMaeCheckDone` in `newbattle/nbMakePacket` is **byte-exact on its
own** and is parked behind a `NON_MATCHING` fallback anyway, because compiling
it turns a neighbouring function's `bnel` into a `bne`. A matching function
can cost more than it earns, so check the whole unit's result after adding
one, not just the function you wrote. This is the same mechanism as the CONTEXT row below,
seen from the other side: what the compiler can see of the rest of the unit
changes the code it emits for one function.
