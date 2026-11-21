# Puzzle Boy debug color state

The sprite editor stores writable R, G, B and A as four signed 32-bit
channels at 0x326C50. The editor changes a selected channel in steps of 10
and clamps it to 0..255. The sprite renderer reads the low bytes of RGB
and blends the full alpha with the pulse. Native initial channels are
255, 255, 255 and 128. The preceding eight bytes remain raw.

The shared union is unchanged; the consumer's full stock assembly and
object are byte-identical. The new data owner matches all 16 native bytes.
