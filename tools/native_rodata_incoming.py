"""Read-only whole-link census for explicit SMT3 readonly reference-fill gaps.

Call once with all opted native VA intervals and the receipt already validated
by production_report.read_receipt. The scope is the complete canonical GNU
link input list, not merely objects containing the candidate .rodata.
"""
from collections import Counter, defaultdict
import hashlib
from io import BytesIO
from pathlib import Path
import re
import struct

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

if __package__:
    from .production_report import live_input_sections, load_segments, mapped_bytes
else:
    from production_report import live_input_sections, load_segments, mapped_bytes


WORD = struct.Struct('<I')
MASK32 = (1 << 32) - 1
SUPPORTED = (2, 4, 5, 6, 7, 8)  # 32, 26, HI16, LO16, GPREL16, LITERAL.


def must(condition, reason):
    if not condition:
        raise ValueError(reason)


def signed_low(word):
    return (word & 0x7fff) - (word & 0x8000)


def parse_elf(data, kind):
    elf = ELFFile(BytesIO(data))
    must(elf.elfclass == 32 and elf.little_endian
         and elf['e_machine'] == 'EM_MIPS' and elf['e_type'] == kind,
         f'expected MIPS little-endian ELF32 {kind}')
    return elf


def verify_no_linked_literal_pointers(elf, linked, spans):
    # Raw pointers in retained data need not have relocation records. For this
    # narrow scope, conservatively inspect every native aligned word in each
    # file-backed load rather than guessing which sections contain pointers.
    checked = 0
    for seg in elf.iter_segments():
        if seg['p_type'] != 'PT_LOAD' or not seg['p_filesz']:
            continue
        addr, offset, size = seg['p_vaddr'], seg['p_offset'], seg['p_filesz']
        must(0 <= offset < offset + size <= len(linked)
             and 0 <= addr < addr + size <= 1 << 32, 'invalid literal-pointer load extent')
        start = (-addr) & 3
        if size - start < 4:
            continue
        length = (size - start) & ~3
        chunk = memoryview(linked)[offset+start:offset+start+length]
        for i, (value,) in enumerate(WORD.iter_unpack(chunk)):
            must(not any(lo <= value < hi for lo, hi in spans),
                 f'aligned literal pointer into linker fill: 0x{addr+start+4*i:08x} -> 0x{value:08x}')
        checked += length // 4
    must(checked > 0, 'no mapped words for literal pointer census')
    return checked

