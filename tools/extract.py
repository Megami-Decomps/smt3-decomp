#!/usr/bin/env python3
"""Extract the retail ELF from a disc image you own into orig/<version>/.

    python tools/extract.py [--iso path/to.iso smt3]
"""
# Without --iso the image is found by config/versions.json's `iso_glob` in the
# repository root and orig/. An extracted file with the recorded SHA-1 is kept.
from __future__ import annotations

import argparse
import hashlib
import io
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def load_versions() -> dict:
    path = ROOT / "config" / "versions.json"
    return json.loads(path.read_text()) if path.exists() else {}


VERSIONS = load_versions()


def find_iso(glob: str) -> Path | None:
    for base in (ROOT, ROOT / "orig"):
        hits = sorted(base.glob(glob))
        if hits:
            return hits[0]
    return None


def sha1(path: Path) -> str:
    digest = hashlib.sha1()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1 << 20), b""):
            digest.update(chunk)
    return digest.hexdigest()


def already_extracted(version: str) -> bool:
    """True when orig/<v>/<serial> is present and has the recorded SHA-1."""
    elf = ROOT / "orig" / version / VERSIONS[version]["serial"]
    if not elf.is_file():
        return False
    digest = sha1(elf)
    if digest != VERSIONS[version]["elf_sha1"]:
        raise SystemExit(
            f"{version}: {elf.relative_to(ROOT)} SHA-1 {digest} != expected "
            f"{VERSIONS[version]['elf_sha1']}; remove it to re-extract"
        )
    print(f"{version}: {elf.relative_to(ROOT)} already present ({digest})")
    return True


def extract(version: str, iso: Path) -> Path:
    import pycdlib

    info = VERSIONS[version]
    serial = info["serial"]
    out_dir = ROOT / "orig" / version
    out_dir.mkdir(parents=True, exist_ok=True)

    cd = pycdlib.PyCdlib()
    cd.open(str(iso))
    try:
        for name in (serial, "SYSTEM.CNF"):
            buf = io.BytesIO()
            cd.get_file_from_iso_fp(buf, iso_path=f"/{name};1")
            (out_dir / name).write_bytes(buf.getvalue())
    finally:
        cd.close()

    elf = out_dir / serial
    digest = sha1(elf)
    if digest != info["elf_sha1"]:
        elf.unlink()
        raise SystemExit(f"{version}: {serial} SHA-1 {digest} != expected {info['elf_sha1']}; wrong or modified disc")
    print(f"{version}: {elf.relative_to(ROOT)} OK ({digest})")
    return elf


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("versions", nargs="*", help=f"any of: {', '.join(VERSIONS)} (default: all)")
    ap.add_argument("--iso", type=Path, help="disc image (requires exactly one version)")
    ap.add_argument("--force", action="store_true", help="extract even if orig/ already matches")
    args = ap.parse_args()
    if not VERSIONS:
        ap.error("config/versions.json is missing or empty")
    unknown = set(args.versions) - VERSIONS.keys()
    if unknown:
        ap.error(f"unknown version(s): {', '.join(sorted(unknown))}")

    if args.iso:
        if len(args.versions) != 1:
            ap.error("--iso needs exactly one version")
        if args.force or not already_extracted(args.versions[0]):
            extract(args.versions[0], args.iso)
        return

    missing = []
    for version in args.versions or VERSIONS:
        if not args.force and already_extracted(version):
            continue
        iso = find_iso(VERSIONS[version]["iso_glob"])
        if iso is None:
            print(f"{version}: no ISO matching {VERSIONS[version]['iso_glob']!r}; skipped", file=sys.stderr)
            missing.append(version)
            continue
        extract(version, iso)
    if missing:
        raise SystemExit(f"no disc image for {', '.join(missing)}; pass --iso")


if __name__ == "__main__":
    main()
