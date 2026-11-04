#!/usr/bin/env python3
"""Fail if a global whose type is quad-aligned on the host sits at a retail
address that is not 16-aligned.

The host data image places globals at their retail addresses, so a type that
promises aligned(16) (QWORD_ALIGNED, directly or through a member) faults an
lq/sq-turned-movaps at an address the retail game never aligned. Such a global
is declared with an unaligned type instead (sdfVector4Unaligned_t).
Addresses come from `nm` on the built ELF.
"""
import argparse, glob, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def strip(s):
    s = re.sub(r'/\*.*?\*/', ' ', s, flags=re.S)
    return re.sub(r'//[^\n]*', ' ', s)


def quad_types(texts):
    """Names of every typedef that is 16-aligned on the host."""
    bodies = {}
    for t in texts:
        for m in re.finditer(r'typedef\s+(?:struct|union)\s*(QWORD_ALIGNED)?\s*\w*\s*\{(.*?)\}\s*(\w+)\s*;', t, re.S):
            bodies[m.group(3)] = (bool(m.group(1)), m.group(2))
    quad = {n for n, (q, _) in bodies.items() if q}
    changed = True
    while changed:
        changed = False
        for n, (_, body) in bodies.items():
            if n in quad:
                continue
            for mem in re.finditer(r'^\s*(?:const\s+|volatile\s+)*(\w+)\s+\w+(?:\[[^\]]*\])*\s*;', body, re.M):
                if mem.group(1) in quad:
                    quad.add(n)
                    changed = True
                    break
    return quad


def declared_globals(texts, quad):
    """(name, type) for column-0 declarations of a quad-aligned type by value."""
    out = []
    head = re.compile(r'(?:extern\s+|static\s+)?(?:const\s+|volatile\s+)*(\w+)\s+(.*)')
    for t in texts:
        for stmt in re.split(r'(?<=;)', t):
            line = stmt.lstrip('\n')
            if not line or line[0] in ' \t}' or '(' in line or '*' in line.split('=')[0]:
                continue
            m = head.match(line.split('\n')[0] if '\n' not in line.strip() else line.strip().replace('\n', ' '))
            if not m or m.group(1) not in quad:
                continue
            for part in m.group(2).split('=')[0].rstrip(';').split(','):
                n = re.match(r'\s*([A-Za-z_]\w*)', part)
                if n:
                    out.append((n.group(1), m.group(1)))
    return out


def symbols(elf):
    nm = subprocess.run(['nm', elf], capture_output=True, text=True)
    if nm.returncode != 0 or not nm.stdout:
        sys.exit(f'check_qword_globals: cannot look: nm {elf} failed: {nm.stderr.strip()}')
    return {p[2]: int(p[0], 16) for p in (l.split() for l in nm.stdout.splitlines()) if len(p) == 3}


def find_misaligned(texts, syms):
    quad = quad_types(texts)
    seen, bad, checked = set(), [], 0
    for name, typ in declared_globals(texts, quad):
        if name in seen or name not in syms:
            continue
        seen.add(name)
        checked += 1
        if syms[name] % 16:
            bad.append((name, typ, syms[name]))
    return bad, checked


def control():
    """The check must flag a misaligned quad global and pass an aligned one."""
    src = ['typedef struct QWORD_ALIGNED { f32 x; } vec_t;\n'
           'typedef struct { vec_t v; } wrap_t;\n'
           'extern vec_t bad_a;\nextern wrap_t bad_b;\nextern vec_t good;\nextern f32 plain;\n']
    bad, checked = find_misaligned(src, {'bad_a': 8, 'bad_b': 0x14, 'good': 16, 'plain': 4})
    if sorted(n for n, _, _ in bad) != ['bad_a', 'bad_b'] or checked != 3:
        sys.exit(f'check_qword_globals: control failed: {bad} {checked}')


def main():
    control()
    ap = argparse.ArgumentParser()
    ap.add_argument('--elf', default=os.path.join(ROOT, 'build/smt3/SLUS_209.11.elf'))
    ap.add_argument('--src', default=ROOT)
    a = ap.parse_args()
    if not os.path.isfile(a.elf):
        sys.exit(f'check_qword_globals: cannot look: {a.elf} is absent')
    files = glob.glob(a.src + '/include/*.h') + glob.glob(a.src + '/src/**/*.c', recursive=True)
    if not files:
        sys.exit(f'check_qword_globals: cannot look: no sources under {a.src}')
    texts = [strip(open(f, errors='replace').read()) for f in files]
    quad = quad_types(texts)
    if 'sdfVector4_t' not in quad:
        sys.exit('check_qword_globals: sdfVector4_t is not QWORD_ALIGNED')
    bad, checked = find_misaligned(texts, symbols(a.elf))
    for name, typ, addr in sorted(bad):
        print(f'{name}: {typ} at 0x{addr:08x} is not 16-aligned')
    print(f'check_qword_globals: {checked} quad-aligned globals checked, {len(bad)} misaligned')
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
