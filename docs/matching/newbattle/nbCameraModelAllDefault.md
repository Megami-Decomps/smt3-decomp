# Command-camera model fallback

`nbCameraGetModelAllTable` returns a shared override record when no formation
is available, its active bit is clear, or the debug table is disabled. The
record has the same three-float layout as the 384 roster overrides. Zero
radius selects the model's scaled radius; zero distance and elevation leave
those dimensions unoverridden. The 12 bytes at ROM 0x226950–0x22695B are the
writable zero record, with the next four alignment bytes left raw.

The consumer and provider share the previously recovered record type.
The consumer's emitted code, data, symbols and allocated relocations do
not change.
