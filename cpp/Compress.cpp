#include <algorithm>
#include <vector>

template <class T>
class Compress {
  std::vector<T> d;

 public:
  Compress(const std::vector<T>& v) : d(v) {
    std::sort(d.begin(), d.end());
    d.erase(std::unique(d.begin(), d.end()), d.end());
  }
  int operator[](T& e) const {
    return (int)(std::lower_bound(d.begin(), d.end(), e) - d.begin());
  }
  int size() const { return d.size(); }
  T value(int i) const { return d[i]; }
};