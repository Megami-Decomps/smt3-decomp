#!/usr/bin/env python3
"""Render the reconciled progress reports as a static site for GitHub Pages.

    python3 tools/progress_site.py [smt3] [--out build/site] [--history FILE]
                                   [--repo URL] [--commit SHA] [--date ISO-8601]

Reads build/<v>/report.json (game scope, the headline) and
build/<v>/report.all.json (whole binary, by category), both written by
`ninja report`. FILE is the history.json of the previously published site; the
current commit is added to it, replacing an entry for the same commit. A
missing FILE starts a new history. Writes:

    index.html             headline, history chart, categories, units, functions
    history.json           one entry per published commit, oldest first
    report.json            the game-scope report the page was rendered from
    badges/code.json       shields.io endpoint badge: matched code bytes
    badges/functions.json  shields.io endpoint badge: matched functions

Only reconciled credit is shown, never raw objdiff: see docs/progress.md.
"""
from __future__ import annotations

import argparse
import html
import json
import os
import shutil
import subprocess
import sys
from datetime import datetime
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from check_nonmatching import draft_functions  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
VERSIONS = json.loads((ROOT / "config/versions.json").read_text())
MEASURES = ("matched_code", "total_code", "matched_functions", "total_functions")


def measure(measures: dict, key: str) -> int:
    # objdiff writes 64-bit counts as strings and omits zero-valued fields.
    return int(measures.get(key, 0))


def percent(matched: int, total: int) -> float:
    return 100.0 * matched / total if total else 0.0


def totals(measures: dict) -> dict:
    return {key: measure(measures, key) for key in MEASURES}


def merge_history(history: list[dict], entry: dict) -> list[dict]:
    """Add entry, replacing a re-run of the same commit, ordered by date."""
    merged = [item for item in history if item["commit"] != entry["commit"]]
    merged.append(entry)
    merged.sort(key=lambda item: datetime.fromisoformat(item["date"]))
    return merged


def badge(label: str, matched: int, total: int) -> dict:
    return {
        "schemaVersion": 1,
        "label": label,
        "message": f"{percent(matched, total):.2f}%",
        "color": "blue",
    }


def git(*args: str) -> str:
    return subprocess.run(["git", *args], cwd=ROOT, check=True,
                          capture_output=True, text=True).stdout.strip()


def number(value: int) -> str:
    return f"{value:,}"


def bar(matched: int, total: int) -> str:
    width = percent(matched, total)
    return f'<span class="bar"><span style="width:{width:.3f}%"></span></span>'


def state(function: dict, drafted: set[str]) -> str:
    if function.get("fuzzy_match_percent") == 100.0:
        return "ok"
    return "draft" if function["name"] in drafted else "todo"


def count_drafts(report: dict, drafts: dict[str, set[str]] | None) -> int:
    """Drafts in the published units: work in hand, never credit."""
    reported = {unit["name"] for unit in report["units"]}
    return sum(len(names) for name, names in (drafts or {}).items() if name in reported)


