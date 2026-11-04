// Prueba de profundidad de recursion: arma una cadena de n vertices
// (el peor caso para los DFS recursivos) y corre la estructura pedida.
// Si la pila no alcanza, el programa se cae.
// Uso: stack_depth <estructura> <n>
#include "kactl_begin.h"
namespace klca {
#include "graph/LCA.h"
}
namespace kbcc {
#include "graph/BiconnectedComponents.h"
}
namespace kscc {
#include "graph/SCC.h"
}
#include "kactl_end.h"
#include "../../codigo/estructuras/segtree_lazy.cpp"
#include "../../codigo/estructuras/hld.cpp"
#include "../../codigo/estructuras/centroid.cpp"
#include "../../codigo/grafos/biconexas.cpp"
#include "../../codigo/grafos/scc.cpp"

int main(int argc, char** argv) {
  if (argc < 3) return 2;
  string which = argv[1];
  int n = atoi(argv[2]);
  long long check = 0;
  if (which == "hld") {
    HLD h(n);
    for (int i = 1; i < n; i++) h.add_edge(i - 1, i);
    h.build(0);
    check = h.dist(0, n - 1);
  } else if (which == "centroid") {
    Centroid c(n);
    for (int i = 1; i < n; i++) c.add_edge(i - 1, i);
    c.build();
    check = c.level[0];
  } else if (which == "biconnected") {
    Biconnected b(n);
    for (int i = 1; i < n; i++) b.add_edge(i - 1, i);
    b.run();
    check = b.comps.size();
  } else if (which == "scc") {
    SCC s(n);
    for (int i = 1; i < n; i++) s.add_edge(i - 1, i);
    check = s.run();
  } else if (which == "kactl_lca") {
    vector<vi> g(n);
    for (int i = 1; i < n; i++) g[i - 1].push_back(i), g[i].push_back(i - 1);
    klca::LCA l(g);
    check = l.lca(0, n - 1);
  } else if (which == "kactl_bcc") {
    kbcc::ed.assign(n, {});
    for (int i = 1; i < n; i++) kbcc::ed[i - 1].emplace_back(i, i - 1), kbcc::ed[i].emplace_back(i - 1, i - 1);
    int cnt = 0;
    kbcc::bicomps([&](const vi&) { cnt++; });
    check = cnt;
  } else if (which == "kactl_scc") {
    vector<vi> g(n);
    for (int i = 1; i < n; i++) g[i - 1].push_back(i);
    kscc::scc(g, [&](vi&) {});
    check = kscc::ncomps;
  } else {
    return 2;
  }
  printf("%lld\n", check);
}