def verify_live_no_incoming(root: Path, receipt: dict, spans):
    """Reject every known live relocated reference into either verified fill.

    Spans are half-open native byte ranges, not object-relative offsets.
    Unknown relocation types, ambiguous input placement, unresolved standalone
    LO16, or any pending unmatched HI16 are blockers even outside the gaps.
    No generated files are read outside the exact production link ledger.
    """
    spans = sorted(spans)
    must(bool(spans) and all(type(lo) is int and type(hi) is int
         and 0x10000 <= lo < hi <= 1 << 32 for lo, hi in spans)
         and all(a[1] <= b[0] for a, b in zip(spans, spans[1:])),
         'missing, overlapping or invalid native fill intervals')
    mate = 'build/smt3/SLUS_209.11.'
    must(receipt.get('schema') == 1 and receipt.get('elf') == mate + 'elf'
         and receipt.get('map') == mate + 'map', 'wrong SMT3 link receipt identity')
    objects = receipt['objects']
    must(bool(objects) and len(objects) == len(set(objects)),
         'empty or duplicate linked input inventory')
    must(all(re.fullmatch(r'build/smt3/[A-Za-z0-9_./-]+[.]o', name)
             and not {'..', '.'}.intersection(name.split('/')) for name in objects),
         'unexpected linked input path')
    entries = receipt['files']

    def pinned(name):
        data = (root / name).read_bytes()
        must(hashlib.sha256(data).hexdigest() == entries.get(name),
             f'unbound or changed live input: {name}')
        return data

    linked = pinned(receipt['elf'])
    map_text = pinned(receipt['map']).decode()
    _, marker, body = map_text.partition('Linker script and memory map')
    must(bool(marker), 'missing actual link map')
    loads = []
    for row in body.splitlines():
        if not re.match(r'\s*LOAD(?:\s|$)', row):
            continue
        m = re.fullmatch(r'LOAD[ \t]+(\S+)[ \t]*', row)
        must(m is not None, 'unsupported map LOAD record')
        loads.append(m[1])
    must(loads == objects, 'map LOAD and receipt inventories differ')
    live = live_input_sections(map_text)
    load_paths = set(loads)
    must(all(obj in entries and obj in load_paths for obj, _ in live),
         'map has an unbound live input')
    for (name, sec), places in live.items():
        for base, length in places:
            must(0 <= base < base + length <= 1 << 32, 'invalid live input extent')
            must(not any(base < hi and lo < base + length for lo, hi in spans),
                 f'live input section intersects fill: {name} {sec}')

    full = parse_elf(linked, 'ET_EXEC')
    segments = load_segments(linked)
    symtabs = [s for s in full.iter_sections() if s.name == '.symtab']
    must(len(symtabs) == 1, 'missing unique linked symbol table')
    symbols = defaultdict(set)
    for sym in symtabs[0].iter_symbols():
        if sym['st_shndx'] == 'SHN_UNDEF' or sym['st_info']['type'] in ('STT_SECTION', 'STT_FILE'):
            continue
        value, size = sym['st_value'], sym['st_size']
        must(not any(lo <= value < hi or value < lo < value + size for lo, hi in spans),
             f'linked symbol intersects native fill: {sym.name}')
        if sym.name:
            symbols[sym.name].add(value)
    must(len(symbols['_gp']) == 1, 'no unique real linked GP')
    gp = next(iter(symbols['_gp']))

    counts = Counter()
    pairs = solo = unsigned_pairs = 0
    for name in objects:
        obj = parse_elf(pinned(name), 'ET_REL')
        origins = set()
        for relsec in obj.iter_sections():
            if not isinstance(relsec, RelocationSection) or not relsec.num_relocations():
                continue
            source = obj.get_section(relsec['sh_info'])
            if not source['sh_flags'] & 2:
                continue
            tag = f'{name} {source.name}'
            placements = live.get((name, source.name), [])
            must(len(placements) == 1 and placements[0][1] == source['sh_size']
                 and source.name not in origins and source['sh_type'] == 'SHT_PROGBITS'
                 and relsec['sh_type'] == 'SHT_REL',
                 'unsupported or ambiguous live relocation origin: ' + tag)
            origins.add(source.name)
            va, length = placements[0]
            live_words = mapped_bytes(linked, segments, va, length)
            original = source.data()
            must(len(live_words) == len(original) == length, 'live relocation extent: ' + tag)
            symtab = obj.get_section(relsec['sh_link'])
            must(symtab['sh_type'] == 'SHT_SYMTAB', 'invalid relocation symbol table: ' + tag)
            pending = defaultdict(list)

            def forbid(address, site, kind, symbol, form=''):
                address &= MASK32
                must(not any(lo <= address < hi for lo, hi in spans),
                     f'incoming relocation into linker fill: {tag}+0x{site:x} '
                     f'type {kind}{form} {symbol or "<section>"} -> 0x{address:08x}')

            for rel in relsec.iter_relocations():
                offset, kind, sym_index = rel['r_offset'], rel['r_info_type'], rel['r_info_sym']
                must(0 <= offset <= length - 4 and kind in SUPPORTED,
                     f'unsupported live relocation type/extent: {tag}+0x{offset:x} type {kind}')
                counts[kind] += 1
                site = va + offset
                word = WORD.unpack_from(live_words, offset)[0]
                sym = symtab.get_symbol(sym_index)
                if kind == 2:
                    forbid(word, offset, kind, sym.name)
                elif kind == 4:
                    forbid(((site + 4) & 0xf0000000) | ((word & 0x03ffffff) << 2),
                           offset, kind, sym.name)
                elif kind == 5:
                    pending[sym_index].append(word)
                elif kind == 6:
                    his = pending.pop(sym_index, ())
                    zero_extended = bool(source['sh_flags'] & 4) and (word >> 26) in (13, 14)
                    for hi_word in his:
                        high = (hi_word & 0xffff) << 16
                        forbid(high + signed_low(word), offset, kind, sym.name)
                        pairs += 1
                        if zero_extended:
                            # ELF's signed displacement and an executing ORI/XORI
                            # with a materialized high half are not equivalent.
                            # Check both; making an unproved controller safe by
                            # picking only one address would give false assurance.
                            forbid(high | (word & 0xffff), offset, kind, sym.name, ' unsigned LO')
                            unsigned_pairs += 1
                    if not his:
                        if zero_extended:
                            must((word >> 21) & 31 == 0,
                                 'unproved unsigned standalone LO16 base: ' + tag)
                            forbid(word & 0xffff, offset, kind, sym.name, ' unsigned LO')
                        # A standalone LO can also occur when code shares an
                        # existing high load. Validate S+A against the actual
                        # low bits, and reject absent/unplaced/conflicting S.
                        n = sym['st_shndx']
                        if isinstance(n, int):
                            targets = live.get((name, obj.get_section(n).name), [])
                            must(len(targets) == 1, 'ambiguous standalone LO16 local definition: ' + tag)
                            base = {targets[0][0] + sym['st_value']}
                        elif n == 'SHN_ABS':
                            base = {sym['st_value']}
                        else:
                            base = symbols.get(sym.name, set())
                        must(len(base) == 1, 'ambiguous standalone LO16: ' + tag)
                        addend = signed_low(WORD.unpack_from(original, offset)[0])
                        resolved = (next(iter(base)) + addend) & MASK32
                        must(resolved & 0xffff == word & 0xffff,
                             f'standalone LO16 addend does not bind linked bytes: {tag}+0x{offset:x}')
                        forbid(resolved, offset, kind, sym.name)
                        solo += 1
                else:
                    forbid(gp + signed_low(word), offset, kind, sym.name)
            must(not pending, 'unpaired live HI16: ' + tag)
    literal_words = verify_no_linked_literal_pointers(full, linked, spans)
    return {'objects': len(objects),
            'map_contributions': sum(len(v) for v in live.values()),
            'relocation_types': dict(sorted(counts.items())),
            'relocations': sum(counts.values()),
            'paired_hi16': pairs, 'unsigned_lo16_checks': unsigned_pairs,
            'standalone_lo16': solo, 'linked_aligned_words': literal_words}
