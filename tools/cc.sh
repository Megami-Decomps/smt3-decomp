#!/bin/sh
# Compile one C file with the build's per-unit toolchain and flags,
# for the permuter and quick experiments:
#   tools/cc.sh [extra cc1 flags] in.c -o out.o
# SMT3_VERSION selects the INCLUDE_ASM tree (default smt3); SMT3_I386_LIBDIR as
# for configure.py when the system has no 32-bit loader.
set -e
root=$(cd "$(dirname "$0")/.." && pwd)
in= out= flags=
while [ $# -gt 0 ]; do
    case $1 in
        -o) out=$2; shift 2 ;;
        *.c) in=$1; shift ;;
        *) flags="$flags $1"; shift ;;
    esac
done
[ -n "$in" ] && [ -n "$out" ] || { echo "usage: $0 [flags] in.c -o out.o" >&2; exit 2; }
case $in in /*) ;; *) in="$PWD/$in" ;; esac
case $out in /*) ;; *) out="$PWD/$out" ;; esac
version=${SMT3_VERSION:-smt3}
canon=${SMT3_AS_UNIT:-}
case $in in
    "$root"/src/"$version"/*) [ -z "$canon" ] && canon=${in#"$root"/} ;;
esac
compiler=ee-gcc2.96
if [ -n "$canon" ]; then
    rest=${canon#src/}
    rel=${rest#*/}; rel=${rel%.c}
    configured=
    if [ -f "$root/config/$version/compilers.txt" ]; then
        configured=$(awk -v unit="$rel" '$1 == unit { compiler = $2 } END { print compiler }' "$root/config/$version/compilers.txt")
    fi
    [ -z "$configured" ] || compiler=$configured
fi
case $compiler in
    ee-gcc2.96)
        ee="$root/tools/compilers/ee-gcc2.96"
        cc1="$ee/lib/gcc-lib/ee/2.96-ee-001003-1/cc1"
        minor=96
        ccflags='-quiet -O2'
        asflags='-EL -G8 -g -Iinclude'
        ;;
    ee-gcc2.9-991111-01)
        ee="$root/tools/compilers/ee-gcc2.9-991111-01"
        cc1="$ee/lib/gcc-lib/ee/2.9-ee-991111-01/cc1"
        minor=9
        ccflags='-quiet -O2 -G 0 -fno-common'
        asflags='-EL -G0 -Iinclude'
        ;;
    *) echo "unsupported compiler for $canon: $compiler" >&2; exit 2 ;;
esac
# Pinned 32-bit glibc (tools/download_tools.py) unless overridden.
[ -z "$SMT3_I386_LIBDIR" ] && [ -f "$root/tools/glibc32/libc.so.6" ] && SMT3_I386_LIBDIR="$root/tools/glibc32"
# Match configure.py's shared ASLR decision, which keys off the patched 2.96
# compiler marker even when this unit selects the older SDK compiler.
aslr=
patched_sha=d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1
if ! grep -qx "$patched_sha" "$root/tools/compilers/ee-gcc2.96/lib/gcc-lib/ee/2.96-ee-001003-1/cc1.aslr-fixed" 2>/dev/null; then
    [ -z "$SMT3_ALLOW_ASLR" ] && command -v setarch >/dev/null && aslr="setarch $(uname -m) -R"
fi
run() {
    if [ -n "$SMT3_I386_LIBDIR" ]; then
        $aslr "$SMT3_I386_LIBDIR/ld-linux.so.2" --library-path "$SMT3_I386_LIBDIR" "$@"
    else
        $aslr "$@"
    fi
}
tmp=$(mktemp -d)
cleanup() { rm -rf "$tmp" ${srcdir:+"$root/$srcdir"} ${outdir:+"$root/$outdir"}; }
trap cleanup EXIT
cd "$root"
# ee-gcc 2.96's code can depend on the lengths of the file names cc1 is given
# (they shift its heap, and CSE hashes heap addresses). Keep canonical scratch
# names for both compilers; SMT3_AS_UNIT treats an experiment as that unit.
cc_in=$in
cc_out=$tmp/out.s
if [ -n "$canon" ]; then
    while :; do srcdir=.$(od -An -N1 -tx1 /dev/urandom | tr -d ' \n'); mkdir "$srcdir" 2>/dev/null && break; done
    while :; do outdir=.$(od -An -N2 -tx1 /dev/urandom | tr -d ' \n'); mkdir "$outdir" 2>/dev/null && break; done
    mkdir -p "$srcdir/$(dirname "$rest")" "$outdir/$version/src/$(dirname "$rest")"
    cp "$in" "$srcdir/$rest"
    cc_in=$srcdir/$rest
    cc_out=$outdir/$version/src/${rest%.c}.o.s
    # Per-unit flags of the original build (config/<v>/cflags.txt), as configure.py uses.
    unit_flags=$(sed -n "s|^$rel[[:space:]]\{1,\}\([^#]*\).*|\1|p" "config/$version/cflags.txt" 2>/dev/null)
    flags="$flags $unit_flags"
fi
run "$cc1" \
    -D__GNUC__=2 "-D__GNUC_MINOR__=$minor" -D__GNUC_PATCHLEVEL__=0 \
    -Dmips -DMIPSEL -DR5900 -D_mips -D_MIPSEL -D_R5900 -D__ee__ \
    -D__mips__ -D__MIPSEL__ -D__R5900__ -D__mips -D__MIPSEL -D__R5900 -D__OPTIMIZE__ \
    -D__LANGUAGE_C -D_LANGUAGE_C -DLANGUAGE_C \
    "-D__SIZE_TYPE__=unsigned int" "-D__PTRDIFF_TYPE__=int" -D__LONG_MAX__=9223372036854775807L \
    -U__mips -D__mips=3 -D__mips64 -D__mips_eabi -D__mips_single_float \
    -Iinclude -Isrc "-DASM_ROOT=\"build/eeasm/asm/$version/nonmatchings/\"" "-DVERSION_$(echo $version | tr a-z A-Z)" \
    $ccflags $flags "$cc_in" -o "$cc_out"
# SMT3_KEEP_S=path keeps the compiler's assembly (check_unit measures inline asm).
if [ -n "$SMT3_KEEP_S" ]; then cp "$cc_out" "$SMT3_KEEP_S"; fi
run "$ee/ee/bin/as" $asflags -o "$out" "$cc_out"
