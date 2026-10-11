// Hashing polinomial modulo 2^61 - 1 con base aleatoria
// (resistente a anti-hash). Construccion O(n), consulta O(1).
// get(l, r): hash de s[l..r), comparable entre strings distintos.
struct Hashing {
  static const long long MOD = (1LL << 61) - 1;
  static long long mul(long long a, long long b) {
    return (long long)((__int128)a * b % MOD);
  }
  static long long base() {
    static mt19937_64 gen(
        chrono::steady_clock::now().time_since_epoch().count());
    static long long b = 256 + gen() % (MOD - 1000);
    return b;
  }
  vector<long long> h, pot;
  Hashing(const string& s) : h(s.size() + 1, 0),
                             pot(s.size() + 1, 1) {
    for (int i = 0; i < (int)s.size(); i++) {
      h[i + 1] = (mul(h[i], base()) + (unsigned char)s[i]) % MOD;
      pot[i + 1] = mul(pot[i], base());
    }
  }
  long long get(int l, int r) {  // [l, r)
    return (h[r] - mul(h[l], pot[r - l]) + MOD) % MOD;
  }
};
