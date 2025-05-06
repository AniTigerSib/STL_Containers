#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "../../s21_containers.h"

// Test fixture for s21::vector testing
class VectorTest : public ::testing::Test {
// protected:
  // static constexpr int k_int_array[5] = {1, 2, 3, 4, 5};
  // inline static const std::array<std::string, 5> string_array = {"one", "two", "three", "four", "five"};
};

// Constructor tests
TEST_F(VectorTest, DefaultConstructor) {
  s21::vector<int> vect;
  EXPECT_EQ(vect.size(), 0);
  EXPECT_EQ(vect.capacity(), 0);
  EXPECT_TRUE(vect.empty());
}

TEST_F(VectorTest, SizeConstructor) {
  s21::vector<int> vect(5);
  EXPECT_EQ(vect.size(), 5);
  EXPECT_GE(vect.capacity(), 5);
  EXPECT_FALSE(vect.empty());
  for (int & i : vect) {
    EXPECT_EQ(i, 0); // Default-initialized
  }
}

TEST_F(VectorTest, ValueConstructor) {
  s21::vector<int> vect(5, 10);
  EXPECT_EQ(vect.size(), 5);
  EXPECT_GE(vect.capacity(), 5);
  EXPECT_FALSE(vect.empty());
  for (int & i : vect) {
    EXPECT_EQ(i, 10);
  }
}

TEST_F(VectorTest, InitializerListConstructor) {
  const std::initializer_list<int> int_values = {1, 2, 3, 4, 5};
  s21::vector<int> vect(int_values);
  EXPECT_EQ(vect.size(), int_values.size());
  EXPECT_GE(vect.capacity(), int_values.size());
  
  const auto *iter = int_values.begin();
  for (size_t i = 0; i < vect.size(); ++i, ++iter) {
    EXPECT_EQ(vect[i], *iter);
  }
}

TEST_F(VectorTest, CopyConstructor) {
  const std::initializer_list<int> int_values = {1, 2, 3, 4, 5};
  s21::vector<int> vect1(int_values);
  s21::vector<int> vect2(vect1);
  
  EXPECT_EQ(vect2.size(), vect1.size());
  EXPECT_GE(vect2.capacity(), vect1.size());
  
  for (size_t i = 0; i < vect1.size(); ++i) {
    EXPECT_EQ(vect2[i], vect1[i]);
  }
  
  // Modify v1 and check that v2 is not affected
  vect1[0] = 100;
  EXPECT_NE(vect2[0], vect1[0]);
}

TEST_F(VectorTest, MoveConstructor) {
  const std::initializer_list<int> int_values = {1, 2, 3, 4, 5};
  s21::vector<int> vect1(int_values);
  size_t size = vect1.size();
  size_t capacity = vect1.capacity();
  
  s21::vector<int> vect2(std::move(vect1));
  
  EXPECT_EQ(vect2.size(), size);
  EXPECT_GE(vect2.capacity(), capacity);
  EXPECT_EQ(vect1.size(), 0); // v1 should be emptied
  
  const auto *iter = int_values.begin();
  for (size_t i = 0; i < vect2.size(); ++i, ++iter) {
    EXPECT_EQ(vect2[i], *iter);
  }
}

// Move assignment operator test
TEST_F(VectorTest, MoveAssignmentOperator) {
  const std::initializer_list<int> int_values = {1, 2, 3, 4, 5};
  s21::vector<int> vect1(int_values);
  s21::vector<int> vect2;
  
  size_t size = vect1.size();
  size_t capacity = vect1.capacity();
  
  vect2 = std::move(vect1);
  
  EXPECT_EQ(vect2.size(), size);
  EXPECT_GE(vect2.capacity(), capacity);
  EXPECT_EQ(vect1.size(), 0); // v1 should be emptied
  
  const auto *iter = int_values.begin();
  for (size_t i = 0; i < vect2.size(); ++i, ++iter) {
    EXPECT_EQ(vect2[i], *iter);
  }
}

