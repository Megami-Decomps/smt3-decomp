# A value held in a callee-saved float register shares a local with the tail


A local that is set and used inside one arm gets a call-clobbered register,
because its allocno crosses no call; one given its own name per arm is
block-local and lands in `$f0`. Where retail holds such a value in `$f20` or
another callee-saved register, the source is reusing **one** local that is
also live across a call somewhere else in the function. In both Spark
`BuffInit` functions the per-arm jitter scale is the same local as the tail's
`head.speedJitter`/`head.spinJitter`, and only that sharing puts it in `$f20`;
its own local gives `$f5`, three per-arm locals give `$f0`, and either one
costs the three `mul.s` registers and the schedule around them.
