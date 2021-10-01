#include <vector>
template <class T>
struct SegmentTree {
  SegmentTree(int n, T e, T (*f)(T, T))
      : _n(n), sz(1 << ceil_pow2(n)), d(2 * sz, id), op(f), id(e) {}
  void set(int p, T x) {
    p += sz;
    d[p] = x;
    while (p >> 1 > 0) update(p >>= 1);
  }
  T get(int p) { return d[p + sz]; }
  T prod(int l, int r) {
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
  T all_prod() { return d[1]; }

 private:
  int _n;
  int sz;
  std::vector<T> d;
  T (*op)(T, T);
  T id;
  int ceil_pow2(int k) {
    int res = 0;
    while (1 << res < k) res++;
    return res;
  }
  void update(int k) { d[k] = op(d[k * 2], d[k * 2 + 1]); }
};