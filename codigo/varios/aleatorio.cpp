// Numeros aleatorios de buena calidad (no usar rand()).
mt19937_64 rng(
    chrono::steady_clock::now().time_since_epoch().count());
long long aleatorio(long long l, long long r) {  // en [l, r]
  return uniform_int_distribution<long long>(l, r)(rng);
}
// shuffle(v.begin(), v.end(), rng);
