#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "../../s21_containers.h"

// Test fixture for s21::vector testing
class VectorTest : public ::testing::Test {
 protected:
  static constexpr std::initializer_list<int> k_int_values = {1, 2, 3, 4, 5};
  inline static const std::initializer_list<std::string> k_string_values = {"one", "two", "three", "four", "five"};
  // static constexpr int k_int_array[5] = {1, 2, 3, 4, 5};
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
  
  s21::vector<int> vect(k_int_values);
  EXPECT_EQ(vect.size(), k_int_values.size());
  EXPECT_GE(vect.capacity(), k_int_values.size());
  
  const auto *iter = k_int_values.begin();
  for (size_t i = 0; i < vect.size(); ++i, ++iter) {
    EXPECT_EQ(vect[i], *iter);
  }
}

TEST_F(VectorTest, CopyConstructor) {
  
  s21::vector<int> vect1(k_int_values);
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
  
  s21::vector<int> vect1(k_int_values);
  size_t size = vect1.size();
  size_t capacity = vect1.capacity();
  
  s21::vector<int> vect2(std::move(vect1));
  
  EXPECT_EQ(vect2.size(), size);
  EXPECT_GE(vect2.capacity(), capacity);
  EXPECT_EQ(vect1.size(), 0); // v1 should be emptied
  
  const auto *iter = k_int_values.begin();
  for (size_t i = 0; i < vect2.size(); ++i, ++iter) {
    EXPECT_EQ(vect2[i], *iter);
  }
}

// Move assignment operator test
TEST_F(VectorTest, MoveAssignmentOperator) {
  
  s21::vector<int> vect1(k_int_values);
  s21::vector<int> vect2;
  
  size_t size = vect1.size();
  size_t capacity = vect1.capacity();
  
  vect2 = std::move(vect1);
  
  EXPECT_EQ(vect2.size(), size);
  EXPECT_GE(vect2.capacity(), capacity);
  EXPECT_EQ(vect1.size(), 0); // v1 should be emptied
  
  const auto *iter = k_int_values.begin();
  for (size_t i = 0; i < vect2.size(); ++i, ++iter) {
    EXPECT_EQ(vect2[i], *iter);
  }
}

// Element access tests
TEST_F(VectorTest, AtMethod) {
  
  s21::vector<int> vect(k_int_values);
  
  // Valid access
  for (size_t i = 0; i < vect.size(); ++i) {
    EXPECT_EQ(vect.at(i), *(k_int_values.begin() + i));
  }
  
  // Out of range access
  EXPECT_THROW(vect.at(vect.size()), std::out_of_range);
}

TEST_F(VectorTest, SquareBracketOperator) {
  
  s21::vector<int> vect(k_int_values);
  
  for (size_t i = 0; i < vect.size(); ++i) {
    EXPECT_EQ(vect[i], *(k_int_values.begin() + i));
  }
  
  // Modify elements
  vect[0] = 100;
  EXPECT_EQ(vect[0], 100);
}

TEST_F(VectorTest, FrontMethod) {
  
  s21::vector<int> vect(k_int_values);
  EXPECT_EQ(vect.front(), *k_int_values.begin());
  
  // Modify front element
  vect.front() = 100;
  EXPECT_EQ(vect[0], 100);
}

TEST_F(VectorTest, BackMethod) {
  
  s21::vector<int> vect(k_int_values);
  EXPECT_EQ(vect.back(), *(k_int_values.end() - 1));
  
  // Modify back element
  vect.back() = 100;
  EXPECT_EQ(vect[vect.size() - 1], 100);
}

TEST_F(VectorTest, DataMethod) {
  
  s21::vector<int> vect(k_int_values);
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
  
  s21::vector<int> vect(k_int_values);
  
  auto iter = vect.begin();
  const auto *std_it = k_int_values.begin();
  
  for (; iter != vect.end(); ++iter, ++std_it) {
    EXPECT_EQ(*iter, *std_it);
  }
}

