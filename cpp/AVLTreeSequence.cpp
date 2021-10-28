#include <algorithm>
#include <functional>
#include <utility>
template <class T, T (*op)(T, T), T (*e)(), class F, T (*mapping)(F, T),
          F (*composition)(F, F), F (*id)()>
class AVLTreeSequence {
  struct Node {
    T key, sum;
    F lz;
    int rank, size;
    Node *left, *right;
    bool rev;
    Node(T k)
        : key(k),
          sum(k),
          lz(id()),
          rank(0),
          size(1),
          left(nullptr),
          right(nullptr),
          rev(false) {}
    void push() {
      if (rev) {
        if (left != nullptr) (*left).toggle();
        if (right != nullptr) (*right).toggle();
        rev = false;
      }
      if (lz != id()) {
        if (left != nullptr) (*left).propagate(lz);
        if (right != nullptr) (*right).propagate(lz);
        lz = id();
      }
    }
    void update() {
      rank = std::max(get_rank(left), get_rank(right)) + 1;
      size = get_size(left) + get_size(right) + 1;
      sum = key;
      if (left != nullptr) sum = op((*left).sum, sum);
      if (right != nullptr) sum = op(sum, (*right).sum);
    }
    void toggle() {
      std::swap(left, right);
      rev ^= true;
    }
    void propagate(F f) {
      key = mapping(f, key);
      sum = mapping(f, sum);
      lz = composition(f, lz);
    }
  };
  static int get_rank(Node *x) { return x == nullptr ? -1 : (*x).rank; }
  static int get_size(Node *x) { return x == nullptr ? 0 : (*x).size; }
  Node *left_rotate(Node *x) {
    Node *y = (*x).right;
    if (y == nullptr) return x;
    (*y).push();
    (*x).right = (*y).left;
    (*y).left = x;
    (*x).update();
    (*y).update();
    return y;
  }
  Node *right_rotate(Node *y) {
    Node *x = (*y).left;
    if (x == nullptr) return y;
    (*x).push();
    (*y).left = (*x).right;
    (*x).right = y;
    (*y).update();
    (*x).update();
    return x;
  }
  Node *fixup(Node *x) {
    (*x).push();
    (*x).update();
    int lrank = get_rank((*x).left), rrank = get_rank((*x).right);
    if (abs(lrank - rrank) != 2) return x;
    if (lrank - rrank == 2) {
      (*(*x).left).push();
      if (get_rank((*(*x).left).left) < get_rank((*(*x).left).right)) {
        (*x).left = left_rotate((*x).left);
      }
      return right_rotate(x);
    } else {
      (*(*x).right).push();
      if (get_rank((*(*x).right).left) > get_rank((*(*x).right).right)) {
        (*x).right = right_rotate((*x).right);
      }
      return left_rotate(x);
    }
  }
  Node *take_min(Node *x) {
    if (x == nullptr) return nullptr;
    (*x).push();
    if ((*x).left == nullptr) return x;
    Node *res = take_min((*x).left);
    (*x).left = (*res).right;
    (*res).right = fixup(x);
    return res;
  }
  Node *merge_with_root(Node *l, Node *m, Node *r) {
    int lrank = get_rank(l), rrank = get_rank(r);
    if (abs(lrank - rrank) <= 1) {
      (*m).left = l;
      (*m).right = r;
      (*m).update();
      return m;
    }
    if (l != nullptr) (*l).push();
    if (r != nullptr) (*r).push();
    if (lrank - rrank > 1) {
      (*l).right = merge_with_root((*l).right, m, r);
      return fixup(l);
    } else {
      (*r).left = merge_with_root(l, m, (*r).left);
      return fixup(r);
    }
  }
  Node *merge(Node *l, Node *r) {
    if (l == nullptr) return r;
    if (r == nullptr) return l;
    (*l).push();
    (*r).push();
    Node *m = take_min(r);
    r = (*m).right;
    (*m).right = nullptr;
    return merge_with_root(l, m, r);
  }
  std::pair<Node *, Node *> split(Node *x, int k) {
    if (x == nullptr) {
      return std::make_pair(nullptr, nullptr);
    }
    (*x).push();
    Node *l = (*x).left, *r = (*x).right;
    (*x).left = (*x).right = nullptr;
    (*x).update();
    int lsize = get_size(l);
    if (k == lsize) {
      return std::make_pair(l, merge_with_root(nullptr, x, r));
    }
    if (k < lsize) {
      std::pair<Node *, Node *> t = split(l, k);
      return std::make_pair(t.first, merge_with_root(t.second, x, r));
    } else {
      std::pair<Node *, Node *> t = split(r, k - lsize - 1);
      return std::make_pair(merge_with_root(l, x, t.first), t.second);
    }
  }
  void _set(Node *x, int p, T k) {
    if (x == nullptr) return;
    int lsize = get_size((*x).left);
    if (p < lsize) {
      _set((*x).left, p, k);
    } else if (p == lsize) {
      (*x).key = k;
    } else {
      _set((*x).right, p - lsize - 1, k);
    }
    (*x).update();
  }
  void _add(Node *x, int p, T k) {
    if (x == nullptr) return;
    int lsize = get_size((*x).left);
    if (p < lsize) {
      _add((*x).left, p, k);
    } else if (p == lsize) {
      (*x).key = op(k, (*x).key);
    } else {
      _add((*x).right, p - lsize - 1, k);
    }
    (*x).update();
  }
  Node *find(Node *x, int k) {
    if (x == nullptr) return nullptr;
    (*x).push();
    int lsize = get_size((*x).left);
    if (k == lsize) return x;
    if (k < lsize) return find((*x).left, k);
    return find((*x).right, k - lsize - 1);
  }
  Node *from_vec(int l, int r, const std::vector<T> &v) {
    if (l == r) return nullptr;
    int m = (l + r) / 2;
    Node *res = new Node(v[m]);
    (*res).left = from_vec(l, m, v);
    (*res).right = from_vec(m + 1, r, v);
    (*res).update();
    return res;
  }
  Node *root;

