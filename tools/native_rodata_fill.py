"""Validate and partition explicit native mixed-owner report references, never link inputs."""
from dataclasses import dataclass
import hashlib
from contextlib import ExitStack
import io
from importlib.metadata import version as package_version
import json
from pathlib import Path
import re
import struct
import shlex
import subprocess
import sys
import tempfile
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

from .linker_inputs import parse_object_sections
from .native_data_tail import name_records, verify_reloc_overrides, yaml_inputs
from .native_rodata_incoming import verify_live_no_incoming
from .production_report import (live_input_sections, read_object_receipt,
                                read_receipt, verify_linked_image, write_json)

BASE=0xFF000
FILL='.smt3_linker_fill'
RETAIL_SHA1='5facfbf7916836613449c922e26a4ec412207b32'
IDENT=r'[A-Za-z_][\w.$]*'

@dataclass(frozen=True)
class Case:
    unit: str
    next: str
    begin: int
    end: int
    final: int
    label: str
    value: str
    align: int
    asm_literal: str | None = None

MODE_SELECT='kernel/ModeSelect'
MODE_TEXT=(0x229030,0x1530)
MODE_SWITCH=(0x2296B8,0x22A434)
MODE_R32_OFFSETS=tuple(range(0x70,0xcc,4))

CASES={
    'pb/pbDebugMode': Case('pb/pbDebugMode','model/mdlManager',0x437638,0x43B648,
                         0x43B630,'pbFileMiscColorbarTB','misc/colorbar.TB',8),
    'event/evtCommand': Case('event/evtCommand','event/evtMoon',0x43B8A8,0x43C060,
                         0x43C040,'D_0053B040','/fld/f/bin/moon_07.tmx',16),
    MODE_SELECT: Case(MODE_SELECT,'kernel/dds3CreateData',0x43EEC0,0x43FFB8,
                      0x43FFA0,'D_0053EFA0','/staff/stf_mpic01.tmx',8),
    'pb/pbDraw': Case('pb/pbDraw','pb/pbMain',0x436CD8,0x436D50,
                      0x436D40,'D_00535D40','/pb/pbmsg.bf',16),
    'newbattle/nbCameraProcess': Case('newbattle/nbCameraProcess','newbattle/nbCameraStartEnd',
                      0x4355A8,0x4355F0,0x4355D8,'D_005345D8','nb_camera_draw',16),
    'pb/pbGameDraw': Case('pb/pbGameDraw','pb/pbRecord',0x437040,0x4371C8,
                      0x4371B0,'pbDrawMapCursorProcName','pb-draw-map cursor',8),
    'sdf/sdfHdd': Case('sdf/sdfHdd','sdf/sdfDev9',0x44A6C8,0x44A838,
                      0x44A828,'D_00549828','hdd0:dds3,',8),
    'pb/pbRecord': Case('pb/pbRecord','pb/pbHelp',0x4371C8,0x437398,0x437388,
                      'D_00536388','\t<floor = %d>\n',8,r'\t<floor = %d>\n'),
    'effect/billManager': Case('effect/billManager','effect/parManager',0x433C28,
                      0x433D50,0x433D30,'D_00532D30','BillManager Load...[%s]\n',
                      16,r'BillManager Load...[%s]\n'),
    'effect/effPCPNeedle': Case('effect/effPCPNeedle','effect/effPCPMisc',
                          0x434C68,0x434CC0,0x434C90,'D_00533C90',
                          'dds3NeedleModelVNCListDelete[%p] ****\n',16,
                          r'dds3NeedleModelVNCListDelete[%p] ****\n'),
    'kernel/dds3KernelCore': Case('kernel/dds3KernelCore', 'sdf/sdfDefaultMain',
                      0x42ED18, 0x42EE60, 0x42EE48, 'D_0052DE48',
                      'now process [%s]\n', 8, 'now process [%s]\\n'),
    'kernel/dds3CreateData': Case('kernel/dds3CreateData', 'staff/stfMain',
                      0x43FFB8, 0x4400F0, 0x4400D8, 'D_0053F0D8',
                      '/staff/capcom_logo.tmx', 8),
    'facility/fclRagInit': Case('facility/fclRagInit', 'facility/fclRagCalc',
                      0x43E140, 0x43E210, 0x43E200, 'ragUpdateProcName',
                      'rag_update', 16),
    'basic/dds3SceneBasic': Case('basic/dds3SceneBasic', 'basic/dds3AreaBasic',
                      0x430730, 0x4307C0, 0x4307A0, 'D_0052F7A0',
                      'script procname invalid [%s]\n', 8, 'script procname invalid [%s]\\n'),
    'pipe/pipeEffect': Case('pipe/pipeEffect', 'pipe/pipeMakaAnim',
                      0x449F58, 0x449FD8, 0x449FC8, 'D_00548FC8',
                      '/pipe/FPS3.D3P', 8),
    'interface/frTestFont': Case('interface/frTestFont', 'interface/itfMisc',
                      0x436A10, 0x436A68, 0x436A38, 'D_00535A38',
                      'host0:../../../dds3data/facility/test.bf', 8),
    'script/scrCommonCommand': Case('script/scrCommonCommand','basic/dds3DbgBasic',
        0x430588,0x4306B8,0x430698,'D_0052F698','error: MFILTER_PARA blend\n',8,
        'error: MFILTER_PARA blend\\n'),
    'code_001C55F8': Case('code_001C55F8','interface/frFont',
        0x436830,0x4368F0,0x4368E0,'D_005358E0','FRAME: %d/%d',16),
    'field/fldTitle': Case('field/fldTitle','mc/slMain',
        0x4337B0,0x433860,0x433848,'D_00532848','/fld/f/pnl/dun_04.tmx',16),
    'sdf/sdfSound': Case('sdf/sdfSound','sdf/sdfDevFile',0x44A8A0,0x44B2D8,
                      0x44B2B8,'D_0054A2B8','       for %3d .. %4d : %d\n',8,
                      r'       for %3d .. %4d : %d\n'),
    'event/evtEventViewerMenu': Case('event/evtEventViewerMenu','event/evtSound',0x43CCC0,0x43D5B8,
                      0x43D5A8,'D_0053C5A8','%s_MOTION',8),
    'effect/effBattle': Case('effect/effBattle','effect/effBattleMisc',0x433E20,0x434080,
                      0x434068,'D_00533068','nb_effect_draw',16),
}
# These exact objects have a selector but emit no readonly section at all.
# Never infer or skip empty objects for owners not explicitly named here.
INTERVENING_SELECTORS = {
    'script/scrCommonCommand': 'script/scrParameterCommand',
    'code_001C55F8': 'interface/fmGslCont',
    'field/fldTitle': 'field/fldSnow',
}

