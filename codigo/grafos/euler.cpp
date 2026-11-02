// Camino / circuito euleriano (Hierholzer iterativo). O(n + m).
// Recorre cada ARISTA exactamente una vez. Admite aristas
// multiples y lazos, dirigido o no dirigido.
// camino(): secuencia de m + 1 vertices, o vacio si no existe.
//   Si el primero y el ultimo coinciden es un circuito (se puede
//   rotar para empezar en cualquier vertice).
// Condiciones de existencia (todas las aristas en una componente):
//   No dirigido: 0 vertices de grado impar (circuito) o 2 (camino
//     que va de uno al otro).
//   Dirigido: todos con entrada == salida (circuito), o uno con
//     salida - entrada = 1 (inicio) y otro con entrada - salida = 1
//     (final) y el resto iguales.
struct Euler {
  int n, m = 0;
  bool dirigido;
  vector<vector<pair<int, int>>> adj;  // (vecino, id de arista)
  vector<int> sale, entra;
  Euler(int n_, bool dirigido_) : n(n_), dirigido(dirigido_),
      adj(n_), sale(n_, 0), entra(n_, 0) {}
  void add_edge(int u, int v) {
    adj[u].push_back({v, m});
    if (!dirigido) adj[v].push_back({u, m});
    sale[u]++;
    entra[v]++;
    m++;
  }
  vector<int> camino() {
    int inicio = -1, impares = 0, extremos = 0;
    for (int u = 0; u < n; u++) {
      if (dirigido) {
        int d = sale[u] - entra[u];
        if (d == 1) { inicio = u; extremos++; }
        else if (d == -1) extremos++;
        else if (d != 0) return {};
      } else if ((sale[u] + entra[u]) % 2 == 1) {
        impares++;
        if (inicio == -1) inicio = u;
      }
    }
    if (dirigido && extremos != 0 && !(extremos == 2 && inicio != -1))
      return {};
    if (!dirigido && impares != 0 && impares != 2) return {};
    if (inicio == -1)  // circuito: cualquier vertice con aristas
      for (int u = 0; u < n && inicio == -1; u++)
        if (!adj[u].empty()) inicio = u;
    if (inicio == -1) return {0};  // grafo sin aristas
    vector<int> it(n, 0), res, pila = {inicio};
    vector<bool> usada(m, false);
    while (!pila.empty()) {
      int u = pila.back();
      while (it[u] < (int)adj[u].size() && usada[adj[u][it[u]].second])
        it[u]++;
      if (it[u] == (int)adj[u].size()) {
        res.push_back(u);  // u ya no tiene aristas libres
        pila.pop_back();
      } else {
        auto [v, id] = adj[u][it[u]];
        usada[id] = true;
        pila.push_back(v);
      }
    }
    if ((int)res.size() != m + 1) return {};  // aristas desconectadas
    reverse(res.begin(), res.end());
    return res;
  }
};
