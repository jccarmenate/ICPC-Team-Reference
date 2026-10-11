// Union-Find con compresion de caminos y union por tamano.
// Casi O(1) amortizado por operacion.
struct DSU {
  vector<int> padre, tam;
  int comps;
  DSU(int n) : padre(n), tam(n, 1), comps(n) {
    iota(padre.begin(), padre.end(), 0);
  }
  int find(int x) {
    return padre[x] == x ? x : padre[x] = find(padre[x]);
  }
  bool unir(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b) return false;
    if (tam[a] < tam[b]) swap(a, b);
    padre[b] = a;
    tam[a] += tam[b];
    comps--;
    return true;
  }
};