def must(ok, why):
    if not ok:
        raise ValueError(why)
def sha(b):
    return hashlib.sha256(b).hexdigest()
def elf(b,kind='ET_REL'):
    e=ELFFile(io.BytesIO(b))
    must(e.elfclass==32 and e.little_endian and e['e_machine']=='EM_MIPS' and e['e_type']==kind,'ELF identity')
    return e
def section(e,name):
    found=[(i,s) for i,s in enumerate(e.iter_sections()) if s.name==name]
    must(len(found)==1,'unique section: '+name)
    return found[0]
def ro(e):
    i,s=section(e,'.rodata')
    must(s['sh_flags']==2 and s['sh_type']=='SHT_PROGBITS' and s['sh_size']>0,'readonly section')
    return i,s
def sym_sem(e,s):
    sh=s['st_shndx']
    return (s.name,e.get_section(sh).name if isinstance(sh,int) else sh,s['st_value'],s['st_size'],
            s['st_info']['type'],s['st_info']['bind'],s['st_other']['visibility'])
def inspect_symbols(e,lo,hi,sec,allow=None):
    _,s=section(e,'.symtab')
    for t in s.iter_symbols():
        if (t['st_shndx']!=sec and t['st_shndx']!='SHN_ABS') or t['st_info']['type'] in ('STT_SECTION','STT_FILE'): continue
        n,a,z=t.name,t['st_value'],t['st_size']
        if allow and (n,a,z)==allow: continue
        must(not (lo<=a<hi or a<lo<a+z),'symbol intersects fill: '+n)
def mapped(e,begin,end):
    p=[s for s in e.iter_segments() if s['p_type']=='PT_LOAD'
         and s['p_vaddr']<=begin<end<=s['p_vaddr']+s['p_filesz']]
    must(len(p)==1,'unique file-backed load mapping')
    s=p[0]; at=s['p_offset']+begin-s['p_vaddr']
    e.stream.seek(at)
    return at,e.stream.read(end-begin)
def prove_intervening(pins, obj, objects, files, map_body):
    name = str(obj)
    proof = name + '.proof.json'
    must(name in objects and name in files and proof in files,
         'missing live intervening selector receipt')
    # Both are explicit batch dependencies and must still match the live-link receipt.
    for path in (name, proof):
        pins.bind(path, files[path])
    raw = pins.read(obj)
    pins.read(proof)
    readonly = re.compile(r'(?:\.rel(?:a)?)?\.rodata(?:\..*)?\Z')
    must(not any(readonly.fullmatch(sec.name) for sec in elf(raw).iter_sections()),
         'intervening selector has readonly section or relocation section')
    # The normal map parser ignores zero-sized rows. For this exception even
    # a zero-sized readonly input row contradicts the empty-selector proof.
    contributions = re.findall(
        r'^ ([.][^\s]+)[ \t]*(?:\n[ \t]+|[ \t]+)'
        r'0x[0-9a-fA-F]+[ \t]+0x[0-9a-fA-F]+[ \t]+(\S+\.o)[ \t]*$',
        map_body, re.M)
    must(not any(path == name and readonly.fullmatch(sec) for sec, path in contributions),
         'intervening selector has live readonly contribution')


