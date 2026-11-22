# Additional diagnostics in retained assembly

The field treasure and damage diagnostics and the pad-port error messages
use standalone strings. Their native
calls take the same addresses and arguments. Only text and terminators
move to C; neighboring padding and jump tables remain outside the objects.

The fusion birth diagnostic likewise retains its original format and arguments.
