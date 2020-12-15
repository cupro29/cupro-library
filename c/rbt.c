#include <stdlib.h>
#include <stdbool.h>

// Red-Black-Tree
enum Color {red, black};
typedef int RBT_key_type;
bool RBT_key_lt(RBT_key_type a, RBT_key_type b){
	return a < b;
}
bool RBT_key_eq(RBT_key_type a, RBT_key_type b){
	return a == b;
}
bool RBT_key_gt(RBT_key_type a, RBT_key_type b){
	return a > b;
	// return !RBT_key_lt(a, b) && !RBT_key_eq(a, b);
}
typedef struct RBT_Node {
	RBT_key_type key;
	enum Color color;
	struct RBT_Node *left, *right, *parent;
} RBT_Node;
typedef struct {
	RBT_Node *root;
} RBT;
RBT_Node *RBT_nil;
__attribute__((constructor))
void RBT_nil_init(){
	RBT_nil = (RBT_Node*)malloc(sizeof(RBT_Node));
	(*RBT_nil).color = black;
	(*RBT_nil).left = (*RBT_nil).right = (*RBT_nil).parent = RBT_nil;
	return;
}
RBT_Node* tree_minimum(RBT_Node *x){
	RBT_Node *res = x;
	while((*res).left != RBT_nil) res = (*res).left;
	return res;
}
RBT_Node* tree_maximum(RBT_Node *x){
	RBT_Node *res = x;
	while((*res).right != RBT_nil) res = (*res).right;
	return res;
}
RBT_Node* RBT_find(RBT_Node *root, RBT_key_type t){
	RBT_Node *res = root;
	while(res != RBT_nil && !RBT_key_eq((*res).key, t)){
		if(RBT_key_gt((*res).key, t)){
			res = (*res).left;
		}else{
			res = (*res).right;
		}
	}
	return res;
}
void left_rotate(RBT *T, RBT_Node *x){
	RBT_Node *y = (*x).right;
	(*x).right = (*y).left;
	if((*y).left != RBT_nil){
		(*(*y).left).parent = x;
	}
	(*y).parent = (*x).parent;
	if((*x).parent == RBT_nil){
		(*T).root = y;
	}else if(x == (*(*x).parent).left){
		(*(*x).parent).left = y;
	}else{
		(*(*x).parent).right = y;
	}
	(*y).left = x;
	(*x).parent = y;
	return;
}
void right_rotate(RBT *T, RBT_Node *x){
	RBT_Node *y = (*x).left;
	(*x).left = (*y).right;
	if((*y).right != RBT_nil){
		(*(*y).right).parent = x;
	}
	(*y).parent = (*x).parent;
	if((*x).parent == RBT_nil){
		(*T).root = y;
	}else if(x == (*(*x).parent).right){
		(*(*x).parent).right = y;
	}else{
		(*(*x).parent).left = y;
	}
	(*y).right = x;
	(*x).parent = y;
	return;
}
void insert_fixup(RBT *T, RBT_Node *z){
	while((*(*z).parent).color == red){
		if((*z).parent == (*(*(*z).parent).parent).left){
			RBT_Node *y = (*(*(*z).parent).parent).right;
			if((*y).color == red){
				(*(*z).parent).color = black;
				(*y).color = black;
				(*(*(*z).parent).parent).color = red;
				z = (*(*z).parent).parent;
			}else {
				if(z == (*(*z).parent).right){
					z = (*z).parent;
					left_rotate(T, z);
				}
				(*(*z).parent).color = black;
				(*(*(*z).parent).parent).color = red;
				right_rotate(T, (*(*z).parent).parent);
			}
		}else{
			RBT_Node *y = (*(*(*z).parent).parent).left;
			if((*y).color == red){
				(*(*z).parent).color = black;
				(*y).color = black;
				(*(*(*z).parent).parent).color = red;
				z = (*(*z).parent).parent;
			}else {
				if(z == (*(*z).parent).left){
					z = (*z).parent;
					right_rotate(T, z);
				}
				(*(*z).parent).color = black;
				(*(*(*z).parent).parent).color = red;
				left_rotate(T, (*(*z).parent).parent);
			}
		}
	}
	(*(*T).root).color = black;
	return;
}
void RBT_insert(RBT *T, RBT_Node *z){
	RBT_Node *y = RBT_nil;
	RBT_Node *x = (*T).root;
	while(x != RBT_nil){
		y = x;
		if(RBT_key_lt((*z).key, (*x).key)){
			x = (*x).left;
		}else {
			x = (*x).right;
		}
	}
	(*z).parent = y;
	if(y == RBT_nil){
		(*T).root = z;
	}else if(RBT_key_lt((*z).key, (*y).key)){
		(*y).left = z;
	}else{
		(*y).right = z;
	}
	insert_fixup(T, z);
	return;
}
void RBT_push(RBT *T, RBT_key_type x){
	RBT_Node *z = (RBT_Node*)malloc(sizeof(RBT_Node));
	(*z).key = x;
	(*z).parent = (*z).left = (*z).right = RBT_nil;
	(*z).color = red;
	RBT_insert(T, z);
	return;
}
void transplant(RBT *T, RBT_Node *u, RBT_Node *v){
	if((*u).parent == RBT_nil){
		(*T).root = v;
	}else if(u == (*(*u).parent).left){
		(*(*u).parent).left = v;
	}else{
		(*(*u).parent).right = v;
	}
	(*v).parent = (*u).parent;
	return;
}
void delete_fixup(RBT *T, RBT_Node *x){
	while(x != (*T).root && (*x).color == black){
		if(x == (*(*x).parent).left){
			RBT_Node *w = (*(*x).parent).right;
			if((*w).color == red){
				(*w).color = black;
				(*(*x).parent).color = red;
				left_rotate(T, (*x).parent);
				w = (*(*x).parent).right;
			}
			if((*(*w).left).color == black && (*(*w).right).color == black){
				(*w).color = red;
				x = (*x).parent;
			}else{
				if((*(*w).right).color == black){
					(*(*w).left).color = black;
					(*w).color = red;
					right_rotate(T, w);
					w = (*(*x).parent).right;
				}
				(*w).color = (*(*x).parent).color;
				(*(*x).parent).color = black;
				(*(*w).right).color = black;
				left_rotate(T, (*x).parent);
				x = (*T).root;
			}
		}else{
			RBT_Node *w = (*(*x).parent).left;
			if((*w).color == red){
				(*w).color = black;
				(*(*x).parent).color = red;
				right_rotate(T, (*x).parent);
				w = (*(*x).parent).left;
			}
			if((*(*w).right).color == black && (*(*w).left).color == black){
				(*w).color = red;
				x = (*x).parent;
			}else{
				if((*(*w).left).color == black){
					(*(*w).right).color = black;
					(*w).color = red;
					left_rotate(T, w);
					w = (*(*x).parent).left;
				}
				(*w).color = (*(*x).parent).color;
				(*(*x).parent).color = black;
				(*(*w).left).color = black;
				right_rotate(T, (*x).parent);
				x = (*T).root;
			}
		}
	}
	(*x).color = black;
	return;
}
bool RBT_delete(RBT *T, RBT_key_type t){
	RBT_Node *z = RBT_find((*T).root, t);
	if(z == RBT_nil) return false;
	RBT_Node *x;
	RBT_Node *y = z;
	enum Color y_original_color = (*y).color;
	if((*z).left == RBT_nil){
		x = (*z).right;
		transplant(T, z, (*z).right);
	}else if((*z).right == RBT_nil){
		x = (*z).left;
		transplant(T, z, (*z).left);
	}else{
		y = tree_minimum((*z).right);
		y_original_color = (*y).color;
		x = (*y).right;
		if((*y).parent == z){
			(*x).parent = y;
		}else{
			transplant(T, y, (*y).right);
			(*y).right = (*z).right;
			(*(*y).right).parent = y;
		}
		transplant(T, z, y);
		(*y).left = (*z).left;
		(*(*y).left).parent = y;
		(*y).color = (*z).color;
	}
	free(z);
	if(y_original_color == black){
		delete_fixup(T, x);
	}
	return true;
}