// Element access tests
TEST_F(VectorTest, AtMethod) {
  const std::initializer_list<int> int_values = {1, 2, 3, 4, 5};
  s21::vector<int> vect(int_values);
  
  // Valid access
  for (size_t i = 0; i < vect.size(); ++i) {
    EXPECT_EQ(vect.at(i), *(int_values.begin() + i));
  }
  
  // Out of range access
  EXPECT_THROW(vect.at(vect.size()), std::out_of_range);
}

TEST_F(VectorTest, SquareBracketOperator) {
  const std::initializer_list<int> int_values = {1, 2, 3, 4, 5};
  s21::vector<int> vect(int_values);
  
  for (size_t i = 0; i < vect.size(); ++i) {
    EXPECT_EQ(vect[i], *(int_values.begin() + i));
  }
  
  // Modify elements
  vect[0] = 100;
  EXPECT_EQ(vect[0], 100);
}

TEST_F(VectorTest, FrontMethod) {
  const std::initializer_list<int> int_values = {1, 2, 3, 4, 5};
  s21::vector<int> vect(int_values);
  EXPECT_EQ(vect.front(), *int_values.begin());
  
  // Modify front element
  vect.front() = 100;
  EXPECT_EQ(vect[0], 100);
}

TEST_F(VectorTest, BackMethod) {
  const std::initializer_list<int> int_values = {1, 2, 3, 4, 5};
  s21::vector<int> vect(int_values);
  EXPECT_EQ(vect.back(), *(int_values.end() - 1));
  
  // Modify back element
  vect.back() = 100;
  EXPECT_EQ(vect[vect.size() - 1], 100);
}

TEST_F(VectorTest, DataMethod) {
  const std::initializer_list<int> int_values = {1, 2, 3, 4, 5};
  s21::vector<int> vect(int_values);
  int* data_ptr = vect.data();
  
  for (size_t i = 0; i < vect.size(); ++i) {
    EXPECT_EQ(data_ptr[i], vect[i]);
  }
  
  // Modify through data pointer
  data_ptr[0] = 100;
  EXPECT_EQ(vect[0], 100);
}

// Iterator tests
TEST_F(VectorTest, BeginEndIterators) {
  const std::initializer_list<int> int_values = {1, 2, 3, 4, 5};
  s21::vector<int> vect(int_values);
  
  auto iter = vect.begin();
  const auto *std_it = int_values.begin();
  
  for (; iter != vect.end(); ++iter, ++std_it) {
    EXPECT_EQ(*iter, *std_it);
  }
}

TEST_F(VectorTest, ConstBeginEndIterators) {
  const std::initializer_list<int> int_values = {1, 2, 3, 4, 5};
  const s21::vector<int> vect(int_values);
  
  auto iter = vect.begin();
  const auto *std_it = int_values.begin();
  
  for (; iter != vect.end(); ++iter, ++std_it) {
    EXPECT_EQ(*iter, *std_it);
  }
}

TEST_F(VectorTest, CBeginCEndIterators) {
  const std::initializer_list<int> int_values = {1, 2, 3, 4, 5};
  s21::vector<int> vect(int_values);
  
  auto iter = vect.cbegin();
  const auto *std_it = int_values.begin();
  
  for (; iter != vect.cend(); ++iter, ++std_it) {
    EXPECT_EQ(*iter, *std_it);
  }
}

TEST_F(VectorTest, IteratorComparisons) {
  const std::initializer_list<int> int_values = {1, 2, 3, 4, 5};
  s21::vector<int> vect(int_values);
  
  auto it1 = vect.begin();
  auto it2 = vect.begin() + 2;
  auto it3 = vect.end();
  
  EXPECT_TRUE(it1 < it2);
  EXPECT_TRUE(it2 < it3);
  EXPECT_TRUE(it1 <= it2);
  EXPECT_TRUE(it2 > it1);
  EXPECT_TRUE(it3 > it2);
  EXPECT_TRUE(it2 >= it1);
  EXPECT_FALSE(it1 == it2);
  EXPECT_TRUE(it1 != it2);
}

TEST_F(VectorTest, IteratorArithmetic) {
  const std::initializer_list<int> int_values = {1, 2, 3, 4, 5};
  s21::vector<int> vect(int_values);
  
  auto iter = vect.begin();
  EXPECT_EQ(*(iter + 2), *(int_values.begin() + 2));
  EXPECT_EQ(*(iter += 2), *(int_values.begin() + 2));
  EXPECT_EQ(*(iter - 1), *(int_values.begin() + 1));
  EXPECT_EQ(*(iter -= 1), *(int_values.begin() + 1));
  
  auto it2 = vect.begin() + 3;
  EXPECT_EQ(it2 - iter, 2);
}