def adjacency(c,plan,ld,linkmap,start,psize,ns):
    main=[i for i in plan['segments'] if isinstance(i,dict) and i.get('name')=='main']
    must(len(main)==1,'unique native main')
    rows=main[0]['subsegments']
    matches=[i for i,row in enumerate(rows) if row==[c.begin,'.rodata',c.unit]]
    must(len(matches)==1 and rows[matches[0]+1]==[c.end,'.rodata',c.next],'exact YAML successor')
    path=lambda n:f'build/smt3/src/smt3/{n}.o'
    a,b=map(path,(c.unit,c.next))
    lds=[i.strip() for i in ld.splitlines()]
    pri,foll=f'{a}(.rodata);',f'{b}(.rodata);'
    interposer = INTERVENING_SELECTORS.get(c.unit)
    middle = path(interposer) if interposer else None
    expected = [pri, f'{middle}(.rodata);', foll] if middle else [pri, foll]
    must(all(lds.count(i) == 1 for i in expected) and
         lds[lds.index(pri):lds.index(pri)+len(expected)] == expected,
         'exact linker selectors')
    selector_only = f' {middle}(.rodata)\n' if middle else ''
    if middle:
        must(len(re.findall(rf'^\s*{re.escape(middle)}\(\.rodata\)[ \t]*$', linkmap, re.M)) == 1,
             'unique live intervening selector')

    startv,endv,gap=start+BASE,c.end+BASE,c.end-start
    pat=(rf'^ {re.escape(a)}\(\.rodata\)\n(?: \*fill\* +0x[0-9a-f]+ +0x[0-9a-f]+ 00000000\n)? \.rodata +0x0*{c.begin+BASE:x} +0x0*{psize:x} {re.escape(a)}\n'
         rf'(?: +0x[0-9a-f]+ +{IDENT}\n)*'
         + re.escape(selector_only) +
         rf' {re.escape(b)}\(\.rodata\)\n \*fill\* +0x0*{startv:x} +0x0*{gap:x} 00000000\n'
         rf' \.rodata +0x0*{endv:x} +0x0*{ns:x} {re.escape(b)}$')
    must(len(re.findall(pat,linkmap,re.M))==1,'exact live map fill and successor')
def verify_metadata(symbols,relocs,start,end):
    for name,(a,n) in name_records(symbols).items():
        must(not (start+BASE<=a<end+BASE or n and a<start+BASE<a+n),
             'native symbol intersects fill: '+name)
    verify_reloc_overrides(relocs,start,end)

def local_incoming(e,rid,have,span):
    """Conservative for these objects: test every possible same-symbol HI/LO pair."""
    found=0
    for rs in e.iter_sections():
        if not isinstance(rs,RelocationSection): continue
        st=e.get_section(rs['sh_link'])
        groups={}
        for r in rs.iter_relocations():
            width=4
            site=r['r_offset']
            if rs['sh_info']==rid:
                must(not (site<span and have<site+width),'outgoing relocation intersects fill')
            t=st.get_symbol(r['r_info_sym'])
            if t['st_shndx']!=rid: continue
            found+=1
            must(rs['sh_info']!=rid and not r.is_RELA(),'unsupported incoming rodata relocation')
            body=e.get_section(rs['sh_info']).data()
            must(site%4==0 and 0<=site<=len(body)-4,'incoming relocation extent')
            word=struct.unpack_from('<I',body,site)[0]
            op=r['r_info_type']
            must(op in (2,5,6),'unsupported incoming rodata relocation type')
            form=groups.setdefault(r['r_info_sym'],(t['st_value'],[],[],[]))
            form[{5:1,6:2,2:3}[op]].append(word)
        for v,hi,lo,words in groups.values():
            must(bool(hi)==bool(lo),'unpaired incoming HI16/LO16')
            signed=lambda x:(x&0x7fff)-(x&0x8000)
            dest=[v+w for w in words]+[v+((h&0xffff)<<16)+signed(l) for h in hi for l in lo]
            must(not any(have<=a<span for a in dest),'incoming relocation targets fill')
    return found
@dataclass(frozen=True)
class LocalTextReadOnly:
    text: tuple[int,int]
    stock_text_size: int
    slots: int
    sites_and_targets: str


# Only these three existing jump-table owners use this data. Hash the ordered
# (rodata-relative site, linked text virtual address) LE u32 pairs, independently
# identical in production, SKIP and stock. The linked text window is stricter
# than the complete stock .text for two owners (four terminal zero bytes).
LOCAL_TEXT_READONLY = {
    'sdf/sdfSound': LocalTextReadOnly((0x2C0E90,0x27D0),0x27D0,137,
        'df86c98550f2bb970e908cdaffc16914f4101fce81a83a7c226fe019a3665248'),
    'event/evtEventViewerMenu': LocalTextReadOnly((0x1F4B28,0x53BC),0x53C0,132,
        'cec7986b36b371ba4213d09bb9ca9a8cc53f67f12b0428d0706a655a168fdbe9'),
    'effect/effBattle': LocalTextReadOnly((0x16A9C0,0x1C24),0x1C28,12,
        '887f3834b092b3f0eed3f81f52415d0211e8e0e537602b42705c0903f7018b52'),
}


def project_local_text(e,index,outgoing,raw,c,layout,reference):
    must(len(outgoing)==(1 if reference else 2) and outgoing[0].name=='.rel.rodata' and
         outgoing[0]['sh_type']=='SHT_REL' and outgoing[0]['sh_info']==index,
         'pinned readonly relocation section')
    ti,ts=section(e,'.text')
    start,extent=layout.text
    size=layout.stock_text_size if reference else extent
    must(ts['sh_type']=='SHT_PROGBITS' and ts['sh_flags']==6 and
         ts['sh_addr']==0 and ts['sh_size']==size and
         not any(ts.data()[extent:]),'pinned readonly relocation text extent')
    sid,st=section(e,'.symtab')
    # The pinned EE assembler emits this one empty RELA companion in
    # production/SKIP, while the GNU comparison assembler emits none.
    if not reference:
        empty=outgoing[1]
        must(empty.name=='.rela.rodata' and empty['sh_type']=='SHT_RELA' and
             empty['sh_info']==index and empty['sh_link']==sid and
             empty['sh_size']==0 and empty['sh_entsize']==12 and
             empty['sh_flags']==empty['sh_addr']==0 and empty['sh_addralign']==4 and
             empty.num_relocations()==0 and empty.data()==b'',
             'pinned readonly empty EE relocation companion')
    rs=outgoing[0]
    must(rs['sh_link']==sid and rs['sh_entsize']==8 and
         rs['sh_size']==8*layout.slots and rs.num_relocations()==layout.slots,
         'pinned readonly relocation symbol table or count')
    out=bytearray(raw)
    pairs=bytearray()
    last=-1
    payload_end=c.final+len(c.value.encode())+1-c.begin
    for r in rs.iter_relocations():
        t=st.get_symbol(r['r_info_sym'])
        must(r['r_info_type']==2 and t['st_shndx']==ti and
             t['st_info']['type']=='STT_SECTION' and t['st_info']['bind']=='STB_LOCAL' and
             t['st_other']['visibility']=='STV_DEFAULT' and t.name in ('','.text') and
             t['st_value']==t['st_size']==0,'pinned readonly jump requires local text R_MIPS_32')
        off=r['r_offset']
        must(last<off and off%4==0 and off+4<=min(len(raw),payload_end),
             'pinned readonly relocation site outside payload or order')
        last=off
        dest=struct.unpack_from('<I',raw,off)[0]
        must(dest<extent and dest%4==0,'pinned readonly relocation destination outside text')
        pairs.extend(struct.pack('<II',off,start+dest))
        struct.pack_into('<I',out,off,start+dest)
    must(sha(pairs)==layout.sites_and_targets,'pinned readonly exact relocation sites and destinations')
    return bytes(out)


