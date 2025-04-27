#ifndef S21_STACK_H_
#define S21_STACK_H_

#include <deque>

namespace s21 {
template <typename T, typename Container = std::deque<T>>
class stack {
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

 protected:
  Container container_;

 public:
  stack() : stack(Container()) {}
  explicit stack(const Container& container) {
    container_ = container;
  }
  stack(std::initializer_list<value_type> const& items) {
    container_ = std::move(Container(items));
  }
  stack(const stack& stack) {
    container_ = stack.container_;
  }
  stack(stack&& stack) {
    container_ = std::move(stack.container_);
  }
  ~stack() = default;

  stack& operator=(stack&& other) {
    container_ = std::move(other.container_);
    return *this;
  }

  template <typename Alloc>
  explicit stack(const Alloc& alloc) {
    container_ = std::move(Container(alloc));
  }

  reference top() {
    return container_.top();
  }
  const_reference top() const {
    return container_.top();
  }

  [[nodiscard]] bool empty() const {
    return container_.empty();
  }
  [[nodiscard]] size_type size() const {
    return container_.size();
  }

  void push(const_reference value) {
    container_.push_back(value);
  }
  void push(value_type&& value) {
    container_.push_back(std::move(value));
  }
  void pop() {
    container_.pop_back();
  }
  void swap(stack& other) noexcept(noexcept(swap(container_, other.container_))) {
    std::swap(container_, other.container_);
  }
};
} // namespace s21

#endif // S21_STACK_H_