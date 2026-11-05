# Matching notes: interface/lstListWindow

## lstDrawListWindow (interface/lstListWindow, 0x84)

33 of 33 words match at the native 132-byte size. Two localized input-only
empty operand constraints preserve native scheduling of genuine live values.
Neither binds a register, emits an instruction or declares a memory barrier.
They are target-specific reconstruction aids, not original-source evidence.

The signed-byte parameter is promoted to an initialized signed word. Reading
that real value through a constraint after the first panel draw retains its
native sign-extension before the call. Its actual consumer remains the zero
test after the next draw; emitted code keeps it in a callee-saved register
through both calls. The other constraint reads the initialized final sprite
ID immediately before that ID's real call argument use, keeping its full
constant materialization before the ordering-table argument move.

Qualified configured-compiler snapshots distinguish these dependencies. With
only the cursor constraint, the final sprite's high constant, ordering-table
move and low constant are scheduled in that order, and reorganization places
the low constant in the call delay slot. The second real-value constraint
keeps high and low constant together; the ordering-table move then occupies
the native delay slot. No unobserved scheduler ranking decision is claimed.

Independent deletion controls retain declarations, initializations and scopes:
removing the cursor constraint leaves 28 of 33 words matching; removing the
sprite constraint leaves 31 of 33; removing both restores the ordinary 26 of
33. All retain the native extent. Existing read/write volatile constraints at
both sites also match, but the scalar READ macro states the accurate input-only
contract and follows the existing READ_PAIR family. Input-only extended asm is
implicitly volatile, despite omitting the keyword; this does not make it a
general ordering guarantee for memory accesses.

Calls and all arguments remain panel draw, sprite 0x53001, optional up/down
cursor update, sprite 0x5101B, cursor-slot update and tail panel update. The
cursor helper retains its two-argument ABI, including the ordering-table
argument. No extra work-memory access or callback is introduced.

The list record now contains an actual 0x40-byte fclPanel_t as its first
member, rather than casting its old byte-array prefix to an unrelated type.
Initializer, draw and update providers receive this real subobject through
matching declarations. The flag access uses panel.Flag. Both cursor loops
use the existing slotFlags array at +0x59 instead of indexing beyond an
obsolete short prefix array. Native addresses, widths and control flow remain
unchanged, with no new bounds or input assumptions. The original layout is
preserved: list work 0x194 bytes, cursor 0x38, position +0x40, slot flags +0x59
and list cursor +0x15C. Declared embedded records therefore retain their sizes.
The existing memset declaration is made unconditional for strict compilation.

The separate initializer-position caller's unsigned width/height contract is
unchanged. The old note derives its zero-extension from native call masks;
the other owner's signed-width/destination declaration discrepancy is not
resolved by this draw-function change. It does not justify a panel-prefix cast.

Independent semantic review and 156 fresh builds cover the 13 affected owners,
baseline/revised headers, production/SKIP_ASM/retained-C and plain/dump output.
Every same-source full assembly/ELF parity pair passes. Header/type recovery
preserves all allocated code/data, extents, non-file symbols and resolved
relocations across all contexts. Private forced-header comparisons differ only
in filename/debug metadata and its physical ELF offset; these are inspected
separately. Both complete-owner checkers pass all affected owners and both
provider owners. The target owner advances from 10 to 11 exact functions,
with all ten peers and owned data preserved. Strict warning-as-error builds
pass in all three modes. Final retail, generated-source, regression, report
and production-provenance checks accompany the source landing.

Earlier ordinary controls remain closed: wider parameter plus cast, separate
signed locals, boolean local and direct parameter test did not recover native
conversion scheduling. No repeated width or register permutation search was
needed for the final two real-value dependencies.
