# Force-filter defaults

The writable 24-byte rectangle begins at the native color. The existing
force-filter editor references its blend and rectangle view at offset four,
with one actual relocation. That label names the remaining twenty bytes
inside the owner and does not allocate a second object.
