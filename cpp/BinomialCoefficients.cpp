#include <vector>

template <class Mint>
class BinomialCoefficients {
  int N;
  std::vector<Mint> fac, ifac;

 public:
  BinomialCoefficients(int n = 0) : N(0), fac(1, Mint(1)), ifac(1, Mint(1)) {
    resize(n);
  }
  Mint calc(int n, int k) const {
    if (n < k || n < 0 || n < k || n > N) return Mint(0);
    return fac[n] * ifac[k] * ifac[n - k];
  }
  void resize(int n) {
    if (n <= N) return;
    fac.resize(n + 1);
    ifac.resize(n + 1);
    for (int i = N + 1; i <= n; i++) fac[i] = fac[i - 1] * Mint(i);
    ifac.back() = fac.back().inv();
    for (int i = n - 1; i > N; i--) ifac[i] = ifac[i + 1] * Mint(i + 1);
    N = n;
  }
};