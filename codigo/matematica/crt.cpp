// Teorema chino del resto generalizado (modulos no coprimos).
// Requiere extgcd. Une x = a1 (mod m1) con x = a2 (mod m2).
// Retorna (x, lcm(m1, m2)) con 0 <= x < lcm, o (-1, -1) si no
// hay solucion. El lcm debe caber en long long.
// Para varias congruencias: plegar empezando con (0, 1).
pair<long long, long long> crt(long long a1, long long m1,
                               long long a2, long long m2) {
  long long p, q;
  long long g = extgcd(m1, m2, p, q);
  if ((a2 - a1) % g != 0) return {-1, -1};
  long long l = m1 / g * m2, mg = m2 / g;
  __int128 dif = (a2 - a1) / g % mg;
  long long k = (long long)(dif * p % mg);
  long long x = (long long)((a1 + (__int128)k * m1) % l);
  return {(x + l) % l, l};
}