TEST_F(VectorTest, IteratorIndexingOperator) {
  const std::initializer_list<int> int_values = {1, 2, 3, 4, 5};
  s21::vector<int> vect(int_values);
  
  auto iter = vect.begin();
  EXPECT_EQ(iter[2], *(int_values.begin() + 2));
}

// Capacity tests
TEST_F(VectorTest, EmptyMethod) {
  const std::initializer_list<int> int_values = {1, 2, 3, 4, 5};
  s21::vector<int> vect1;
  EXPECT_TRUE(vect1.empty());
  
  s21::vector<int> vect2(int_values);
  EXPECT_FALSE(vect2.empty());
  
  vect2.clear();
  EXPECT_TRUE(vect2.empty());
}

TEST_F(VectorTest, SizeMethod) {
  const std::initializer_list<int> int_values = {1, 2, 3, 4, 5};
  s21::vector<int> vect1;
  EXPECT_EQ(vect1.size(), 0);
  
  s21::vector<int> vect2(int_values);
  EXPECT_EQ(vect2.size(), int_values.size());
  
  vect2.push_back(6);
  EXPECT_EQ(vect2.size(), int_values.size() + 1);
  
  vect2.pop_back();
  EXPECT_EQ(vect2.size(), int_values.size());
}

TEST_F(VectorTest, MaxSizeMethod) {
  s21::vector<int> vect;
  std::vector<int> std_v;
  
  // This is not a perfect test as implementations can differ,
  // but they should be reasonable values
  EXPECT_GT(vect.max_size(), 0);
}

TEST_F(VectorTest, ReserveMethod) {
  const std::initializer_list<int> int_values = {1, 2, 3, 4, 5};
  s21::vector<int> vect(int_values);
  size_t original_capacity = vect.capacity();
  size_t new_capacity = original_capacity * 2;
  
  vect.reserve(new_capacity);
  EXPECT_GE(vect.capacity(), new_capacity);
  
  // Elements should remain unchanged
  const auto *iter = int_values.begin();
  for (size_t i = 0; i < vect.size(); ++i, ++iter) {
    EXPECT_EQ(vect[i], *iter);
  }
  
  // Reserve less than current capacity should not change capacity
  vect.reserve(original_capacity);
  EXPECT_GE(vect.capacity(), new_capacity);
}

TEST_F(VectorTest, CapacityMethod) {
  s21::vector<int> vect;
  EXPECT_EQ(vect.capacity(), 0);
  
  vect.reserve(10);
  EXPECT_GE(vect.capacity(), 10);
  
  // Adding elements
  for (int i = 0; i < 15; ++i) {
    size_t old_capacity = vect.capacity();
    vect.push_back(i);
    
    if (i < 10) {
      EXPECT_EQ(vect.capacity(), old_capacity); // Should not change until capacity is reached
    }
  }
  
  EXPECT_GE(vect.capacity(), 15);
}

TEST_F(VectorTest, ShrinkToFitMethod) {
  s21::vector<int> vect;
  vect.reserve(20);
  EXPECT_GE(vect.capacity(), 20);
  
  for (int i = 0; i < 5; ++i) {
    vect.push_back(i);
  }
  
  vect.shrink_to_fit();
  EXPECT_EQ(vect.capacity(), 5);
  
  // Elements should remain unchanged
  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(vect[i], i);
  }
  
  // Test shrink_to_fit on empty vector
  s21::vector<int> empty_v;
  empty_v.reserve(10);
  empty_v.shrink_to_fit();
  EXPECT_EQ(empty_v.capacity(), 0);
}

// Modifier tests
TEST_F(VectorTest, ClearMethod) {
  const std::initializer_list<int> int_values = {1, 2, 3, 4, 5};
  s21::vector<int> vect(int_values);
  EXPECT_FALSE(vect.empty());
  
  vect.clear();
  EXPECT_TRUE(vect.empty());
  EXPECT_EQ(vect.size(), 0);
  // Capacity should remain unchanged
  EXPECT_GE(vect.capacity(), int_values.size());
}

