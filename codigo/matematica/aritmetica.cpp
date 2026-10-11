// Aritmetica modular de 64 bits (usa __int128 de GCC).
// Requiere 0 <= a, b < m en mulmod. powmod en O(log e).
// Inverso con m primo: powmod(a, m - 2, m).
long long mulmod(long long a, long long b, long long m) {
  return (long long)((__int128)a * b % m);
}
long long powmod(long long b, long long e, long long m) {
  long long r = 1 % m;
  b %= m;
  while (e > 0) {
    if (e & 1) r = mulmod(r, b, m);
    b = mulmod(b, b, m);
    e >>= 1;
  }
  return r;
}
