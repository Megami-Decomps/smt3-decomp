# Pipe part rotation matching

`PipemdlRot` matches 808 game EE instruction bytes, including its six-entry
jump table. Its input is the `getPartsInfo` record: incoming and outgoing
ways are unsigned bytes at offsets 2 and 3. Unknown ways retain zero angles.
The case body order is 1, 6, 3, 4, 2, 5, with ordinary conditional chains inside.
The complete native branch audit matters: for example 2/1 rotates X by -90,
3/1 rotates X by -90 and Y by 90, and 5/3 rotates X by 90 and Z by -90.

The native initial VU load reads the actual 16-byte stack vector before dispatch;
`FromEuler` subsequently produces the quaternion in vf10. The conversion factor
is a binary32 degree-to-radian literal, not a mutable external variable.
The existing SDK input-address-only `SDF_VU0_STOREVECTOR_NOMEM` hands the result
to `dds3SetRotPoseSet` immediately. No C scalar accesses the vector after that
store; the basic pointer at offset 0 is outside the vector at 0x20..0x2F.
The matched setter copies that vector through VU0. This is the same scoped
contract described in [unit visual matching](unit-visual-matching.md);
`fld_Npc.c` also uses this store followed by the same pose setter.

The broad-memory-clobber store control emits 804 bytes: it shares the final
vector address with the setter argument. The SDK store emits 808 and preserves
native address rematerialization and call scheduling. Both actual-owner
compiler probes pass normal/dumped/hooked assembly and all RTL parity.
No macro, register pin, artificial dependency or compiler flag was added.