// TEST_F(VectorTest, InsertMethod) {
//   s21::vector<int> vect(int_values_);
//   size_t original_size = vect.size();
  
//   // Insert at beginning
//   auto iter = vect.insert(vect.begin(), 0);
//   EXPECT_EQ(*iter, 0);
//   EXPECT_EQ(vect.size(), original_size + 1);
//   EXPECT_EQ(vect[0], 0);
  
//   // Insert in middle
//   iter = vect.insert(vect.begin() + 3, 100);
//   EXPECT_EQ(*iter, 100);
//   EXPECT_EQ(vect.size(), original_size + 2);
//   EXPECT_EQ(vect[3], 100);
  
//   // Insert at end
//   iter = vect.insert(vect.end(), 200);
//   EXPECT_EQ(*iter, 200);
//   EXPECT_EQ(vect.size(), original_size + 3);
//   EXPECT_EQ(vect[vect.size() - 1], 200);
  
//   // Insert when capacity needs to increase
//   s21::vector<int> small_v = {1};
//   small_v.shrink_to_fit();
//   size_t old_capacity = small_v.capacity();
  
//   for (int i = 0; i < 10; ++i) {
//     small_v.insert(small_v.begin(), i);
//   }
  
//   EXPECT_GT(small_v.capacity(), old_capacity);
//   EXPECT_EQ(small_v.size(), 11);
//   EXPECT_EQ(small_v[0], 9); // Last inserted element
// }

// TEST_F(VectorTest, EraseMethod) {
//   s21::vector<int> v(int_values_);
//   size_t original_size = v.size();
  
//   // Erase from beginning
//   auto it = v.erase(v.cbegin());
//   EXPECT_EQ(v.size(), original_size - 1);
//   EXPECT_EQ(*it, *(int_values_.begin() + 1)); // Should point to the next element
  
//   // Erase from middle
//   it = v.erase(v.cbegin() + 1);
//   EXPECT_EQ(v.size(), original_size - 2);
//   EXPECT_EQ(*it, *(int_values_.begin() + 3)); // Should point to the next element
  
//   // Erase the last element
//   it = v.erase(v.cend() - 1);
//   EXPECT_EQ(v.size(), original_size - 3);
//   EXPECT_EQ(it, v.end()); // Should be end iterator
  
//   // Test invalid erase
//   EXPECT_THROW(v.erase(v.cend()), std::out_of_range);
// }

// TEST_F(VectorTest, EraseRangeMethod) {
//   s21::vector<int> v(int_values_);
//   size_t original_size = v.size();
  
//   // Erase a range in the middle
//   auto it = v.erase(v.cbegin() + 1, v.cbegin() + 3);
//   EXPECT_EQ(v.size(), original_size - 2);
//   EXPECT_EQ(*it, *(int_values_.begin() + 3)); // Should point to the element after the last erased
  
//   // Erase from beginning to middle
//   it = v.erase(v.cbegin(), v.cbegin() + 1);
//   EXPECT_EQ(v.size(), original_size - 3);
//   EXPECT_EQ(*it, *(int_values_.begin() + 3)); // First element is now the element at index 3 from original
  
//   // Erase from middle to end
//   it = v.erase(v.cbegin() + 1, v.cend());
//   EXPECT_EQ(v.size(), 1);
//   EXPECT_EQ(it, v.end()); // Should be end iterator
  
//   // Erase all
//   it = v.erase(v.cbegin(), v.cend());
//   EXPECT_EQ(v.size(), 0);
//   EXPECT_EQ(it, v.end());
  
//   // Test invalid range
//   s21::vector<int> v2(int_values_);
//   EXPECT_THROW(v2.erase(v2.cbegin() + 3, v2.cbegin() + 1), std::out_of_range); // Reversed range
//   EXPECT_THROW(v2.erase(v2.cbegin() - 1, v2.cbegin() + 1), std::out_of_range); // Out of bounds
// }

// TEST_F(VectorTest, PushBackMethod) {
//   s21::vector<int> v;
  
