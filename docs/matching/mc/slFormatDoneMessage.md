# Format completion prompt

`slMain` passes `slFormatDoneMsg` to `slKanjiDrawH` when formatting completes.
The writable string occupies exactly 18 bytes at 0x528748, including its
terminating NUL. The next six zero bytes remain raw and are not an
extension of the string.

The existing native data-tail helper restores the last period and NUL to
the comparison reference only. The production link uses the C string.