def readonly_projection(e,index,data,c,*,reference=False):
    outgoing=[r for r in e.iter_sections() if isinstance(r,RelocationSection)
              and r['sh_info']==index and r.num_relocations()]
    raw=data.data()
    if (layout := LOCAL_TEXT_READONLY.get(c.unit)) is not None:
        # Inspect all sections, including the exactly pinned EE empty companion.
        related=[r for r in e.iter_sections() if isinstance(r,RelocationSection) and
                 (r['sh_info']==index or r.name in ('.rel.rodata','.rela.rodata'))]
        return project_local_text(e,index,related,raw,c,layout,reference)
    if c.unit!=MODE_SELECT:
        must(not outgoing,'owner .rodata has outgoing relocations')
        return raw

    # Only ModeSelect's known 23-case mselMain switch is admitted. Both
    # assemblers emit REL to the local .text section symbol; GNU spells its
    # name .text, while the stock EE assembler leaves it blank.
    must(len(outgoing)==1 and outgoing[0].name=='.rel.rodata' and
         outgoing[0]['sh_type']=='SHT_REL','ModeSelect jump table relocation section')
    ti,ts=section(e,'.text')
    start,extent=MODE_TEXT
    must(ts['sh_type']=='SHT_PROGBITS' and ts['sh_flags']==6 and
         ts['sh_addr']==0 and ts['sh_size']==extent,'ModeSelect original text section')
    sid,st=section(e,'.symtab')
    rs=outgoing[0]
    must(rs['sh_link']==sid and rs['sh_entsize']==8 and
         rs['sh_size']==8*len(MODE_R32_OFFSETS),'ModeSelect jump table symbol table or extent')
    entries=list(rs.iter_relocations())
    must(tuple(r['r_offset'] for r in entries)==MODE_R32_OFFSETS,
         'ModeSelect exact 23 native jump table slots')
    out=bytearray(raw)
    for r in entries:
        t=st.get_symbol(r['r_info_sym'])
        must(r['r_info_type']==2 and t['st_shndx']==ti and
             t['st_info']['type']=='STT_SECTION' and t['st_info']['bind']=='STB_LOCAL' and
             t['st_other']['visibility']=='STV_DEFAULT' and t.name in ('','.text') and
             t['st_value']==t['st_size']==0,'ModeSelect jump table requires local text R_MIPS_32')
        off=r['r_offset']
        must(off+4<=len(raw),'ModeSelect jump table points outside read-only payload')
        dest=start+struct.unpack_from('<I',raw,off)[0]
        must(MODE_SWITCH[0]<=dest<MODE_SWITCH[1] and dest%4==0,
             'ModeSelect jump destination outside mselMain')
        struct.pack_into('<I',out,off,dest)
    return bytes(out)

