// Set con estadisticas de orden (GCC). O(log n).
// s.find_by_order(k): iterador al k-esimo menor (desde 0).
// s.order_of_key(x): cantidad de elementos menores que x.
// Para multiset: guardar pair<valor, id unico>.
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
template <class T>
using ordered_set = __gnu_pbds::tree<
    T, __gnu_pbds::null_type, less<T>, __gnu_pbds::rb_tree_tag,
    __gnu_pbds::tree_order_statistics_node_update>;
