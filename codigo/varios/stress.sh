# Stress test: sol = solucion, brute = fuerza bruta,
# gen = generador que recibe la semilla como argumento.
for ((i = 1; ; i++)); do
  ./gen $i > in.txt
  ./sol < in.txt > out1.txt
  ./brute < in.txt > out2.txt
  if ! cmp -s out1.txt out2.txt; then
    echo "Failed on seed $i (see in.txt)"
    break
  fi
  echo "ok $i"
done
