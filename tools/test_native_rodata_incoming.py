"""Tiny real ELF32 relocation and linked-map fixtures; no compiler or linker."""
import hashlib
from pathlib import Path
import struct
import tempfile
import unittest

if __package__:
    from .native_rodata_incoming import verify_live_no_incoming
else:
    from native_rodata_incoming import verify_live_no_incoming


GAPS = [(0x53A641, 0x53A648), (0x53B057, 0x53B060)]
MATE = 'build/smt3/SLUS_209.11.'
# Neither candidate owner: the local-only classifier cannot see this object.
EXTERNAL = 'build/smt3/src/smt3/model/externalConsumer.o'
FROM = 0x00400000
PROVIDER = 'terminal_literal'
SYM_VA = 0x0053a630


def word(values):
    return struct.pack('<' + 'I' * len(values), *values)


def fixture_elf(*, linked, source, values, relocs=(), extra=()):
    """Construct minimal valid MIPS ELF with a real SHT_REL and/or PT_LOAD."""
    sec = [source, '.symtab', '.strtab', '.shstrtab']
    if relocs:
        sec.append('.rel' + source)

    def strings(items):
        pool = bytearray(1)
        offsets = {}
        for item in items:
            if item not in offsets:
                offsets[item] = len(pool)
                pool.extend(item.encode('ascii') + bytes(1))
        return bytes(pool), offsets

    names, names_ix = strings(sec)
    syms = [(PROVIDER, SYM_VA if linked else 0, 0, 0xfff1 if linked else 0)]
    if linked:
        syms.append(('_gp', 0x554170, 0, 0xfff1))
        syms.extend((n, a, s, 0xfff1) for n, a, s in extra)
    st, st_ix = strings(n for n, _, _, _ in syms)
    sdata = bytes(16) + b''.join(struct.pack('<IIIBBH', st_ix[n], a, s, 0x10, 0, sh)
                                  for n, a, s, sh in syms)
    dat = {
        source: word(values),
        '.symtab': sdata,
        '.strtab': st,
        '.shstrtab': names,
    }
    shdr = {
        source: (1, 3 if source != '.text' else 6, FROM if linked else 0, 0, 0, 4, 0),
        '.symtab': (2, 0, 0, 3, 1, 4, 16),
        '.strtab': (3, 0, 0, 0, 0, 1, 0),
        '.shstrtab': (3, 0, 0, 0, 0, 1, 0),
    }
    if relocs:
        dat[sec[-1]] = b''.join(word([offset, (1 << 8) | kind]) for offset, kind in relocs)
        shdr[sec[-1]] = (9, 0x40, 0, 2, 1, 4, 8)
    res = bytearray(0x100)
    rows = [bytes(40)]
    for name in sec:
        kind, flags, addr, link, info, align, ent = shdr[name]
        res.extend(bytes((-len(res)) % align))
        rows.append(word([names_ix[name], kind, flags, addr, len(res), len(dat[name]),
                          link, info, align, ent]))
        res.extend(dat[name])
    res.extend(bytes((-len(res)) % 4))
    shoffs = len(res)
    res.extend(b''.join(rows))
    ident = bytes([127, 69, 76, 70, 1, 1, 1, 0]) + bytes(8)
    res[:52] = struct.pack('<16sHHIIIIIHHHHHH', ident, 2 if linked else 1, 8, 1, 0,
                           52 if linked else 0, shoffs, 0, 52, 32 if linked else 0,
                           int(linked), 40, len(rows), sec.index('.shstrtab')+1)
    if linked:
        size = len(dat[source])
        res[52:84] = word([1, 0x100, FROM, FROM, size, size, 4, 4])
    return bytes(res)


