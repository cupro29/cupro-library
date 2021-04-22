#include<stdlib.h>
#include<assert.h>
typedef struct node{
	int key,size;
	long sum;
	struct node*left,*right;
}node;
int get_size(node*x){return x?(*x).size:0;}
long get_sum(node*x){return x?(*x).sum:0;}
static void update(node*x){
	(*x).size=get_size((*x).left)+get_size((*x).right)+1;
	(*x).sum=get_sum((*x).left)+get_sum((*x).right)+(*x).key;
}
static node*left_rotate(node*x){
	assert(x);
	node*y=(*x).right;
	if(y==NULL)return x;
	(*x).right=(*y).left;
	(*y).left=x;
	update(x);
	update(y);
	return y;
}
static node*right_rotate(node*y){
	assert(y);
	node*x=(*y).left;
	if(x==NULL)return y;
	(*y).left=(*x).right;
	(*x).right=y;
	update(y);
	update(x);
	return x;
}
static node*balance(node*x){
	if(x==NULL)return NULL;
	update(x);
	int l=get_size((*x).left),r=get_size((*x).right);
	int ll=0,lr=0,rl=0,rr=0;
	if((*x).left){
		ll=get_size((*(*x).left).left);
		lr=get_size((*(*x).left).right);
	}
	if((*x).right){
		rl=get_size((*(*x).right).left);
		rr=get_size((*(*x).right).right);
	}
	if(l<rl){
		(*x).right=right_rotate((*x).right);
		node*y=left_rotate(x);
		(*y).left=balance((*y).left);
		(*y).right=balance((*y).right);
		return balance(y);
	}else if(l<rr){
		node*y=left_rotate(x);
		(*y).left=balance((*y).left);
		return balance(y);
	}else if(r<lr){
		(*x).left=left_rotate((*x).left);
		node*y=right_rotate(x);
		(*y).left=balance((*y).left);
		(*y).right=balance((*y).right);
		return balance(y);
	}else if(r<ll){
		node*y=right_rotate(x);
		(*y).right=balance((*y).right);
		return balance(y);
	}else{
		return x;
	}
	return NULL;
}
node*insert(node*x,int k){
	if(x==NULL){
		node*z=(node*)malloc(sizeof(node));
		(*z).key=k;
		(*z).size=1;
		(*z).sum=k;
		(*z).left=NULL;
		(*z).right=NULL;
		return z;
	}
	if((*x).key>k){
		(*x).left=insert((*x).left,k);
	}else{
		(*x).right=insert((*x).right,k);
	}
	return balance(x);
}
static node*take_min(node*x){
	assert(x);
	if((*x).left==NULL)return x;
	node*res=take_min((*x).left);
	(*x).left=(*res).right;
	(*res).right=balance(x);
	return res;
}
node*delete(node*x,int k){
	if(x==NULL)return NULL;
	if(k==(*x).key){
		node*res;
		if((*x).right==NULL){
			res=(*x).left;
		}else{
			res=take_min((*x).right);
			(*res).left=(*x).left;
		}
		free(x);
		return balance(res);
	}else if(k<(*x).key){
		(*x).left=delete((*x).left,k);
	}else{
		(*x).right=delete((*x).right,k);
	}
	return balance(x);
}