#ifndef S21_LIST_TPP_
#define S21_LIST_TPP_
#include "s21_list.h"

namespace s21 {
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
  list<T, Allocator>::list(const list &other, const Allocator& alloc) : list(alloc) {
    for (auto &value : other) {
      this->push_back(value);
    }
  }

  template <typename T, typename Allocator>
  list<T, Allocator>::list(list &&other) {
    this->allocator_(std::move(other.allocator_));
    this->init();
    this->splice(this->begin(), other);
  }
  
  template <typename T, typename Allocator>
  list<T, Allocator>::list(list &&other, const Allocator& alloc) : list(alloc) {
    this->splice(this->begin(), other);
  }
  
  template <typename T, typename Allocator>
  list<T, Allocator>::~list() noexcept {
    clear();
    destroy_node(head_);
  }
  
  template <typename T, typename Allocator>
  void list<T, Allocator>::clear() noexcept {
    while (!empty()) {
      pop_back();
    }
  }
  
  template <typename T, typename Allocator>
  typename list<T, Allocator>::list_iterator list<T, Allocator>::insert(const_iterator pos, const_reference value) {
    auto *node = create_node<T>(value);
    node->ptr()->link_before(pos.current_);
    ++size_;
    iterator iter(node);
    return iter;
  }
  
  template <typename T, typename Allocator>
  void list<T, Allocator>::erase(iterator pos) noexcept {
    if (pos == end()) {
      return;
    }
    if (pos == begin()) {
      head_ = static_cast<list_node*>(head_->ptr()->next);
    }
    auto *node = pos.current_;
    node->unlink();
    destroy_node(static_cast<list_node*>(node));
    --size_;
  }
  
  template <typename T, typename Allocator>
  void list<T, Allocator>::push_back(const_reference value) {
    auto *node = create_node<T>(value);
    node->ptr()->link_before(tail_);
    if (this->empty()) {
      head_ = node;
    }
    ++size_;
  }
  
  template <typename T, typename Allocator>
  void list<T, Allocator>::pop_back() noexcept {
    if (empty()) {
      return;
    }
    auto *node = tail_->prev;
    node->unlink();
    destroy_node(static_cast<list_node*>(node));
    --size_;
    if (empty()) {
      head_ = tail_;
    }
  }
  
  template <typename T, typename Allocator>
  void list<T, Allocator>::push_front(const_reference value) {
    auto *node = create_node<T>(value);
    node->ptr()->link_before(head_);
    head_ = node;
    ++size_;
  }
  
  template <typename T, typename Allocator>
  void list<T, Allocator>::pop_front() noexcept {
    if (empty()) {
      return;
    }
    auto *node = head_;
    head_ = static_cast<list_node*>(head_->ptr()->next);
    node->unlink();
    destroy_node(node);
    --size_;
  }
  
  template <typename T, typename Allocator>
  void list<T, Allocator>::swap(list<T, Allocator> &other) {
    if (&other == this) {
      return;
    }
    std::swap(this->head_, other.head_);
    std::swap(this->tail_, other.tail_);
    std::swap(this->size_, other.size_);
  }
  
  template <typename T, typename Allocator>
  void list<T, Allocator>::merge(list<T, Allocator> &other) {
    if (&other == this) {
      return;
    }
    auto iter1 = this->begin();
    auto iter2 = other.begin();
    while (iter1 != this->end() && iter2 != other.end()) {
      if (*iter1 > *iter2) {
        this->splice(iter1, other, iter2++);
      } else {
        ++iter1;
      }
    }
    if (iter2 != other.end()) {
      this->splice(this->end(), other, iter2, other.end());
    }
    other.clear();
  }
  
  template <typename T, typename Allocator>
  void list<T, Allocator>::splice(const_iterator pos, list &other) {
    splice(pos, other, other.begin(), other.end());
  }
  
  template <typename T, typename Allocator>
  void list<T, Allocator>::splice(const_iterator pos, list &other, const_iterator iter) {
    splice(pos, other, iter, ++iter);
  }
  
  template <typename T, typename Allocator>
  void list<T, Allocator>::splice(const_iterator pos, list &other, const_iterator first, const_iterator last) {
    if (first == last) {
      return;
    }
    
    auto diff = std::distance(first, last);
    list_node* node = pos.current_;
    
    list_node_base::unlink_group(first.current_, last.current_->prev);
    node->ptr()->link_group_before(first.current_, last.current_->prev);
    
    other.size_ -= diff;
    size_ += diff;
  }
  
  template <typename T, typename Allocator>
  void list<T, Allocator>::reverse() {
    iterator iter = begin();
    for (int i = 0; i < size(); ++i) {
      iter.current_->reverse();
      --iter;
    }
    iter.current_->reverse();
    head_ = static_cast<list_node*>(iter.current_->next);
  }

  template <typename T, typename Allocator>
  void list<T, Allocator>::unique() {
    for (auto iter = begin(); iter != end();) {
      if (iter != begin() && *iter == *(--iter)) {
        erase(iter++);
      } else {
        ++iter;
      }
    }
  }

  template <typename T, typename Allocator>
  void list<T, Allocator>::sort() {
    if (empty() || size() == 1) {
      return;
    } // TODO: Not implemented yet
    auto it = ++begin();
    for (int i = 1; i < size(); ++i) {
      iterator it_copy = it;
      iterator node(it.current_.ptr()->prev);
      size_type j = i;
      while (j > 0 && *iterator(node))
    }
  }
} // namespace s21

#endif