# Puzzle Boy fog preset

The Puzzle Boy draw path passes one twenty-byte fog record to
`dds3SetFog`. The setter copies the whole record without modifying it,
and both event-stage calls already pass the same record type. The
following twelve zero bytes remain separate alignment.
