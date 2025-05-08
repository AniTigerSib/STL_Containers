#include <gtest/gtest.h>
#include <array>
#include <stdexcept>
#include <string>
#include "../../s21_containers.h"

// Тестовый класс для проверки нетривиальных типов
class TestClass {
 public:
  TestClass() : value_(0) {}
  explicit TestClass(int val) : value_(val) {}
  bool operator==(const TestClass& other) const { return value_ == other.value_; }
  bool operator!=(const TestClass& other) const { return !(*this == other); }
  [[nodiscard]] int get_value() const { return value_; }
 private:
  int value_;
};

// Тест для проверки конструкторов и операторов присваивания
TEST(ArrayTest, Constructors) {
  // Проверка конструктора по умолчанию
  s21::array<int, 5> arr1{};
  EXPECT_EQ(arr1.size(), 5);
  
  // Проверка конструктора initializer_list
  s21::array<int, 5> arr2 = {1, 2, 3, 4, 5};
  EXPECT_EQ(arr2[0], 1);
  EXPECT_EQ(arr2[4], 5);
  
  // Проверка конструктора копирования
  s21::array<int, 5> arr3(arr2);
  EXPECT_EQ(arr3[0], 1);
  EXPECT_EQ(arr3[4], 5);
  
  // Проверка конструктора перемещения
  s21::array<int, 5> arr4(std::move(arr3));
  EXPECT_EQ(arr4[0], 1);
  EXPECT_EQ(arr4[4], 5);
  
  // Проверка оператора присваивания копированием
  s21::array<int, 5> arr5;
  arr5 = arr2;
  EXPECT_EQ(arr5[0], 1);
  EXPECT_EQ(arr5[4], 5);
  
  // Проверка оператора присваивания перемещением
  s21::array<int, 5> arr6;
  arr6 = std::move(arr5);
  EXPECT_EQ(arr6[0], 1);
  EXPECT_EQ(arr6[4], 5);
}

// Тестирование исключения в конструкторе с initializer_list
TEST(ArrayTest, InitializerListException) {
  try {
    s21::array<int, 3> arr = {1, 2, 3, 4, 5};  // Слишком много элементов
  } catch (const std::out_of_range& e) {
    EXPECT_TRUE(true);
  } catch (...) {
    EXPECT_TRUE(false);
  }
}

// Тест для проверки методов доступа к элементам
TEST(ArrayTest, ElementAccess) {
  s21::array<int, 5> arr = {10, 20, 30, 40, 50};
  
  // Проверка метода at()
  EXPECT_EQ(arr.at(0), 10);
  EXPECT_EQ(arr.at(4), 50);
  EXPECT_THROW(arr.at(5), std::out_of_range);
  
  // Проверка operator[]
  EXPECT_EQ(arr[0], 10);
  EXPECT_EQ(arr[4], 50);
  
  // Проверка front() и back()
  EXPECT_EQ(arr.front(), 10);
  EXPECT_EQ(arr.back(), 50);
  
  // Проверка data()
  EXPECT_EQ(*(arr.data()), 10);
  EXPECT_EQ(*(arr.data() + 4), 50);
  
  // Проверка const версий методов (для полноты покрытия)
  const s21::array<int, 5> const_arr = {10, 20, 30, 40, 50};
  EXPECT_EQ(*(const_arr.data()), 10);
}

// Тест для проверки итераторов
TEST(ArrayTest, Iterators) {
  s21::array<int, 5> arr = {1, 2, 3, 4, 5};
  
  // Проверка begin() и end()
  int sum = 0;
  for (int & iter : arr) {
    sum += iter;
  }
  EXPECT_EQ(sum, 15);
  
  // Проверка cbegin() и cend()
  sum = 0;
  for (int iterer : arr) {
    sum += iterer;
  }
  EXPECT_EQ(sum, 15);
  
  // Проверка range-based for с array
  sum = 0;
  for (const auto& val : arr) {
    sum += val;
  }
  EXPECT_EQ(sum, 15);
  
  // Проверка const итераторов
  const s21::array<int, 5> const_arr = {1, 2, 3, 4, 5};
  sum = 0;
  for (int iter : const_arr) {
    sum += iter;
  }
  EXPECT_EQ(sum, 15);
}

// Тест для проверки методов емкости
TEST(ArrayTest, Capacity) {
  s21::array<int, 5> non_empty_arr = {1, 2, 3, 4, 5};
  EXPECT_FALSE(non_empty_arr.empty());
  EXPECT_EQ(non_empty_arr.size(), 5);
  EXPECT_EQ(non_empty_arr.max_size(), 5);
  
  s21::array<int, 0> empty_arr;
  EXPECT_TRUE(empty_arr.empty());
  EXPECT_EQ(empty_arr.size(), 0);
  EXPECT_EQ(empty_arr.max_size(), 0);
}

