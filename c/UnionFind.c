//Union-Find-Tree

#include<stdlib.h>
#include<stdbool.h>

int* UF(int N){
	int* res = (int*)malloc(sizeof(int) * N);
	for(int i=0; i<N; i++) res[i] = -1;
	return res;
}
int find(int *ary, int x){
	if(ary[x] < 0) return x;
	return ary[x] = find(ary, ary[x]);
}
bool unite(int *ary, int x, int y){
	x = find(ary, x); y = find(ary, y);
	if(x == y) return false;
	if(ary[x] > ary[y]){
		int tmp = x;
		x = y;
		y = tmp;
	}
	ary[x] += ary[y];
	ary[y] = x;
	return true;
}
bool same(int *ary, int x, int y){return find(ary, x) == find(ary, y);}
int size(int *ary, int x){return -ary[find(ary, x)];}
int main(){}