TEST_F(VectorTest, ConstBeginEndIterators) {
  
  const s21::vector<int> vect(k_int_values);
  
  auto iter = vect.begin();
  const auto *std_it = k_int_values.begin();
  
  for (; iter != vect.end(); ++iter, ++std_it) {
    EXPECT_EQ(*iter, *std_it);
  }
}

TEST_F(VectorTest, CBeginCEndIterators) {
  
  s21::vector<int> vect(k_int_values);
  
  auto iter = vect.cbegin();
  const auto *std_it = k_int_values.begin();
  
  for (; iter != vect.cend(); ++iter, ++std_it) {
    EXPECT_EQ(*iter, *std_it);
  }
}

TEST_F(VectorTest, IteratorComparisons) {
  
  s21::vector<int> vect(k_int_values);
  
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
  
  s21::vector<int> vect(k_int_values);
  
  auto iter = vect.begin();
  EXPECT_EQ(*(iter + 2), *(k_int_values.begin() + 2));
  EXPECT_EQ(*(iter += 2), *(k_int_values.begin() + 2));
  EXPECT_EQ(*(iter - 1), *(k_int_values.begin() + 1));
  EXPECT_EQ(*(iter -= 1), *(k_int_values.begin() + 1));
  
  auto it2 = vect.begin() + 3;
  EXPECT_EQ(it2 - iter, 2);
}

TEST_F(VectorTest, IteratorIndexingOperator) {
  
  s21::vector<int> vect(k_int_values);
  
  auto iter = vect.begin();
  EXPECT_EQ(iter[2], *(k_int_values.begin() + 2));
}

// Capacity tests
TEST_F(VectorTest, EmptyMethod) {
  
  s21::vector<int> vect1;
  EXPECT_TRUE(vect1.empty());
  
  s21::vector<int> vect2(k_int_values);
  EXPECT_FALSE(vect2.empty());
  
  vect2.clear();
  EXPECT_TRUE(vect2.empty());
}

TEST_F(VectorTest, SizeMethod) {
  
  s21::vector<int> vect1;
  EXPECT_EQ(vect1.size(), 0);
  
  s21::vector<int> vect2(k_int_values);
  EXPECT_EQ(vect2.size(), k_int_values.size());
  
  vect2.push_back(6);
  EXPECT_EQ(vect2.size(), k_int_values.size() + 1);
  
  vect2.pop_back();
  EXPECT_EQ(vect2.size(), k_int_values.size());
}

TEST_F(VectorTest, MaxSizeMethod) {
  s21::vector<int> vect;
  std::vector<int> std_v;
  
  // This is not a perfect test as implementations can differ,
  // but they should be reasonable values
  EXPECT_GT(vect.max_size(), 0);
}

TEST_F(VectorTest, ReserveMethod) {
  
  s21::vector<int> vect(k_int_values);
  size_t original_capacity = vect.capacity();
  size_t new_capacity = original_capacity * 2;
  
  vect.reserve(new_capacity);
  EXPECT_GE(vect.capacity(), new_capacity);
  
  // Elements should remain unchanged
  const auto *iter = k_int_values.begin();
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
  
  s21::vector<int> vect(k_int_values);
  EXPECT_FALSE(vect.empty());
  
  vect.clear();
  EXPECT_TRUE(vect.empty());
  EXPECT_EQ(vect.size(), 0);
  // Capacity should remain unchanged
  EXPECT_GE(vect.capacity(), k_int_values.size());
}