def render(stock,c,ret,have):
    """Preserve the full native listing outside the verified terminal label body."""
    lab=re.escape(c.label)
    m=list(re.finditer(rf'(?ms)^dlabel {lab}\n(.*?)^enddlabel {lab}\n',stock))
    must(len(m)==1,'missing/ambiguous terminal label')
    m=m[0]
    rest=stock[m.end():].lstrip()
    # These are exact generated seams for the named cases, not accepted
    # alternatives for other owners. All peer sections are still compared.
    if c.unit in (MODE_SELECT, 'pb/pbGameDraw', 'pb/pbRecord', 'effect/billManager', 'basic/dds3SceneBasic', 'interface/frTestFont', 'field/fldTitle'):
        correctly_terminal=not rest
    elif c.unit in ('sdf/sdfHdd', 'kernel/dds3KernelCore', 'kernel/dds3CreateData', 'facility/fclRagInit', 'pipe/pipeEffect'):
        correctly_terminal=rest.startswith('.section .sdata, "wa"\n')
    else:
        correctly_terminal=rest.startswith('.section .lit4, "wa"\n')
    must(len(re.findall(r'^\.section \.rodata, "a"$',stock,re.M))==1 and
         stock[:m.start()].rfind('\n.section ')==stock[:m.start()].rfind('\n.section .rodata, "a"') and
         correctly_terminal,'not terminal native rodata label')
    lines=m[1].splitlines()
    # Only the named cases above supply pinned escaped assembler spellings.
    # Real native bytes still use the independently specified literal value.
    display=c.value if c.asm_literal is None else c.asm_literal
    primary=f'    /* {c.final:06X} {c.final+BASE:08X} */ .asciz "{display}"'
    must(len(lines)>=2 and lines[0]==primary,'terminal real literal differs')
    prefix=re.fullmatch(r'    /\* ([0-9A-F]+) \*/',lines[1])
    must(prefix is not None,'terminal literal byte annotation')
    note=bytes.fromhex(prefix[1])
    must(len(note)%4==0 and have-c.final<=len(note) and note==ret[c.final:c.final+len(note)],
         'terminal literal annotation differs from native')
    at=have
    for line in lines[2:]:
        m_align=re.fullmatch(r'\.align ([23])',line)
        m_empty=re.fullmatch(r'    /\* ([0-9A-F]{6}) ([0-9A-F]{8}) \*/ \.asciz ""',line)
        if m_align:
            n=1<<int(m_align[1]); at=(at+n-1)&-n
        elif m_empty:
            must((int(m_empty[1],16),int(m_empty[2],16))==(at,at+BASE),'unproved padding directive position')
            at+=1
        else:
            must(line=='    /* 00000000 */','unexpected terminal directive (possibly symbol/relocation)')
        must(at<=c.end,'terminal directive crosses seam')
    must(at==c.end,'generated tail not exact complete fill')
    return stock[:m.start()]+f'dlabel {c.label}\n{primary}\nenddlabel {c.label}\n\n.section {FILL}, "a"\n    .zero {c.end-have}\n'+stock[m.end():]

def verify_output(stock,built,c,ret,have):
    a,b=map(elf,(stock,built))
    ai,ar=ro(a); bi,br=ro(b)
    _,fill=section(b,FILL)
    must(fill['sh_flags']==2 and fill['sh_type']=='SHT_PROGBITS' and fill['sh_addralign']==1 and
         fill.data()==ret[have:c.end] and not any(fill.data()),'new audit section not exact fill')
    must(ar.data()==br.data()+fill.data() and
         readonly_projection(a,ai,ar,c,reference=True)==ret[c.begin:c.end] and
         readonly_projection(b,bi,br,c,reference=True)==ret[c.begin:have] and
         ar['sh_addralign']==br['sh_addralign'],'reassembled prefix bytes/alignment')
    sig=lambda s:(s.name,s['sh_type'],s['sh_flags'],s['sh_size'],s['sh_addralign'],s['sh_entsize'],s.data())
    excluded={'','.rodata',FILL,'.symtab','.strtab','.shstrtab'}
    peers=lambda e:[sig(s) for s in e.iter_sections() if s.name not in excluded and not isinstance(s,RelocationSection)]
    must(peers(a)==peers(b),'altered peer section bytes')
    sym=lambda e:[sym_sem(e,s) for s in section(e,'.symtab')[1].iter_symbols()]
    sa,sb=map(sym,(a,b))
    normalized=[]
    for s in sb:
        if s[1]==FILL:
            must(s[4]=='STT_SECTION' and s[3]==0,'unexpected symbol in audit section'); continue
        if s[0]==c.label:
            must(s[1:4]==('.rodata',c.final-c.begin,have-c.final),'wrong terminal object extent')
            normalized.append(s[:3]+(c.end-c.final,)+s[4:])
        else:
            normalized.append(s)
    must(sa==normalized,'unexpected reference symbol/address/extent change')
    def relocations(e):
        out={}
        for rs in e.iter_sections():
            if not isinstance(rs,RelocationSection): continue
            src=e.get_section(rs['sh_info']).name
            must(src!=FILL,'unexpected outgoing audit relocation')
            st=e.get_section(rs['sh_link'])
            ent=[]
            for r in rs.iter_relocations():
                t=sym_sem(e,st.get_symbol(r['r_info_sym']))
                must(t[1]!=FILL,'unexpected incoming audit relocation')
                if t[0]==c.label: t=t[:3]+(0,)+t[4:]
                ent.append((r['r_offset'],r['r_info_type'],t,r.entry.get('r_addend')))
            # readonly_projection has already strictly admitted the only
            # outgoing read-only section that may be present in this case.
            must(rs.name not in out and (src!='.rodata' or c.unit==MODE_SELECT or c.unit in LOCAL_TEXT_READONLY),
                 'unexpected target rodata relocation section')
            out[rs.name]=(src,ent)
        return out
    must(relocations(a)==relocations(b),'altered native code or data relocations')

def actual_alignment(c,have,al):
    must(al==c.align and al>1 and al&(al-1)==0 and (have+al-1)&-al==c.end,'actual following owner alignment')


GENERATOR_INPUTS = ('requirements.txt', 'configure.py', 'tools/resolve_jtbl_targets.py',
                    'tools/include_rodata.py', 'tools/include_sdata.py',
                    'tools/vu_include.py', 'tools/label_string_padding.py')
RUNTIME_INPUTS = ('tools/native_rodata_fill.py', 'tools/native_rodata_incoming.py',
                  'tools/native_data_tail.py',
                  'tools/linker_inputs.py', 'tools/production_report.py',
                  'tools/retail_elf.py', 'tools/check_unit.py', 'tools/symbol_aliases.py',
                  'config/versions.json', 'include/macro.inc')

def opt_path(version):
    return Path(f'config/{version}/native_rodata_fills.json')

