// Euclides extendido: retorna g = gcd(a, b) y deja x, y con
// a x + b y = g. O(log min(a, b)). Para a, b >= 0.
long long extgcd(long long a, long long b, long long& x,
                 long long& y) {
  if (b == 0) { x = 1; y = 0; return a; }
  long long x1, y1;
  long long g = extgcd(b, a % b, x1, y1);
  x = y1;
  y = x1 - (a / b) * y1;
  return g;
}
// Inverso modular de a modulo m (m no necesita ser primo).
// Retorna -1 si no existe (gcd(a, m) != 1).
long long inv_mod(long long a, long long m) {
  long long x, y;
  long long g = extgcd(((a % m) + m) % m, m, x, y);
  return g == 1 ? ((x % m) + m) % m : -1;
}
// Ecuacion diofantica a x + b y = c. Retorna false si no hay
// solucion. Si hay, (x0, y0) es una y todas las demas son
//   x = x0 + k * (b / g),  y = y0 - k * (a / g),  k entero.
bool diofantica(long long a, long long b, long long c,
                long long& x0, long long& y0, long long& g) {
  g = extgcd(llabs(a), llabs(b), x0, y0);
  if (g == 0) { x0 = y0 = 0; return c == 0; }
  if (c % g != 0) return false;
  x0 *= c / g;
  y0 *= c / g;
  if (a < 0) x0 = -x0;
  if (b < 0) y0 = -y0;
  return true;
}
