# Matching notes: Dante's circular shadow

The native function computes the current and previous integer phases before
asking for either sine or cosine. It draws one inner triangle and one outer
quad per ten-degree step; both primitives use the same inner-vertex expressions.
The center and edge colors retain their original alpha and packed color bytes.

Share the truthful quad declaration: twelve floating-point vertex coordinates
and four packed unsigned colors. The old interleaved declaration happened to
route both kinds of values to the correct EE register banks at existing call
sites, but it changed how the compiler scheduled the shadow's argument setup.
The provider and the existing hit-mask caller remain byte-identical. The
hit-mask caller's existing local color binding is unchanged; removing it
changes that caller's native argument preparation. There is no new constraint.
