#include <vector>
template <class T>
struct BinaryIndexedTree {
  BinaryIndexedTree(std::size_t n) : sz(n), d(sz) {}
  void add(std::size_t p, T x) {
    p++;
    while (p <= sz) {
      d[p - 1] += x;
      p += p & -p;
    }
  }
  T range(std::size_t l, std::size_t r) const { return sum(r) - sum(l); }
  T sum(std::size_t r) const {
    T res = 0;
    while (r > 0) {
      res += d[r - 1];
      r -= r & -r;
    }
    return res;
  }

 private:
  std::size_t sz;
  std::vector<T> d;
};
