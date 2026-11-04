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

## Resume 5
Done: map10 (fld vectors, floats, shadow tables, 122) in decomp a7f2d560, map11-14 (131 strings: slMain, fclEncyc, effBattleSystem, fclCombineInit, fldAutoMap) in da577ece. Both on origin/main, gates green on the exact tree, IDB renamed and saved. D_00551888 stays unnamed (write-only). sdfport dname-bundle3 local: 5e300d4 and one commit for maps 11-14, unpushed. smt3-port hit (not edited): e2e/captures/f001_hold/scene/native/symbols.tsv.
