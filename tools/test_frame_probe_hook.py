#!/usr/bin/env python3
"""Exercise the actual player probe wrapper and frame hook with a host C compiler.

This checks configuration/evaluation contracts and old-versus-new host float
operations, not PS2 floating-point equivalence or player-function correctness.
Run from any directory; CC may contain a compiler plus wrapper arguments.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shlex
import subprocess
import tempfile

FIXTURE = r'''#include <stdint.h>
#include <stdio.h>
#include <string.h>
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
typedef uint8_t u8; typedef uint16_t u16;
typedef struct {float x,y,z;} Pos;
static float w[3],r[3]; static Pos pos;
static unsigned wc,rc,pc,overrides; static int alias,poison;
static float *work(void){++wc;return poison?0:w;}
static float *retail(void){++rc;return poison?0:(alias?w:r);}
static Pos *position(void){++pc;return poison?0:&pos;}
#define OLD(work,retail,position) do { (work)[0]+=(position).x-(retail)[0]; (work)[1]+=(position).y-(retail)[1]; (work)[2]+=(position).z-(retail)[2]; }while(0)
#if MODE==1 || MODE==3
#define SDF_FRAME_STEP(x) ((x)*0.5f)
#endif
#if MODE==2 || MODE==3
#define SDF_FRAME_TOWARD(s,x,h) ((h)?(x):(s)+((x)-(s))*0.5f)
#endif
#if MODE==4
#define SDF_FRAME_PROBE_FOLLOW(w,r,p) do{++overrides;}while(0)
#elif MODE==5
#define SDF_FRAME_PROBE_FOLLOW(w,r,p) do{++overrides;OLD(w,r,p);}while(0)
#endif
@HEADER@
#define fldHitChkWork work()
@WRAPPER@
static const uint32_t bits[]={0,0x80000000,0x3f800000,0xbf800000,0x41280000,0x00000001,0x7f800000,0xff800000,0x7fc12345,0xffc56789};
static float from(uint32_t b){float f;memcpy(&f,&b,4);return f;}
static void setup(unsigned a,unsigned b,unsigned c){unsigned i;for(i=0;i<3;i++){w[i]=from(bits[(a+i)%10]);r[i]=from(bits[(b+i)%10]);}pos.x=from(bits[c]);pos.y=from(bits[(c+1)%10]);pos.z=from(bits[(c+2)%10]);wc=rc=pc=overrides=0;}
int main(void){unsigned a,b,c,cases=0;float expected[3],original[3];
 for(alias=0;alias<2;alias++)for(a=0;a<10;a++)for(b=0;b<10;b++)for(c=0;c<10;c++){
  setup(a,b,c);memcpy(original,w,12);OLD(work(),retail(),(*position()));memcpy(expected,w,12);assert(wc==3&&rc==3&&pc==3);
  setup(a,b,c);FLD_HIT_WORK_FOLLOW(retail(),(*position()));
#if MODE==0 || MODE==4
  assert(wc==0&&rc==0&&pc==0);assert(!memcmp(w,original,12));
#else
  assert(wc==3&&rc==3&&pc==3);assert(!memcmp(w,expected,12));
#endif
#if MODE>=4
  assert(overrides==1);
#else
  assert(overrides==0);
#endif
  cases++;
 }
#if MODE==0 || MODE==4
 poison=1;wc=rc=pc=0;FLD_HIT_WORK_FOLLOW(retail(),(*position()));assert(wc==0&&rc==0&&pc==0);
#endif
 printf("mode=%d cases=%u PASS\n",MODE,cases);return 0;}
'''


def extract_wrapper(source):
    lines = source.splitlines(keepends=True)
    starts = [i for i, line in enumerate(lines)
              if re.match(r"^\s*#\s*define\s+FLD_HIT_WORK_FOLLOW\(", line)]
    if len(starts) != 1:
        raise ValueError("expected exactly one FLD_HIT_WORK_FOLLOW definition")
    i = starts[0]
    result = []
    while True:
        line = lines[i]
        result.append(line)
        if not line.rstrip("\r\n").endswith("\\"):
            return "".join(result)
        i += 1
        if i >= len(lines):
            raise ValueError("unterminated FLD_HIT_WORK_FOLLOW definition")


def check(header, player, compiler):
    wrapper = extract_wrapper(player.read_text())
    fixture = FIXTURE.replace("@HEADER@", "#include " + json.dumps(str(header.resolve())))
    fixture = fixture.replace("@WRAPPER@", wrapper)
    with tempfile.TemporaryDirectory(prefix="frame-probe-test-") as directory:
        root = Path(directory)
        source = root / "probe.c"
        source.write_text(fixture)
        for optimization in (0, 2):
            for mode in range(6):
                binary = root / ("probe-%s-%s" % (mode, optimization))
                command = compiler + ["-std=c99", "-O%s" % optimization,
                    "-fno-fast-math", "-ffp-contract=off", "-DMODE=%s" % mode,
                    str(source), "-o", str(binary)]
                build = subprocess.run(command, capture_output=True, text=True, timeout=30)
                if build.returncode:
                    raise RuntimeError("host compile failed: %s\n%s" % (command, build.stdout + build.stderr))
                run = subprocess.run([str(binary)], capture_output=True, text=True, timeout=10)
                if run.returncode:
                    raise RuntimeError("mode %s / O%s failed (%s):\n%s" %
                        (mode, optimization, run.returncode, run.stdout + run.stderr))
                print(run.stdout.strip())


def main():
    repo = Path(__file__).resolve().parent.parent
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--header", type=Path, default=repo / "include/sdfFrame.h")
    parser.add_argument("--player", type=Path, default=repo / "src/smt3/field/fldPlayer.c")
    args = parser.parse_args()
    compiler = shlex.split(os.environ.get("CC", "cc"))
    if not compiler:
        parser.error("CC must name a host C compiler")
    check(args.header, args.player, compiler)
    print("PASS: frame probe hook and actual player wrapper (12 configurations)")


if __name__ == "__main__":
    main()
