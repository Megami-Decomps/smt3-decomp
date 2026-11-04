# D_ rename handoff

Worktree /home/wsl/projects/smt3-decomp-dnames, branch dname-rename (merged to main at a00462c4).
Map used: see docs/idb-corrections.md "Data symbol renames" (44 symbols).
Method: grep src for the D_ extern, read use site, read bytes (file offset = vram - 0xff000),
rename in symbol_addrs (add a line for symbols splat auto-names), sed src/include, configure
--force-split, ninja, checksum, test_headers, check_nonmatching, check_generated_sources, IDB set_name via
ida.py py SLUS_209.11, idb_save. Names-only diff of smt3-port tools/gen_data_host.py output: scratch ROOT
with external/smt3-decomp symlinked to a built tree.
Skipped as unproven: D_0033A540 (0x5AC0 blob), D_002FC1A2, D_0030A2D8, rstcalc D_00339BF8/D_00339D10,
D_00345D88, D_00526B50 chain (11 AI records), sdata pool strings ("ON"/"OFF"/"END"...).
main.elf IDB hangs on every call; not used.
sdfport: branch dname-rename in /home/wsl/projects/sdfport-dnames, 9f8aea0, unmerged.
