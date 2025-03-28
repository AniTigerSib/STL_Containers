#ifndef S21_LIST_H_
#define S21_LIST_H_

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <type_traits>

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
    // void link_after(list_node_base *node) {
    //   this->next = node->next;
    //   this->prev = node;
    //   node->next->prev = this;
    //   node->next = this;
    // }
    void link_before(list_node_base *node) {
      this->prev = node->prev;
      this->next = node;
      node->prev->next = this;
      node->prev = this;
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
  struct list_node : protected list_node_base {
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
    explicit list_iterator(list_node_base *node) : current_(node) {}

    reference operator*() noexcept {
      return static_cast<list_node>(current_).data;
    }

    pointer operator->() noexcept {
      return &static_cast<list_node>(current_).data;
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
  
  list() { init(); };
  explicit list(const Allocator& alloc) : allocator_(alloc) { init(); }
  explicit list(size_type count, const_reference value = value_type(), const Allocator& alloc = Allocator());
  list(std::initializer_list<value_type> const &items, const Allocator& alloc = Allocator());
  list(const list &other);
  list(list &&other) noexcept;
  ~list() noexcept;
  list &operator=(list &&other);

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

  void clear() noexcept;
  iterator insert(iterator pos, const_reference value);
  void erase(iterator pos) noexcept;
  void push_back(const_reference value);
  void pop_back() noexcept;
  void push_front(const_reference value);
  void pop_front() noexcept;
  void swap(list &other);
  void merge(list &other);
  void splice(const_iterator pos, list &other);
  void reverse();
  void unique();
  void sort();

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
    tail_->ptr()->next = tail_->ptr()->prev = head_;
    size_ = 0;
  }
};
} // namespace s21
  
#endif // S21_LIST_H_