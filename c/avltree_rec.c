#include<stdlib.h>
#include<stdbool.h>

static int max(const int a, const int b){return a>b?a:b;}

typedef struct node{
	int key;
	int rank;
	int size;
	struct node *left, *right;
} node;
typedef struct {
	node *left, *right;
} node_pair;
int get_rank(node *x){return x == NULL ? -1 : (*x).rank;}
int get_size(node *x){return x == NULL ?  0 : (*x).size;}
static void update(node *x){
		(*x).rank = max(get_rank((*x).left),get_rank((*x).right)) + 1;
		(*x).size = get_size((*x).left) + get_size((*x).right) + 1;
}
static node* new_node(int k){
	node* res = (node*)malloc(sizeof(node));
	(*res).key = k;
	(*res).left = NULL;
	(*res).right = NULL;
	(*res).rank = 0;
	(*res).size = 1;
	return res;
}
static node* left_rotate(node* x){
	if(x == NULL) return NULL;
	node *y = (*x).right;
	if(y == NULL) return x;
	(*x).right = (*y).left;
	(*y).left = x;
	update(x);
	update(y);
	return y;
}
static node* right_rotate(node *y){
	if(y == NULL) return NULL;
	node *x = (*y).left;
	if(x == NULL) return y;
	(*y).left = (*x).right;
	(*x).right = y;
	update(y);
	update(x);
	return x;
}
static node* fixup(node *x){
	if(x == NULL) return NULL;
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
	if(x == NULL){
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
	if(x == NULL) return NULL;
	if((*x).left == NULL) return x;
	node *res = take_min((*x).left);
	(*x).left = (*res).right;
	update(x);
	(*res).right = fixup(x);
	return res;
}
node* delete(node *x, int t){
	if(x == NULL) return NULL;
	if(t == (*x).key){
		node *res;
		if((*x).right == NULL){
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
	update(x);
	return fixup(x);
}
node* push_front(node *x, int t){
	if(x == NULL) return new_node(t);
	(*x).left = push_front((*x).left, t);
	return fixup(x);
}
node* push_back(node *x, int t){
	if(x == NULL) return new_node(t);
	(*x).right = push_back((*x).right, t);
	return fixup(x);
}
node* pop_front(node *x){
	if((*x).left == NULL){
		node *r = (*x).right;
		free(x);
		return r;
	}
	(*x).left = pop_front((*x).left);
	return fixup(x);
}
node* pop_back(node *x){
	if((*x).right == NULL){
		node *l = (*x).left;
		free(x);
		return l;
	}
	(*x).right = pop_back((*x).right);
	return fixup(x);
}
node* merge_with_root(node *l, node *root, node *r){
	int ll = get_rank(l), rr = get_rank(r);
	if(abs(ll - rr) <= 1){
		(*root).left = l;
		(*root).right = r;
		update(root);
		return root;
	}else if(ll - rr > 1){
		(*l).right = merge_with_root((*l).right, root, r);
		return fixup(l);
	}else{
		(*r).left = merge_with_root(l, root, (*r).left);
		return fixup(r);
	}
}
node* merge(node *l, node *r){
	if(l == NULL) return r;
	if(r == NULL) return l;
	node *root = take_min(r);
	r = (*root).right;
	(*root).right = NULL;
	return merge_with_root(l, root, r);
}
node_pair split(node *x, int k){
	if(x == NULL){
		return (node_pair){NULL, NULL};
	}
	node *l = (*x).left, *r = (*x).right;
	node *z = new_node((*x).key);
	free(x);
	int lsize = get_size(l);
	if(k == lsize){
		return (node_pair){l, merge_with_root(NULL, z, r)};
	}
	if(k < lsize){
		node_pair t = split(l, k);
		return (node_pair){t.left, merge_with_root(t.right, z, r)};
	}else{
		node_pair t = split(r, k - lsize - 1);
		return (node_pair){merge_with_root(l, z, t.left), t.right};
	}
}