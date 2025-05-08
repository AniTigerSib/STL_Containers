#ifndef S21_ARRAY_H_
#define S21_ARRAY_H_

#include <algorithm>
#include <stdexcept>

namespace s21 {
template <typename T, std::size_t N>
struct array_inner {
  using array = T[N];

  static constexpr T& ref(const array& arr, std::size_t idx) noexcept {
    return const_cast<T&>(arr[idx]);
  }

  static constexpr T* ptr(const array& arr) noexcept {
    return const_cast<T*>(arr);
  }
};

template <typename T>
struct array_inner<T, 0> {
  struct array { };

  static constexpr T& ref(const array&, std::size_t) noexcept {
    return *const_cast<T*>(nullptr);
  }

  static constexpr T* ptr(const array&) noexcept {
    return nullptr;
  }
};

template <typename T, std::size_t N>
struct array {
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using pointer = T*;
  using const_pointer = const T*;
  using size_type = std::size_t;

  // Iterators
  using iterator = T*;
  using const_iterator = const T*;

  using arr_inner = array_inner<T, N>;

  constexpr array() = default;
  constexpr array(const array&) = default;
  constexpr array(array&&) = default;
  constexpr array(std::initializer_list<value_type> const &items) : elements_() {
    if (items.size() > size()) {
      throw std::out_of_range("array::array: initializer list size is greater than array size");
    }
    std::copy(items.begin(), items.end(), elements_);
  }
  ~array() noexcept = default;
  constexpr array& operator=(const array&) = default;
  constexpr array& operator=(const array&& other) noexcept(std::is_nothrow_move_constructible_v<T>) {
    std::move(other.begin(), other.end(), begin());
    return *this;
  }

  constexpr reference at(size_type pos) {
    if (pos >= size()) {
      throw std::out_of_range("array::at: index out of range");
    }
    return elements_[pos];
  }
  constexpr reference operator[](size_type pos) {
    return elements_[pos];
  }
  constexpr const_reference front() {
    return elements_[0];
  }
  constexpr const_reference back() {
    return elements_[N - 1];
  }
  constexpr pointer data() noexcept {
    return arr_inner::ptr(elements_);
  }
  constexpr const_pointer data() const noexcept {
    return arr_inner::ptr(elements_);
  }

  iterator begin() noexcept {
    return iterator(data());
  }
  const_iterator begin() const noexcept {
    return const_iterator(data());
  }
  const_iterator cbegin() const noexcept {
    return const_iterator(data());
  }

  iterator end() noexcept {
    return iterator(data() + N);
  }
  const_iterator end() const noexcept {
    return const_iterator(data() + N);
  }
  const_iterator cend() const noexcept {
    return const_iterator(data() + N);
  }

  [[nodiscard]] bool empty() const noexcept {
    return size() == 0;
  }
  [[nodiscard]] constexpr size_type size() const noexcept {
    return N;
  }
  [[nodiscard]] constexpr size_type max_size() const noexcept {
    return N;
  }

  constexpr void swap(array& other)
  noexcept(std::is_nothrow_swappable_v<T>) {
    std::swap_ranges(this->begin(), this->end(), other.begin());
  }

  constexpr void fill(const_reference value) {
    std::fill_n(data(), size(), value);
  }

  private:
    typename arr_inner::array elements_;
};
} // namespace s21

#endif // S21_ARRAY_H_