def options(root,version):
    path=root/opt_path(version)
    if not path.exists():
        return ()
    data=json.loads(path.read_text())
    must(version=='smt3' and isinstance(data,list) and
         all(isinstance(v,str) and v in CASES for v in data) and
         len(set(data))==len(data)>0,'invalid native readonly fill opt-ins')
    return tuple(data)

def split_stamp(version):
    return Path(f'build/{version}/native-rodata-fill.splat.json')

def full_asm(version,unit):
    return Path(f'asm/{version}/{unit}.s')

def stock_obj(version,unit):
    return Path(f'build/{version}/native-rodata-fill-stock/{unit}.o')

def target_obj(version,unit):
    return Path(f'build/{version}/target')/full_asm(version,unit).with_suffix('.o')

def current_splat_inputs(root,version,serial,opts):
    conf,_,rels=yaml_inputs(root,version,serial)
    cfg=Path(f'config/{version}')
    paths={str(p.relative_to(root)) for p in (conf,*rels)}|set(GENERATOR_INPUTS)|{
        str(opt_path(version)),str(cfg/'symbol_addrs.txt'),f'orig/{version}/{serial}',
        *(str(full_asm(version,u)) for u in opts),
        *(f'src/{version}/{u}.c' for u in set(opts)|{CASES[u].next for u in opts}),
    }
    aliases=cfg/'symbol_aliases.json'
    if (root/aliases).is_file():
        paths.add(str(aliases))
    return paths

def split_provenance(root,version,serial,opts):
    # The Splat generator only reads C's ownership signature, not body text;
    # compile source/dependencies are separately bound by the object receipts.
    from configure import source_matching_ownership
    p=current_splat_inputs(root,version,serial,opts)
    return {'schema':1,'version':version,'serial':serial,'units':list(opts),
            'disassembler_packages':{n:package_version(n) for n in ('splat64','spimdisasm','rabbitizer')},
            'source_ownership_sha256':sha(json.dumps(source_matching_ownership(version)).encode()),
            'files':{n:sha((root/n).read_bytes()) for n in sorted(p)}}

def record_split(root,version,serial,opts):
    write_json(root/split_stamp(version),split_provenance(root,version,serial,opts))

def check_split(root,version,serial,opts):
    stamp=root/split_stamp(version)
    must(stamp.is_file(),'no current Splat receipt for native readonly fills; run configure without --no-split')
    expected=split_provenance(root,version,serial,opts)
    must(json.loads(stamp.read_text())==expected,'stale native readonly fill Splat/source receipt')
    return expected

def generated_link_inputs(version,serial):
    return tuple(Path(p) for p in (f'build/{version}/{serial}.ld',
        f'config/{version}/{serial}.yaml',f'config/{version}/undefined_syms_auto.txt',
        f'config/{version}/undefined_funcs_auto.txt',f'config/{version}/symbol_addrs.txt'))

def strict_selectors(selectors,primary,following):
    for obj in (primary,following):
        matched=selectors.get(obj)
        must(matched is not None and matched.count('.rodata')==1 and
             '.text' in matched and len(set(matched))==len(matched) and
             all(n in ('.text','.data','.rodata','.lit4','.lit8','.sdata','.sbss','.bss') for n in matched),
             f'expected exactly one unambiguous .rodata selector for {obj}')

def unit_inputs(version,unit):
    c=CASES[unit]
    return (Path(f'build/{version}/src/{version}/{unit}.o'),
            Path(f'build/{version}/base/src/{version}/{unit}.o'),
            Path(f'build/{version}/src/{version}/{c.next}.o'))


def intervening_object(version,unit):
    name = INTERVENING_SELECTORS.get(unit)
    return Path(f'build/{version}/src/{version}/{name}.o') if name else None


def reference_dependencies(root,version,serial,opts):
    from configure import AS
    binary=Path(f'build/{version}/{serial}')
    deps={str(AS), str(binary), *(str(binary)+suffix for suffix in
          ('.elf','.map','.elf.proof.json','.ok')),str(split_stamp(version)),
          *(str(p) for p in generated_link_inputs(version,serial)),*RUNTIME_INPUTS,
          *current_splat_inputs(root,version,serial,opts)}
    for unit in opts:
        obj,base,nxt=unit_inputs(version,unit)
        deps.update((str(obj),str(base),str(nxt),str(obj)+'.proof.json',
                     str(base)+'.proof.json',str(full_asm(version,unit))))
        if (middle := intervening_object(version,unit)) is not None:
            deps.update((str(middle), str(middle)+'.proof.json'))
    return deps


ASM_FILE_DIRECTIVE = re.compile(r'\.inc(?:lude|bin)\b', re.IGNORECASE)
ASM_MACRO_INCLUDE = '.include "macro.inc"'
ASM_MACRO_PATH = 'include/macro.inc'


def validate_asm_text(text):
    # Deliberately accept only Splat's exact single macro include. Treat
    # directive-looking strings/comments as suspicious rather than parsing GAS.
    lines = [line.strip() for line in text.splitlines() if ASM_FILE_DIRECTIVE.search(line)]
    must(lines == [ASM_MACRO_INCLUDE], 'unsupported native readonly assembler file directive')


def validate_asm_inputs(root,version,opts,read_text):
    shadow = root / 'macro.inc'
    must(not (shadow.exists() or shadow.is_symlink()), 'shadowed native readonly macro.inc')
    must(not ASM_FILE_DIRECTIVE.search(read_text(ASM_MACRO_PATH)),
         'recursive native readonly assembler file directive')
    for unit in opts:
        validate_asm_text(read_text(full_asm(version,unit)))


