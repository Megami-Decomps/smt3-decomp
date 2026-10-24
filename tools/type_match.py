#!/usr/bin/env python3
"""Pick the record an invented type is, from ranked candidates, by hand.

The mechanical passes have taken the types whose evidence is decisive. What is
left needs a reading: two layouts side by side and the question of whether they
are the same record. This assembles the candidates and the evidence for each
remaining type, serves them as a page on localhost, and records each decision in
config/smt3/type_decisions.tsv, which --write-plans turns into the inputs
tools/hd_align.py and tools/hd_merge.py already take.

    python3 tools/type_match.py --serve          # then open the printed URL
    python3 tools/type_match.py --write-plans DIR
"""

import argparse
import json
import re
import sys
from collections import Counter
from http.server import BaseHTTPRequestHandler, HTTPServer
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import ida_members  # noqa: E402
import ida_type_votes  # noqa: E402
from hd_align import CONVENTION, our_types  # noqa: E402
from hd_merge import align, member_uses  # noqa: E402
from ida_fill import ida_sizes, layout, project_sizes  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
DECISIONS = ROOT / "config" / "smt3" / "type_decisions.tsv"
CACHE = ROOT / "build" / "type_match.json"
HTML = Path(__file__).resolve().parent / "type_match.html"
TOP = 14


def invented():
    return [l.split()[1] for l in
            (ROOT / "config" / "smt3" / "drift_baseline.txt").read_text().splitlines()
            if l.startswith("type ")]


def corpus_records(name, source):
    """name -> (size, [(offset, size, type, member)]) for one export file."""
    out, cur = {}, None
    path = ROOT / "config" / "smt3" / name
    if not path.exists():
        return out
    for line in path.read_text().splitlines():
        if line.startswith("T "):
            p = line.split()
            cur = p[1]
            out[cur] = [int(p[3]) if p[3].isdigit() else -1, [], source]
        elif line.startswith("M ") and cur:
            p = line.split(None, 4)
            out[cur][1].append((int(p[1]) if p[1].isdigit() else -1,
                                int(p[2]) if p[2].isdigit() else -1,
                                p[3], p[4].strip() if len(p) > 4 else ""))
    return {k: v for k, v in out.items() if v[1]}


def tree_records(ours, sizes):
    """Our own records, laid out, keyed by name; the first definition wins."""
    out = {}
    for name, defs in ours.items():
        for fields in defs.values():
            rows = layout([(ty.replace("*", ""), ty.count("*"), m, dims)
                           for ty, m, dims in fields], sizes)
            if rows:
                types = {m: ty for ty, m, _ in fields}
                out[name] = [rows[-1][0] + rows[-1][1],
                             [(o, s, types.get(m, "?"), m) for o, s, m in rows], "tree"]
                break
    return out


def stem(name):
    """`SdfDrawPacket` -> `sdfdrawpacket`, for comparing our name with theirs."""
    return re.sub(r"[^a-z0-9]", "", CONVENTION.sub("", name).lower())


def fold(rows):
    return {m.lower() for _, _, _, m in rows if m}


def score(mine, theirs, votes, ours_name, their_name, reliable):
    """(score, why rows) for one candidate."""
    my_rows, their_rows = mine[1], theirs[1]
    by_offset = {o: s for o, s, _, _ in their_rows}
    exact = sum(1 for o, s, _, _ in my_rows if by_offset.get(o) == s)
    offset_only = sum(1 for o, s, _, _ in my_rows if o in by_offset and by_offset[o] != s)
    names = fold(my_rows) & fold(their_rows)
    size_gap = abs(mine[0] - theirs[0]) if theirs[0] > 0 else 999
    a, b = stem(ours_name), stem(their_name)
    affinity = 1.0 if a == b else 0.6 if (a in b or b in a) else \
        0.3 if a[:4] and a[:4] == b[:4] else 0.0
    layout_part = (exact / max(len(my_rows), 1)) if reliable else 0.0
    total = (25 * min(votes, 4) + 30 * layout_part + 12 * affinity
             + 8 * min(len(names), 4) / 4 + (6 if size_gap == 0 else 0)
             - (8 if mine[0] > theirs[0] > 0 else 0))
    why = []
    if votes:
        why.append(f"{votes} IDB slot vote{'s' if votes > 1 else ''}")
    if reliable:
        why.append(f"{exact}/{len(my_rows)} offsets exact"
                   + (f", {offset_only} at the same offset only" if offset_only else ""))
    else:
        why.append(f"{len(their_rows)} members, offsets not comparable")
    if names:
        why.append(f"{len(names)} member name{'s' if len(names) > 1 else ''} agree: "
                   + " ".join(sorted(names)[:4]))
    if theirs[0] > 0:
        why.append(f"{theirs[0]} bytes against our {mine[0]}")
    return round(total, 1), why


