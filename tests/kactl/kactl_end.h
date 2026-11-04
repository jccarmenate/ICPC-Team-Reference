// Quita las macros de KACTL (el notebook usa "sz" como variable) y
// agrega utilidades para comparar tiempos.
#undef rep
#undef all
#undef sz
#include "../comun.h"

struct Timing {
  string name;
  double ours, kactl;
};
vector<Timing> timings;

// Segundos que tarda f().
template <class F>
double measure(F f) {
  auto t0 = chrono::steady_clock::now();
  f();
  return chrono::duration<double>(chrono::steady_clock::now() - t0).count();
}
void report(const string& name, double ours, double kactl) {
  timings.push_back({name, ours, kactl});
}
// Imprime la tabla de tiempos. Marca LENTO si el notebook tarda mas
// de 0.2 s y mas del triple que KACTL.
void print_timings() {
  printf("  %-44s %9s %9s\n", "tiempo (tamano de competencia)", "notebook", "KACTL");
  for (auto& t : timings) {
    bool slow = t.ours > 0.2 && t.ours > 3 * t.kactl;
    printf("  %-44s %8.3fs %8.3fs%s\n", t.name.c_str(), t.ours, t.kactl,
           slow ? "  <-- LENTO" : "");
  }
}
