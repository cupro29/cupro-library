/*
typedef struct AVLT_Node{
	AVLT_key_type key;
	int rank, count;
	struct AVLT_Node *left, *right;
}AVLT_Node;
AVLT_Node* AVLT_make_Node(AVLT_key_type t, int cnt){
	AVLT_Node *res = (AVLT_Node*)malloc(sizeof(AVLT_Node));
	(*res).key = t;
	(*res).rank = 0;
	(*res).count = cnt;
	(*res).left = (*res).right = NULL;
	return res;
}
int AVLT_get_count(AVLT_Node *x){
	if(is_null(x)) return 0;
	return (*x).count;
}

void push(AVLT *T, AVLT_key_type t, int cnt){
	AVLT_Node *x = AVLT_find((*T).root, t);
	if(is_null(x)){
		AVLT_insert(T, AVLT_make_Node(t, cnt));
	}else{
		(*x).count += cnt;
	}
	return;
}
// */




#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>
bool is_null(void *p){return p==NULL;}
 
int min(const int a, const int b){return a>b?b:a;}
int max(const int a, const int b){return a>b?a:b;}
 
//AVL-Tree
typedef int AVLT_key_type;
bool AVLT_key_lt(AVLT_key_type a, AVLT_key_type b){
	return a < b;
}
bool AVLT_key_eq(AVLT_key_type a, AVLT_key_type b){
	return a == b;
}
bool AVLT_key_gt(AVLT_key_type a, AVLT_key_type b){
	return a > b;
	// return !AVLT_key_lt(a, b) && !AVLT_key_eq(a, b);
}
typedef struct AVLT_Node{
	AVLT_key_type key;
	int rank;
	struct AVLT_Node *left, *right;
}AVLT_Node;
AVLT_Node* AVLT_make_Node(AVLT_key_type t){
	AVLT_Node *res = (AVLT_Node*)malloc(sizeof(AVLT_Node));
	(*res).key = t;
	(*res).rank = 0;
	(*res).left = (*res).right = NULL;
	return res;
}
 