def definition(path, name):
    """Our declaration as written, comments and all."""
    lines = path.read_text(errors="replace").split("\n")
    opener = re.compile(rf"\b(?:struct|union)\s+{re.escape(name)}\b|\btypedef\s+(?:struct|union)\b")
    closer = re.compile(rf"^\s*\}}\s*{re.escape(name)}\s*;")
    for i, line in enumerate(lines):
        if closer.match(line):
            depth, j = 0, i
            while j >= 0:
                depth += lines[j].count("}") - lines[j].count("{")
                if depth == 0:
                    return "\n".join(lines[j:i + 1])
                j -= 1
        if opener.search(line) and name in line and "{" in "".join(lines[i:i + 2]):
            depth, j = 0, i
            while j < len(lines):
                depth += lines[j].count("{") - lines[j].count("}")
                if depth == 0 and j > i:
                    return "\n".join(lines[i:j + 1])
                j += 1
    return ""


def users(name):
    """The functions whose source mentions our type, and where."""
    out = []
    pat = re.compile(rf"\b{re.escape(name)}\b")
    sig = re.compile(r"^([A-Za-z_][\w \t\*]*?)\b(\w+)\s*\(([^;{}]*)\)\s*\{", re.M)
    for path in sorted((ROOT / "src").rglob("*.c")):
        text = path.read_text(errors="replace")
        if not pat.search(text):
            continue
        for m in sig.finditer(text):
            if pat.search(m.group(0)):
                out.append(f"{m.group(2)} ({path.name})")
    return out[:12]


def build():
    sizes = project_sizes(ida_sizes())
    ours = our_types()
    mine_all = tree_records(ours, sizes)
    idb = {k: [v[1], [(o, s, t, m) for o, s, t, m in v[2]], "idb"]
           for k, v in ida_members.load(ROOT / "config" / "smt3" / "ida_members.txt").items()
           if v[2]}
    hd = corpus_records("hd_types.txt", "hd")
    il2 = corpus_records("hd_il2cpp_types.txt", "il2cpp")
    votes, why = ida_type_votes.collect()
    uses = member_uses()

    pool = {}
    for source in (il2, hd, idb, mine_all):   # later sources win on a name clash
        for k, v in source.items():
            pool[k] = v
    reliable = {"tree", "idb"}

    out = []
    for name in invented():
        mine = mine_all.get(name)
        if not mine:
            continue
        ranked = []
        for other, theirs in pool.items():
            if other == name or other in {n for n in invented()}:
                continue
            n = votes.get(name, Counter()).get(other, 0)
            s, rows = score(mine, theirs, n, name, other,
                            mine[2] in reliable and theirs[2] in reliable)
            if s <= 0 and not n:
                continue
            # Whether tools/hd_merge.py could take this pick as it stands, which
            # separates a decision that applies from one that needs the record read.
            held = ""
            if other in mine_all:
                pairs, split, blocked = align(
                    [(o, s, m) for o, s, _, m in mine[1]],
                    [(o, s, m) for o, s, _, m in theirs[1]], uses)
                held = blocked or ""
                if not held and mine[0] > theirs[0] > 0:
                    held = f"ours is {mine[0] - theirs[0]} bytes longer"
            ranked.append({"name": other, "source": theirs[2], "score": s, "why": rows,
                           "kind": "merge" if other in mine_all else "rename",
                           "size": theirs[0], "rows": theirs[1], "held": held,
                           "evidence": why.get((name, other), "")})
        ranked.sort(key=lambda c: -c["score"])
        first = sorted(ours[name])[0]
        out.append({"type": name, "size": mine[0], "rows": mine[1],
                    "files": sorted(p.relative_to(ROOT).as_posix() for p in ours[name]),
                    "users": users(name), "candidates": ranked[:TOP],
                    "source": definition(first, name)})
    index = [{"name": k, "source": v[2], "size": v[0], "members": len(v[1]),
              "kind": "merge" if k in mine_all else "rename"}
             for k, v in sorted(pool.items()) if k not in set(invented())]
    return {"types": out, "index": index,
            "records": {k: v[1] for k, v in pool.items()}}


def load_decisions():
    out = {}
    if DECISIONS.exists():
        for line in DECISIONS.read_text().splitlines():
            if line.strip() and not line.startswith("#"):
                p = (line.split("\t") + ["", "", ""])[:4]
                out[p[0]] = {"target": p[1], "kind": p[2], "note": p[3]}
    return out


