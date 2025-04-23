#ifndef S21_RB_TREE_H_
#define S21_RB_TREE_H_

#include <algorithm>
#include <iostream>

namespace s21::rbtree {
template <typename Key>
struct btree_node {
  Key key;
  bool is_red = true;
  btree_node *parent;
  btree_node *left;
  btree_node *right;

  btree_node() = default;
  explicit btree_node(const Key &key) :
    key(key), left(nullptr), right(nullptr), parent(nullptr) {}
  constexpr btree_node(const Key &key, btree_node *left,
    btree_node *right, btree_node *parent) :
    key(key), left(left), right(right), parent(parent) {}

  bool operator<(btree_node *other) {
    return this->key < other->key;
  }
  bool operator>(btree_node *other) {
    return this->key > other->key;
  }
};

template <typename Key, typename Value>
struct btree_node_ext : public btree_node<Key> {
  using Base = btree_node<Key>;
  Value value;

  btree_node_ext() = default;
  explicit btree_node_ext(const Key &key) :
    Base(key), value() {}
  constexpr btree_node_ext(const Key &key, const Value &value) :
    Base(key), value(value) {}
  constexpr btree_node_ext(const Key &key, const Value &value,
    btree_node_ext *left, btree_node_ext *right,
    btree_node_ext *parent) : Base(key, left, right, parent),
    value(value) {}
};

template <typename Key>
class btree_iterator {
  using node = btree_node<Key>;
  node *ptr_;
public:

};

// template <typename Key>
// class IBtree {
//   virtual void insert(Key &&key) = 0;
// };

template <typename Key>
class btree {
  using value_type = Key;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;

  using tree_node = btree_node<Key>;
  using node_ref = tree_node&;
  using node_ptr = tree_node*;
  node_ptr root_;
 public:
  btree() : root_(nullptr) {}
  explicit btree(const_reference key) : root_(new tree_node(key)) {}
  // btree(const btree& other) {
  //   // TODO
  // }
  btree(btree&& other) noexcept {
    root_ = std::move(other.root_);
  }
  ~btree() {
    postorder_traversal([](node_ptr node) { delete node; });
  }

  /////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////

  template<typename Func>
  constexpr void sorted_traversal(node_ptr node, Func func) {
    if (node) {
      sorted_traversal(node->left, func);
      func(node);
      sorted_traversal(node->right, func);
    }
  }

  template<typename Func>
  constexpr void sorted_traversal(Func func) {
    if (root_) {
      sorted_traversal(root_->left, func);
      func(root_);
      sorted_traversal(root_->right, func);
    }
  }

  template<typename Func>
  constexpr void preorder_traversal(node_ptr node, Func func) {
    if (node) {
      func(node);
      preorder_traversal(node->left, func);
      preorder_traversal(node->right, func);
    }
  }

  template<typename Func>
  constexpr void preorder_traversal(Func func) {
    if (root_) {
      func(root_);
      preorder_traversal(root_->left, func);
      preorder_traversal(root_->right, func);
    }
  }

  template<typename Func>
  constexpr void postorder_traversal(node_ptr node, Func func) {
    if (node) {
      postorder_traversal(node->left, func);
      postorder_traversal(node->right, func);
      func(node);
    }
  }

  template<typename Func>
  constexpr void postorder_traversal(Func func) {
    if (root_) {
      postorder_traversal(root_->left, func);
      postorder_traversal(root_->right, func);
      func(root_);
    }
  }


  int height(node_ptr node) {
    if (!node) {
      return 0;
    }
    return std::max(height(node->left), height(node->right)) + 1;
  }

  int height() {
    return std::max(height(root_->left), height(root_->right)) + 1;
  }

  /////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////

  node_ptr search(const_reference key) {
    node_ptr current = root_;
    while (current == nullptr || current->key == key) {
      if (key < current->key) {
        current = current->left;
      } else if (key > current->key) {
        current = current->right;
      }
    }
    return current;
  }

