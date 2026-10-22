#!/usr/bin/env python3
"""Carry a dds-decomp names.tsv into that game's IDA database.

    python3 tools/dds_idb_names.py --instance SLUS_209.74 build/dds_names/names_dds1.tsv [--apply]

Dry run unless --apply: prints what would change. The IDB is addressed by module
name; rows are <0xADDR>\\t<name>\\t<kind>\\t<note> as tools/names.py takes them."""

import argparse
import base64
import json
import os
import sys
import zlib
from pathlib import Path

IDA_SCRIPTS = Path(
    os.environ.get("IDA_SKILL_SCRIPTS", Path.home() / ".claude/skills/ida-instances/scripts"))
PORTS = os.environ.get("IDA_MCP_PORTS", "13337-13437")

# py_eval execs this with split globals and locals, so the blob and the flag come
# in as arguments and the imports live inside the function.
PAYLOAD = r'''
def run(ROWS, APPLY):
    import base64, json, zlib
    import ida_name, idc
    rows = json.loads(zlib.decompress(base64.b64decode(ROWS)))
    out = {"renamed": 0, "same": 0, "failed": [], "nofunc": []}
    for addr, name in rows:
        if idc.get_func_attr(addr, idc.FUNCATTR_START) != addr:
            out["nofunc"].append(addr)
            continue
        if idc.get_name(addr) == name:
            out["same"] += 1
            continue
        if APPLY:
            if ida_name.set_name(addr, name, ida_name.SN_NOWARN | ida_name.SN_FORCE):
                out["renamed"] += 1
            else:
                out["failed"].append(addr)
        else:
            out["renamed"] += 1
    return json.dumps(out)

'''


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("names", type=Path)
    ap.add_argument("--instance", required=True, help="DDS IDB module name, e.g. SLUS_209.74")
    ap.add_argument("--apply", action="store_true", help="write the names; default is a dry run")
    args = ap.parse_args()

    rows = []
    for line in args.names.read_text().splitlines():
        if line.strip() and not line.startswith("#"):
            addr, name = line.split("\t")[:2]
            rows.append([int(addr, 16), name])
    blob = base64.b64encode(zlib.compress(json.dumps(rows).encode(), 9)).decode()

    sys.path.insert(0, str(IDA_SCRIPTS))
    import ida

    instances = ida.get_instances(ida.parse_ports(PORTS), refresh=False)
    port = ida.resolve_target(args.instance, instances)
    code = PAYLOAD + f"\nRESULT = run({blob!r}, {args.apply!r})\nRESULT\n"
    out = ida.call_tool(port, "py_eval", {"code": code, "new_locals": True})
    if out.get("stderr"):
        sys.exit(out["stderr"])
    result = json.loads(out["result"])
    verb = "renamed" if args.apply else "would rename"
    print(f"{args.instance}: {verb} {result['renamed']}, already named {result['same']}, "
          f"not a function start {len(result['nofunc'])}, failed {len(result['failed'])}")
    for addr in result["nofunc"][:10]:
        print(f"  no function at 0x{addr:08X}")
    for addr in result["failed"][:10]:
        print(f"  set_name failed at 0x{addr:08X}")
    return 1 if result["failed"] else 0


if __name__ == "__main__":
    sys.exit(main())