def completion_marker(version):
    return Path(f'build/{version}/native-rodata-fill.complete')


def write_reference_batch(n,root,version,serial,selectors,opts):
    if not opts:
        return
    from configure import AS
    validate_asm_inputs(root,version,opts,lambda path:(root/path).read_text())
    for unit in opts:
        obj,_,nxt=unit_inputs(version,unit)
        strict_selectors(selectors,obj,nxt)
        if (middle := intervening_object(version,unit)) is not None:
            strict_selectors(selectors,obj,middle)
        n.build(str(stock_obj(version,unit)),"as",str(full_asm(version,unit)),
                implicit=[ASM_MACRO_PATH,str(AS)])
    n.build([str(target_obj(version,u)) for u in opts],'native_rodata_fill',
            [str(stock_obj(version,u)) for u in opts],
            implicit=sorted(reference_dependencies(root,version,serial,opts)),
            implicit_outputs=[str(completion_marker(version))],
            variables={'version':version})


class InputPins:
    """Command-local SHA bindings; no reusable proof or stat shortcut."""
    def __init__(self,root):
        self.root=root
        self.digests={}

    def name(self,path):
        path=Path(path)
        if path.is_absolute():
            path=path.relative_to(self.root)
        must('..' not in path.parts and path.parts,'invalid native readonly input path')
        return path.as_posix()

    def bind(self,path,expected):
        name=self.name(path)
        must(self.digests.setdefault(name,expected)==expected,
             'conflicting native readonly input digest: '+name)

    def read(self,path):
        name=self.name(path)
        data=(self.root/name).read_bytes()
        self.bind(name,sha(data))
        return data

    def text(self,path):
        # Same universal newline + locale decoding as Path.read_text().
        with io.TextIOWrapper(io.BytesIO(self.read(path))) as f:
            return f.read()

    def recheck(self):
        for name,expected in sorted(self.digests.items()):
            must(sha((self.root/name).read_bytes())==expected,
                 'changed native readonly batch input: '+name)


def prepare_live(root,version,opts,pins):
    from configure import VERSIONS
    spec=VERSIONS[version]; serial=spec['serial']
    pins.read(split_stamp(version))
    split=check_split(root,version,serial,opts)
    must(json.loads(pins.text(split_stamp(version)))==split,'changed split receipt')
    for name,expected in split['files'].items():
        pins.bind(name,expected)
    conf,plan,override_paths=yaml_inputs(root,version,serial,read_text=pins.text)
    ld,yml,undef,udfn,symbols=generated_link_inputs(version,serial)
    must(conf==root/yml,'link YAML identity')
    image=Path(f'build/{version}/{serial}')
    lfile,mfile=Path(str(image)+'.elf'),Path(str(image)+'.map')
    pfile=Path(str(lfile)+'.proof.json')
    pins.read(pfile)
    receipt=read_receipt(root,lfile,mfile,pfile)
    for name,expected in receipt['files'].items():
        pins.bind(name,expected)
    must(json.loads(pins.text(pfile))==receipt,'changed link receipt')
    for p in ld,yml,undef,udfn,symbols:
        must(str(p) in receipt['files'],'missing exact linker/YAML/undef/symbol receipt pins')
    spans = [(case.final + len(case.value.encode()) + 1 + BASE, case.end + BASE)
             for key, case in CASES.items() if key in opts]
    verify_live_no_incoming(root,receipt,spans)
    retail=pins.read(f'orig/{version}/{serial}')
    live_image=pins.read(image)
    must(len(retail)==spec['elf_size'] and hashlib.sha1(retail).hexdigest()==spec['elf_sha1']==RETAIL_SHA1
         and live_image==retail and (root/(str(image)+'.ok')).is_file(),'no exact linked retail image')
    linked=pins.read(lfile)
    verify_linked_image(linked,live_image)
    r=elf(retail,'ET_EXEC')
    l=elf(linked,'ET_EXEC')
    _,rs=ro(r); li,ls=section(l,'.main')
    real_map=pins.read(mfile).decode()
    maps=live_input_sections(real_map)
    _,mark,body=real_map.partition('Linker script and memory map')
    must(bool(mark),'malformed map')
    ld_text=pins.text(ld)
    return (retail,r,l,rs,li,ls,plan,ld_text,parse_object_sections(ld_text),
            maps,body,pins.text(symbols),[pins.text(p) for p in override_paths],
            set(receipt['objects']),receipt['files'])


def verify_native_prefix(c,normal,skip,stock,native_prefix,linked_span):
    nsec,bsec=normal[2],skip[2]
    must(nsec.data()==bsec.data() and
         readonly_projection(*normal,c)==readonly_projection(*skip,c)==native_prefix and
         readonly_projection(*stock,c,reference=True)==linked_span and
         nsec['sh_addralign']==bsec['sh_addralign'],'stock/SKIP/native prefix mismatch')