TEST_F(VectorTest, InsertMethod) {
  
  s21::vector<int> vect(k_int_values);
  size_t original_size = vect.size();
  
  // Insert at beginning
  auto iter = vect.insert(vect.begin(), 0);
  EXPECT_EQ(*iter, 0);
  EXPECT_EQ(vect.size(), original_size + 1);
  EXPECT_EQ(vect[0], 0);
  
  // Insert in middle
  iter = vect.insert(vect.begin() + 3, 100);
  EXPECT_EQ(*iter, 100);
  EXPECT_EQ(vect.size(), original_size + 2);
  EXPECT_EQ(vect[3], 100);
  
  // Insert at end
  iter = vect.insert(vect.end(), 200);
  EXPECT_EQ(*iter, 200);
  EXPECT_EQ(vect.size(), original_size + 3);
  EXPECT_EQ(vect[vect.size() - 1], 200);
  
  // Insert when capacity needs to increase
  s21::vector<int> small_v = {1};
  small_v.shrink_to_fit();
  size_t old_capacity = small_v.capacity();
  
  for (int i = 0; i < 10; ++i) {
    small_v.insert(small_v.begin(), i);
  }
  
  EXPECT_GT(small_v.capacity(), old_capacity);
  EXPECT_EQ(small_v.size(), 11);
  EXPECT_EQ(small_v[0], 9); // Last inserted element
}

TEST_F(VectorTest, EraseMethod) {
  s21::vector<int> vect(k_int_values);
  size_t original_size = vect.size();
  
  // Erase from beginning
  auto iter = vect.erase(vect.cbegin());
  EXPECT_EQ(vect.size(), original_size - 1);
  EXPECT_EQ(*iter, *(k_int_values.begin() + 1)); // Should point to the next element
  
  // Erase from middle
  iter = vect.erase(vect.cbegin() + 1);
  EXPECT_EQ(vect.size(), original_size - 2);
  EXPECT_EQ(*iter, *(k_int_values.begin() + 3)); // Should point to the next element
  
  // Erase the last element
  iter = vect.erase(vect.cend() - 1);
  EXPECT_EQ(vect.size(), original_size - 3);
  EXPECT_EQ(iter, vect.end()); // Should be end iterator
  
  // Test invalid erase
  EXPECT_THROW(vect.erase(vect.cend()), std::out_of_range);
}

TEST_F(VectorTest, EraseRangeMethod) {
  s21::vector<int> vect(k_int_values);
  size_t original_size = vect.size();
  
  // Erase a range in the middle
  auto iter = vect.erase(vect.cbegin() + 1, vect.cbegin() + 3);
  EXPECT_EQ(vect.size(), original_size - 2);
  EXPECT_EQ(*iter, *(k_int_values.begin() + 3)); // Should point to the element after the last erased
  
  // Erase from beginning to middle
  iter = vect.erase(vect.cbegin(), vect.cbegin() + 1);
  EXPECT_EQ(vect.size(), original_size - 3);
  EXPECT_EQ(*iter, *(k_int_values.begin() + 3)); // First element is now the element at index 3 from original
  
  // Erase from middle to end
  iter = vect.erase(vect.cbegin() + 1, vect.cend());
  EXPECT_EQ(vect.size(), 1);
  EXPECT_EQ(iter, vect.end()); // Should be end iterator
  
  // Erase all
  iter = vect.erase(vect.cbegin(), vect.cend());
  EXPECT_EQ(vect.size(), 0);
  EXPECT_EQ(iter, vect.end());
  
  // Test invalid range
  s21::vector<int> vect2(k_int_values);
  EXPECT_THROW(vect2.erase(vect2.cbegin() + 3, vect2.cbegin() + 1), std::out_of_range); // Reversed range
  EXPECT_THROW(vect2.erase(vect2.cbegin() - 1, vect2.cbegin() + 1), std::out_of_range); // Out of bounds
}

