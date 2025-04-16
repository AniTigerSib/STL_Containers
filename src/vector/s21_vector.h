#ifndef S21_VECTOR_H_
#define S21_VECTOR_H_

#include <algorithm>
#include <deque>
#include <iterator>
#include <memory>

namespace s21 {
template <typename T, typename Allocator>
struct vector_base {
  using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<T>;
  using pointer = typename std::allocator_traits<allocator_type>::pointer;

  struct vector_data {
    pointer start;
    pointer finish;
    pointer end_of_storage;

    vector_data() noexcept : start(), finish(), end_of_storage() {}
    vector_data(vector_data&& other_data) noexcept : start(other_data.start),
      finish(other_data.finish), end_of_storage(other_data.end_of_storage) {
      other_data.start = other_data.finish = other_data.end_of_storage = pointer();
    }
    
    void copy(vector_data& data) noexcept {
      start = data.start;
      finish = data.finish;
      end_of_storage = data.end_of_storage;
    }

    void swap(vector_data& data) noexcept {
      vector_data tmp;
      tmp.copy(*this);
      this->copy(data);
      data.copy(tmp);
    }
  };

  // vector_base() = default;
  explicit vector_base(const allocator_type& alloc = allocator_type()) : allocator_(alloc), data_() {}
  explicit vector_base(size_t size, const allocator_type& alloc = allocator_type()) : allocator_(alloc), data_() { create_storage(size); }
  vector_base(vector_base&& other) = default;
  ~vector_base() { deallocate(data_.start, data_.end_of_storage - data_.start); }

 protected:
  vector_data data_;
  allocator_type allocator_;

  pointer allocate(size_t size) noexcept {
    return size != 0 ? std::allocator_traits<allocator_type>::
      allocate(allocator_, size) : pointer();
  }

  void deallocate(pointer ptr, size_t size) noexcept {
    if (ptr) {
      std::allocator_traits<allocator_type>::
        deallocate(allocator_, ptr, size);
    }
  }

  allocator_type get_allocator() const { return allocator_; }

  void create_storage(size_t size) noexcept {
    data_.start = allocate(size);
    data_.finish = data_.start;
    data_.end_of_storage = data_.start + size;
  }
};

template <typename T, typename Allocator = std::allocator<T>>
class vector : protected vector_base<T, Allocator> {
 private:
  using Base = vector_base<T, Allocator>;

 public:
  struct vector_iterator {
    using Self = vector_iterator;

    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using pointer = T*;
    using reference = T&;

    vector_iterator() noexcept : ptr_(nullptr) {}
    vector_iterator(Self& other) noexcept : ptr_(other.ptr_) {}
    explicit vector_iterator(pointer ptr) noexcept : ptr_(ptr) {}

    reference operator*() const noexcept { return *ptr_; }
    pointer operator->() const noexcept { return ptr_; }

    bool operator==(const Self& rhs) const noexcept { return this->ptr_ == rhs.ptr_; }
    bool operator!=(const Self& rhs) const noexcept { return this->ptr_ != rhs.ptr_; }

    bool operator<(const Self& rhs) const noexcept { return this->ptr_ < rhs.ptr_; }
    bool operator>(const Self& rhs) const noexcept { return this->ptr_ > rhs.ptr_; }
    bool operator<=(const Self& rhs) const noexcept { return this->ptr_ <= rhs.ptr_; }
    bool operator>=(const Self& rhs) const noexcept { return this->ptr_ >= rhs.ptr_; }

    Self &operator++() noexcept { ++ptr_; return *this; }
    Self operator++(int) noexcept {
      vector_iterator tmp(*this);
      ++ptr_;
      return tmp;
    }

    Self &operator--() noexcept { --ptr_; return *this; }
    Self operator--(int) noexcept {
      vector_iterator tmp(*this);
      --ptr_;
      return tmp;
    }

    Self operator+(difference_type diff) noexcept { return vector_iterator(ptr_ + diff); }
    difference_type operator-(const Self& rhs) const noexcept { return this->ptr_ - rhs.ptr_; }