class NativeReadonlyIncoming(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)

    def setup_case(self, linked_words, *, rels=((0, 2),), original_words=None,
                   extra_map='', skip_origin=False, extra_symbols=(), src='.data'):
        original_words = original_words or [0] * len(linked_words)
        data = fixture_elf(linked=False, source=src, values=original_words, relocs=rels)
        exe = fixture_elf(linked=True, source='.main', values=linked_words, extra=extra_symbols)
        txt = f'Linker script and memory map\nLOAD {EXTERNAL}\n'
        if not skip_origin:
            txt += f' {src}  0x{FROM:08x}  0x{len(linked_words)*4:x}  {EXTERNAL}\n'
        txt += extra_map
        files = {MATE+'elf': exe, MATE+'map': txt.encode(), EXTERNAL: data}
        for key, content in files.items():
            path = self.root/key
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(content)
        return {'schema': 1, 'objects':[EXTERNAL], 'elf': MATE+'elf', 'map': MATE+'map',
                'files': {name: hashlib.sha256(content).hexdigest()
                          for name, content in files.items()}}

    def census(self, rec):
        return verify_live_no_incoming(self.root, rec, GAPS)

    def test_actual_external_r32_addend_into_second_gap_rejected(self):
        valid = self.setup_case([SYM_VA])
        self.assertEqual(self.census(valid)['relocation_types'], {2: 1})
        # Later link has valid matching object/map/ELF digests but adds an offset
        # into the second gap. The candidate owners and literal are untouched.
        st = self.setup_case([GAPS[1][0] + 2], original_words=[GAPS[1][0] + 2 - SYM_VA])
        with self.assertRaisesRegex(ValueError, 'incoming relocation.*externalConsumer.*0x0053b059'):
            self.census(st)

    def test_paired_hi_lo_with_signed_carry(self):
        dst = GAPS[0][0]
        hi = (dst + 0x8000) >> 16
        r = self.setup_case([0x3c020000| hi, 0x24420000| (dst&0xffff)],
                            rels=((0, 5), (4, 6)), src='.text')
        with self.assertRaisesRegex(ValueError, 'incoming relocation.*type 6'):
            self.census(r)

    def test_executing_ori_lo_can_target_gap_with_non_target_signed_sum(self):
        # Physical LUI 0x53; ORI 0xa641. Signed disassembly yields 0x52a641,
        # but the actual zero-ext instruction materializes 0x53a641.
        rec = self.setup_case([0x3c020053, 0x3442a641],
                              rels=((0, 5), (4, 6)), src='.text')
        with self.assertRaisesRegex(ValueError, 'incoming relocation.*unsigned LO.*0x0053a641'):
            self.census(rec)
        rec = self.setup_case([0x3c020052, 0x3442a641],
                              rels=((0, 5), (4, 6)), src='.text')
        self.assertEqual(self.census(rec)['unsigned_lo16_checks'], 1)
        rec = self.setup_case([0x3442a630], rels=((0, 6),), src='.text')
        with self.assertRaisesRegex(ValueError, 'unproved unsigned standalone LO16 base'):
            self.census(rec)

    def test_aligned_literal_pointer_without_relocation_rejected(self):
        rec = self.setup_case([GAPS[1][0] + 2], rels=())
        with self.assertRaisesRegex(ValueError, 'aligned literal pointer.*0x00400000 -> 0x0053b059'):
            self.census(rec)
        rec = self.setup_case([GAPS[1][1]], rels=())
        self.assertEqual(self.census(rec)['linked_aligned_words'], 1)

    def test_standalone_lo_binds_complete_address(self):
        r = self.setup_case([0x2442a630], rels=((0,6),), original_words=[0x24420000])
        self.assertEqual(self.census(r)['standalone_lo16'], 1)
        r = self.setup_case([0x2442a641], rels=((0,6),), original_words=[0x24420011])
        with self.assertRaisesRegex(ValueError, 'incoming relocation.*type 6'):
            self.census(r)
        r = self.setup_case([0x2442a630], rels=((0,6),), original_words=[0x24420000],
                            extra_symbols=[(PROVIDER, 0x61aa22, 0)])
        with self.assertRaisesRegex(ValueError,'ambiguous standalone LO16'):
            self.census(r)

    def test_unsupported_kind_missing_origin_and_unpaired_hi(self):
        for rels, skip, error in [(((0, 9),),False,'unsupported live relocation type'),
                                  (((0, 2),),True,'ambiguous live relocation origin'),
                                  (((0, 5),),False,'unpaired live HI16')]:
            r = self.setup_case([0x3c020054], rels=rels, skip_origin=skip)
            with self.subTest(error=error), self.assertRaisesRegex(ValueError,error):
                self.census(r)

    def test_changed_receipt_unknown_map_load_or_overlapping_owner(self):
        r = self.setup_case([SYM_VA])
        r['files'][EXTERNAL] = '0' * 64
        with self.assertRaisesRegex(ValueError, 'changed live input'):
            self.census(r)
        r = self.setup_case([SYM_VA], extra_map='LOAD unsupported extra record\n')
        with self.assertRaisesRegex(ValueError,'unsupported map LOAD'):
            self.census(r)
        r = self.setup_case([SYM_VA], extra_map=f' .data  0x{FROM+8:08x}  0x4  {EXTERNAL}\n')
        with self.assertRaisesRegex(ValueError, 'ambiguous live relocation origin'):
            self.census(r)
        r = self.setup_case([SYM_VA], extra_map=f' .odd  0x53a642  0x1  {EXTERNAL}\n')
        with self.assertRaisesRegex(ValueError,'live input section intersects fill'):
            self.census(r)

    def test_target_defined_absolute_in_gap_fails_even_before_iteration(self):
        r = self.setup_case([SYM_VA], extra_symbols=[('live_assembly_alias', GAPS[0][0], 0)])
        with self.assertRaisesRegex(ValueError,'linked symbol intersects native fill'):
            self.census(r)


if __name__ == '__main__':
    unittest.main()
