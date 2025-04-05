#ifndef S21_VECTOR_H_
#define S21_VECTOR_H_

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
    vector_data(vector_data&& data) noexcept : start(data.start),
      finish(data.finish), end_of_storage(data.end_of_storage) {
      data.start = data.finish = data.end_of_storage = pointer();
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

 public:
  vector_base() = default;
  explicit vector_base(size_t size) : data() { create_storage(size); }
  vector_base(vector_base&& other) = default;
  ~vector_base() { deallocate(data.start, data.end_of_storage - data.start); }

  vector_data data;

  pointer allocate(size_t size) noexcept {
    return size != 0 ? std::allocator_traits<allocator_type>::
      allocate(allocator_type(), size) : pointer();
  }

  void deallocate(pointer ptr, size_t size) noexcept {
    if (ptr) {
      std::allocator_traits<allocator_type>::
        deallocate(allocator_type(), ptr, size);
    }
  }
 protected:
  void create_storage(size_t size) noexcept {
    data.start = allocate(size);
    data.finish = data.start;
    data.end_of_storage = data.start + size;
  }
};

template <typename T, typename Allocator = std::allocator<T>>
class vector : protected vector_base<T, Allocator> {
 private:
  using Base = vector_base<T, Allocator>;
  using Base::data;
  using Base::allocate;
  using Base::deallocate;
  using Base::create_storage;

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

  vector() noexcept(noexcept(Allocator())) : vector(Allocator()) {}
  explicit constexpr vector(const Allocator& alloc) noexcept;
  explicit vector(size_type count, const Allocator& alloc = Allocator());
  explicit vector(size_type count, const_reference value = value_type(),
                  const Allocator& alloc = Allocator());
  vector(std::initializer_list<value_type> const &items, const Allocator& alloc = Allocator());
  constexpr vector(const vector& other);
  constexpr vector(const vector& other, const Allocator& alloc);
  constexpr vector(vector&& other) noexcept;
  vector(vector&& other, const Allocator& alloc);
  constexpr ~vector();

  vector& operator=(vector&& other)
    noexcept(std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value
    || std::allocator_traits<Allocator>::is_always_equal::value);

  constexpr reference at(size_type pos);
  constexpr const_reference at(size_type pos) const;

  constexpr reference operator[](size_type pos);
  constexpr const_reference operator[](size_type pos) const;

  constexpr reference front();
  constexpr const_reference front() const;

  constexpr reference back();
  constexpr const_reference back() const;

  constexpr pointer data();
  constexpr const_pointer data() const;

  constexpr iterator begin();
  constexpr iterator end();
  constexpr const_iterator begin() const;
  constexpr const_iterator end() const;
  constexpr const_iterator cbegin() const noexcept;
  constexpr const_iterator cend() const noexcept;
  
  [[nodiscard]] constexpr bool empty() const noexcept;
};
} // namespace s21

#endif