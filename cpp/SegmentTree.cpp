#include <functional>
#include <vector>
template <class T>
struct SegmentTree {
  SegmentTree(int n, T e, std::function<T(T, T)> f)
      : _n(n), sz(1 << ceil_pow2(n)), d(2 * sz, e), op(f), id(e) {}
  void set(int p, T x) {
    p += sz;
    d[p] = x;
    while (p >> 1 > 0) update(p >>= 1);
  }
  void add(int p, T x) { set(p, op(get(p), x)); }
  T get(int p) const { return d[p + sz]; }
  T prod(int l, int r) const {
    T sml = id, smr = id;
    l += sz;
    r += sz;
    while (l < r) {
      if (l & 1) sml = op(sml, d[l++]);
      if (r & 1) smr = op(d[--r], smr);
      l >>= 1;
      r >>= 1;
    }
    return op(sml, smr);
  }
  T all_prod() const { return d[1]; }

 private:
  int _n;
  int sz;
  std::vector<T> d;
  std::function<T(T, T)> op;
  T id;
  int ceil_pow2(int k) const {
    int res = 0;
    while (1 << res < k) res++;
    return res;
  }
  void update(int k) { d[k] = op(d[k * 2], d[k * 2 + 1]); }
};