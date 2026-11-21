# Matching notes: sdf/sdfTmx

## Memory-card module names

The loader selects one of two groups, then walks two pointers in that
group to build the paths passed to the module loader. The four native
pointers name the host and disc variants of mcman and mcserv. The SDK
identifier immediately after those pointers is a separate raw span.