 public:
  AVLTreeSequence(int n = 0) : AVLTreeSequence(std::vector<T>(n, e())) {}
  AVLTreeSequence(const std::vector<T> &v)
      : root(from_vec(0, (int)v.size(), v)) {}
  int size() { return get_size(root); }
  void push_front(T k) { root = merge_with_root(nullptr, new Node(k), root); }
  void push_back(T k) { root = merge_with_root(root, new Node(k), nullptr); }
  void insert(int p, T k) {
    std::pair<Node *, Node *> q = split(root, p);
    root = merge_with_root(q.first, new Node(k), q.second);
  }
  void erase(int p) {
    std::pair<Node *, Node *> q = split(root, p);
    Node *x = take_min(q.second);
    root = merge(q.first, (*x).right);
    delete x;
  }
  void set(int p, T k) { _set(root, p, k); }
  void add(int p, T k) { _add(root, p, k); }
  T get(int p) {
    Node *x = find(root, p);
    return (*x).key;
  }
  T prod(int l, int r) {
    std::pair<Node *, Node *> p = split(root, l);
    std::pair<Node *, Node *> q = split(p.second, r - l);
    T res = (*q.first).sum;
    root = merge(p.first, merge(q.first, q.second));
    return res;
  }
  void reverse(int l, int r) {
    std::pair<Node *, Node *> p = split(root, l);
    std::pair<Node *, Node *> q = split(p.second, r - l);
    (*q.first).toggle();
    root = merge(p.first, merge(q.first, q.second));
  }
  void apply(int l, int r, F f) {
    std::pair<Node *, Node *> p = split(root, l);
    std::pair<Node *, Node *> q = split(p.second, r - l);
    (*q.first).propagate(f);
    root = merge(p.first, merge(q.first, q.second));
  }
};
