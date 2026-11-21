"""Validate and partition two native mixed-owner report references, never link inputs."""
from dataclasses import dataclass
import hashlib
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

from .linker_inputs import linker_object_sections
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

CASES={
    'pb/pbDebugMode': Case('pb/pbDebugMode','model/mdlManager',0x437638,0x43B648,
                         0x43B630,'pbFileMiscColorbarTB','misc/colorbar.TB',8),
    'event/evtCommand': Case('event/evtCommand','event/evtMoon',0x43B8A8,0x43C060,
                         0x43C040,'D_0053B040','/fld/f/bin/moon_07.tmx',16),
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
    must(lds.count(pri)==lds.count(foll)==1 and lds.index(foll)==lds.index(pri)+1,'exact linker selectors')
    startv,endv,gap=start+BASE,c.end+BASE,c.end-start
    pat=(rf'^ {re.escape(a)}\(\.rodata\)\n(?: \*fill\* +0x[0-9a-f]+ +0x[0-9a-f]+ 00000000\n)? \.rodata +0x0*{c.begin+BASE:x} +0x0*{psize:x} {re.escape(a)}\n'
         rf'(?: +0x[0-9a-f]+ +{IDENT}\n)*'
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
def render(stock,c,ret,have):
    """Preserve the full native listing outside the verified terminal label body."""
    lab=re.escape(c.label)
    m=list(re.finditer(rf'(?ms)^dlabel {lab}\n(.*?)^enddlabel {lab}\n',stock))
    must(len(m)==1,'missing/ambiguous terminal label')
    m=m[0]
    must(len(re.findall(r'^\.section \.rodata, "a"$',stock,re.M))==1 and
         stock[:m.start()].rfind('\n.section ')==stock[:m.start()].rfind('\n.section .rodata, "a"') and
         stock[m.end():].lstrip().startswith('.section .lit4, "wa"\n'),'not terminal native rodata label')
    lines=m[1].splitlines()
    primary=f'    /* {c.final:06X} {c.final+BASE:08X} */ .asciz "{c.value}"'
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
    must(ar.data()==br.data()+fill.data() and br.data()==ret[c.begin:have] and
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
            must(rs.name not in out and src!='.rodata','unexpected target rodata relocation section')
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

def write_reference_edge(n,root,version,serial,unit,selectors,opts):
    c=CASES[unit]
    obj=Path(f'build/{version}/src/{version}/{unit}.o')
    base=Path(f'build/{version}/base/src/{version}/{unit}.o')
    nxt=Path(f'build/{version}/src/{version}/{c.next}.o')
    strict_selectors(selectors,obj,nxt)
    binary=Path(f'build/{version}/{serial}')
    reference=full_asm(version,unit)
    backup=stock_obj(version,unit)
    target=target_obj(version,unit)
    n.build(str(backup),"as",str(reference),implicit=["include/macro.inc"])
    deps={str(obj),str(base),str(nxt),str(obj)+'.proof.json',str(base)+'.proof.json',
          str(binary),str(binary)+'.elf',str(binary)+'.map',str(binary)+'.elf.proof.json',
          str(binary)+'.ok',str(split_stamp(version)),str(reference),
          *(str(p) for p in generated_link_inputs(version,serial)),*RUNTIME_INPUTS,
          *current_splat_inputs(root,version,serial,opts)}
    n.build(str(target),'native_rodata_fill',str(backup),implicit=sorted(deps),
            variables={'version':version,'unit':unit})

def validate_live(root,version,unit,production,base,neighbor,stock):
    from configure import VERSIONS
    c=CASES[unit]
    spec=VERSIONS[version]; serial=spec['serial']
    opts=options(root,version)
    must(unit in opts,'missing explicit native readonly fill opt-in')
    check_split(root,version,serial,opts)
    conf,plan,override_paths=yaml_inputs(root,version,serial)
    ld,yml,undef,udfn,symbols=generated_link_inputs(version,serial)
    must(conf==root/yml,'link YAML identity')
    image=Path(f'build/{version}/{serial}')
    # Keep the serial spelling intact; it already contains a suffix (SLUS_209.11).
    lfile,mfile=Path(str(image)+'.elf'),Path(str(image)+'.map')
    pfile=Path(str(lfile)+'.proof.json')
    obj=Path(f'build/{version}/src/{version}/{unit}.o')
    nxt=Path(f'build/{version}/src/{version}/{c.next}.o')
    strict_selectors(linker_object_sections(root/ld),obj,nxt)
    receipt=read_receipt(root,lfile,mfile,pfile)
    required=(ld,yml,undef,udfn,symbols)
    entries=receipt['files']
    must({str(p) for p in required}<=entries.keys(),'missing exact linker/YAML/undef/symbol receipt pins')
    must({str(obj),str(nxt)}<=set(receipt['objects']),'missing live owner/neighbor')
    # These terminal bytes are linker placement fill, not C or native objects.
    # Prove both configured gaps against the complete current link inventory.
    spans = [(case.final + len(case.value.encode()) + 1 + BASE, case.end + BASE)
             for key, case in CASES.items() if key in opts]
    verify_live_no_incoming(root, receipt, spans)
    for filename,payload in ((obj,production),(nxt,neighbor)):
        must(sha(payload)==entries[str(filename)],'changed physical link input '+str(filename))
    skip=Path(f'build/{version}/base/src/{version}/{unit}.o')
    base_receipt=read_object_receipt(root,str(skip))
    must(sha(base)==base_receipt['files'][str(skip)],'changed SKIP object')
    retail=(root/f'orig/{version}/{serial}').read_bytes()
    live_image=(root/image).read_bytes()
    must(len(retail)==spec['elf_size'] and hashlib.sha1(retail).hexdigest()==spec['elf_sha1']==RETAIL_SHA1
         and live_image==retail and (root/(str(image)+'.ok')).is_file(),'no exact linked retail image')
    linked=(root/lfile).read_bytes()
    must(sha(linked)==entries[str(lfile)],'changed physical linked image')
    verify_linked_image(linked,live_image)
    r=elf(retail,'ET_EXEC')
    l=elf(linked,'ET_EXEC')
    _,rs=ro(r); li,ls=section(l,'.main')
    startv,endv=c.begin+BASE,c.end+BASE
    must(rs['sh_addr']<=startv<endv<=rs['sh_addr']+rs['sh_size'] and
         ls['sh_addr']<=startv<endv<=ls['sh_addr']+ls['sh_size'],'mapped native readonly span')
    at,rb=mapped(r,startv,endv)
    must(at==c.begin and rb==mapped(l,startv,endv)[1],'native/linked rodata span')
    normal,skip_obj,next_obj,original=map(elf,(production,base,neighbor,stock))
    nr,nsec=ro(normal); br,bsec=ro(skip_obj); rr,rsec=ro(original); _,following=ro(next_obj)
    have=c.begin+nsec['sh_size']
    must(c.final+len(c.value)+1==have<c.end and retail[c.final:have]==c.value.encode()+b'\0','actual literal extent')
    must(nsec.data()==bsec.data()==retail[c.begin:have] and rsec.data()==rb and
         nsec['sh_addralign']==bsec['sh_addralign'],'stock/SKIP/native prefix mismatch')
    must(retail[have:c.end]==bytes(c.end-have),'nonzero actual fill')
    actual_alignment(c,have,following['sh_addralign'])
    real_map=(root/mfile).read_bytes()
    must(sha(real_map)==entries[str(mfile)],'changed physical link map')
    maps=live_input_sections(real_map.decode())
    must(maps.get((str(obj),'.rodata'))==[(startv,nsec['sh_size'])] and
         maps.get((str(nxt),'.rodata'))==[(endv,following['sh_size'])],'ambiguous live rodata ownership')
    _,mark,body=real_map.decode().partition('Linker script and memory map')
    must(bool(mark),'malformed map')
    adjacency(c,plan,(root/ld).read_text(),body,have,nsec['sh_size'],following['sh_size'])
    verify_metadata((root/symbols).read_text(),[p.read_text() for p in override_paths],have,c.end)
    inspect_symbols(l,have+BASE,endv,li)
    for e,i in ((normal,nr),(skip_obj,br),(original,rr)):
        allow=(c.label,c.final-c.begin,c.end-c.final) if e is original else None
        inspect_symbols(e,have-c.begin,c.end-c.begin,i,allow)
        must(not any(isinstance(s,RelocationSection) and s['sh_info']==i and s.num_relocations()
                     for s in e.iter_sections()),'owner .rodata has outgoing relocations')
        local_incoming(e,i,have-c.begin,c.end-c.begin)
    return c,retail,have

def run_target(root,version,unit,inp,out):
    from configure import AS,AS_FLAGS
    must(unit in options(root,version) and inp==stock_obj(version,unit) and
         out==target_obj(version,unit),'invalid native readonly fill target paths')
    c=CASES[unit]
    prod=lambda n:(root/f'build/{version}/src/{version}/{n}.o').read_bytes()
    before=(root/inp).read_bytes()
    c,retail,have=validate_live(root,version,unit,prod(unit),
        (root/f'build/{version}/base/src/{version}/{unit}.o').read_bytes(),prod(c.next),before)
    asm=(root/full_asm(version,unit)).read_text()
    corrected=render(asm,c,retail,have)
    target=root/out
    target.parent.mkdir(parents=True,exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='native-rodata-fill-',dir=target.parent) as dirname:
        tmp=Path(dirname)
        a,b=tmp/'reference.s',tmp/'reference.o'
        a.write_text(corrected)
        subprocess.run([str(AS),*shlex.split(AS_FLAGS),'-o',str(b),str(a)],cwd=root,check=True)
        verify_output(before,b.read_bytes(),c,retail,have)
        b.replace(target)

if __name__=='__main__':
    root=Path(__file__).resolve().parent.parent
    try:
        version,unit,inp,out=sys.argv[1:]
        run_target(root,version,unit,Path(inp),Path(out))
    except (OSError,ValueError) as exc:
        sys.exit(str(exc))
