void swap(int*x,int*y){
	int t=*x;
	*x=*y;
	*y=t;
}
void push(int*arr,int k){
	arr[++*arr]=k;
	for(int i=*arr;i>1&&arr[i]<arr[i/2];i/=2){
		swap(arr+i,arr+i/2);
	}
}
int pop(int*arr){
	int res=arr[1];
	arr[1]=arr[(*arr)--];
	for(int i=1;;){
		if(i*2<=*arr&&arr[i]>arr[i*2]&&(i*2==*arr||arr[i*2]<=arr[i*2+1])){
			swap(arr+i,arr+i*2);
			i=i*2;
		}else if(i*2+1<=*arr&&arr[i]>arr[i*2+1]&&arr[i*2+1]<arr[i*2]){
			swap(arr+i,arr+i*2+1);
			i=i*2+1;
		}else break;
	}
	return res;
}