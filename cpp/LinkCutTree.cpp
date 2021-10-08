#include <functional>
#include <utility>
#include <vector>
template <class T, T (*op)(T, T), T (*e)()>
class LinkCutTree {
  struct Node {
    Node *parent, *left, *right;
    T data, sum;
    bool rev;
    Node()
        : parent(nullptr),
          left(nullptr),
          right(nullptr),
          data(e()),
          sum(e()),
          rev(false) {}
    bool is_root() const {
      return parent == nullptr ||
             ((*parent).left != this && (*parent).right != this);
    }
    void update() {
      sum = data;
      if (left != nullptr) sum = op((*left).sum, sum);
      if (right != nullptr) sum = op(sum, (*right).sum);
    }
    void toggle() {
      std::swap(left, right);
      rev ^= true;
    }
    void push() {
      if (rev) {
        if (left != nullptr) (*left).toggle();
        if (right != nullptr) (*right).toggle();
        rev = false;
      }
    }
    void rotate_right() {
      Node *q = parent, *r = (*q).parent;
      (*q).left = right;
      if (right != nullptr) (*right).parent = q;
      right = q;
      (*q).parent = this;
      parent = r;
      (*q).update();
      update();
      if (r != nullptr) {
        if ((*r).left == q) (*r).left = this;
        if ((*r).right == q) (*r).right = this;
        (*r).update();
      }
    }
    void rotate_left() {
      Node *q = parent, *r = (*q).parent;
      (*q).right = left;
      if (left != nullptr) (*left).parent = q;
      left = q;
      (*q).parent = this;
      parent = r;
      (*q).update();
      update();
      if (r != nullptr) {
        if ((*r).left == q) (*r).left = this;
        if ((*r).right == q) (*r).right = this;
        (*r).update();
      }
    }
    void splay() {
      push();
      while (!is_root()) {
        Node *q = parent;
        if ((*q).is_root()) {
          (*q).push();
          push();
          if ((*q).left == this) {
            rotate_right();
          } else {
            rotate_left();
          }
        } else {
          Node *r = (*q).parent;
          (*r).push();
          (*q).push();
          push();
          if ((*r).left == q) {
            if ((*q).left == this) {
              (*q).rotate_right();
              rotate_right();
            } else {
              rotate_left();
              rotate_right();
            }
          } else {
            if ((*q).right == this) {
              (*q).rotate_left();
              rotate_left();
            } else {
              rotate_right();
              rotate_left();
            }
          }
        }
      }
    }
  };
  Node *expose(Node *x) {
    Node *rp = nullptr;
    for (Node *p = x; p != nullptr; p = (*p).parent) {
      (*p).splay();
      (*p).right = rp;
      (*p).update();
      rp = p;
    }
    (*x).splay();
    return x;
  }
  void _cut(Node *c) {
    expose(c);
    Node *p = (*c).left;
    (*c).left = nullptr;
    (*p).parent = nullptr;
    (*c).update();
  }
  void _link(Node *c, Node *p) {
    expose(c);
    expose(p);
    (*c).parent = p;
    (*p).right = c;
    (*p).update();
  }
  void _evert(Node *c) {
    expose(c);
    (*c).toggle();
    (*c).push();
  }
  Node *_lca(Node *u, Node *v) {
    expose(u);
    expose(v);
    if ((*u).parent == nullptr) return nullptr;
    Node *d = u;
    while ((*u).parent != v) {
      if ((*u).is_root()) {
        d = (*u).parent;
      }
      u = (*u).parent;
    }
    if (u == (*v).left) {
      return d;
    } else {
      return v;
    }
  }
  std::vector<Node> tree;

 public:
  LinkCutTree(int n) : tree(n){};
  void cut(int c) { _cut(&tree[c]); }
  void link(int c, int p) { _link(&tree[c], &tree[p]); }
  void evert(int c) { _evert(&tree[c]); }
  T query(int u) {
    expose(&tree[u]);
    return tree[u].sum;
  }
  T query(int u, int v) {
    evert(u);
    return query(v);
  }
  void set(int u, T x) {
    expose(&tree[u]);
    tree[u].data = x;
    tree[u].update();
  }
  void add(int u, T x) { set(u, op(x, tree[u].data)); }
  int lca(int u, int v) {
    Node *m = _lca(&tree[u], &tree[v]);
    if (m == nullptr) {
      return -1;
    } else {
      return (int)(m - &tree[0]);
    }
  }
};