typedef struct{ AVLT_Node *root; } AVLT;
int AVLT_get_rank(AVLT_Node *x){ return is_null(x) ? -1 : (*x).rank; }
AVLT_Node* AVLT_minimum(AVLT_Node *x){
	if(is_null(x)) return NULL;
	AVLT_Node *res = x;
	while(!is_null((*res).left)) res = (*res).left;
	return res;
}
AVLT_Node* AVLT_maximum(AVLT_Node *x){
	if(is_null(x)) return NULL;
	AVLT_Node *res = x;
	while(!is_null((*res).right)) res = (*res).right;
	return res;
}
AVLT_Node* AVLT_find(AVLT_Node *x, AVLT_key_type t){
	AVLT_Node *res = x;
	while(!is_null(res) && !AVLT_key_eq((*res).key, t)){
		if(AVLT_key_gt((*res).key, t)){
			res = (*res).left;
		}else{
			res = (*res).right;
		}
	}
	return res;
}
//       x               y
//     a   y    =>     x   c
//        b c         a b
AVLT_Node* AVLT_left_rotate(AVLT *T, AVLT_Node *x){
	if(is_null(x)) return NULL;
	AVLT_Node *y = (*x).right;
	if(is_null(y)) return NULL;
	(*x).right = (*y).left;
	if(x == (*T).root){
		(*T).root = y;
	}
	(*y).left = x;
	(*x).rank = max(AVLT_get_rank((*x).left), AVLT_get_rank((*x).right)) + 1;
	(*y).rank = max(AVLT_get_rank((*y).left), AVLT_get_rank((*y).right)) + 1;
	return y;
}
//       x               y
//     a   y    <=     x   c
//        b c         a b
AVLT_Node* AVLT_right_rotate(AVLT *T, AVLT_Node *y){
	if(is_null(y)) return NULL;
	AVLT_Node *x = (*y).left;
	if(is_null(x)) return NULL;
	(*y).left = (*x).right;
	if(y == (*T).root){
		(*T).root = x;
	}
	(*x).right = y;
	(*y).rank = max(AVLT_get_rank((*y).left), AVLT_get_rank((*y).right)) + 1;
	(*x).rank = max(AVLT_get_rank((*x).left), AVLT_get_rank((*x).right)) + 1;
	return x;
}
AVLT_Node* AVLT_fixup(AVLT *T, AVLT_Node *x){
	if(is_null(x)) return NULL;
	int l = AVLT_get_rank((*x).left), r = AVLT_get_rank((*x).right);
	if(abs(l - r) <= 1) return x;
	if(l - r == 2){
		if(AVLT_get_rank((*(*x).left).left) < AVLT_get_rank((*(*x).left).right)){
			(*x).left = AVLT_left_rotate(T, (*x).left);
		}
		return AVLT_right_rotate(T, x);
	}else if(l - r == -2){
		if(AVLT_get_rank((*(*x).right).left) > AVLT_get_rank((*(*x).right).right)){
			(*x).right = AVLT_right_rotate(T, (*x).right);
		}
		return AVLT_left_rotate(T, x);
	}
	return NULL;
}
void AVLT_insert(AVLT *T, AVLT_Node *z){
	AVLT_Node *sta[60];
	int sz = 0;
	unsigned long long is_left = 0;
	AVLT_Node *y = NULL;
	AVLT_Node *x = (*T).root;
	while(!is_null(x)){
		sta[sz] = y = x;
		if(AVLT_key_lt((*z).key, (*x).key)){
			x = (*x).left;
			is_left |= (1ull<<sz);
		}else{
			x = (*x).right;
		}
		sz++;
	}
	if(is_null(y)){
		(*T).root = z;
	}else if(AVLT_key_lt((*z).key, (*y).key)){
		(*y).left = z;
	}else {
		(*y).right = z;
	}
	while(sz--){
		y = sta[sz];
		int l = AVLT_get_rank((*y).left), r = AVLT_get_rank((*y).right);
		(*y).rank = max(l, r) + 1;
		if(l - r == 0){
			return;
		}else if(abs(l - r) == 2){
			y = AVLT_fixup(T, y);
			if(sz == 0 || is_null(y)) return;
			if((is_left >> (sz - 1)) & 1){
				(*sta[sz-1]).left = y;
			}else{
				(*sta[sz-1]).right = y;
			}
			return;
		}
	}
	return;
}
bool AVLT_delete(AVLT *T, AVLT_key_type t){
	AVLT_Node *sta[60];
	int sz = 0;
	unsigned long long is_left = 0;
	AVLT_Node *x = (*T).root;
	while(!is_null(x) && !AVLT_key_eq((*x).key, t)){
		sta[sz] = x;
		if(AVLT_key_lt(t, (*x).key)){
			x = (*x).left;
			is_left |= (1ull<<sz);
		}else{
			x = (*x).right;
		}
		sz++;
	}
	if(is_null(x)) return false;
	int d = sz;
	sta[sz++] = x;
	if(is_null((*x).right)){
		sta[d] = (*x).left;
		is_left |= (1ull << d);
		if(is_null((*x).left)) sz--;
	}else{
		AVLT_Node *y = (*x).right;
		sta[sz++] = y;
		while(!is_null((*y).left)){
			sta[sz] = y = (*y).left;
			is_left |= (1ull<<(sz-1));
			sz++;
		}sz--;
		if(!is_null(sta[sz])){
			if((is_left >> (sz-1)) & 1){
				(*sta[sz-1]).left = (*sta[sz]).right;
			}else{
				(*sta[sz-1]).right = (*sta[sz]).right;
			}
			(*sta[sz]).left = (*x).left;
			(*sta[sz]).right = (*x).right;
			(*sta[sz]).rank = (*x).rank;
		}
		sta[d] = sta[sz];
		sta[sz] = NULL;
	}
	if(d == 0){
		(*T).root = sta[d];
	}else{
		if((is_left >> (d-1)) & 1){
			(*sta[d-1]).left = sta[d];
		}else{
			(*sta[d-1]).right = sta[d];
		}
	}
	while(sz--){
		AVLT_Node *y = sta[sz];
		int l = AVLT_get_rank((*y).left), r = AVLT_get_rank((*y).right);
		(*y).rank = max(l, r) + 1;
		if(abs(l - r) >= 2){
			y = AVLT_fixup(T, y);
			if(sz == 0 || is_null(y)) break;
			if((is_left >> (sz - 1)) & 1){
				(*sta[sz-1]).left = y;
			}else{
				(*sta[sz-1]).right = y;
			}
		}
	}
	free(x);
	return true;
}