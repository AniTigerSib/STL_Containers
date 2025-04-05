#ifndef S21_VECTOR_H_
#define S21_VECTOR_H_

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
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  // using iterator = vector_iterator;
  // using const_iterator = vector_const_iterator;
  using size_type = size_t;
};
}

#endif