def chart(history: list[dict]) -> str:
    """Inline SVG of both percentages over time; no script, no external assets."""
    if len(history) < 2:
        return '<p class="muted">The chart starts once a second commit is published.</p>'
    width, height, left, right, top, bottom = 900, 260, 44, 12, 12, 28
    times = [datetime.fromisoformat(item["date"]).timestamp() for item in history]
    series = {
        "code": [percent(i["matched_code"], i["total_code"]) for i in history],
        "functions": [percent(i["matched_functions"], i["total_functions"]) for i in history],
    }
    ceiling = max(10, -(-max(max(v) for v in series.values()) // 10) * 10)
    span = (times[-1] - times[0]) or 1.0

    def x(t: float) -> float:
        return left + (t - times[0]) / span * (width - left - right)

    def y(v: float) -> float:
        return top + (1 - v / ceiling) * (height - top - bottom)

    parts = [f'<svg class="chart" viewBox="0 0 {width} {height}" role="img" '
             f'aria-label="Progress over time">']
    for step in range(0, int(ceiling) + 1, int(ceiling) // 5):
        parts.append(f'<line class="grid" x1="{left}" x2="{width - right}" '
                     f'y1="{y(step):.1f}" y2="{y(step):.1f}"/>'
                     f'<text x="{left - 6}" y="{y(step) + 4:.1f}" text-anchor="end">{step}%</text>')
    for label, when in ((history[0]["date"], times[0]), (history[-1]["date"], times[-1])):
        anchor = "start" if when == times[0] else "end"
        parts.append(f'<text x="{x(when):.1f}" y="{height - 8}" text-anchor="{anchor}">'
                     f'{html.escape(label[:10])}</text>')
    for name, values in series.items():
        points = " ".join(f"{x(t):.1f},{y(v):.1f}" for t, v in zip(times, values))
        parts.append(f'<polyline class="{name}" points="{points}"/>')
    parts.append("</svg>")
    return "".join(parts)


def source_link(repo: str | None, commit: str, path: str, text: str) -> str:
    if not repo:
        return html.escape(text)
    return f'<a href="{html.escape(repo)}/blob/{commit}/{html.escape(path)}">{html.escape(text)}</a>'


def render(version: str, report: dict, audit: dict, history: list[dict],
           repo: str | None, commit: str, date: str,
           drafts: dict[str, set[str]] | None = None) -> str:
    info = VERSIONS[version]
    game = totals(report["measures"])
    commit_text = html.escape(commit[:12])
    if repo:
        commit_text = f'<a href="{html.escape(repo)}/commit/{commit}">{commit_text}</a>'
    docs = source_link(repo, commit, "docs/progress.md", "how progress is measured")

    cards = []
    for title, matched, total, unit in (
            ("Code bytes", game["matched_code"], game["total_code"], "bytes"),
            ("Functions", game["matched_functions"], game["total_functions"], "functions")):
        cards.append(
            f'<div class="card"><h2>{title}</h2>'
            f'<p class="big">{percent(matched, total):.2f}%</p>{bar(matched, total)}'
            f'<p class="muted">{number(matched)} / {number(total)} {unit}</p></div>')
    # A draft earns no credit, so the card counts drafts against what is
    # still assembled rather than against the game.
    drafted = count_drafts(report, drafts)
    unmatched = game["total_functions"] - game["matched_functions"]
    cards.append(
        f'<div class="card"><h2>Non-matching drafts</h2>'
        f'<p class="big">{number(drafted)}</p>{bar(drafted, unmatched)}'
        f'<p class="muted">of {number(unmatched)} unmatched functions, credited as none</p></div>')

    categories = []
    for category in audit["categories"]:
        m = totals(category["measures"])
        units = measure(category["measures"], "total_units")
        if m["total_code"]:
            code = (f'{bar(m["matched_code"], m["total_code"])} '
                    f'{percent(m["matched_code"], m["total_code"]):.2f}%</td>'
                    f'<td class="num">{number(m["matched_code"])} / {number(m["total_code"])}</td>'
                    f'<td class="num">{number(m["matched_functions"])} / {number(m["total_functions"])}')
        else:
            code = 'not compared</td><td class="num">&ndash;</td><td class="num">&ndash;'
        categories.append(f'<tr><td>{html.escape(category["name"])}</td>'
                          f'<td class="num">{units}</td><td>{code}</td></tr>')
    whole = totals(audit["measures"])
    categories.append(
        f'<tr class="total"><td>Whole binary</td>'
        f'<td class="num">{measure(audit["measures"], "total_units")}</td>'
        f'<td>{bar(whole["matched_code"], whole["total_code"])} '
        f'{percent(whole["matched_code"], whole["total_code"]):.2f}%</td>'
        f'<td class="num">{number(whole["matched_code"])} / {number(whole["total_code"])}</td>'
        f'<td class="num">{number(whole["matched_functions"])} / {number(whole["total_functions"])}</td></tr>')

    units = []
    for unit in sorted(report["units"], key=lambda u: u["name"]):
        m = totals(unit["measures"])
        share = percent(m["matched_code"], m["total_code"])
        functions = sorted(unit.get("functions", []), key=lambda f: int(f.get("address", 0)))
        drafted_here = set((drafts or {}).get(unit["name"], ()))
        rows = "".join(
            f'<li class="{state(f, drafted_here)}">'
            f'<span>{html.escape(f["name"])}</span><span class="num">{number(int(f.get("size", 0)))}</span></li>'
            for f in functions)
        path = f"src/{version}/{unit['name']}.c"
        units.append(
            f'<details data-name="{html.escape(unit["name"].lower())}" data-code="{share:.6f}" '
            f'data-size="{m["total_code"]}"><summary>'
            f'<span class="name">{html.escape(unit["name"])}</span>'
            f'<span>{bar(m["matched_code"], m["total_code"])} {share:.2f}%</span>'
            f'<span class="num">{number(m["matched_code"])} / {number(m["total_code"])}</span>'
            f'<span class="num">{number(m["matched_functions"])} / {number(m["total_functions"])}</span>'
            f'</summary><p>{source_link(repo, commit, path, path)}</p>'
            f'<ol class="functions">{rows}</ol></details>')

    return TEMPLATE.format(
        title=html.escape(info["title"]),
        serial=html.escape(info["serial"]),
        commit=commit_text,
        date=html.escape(date),
        docs=docs,
        unit_count=len(report["units"]),
        cards="".join(cards),
        chart=chart(history),
        categories="".join(categories),
        units="".join(units),
    )


TEMPLATE = """<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>{title} decompilation progress</title>
<style>
:root {{ color-scheme: light dark; --fg: #1f2328; --muted: #656d76; --border: #d0d7de;
  --surface: #f6f8fa; --accent: #0969da; --ok: #1a7f37; --code: #0969da; --functions: #bf8700; }}
@media (prefers-color-scheme: dark) {{ :root {{ --fg: #e6edf3; --muted: #8d96a0; --border: #30363d;
  --surface: #161b22; --accent: #4493f8; --ok: #3fb950; --code: #4493f8; --functions: #d29922; }} }}
body {{ font: 15px/1.5 system-ui, sans-serif; color: var(--fg); max-width: 1100px; margin: 0 auto; padding: 24px; }}
a {{ color: var(--accent); }}
h1 {{ margin: 0 0 4px; font-size: 1.6em; }}
.muted {{ color: var(--muted); }}
.cards {{ display: grid; grid-template-columns: repeat(auto-fit, minmax(260px, 1fr)); gap: 16px; margin: 24px 0; }}
.card {{ border: 1px solid var(--border); border-radius: 8px; padding: 16px; background: var(--surface); }}
.card h2 {{ margin: 0; font-size: 1em; color: var(--muted); }}
.big {{ font-size: 2.2em; font-weight: 600; margin: 4px 0 8px; }}
.card .bar {{ display: block; width: 100%; height: 10px; }}
.bar {{ display: inline-block; width: 120px; height: 8px; background: var(--border); border-radius: 4px;
  overflow: hidden; vertical-align: middle; }}
.bar span {{ display: block; height: 100%; background: var(--ok); }}
.chart {{ width: 100%; height: auto; }}
.chart text {{ fill: var(--muted); font-size: 12px; }}
.chart .grid {{ stroke: var(--border); }}
.chart polyline {{ fill: none; stroke-width: 2; }}
.chart .code {{ stroke: var(--code); }}
.chart .functions {{ stroke: var(--functions); }}
.key-code {{ color: var(--code); }} .key-functions {{ color: var(--functions); }}
table {{ border-collapse: collapse; width: 100%; }}
th, td {{ text-align: left; padding: 6px 8px; border-bottom: 1px solid var(--border); }}
.total td {{ font-weight: 600; }}
.num {{ text-align: right; font-variant-numeric: tabular-nums; white-space: nowrap; }}
.controls {{ display: flex; gap: 8px; margin: 8px 0; flex-wrap: wrap; }}
input, select {{ font: inherit; color: inherit; background: var(--surface); border: 1px solid var(--border);
  border-radius: 6px; padding: 4px 8px; }}
.units summary, .units .head {{ display: grid; grid-template-columns: 1fr 200px 170px 110px; gap: 8px;
  padding: 6px 8px; border-bottom: 1px solid var(--border); cursor: pointer; }}
.units .head {{ font-weight: 600; cursor: default; }}
.units .name {{ overflow: hidden; text-overflow: ellipsis; white-space: nowrap; }}
.units details > p {{ margin: 8px 16px; }}
.functions {{ columns: 3 280px; margin: 0 16px 16px; padding: 0; list-style: none;
  font: 13px/1.6 ui-monospace, monospace; }}
.functions li {{ display: flex; justify-content: space-between; gap: 8px; break-inside: avoid; padding-right: 16px; }}
.functions .ok {{ color: var(--ok); }}
.functions .draft {{ color: var(--functions); }}
.functions .todo {{ color: var(--muted); }}
</style>
</head>
<body>
<h1>{title}</h1>
<p class="muted">Matching decompilation of <code>{serial}</code> &middot; commit {commit} &middot; {date}</p>
<p>Game and engine code, {unit_count} units. A function counts only when the linked build proves its
retail bytes come from C; assembly fallbacks earn nothing. See {docs}.</p>
<div class="cards">{cards}</div>
<h2>History</h2>
<p class="muted"><span class="key-code">&#9632; code bytes</span> &nbsp; <span class="key-functions">&#9632; functions</span></p>
{chart}
<h2>Categories</h2>
<table>
<thead><tr><th>Category</th><th class="num">Units</th><th>Code bytes</th><th class="num">Matched / total bytes</th>
<th class="num">Functions</th></tr></thead>
<tbody>{categories}</tbody>
</table>
<h2>Units</h2>
<p class="muted"><span class="key-functions">&#9632;</span> A non-matching draft is a close
reconstruction kept in the source behind <code>#ifdef NON_MATCHING</code>. The build takes the
assembly instead, so it counts as unmatched.</p>
<div class="controls">
<input id="filter" type="search" placeholder="Filter units" aria-label="Filter units">
<select id="sort" aria-label="Sort units">
<option value="name">Sort by name</option>
<option value="code">Sort by matched share</option>
<option value="size">Sort by size</option>
</select>
</div>
<div class="units">
<div class="head"><span>Unit</span><span>Code bytes</span><span class="num">Matched / total</span>
<span class="num">Functions</span></div>
<div id="units">{units}</div>
</div>
<script>
const list = document.getElementById("units");
document.getElementById("filter").addEventListener("input", (event) => {{
  const text = event.target.value.toLowerCase();
  for (const unit of list.children) unit.hidden = !unit.dataset.name.includes(text);
}});
document.getElementById("sort").addEventListener("change", (event) => {{
  const key = event.target.value;
  const units = [...list.children];
  units.sort(key === "name"
    ? (a, b) => a.dataset.name.localeCompare(b.dataset.name)
    : (a, b) => Number(b.dataset[key]) - Number(a.dataset[key]));
  list.append(...units);
}});
</script>
</body>
</html>
"""


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("version", nargs="?", default=next(iter(VERSIONS)), choices=VERSIONS)
    parser.add_argument("--out", type=Path, default=ROOT / "build" / "site")
    parser.add_argument("--history", type=Path, help="history.json of the published site")
    parser.add_argument("--repo", default=os.environ.get("GITHUB_REPOSITORY") and
                        f"{os.environ.get('GITHUB_SERVER_URL', 'https://github.com')}/"
                        f"{os.environ['GITHUB_REPOSITORY']}",
                        help="repository URL for source links (default: from GitHub Actions)")
    parser.add_argument("--commit", help="commit the reports were built from (default: HEAD)")
    parser.add_argument("--date", help="commit date, ISO 8601 (default: the commit's date)")
    args = parser.parse_args()

    build = ROOT / "build" / args.version
    report_path = build / "report.json"
    try:
        report = json.loads(report_path.read_text())
        audit = json.loads((build / "report.all.json").read_text())
    except FileNotFoundError as error:
        sys.exit(f"{error.filename} is missing: run `ninja report` first")
    commit = args.commit or git("rev-parse", "HEAD")
    date = args.date or git("show", "-s", "--format=%cI", commit)

    history = []
    if args.history and args.history.exists():
        history = json.loads(args.history.read_text())
    history = merge_history(history, {"commit": commit, "date": date, **totals(report["measures"])})

    out = args.out
    (out / "badges").mkdir(parents=True, exist_ok=True)
    game = totals(report["measures"])
    (out / "badges" / "code.json").write_text(json.dumps(
        badge("code bytes", game["matched_code"], game["total_code"])) + "\n")
    (out / "badges" / "functions.json").write_text(json.dumps(
        badge("functions", game["matched_functions"], game["total_functions"])) + "\n")
    (out / "history.json").write_text(json.dumps(history, indent=1) + "\n")
    shutil.copyfile(report_path, out / "report.json")
    drafts = draft_functions(args.version)
    (out / "index.html").write_text(render(args.version, report, audit, history,
                                           args.repo, commit, date, drafts))
    print(f"wrote {out.relative_to(ROOT) if out.is_relative_to(ROOT) else out}: "
          f"{percent(game['matched_code'], game['total_code']):.3f}% code, "
          f"{percent(game['matched_functions'], game['total_functions']):.3f}% functions, "
          f"{count_drafts(report, drafts)} non-matching drafts, "
          f"{len(history)} history entries")


if __name__ == "__main__":
    main()
