#!/bin/sh
set -eu
root=$(cd "$(dirname "$0")/.." && pwd)
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT
repo=$tmp/repo
mkdir -p "$repo/tools" "$repo/config/smt3" "$repo/src/smt3/sdk" \
    "$repo/tools/compilers/ee-gcc2.96/lib/gcc-lib/ee/2.96-ee-001003-1" \
    "$repo/tools/compilers/ee-gcc2.96/ee/bin" \
    "$repo/tools/compilers/ee-gcc2.9-991111-01/lib/gcc-lib/ee/2.9-ee-991111-01" \
    "$repo/tools/compilers/ee-gcc2.9-991111-01/ee/bin"
cp "$root/tools/cc.sh" "$repo/tools/cc.sh"
printf 'sdk/libipu  -funit-flag\n' > "$repo/config/smt3/cflags.txt"
printf 'sdk/libipu ee-gcc2.9-991111-01\n' > "$repo/config/smt3/compilers.txt"
printf 'int sample;\n' > "$repo/src/smt3/sdk/libipu.c"
cat > "$tmp/fake-cc1" <<'SCRIPT'
#!/bin/sh
printf '%s\n' "$0" "$@" > "$CC_LOG"
while [ $# -gt 0 ]; do
    if [ "$1" = -o ]; then : > "$2"; exit 0; fi
    shift
done
exit 1
SCRIPT
cat > "$tmp/fake-as" <<'SCRIPT'
#!/bin/sh
printf '%s\n' "$0" "$@" > "$AS_LOG"
while [ $# -gt 0 ]; do
    if [ "$1" = -o ]; then : > "$2"; exit 0; fi
    shift
done
exit 1
SCRIPT
chmod +x "$tmp/fake-cc1" "$tmp/fake-as"
for id in ee-gcc2.96 ee-gcc2.9-991111-01; do
    case $id in
        ee-gcc2.96)
            ccpath="$repo/tools/compilers/ee-gcc2.96/lib/gcc-lib/ee/2.96-ee-001003-1/cc1"
            aspath="$repo/tools/compilers/ee-gcc2.96/ee/bin/as"
            minor=96
            grepcc='-O2'
            grepas='-G8'
            ;;
        *)
            ccpath="$repo/tools/compilers/ee-gcc2.9-991111-01/lib/gcc-lib/ee/2.9-ee-991111-01/cc1"
            aspath="$repo/tools/compilers/ee-gcc2.9-991111-01/ee/bin/as"
            minor=9
            grepcc='-fno-common'
            grepas='-G0'
            ;;
    esac
    cp "$tmp/fake-cc1" "$ccpath"
    cp "$tmp/fake-as" "$aspath"
    chmod +x "$ccpath" "$aspath"
    case $id in
        ee-gcc2.96) printf 'sdk/libipu ee-gcc2.96\n' > "$repo/config/smt3/compilers.txt" ;;
        *) printf 'sdk/libipu ee-gcc2.9-991111-01\n' > "$repo/config/smt3/compilers.txt" ;;
    esac
    CC_LOG="$tmp/cc.log" AS_LOG="$tmp/as.log" SMT3_ALLOW_ASLR=1 \
        "$repo/tools/cc.sh" "$repo/src/smt3/sdk/libipu.c" -o "$tmp/out.o"
    grep -Fx -- "$ccpath" "$tmp/cc.log" >/dev/null
    grep -Fx -- "-D__GNUC_MINOR__=$minor" "$tmp/cc.log" >/dev/null
    grep -Fx -- "$grepcc" "$tmp/cc.log" >/dev/null
    grep -Fx -- '-funit-flag' "$tmp/cc.log" >/dev/null
    grep -Fx -- "$aspath" "$tmp/as.log" >/dev/null
    grep -Fx -- "$grepas" "$tmp/as.log" >/dev/null
done

# Source overrides must select the canonical unit, not the temporary filename.
printf '   sdk/libipu ee-gcc2.9-991111-01 # selected SDK compiler\n' > "$repo/config/smt3/compilers.txt"
printf 'int override;\n' > "$tmp/override.c"
CC_LOG="$tmp/cc.log" AS_LOG="$tmp/as.log" SMT3_ALLOW_ASLR=1 \
    SMT3_AS_UNIT=src/smt3/sdk/libipu.c \
    "$repo/tools/cc.sh" "$tmp/override.c" -o "$tmp/out.o"
grep -Fx -- '-D__GNUC_MINOR__=9' "$tmp/cc.log" >/dev/null
grep -Fx -- '-funit-flag' "$tmp/cc.log" >/dev/null

# Unconfigured game units retain the default compiler.
CC_LOG="$tmp/cc.log" AS_LOG="$tmp/as.log" SMT3_ALLOW_ASLR=1 \
    SMT3_AS_UNIT=src/smt3/field/fixture.c \
    "$repo/tools/cc.sh" "$tmp/override.c" -o "$tmp/out.o"
grep -Fx -- '-D__GNUC_MINOR__=96' "$tmp/cc.log" >/dev/null

# Pinned decision hooks must reject SDK units before launching a compiler.
cp "$root/tools/cc1probe.sh" "$repo/tools/cc1probe.sh"
if "$repo/tools/cc1probe.sh" "$repo/src/smt3/sdk/libipu.c" --alloc --out "$tmp/probe" 2>"$tmp/probe-error"; then
    echo "2.96 decision hooks accepted a 2.9 unit" >&2
    exit 1
else
    [ "$?" -eq 3 ]
fi
grep -F -- 'cc1probe hooks require ee-gcc2.96' "$tmp/probe-error" >/dev/null
[ ! -e "$tmp/probe" ]

printf 'sdk/libipu unsupported-compiler\n' > "$repo/config/smt3/compilers.txt"
if SMT3_ALLOW_ASLR=1 "$repo/tools/cc.sh" "$repo/src/smt3/sdk/libipu.c" -o "$tmp/out.o" 2>"$tmp/error"; then
    echo "unknown compiler was accepted" >&2
    exit 1
fi
grep -F -- 'unsupported compiler' "$tmp/error" >/dev/null
