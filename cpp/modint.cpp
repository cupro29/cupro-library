#include<cstdint>
template <std::uint_fast64_t mod>
class Modint {
	using u64 = std::uint_fast64_t;
	u64 v;
public:
	constexpr Modint(const u64 x = 0): v(x % mod) {}
	constexpr u64 val() { return v; }
	constexpr Modint operator+(const Modint rhs) {
		return Modint(*this) += rhs;
	}
	constexpr Modint operator-(const Modint rhs) {
		return Modint(*this) -= rhs;
	}
	constexpr Modint operator*(const Modint rhs) {
		return Modint(*this) *= rhs;
	}
	constexpr Modint operator/(const Modint rhs) {
		return Modint(*this) /= rhs;
	}
	constexpr Modint &operator+=(const Modint rhs) {
		v += rhs.v;
		if(v >= mod) {
			v -= mod;
		}
		return *this;
	}
	constexpr Modint &operator-=(const Modint rhs) {
		if(v < rhs.v) {
			v += mod;
		}
		v -= rhs.v;
		return *this;
	}
	constexpr Modint &operator*=(const Modint rhs) {
		v = v * rhs.v % mod;
		return *this;
	}
	constexpr Modint &operator /=(Modint rhs) {
		u64 exp = mod - 2;
		while(exp) {
			if(exp & 1) {
				*this *= rhs;
			}
			rhs *= rhs;
			exp >>= 1;
		}
		return *this;
	}
	Modint pow(u64 p) {
		u64 x = v;
		u64 res = 1;
		while(p){
			if(p&1) {
				res = res * x % mod;
			}
			x = x * x % mod;
			p >>= 1;
		}
		return Modint<mod>(res);
	}
};
