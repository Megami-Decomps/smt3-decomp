# Kernel process-tree format

The process tree prints its indentation and label using the eight-byte
format at 0x54E8B0, including its terminating NUL. It now comes from
a C string in the original owner. The early incomplete declaration
preserves the native HI16/LO16 reference at the print call.

The next four zero bytes remain separate linker padding. Both stock
outputs reproduce the full retail small-data range and complete code
at their original addresses. No function body or compiler constraint
is changed.
