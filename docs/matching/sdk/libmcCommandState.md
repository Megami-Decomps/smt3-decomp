# Matching notes: memory-card command state

The memory-card RPC client starts without an outstanding command and
with a semaphore ID of -1. Initialization creates the semaphore if its
ID is negative. The RPC paths store command IDs, and sceMcSync returns
the completed ID and clears the word to zero.

Each section attribute keeps its word in native writable .data.
Removing either attribute alone places just that word in .sdata in
both stock modes. The SDK version marker before these words and the
destructor data after them retain their complete raw bytes and labels.
