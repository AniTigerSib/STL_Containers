#ifndef S21_LIST_H_
#define S21_LIST_H_

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <type_traits>
#include <cstring>

#include <iostream>

namespace s21 {
namespace list_details {
struct list_node_base {
  list_node_base *prev;
  list_node_base *next;

  list_node_base() {
    init();
  }

  void unlink();
  void link_before(list_node_base *node);
  void link_group_before(list_node_base *first, list_node_base *last);
  static void unlink_group(list_node_base *first, list_node_base *last) noexcept;
  static void swap(list_node_base *lhs, list_node_base *rhs) noexcept;
  void reverse() {
    std::swap(next, prev);
  }

 private:
  void init() {
    next = this;
    prev = this;
  }
};
} // namespace list_details

template <typename T, typename Allocator = std::allocator<T>>
class list {

  // NODES //////////////////////

 private:
  struct list_node : public list_details::list_node_base {
    T data;

    explicit list_node(const T &value)
      : data(value) {}
    
    list_node_base *ptr() noexcept {
      return this;
    }
  };

  // ITERATORS //////////////////

 public:
  struct list_iterator {
    using Self = list_iterator;
    using Node = list_node;

    using difference_type = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    
    list_iterator() noexcept : current_(nullptr) {}
    list_iterator(list_iterator &other) noexcept : current_(other.current_) {}
    explicit list_iterator(list_details::list_node_base *node) : current_(node) {}

    reference operator*() noexcept {
      return static_cast<list_node*>(current_)->data;
    }

    pointer operator->() noexcept {
      return &(static_cast<list_node*>(current_)->data);
    }

    bool operator==(const Self &other) const noexcept = default;
    bool operator!=(const Self &other) const noexcept = default;

    Self &operator++() noexcept {
      current_ = current_->next;
      return *this;
    }

    Self operator++(int) noexcept {
      Self tmp = *this;
      current_ = current_->next;
      return tmp;
    }

    Self &operator--() noexcept {
      current_ = current_->prev;
      return *this;
    }

    Self operator--(int) noexcept {
      Self tmp = *this;
      current_ = current_->prev;
      return tmp;
    }

    Self operator+(int count) noexcept {
      Self res = *this;
      for (int i = 0; i < count; ++i) {
        res.current_ = res.current_->next;
      }
      return res;
    }

    Self operator-(int count) noexcept {
      Self res = *this;
      for (int i = 0; i < count; ++i) {
        res.current_ = res.current_->prev;
      }
      return res;
    }

   private:
   list_details::list_node_base* current_;
    friend class list<T, Allocator>;
  };

  struct list_const_iterator {
    using Self = list_const_iterator;
    using Node = const list_node;

    using difference_type = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using pointer = const T*;
    using reference = const T&;

    list_const_iterator() noexcept : current_(nullptr) {}
    list_const_iterator(list_const_iterator &other) noexcept : current_(other.current_) {}
    explicit list_const_iterator(const list_details::list_node_base *node) noexcept : current_(node) {}

    reference operator*() const noexcept {
      return static_cast<const list_node*>(current_)->data;
    }

    pointer operator->() const noexcept {
      return &static_cast<const list_node*>(current_)->data;
    }

    bool operator==(const Self &other) const noexcept = default;
    bool operator!=(const Self &other) const noexcept = default;

    Self &operator++() noexcept {
      current_ = current_->next;
      return *this;
    }

    Self operator++(int) noexcept {
      Self tmp = *this;
      current_ = current_->next;
      return tmp;
    }

    Self &operator--() noexcept {
      current_ = current_->prev;
      return *this;
    }

    Self operator--(int) noexcept {
      Self tmp = *this;
      current_ = current_->prev;
      return tmp;
    }

   private:
    const list_details::list_node_base *current_;
    friend class list<T, Allocator>;
  };

  // MAIN ///////////////////////

  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using iterator = list_iterator;
  using const_iterator = list_const_iterator;
  using size_type = size_t;
  using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<list_node>;
  
  list() : list(Allocator()) {};
  explicit list(const Allocator& alloc) : allocator_(alloc) { init(); }
  explicit list(size_type count, const_reference value = value_type(),
                const Allocator& alloc = Allocator()) : allocator_(alloc) {
    init();
    for (size_type i = 0; i < count; ++i) {
      push_back(value);
    }
  }
  list(std::initializer_list<value_type> const &items,
       const Allocator& alloc = Allocator()) : allocator_(alloc) {
    init();
    for (const auto &value : items) {
      push_back(value);
    }
  }
  list(const list &other) {
    this->allocator_ = other.allocator_;
    this->init();
    for (auto &value : other) {
      this->push_back(value);
    }
  }
  list(const list &other, const Allocator& alloc) : list(alloc) {
    for (auto &value : other) {
      this->push_back(value);
    }
  }
  list(list &&other) {
    this->allocator_ = std::move(other.allocator_);
    this->init();
    this->splice(this->begin(), other);
  }
  list(list &&other, const Allocator& alloc) : list(alloc) {
    this->splice(this->begin(), other);
  }
  ~list() noexcept {
    clear();
    destroy_node(head_);
  }
  constexpr list &operator=(list &&other) {
    // TODO(michael): To test this
    this->clear();
    if (static_cast<bool>(std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value)) {
      auto node = this->end().current_;
      destroy_node(node, false);
      this->allocator_(std::move(other.allocator_));
      init();
    }
    this->splice(this->begin(), other);
    return *this;
  }

