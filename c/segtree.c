#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>

int ceil_pow2(int n){
	int res = 0;
	while(1U<<res < (unsigned int)n)res++;
	return res;
}

typedef int32_t Int;
inline Int max(Int a, Int b){return a>b?a:b;}
Int segtree_op(Int a, Int b){return max(a, b);}
const Int segtree_e = INT32_MIN;

Int* segtree(int n){
	int lg = ceil_pow2(n);
	Int* res = (Int*)malloc(sizeof(Int) * (1<<lg) * 2);
	for(int i=0; i<=(1<<lg)*2; i++)res[i] = segtree_e;
	return res;
}
void update(Int arr[], int k){arr[k] = segtree_op(arr[k*2], arr[2*k+1]);}
void set(Int arr[], int n, int p, Int x){
	int lg = ceil_pow2(n);
	p += 1<<lg;
	arr[p] = x;
	for(int i=1; i<=lg; i++)update(arr, p>>i);
}
Int get(Int arr[], int n, int p){
	return arr[p +(1<<ceil_pow2(n))];
}
Int prod(Int arr[], int n, int l, int r){
	Int sml = segtree_e, smr = segtree_e;
	int lg = ceil_pow2(n);
	l += 1<<lg;
	r += 1<<lg;
	while(l < r){
		if(l & 1) sml = segtree_op(sml, arr[l++]);
		if(r & 1) smr = segtree_op(arr[--r], smr);
		l >>= 1;
		r >>= 1;
	}
	return segtree_op(sml, smr);
}
Int all_prod(Int arr[]){return arr[1];}
int max_right(Int arr[], int n, int l, bool (*f)(Int)){
	if(l == n) return n;
	int lg = ceil_pow2(n);
	int size = 1<<lg;
	l += size;
	Int sm = segtree_e;
	do{
		while(l % 2 == 0) l >>= 1;
		if(!f(segtree_op(sm, arr[l]))){
			while(l < size){
				l = 2*l;
				if(f(segtree_op(sm, arr[l]))){
					sm = segtree_op(sm, arr[l]);
					l++;
				}
			}
			return l - size;
		}
		sm = segtree_op(sm, arr[l]);
		l++;
	}while((l & -l) != l);
	return n;
}
int min_right(Int arr[], int n, int r, bool (*f)(Int)){
	if(r == 0) return 0;
	int lg = ceil_pow2(n);
	int size = 1<<lg;
	r += size;
	Int sm = segtree_e;
	do{
		r--;
		while(r>1 && r&1) r >>= 1;
		if(!f(segtree_op(arr[r], sm))){
			while(r < size){
				r = 2*r + 1;
				if(f(segtree_op(arr[r], sm))){
					sm = segtree_op(arr[r], sm);
					r--;
				}
			}
			return r + 1 - size;
		}
		sm = segtree_op(arr[r], sm);
	}while((r & -r) != r);
	return 0;
}