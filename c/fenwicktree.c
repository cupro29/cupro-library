#include<stdlib.h>
#include<assert.h>

long* fentree(int n){
	long *res = (long*)calloc(n, sizeof(long));
	assert(res != NULL);
	return res;
}
void fen_add(long *arr, int n, int p, int x){
	p++;
	while(p <= n){
		arr[p-1] += x;
		p += p & -p;
	}
}
long fen_sum(long *arr, int r){
	long res = 0;
	while(r > 0){
		res += arr[r-1];
		r -= r & -r;
	}
	return res;
}
long fen_range(long *arr, int l, int r){
	return fen_sum(arr, r) - fen_sum(arr, l);
}