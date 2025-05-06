#ifndef S21_LIST_H_
#define S21_LIST_H_

#include <initializer_list>
#include <iterator>

namespace s21 {
namespace list_details {
struct list_node_base {
  list_node_base *prev;
  list_node_base *next;

  list_node_base() {
    init();
  }

  void unlink() {
    next->prev = prev;
    prev->next = next;
    init();
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

#ifdef TESTS
 public:
#else
 private:
#endif
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
    using Node = typename list<T, Allocator>::list_node;

    using difference_type = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    
    list_iterator() noexcept : current_(nullptr) {}
    list_iterator(Self &other) noexcept : current_(other.current_) {}
    explicit list_iterator(list_details::list_node_base *node) : current_(node) {}

    list_iterator& operator=(list_iterator &other) noexcept {
      this->current_ = other.current_;
      return *this;
    }
    list_iterator& operator=(list_iterator &&other) noexcept {
      this->current_ = other.current_;
      return *this;
    }

    reference operator*() noexcept {
      return static_cast<Node*>(current_)->data;
    }

    pointer operator->() noexcept {
      return &(static_cast<Node*>(current_)->data);
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

#ifdef TESTS
   public:
#else
   private:
#endif
    list_details::list_node_base* current_;
    friend class list;
  };

  struct list_const_iterator {
    using Self = list_const_iterator;
    using Node = const typename list<T, Allocator>::list_node;
  
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using pointer = const T*;
    using reference = const T&;
  
    list_const_iterator() noexcept : current_(nullptr) {}
    list_const_iterator(Self &other) noexcept : current_(other.current_) {}
    explicit list_const_iterator(const list_details::list_node_base *node) noexcept : current_(node) {}

    list_const_iterator& operator=(list_const_iterator &&other) noexcept {
      this->current_ = other.current_;
      return *this;
    }
  
    reference operator*() const noexcept {
      return static_cast<const Node*>(current_)->data;
    }
  
    pointer operator->() const noexcept {
      return &static_cast<const Node*>(current_)->data;
    }
  
    bool operator==(const Self &other) const noexcept {
      return current_ == other.current_;
    }
    bool operator!=(const Self &other) const noexcept {
      return current_ != other.current_;
    }
  
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

    list_iterator non_const() const noexcept {
      return list_iterator(const_cast<list_details::list_node_base*>(current_));
    }
  
#ifdef TESTS
   public:
#else
   private:
#endif
    const list_details::list_node_base *current_;
    friend class list;
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
    if (!other.empty()) {
      this->splice(this->cbegin(), other);
    }
  }
  list(list &&other, const Allocator& alloc) : list(alloc) {
    if (!other.empty()) {
      this->splice(this->cbegin(), other);
    }
  }
  ~list() noexcept {
    clear();
    destroy_node(head_);
  }
  list& operator=(const list& other) {
    if (this != &other) {
      list tmp(other);
      swap(tmp);
    }
    return *this;
  }
  constexpr list &operator=(list &&other) {
    this->clear();
    if (static_cast<bool>(std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value)) {
      auto node = static_cast<list_node*>(this->end().current_);
      destroy_node(node, false);
      this->allocator_ = std::move(other.allocator_);
      init();
    }
    this->splice(this->cbegin(), other);
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
  iterator insert(iterator pos, const_reference value) {
    auto *node = create_node(value);
    node->ptr()->link_before(pos.current_);
    ++size_;
    if (pos == begin()) {
      head_ = node;
    }
    iterator iter(node);
    return iter;
  }

  template<typename... Args> requires (std::same_as<value_type, Args> && ...)
  iterator insert_many(const_iterator pos, Args&& ...args) {
    iterator iter = pos.non_const();
    iterator res = (insert(iter, std::forward<Args>(args)), ...);
    return res;
  }

  iterator erase(iterator pos) noexcept {
    if (pos == end()) {
      return end();
    }
    if (pos == begin()) {
      head_ = static_cast<list_node*>(head_->ptr()->next);
    }
    iterator to_return = pos;
    ++to_return;
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
    if (size_ == 0) {
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
    auto iter1 = this->cbegin();
    auto iter2 = other.cbegin();
    while (iter1 != this->cend()
        && iter2 != other.cend()) {
      if (*iter1 > *iter2) {
        this->splice(iter1, other, iter2++);
      } else {
        ++iter1;
      }
    }
    if (iter2 != other.cend()) {
      this->splice(this->cend(), other, iter2, other.cend());
    }
    other.clear();
  }

  void splice(const_iterator pos, list &other) {
    splice(pos, other, other.cbegin(), other.cend());
  }
  void splice(const_iterator pos, list &other,
              const_iterator iter) {
    auto last = iter;
    ++last;
    splice(pos, other, iter, last);
  }
  void splice(const_iterator pos, list &other,
    const_iterator first, const_iterator last) {
    if (first == last) {
      return;
    }

    auto diff = std::distance(first, last);
    auto* node = static_cast<list_node*>(const_cast<list_details::list_node_base*>(pos.current_));
    auto* last_to_insert = const_cast<list_details::list_node_base*>(last.current_->prev);

    if (other.head_ == static_cast<list_node*>(const_cast<list_details::list_node_base*>(first.current_))) {
      other.head_ = static_cast<list_node*>(const_cast<list_details::list_node_base*>(last.current_));
    }
    list_details::list_node_base::unlink_group(
      const_cast<list_details::list_node_base*>(first.current_),
      const_cast<list_details::list_node_base*>(last.current_->prev));
    
    node->link_group_before(
      const_cast<list_details::list_node_base*>(first.current_), last_to_insert);

    if (node == this->head_) {
      this->head_ = static_cast<list_node*>(const_cast<list_details::list_node_base*>(first.current_));
    }
    other.size_ -= diff;
    size_ += diff;
  }

  void reverse() {
    if (empty()) {
      return;
    }
    iterator iter = begin();
    for (size_type i = 0; i < size(); ++i) {
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
    auto current = begin();
    auto next = begin();
    ++next;
    for (; next != end();) {
      if (*current == *next) {
        next = erase(next);  // erase returns iterator to next element
      } else {
        current = next;
        ++next;
      }
    }
  }
  void sort() {
    if (size() <= 1) return;
  
    list left(allocator_);
    list right(allocator_);

    size_type half_size = size() / 2;
    auto mid = cbegin();
    std::advance(mid, half_size);

    left.splice(left.cbegin(), *this, cbegin(), mid);
    right.splice(right.cbegin(), *this);

    left.sort();
    right.sort();

    left.merge(right);
    *this = std::move(left);
  }

#ifdef TESTS
 public:
#else
 private:
#endif
  list_node *head_, *tail_;
  size_type size_;
  allocator_type allocator_;
  friend struct list_iterator;

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
    // memset(&(head_->data), 0, sizeof(value_type));
    tail_->ptr()->next = tail_->ptr()->prev = tail_;
    size_ = 0;
  }
};
} // namespace s21

#endif // S21_LIST_H_