  const_reference front() const noexcept { return *begin(); }
  const_reference back() const noexcept { return *(--end()); }

  iterator begin() noexcept { return iterator(head_); }
  iterator end() noexcept { return iterator(tail_); }
  const_iterator begin() const noexcept { return const_iterator(head_); }
  const_iterator end() const noexcept { return const_iterator(tail_); }
  const_iterator cbegin() const noexcept { return const_iterator(head_); }
  const_iterator cend() const noexcept { return const_iterator(tail_); }

  [[nodiscard]] bool empty() const noexcept { return size_ == 0; }
  [[nodiscard]] size_type size() const noexcept { return size_; }
  [[nodiscard]] size_type max_size() const noexcept {
    return std::allocator_traits<allocator_type>::max_size(allocator_); 
  }

  void clear() noexcept {
    while (!empty()) {
      pop_back();
    }
  }
  iterator insert(const_iterator pos, const_reference value) {
    auto *node = create_node(value);
    node->ptr()->link_before(pos.current_);
    ++size_;
    iterator iter(node);
    return iter;
  }
  iterator erase(iterator pos) noexcept {
    if (pos == end()) {
      return end();
    }
    if (pos == begin()) {
      head_ = static_cast<list_node*>(head_->ptr()->next);
    }
    iterator to_return = pos + 1;
    auto *node = pos.current_;
    node->unlink();
    destroy_node(static_cast<list_node*>(node));
    --size_;
    return to_return;
  }
  void push_back(const_reference value) {
    auto *node = create_node(value);
    node->ptr()->link_before(tail_);
    if (this->empty()) {
      head_ = node;
    }
    ++size_;
  }
  void pop_back() noexcept {
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
  void push_front(const_reference value) {
    auto *node = create_node(value);
    node->ptr()->link_before(head_);
    head_ = node;
    ++size_;
  }
  void pop_front() noexcept {
    if (empty()) {
      return;
    }
    auto *node = head_;
    head_ = static_cast<list_node*>(head_->ptr()->next);
    node->unlink();
    destroy_node(node);
    --size_;
  }
  void swap(list &other) {
    if (&other == this) {
      return;
    }
    std::swap(this->head_, other.head_);
    std::swap(this->tail_, other.tail_);
    std::swap(this->size_, other.size_);
  }
  void merge(list &other) {
    if (&other == this) {
      return;
    }
    auto iter1 = this->begin();
    auto iter2 = other.begin();
    while (iter1 != this->end()
        && iter2 != other.end()) {
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

  void splice(const_iterator pos, list &other) {
    splice(pos, other, other.begin(), other.end());
  }
  void splice(const_iterator pos, list &other,
              const_iterator iter) {
    splice(pos, other, iter, ++iter);
  }
  void splice(const_iterator pos, list &other,
    const_iterator first, const_iterator last) {
    if (first == last) {
      return;
    }

    auto diff = std::distance(first, last);
    list_node* node = pos.current_;

    list_details::list_node_base::unlink_group(first.current_, last.current_->prev);
    node->ptr()->link_group_before(first.current_, last.current_->prev);

    other.size_ -= diff;
    size_ += diff;
  }

  void reverse() {
    if (empty()) {
      return;
    }
    iterator iter = begin();
    for (int i = 0; i < size(); ++i) {
      iter.current_->reverse();
      --iter;
    }
    iter.current_->reverse();
    head_ = static_cast<list_node*>(iter.current_->next);
  }
  void unique() {
    if (empty()) {
      return;
    }
    for (auto current = begin(), next = ++begin(); next != end();) {
      if (*current == *next) {
        next = erase(next);  // erase returns iterator to next element
      } else {
        current = next;
        ++next;
      }
    }
  }
  void sort() {
    quick_sort(this->begin(), --this->end());
  }

 private:
  list_node *head_, *tail_;
  size_type size_;
  allocator_type allocator_;

  list_node* create_node(const_reference value) {
    list_node* new_node = std::allocator_traits<allocator_type>::allocate(allocator_, 1);
    try {
      std::allocator_traits<allocator_type>::construct(allocator_, new_node, value);
    } catch (...) {
      std::allocator_traits<allocator_type>::deallocate(allocator_, new_node, 1);
      throw;
    }
    return new_node;
  }

  void destroy_node(list_node* node, bool to_destroy_before = true) {
    if (to_destroy_before) {
      std::allocator_traits<allocator_type>::destroy(allocator_, node);
    }
    std::allocator_traits<allocator_type>::deallocate(allocator_, node, 1);
  }

  void init() noexcept {
    head_ = tail_ = std::allocator_traits<allocator_type>::allocate(allocator_, 1);
    memset(&(head_->data), 0, sizeof(value_type));
    tail_->ptr()->next = tail_->ptr()->prev = tail_;
    size_ = 0;
  }

  static iterator patrition(iterator low, iterator high) {
    const auto& pivot_value = *high;
    iterator iter = low;
  
    for (iterator j = low; j != high; ++j) {
      if (*j < pivot_value) {
        if (iter != j) {
          std::swap(*iter, *j);
        }
        ++iter;
      }
    }
  
    std::swap(*iter, *high);
    return iter;
  }

  void quick_sort(iterator low, iterator high) {
    if (low == high || low == high + 1 ||
      low == this->end() || high == this->end()) {
      return;
    }
    auto pivot = patrition(low, high);
    quick_sort(low, pivot - 1);
    quick_sort(pivot + 1, high);
  }
};
} // namespace s21

// #include "s21_list.tpp"

#endif // S21_LIST_H_