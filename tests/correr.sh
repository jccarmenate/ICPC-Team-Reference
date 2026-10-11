#!/bin/bash
# Compila y ejecuta todos los tests contra fuerza bruta.
# Uso: bash tests/correr.sh
cd "$(dirname "$0")"
fallas=0
for t in estructuras grafos matematica strings geometria; do
  if g++ -std=c++17 -O2 -Wall -Wextra -Wshadow -static \
      -o test_$t.exe test_$t.cpp \
      && ./test_$t.exe; then
    :
  else
    echo "FALLO test_$t"
    fallas=1
  fi
done
rm -f test_*.exe
exit $fallas