def validate_unit(root,version,unit,live,pins,stock):
    retail,r,l,rs,li,ls,plan,ld_text,selectors,maps,body,symbols,overrides,objects,files=live
    c=CASES[unit]
    obj,skip,nxt=unit_inputs(version,unit)
    strict_selectors(selectors,obj,nxt)
    must({str(obj),str(nxt)}<=objects,'missing live owner/neighbor')
    if (middle := intervening_object(version,unit)) is not None:
        strict_selectors(selectors,obj,middle)
        prove_intervening(pins,middle,objects,files,body)
    production,neighbor=pins.read(obj),pins.read(nxt)
    skip_proof=str(skip)+'.proof.json'
    pins.read(skip_proof)
    base_receipt=read_object_receipt(root,str(skip))
    must(json.loads(pins.text(skip_proof))==base_receipt,'changed SKIP receipt')
    for name,expected in base_receipt['files'].items():
        pins.bind(name,expected)
    base=pins.read(skip)
    startv,endv=c.begin+BASE,c.end+BASE
    must(rs['sh_addr']<=startv<endv<=rs['sh_addr']+rs['sh_size'] and
         ls['sh_addr']<=startv<endv<=ls['sh_addr']+ls['sh_size'],'mapped native readonly span')
    at,rb=mapped(r,startv,endv)
    must(at==c.begin and rb==mapped(l,startv,endv)[1],'native/linked rodata span')
    normal,skip_obj,next_obj,original=map(elf,(production,base,neighbor,stock))
    nr,nsec=ro(normal); br,bsec=ro(skip_obj); rr,rsec=ro(original); _,following=ro(next_obj)
    have=c.begin+nsec['sh_size']
    must(c.final+len(c.value)+1==have<c.end and retail[c.final:have]==c.value.encode()+b'\0','actual literal extent')
    verify_native_prefix(c,(normal,nr,nsec),(skip_obj,br,bsec),(original,rr,rsec),
                         retail[c.begin:have],rb)
    must(retail[have:c.end]==bytes(c.end-have),'nonzero actual fill')
    actual_alignment(c,have,following['sh_addralign'])
    must(maps.get((str(obj),'.rodata'))==[(startv,nsec['sh_size'])] and
         maps.get((str(nxt),'.rodata'))==[(endv,following['sh_size'])],'ambiguous live rodata ownership')
    if c.unit==MODE_SELECT:
        must(maps.get((str(obj),'.text'))==[MODE_TEXT],'wrong mapped ModeSelect text extent')
    if (layout := LOCAL_TEXT_READONLY.get(c.unit)) is not None:
        must(maps.get((str(obj),'.text'))==[layout.text],'wrong mapped readonly relocation text extent')
    adjacency(c,plan,ld_text,body,have,nsec['sh_size'],following['sh_size'])
    verify_metadata(symbols,overrides,have,c.end)
    inspect_symbols(l,have+BASE,endv,li)
    for e,i in ((normal,nr),(skip_obj,br),(original,rr)):
        allow=(c.label,c.final-c.begin,c.end-c.final) if e is original else None
        inspect_symbols(e,have-c.begin,c.end-c.begin,i,allow)
        local_incoming(e,i,have-c.begin,c.end-c.begin)
    return c,retail,have

def publish(staged,marker):
    # Ninja uses this only as an output-presence sentinel. Leave old valid
    # output bytes untouched until all new outputs verify; once publication
    # starts, any interrupted/incomplete set is missing the marker.
    marker.parent.mkdir(parents=True,exist_ok=True)
    marker.unlink(missing_ok=True)
    try:
        for obj,target in staged:
            obj.replace(target)
        marker.touch(exist_ok=False)
    except BaseException as error:
        for _,target in staged:
            try:
                target.unlink(missing_ok=True)
            except OSError as exc:
                error.add_note(f"could not invalidate {target}: {exc}")
        raise


def run_batch(root,version,outputs):
    from configure import AS,AS_FLAGS,VERSIONS
    must(version in VERSIONS,'invalid native readonly fill version')
    opts=options(root,version)
    must(opts and outputs==[target_obj(version,u) for u in opts],
         'invalid native readonly fill output list')
    serial=VERSIONS[version]['serial']
    pins=InputPins(root)
    for name in sorted(reference_dependencies(root,version,serial,opts)):
        pins.read(name)
    for unit in opts:
        pins.read(stock_obj(version,unit))
    must(json.loads(pins.text("config/versions.json"))==VERSIONS,
         "changed native readonly version config")
    must(json.loads(pins.text(opt_path(version)))==list(opts),'changed native readonly options')
    validate_asm_inputs(root,version,opts,pins.text)
    live=prepare_live(root,version,opts,pins)
    with ExitStack() as stack:
        staged=[]
        for unit,out in zip(opts,outputs):
            before=pins.read(stock_obj(version,unit))
            c,retail,have=validate_unit(root,version,unit,live,pins,before)
            corrected=render(pins.text(full_asm(version,unit)),c,retail,have)
            validate_asm_text(corrected)
            target=root/out
            target.parent.mkdir(parents=True,exist_ok=True)
            tmp=Path(stack.enter_context(tempfile.TemporaryDirectory(
                prefix='native-rodata-fill-',dir=target.parent)))
            a,b=tmp/'reference.s',tmp/'reference.o'
            a.write_text(corrected)
            subprocess.run([str(AS),*shlex.split(AS_FLAGS),'-o',str(b),str(a)],
                           cwd=root,check=True)
            verify_output(before,b.read_bytes(),c,retail,have)
            staged.append((b,target))
        # This build requires stable inputs and a single workspace writer.
        # Re-prove dynamic split ownership/pin set, then rehash the complete
        # command-local dependency set, including validated receipt SHA pins.
        check_split(root,version,serial,opts)
        validate_asm_inputs(root,version,opts,pins.text)
        pins.recheck()
        publish(staged,root/completion_marker(version))


if __name__=='__main__':
    root=Path(__file__).resolve().parent.parent
    try:
        version,*outputs=sys.argv[1:]
        run_batch(root,version,[Path(o) for o in outputs])
    except (OSError,ValueError) as exc:
        sys.exit(str(exc))
