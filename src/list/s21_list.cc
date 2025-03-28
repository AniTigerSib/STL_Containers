#include "s21_list.h"

namespace s21 {
  template <typename T, typename Allocator>
  using iterator = typename list<T, Allocator>::list_iterator;

  template <typename T, typename Allocator>
  list<T, Allocator>::list(size_type count, const_reference value, const Allocator& alloc)
    : allocator_(alloc) {
    init();
    for (size_type i = 0; i < count; ++i) {
      push_back(value);
    }
  }

  template <typename T, typename Allocator>
  list<T, Allocator>::list(std::initializer_list<value_type> const &items, const Allocator& alloc)
  : allocator_(alloc) {
    init();
    for (const auto &value : items) {
      push_back(value);
    }
  }

  template <typename T, typename Allocator>
  list<T, Allocator>::list(const list &other) {
    this->allocator_ = other.allocator_;
    this->init();
    for (auto &value : other) {
      this->push_back(value);
    }
  }

  template <typename T, typename Allocator>
  void list<T, Allocator>::clear() noexcept {
    while (!empty()) {
      pop_back();
    }
  }

  template <typename T, typename Allocator>
  iterator<T, Allocator> list<T, Allocator>::insert(iterator pos, const_reference value) {
    auto *node = create_node<T>(value);
    node->ptr()->link_before(pos.current_);
    iterator iter(node);
    return iter;
  }

  template <typename T, typename Allocator>
  void list<T, Allocator>::erase(iterator pos) noexcept {
    if (pos == end()) {
      return;
    }
    auto *node = pos.current_;
    node->unlink();
    destroy_node(node);
  }

  template <typename T, typename Allocator>
  void list<T, Allocator>::push_back(const_reference value) {
    auto *node = create_node<T>(value);
    node->ptr()->link_before(tail_);
    ++size_;
  }

  template <typename T, typename Allocator>
  void list<T, Allocator>::pop_back() noexcept {
    if (empty()) {
      return;
    }
    auto *node = tail_->prev_;
    node->unlink();
    destroy_node(node);
    --size_;
  }

  template <typename T, typename Allocator>
  void list<T, Allocator>::push_front(const_reference value) {
    auto *node = create_node<T>(value);
    node->ptr()->link_before(head_);
    ++size_;
  }

  template <typename T, typename Allocator>
  void list<T, Allocator>::pop_front() noexcept {
    if (empty()) {
      return;
    }
    auto *node = head_->next_;
    node->unlink();
    destroy_node(node);
    --size_;
  }
} // namespace s21