"""Conservative input-section inventory for the generated linker script."""

from collections import Counter
from pathlib import Path
import re


# Preserve the existing linker object order, but only treat a complete
# one-input line as evidence that an object has exactly one selector.
LD_OBJECT_REFERENCE = re.compile(r"(build/[^\s()]+\.o)\(")
LD_OBJECT_LINE = re.compile(r"\s*(build/[^\s()]+\.o)\(([^()]+)\)\s*;?\s*")


def linker_object_sections(ld_script: Path) -> dict[Path, list[str] | None]:
    """Read generated input selectors in link order; None means ambiguous."""
    return parse_object_sections(ld_script.read_text())


def parse_object_sections(text: str) -> dict[Path, list[str] | None]:
    # Count exact paths conservatively even on unrecognized lines. A
    # spaced, multiline, quoted or commented repeat cannot establish
    # that the object has just one .data selector.
    canonical: Counter[Path] = Counter()
    objects: dict[Path, list[str]] = {}
    uncommented = re.sub(r"/\*.*?\*/", lambda m: " " + "\n" * m[0].count("\n"), text, flags=re.S)
    for line in uncommented.splitlines():
        match = LD_OBJECT_LINE.fullmatch(line)
        if match:
            obj = Path(match.group(1))
            objects.setdefault(obj, []).extend(match.group(2).split())
            canonical[obj] += 1
        else:
            # Preserve the historical object discovery on other lines.
            first = LD_OBJECT_REFERENCE.match(line.lstrip())
            if first:
                objects.setdefault(Path(first.group(1)), [])
    return {obj: sections if canonical[obj] == text.count(obj.as_posix()) else None
            for obj, sections in objects.items()}
