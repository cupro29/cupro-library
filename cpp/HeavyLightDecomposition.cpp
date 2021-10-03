#include <functional>
#include <utility>
#include <vector>
struct HeavyLightDecomposition {
  int _n;
  std::vector<int> sz, in, nxt, out, par;
  std::vector<std::vector<int>> g;
  void dfs_sz(int v, int p = -1) {
    sz[v] = 1;
    par[v] = p;
    if (!g[v].empty() && g[v][0] == p) std::swap(g[v][0], g[v].back());
    for (int& u : g[v]) {
      if (u == p) continue;
      dfs_sz(u, v);
      sz[v] += sz[u];
      if (sz[u] > sz[g[v][0]]) std::swap(u, g[v][0]);
    }
  }
  void dfs_hld(int v, int& t, int p = -1) {
    in[v] = t++;
    for (int u : g[v]) {
      if (u == p) continue;
      nxt[u] = (u == g[v][0] ? nxt[v] : u);
      dfs_hld(u, t, v);
    }
    out[v] = t;
  }

 public:
  HeavyLightDecomposition(int n = 0)
      : _n(n), sz(n), in(n), nxt(n), out(n), par(n), g(n) {}
  void add_edge(int u, int v) {
    g[u].push_back(v);
    g[v].push_back(u);
  }
  void build(int root = 0) {
    dfs_sz(root);
    int t = 0;
    dfs_hld(root, t);
  }
  int lca(int u, int v) const {
    while (true) {
      if (in[u] > in[v]) std::swap(u, v);
      if (nxt[u] == nxt[v]) break;
      v = par[nxt[v]];
    }
    return u;
  }
  template <class T>
  T query(int u, int v, T e, std::function<T(int, int)> q,
          std::function<T(T, T)> f) {
    T l = e, r = e;
    while (true) {
      if (in[u] > in[v]) {
        std::swap(u, v);
        std::swap(l, r);
      }
      if (nxt[u] == nxt[v]) break;
      l = f(q(in[nxt[v]], in[v] + 1), l);
      v = par[nxt[v]];
    }
    return f(f(q(in[u], in[v] + 1), l), r);
  }
};