#include <vector>
class LinkCutTree {
 private:
  struct Node {
    Node *parent, *left, *right;
    Node() : parent(nullptr), left(nullptr), right(nullptr) {}
    bool is_root() {
      return parent == nullptr ||
             ((*parent).left != this && (*parent).right != this);
    }
    void rotate_right() {
      Node *q = parent, *r = (*q).parent;
      (*q).left = right;
      if (right != nullptr) (*right).parent = q;
      right = q;
      (*q).parent = this;
      parent = r;
      if (r != nullptr) {
        if ((*r).left == q) (*r).left = this;
        if ((*r).right == q) (*r).right = this;
      }
    }
    void rotate_left() {
      Node *q = parent, *r = (*q).parent;
      (*q).right = left;
      if (left != nullptr) (*left).parent = q;
      left = q;
      (*q).parent = this;
      parent = r;
      if (r != nullptr) {
        if ((*r).left == q) (*r).left = this;
        if ((*r).right == q) (*r).right = this;
      }
    }
    void splay() {
      while (!is_root()) {
        Node *q = parent;
        if ((*q).is_root()) {
          if ((*q).left == this) {
            rotate_right();
          } else {
            rotate_left();
          }
        } else {
          Node *r = (*q).parent;
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
  }
  void _link(Node *c, Node *p) {
    expose(c);
    expose(p);
    (*c).parent = p;
    (*p).right = c;
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
  int lca(int u, int v) {
    Node *m = _lca(&tree[u], &tree[v]);
    if (m == nullptr) {
      return -1;
    } else {
      return (int)(m - &tree[0]);
    }
  }
};