//   // Push to empty vector
//   v.push_back(1);
//   EXPECT_EQ(v.size(), 1);
//   EXPECT_EQ(v[0], 1);
  
//   // Push more elements
//   v.push_back(2);
//   v.push_back(3);
//   EXPECT_EQ(v.size(), 3);
//   EXPECT_EQ(v[1], 2);
//   EXPECT_EQ(v[2], 3);
  
//   // Test capacity expansion
//   s21::vector<int> v2;
//   v2.reserve(2);
//   v2.push_back(1);
//   v2.push_back(2);
//   size_t capacity_before = v2.capacity();
//   v2.push_back(3); // This should trigger expansion
//   EXPECT_GT(v2.capacity(), capacity_before);
//   EXPECT_EQ(v2[2], 3);
// }

// TEST_F(VectorTest, PopBackMethod) {
//   s21::vector<int> v(int_values_);
//   size_t original_size = v.size();
  
//   v.pop_back();
//   EXPECT_EQ(v.size(), original_size - 1);
//   EXPECT_EQ(v.back(), *(int_values_.end() - 2)); // Second-to-last element
  
//   // Pop until empty
//   while (!v.empty()) {
//     v.pop_back();
//   }
//   EXPECT_EQ(v.size(), 0);
  
//   // Pop from empty vector (should not crash)
//   v.pop_back();
//   EXPECT_EQ(v.size(), 0);
// }

// TEST_F(VectorTest, SwapMethod) {
//   s21::vector<int> v1(int_values_);
//   s21::vector<int> v2(5, 10);
  
//   size_t v1_size = v1.size();
//   size_t v2_size = v2.size();
  
//   v1.swap(v2);
  
//   EXPECT_EQ(v1.size(), v2_size);
//   EXPECT_EQ(v2.size(), v1_size);
  
//   // Check v1 elements (should be all 10s)
//   for (size_t i = 0; i < v1.size(); ++i) {
//     EXPECT_EQ(v1[i], 10);
//   }
  
//   // Check v2 elements (should be original int_values)
//   auto it = int_values_.begin();
//   for (size_t i = 0; i < v2.size(); ++i, ++it) {
//     EXPECT_EQ(v2[i], *it);
//   }
// }

// // Test with custom types
// TEST_F(VectorTest, StringType) {
//   s21::vector<std::string> v(string_values_);
  
//   EXPECT_EQ(v.size(), string_values_.size());
  
//   auto it = string_values_.begin();
//   for (size_t i = 0; i < v.size(); ++i, ++it) {
//     EXPECT_EQ(v[i], *it);
//   }
  
//   // Test push_back with string
//   v.push_back("six");
//   EXPECT_EQ(v.size(), string_values_.size() + 1);
//   EXPECT_EQ(v.back(), "six");
  
//   // Test string modification
//   v[0] = "ONE";
//   EXPECT_EQ(v[0], "ONE");
// }

// // Edge case tests
// TEST_F(VectorTest, EmptyVectorOperations) {
//   s21::vector<int> v;
  
//   // Try operations on empty vector
//   EXPECT_THROW(v.at(0), std::out_of_range);
//   EXPECT_THROW(v.erase(v.cbegin()), std::out_of_range);
  
//   // These should not crash
//   v.clear();
//   v.shrink_to_fit();
//   v.reserve(0);
//   v.pop_back();
  
//   // Begin and end should be equal
//   EXPECT_EQ(v.begin(), v.end());
//   EXPECT_EQ(v.cbegin(), v.cend());
// }

// TEST_F(VectorTest, LargeVector) {
//   // Test with a larger number of elements
//   const size_t large_size = 10000;
//   s21::vector<int> v;
  
//   for (size_t i = 0; i < large_size; ++i) {
//     v.push_back(static_cast<int>(i));
//   }
  
//   EXPECT_EQ(v.size(), large_size);
  
//   // Check elements
//   for (size_t i = 0; i < large_size; ++i) {
//     EXPECT_EQ(v[i], static_cast<int>(i));
//   }
  
//   // Test clear on large vector
//   v.clear();
//   EXPECT_EQ(v.size(), 0);
//   EXPECT_GE(v.capacity(), large_size);
// }