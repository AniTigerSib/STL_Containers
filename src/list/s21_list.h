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
template <class T, class Allocator = std::allocator<T>>
class list {

  // NODES //////////////////////

 public:
  struct list_node_base {
    list_node_base *prev;
    list_node_base *next;

    list_node_base(list_node_base *prv, list_node_base *nxt) : prev(prv), next(nxt) {}

    void unlink() {
      this->next->prev = this->prev;
      this->prev->next = this->next;
      init();
    }
    void link_after(list_node_base *node) {
      this->next = node->next;
      this->prev = node;
      node->next->prev = this;
      node->next = this;
    }
    void link_before(list_node_base *node) {
      this->prev = node->prev;
      this->next = node;
      node->prev->next = this;
      node->prev = this;
    }
    void link_group_before(list_node_base *first, list_node_base *last) {
      this->prev->next = first;
      first->prev = this->prev;
      this->prev = last;
      last->next = this;
    }
    static void unlink_group(list_node_base *first, list_node_base *last) noexcept {
      first->prev->next = last->next;
      last->next->prev = first->prev;
      last->next = first;
      first->prev = last;
    }
    static void swap(list_node_base *lhs, list_node_base *rhs) noexcept {
      std::swap(lhs->prev, rhs->prev);
      std::swap(lhs->next, rhs->next);
    }
    void reverse() {
      std::swap(this->next, this->prev);
    }

   private:
    void init() {
      this->next = this;
      this->prev = this;
    }
  };

 private:
  struct list_node : public list_node_base {
    T data;

    template<typename... Args>
    explicit list_node(list_node *prv, list_node *nxt, Args&&... args)
      : list_node_base(prv, nxt), data(std::forward<Args>(args)...) {}
    
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
    explicit list_iterator(list_node_base *node) : current_(node) {}

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
        current_ = current_->prev;
      }
      return res;
    }

   private:
    list_node_base* current_;
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
    explicit list_const_iterator(const list_node_base *node) noexcept : current_(node) {}

    reference operator*() const noexcept {
      return static_cast<const list_node>(current_).data;
    }

    pointer operator->() const noexcept {
      return &static_cast<const list_node>(current_).data;
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
    const list_node_base *current_;
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
    this->allocator_(std::move(other.allocator_));
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
  list &operator=(list &&other) {
    // TODO(michael): To test this
    this->clear();
    this->allocator_(std::move(other.allocator_));
    this->splice(this->begin(), other);
  }

  const_reference front() const noexcept { return *begin(); }
  const_reference back() const noexcept { return *end(); }

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
    auto *node = create_node<T>(value);
    node->ptr()->link_before(pos.current_);
    ++size_;
    iterator iter(node);
    return iter;
  }
  void erase(iterator pos) noexcept {
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
  void push_back(const_reference value) {
    auto *node = create_node<T>(value);
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
    auto *node = create_node<T>(value);
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

    list_node_base::unlink_group(first.current_, last.current_->prev);
    node->ptr()->link_group_before(first.current_, last.current_->prev);

    other.size_ -= diff;
    size_ += diff;
  }

  void reverse() {
    iterator iter = begin();
    for (int i = 0; i < size(); ++i) {
      iter.current_->reverse();
      --iter;
    }
    iter.current_->reverse();
    head_ = static_cast<list_node*>(iter.current_->next);
  }
  void unique() {
    for (auto iter = begin(); iter != end();) {
      if (iter != begin() && *iter == *(--iter)) {
        erase(iter++);
      } else {
        ++iter;
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

  template<typename... Args>
  list_node* create_node(const_reference value) {
    list_node* new_node = std::allocator_traits<allocator_type>::allocate(allocator_, 1);
    try {
      std::allocator_traits<allocator_type>::construct(allocator_, new_node, head_, head_, value);
    } catch (...) {
      std::allocator_traits<allocator_type>::deallocate(allocator_, new_node, 1);
      throw;
    }
    return new_node;
  }

  void destroy_node(list_node* node) {
    std::allocator_traits<allocator_type>::destroy(allocator_, node);
    std::allocator_traits<allocator_type>::deallocate(allocator_, node, 1);
  }

  void init() noexcept {
    head_ = tail_ = std::allocator_traits<allocator_type>::allocate(allocator_, 1);
    memset(&(head_->data), 0, sizeof(value_type));
    tail_->ptr()->next = tail_->ptr()->prev = tail_;
    size_ = 0;
  }

  static iterator patrition(iterator low, iterator high) {
    std::cout << "partition\n";
    iterator pivot = high;
    iterator base = --low;
    for (auto it = low; it != high; ++it) {
      if (*it < *pivot) {
        ++base;
        if (it != base) {
          std::swap(*it, *base);
        }
      }
    }
    ++base;
    std::swap(*pivot, *base);
    return base;
  }

  void quick_sort(iterator low, iterator high) {
    std::cout << "qsort: " << *low << " " << *high << "\n";
    if (low == high || low == high + 1 ||
      low == this->end() || high == this->end()) {
      return;
    }
    std::cout << "qsort checked";
    auto pivot = patrition(low, high);
    quick_sort(low, pivot - 1);
    quick_sort(pivot + 1, high);
  }
};
} // namespace s21

// #include "s21_list.tpp"

#endif // S21_LIST_H_