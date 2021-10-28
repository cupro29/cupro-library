#include <functional>
#include <vector>
template <class T>
struct DualSegmentTree {
  DualSegmentTree(int n, T e, std::function<T(T, T)> f)
      : _n(n), sz(1 << lg), lg(ceil_pow2(n)), d(2 * sz, e), op(f), id(e) {}
  void set(int index, T x) {
    int p = index + sz;
    for (int i = lg; i > 0; --i) {
      push(p >> i);
    }
    d[p] = x;
  }
  T get(int index) {
    int p = index + sz;
    for (int i = lg; i > 0; --i) {
      push(p >> i);
    }
    return d[p];
  }
  void apply(int index, T fn) {
    int p = index + sz;
    for (int i = lg; i > 0; --i) {
      push(p >> i);
    }
    d[p] = op(fn, d[p]);
  }
  void apply(int left, int right, T fn) {
    if (left == right) return;
    int l = left + sz, r = right + sz;
    for (int i = lg; i > 0; --i) {
      if (((l >> i) << i) != l) {
        push(l >> i);
      }
      if (((r >> i) << i) != r) {
        push((r - 1) >> i);
      }
    }
    while (l < r) {
      if ((l & 1) != 0) {
        all_apply(l, fn);
        ++l;
      }
      if ((r & 1) != 0) {
        --r;
        all_apply(r, fn);
      }
      l >>= 1;
      r >>= 1;
    }
  }

 private:
  int _n;
  int sz;
  int lg;
  std::vector<T> d;
  std::function<T(T, T)> op;
  T id;
  int ceil_pow2(int k) const {
    int res = 0;
    while (1 << res < k) res++;
    return res;
  }
  void all_apply(int k, T fn) { d[k] = op(fn, d[k]); }
  void push(int k) {
    if (d[k] != id) {
      all_apply(k << 1 | 0, d[k]);
      all_apply(k << 1 | 1, d[k]);
      d[k] = id;
    }
  }
};