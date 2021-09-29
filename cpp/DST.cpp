#include<vector>
template<class T>
struct DST{
	DST(const std::vector<T> &v, T (*f)(T, T)): _n(v.size()), op(f) {
		table.emplace_back(v);
		for(int i = 2; i < _n; i <<= 1) {
			table.emplace_back(std::vector<T>(_n));
			for(int j = i; j < _n; j += i << 1) {
				table.back()[j - 1] = table[0][j - 1];
				for(int k = 2; k <= i; k++) {
					table.back()[j - k] = op(table[0][j - k], table.back()[j - k + 1]);
				}
				table.back()[j] = table[0][j];
				for(int k = 2; k <= i && j + k <= _n; k++) {
					table.back()[j + k - 1] = op(table.back()[j + k - 2], table[0][j + k - 1]);
				}
			}
		}
	}
	T query(int l, int r) {
		r--;
		if(l == r) return table[0][l];
		int k = 31 - __builtin_clz(l ^ r);
		return op(table[k][l], table[k][r]);
	}
private:
	int _n;
	std::vector<std::vector<T> > table;
	T (*op)(T, T);
};