  bool contains(const_reference key) {
    node_ptr current = root_;
    while (current != nullptr && current->key != key) {
      if (key < current->key) {
        current = current->left;
      } else if (key > current->key) {
        current = current->right;
      }
    }
    return current && current->key == key;
  }

  void insert(const_reference key) {
    auto node = new tree_node(key);
    if (!root_) {
      root_ = node;
      return;
    }
    node_ptr current = root_;
    node_ptr prev = nullptr;
    while (current) {
      prev = current;
      if (node->key < current->key) {
        current = current->left;
      } else {
        current = current->right;
      }
    }
    node->parent = prev;
    if (node->key < prev->key) {
      prev->left = node;
    } else {
      prev->right = node;
    }
    fix_insertion(node);
  }

  void remove(const_reference key) {
    // TODO(micahel): REFACTOR THIS PIECE OF SHIT
    node_ptr node = search(key);
    if (!node) {
      return;
    }
    const bool k_is_left = node->parent && node->parent->left == node;
    if (!node->left && !node->right) {
      if (node == root_) {
        root_ = nullptr;
      } else {
        if (k_is_left) {
          node->parent->left = nullptr;
        } else {
          node->parent->right = nullptr;
        }
      }
      delete node;
      return;
    }
    if (node->right && node->left) {
      node_ptr temp = node->right;
      while (temp->left) {
        temp = temp->left;
      }
      if (temp->right) {
        temp->right->parent = temp->parent;
      }
      if (temp->parent == node) {
        node->right = temp->right;
      } else {
        temp->parent->left = temp->right;
      }
    } else {
      if (node == root_) {
        root_ = node->left ? node->left : node->right;
      } else {
        if (k_is_left) {
          node->parent->left = node->left ? node->left : node->right;
        } else {
          node->parent->right = node->left ? node->left : node->right;
        }
      }
    }
  }

  /////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////

 private:
  void left_rotate(node_ptr node) {
    if (!node || node->right == nullptr) {
      return;
    }
    node_ptr temp = node->right;
    node->right = temp->left;
    if (temp->left) {
      temp->left->parent = node;
    }
    if (node == root_) {
      root_ = temp;
    } else {
      if (node->parent->left == node) {
        node->parent->left = temp;
      } else {
        node->parent->right = temp;
      }
    }
    temp->parent = node->parent;
    temp->left = node;
    node->parent = temp;
  }

  void right_rotate(node_ptr node) {
    if (!node || node->left == nullptr) {
      return;
    }
    node_ptr temp = node->left;
    node->left = temp->right;
    if (temp->right) {
      temp->right->parent = node;
    }
    if (node == root_) {
      root_ = temp;
    } else {
      if (node->parent->left == node) {
        node->parent->left = temp;
      } else {
        node->parent->right = temp;
      }
    }
    temp->parent = node->parent;
    temp->right = node;
    node->parent = temp;
  }

  void fix_insertion(node_ptr node) {
    if (node == root_) {
      node->is_red = false;
      return;
    }
    while (node->parent && node->parent->parent && node->parent->is_red) {
      if (node->parent == node->parent->parent->left) {
        if (node->parent->parent->right && node->parent->parent->right->is_red) {
          node->parent->is_red = false;
          node->parent->parent->right->is_red = false;
          node->parent->parent->is_red = true;
          node = node->parent->parent;
        } else {
          if (node == node->parent->right) {
            node = node->parent;
            left_rotate(node);
          }
          node->parent->is_red = false;
          node->parent->parent->is_red = true;
          right_rotate(node->parent->parent);
        }
      } else {
        if (node->parent->parent->left && node->parent->parent->left->is_red) {
          node->parent->is_red = false;
          node->parent->parent->left->is_red = false;
          node->parent->parent->is_red = true;
          node = node->parent->parent;
        } else {
          if (node == node->parent->left) {
            node = node->parent;
            right_rotate(node);
          }
          node->parent->is_red = false;
          node->parent->parent->is_red = true;
          left_rotate(node->parent->parent);
        }
      }
    }
    root_->is_red = false;
  }

  // void fix_removing(node_ptr node) {}
};
} // namespace s21::rbtree

#endif // S21_RB_TREE_H_