#!/bin/bash
# Compara el notebook contra KACTL (mismos resultados con entradas
# aleatorias y tiempos con tamanos de competencia) y prueba la
# profundidad de recursion con una pila de 8 MB.
# Uso: bash tests/kactl/run.sh [carpeta de KACTL]
# Sin carpeta, clona KACTL en un directorio temporal.
cd "$(dirname "$0")"
K=${1:-${TMPDIR:-/tmp}/kactl}
if [ ! -d "$K/content" ]; then
  git clone --depth 1 -q https://github.com/kth-competitive-programming/kactl.git "$K" || exit 1
fi
C="$K/content"
P=$(mktemp -d)
# Parches minimos: variantes que la propia KACTL indica en sus comentarios.
sed -e 's/typedef int T;/typedef long long T;/' -e 's/unit = INT_MIN/unit = 0/' \
    -e 's/return max(a, b);/return a + b;/' "$C/data-structures/SegmentTree.h" > "$P/SegmentTreeSum.h"
sed -e "s/first = 'A'/first = 'a'/" "$C/strings/AhoCorasick.h" > "$P/AhoCorasickLower.h"
{
  echo '#pragma once'
  echo 'const ll mod = (119 << 23) + 1, root = 62;'
  echo 'll modpow(ll b, ll e) { ll ans = 1; for (; e; b = b * b % mod, e /= 2) if (e & 1) ans = ans * b % mod; return ans; }'
  sed -e '/#include/d' -e '/^const ll mod = (119/d' "$C/numerical/NumberTheoreticTransform.h"
} > "$P/NumberTheoreticTransformPatched.h"
case "$(uname -s)" in
  MINGW* | MSYS* | CYGWIN*) BIG="-Wl,--stack,268435456"; SMALL="-Wl,--stack,8388608"; LIMIT="" ;;
  *) BIG=""; SMALL=""; ulimit -s unlimited 2> /dev/null; LIMIT="ulimit -s 8192;" ;;
esac
fails=0
for t in structures graphs math strings geometry geometry2; do
  echo "== compare_$t"
  if g++ -std=c++17 -O2 -static $BIG -I"$C" -I"$P" -o "$P/c_$t.exe" compare_$t.cpp && "$P/c_$t.exe"; then
    :
  else
    echo "FAIL compare_$t"
    fails=1
  fi
done
echo "== profundidad de recursion con pila de 8 MB (cadena de n vertices)"
g++ -std=c++17 -O2 -static $SMALL -I"$C" -I"$P" -o "$P/depth.exe" stack_depth.cpp || fails=1
for which in hld centroid biconnected scc kactl_lca kactl_bcc kactl_scc; do
  line="  $which:"
  for n in 100000 200000 500000 1000000; do
    if bash -c "$LIMIT '$P/depth.exe' $which $n" > /dev/null 2>&1; then line="$line ok($n)"
    else line="$line CAE($n)"; fi
  done
  echo "$line"
done
rm -rf "$P"
exit $fails
