#include<stdlib.h>
#include<stdbool.h>

void bsort(int arr[], int sz){
	while(true){
		bool is_sorted = true;
		for(int i=1; i<sz; i++) is_sorted &= (arr[i-1] <= arr[i]);
		if(is_sorted) return;
		for(int i=0; i<sz; i++){
			int j = rand() % sz;
			int tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
		}
	}
}