// Algoritmo de Mo: consultas offline en O((N + Q) sqrt N).
// Ejemplo: cantidad de valores distintos en [l, r] (cerrado).
// Para otro problema solo cambian add_pos() y remove_pos().
// Los valores de a deben ser >= 0 y chicos; si no, comprimirlos.
struct Query { int l, r, id; };

vector<int> mo(const vector<int>& a, vector<Query> qs) {
  if (qs.empty()) return {};
  int n = a.size();
  int B = max(1, (int)(n / sqrt((double)qs.size())));
  sort(qs.begin(), qs.end(),
       [&](const Query& x, const Query& y) {
    int bx = x.l / B, by = y.l / B;
    if (bx != by) return bx < by;
    return bx % 2 == 0 ? x.r < y.r : x.r > y.r;
  });
  vector<int> cnt(*max_element(a.begin(), a.end()) + 1, 0);
  vector<int> res(qs.size());
  int distinct = 0, L = 0, R = -1;
  auto add_pos = [&](int i) { if (cnt[a[i]]++ == 0) distinct++; };
  auto remove_pos = [&](int i) { if (--cnt[a[i]] == 0) distinct--; };
  for (const Query& q : qs) {
    while (L > q.l) add_pos(--L);
    while (R < q.r) add_pos(++R);
    while (L < q.l) remove_pos(L++);
    while (R > q.r) remove_pos(R--);
    res[q.id] = distinct;
  }
  return res;
}