    Self &operator+=(difference_type diff) noexcept {
      ptr_ += diff;
      return *this;
    }

    Self operator-(difference_type diff) noexcept { return vector_iterator(ptr_ - diff); }
    difference_type operator-(const Self& rhs) noexcept { return rhs.ptr_ - this->ptr_; }

    Self &operator-=(difference_type diff) noexcept {
      ptr_ -= diff;
      return *this;
    }

    reference operator[](difference_type diff) noexcept { return ptr_[diff]; }

   private:
    pointer ptr_;
    friend class vector<T, Allocator>;
  };

  struct vector_const_iterator {
    using Self = vector_const_iterator;

    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using pointer = const T*;
    using reference = const T&;

    vector_const_iterator() noexcept : kptr(nullptr) {}
    vector_const_iterator(Self& other) noexcept : kptr(other.kptr) {}
    explicit vector_const_iterator(pointer ptr) noexcept : kptr(ptr) {}

    reference operator*() const noexcept { return *kptr; }
    pointer operator->() const noexcept { return kptr; }

    bool operator==(const Self& rhs) const noexcept { return this->kptr == rhs.kptr; }
    bool operator!=(const Self& rhs) const noexcept { return this->kptr != rhs.kptr; }

    bool operator<(const Self& rhs) const noexcept { return this->kptr < rhs.kptr; }
    bool operator>(const Self& rhs) const noexcept { return this->kptr > rhs.kptr; }
    bool operator<=(const Self& rhs) const noexcept { return this->kptr <= rhs.kptr; }
    bool operator>=(const Self& rhs) const noexcept { return this->kptr >= rhs.kptr; }

    Self &operator++() noexcept { ++kptr; return *this; }
    Self operator++(int) noexcept {
      vector_iterator tmp(*this);
      ++kptr;
      return tmp;
    }

    Self &operator--() noexcept { --kptr; return *this; }
    Self operator--(int) noexcept {
      vector_iterator tmp(*this);
      --kptr;
      return tmp;
    }

    Self operator+(difference_type diff) noexcept { return vector_iterator(kptr + diff); }
    difference_type operator-(const Self& rhs) const noexcept { return this->kptr - rhs.kptr; }

    Self &operator+=(difference_type diff) noexcept {
      kptr += diff;
      return *this;
    }

    Self operator-(difference_type diff) noexcept { return vector_iterator(kptr - diff); }
    difference_type operator-(const Self& rhs) noexcept { return rhs.kptr - this->kptr; }

    Self &operator-=(difference_type diff) noexcept {
      kptr -= diff;
      return *this;
    }

    reference operator[](difference_type diff) noexcept { return kptr[diff]; }

   private:
    const pointer kptr;
    friend class vector<T, Allocator>;
  };

  using value_type = T;
  using allocator_type = Allocator;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using reference = T&;
  using const_reference = const T&;
  using pointer = typename std::allocator_traits<allocator_type>::pointer;
  using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;

  // Iterators:
  using iterator = vector_iterator;
  using const_iterator = vector_const_iterator;

  vector() noexcept(noexcept(allocator_type())) : vector(allocator_type()) {}

  explicit constexpr vector(const allocator_type& alloc) noexcept : Base(alloc) {}

  explicit vector(size_type count, const allocator_type& alloc = allocator_type())
    : Base(count, alloc) {
    std::uninitialized_default_construct(this->data_.start, this->data_.end_of_storage);
    this->data_.finish = this->data_.end_of_storage;
  }

  constexpr vector(size_type count, const_reference value,
    const allocator_type& alloc = allocator_type()) : Base(count, alloc) {
    std::uninitialized_fill_n(this->data_.start, count, value);
    this->data_.finish = this->data_.end_of_storage;
  }

  vector(std::initializer_list<value_type> const &items,
    const Allocator& alloc = Allocator()) : Base(items.size(), alloc) {
    std::uninitialized_copy(items.begin(), items.end(), this->data_.start);
    this->data_.finish = this->data_.end_of_storage;
  }

  constexpr vector(const vector& other) : vector(other, other.alloc) {}

