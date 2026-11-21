# Puzzle Boy editor constructors

The editor's four-entry constructor table starts at 0x327260. The
editor, game and help constructors take the same parent process handle
with a common panel-handle return type. The editor constructor is a
no-op returning null. The null fourth function pointer is intentional:
`pbEditInitProc` halts when the selected constructor is null. The
dispatcher and the three existing providers are unchanged.

Both stock modes produce the native 16-byte writable table, including
the three named pointer relocations and null entry. No section
attribute or compiler constraint is needed.
