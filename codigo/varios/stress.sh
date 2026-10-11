# Stress test: sol = solucion, bruta = fuerza bruta,
# gen = generador que recibe la semilla como argumento.
for ((i = 1; ; i++)); do
  ./gen $i > in.txt
  ./sol < in.txt > out1.txt
  ./bruta < in.txt > out2.txt
  if ! cmp -s out1.txt out2.txt; then
    echo "Falla con semilla $i (ver in.txt)"
    break
  fi
  echo "ok $i"
done
