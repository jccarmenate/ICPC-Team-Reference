// Algoritmo de Mo: consultas offline en O((N + Q) sqrt N).
// Ejemplo: cantidad de valores distintos en [l, r] (cerrado).
// Para otro problema solo cambian agregar() y quitar().
// Los valores de a deben ser >= 0 y chicos; si no, comprimirlos.
struct Consulta { int l, r, id; };

vector<int> mo(const vector<int>& a, vector<Consulta> qs) {
  if (qs.empty()) return {};
  int n = a.size();
  int B = max(1, (int)(n / sqrt((double)qs.size())));
  sort(qs.begin(), qs.end(),
       [&](const Consulta& x, const Consulta& y) {
    int bx = x.l / B, by = y.l / B;
    if (bx != by) return bx < by;
    return bx % 2 == 0 ? x.r < y.r : x.r > y.r;
  });
  vector<int> cnt(*max_element(a.begin(), a.end()) + 1, 0);
  vector<int> res(qs.size());
  int distintos = 0, L = 0, R = -1;
  auto agregar = [&](int i) { if (cnt[a[i]]++ == 0) distintos++; };
  auto quitar = [&](int i) { if (--cnt[a[i]] == 0) distintos--; };
  for (const Consulta& q : qs) {
    while (L > q.l) agregar(--L);
    while (R < q.r) agregar(++R);
    while (L < q.l) quitar(L++);
    while (R > q.r) quitar(R--);
    res[q.id] = distintos;
  }
  return res;
}
