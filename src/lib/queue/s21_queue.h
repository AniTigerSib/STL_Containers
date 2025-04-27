#ifndef S21_QUEUE_H_
#define S21_QUEUE_H_

#include <deque>

namespace s21 {
template <typename T, typename Container = std::deque<T>>
class queue {
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

 protected:
  Container container_;

 public:
  queue() : queue(Container()) {}
  explicit queue(const Container& container) {
    container_ = container;
  }
  queue(std::initializer_list<value_type> const& items) {
    container_ = std::move(Container(items));
  }
  queue(const queue& stack) {
    container_ = stack.container_;
  }
  queue(queue&& stack) {
    container_ = std::move(stack.container_);
  }
  ~queue() = default;

  queue& operator=(queue&& other) {
    container_ = std::move(other.container_);
    return *this;
  }

  template <typename Alloc>
  explicit queue(const Alloc& alloc) {
    container_ = std::move(Container(alloc));
  }

  const_reference front() {
    return container_.front();
  }
  const_reference back() {
    return container_.back();
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
    container_.pop_front();
  }
  void swap(queue& other) noexcept(noexcept(swap(container_, other.container_))) {
    std::swap(container_, other.container_);
  }
};
} // namespace s21

#endif // S21_QUEUE_H_