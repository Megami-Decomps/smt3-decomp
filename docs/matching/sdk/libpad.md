# sdk/libpad

## scePadPortOpen

120 words against retail's 122. The same saved-register swap, plus the
double-buffer initialisation loop: retail keeps the buffer pointer and the
counter in registers the draft spends differently, so the loop body is a
register short and reloads.

The earlier `_send_to_iop` residual is resolved. Snapshot the DMA destination
before publishing the sequence, flush the command buffer, then construct the
descriptor in native field order. Under EE 2.9 that lifetime and ordering give
the native saved-register allocation and all 276 instruction bytes. The
diagnostic remains an external symbol because its data owner includes other
strings and padding.