TEST_F(VectorTest, PushBackMethod) {
  s21::vector<int> vect;
  
  // Push to empty vector
  vect.push_back(1);
  EXPECT_EQ(vect.size(), 1);
  EXPECT_EQ(vect[0], 1);
  
  // Push more elements
  vect.push_back(2);
  vect.push_back(3);
  EXPECT_EQ(vect.size(), 3);
  EXPECT_EQ(vect[1], 2);
  EXPECT_EQ(vect[2], 3);
  
  // Test capacity expansion
  s21::vector<int> vect2;
  vect2.reserve(2);
  vect2.push_back(1);
  vect2.push_back(2);
  size_t capacity_before = vect2.capacity();
  vect2.push_back(3); // This should trigger expansion
  EXPECT_GT(vect2.capacity(), capacity_before);
  EXPECT_EQ(vect2[2], 3);
}

TEST_F(VectorTest, PopBackMethod) {
  s21::vector<int> vect(k_int_values);
  size_t original_size = vect.size();
  
  vect.pop_back();
  EXPECT_EQ(vect.size(), original_size - 1);
  EXPECT_EQ(vect.back(), *(k_int_values.end() - 2)); // Second-to-last element
  
  // Pop until empty
  while (!vect.empty()) {
    vect.pop_back();
  }
  EXPECT_EQ(vect.size(), 0);
  
  // Pop from empty vector (should not crash)
  vect.pop_back();
  EXPECT_EQ(vect.size(), 0);
}

TEST_F(VectorTest, SwapMethod) {
  s21::vector<int> v1(k_int_values);
  s21::vector<int> v2(5, 10);
  
  size_t v1_size = v1.size();
  size_t v2_size = v2.size();
  
  v1.swap(v2);
  
  EXPECT_EQ(v1.size(), v2_size);
  EXPECT_EQ(v2.size(), v1_size);
  
  // Check v1 elements (should be all 10s)
  for (size_t i = 0; i < v1.size(); ++i) {
    EXPECT_EQ(v1[i], 10);
  }
  
  // Check v2 elements (should be original int_values)
  auto it = k_int_values.begin();
  for (size_t i = 0; i < v2.size(); ++i, ++it) {
    EXPECT_EQ(v2[i], *it);
  }
}

// Test with custom types
TEST_F(VectorTest, StringType) {
  s21::vector<std::string> vect(k_string_values);
  
  EXPECT_EQ(vect.size(), k_string_values.size());
  
  const auto *iter = k_string_values.begin();
  for (size_t i = 0; i < vect.size(); ++i, ++iter) {
    EXPECT_EQ(vect[i], *iter);
  }
  
  // Test push_back with string
  vect.push_back("six");
  EXPECT_EQ(vect.size(), k_string_values.size() + 1);
  EXPECT_EQ(vect.back(), "six");
  
  // Test string modification
  vect[0] = "ONE";
  EXPECT_EQ(vect[0], "ONE");
}

// Edge case tests
TEST_F(VectorTest, EmptyVectorOperations) {
  s21::vector<int> vect;
  
  // Try operations on empty vector
  EXPECT_THROW(vect.at(0), std::out_of_range);
  EXPECT_THROW(vect.erase(vect.cbegin()), std::out_of_range);
  
  // These should not crash
  vect.clear();
  vect.shrink_to_fit();
  vect.reserve(0);
  vect.pop_back();
  
  // Begin and end should be equal
  EXPECT_EQ(vect.begin(), vect.end());
  EXPECT_EQ(vect.cbegin(), vect.cend());
}

TEST_F(VectorTest, LargeVector) {
  // Test with a larger number of elements
  const size_t k_large_size = 10000;
  s21::vector<int> vect;
  
  for (size_t i = 0; i < k_large_size; ++i) {
    vect.push_back(static_cast<int>(i));
  }
  
  EXPECT_EQ(vect.size(), k_large_size);
  
  // Check elements
  for (size_t i = 0; i < k_large_size; ++i) {
    EXPECT_EQ(vect[i], static_cast<int>(i));
  }
  
  // Test clear on large vector
  vect.clear();
  EXPECT_EQ(vect.size(), 0);
  EXPECT_GE(vect.capacity(), k_large_size);
}