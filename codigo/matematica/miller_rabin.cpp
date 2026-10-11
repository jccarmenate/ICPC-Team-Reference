// Miller-Rabin determinista para todo n < 2^63.
// Requiere mulmod y powmod. O(12 log n) multiplicaciones.
bool es_primo(long long n) {
  if (n < 2) return false;
  for (long long p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37})
    if (n % p == 0) return n == p;
  long long d = n - 1;
  int s = 0;
  while (d % 2 == 0) {
    d /= 2;
    s++;
  }
  for (long long a : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
    long long x = powmod(a, d, n);
    if (x == 1 || x == n - 1) continue;
    bool compuesto = true;
    for (int r = 1; r < s && compuesto; r++) {
      x = mulmod(x, x, n);
      if (x == n - 1) compuesto = false;
    }
    if (compuesto) return false;
  }
  return true;
}
