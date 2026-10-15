// Busqueda binaria. El arreglo debe estar ORDENADO de menor a
// mayor. Indices en [0, n). Cada busqueda es O(log n).
//
// lower_bound(x): primer indice i con a[i] >= x (n si no hay).
//   Es donde se insertaria x ANTES de todos sus iguales.
// upper_bound(x): primer indice i con a[i] > x (n si no hay).
//   Es donde se insertaria x DESPUES de todos sus iguales.
//
// Ejemplo con a = {1, 3, 3, 3, 7} (n = 5):
//   lower_bound(3) = 1    upper_bound(3) = 4   (los 3 son [1, 4))
//   lower_bound(4) = 4    upper_bound(4) = 4   (4 no esta)
//   lower_bound(0) = 0    upper_bound(9) = 5 = n
//
// Lo que se obtiene de ellas (lb = lower_bound, ub = upper_bound):
//   cantidad de veces que aparece x  = ub(x) - lb(x)
//   x aparece                       <=> lb(x) < n && a[lb(x)] == x
//   cantidad de elementos < x        = lb(x)
//   cantidad de elementos <= x       = ub(x)
//   cantidad de elementos en [l, r]  = ub(r) - lb(l)
//   ultimo indice con a[i] < x       = lb(x) - 1   (-1 si no hay)
//   ultimo indice con a[i] <= x      = ub(x) - 1   (-1 si no hay)

// Invariante: todo indice < lo es "chico" (a[i] < x) y todo
// indice >= hi es "grande" (a[i] >= x). Al terminar lo == hi.
int lower_bound_manual(const vector<long long>& a, long long x) {
  int lo = 0, hi = a.size();
  while (lo < hi) {
    int mid = lo + (hi - lo) / 2;
    if (a[mid] >= x) hi = mid;  // mid cumple: la respuesta es <= mid
    else lo = mid + 1;          // mid no cumple: descartarlo
  }
  return lo;
}
// Igual, pero la condicion es estricta: a[mid] > x.
int upper_bound_manual(const vector<long long>& a, long long x) {
  int lo = 0, hi = a.size();
  while (lo < hi) {
    int mid = lo + (hi - lo) / 2;
    if (a[mid] > x) hi = mid;
    else lo = mid + 1;
  }
  return lo;
}

// Con la STL (devuelven iteradores; restar begin() da el indice):
//   int lb = lower_bound(a.begin(), a.end(), x) - a.begin();
//   int ub = upper_bound(a.begin(), a.end(), x) - a.begin();
//   En set/map usar s.lower_bound(x): O(log n). En cambio
//   lower_bound(s.begin(), s.end(), x) sobre un set es O(n).
//   Arreglo en orden decreciente: pasar greater<long long>() como
//   cuarto argumento (lb = primer a[i] <= x, ub = primer a[i] < x).

// Busqueda binaria sobre la respuesta. ok(m) debe ser monotono en
// [lo, hi]: falso, ..., falso, verdadero, ..., verdadero.
// Retorna el MENOR m con ok(m), o hi + 1 si ninguno cumple.
// lower_bound es el caso ok(i) = a[i] >= x; upper_bound, a[i] > x.
// Para el MAYOR m con ok(m) cuando el patron es verdadero...falso:
//   primer_verdadero(lo, hi, [&](long long m) { return !ok(m); }) - 1
template <class F>
long long primer_verdadero(long long lo, long long hi, F ok) {
  hi++;  // hi pasa a significar "ninguno cumple"
  while (lo < hi) {
    long long mid = lo + (hi - lo) / 2;
    if (ok(mid)) hi = mid;
    else lo = mid + 1;
  }
  return lo;
}

// Version en reales: menor x en [lo, hi] con ok(x) (ok monotono).
// Iterar un numero fijo de veces evita problemas con EPS:
// 100 iteraciones achican el intervalo en un factor de 2^100.
template <class F>
double biseccion(double lo, double hi, F ok) {
  for (int it = 0; it < 100; it++) {
    double mid = (lo + hi) / 2;
    if (ok(mid)) hi = mid;
    else lo = mid;
  }
  return hi;
}