// Тест для проверки методов модификации
TEST(ArrayTest, Modifiers) {
  s21::array<int, 5> arr1 = {1, 2, 3, 4, 5};
  s21::array<int, 5> arr2 = {6, 7, 8, 9, 10};
  
  // Проверка метода swap()
  arr1.swap(arr2);
  EXPECT_EQ(arr1[0], 6);
  EXPECT_EQ(arr1[4], 10);
  EXPECT_EQ(arr2[0], 1);
  EXPECT_EQ(arr2[4], 5);
  
  // Проверка метода fill()
  arr1.fill(42);
  for (const auto& val : arr1) {
    EXPECT_EQ(val, 42);
  }
}

// Тест для проверки работы с пустым массивом
TEST(ArrayTest, EmptyArray) {
  s21::array<int, 0> empty_arr;
  
  EXPECT_TRUE(empty_arr.empty());
  EXPECT_EQ(empty_arr.size(), 0);
  EXPECT_EQ(empty_arr.begin(), empty_arr.end());
  
  // В пустом массиве data() должен вернуть nullptr
  EXPECT_EQ(empty_arr.data(), nullptr);
}

// Тест для проверки работы со строками
TEST(ArrayTest, StringType) {
  s21::array<std::string, 3> str_arr = {"Hello", "World", "Test"};
  
  EXPECT_EQ(str_arr[0], "Hello");
  EXPECT_EQ(str_arr[1], "World");
  EXPECT_EQ(str_arr[2], "Test");
  
  str_arr[1] = "C++";
  EXPECT_EQ(str_arr[1], "C++");
  
  str_arr.fill("Filled");
  for (const auto& str : str_arr) {
    EXPECT_EQ(str, "Filled");
  }
}

// Тест для проверки работы с пользовательскими типами
TEST(ArrayTest, CustomType) {
  s21::array<TestClass, 3> custom_arr = {TestClass(1), TestClass(2), TestClass(3)};
  
  EXPECT_EQ(custom_arr[0].get_value(), 1);
  EXPECT_EQ(custom_arr[1].get_value(), 2);
  EXPECT_EQ(custom_arr[2].get_value(), 3);
  
  custom_arr.fill(TestClass(42));
  for (const auto& obj : custom_arr) {
    EXPECT_EQ(obj.get_value(), 42);
  }
}

// Тест для сравнения с std::array
TEST(ArrayTest, CompareWithStdArray) {
  s21::array<int, 5> s21_arr = {1, 2, 3, 4, 5};
  std::array<int, 5> std_arr = {1, 2, 3, 4, 5};
  
  // Проверка размера
  EXPECT_EQ(s21_arr.size(), std_arr.size());
  
  // Проверка элементов
  for (size_t i = 0; i < s21_arr.size(); ++i) {
    EXPECT_EQ(s21_arr[i], std_arr[i]);
  }
  
  // Проверка методов front() и back()
  EXPECT_EQ(s21_arr.front(), std_arr.front());
  EXPECT_EQ(s21_arr.back(), std_arr.back());
  
  // Проверка метода fill()
  s21_arr.fill(10);
  std_arr.fill(10);
  for (size_t i = 0; i < s21_arr.size(); ++i) {
    EXPECT_EQ(s21_arr[i], std_arr[i]);
  }
}

// Тест для проверки граничных значений
TEST(ArrayTest, EdgeCases) {
  // Тест массива с одним элементом
  s21::array<int, 1> single_element = {42};
  EXPECT_EQ(single_element.size(), 1);
  EXPECT_EQ(single_element.front(), 42);
  EXPECT_EQ(single_element.back(), 42);
  EXPECT_EQ(single_element[0], 42);
  
  // Тест с большим массивом
  constexpr size_t large_size = 10000;
  s21::array<int, large_size> large_arr;
  large_arr.fill(1);
  
  size_t count = 0;
  for (const auto& element : large_arr) {
    EXPECT_EQ(element, 1);
    count++;
  }
  EXPECT_EQ(count, large_size);
}

// Тест для проверки разных типов данных
TEST(ArrayTest, DifferentTypes) {
  // Тест с double
  s21::array<double, 3> double_arr = {1.1, 2.2, 3.3};
  EXPECT_DOUBLE_EQ(double_arr[0], 1.1);
  EXPECT_DOUBLE_EQ(double_arr[1], 2.2);
  EXPECT_DOUBLE_EQ(double_arr[2], 3.3);
  
  // Тест с char
  s21::array<char, 5> char_arr = {'h', 'e', 'l', 'l', 'o'};
  EXPECT_EQ(char_arr[0], 'h');
  EXPECT_EQ(char_arr[4], 'o');
  
  // Тест с bool
  s21::array<bool, 2> bool_arr = {true, false};
  EXPECT_TRUE(bool_arr[0]);
  EXPECT_FALSE(bool_arr[1]);
}