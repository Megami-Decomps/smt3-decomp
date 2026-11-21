"""Synthetic fail-closed graph tests, no proprietary input or stock compiler."""
import io
import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

import ninja_syntax

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from tools import native_rodata_fill as fill
from tools.linker_inputs import linker_object_sections

VERSION = 'smt3'
SERIAL = 'SLUS_209.11'
PB = 'pb/pbDebugMode'
NEXT = 'model/mdlManager'


def cobj(unit):
    return Path(f'build/smt3/src/smt3/{unit}.o')


def write(root, rel, payload):
    out = root / rel
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(payload)
    return out


def elf_part(fields, raw, name):
    class Part(dict):
        def data(self):
            return raw
    obj = Part(fields)
    obj.name = name
    return obj


class NativeReadonlyFill(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.pri = cobj(PB)
        self.nxt = cobj(NEXT)
        self.selectors = {
            self.pri: ['.text', '.rodata', '.lit4'],
            self.nxt: ['.text', '.rodata', '.sdata'],
        }
        self.ld_source = ''.join(f'{p}({sec});\n'
                                 for p, secs in self.selectors.items() for sec in secs)

    def test_duplicate_or_ambiguous_selectors_fail(self):
        for extra in ('', f'{self.pri}(.rodata);\n', f'{self.pri}(\n.rodata\n);',
                      f'/* {self.nxt}(.rodata); */', f'"{self.nxt}" (.rodata);'):
            script = write(self.root, 'build/smt3/SLUS_209.11.ld', self.ld_source + extra)
            actual = linker_object_sections(script)
            if extra:
                with self.assertRaisesRegex(ValueError, 'one unambiguous'):
                    fill.strict_selectors(actual, self.pri, self.nxt)
            else:
                fill.strict_selectors(actual, self.pri, self.nxt)

    def test_explicit_opts_only(self):
        cfg = fill.opt_path(VERSION)
        self.assertEqual(fill.options(self.root, VERSION), ())
        write(self.root, cfg, json.dumps([PB]))
        self.assertEqual(fill.options(self.root, VERSION), (PB,))
        for bad in ([PB, PB], [NEXT], [], {PB: True}, [1]):
            with self.subTest(opts=bad):
                write(self.root, cfg, json.dumps(bad))
                with self.assertRaisesRegex(ValueError, 'invalid native'):
                    fill.options(self.root, VERSION)

    def test_terminal_eof_is_mode_select_only(self):
        for unit in (PB, 'event/evtCommand', fill.MODE_SELECT):
            case=fill.CASES[unit]
            have=case.final+len(case.value)+1
            # Use actual native terminal widths, with a fake peer text line.
            if unit==PB:
                tail='.align 3'
            elif unit=='event/evtCommand':
                tail=('.align 2\n'+
                      f'    /* {case.end-8:06X} {case.end+fill.BASE-8:08X} */ .asciz ""\n.align 2\n'+
                      f'    /* {case.end-4:06X} {case.end+fill.BASE-4:08X} */ .asciz ""\n.align 2')
            else:
                tail='.align 2'
            end4=(have+3)&~3
            payload=case.value.encode()+bytes(end4-have+1)
            ret=bytes(case.final)+payload+bytes(case.end-end4)
            native=(f'.section .text, "ax"\n\n.section .rodata, "a"\n'
                    f'dlabel {case.label}\n'
                    f'    /* {case.final:06X} {case.final+fill.BASE:08X} */ .asciz "{case.value}"\n'
                    f'    /* {payload.hex().upper()} */\n'
                    f'{tail}\nenddlabel {case.label}\n')
            follower='.section .lit4, "wa"\n'
            if unit==fill.MODE_SELECT:
                fill.render(native,case,ret,have)
                with self.assertRaisesRegex(ValueError, 'not terminal'):
                    fill.render(native+follower,case,ret,have)
            else:
                fill.render(native+follower,case,ret,have)
                with self.assertRaisesRegex(ValueError, 'not terminal'):
                    fill.render(native,case,ret,have)

    def test_additional_cases_only_accept_their_pinned_terminal_seams(self):
        lit4 = '.section .lit4, "wa"\n'
        sdata = '.section .sdata, "wa"\n'
        cases = {
            'pb/pbDraw': lit4,
            'newbattle/nbCameraProcess': lit4,
            'pb/pbGameDraw': '',
            'sdf/sdfHdd': sdata,
            'kernel/dds3KernelCore': sdata,
            'kernel/dds3CreateData': sdata,
            'facility/fclRagInit': sdata,
            'basic/dds3SceneBasic': '',
            'pipe/pipeEffect': sdata,
            'interface/frTestFont': '',
            'field/fldTitle': '',
            'script/scrCommonCommand': lit4,
            'code_001C55F8': lit4,
        }
        for unit, suffix in cases.items():
            with self.subTest(unit=unit):
                case = fill.CASES[unit]
                have = case.final + len(case.value) + 1
                end4 = (have + 3) & ~3
                payload = case.value.encode() + bytes(end4 - have + 1)
                tail = '.align 2\n'
                for at in range(end4, case.end, 4):
                    tail += (f'    /* {at:06X} {at+fill.BASE:08X} */ .asciz ""\n'
                             '    /* 00000000 */\n.align 2\n')
                ret = bytes(case.final) + payload + bytes(case.end - end4)
                native = (f'.section .text, "ax"\n\n.section .rodata, "a"\n'
                          f'dlabel {case.label}\n'
                          f'    /* {case.final:06X} {case.final+fill.BASE:08X} */ .asciz "{case.asm_literal or case.value}"\n'
                          f'    /* {payload.hex().upper()} */\n'
                          f'{tail}enddlabel {case.label}\n')
                rendered = fill.render(native + suffix, case, ret, have)
                self.assertIn(f'.section {fill.FILL}, "a"\n    .zero {case.end-have}\n', rendered)
                for bad in ('', lit4, sdata):
                    if bad != suffix:
                        with self.assertRaisesRegex(ValueError, 'not terminal'):
                            fill.render(native + bad, case, ret, have)
                first = f'dlabel {case.label}\n'
                for early in (lit4, sdata):
                    with self.assertRaisesRegex(ValueError, 'not terminal'):
                        fill.render(native.replace(first, early + first, 1) + suffix,
                                    case, ret, have)
                wrong_order = next(bad for bad in (lit4, sdata) if bad != suffix)
                with self.assertRaisesRegex(ValueError, 'not terminal'):
                    fill.render(native + wrong_order + suffix, case, ret, have)
                write(self.root, fill.opt_path(VERSION), json.dumps([unit + ".invalid"]))
                with self.assertRaisesRegex(ValueError, 'invalid native'):
                    fill.options(self.root, VERSION)

    def test_escaped_terminal_literals_are_exact_and_case_specific(self):
        for unit in ('pb/pbRecord', 'effect/billManager', 'effect/effPCPNeedle', 'script/scrCommonCommand'):
            with self.subTest(unit=unit):
                case = fill.CASES[unit]
                have = case.final + len(case.value) + 1
                end4 = (have + 3) & ~3
                payload = case.value.encode() + bytes(end4 - have + 1)
                tail = '.align 2\n'
                for at in range(end4, case.end, 4):
                    tail += (f'    /* {at:06X} {at+fill.BASE:08X} */ .asciz ""\n'
                             '    /* 00000000 */\n.align 2\n')
                ret = bytes(case.final) + payload + bytes(case.end - end4)
                native = (f'.section .text, "ax"\n\n.section .rodata, "a"\n'
                          f'dlabel {case.label}\n'
                          f'    /* {case.final:06X} {case.final+fill.BASE:08X} */ '
                          f'.asciz "{case.asm_literal}"\n'
                          f'    /* {payload.hex().upper()} */\n'
                          f'{tail}enddlabel {case.label}\n')
                suffix = '.section .lit4, "wa"\n' if unit in ('effect/effPCPNeedle', 'script/scrCommonCommand') else ''
                native += suffix
                rendered = fill.render(native,case,ret,have)
                with self.assertRaisesRegex(ValueError, 'literal differs'):
                    fill.render(native.replace(case.asm_literal, case.asm_literal.replace(r'\n', r'\012')),
                                case,ret,have)
                self.assertIn(f'.section {fill.FILL}, "a"\n    .zero {case.end-have}\n', rendered)
                with self.assertRaisesRegex(ValueError, 'literal differs'):
                    fill.render(native.replace(case.asm_literal,case.value),case,ret,have)
                with self.assertRaisesRegex(ValueError, 'annotation differs'):
                    fill.render(native,case,ret[:case.final]+b'X'+ret[case.final+1:],have)
                with self.assertRaisesRegex(ValueError, 'not terminal'):
                    fill.render(native.removesuffix(suffix) if suffix else native+'.section .lit4, "wa"\n',case,ret,have)

    def test_named_interposer_requires_receipted_sectionless_object(self):
        unit = 'script/scrCommonCommand'
        mid = fill.intervening_object(VERSION, unit)
        proof = str(mid) + '.proof.json'
        obj = write(self.root, mid, 'receipted object')
        pr = write(self.root, proof, 'object proof')
        objects = {str(mid)}
        files = {str(mid): fill.sha(obj.read_bytes()), proof: fill.sha(pr.read_bytes())}
        fake_elf = unittest.mock.Mock()
        def prove(body='', linked=objects, hashes=files):
            pins = fill.InputPins(self.root)
            fill.prove_intervening(pins, mid, linked, hashes, body)
            return pins

        with patch.object(fill, 'elf', return_value=fake_elf):
            fake_elf.iter_sections.return_value = [type('S', (), {'name': '.text'})()]
            self.assertEqual(prove(f' {mid}(.rodata)\n').digests, files)
            for name in ('.rodata', '.rodata.str', '.rel.rodata', '.rela.rodata.str'):
                with self.subTest(section=name):
                    fake_elf.iter_sections.return_value = [type('S', (), {'name': name})()]
                    with self.assertRaisesRegex(ValueError, 'has readonly section'):
                        prove()
            fake_elf.iter_sections.return_value = []
            for row in (f' .rodata 0x100 0x0 {mid}\n',
                        f' .rodata.str\n        0x100 0x1 {mid}\n'):
                with self.assertRaisesRegex(ValueError, 'live readonly contribution'):
                    prove(row)
            with self.assertRaisesRegex(ValueError, 'missing live'):
                prove(linked=set())
            with self.assertRaisesRegex(ValueError, 'missing live'):
                prove(hashes={str(mid): files[str(mid)]})
            pr.write_text('changed proof')
            with self.assertRaisesRegex(ValueError, 'conflicting .* digest'):
                prove()
            pr.write_text('object proof')
            obj.write_text('changed object')
            with self.assertRaisesRegex(ValueError, 'conflicting .* digest'):
                prove()

    def test_batch_interposer_dependencies_are_exact(self):
        unit = 'script/scrCommonCommand'
        mid = fill.intervening_object(VERSION, unit)
        with patch.object(fill, 'current_splat_inputs', return_value=set()):
            check = lambda opts: fill.reference_dependencies(self.root, VERSION, SERIAL, opts)
            self.assertGreaterEqual(check((PB, unit)), check((PB,)) | {str(mid), str(mid)+'.proof.json'})
            self.assertNotIn(str(mid), check((PB,)))
            self.assertNotIn(str(mid)+'.proof.json', check((PB,)))
            for other in ('code_001C55F8', 'field/fldTitle'):
                self.assertNotIn(str(fill.intervening_object(VERSION, other)), check((PB, unit)))

    def test_intervening_selector_is_named_unique_and_consecutive(self):
        for unit, name in fill.INTERVENING_SELECTORS.items():
            with self.subTest(unit=unit):
                case = fill.CASES[unit]
                a, m, b = map(cobj, (unit, name, case.next))
                have = case.final + len(case.value) + 1
                size, nsize = have - case.begin, 16
                plan = {'segments': [{'name': 'main', 'subsegments': [
                    [case.begin, '.rodata', unit], [case.end, '.rodata', case.next]]}]}
                ld = ''.join(f'{p}(.rodata);\n' for p in (a, m, b))
                row = f' {m}(.rodata)\n'
                body = (f' {a}(.rodata)\n .rodata 0x{case.begin+fill.BASE:x} 0x{size:x} {a}\n'
                        f'{row} {b}(.rodata)\n'
                        f' *fill* 0x{have+fill.BASE:x} 0x{case.end-have:x} 00000000\n'
                        f' .rodata 0x{case.end+fill.BASE:x} 0x{nsize:x} {b}')
                def prove(script, link_map):
                    fill.adjacency(case, plan, script, link_map, have, size, nsize)

                prove(ld, body)
                with self.assertRaisesRegex(ValueError, 'exact linker selectors'):
                    prove(ld.replace(str(m), str(cobj('unknown'))), body)
                with self.assertRaisesRegex(ValueError, 'exact linker selectors'):
                    prove(ld + f'{m}(.rodata);\n', body)
                with self.assertRaisesRegex(ValueError, 'unique live'):
                    prove(ld, body.replace(row, row * 2))
                with self.assertRaisesRegex(ValueError, 'exact live map'):
                    prove(ld, body.replace(row, row + f' .rodata 0x100 0x0 {m}\n'))

    def test_finite_local_text_relocation_shape_and_destinations(self):
        case = fill.CASES['effect/effBattle']
        normal = fill.LOCAL_TEXT_READONLY[case.unit]
        start, size = normal.text
        good = fill.LocalTextReadOnly(normal.text, size, 1,
                                      fill.sha(struct.pack('<II', 4, start+0x20)))
        text = {'sh_type': 'SHT_PROGBITS', 'sh_flags': 6, 'sh_addr': 0,
                'sh_size': size}
        text = elf_part(text, bytes(size), '.text')
        target = elf_part({'st_shndx': 2, 'st_info': {'type': 'STT_SECTION', 'bind': 'STB_LOCAL'},
                           'st_other': {'visibility': 'STV_DEFAULT'}, 'st_value': 0,
                           'st_size': 0}, b'', '.text')
        symtab = unittest.mock.Mock()
        symtab.get_symbol.return_value = target
        rel = unittest.mock.MagicMock(spec=fill.RelocationSection)
        rel.name = '.rel.rodata'
        hdr = {'sh_type': 'SHT_REL', 'sh_link': 8, 'sh_entsize': 8, 'sh_size': 8, 'sh_info': 4}
        rel.__getitem__.side_effect = hdr.__getitem__
        rel.num_relocations.return_value = 1
        r = {'r_info_type': 2, 'r_info_sym': 1, 'r_offset': 4}
        rel.iter_relocations.side_effect = lambda: iter([r])
        raw = struct.pack('<III', 0, 0x20, 0x20)
        e = unittest.mock.Mock()
        e.iter_sections.return_value = [rel]
        with patch.dict(fill.LOCAL_TEXT_READONLY, {case.unit: good}), \
                patch.object(fill, 'section', side_effect=lambda e, name: {
                    '.text': (2, text), '.symtab': (8, symtab)}[name]):
            project = lambda sample=raw, reference=True: fill.readonly_projection(
                e, 4, elf_part({}, sample, '.rodata'), case, reference=reference)
            self.assertEqual(project(), struct.pack('<III', 0, start+0x20, 0x20))
            empty = unittest.mock.MagicMock(spec=fill.RelocationSection)
            empty.name = '.rela.rodata'
            companion = {**hdr, 'sh_type': 'SHT_RELA', 'sh_size': 0, 'sh_entsize': 12,
                         'sh_flags': 0, 'sh_addr': 0, 'sh_addralign': 4}
            empty.__getitem__.side_effect = companion.__getitem__
            empty.num_relocations.return_value = 0
            empty.data.return_value = b''
            e.iter_sections.return_value = [rel, empty]
            with self.assertRaisesRegex(ValueError, 'pinned readonly relocation section'):
                project()
            self.assertEqual(project(reference=False), struct.pack('<III', 0, start+0x20, 0x20))
            for key, bad in (('sh_size', 12), ('sh_info', 5), ('sh_link', 7)):
                with self.subTest(companion=key):
                    old = companion[key]
                    companion[key] = bad
                    with self.assertRaisesRegex(ValueError, 'empty EE relocation companion'):
                        project(reference=False)
                    companion[key] = old
            e.iter_sections.return_value = [rel, empty, empty]
            with self.assertRaisesRegex(ValueError, 'pinned readonly relocation section'):
                project(reference=False)
            e.iter_sections.return_value = [rel]
            with self.assertRaisesRegex(ValueError, 'pinned readonly relocation section'):
                project(reference=False)
            r['r_offset'] = 8
            with self.assertRaisesRegex(ValueError, 'exact relocation sites and destinations'):
                project()
            r['r_offset'] = 4
            with self.assertRaisesRegex(ValueError, 'exact relocation sites and destinations'):
                project(struct.pack('<III', 0, 0x24, 0x20))
            with self.assertRaisesRegex(ValueError, 'destination outside text'):
                project(struct.pack('<III', 0, size, 0x20))
            r['r_info_type'] = 5
            with self.assertRaisesRegex(ValueError, 'local text R_MIPS_32'):
                project()
            r['r_info_type'] = 2
            target['st_shndx'] = 'SHN_UNDEF'
            target['st_info']['bind'] = 'STB_GLOBAL'
            with self.assertRaisesRegex(ValueError, 'local text R_MIPS_32'):
                project()
            with self.assertRaisesRegex(ValueError, 'owner .rodata has outgoing relocations'):
                fill.readonly_projection(e, 4, elf_part({}, raw, '.rodata'), fill.CASES[PB])

    def test_native_stock_skip_and_live_prefix_disagreement_fails(self):
        case = fill.CASES['effect/effBattle']
        group = lambda raw: (None, 0, elf_part({'sh_addralign': 16}, raw, '.rodata'))
        expect = b'base'
        stock = group(expect + b'\0')
        normal, skip = group(expect), group(expect)
        with patch.object(fill, 'readonly_projection',
                          side_effect=lambda e,i,sec,c,reference=False: sec.data()):
            fill.verify_native_prefix(case, normal, skip, stock, expect, expect + b'\0')
            for one, two, three, live in (
                (normal, group(b'kick'), stock, expect + b'\0'),
                (normal, skip, group(expect + b'\1'), expect + b'\0'),
                (normal, skip, stock, expect + b'\1'),
            ):
                with self.assertRaisesRegex(ValueError, 'stock/SKIP/native prefix mismatch'):
                    fill.verify_native_prefix(case, one, two, three, expect, live)

    def test_named_terminal_table_keeps_native_words_and_rejects_malformed_stock(self):
        for unit, table in fill.TERMINAL_TABLES.items():
            with self.subTest(unit=unit):
                case = fill.CASES[unit]
                target = fill.LOCAL_TEXT_READONLY[unit].text[0]
                have = fill.native_end(case)
                words = struct.pack('<I', target) * table.words
                retail = bytes(case.final) + words + bytes(4)
                rows = [
                    f'    /* {at:06X} {at+fill.BASE:08X} {words[:4].hex().upper()} */'
                    f' .word .L{target:08X}\n'
                    for at in range(case.final, have, 4)
                ]
                fill_word = (f'    /* {have:06X} {have+fill.BASE:08X} 00000000 */'
                             ' .word 0x00000000\n')
                suffix = f'.section {table.next_section}, "wa"\n'
                sample = lambda body: ('.section .text, "ax"\n\n.section .rodata, "a"\n'
                                       f'dlabel {case.label}\n{body}'
                                       f'enddlabel {case.label}\n{suffix}')
                body = ''.join(rows) + fill_word
                stock = sample(body)
                rendered = fill.render(stock, case, retail, have)
                self.assertEqual(rendered,
                    sample(''.join(rows)).replace(suffix,
                        f'\n.section {fill.FILL}, "a"\n    .zero 4\n'+suffix))
                for malformed in (
                    ''.join(rows[1:]) + fill_word,
                    rows[0] + body,
                    rows[1] + rows[0] + ''.join(rows[2:]) + fill_word,
                    body.replace(f'.L{target:08X}', 'imported_symbol', 1),
                    body.replace(f'.L{target:08X}', f'.L{target+4:08X}', 1),
                    body.replace(words[:4].hex().upper(), '00000000', 1),
                    body.replace(fill_word, fill_word.replace('0x00000000', f'.L{target:08X}')),
                    body.replace(fill_word, '\t.align 4\n'),
                ):
                    with self.assertRaisesRegex(ValueError, 'terminal table'):
                        fill.render(sample(malformed), case, retail, have)
                with self.assertRaisesRegex(ValueError, 'terminal native rodata'):
                    fill.render(stock.replace(suffix, '.section .data, "wa"\n'), case, retail, have)
                with self.assertRaisesRegex(ValueError, 'zero word'):
                    fill.render(stock, case, retail[:-1] + b'\1', have)
                with self.assertRaisesRegex(ValueError, 'zero word'):
                    fill.render(stock, case, retail, have-4)

    def test_source_manifest_rejects_changed_parameters_or_stock_assembly(self):
        a = 'config/smt3/reloc_addrs.txt'
        b = fill.full_asm(VERSION, PB)
        write(self.root, a, 'old relocation')
        write(self.root, b, 'old generated assembly')
        with patch.object(fill, 'current_splat_inputs', return_value={a, str(b)}), \
                patch('configure.source_matching_ownership', return_value=[('path', [('scope', 'asm')])]):
            stamp = fill.split_stamp(VERSION)
            (self.root / stamp).parent.mkdir(parents=True)
            fill.record_split(self.root, VERSION, SERIAL, (PB,))
            fill.check_split(self.root, VERSION, SERIAL, (PB,))
            for p, old, new in ((a, 'old relocation', 'diff reloc'), (b, 'old generated assembly', 'diff asm')):
                write(self.root, p, new)
                with self.assertRaisesRegex(ValueError, 'stale'):
                    fill.check_split(self.root, VERSION, SERIAL, (PB,))
                write(self.root, p, old)
            with patch('configure.source_matching_ownership', return_value=[('path', [])]):
                with self.assertRaisesRegex(ValueError, 'stale'):
                    fill.check_split(self.root, VERSION, SERIAL, (PB,))

    def test_assembler_inputs_reject_unpinned_files(self):
        units = (PB, 'kernel/ModeSelect')
        macro = write(self.root, fill.ASM_MACRO_PATH, "")
        for unit in units:
            write(self.root, fill.full_asm(VERSION, unit), fill.ASM_MACRO_INCLUDE)
        validate = lambda: fill.validate_asm_inputs(
            self.root, VERSION, units, lambda p: (self.root / p).read_text())
        validate()
        for directive in ('.include "untracked.inc"', '.incbin "untracked.bin"',
                          'foo: .INCBIN "untracked.bin"', fill.ASM_MACRO_INCLUDE):
            with self.subTest(directive=directive):
                second = self.root / fill.full_asm(VERSION, units[-1])
                second.write_text(fill.ASM_MACRO_INCLUDE + '\n' + directive)
                with self.assertRaisesRegex(ValueError, 'unsupported.*directive'):
                    validate()
                second.write_text(fill.ASM_MACRO_INCLUDE)
        macro.write_text('.include "nested.inc"')
        with self.assertRaisesRegex(ValueError, 'recursive.*directive'):
            validate()
        macro.write_text("")
        (self.root / 'macro.inc').symlink_to('missing')
        with self.assertRaisesRegex(ValueError, 'shadowed'):
            validate()

    def test_batch_rejects_drift_and_invalid_output_cohorts(self):
        units = (PB, 'kernel/ModeSelect')
        opts = write(self.root, fill.opt_path(VERSION), json.dumps(units))
        out = [fill.target_obj(VERSION, u) for u in units]
        with patch.object(fill, 'prepare_live') as prepare:
            for invalid in (out[:1], out[::-1], [out[0], out[0]], out + out):
                with self.assertRaisesRegex(ValueError, 'output list'):
                    fill.run_batch(self.root, VERSION, invalid)
            prepare.assert_not_called()
        pinned = fill.InputPins(self.root)
        pinned.read(opts)
        with self.assertRaisesRegex(ValueError, 'conflicting'):
            pinned.bind(opts, 'not a digest')
        opts.write_text('[]')
        with self.assertRaisesRegex(ValueError, 'changed'):
            pinned.recheck()

    def test_batch_only_publishes_after_last_unit_and_recheck(self):
        from configure import VERSIONS
        units = (PB, 'kernel/ModeSelect')
        opt_file = fill.opt_path(VERSION)
        write(self.root, opt_file, json.dumps(units))
        write(self.root, 'config/versions.json', json.dumps(VERSIONS))
        deps = {str(opt_file), 'watched-input'}
        for unit in units:
            write(self.root, fill.stock_obj(VERSION, unit), unit)
            write(self.root, fill.full_asm(VERSION, unit), fill.ASM_MACRO_INCLUDE)
        write(self.root, fill.ASM_MACRO_PATH, "")
        out = [fill.target_obj(VERSION, u) for u in units]
        watched = write(self.root, 'watched-input', 'before')
        for path in out:
            write(self.root, path, 'old')
        for fault in ('last-unit', 'late-drift', None):
            watched.write_text('before')
            def run(command, **kwargs):
                Path(command[-2]).write_bytes(b'new')
            def verify(stock, *_):
                if stock.decode() == units[-1]:
                    if fault == 'last-unit':
                        raise ValueError('rejected last unit')
                    if fault == 'late-drift':
                        watched.write_text('after')
            with patch.object(fill, 'reference_dependencies', return_value=deps), \
                    patch.object(fill, 'prepare_live') as prepare, \
                    patch.object(fill, 'check_split') as recheck_split, \
                    patch.object(fill, 'validate_unit', return_value=(None, b'', 0)), \
                    patch.object(fill, 'render', return_value=fill.ASM_MACRO_INCLUDE), \
                    patch.object(fill.subprocess, 'run', side_effect=run), \
                    patch.object(fill, 'verify_output', side_effect=verify):
                if fault:
                    with self.assertRaisesRegex(ValueError, 'last unit|changed .* batch input'):
                        fill.run_batch(self.root, VERSION, out)
                else:
                    fill.run_batch(self.root, VERSION, out)
                prepare.assert_called_once()
                self.assertEqual(recheck_split.call_count, int(fault != 'last-unit'))
            self.assertFalse(list(self.root.rglob('native-rodata-fill-*/reference.o')))
            self.assertEqual([(self.root / o).read_text() for o in out],
                             ['old' if fault else 'new'] * 2)

    def test_partial_publication_invalidates_entire_cohort(self):
        first = write(self.root, 'a.new', 'new a')
        a = write(self.root, 'a', 'old a')
        b = write(self.root, 'b', 'old b')
        marker = write(self.root, 'done', '')
        with self.assertRaises(FileNotFoundError):
            fill.publish([(first, a), (self.root / 'b.missing', b)], marker)
        self.assertFalse(marker.exists())
        self.assertFalse(a.exists())
        self.assertFalse(b.exists())

    @unittest.skipUnless(shutil.which('ninja'), 'needs Ninja runner')
    def test_missing_sibling_rebuilds_cohort_even_for_one_requested_output(self):
        out = io.StringIO()
        n = ninja_syntax.Writer(out)
        n.rule('native_rodata_fill', 'test "$out" = "first sibling" && touch $out done && echo run >> counter')
        n.build(['first', 'sibling'], 'native_rodata_fill', implicit_outputs=['done'])
        write(self.root, 'build.ninja', out.getvalue())
        def first():
            subprocess.check_output(['ninja', 'first'], cwd=self.root)
        first()
        first()
        (self.root / 'sibling').unlink()
        first()
        self.assertEqual((self.root / 'counter').read_text(), 'run\nrun\n')
        self.assertTrue((self.root / 'sibling').is_file())
        (self.root / 'done').unlink()
        first()
        first()
        self.assertEqual((self.root / 'counter').read_text(), 'run\nrun\nrun\n')
        self.assertTrue((self.root / 'done').is_file())

    @unittest.skipUnless(shutil.which('ninja'), 'needs Ninja graph reader')
    def test_existing_production_graph_does_not_depend_on_comparison_references(self):
        o = io.StringIO()
        n = ninja_syntax.Writer(o)
        n.rule('as', 'true')
        n.rule('ld', 'true')
        n.rule('native_rodata_fill', 'true')
        with patch.object(fill, 'current_splat_inputs', return_value={'config/smt3/reloc_addrs.txt'}):
            mode = 'kernel/ModeSelect'
            for unit in (mode, fill.CASES[mode].next):
                self.selectors[cobj(unit)] = ['.text', '.rodata']
            write(self.root, fill.ASM_MACRO_PATH, "")
            for unit in (PB, mode):
                write(self.root, fill.full_asm(VERSION, unit), fill.ASM_MACRO_INCLUDE)
            fill.write_reference_batch(n, self.root, VERSION, SERIAL, self.selectors, (PB, mode))
        l = f'build/smt3/{SERIAL}'
        n.build(l + '.elf', 'ld', [str(self.pri), str(self.nxt)],
                implicit_outputs=[l + '.map', l + '.elf.proof.json'])
        n.build(l, 'phony', l + '.elf')
        n.build(l + '.ok', 'phony', l)
        n.build('smt3', 'phony', l + '.ok')
        ref = str(fill.target_obj(VERSION, PB))
        backup = str(fill.stock_obj(VERSION, PB))
        n.build('report', 'phony', ref)
        write(self.root, 'build.ninja', o.getvalue())
        graph = lambda target: subprocess.check_output(['ninja', '-t', 'graph', target],
                                                        cwd=self.root).decode()
        prod, report = graph('smt3'), graph('report')
        self.assertNotIn(ref, prod)
        self.assertNotIn(backup, prod)
        other = graph(str(fill.target_obj(VERSION, mode)))
        self.assertIn(backup, other)
        self.assertEqual(other.count('label="native_rodata_fill"'), 1)
        self.assertEqual(report.count('label="native_rodata_fill"'), 1)
        for edge in (ref, backup, l + '.elf.proof.json', l + '.ok',
                     'build/smt3/base/src/smt3/pb/pbDebugMode.o',
                     'config/smt3/reloc_addrs.txt', str(fill.stock_obj(VERSION, mode)),
                     'tools/bin/mips-ps2-decompals-as'):
            self.assertIn(edge, report)


if __name__ == '__main__':
    unittest.main()