def save_decisions(decisions):
    rows = ["# From tools/type_match.py. ours<TAB>target<TAB>kind<TAB>note;",
            "# an empty target is a type looked at and left alone."]
    for ours_t in sorted(decisions):
        d = decisions[ours_t]
        rows.append(f"{ours_t}\t{d['target']}\t{d['kind']}\t{d.get('note', '')}")
    DECISIONS.write_text("\n".join(rows) + "\n")


class Handler(BaseHTTPRequestHandler):
    payload = []
    index = []
    records = {}
    decisions = {}

    def log_message(self, *a):
        pass

    def _send(self, body, kind="application/json"):
        body = body if isinstance(body, bytes) else body.encode()
        self.send_response(200)
        self.send_header("Content-Type", kind)
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def do_GET(self):
        if self.path.startswith("/api/record"):
            name = self.path.split("name=")[-1]
            self._send(json.dumps({"name": name, "rows": self.records.get(name, [])}))
        elif self.path.startswith("/api/types"):
            self._send(json.dumps({"types": self.payload, "index": self.index,
                                   "decisions": self.decisions}))
        else:
            self._send(HTML.read_text(), "text/html; charset=utf-8")

    def do_POST(self):
        body = json.loads(self.rfile.read(int(self.headers.get("Content-Length", 0))) or b"{}")
        name = body.get("type")
        if body.get("clear"):
            self.decisions.pop(name, None)
        elif name:
            self.decisions[name] = {"target": body.get("target", ""),
                                    "kind": body.get("kind", "skip"),
                                    "note": body.get("note", "")}
        save_decisions(self.decisions)
        self._send(json.dumps({"saved": len(self.decisions)}))


def write_plans(out_dir):
    decisions = load_decisions()
    out_dir.mkdir(parents=True, exist_ok=True)
    renames = [(a, d["target"]) for a, d in sorted(decisions.items())
               if d["target"] and d["kind"] == "rename"]
    merges = [(a, d["target"]) for a, d in sorted(decisions.items())
              if d["target"] and d["kind"] == "merge"]
    report = {"batches": [{"batch": "type_match", "result": {"functions": [
        {"name": "type_match", "types": [{"ours": a, "hd": b,
                                          "evidence": "picked in tools/type_match.py"}],
         "members": []} for a, b in renames]}}]}
    (out_dir / "renames.json").write_text(json.dumps(report, indent=1))
    (out_dir / "merges.tsv").write_text("".join(f"{a}\t{b}\n" for a, b in merges))
    print(f"{len(renames)} renames -> {out_dir / 'renames.json'}\n"
          f"  python3 tools/hd_align.py {out_dir / 'renames.json'} --resolve --apply --fix")
    print(f"{len(merges)} merges -> {out_dir / 'merges.tsv'}\n"
          f"  python3 tools/hd_merge.py {out_dir / 'merges.tsv'} --apply --fix")
    skipped = sum(1 for d in decisions.values() if not d["target"])
    print(f"{skipped} types looked at and left alone")


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--serve", action="store_true")
    ap.add_argument("--port", type=int, default=8787)
    ap.add_argument("--rebuild", action="store_true", help="ignore the cached candidates")
    ap.add_argument("--write-plans", type=Path, metavar="DIR")
    args = ap.parse_args()

    if args.write_plans:
        write_plans(args.write_plans)
        return 0

    if args.rebuild or not CACHE.exists():
        print("assembling candidates (a minute or so) ...")
        data = build()
        CACHE.parent.mkdir(parents=True, exist_ok=True)
        CACHE.write_text(json.dumps(data))
    else:
        data = json.loads(CACHE.read_text())
    payload = data["types"]
    print(f"{len(payload)} types, {sum(len(t['candidates']) for t in payload)} ranked "
          f"candidates, {len(data['index'])} records to search "
          f"({CACHE.relative_to(ROOT)}; --rebuild to refresh)")

    if not args.serve:
        for t in payload[:10]:
            best = t["candidates"][0] if t["candidates"] else None
            print(f"  {t['type']:32s} {t['size']:5d}B  "
                  + (f"{best['name']} ({best['score']}, {best['kind']})" if best else "-"))
        return 0

    Handler.payload = payload
    Handler.index = data["index"]
    Handler.records = data["records"]
    Handler.decisions = load_decisions()
    server = HTTPServer(("127.0.0.1", args.port), Handler)
    print(f"http://127.0.0.1:{args.port}/   (decisions go to "
          f"{DECISIONS.relative_to(ROOT)}; ctrl-c to stop)")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nstopped")
    return 0


if __name__ == "__main__":
    sys.exit(main())
