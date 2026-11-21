"""Validated non-owning aliases into reconstructed storage objects."""

from dataclasses import dataclass
import json
import re


@dataclass(frozen=True)
class SymbolAlias:
    name: str
    owner: str
    unit: str
    section: str
    offset: int
    address: int
    owner_size: int | None = None
    alias_size: int | None = None
    native_type: str | None = None
    record_bytes: int | None = None
    prefix_bytes: int = 0


NATIVE_SYMBOL = re.compile(r"^\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)\s*;([^\n]*)", re.M)
NATIVE_DATA_WIDTH = {"u8": 1, "s8": 1, "u16": 2, "s16": 2, "u32": 4, "s32": 4,
                     "f32": 4, "u64": 8, "s64": 8, "f64": 8}


def native_data_symbol(rows, name, path, *, owner_size=None):
    """Read explicit native storage metadata; first declaration has precedence."""
    values = []
    for address, attrs in rows.get(name, ()):
        sizes = re.findall(r"\bsize:([^\s/]+)", attrs)
        types = re.findall(r"\btype:([^\s/]+)", attrs)
        if len(sizes) > 1 or len(types) > 1:
            raise ValueError(f"{path}: ambiguous native metadata for {name}")
        try:
            size = int(sizes[0], 0) if sizes else None
        except ValueError as error:
            raise ValueError(f"{path}: invalid native size for {name}") from error
        if size is not None and size <= 0:
            raise ValueError(f"{path}: invalid native size for {name}")
        typ = types[0] if types else None
        if typ is not None and typ not in NATIVE_DATA_WIDTH:
            raise ValueError(f"{path}: {name} is not an accepted native data type")
        values.append((address, size, typ))
    if not values:
        raise ValueError(f"{path}: missing canonical native data symbol {name}")
    if owner_size is None and (values[0][1] is None or values[0][2] is None):
        raise ValueError(f"{path}: {name} needs an explicit native data size and alias type")
    address, size, typ = values[0]
    if owner_size is not None:
        if size is not None and size != owner_size:
            raise ValueError(f"{path}: {name} size disagrees with explicit .data owner extent")
        size = owner_size
    if any(a != address or (s is not None and s != size) or (t is not None and t != typ)
           for a, s, t in values[1:]):
        raise ValueError(f"{path}: conflicting native definitions for {name}")
    width = NATIVE_DATA_WIDTH.get(typ, 1)
    if address + size > (1 << 32) or address % width or size % width:
        raise ValueError(f"{path}: unaligned native data definition for {name}")
    return address, size, typ


def data_alias_envelope(alias):
    """The linked anchor can be inside the verified native view."""
    size, off, addr = alias.owner_size, alias.offset, alias.address
    forward, before = alias.alias_size, alias.prefix_bytes
    if (any(type(n) is not int for n in (size, off, addr, forward, before))
            or not 0 <= before <= off or not 0 < off <= size - forward
            or forward <= 0 or not 0 <= addr - off < addr
            or addr - off + size > 1 << 32):
        raise ValueError(f"{alias.name}: out-of-bounds native data view")
    record = alias.record_bytes
    if record is None:
        if before:
            raise ValueError(f"{alias.name}: prefix requires an explicit record view")
    else:
        width = NATIVE_DATA_WIDTH.get(alias.native_type) if isinstance(alias.native_type, str) else None
        if (type(record) is not int or not width or record <= 0 or record % width
                or any(n % record for n in (addr - off, size, off, before, forward))):
            raise ValueError(f"{alias.name}: native view must cover whole aligned records")
    return addr - before, addr + forward


def validate_data_alias_views(aliases):
    """Only explicitly typed same-primary record views may share storage."""
    spans, primaries, names = [], {}, set()
    for alias in aliases:
        if alias.section != ".data":
            if alias.record_bytes is not None or alias.prefix_bytes != 0:
                raise ValueError(f"{alias.name}: record view requires .data")
            continue
        start, end = data_alias_envelope(alias)
        if alias.name in names or alias.name == alias.owner:
            raise ValueError(f"{alias.name}: duplicate or self alias")
        names.add(alias.name)
        base = alias.address - alias.offset
        owner = (alias.unit, base, alias.owner_size)
        if primaries.setdefault(alias.owner, owner) != owner:
            raise ValueError(f"{alias.name}: conflicting data owner units or extents")
        for other, lo, hi in spans:
            other_base = other.address - other.offset
            if (alias.owner != other.owner
                    and base < other_base + other.owner_size
                    and other_base < base + alias.owner_size
                    and (alias.record_bytes is not None or other.record_bytes is not None)):
                raise ValueError(f"{alias.name}: overlapping distinct native data owners")
            if not (lo < end and start < hi):
                continue
            if (alias.record_bytes is None or alias.record_bytes != other.record_bytes
                    or alias.owner != other.owner or alias.address == other.address
                    or NATIVE_DATA_WIDTH[alias.native_type] != NATIVE_DATA_WIDTH[other.native_type]):
                raise ValueError(f"{alias.name}: overlapping native data aliases lack a shared record view")
        spans.append((alias, start, end))
    if names.intersection(primaries):
        raise ValueError("data aliases must refer directly to primary objects")


