#!/usr/bin/env python3
"""Collect the per-agent results of a workflow journal into one reports file."""

import json
import sys


def main():
    journal, out = sys.argv[1], sys.argv[2]
    batches, labels = [], {}
    for line in open(journal):
        row = json.loads(line)
        if row.get("type") == "started":
            labels[row["agentId"]] = row.get("label", "")
        if row.get("type") != "result":
            continue
        value = row.get("value") or row.get("result")
        if isinstance(value, str):
            try:
                value = json.loads(value)
            except ValueError:
                continue
        if isinstance(value, dict) and "functions" in value:
            batches.append({"batch": labels.get(row.get("agentId"), ""), "result": value})
    json.dump({"batches": batches}, open(out, "w"), indent=1)
    print(f"{len(batches)} batch results -> {out}")


if __name__ == "__main__":
    main()
