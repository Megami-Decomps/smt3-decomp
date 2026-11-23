# Matching notes: kernel/dds3TitleMain

## dds3Titlemain (kernel/dds3TitleMain, 0xD20)

840 of 840 words match at 0x00227930. All 29 linked dispatch slots match,
including the no-op mode 28. Separate mode 18 and mode 22 entries retain
their own timer and mode continuations. Equivalent cases 11, 20 and 23 share
a native target. Timer, mode and fade transitions use the current title
work when a real callback can change it.

The source reconstructs the native register and store phases with three
localized operands on real values: the case 12 increment and work output,
and the shared phase scalar paired with the real timer clear. These operands
emit no instructions. Removing each changes native output. Callback-mutating
ASan and UBSan models passed 200,000 cases each at O0 and O2.

### Recovering the title owner

The lower text boundary is 0x00226408, the first title cursor routine after
the six sound shims. The upper boundary remains 0x00229030. The native
boundary scan groups titleAllocWork through titleExit across the old cut:
both use the actual name at 0x0053DD98. This is corroboration, not proof of
the unique historical source layout, since a named external string could
also explain the common address.

The normal compiler packs the renderer's menu literals, the texture path,
the texture-loader table, the process name, the movie diagnostic and the
main dispatch table in one read-only input beginning at 0x0053DCD0. In a
separate main object the unshared diagnostic cannot be its first literal
at 0x0053DDA8: the actual switch table requires 16-byte object alignment.
The title start also agrees with the transition from sound to title
small data at 0x0054FD28. The original four pointer rows stay at 0x00342FD0,
with all ten relocated entries intact. The source retains the existing eight named
menu arrays so generation recognizes that the title unit defines them;
ordinary literal emission was a separate boundary probe.

A control restores the real data but retains the old text split: the
otherwise-identical native image differs in the title movie gate's branch
annul bit. With all real title providers in the common owner, that branch
matches as well. The regroup preserves the provider bodies, both tables,
the text and data of neighbors, and the complete retail executable.