def load_symbol_aliases(root, version):
    """Resolve explicit owner-plus-offset aliases against native symbol facts."""
    path = root / "config" / version / "symbol_aliases.json"
    if not path.exists():
        return ()
    rows = json.loads(path.read_text())
    if not isinstance(rows, list):
        raise ValueError(f"{path}: expected a list of symbol aliases")
    definitions = NATIVE_SYMBOL.findall((path.parent / "symbol_addrs.txt").read_text())
    # Preserve the historical NOBITS lookup, including autogenerated addresses.
    symbols = {name: int(value, 16) for name, value, _ in definitions}
    native_rows = {}
    for name, address, attrs in definitions:
        native_rows.setdefault(name, []).append((int(address, 16), attrs))
    aliases = []
    names = set()
    fields = {"name", "owner", "unit", "section", "offset"}
    for row in rows:
        expected = (fields | {"owner_size"} if isinstance(row, dict) and row.get("section") == ".data"
                    else fields)
        if isinstance(row, dict) and row.get("section") == ".data" and "view" in row:
            expected |= {"view"}
        if not isinstance(row, dict) or set(row) != expected:
            raise ValueError(f"{path}: alias fields must be {sorted(expected)}")
        name, owner = row["name"], row["owner"]
        if any(not isinstance(value, str) or not re.fullmatch(r"[A-Za-z_]\w*", value)
               for value in (name, owner)):
            raise ValueError(f"{path}: invalid alias or owner name")
        if name in names or name == owner:
            raise ValueError(f"{path}: duplicate or self alias {name}")
        names.add(name)
        unit, section, offset = row["unit"], row["section"], row["offset"]
        if not isinstance(unit, str) or not re.fullmatch(r"[A-Za-z0-9_/]+", unit) \
                or section not in (".bss", ".sbss", ".data") \
                or type(offset) is not int or offset < 0:
            raise ValueError(f"{path}: invalid storage alias contract for {name}")
        if section == ".data":
            owner_size = row["owner_size"]
            if not re.fullmatch(r"(?:[A-Za-z0-9_]+/)*[A-Za-z0-9_]+", unit) \
                    or type(owner_size) is not int or owner_size <= 0:
                raise ValueError(f"{path}: invalid data owner unit for {name}")
            owner_address, owner_size, _ = native_data_symbol(native_rows, owner, path,
                                                             owner_size=owner_size)
            native_address, alias_size, typ = native_data_symbol(native_rows, name, path)
            if offset <= 0 or owner_address + offset != native_address or offset + alias_size > owner_size:
                raise ValueError(f"{path}: {name} does not fit inside native {owner} at +0x{offset:X}")
            record, prefix = None, 0
            if "view" in row:
                view = row["view"]
                if (not isinstance(view, dict) or set(view) != {"record_bytes", "prefix_bytes"}
                        or type(view["record_bytes"]) is not int
                        or type(view["prefix_bytes"]) is not int):
                    raise ValueError(f"{path}: invalid record view for {name}")
                record, prefix = view["record_bytes"], view["prefix_bytes"]
            aliases.append(SymbolAlias(name, owner, unit, section, offset, native_address,
                                       owner_size, alias_size, typ, record, prefix))
            continue
        owner_address = symbols.get(owner)
        native_address = symbols.get(name)
        if native_address is None and (match := re.fullmatch(r"D_([0-9A-F]{8})", name)):
            native_address = int(match[1], 16)
        if owner_address is None or native_address is None \
                or owner_address + offset != native_address:
            raise ValueError(f"{path}: {name} does not equal native {owner}+0x{offset:X}")
        aliases.append(SymbolAlias(name, owner, unit, section, offset, native_address))
    if any(alias.owner in names for alias in aliases):
        raise ValueError(f"{path}: aliases must refer directly to primary objects")
    validate_data_alias_views(aliases)
    return tuple(aliases)


def linker_aliases(text, aliases):
    """Replace absolute fallbacks with non-allocating owner-relative aliases."""
    begin = "/* source-owned symbol aliases */"
    end = "/* end source-owned symbol aliases */"
    text = re.sub(re.escape(begin) + r".*?" + re.escape(end) + r"\n?",
                  "", text, flags=re.S)
    for alias in aliases:
        if alias.section == ".data":
            if re.search(rf"(?<![\w.]){re.escape(alias.name)}\s*=", text):
                raise ValueError(f"competing linker definition for .data alias {alias.name}")
            continue
        text = re.sub(rf"^\s*PROVIDE\(\s*{re.escape(alias.name)}\s*=[^;]*;\s*\n?",
                      "", text, flags=re.M)
    if not aliases:
        return text
    rows = [begin, *(f"PROVIDE({alias.name} = {alias.owner} + 0x{alias.offset:X});"
                     for alias in aliases), end, ""]
    return "\n".join(rows) + text
