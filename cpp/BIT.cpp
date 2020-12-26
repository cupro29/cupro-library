#include<vector>
template <class T>
struct BIT {
	BIT(int n): sz(n), d(sz) {}
	void add(int p, T x){
		p++;
		while(p<=sz){
			d[p-1]+=x;
			p+=p&-p;
		}
	}
	T range(int l, int r){
		return sum(r)-sum(l);
	}
	T sum(int r){
		T res=0;
		while(r>0){
			res+=d[r-1];
			r-=r&-r;
		}
		return res;
	}
private:
	int sz;
	vector<T> d;
};