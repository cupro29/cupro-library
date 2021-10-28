#include <numeric>
#include <utility>
#include <vector>
template <class T>
struct WeightedUnionFind {
  WeightedUnionFind(int n) : _n(n) {
    parent = std::vector<int>(n);
    std::iota(parent.begin(), parent.end(), 0);
    rank = std::vector<int>(n);
    diff_weight = std::vector<T>(n);
  }
  int root(int x) {
    int p = parent[x];
    if (p == x) {
      return x;
    } else {
      int r = root(p);
      T d = diff_weight[p];
      diff_weight[x] += d;
      parent[x] = r;
      return r;
    }
  }
  bool same(int x, int y) { return root(x) == root(y); }
  bool merge(int x, int y, T w) {
    w += weight(x);
    w -= weight(y);
    x = root(x);
    y = root(y);
    if (x == y) return false;
    if (rank[x] < rank[y]) {
      std::swap(x, y);
      w *= -1;
    }
    if (rank[x] == rank[y]) {
      rank[x] += 1;
    }
    parent[y] = x;
    diff_weight[y] = w;
    return true;
  }
  T diff(int x, int y) { return weight(y) - weight(x); }

 private:
  int _n;
  std::vector<int> parent;
  std::vector<int> rank;
  std::vector<T> diff_weight;
  T weight(int x) {
    root(x);
    return diff_weight[x];
  }
};