  constexpr vector(const vector& other, const Allocator& alloc)
    : Base(other.size(), alloc) {
    std::uninitialized_copy(other.data_.start, other.data_.finish, this->data_.start);
    this->data_.finish = this->data_.end_of_storage;
  }

  constexpr vector(vector&& other) noexcept : Base(std::move(other)) {}

  vector(vector&& other, const Allocator& alloc) : Base(alloc) {
    if (std::allocator_traits<allocator_type>::is_always_equal::value ||
      alloc == other.get_allocator()) {
      this->data_ = std::move(other.data_);
    } else {
      this->create_storage(other.size());
      std::uninitialized_move(other.data_.start, other.data_.finish, this->data_.start);
      this->data_.finish = this.data.end_of_storage;
    }
  }

  constexpr ~vector() {
    std::destroy(this->data_.start, this->data_.finish);
  }

  vector& operator=(vector&& other)
    noexcept(std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value
    || std::allocator_traits<Allocator>::is_always_equal::value) {
    if (this == &other) {
      return *this;
    }
    std::destroy(this->data_.start, this->data_.finish);
    if constexpr (std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value
    || std::allocator_traits<Allocator>::is_always_equal::value) {
      this->deallocate(this->data_.start, this->data_.end_of_storage - this->data_.start);
      this->allocator_ = std::move(other.allocator_);
      this->data_.copy(other.data_);
    } else if (this->allocator_ == other.allocator_) {
      this->deallocate(this->data_.start, this->data_.end_of_storage - this->data_.start);
      this->data_.copy(other.data_);
    } else {
      this->reserve(other.size());
      this->data_.finish = std::uninitialized_move(other.data_.start, other.data_.finish, this->data_.start);
    }
    other.data_.start = other.data_.finish = other.data_.end_of_storage = pointer();
    return *this;
  }

  constexpr reference at(size_type pos) {
    if (pos >= this->size()) {
      throw std::out_of_range("vector::at: index out of range");
    }
    return this->data_.start[pos];
  }
  constexpr const_reference at(size_type pos) const {
    if (pos >= this->size()) {
      throw std::out_of_range("vector::at: index out of range");
    }
    return this->data_.start[pos];
  }

  constexpr reference operator[](size_type pos) {
    return this->data_.start[pos];
  }
  constexpr const_reference operator[](size_type pos) const {
    return this->data_.start[pos];
  }

  constexpr reference front() {
    return *begin();
  }
  constexpr const_reference front() const {
    return *cbegin();
  }

  constexpr reference back() {
    return *(end() - 1);
  }
  constexpr const_reference back() const {
    return *(cend() - 1);
  }

  constexpr pointer data() {
    return this->data_.start;
  }
  constexpr const_pointer data() const {
    return this->data_.start;
  }

  constexpr iterator begin() {
    return iterator(this->data_.start);
  }
  constexpr const_iterator begin() const {
    return const_iterator(this->data_.start);
  }
  constexpr const_iterator cbegin() const noexcept {
    return const_iterator(this->data_.start);
  }
  constexpr iterator end() {
    return iterator(this->data_.finish);
  }
  constexpr const_iterator end() const {
    return const_iterator(this->data_.finish);
  }
  constexpr const_iterator cend() const noexcept {
    return const_iterator(this->data_.finish);
  }
  
