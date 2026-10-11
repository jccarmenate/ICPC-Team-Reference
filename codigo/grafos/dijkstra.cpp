// Dijkstra con cola de prioridad. O((n + m) log n).
// adj[u] = lista de (vecino, peso >= 0). LLONG_MAX = inalcanzable.
vector<long long> dijkstra(
    const vector<vector<pair<int, long long>>>& adj, int s) {
  vector<long long> dist(adj.size(), LLONG_MAX);
  priority_queue<pair<long long, int>,
                 vector<pair<long long, int>>,
                 greater<pair<long long, int>>> pq;
  dist[s] = 0;
  pq.push({0, s});
  while (!pq.empty()) {
    auto [d, u] = pq.top();
    pq.pop();
    if (d > dist[u]) continue;
    for (auto [v, w] : adj[u])
      if (d + w < dist[v]) {
        dist[v] = d + w;
        pq.push({dist[v], v});
      }
  }
  return dist;
}
