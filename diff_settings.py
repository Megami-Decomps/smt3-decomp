# asm-differ settings: python3 ../asm-differ/diff.py -mwo func_0019D958
# The image is a byte-for-byte copy of the retail ELF, so the ELF is the "rom".
import json
import os
from pathlib import Path

ROOT = Path(__file__).resolve().parent


def apply(config, args):
    version = os.environ.get("SMT3_VERSION", "smt3")
    serial = json.loads((ROOT / "config" / "versions.json").read_text())[version]["serial"]
    config["arch"] = "mipsee"
    config["baseimg"] = f"orig/{version}/{serial}"
    config["myimg"] = f"build/{version}/{serial}"
    config["mapfile"] = f"build/{version}/{serial}.map"
    config["source_directories"] = [f"src/{version}", "include", f"asm/{version}"]
    config["objdump_executable"] = "tools/bin/mips-ps2-decompals-objdump"
    config["make_command"] = ["ninja"]
    config["build_dir"] = f"build/{version}"