  [[nodiscard]] constexpr bool empty() const noexcept {
    return this->data_.finish == this->data_.start;
  }
  [[nodiscard]] constexpr size_type size() const noexcept {
    return static_cast<size_type>(this->data_.finish - this->data_.start);
  }
  [[nodiscard]] constexpr size_type max_size() const noexcept {
    return std::numeric_limits<value_type>::max();
  }
  constexpr void reserve(size_type size) {
    if (size > this->capacity()) {
      pointer new_start = this->allocate(size);
      pointer new_finish = new_start;

      try {
        new_finish = std::uninitialized_move(this->data_.start, this->data_.finish, new_start);
      } catch (...) {
        this->deallocate(new_start, size);
        throw;
      }

      std::destroy(this->data_.start, this->data_.finish);
      this->deallocate(this->data_.start, this->data_.end_of_storage - this->data_.start);

      this->data_.start = new_start;
      this->data_.finish = new_finish;
      this->data_.end_of_storage = this->data_.start + size;
    }
  }
  [[nodiscard]] constexpr size_type capacity() const noexcept {
    return static_cast<size_type>(this->data_.end_of_storage - this->data_.start);
  }
  constexpr void shrink_to_fit() {
    if (this->size() < this->capacity()) {
      if (this->size() == 0) {
        this->deallocate(this->data_.start, this->data_.end_of_storage - this->data_.start);
        this->data_.start = this->data_.finish = this->data_.end_of_storage = pointer();
      } else {
        size_type size = this->size();
        pointer new_start = this->allocate(size);
        pointer new_finish = new_start;

        try {
          new_finish = std::uninitialized_move(this->data_.start, this->data_.finish, new_start);
        } catch (...) {
          this->deallocate(new_start, size);
          throw;
        }

        std::destroy(this->data_.start, this->data_.finish);
        this->deallocate(this->data_.start, this->data_.end_of_storage - this->data_.start);
        
        this->data_.start = new_start;
        this->data_.finish = new_finish;
        this->data_.end_of_storage = this->data_.start + size;
      }
    }
  }

  constexpr void clear() noexcept {
    std::destroy(this->data_.start, this->data_.finish);
    this->data_.finish = this->data_.start;
  }
  constexpr iterator insert(iterator pos, const_reference value) {
    auto size = static_cast<size_type>(this->data_.finish - this->data_.start);
    size_type index = pos.ptr_ - this->data_.start;

    if (size == this->capacity()) {
      size_type new_capacity = size == 0 ? 1 : size * 2;
      this->reserve(new_capacity);
    }

    iterator actual_pos(this->data_.start + index);
    
    if (index < size) {
      std::construct_at(this->data_.finish, std::move(*(this->data_.finish - 1)));
      std::move_backward(actual_pos.ptr_, this->data_.finish - 1, this->data_.finish);
      *actual_pos = value;
    } else {
      std::construct_at(this->data_.finish, value);
    }

    ++this->data_.finish;
    return actual_pos;
  }
  constexpr iterator erase(const_iterator pos) {
    if (pos.kptr < this->data_.start || pos.kptr >= this->data_.finish) {
      throw std::out_of_range("vector::erase: index out of range");
    }

    auto ptr = const_cast<pointer>(pos.kptr);
    std::move(ptr + 1, this->data_.finish, ptr);
    --this->data_.finish;
    std::destroy_at(this->data_.finish);
    return iterator(ptr);
  }
  constexpr iterator erase(const_iterator first, const_iterator last) {
    if (first.kptr < this->data_.start || last.kptr > this->data_.finish || first.kptr > last.kptr) {
      throw std::out_of_range("vector::erase: index out of range");
    }

    auto p_first = const_cast<pointer>(first.kptr);
    auto p_last = const_cast<pointer>(last.kptr);
    auto count = static_cast<size_type>(p_last - p_first);

    if (count > 0) {
      std::move(p_last, this->data_.finish, p_first);
      pointer new_finish = this->data_.finish - count;
      std::destroy(new_finish, this->data_.finish);
      this->data_.finish = new_finish;
    }

    return iterator(p_first);
  }
  constexpr void push_back(const_reference value) {
    if (this->size() == this->capacity()) {
      this->reserve(this->capacity() == 0 ? 1 : this->capacity() * 2);
    }

    std::construct_at(this->data_.finish, value);
    ++this->data_.finish;
  }
  constexpr void pop_back() {
    if (this->size() > 0) {
      --this->data_.finish;
      std::destroy_at(this->data_.finish);
    }
  }
  constexpr void swap(vector& other)
    noexcept(std::allocator_traits<allocator_type>::propagate_on_container_swap::value
    || std::allocator_traits<allocator_type>::is_always_equal::value) {
    if (this != &other) {
      this->data_.swap(other.data_);
    }
  }
};
} // namespace s21

#endif // S21_VECTOR_H_