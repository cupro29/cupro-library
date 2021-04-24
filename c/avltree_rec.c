#include<stdlib.h>
#include<stdbool.h>

bool is_null(void *p){return p==NULL;}
int min(const int a, const int b){return a>b?b:a;}
static int max(const int a, const int b){return a>b?a:b;}

typedef struct node{
	int key;
	int rank;
	struct node *left, *right;
} node;
int get_rank(node *x){return is_null(x)?-1:(*x).rank;}
static void update(node *x){
		(*x).rank = max(get_rank((*x).left),get_rank((*x).right)) + 1;
}
static node* new_node(int k){
	node* res = (node*)malloc(sizeof(node));
	(*res).key = k;
	(*res).left = NULL;
	(*res).right = NULL;
	(*res).rank = 0;
	return res;
}
static node* left_rotate(node* x){
	if(is_null(x)) return NULL;
	node *y = (*x).right;
	if(is_null(y)) return x;
	(*x).right = (*y).left;
	(*y).left = x;
	update(x);
	update(y);
	return y;
}
static node* right_rotate(node *y){
	if(is_null(y)) return NULL;
	node *x = (*y).left;
	if(is_null(x)) return y;
	(*y).left = (*x).right;
	(*x).right = y;
	update(y);
	update(x);
	return x;
}
static node* fixup(node *x){
	if(is_null(x)) return NULL;
	int l = get_rank((*x).left), r = get_rank((*x).right);
	if(abs(l - r) <= 1) return x;
	if(l - r == 2){
		if(get_rank((*(*x).left).left) < get_rank((*(*x).left).right)){
			(*x).left = left_rotate((*x).left);
		}
		return right_rotate(x);
	}
	if(l - r == -2){
		if(get_rank((*(*x).right).left) > get_rank((*(*x).right).right)){
			(*x).right = right_rotate((*x).right);
		}
		return left_rotate(x);
	}
	return NULL;
}
node* insert(node *x, int t){
	if(is_null(x)){
		return new_node(t);
	}
	if(t < (*x).key){
		(*x).left = insert((*x).left, t);
	}else{
		(*x).right = insert((*x).right, t);
	}
	update(x);
	return fixup(x);
}
static node* take_min(node *x){
	if(is_null(x)) return NULL;
	if(is_null((*x).left)) return x;
	node *res = take_min((*x).left);
	(*x).left = (*res).right;
	update(x);
	(*res).right = fixup(x);
	return res;
}
node* delete(node *x, int t){
	if(is_null(x)) return NULL;
	if(t == (*x).key){
		node *res;
		if(is_null((*x).right)){
			res = (*x).left;
		}else{
			res = take_min((*x).right);
			(*res).left = (*x).left;
		}
		free(x);
		update(res);
		return fixup(res);
	}else if(t < (*x).key){
		(*x).left = delete((*x).left, t);
	}else{
		(*x).right = delete((*x).right, t);
	}
	upda(x);
	return fixup(x);
}