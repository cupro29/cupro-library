#include<stdlib.h>
#include<stdint.h>

void mergesort(int arr[], int sz){
	if(sz == 1) return;
	int lsz = sz/2, rsz = (sz+1)/2;
	mergesort(arr, lsz);
	mergesort(arr+lsz, rsz);
	int *larr = (int*)malloc(sizeof(int) * (lsz+1));
	int *rarr = (int*)malloc(sizeof(int) * (rsz+1));
	for(int i=0; i<lsz; i++) larr[i] = arr[i];
	for(int i=0; i<rsz; i++) rarr[i] = arr[lsz+i];
	larr[lsz] = INT32_MAX;
	rarr[rsz] = INT32_MAX;
	int lidx = 0, ridx = 0;
	for(int i=0; i<sz; i++){
		if(larr[lidx] <= rarr[ridx]){
			arr[i] = larr[lidx++];
		}else{
			arr[i] = rarr[ridx++];
		}
	}
	free(larr);
	free(rarr);
}