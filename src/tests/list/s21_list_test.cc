#include <gtest/gtest.h>
#include <list>
// #define TESTS
#include "../../lib/list/s21_list.h"

// Фикстура для тестирования класса s21::list
class S21ListTest : public ::testing::Test {
 protected:
  // Методы, запускаемые перед каждым тестом
  void SetUp() override {
    // Пустые списки для каждого теста
    s21_empty_list_ = new s21::list<int>();
    std_empty_list_ = new std::list<int>();
    
    // Списки с элементами
    s21_list_ = new s21::list<int>({1, 2, 3, 4, 5});
    s21_const_list_ = new s21::list<int>({1, 2, 3, 4, 5});
    std_list_ = new std::list<int>({1, 2, 3, 4, 5});
  }
  void TearDown() override {
    // std::cout << "TESTING: " << s21_list_->size() << std::endl;
    delete s21_empty_list_;
    delete std_empty_list_;
    delete s21_list_;
    // std::cout << "TESTING: " << std_list_->size() << std::endl;
    delete s21_const_list_;
    delete std_list_;
  }
  
  // Вспомогательные функции
  template <typename T>
  void compare_lists(const s21::list<T>& s21_list, const std::list<T>& std_list) {
    ASSERT_EQ(s21_list.size(), std_list.size());
      
    auto s21_it = s21_list.begin();
    auto std_it = std_list.begin();
      
    for (; s21_it != s21_list.end() && std_it != std_list.end(); ++s21_it, ++std_it) {
      EXPECT_EQ(*s21_it, *std_it);
    }
  }
  
  s21::list<int>* s21_empty_list_;
  std::list<int>* std_empty_list_;
  s21::list<int>* s21_list_;
  const s21::list<int>* s21_const_list_;
  std::list<int>* std_list_;
};

// Тесты конструкторов
TEST_F(S21ListTest, DefaultConstructor) {
  EXPECT_TRUE(s21_empty_list_->empty());
  EXPECT_EQ(s21_empty_list_->size(), 0);
  EXPECT_FALSE(s21_list_->empty());
  EXPECT_EQ(s21_list_->size(), 5);
}

TEST_F(S21ListTest, SizeValueConstructor) {
  try {
    s21::list<int> s21_list_size(3, 10);
    std::list<int> std_list_size(3, 10);
    compare_lists(s21_list_size, std_list_size);
  } catch (const std::exception& e) {
    FAIL() << "Unexpected exception: " << e.what();
  }
}

TEST_F(S21ListTest, InitializerListConstructor) {
  compare_lists(*s21_list_, *std_list_);
}

TEST_F(S21ListTest, CopyConstructor) {
  try {
    s21::list<int> s21_list_copy(*s21_list_);
    std::list<int> std_list_copy(*std_list_);
    compare_lists(s21_list_copy, std_list_copy);
  } catch (const std::exception& e) {
    FAIL() << "Unexpected exception: " << e.what();
  }
}

TEST_F(S21ListTest, MoveConstructor) {
  try {
    s21::list<int> s21_list_move(std::move(*s21_list_));
    std::list<int> std_list_move(std::move(*std_list_));
    
    compare_lists(s21_list_move, std_list_move);
    
    EXPECT_EQ(s21_list_->size(), 0);
    EXPECT_EQ(s21_list_move.size(), 5);
    EXPECT_NE(s21_list_move.begin(), s21_list_move.end());
  } catch (const std::exception& e) {
    FAIL() << "Unexpected exception: " << e.what();
  }
}

TEST_F(S21ListTest, Front) {
  EXPECT_EQ(s21_list_->front(), std_list_->front());
}

TEST_F(S21ListTest, Back) {
  EXPECT_EQ(s21_list_->back(), std_list_->back());
}

TEST_F(S21ListTest, Iterator) {
  auto s21_it = s21_list_->begin();
  auto std_it = std_list_->begin();

  for (; s21_it != s21_list_->end() && std_it != std_list_->end(); ++s21_it, ++std_it) {
    EXPECT_EQ(*s21_it, *std_it);
  }

  EXPECT_EQ(s21_it, s21_list_->end());
  EXPECT_EQ(std_it, std_list_->end());
}

TEST_F(S21ListTest, ConstIterator) {
  auto s21_it = s21_list_->cbegin();
  auto std_it = std_list_->cbegin();

  for (; s21_it != s21_list_->cend() && std_it != std_list_->cend(); ++s21_it, ++std_it) {
    EXPECT_EQ(*s21_it, *std_it);
  }

  EXPECT_EQ(s21_it, s21_list_->cend());
  EXPECT_EQ(std_it, std_list_->cend());
}

TEST_F(S21ListTest, ConstIteratorConst) {
  s21::list<int>::const_iterator s21_it = s21_const_list_->begin();
  auto std_it = std_list_->cbegin();

  for (; s21_it != s21_const_list_->end() && std_it != std_list_->cend(); ++s21_it, ++std_it) {
    EXPECT_EQ(*s21_it, *std_it);
  }
}

TEST_F(S21ListTest, IteratorReverse) {
  auto s21_it = s21_list_->end();
  auto std_it = std_list_->end();

  if (!s21_list_->empty() && !std_list_->empty()) {
    do {
      --s21_it;
      --std_it;
      EXPECT_EQ(*s21_it, *std_it);
    } while (s21_it != s21_list_->begin() && std_it != std_list_->begin());
  }
}

TEST_F(S21ListTest, ConstIteratorReverse) {
  auto s21_it = s21_list_->cend();
  auto std_it = std_list_->cend();

  if (!s21_list_->empty() && !std_list_->empty()) {
    do {
      --s21_it;
      --std_it;
      EXPECT_EQ(*s21_it, *std_it);
    } while (s21_it != s21_list_->cbegin() && std_it != std_list_->cbegin());
  }
}

TEST_F(S21ListTest, ConstIteratorReverseConst) {
  auto s21_it = s21_const_list_->end();
  auto std_it = std_list_->cend();

  if (!s21_list_->empty() && !std_list_->empty()) {
    do {
      --s21_it;
      --std_it;
      EXPECT_EQ(*s21_it, *std_it);
    } while (s21_it != s21_list_->cbegin() && std_it != std_list_->cbegin());
  }
}

// Тесты операций с контейнером
TEST_F(S21ListTest, EmptyTest) {
  EXPECT_TRUE(s21_empty_list_->empty());
  EXPECT_FALSE(s21_list_->empty());
}

TEST_F(S21ListTest, SizeTest) {
  EXPECT_EQ(s21_empty_list_->size(), 0);
  EXPECT_EQ(s21_list_->size(), 5);
}

TEST_F(S21ListTest, MaxSizeTest) {
  EXPECT_GT(s21_list_->max_size(), 0);
  // Не проверяем точное значение, так как оно зависит от платформы
}

TEST_F(S21ListTest, ClearTest) {
  try {
    s21_list_->clear();
    std_list_->clear();
    EXPECT_TRUE(s21_list_->empty());
    EXPECT_EQ(s21_list_->size(), 0);
    compare_lists(*s21_list_, *std_list_);
  } catch (const std::exception& e) {
    FAIL() << "Exception thrown during clear: " << e.what();
  }
}

// Тесты модификации контейнера
TEST_F(S21ListTest, InsertTest) {
  try {
    // Создаем свежие списки для тестирования вставки
    s21::list<int> s21_test_list({1, 2, 3, 4, 5});
    std::list<int> std_test_list({1, 2, 3, 4, 5});

    auto s21_it = s21_test_list.begin();
    auto std_it = std_test_list.begin();

    // Вставка в начало
    s21_it = s21_test_list.insert(s21_it, 0);
    std_it = std_test_list.insert(std_it, 0);
    EXPECT_EQ(*s21_it, *std_it);
    compare_lists(s21_test_list, std_test_list);

    // Вставка в середину
    s21_it = s21_test_list.begin();
    std_it = std_test_list.begin();
    std::advance(s21_it, 2);
    std::advance(std_it, 2);

    s21_it = s21_test_list.insert(s21_it, 10);
    std_it = std_test_list.insert(std_it, 10);
    EXPECT_EQ(*s21_it, *std_it);
    compare_lists(s21_test_list, std_test_list);

    // Вставка в конец
    s21_it = s21_test_list.end();
    std_it = std_test_list.end();

    s21_it = s21_test_list.insert(s21_it, 100);
    std_it = std_test_list.insert(std_it, 100);
    EXPECT_EQ(*s21_it, *std_it);
    compare_lists(s21_test_list, std_test_list);
  } catch (const std::exception& e) {
    FAIL() << "Exception thrown during insert test: " << e.what();
  }
}

TEST_F(S21ListTest, InsertManyTest) {
  try {
    s21::list<int> test_list({1, 2, 3, 4, 5});
    auto s21_cit = test_list.cbegin();
    std::advance(s21_cit, 2);

    auto s21_it = test_list.insert_many(s21_cit, 10, 20, 30);

    std::list<int> expected({1, 2, 10, 20, 30, 3, 4, 5});
    compare_lists(test_list, expected);
    EXPECT_EQ(*s21_it, 30); // Последний вставленный элемент
  } catch (const std::exception& e) {
      FAIL() << "Exception thrown during insert_many test: " << e.what();
  }
}

TEST_F(S21ListTest, EraseTest) {
  try {
    // Создаем свежие списки для тестирования удаления
    s21::list<int> s21_test_list({1, 2, 3, 4, 5});
    std::list<int> std_test_list({1, 2, 3, 4, 5});
      
    auto s21_it = s21_test_list.begin();
    auto std_it = std_test_list.begin();
      
    // Удаление первого элемента
    s21_it = s21_test_list.erase(s21_it);
    std_it = std_test_list.erase(std_it);
    compare_lists(s21_test_list, std_test_list);
    EXPECT_EQ(*s21_it, *std_it);
      
    // Удаление из середины
    s21_it = s21_test_list.begin();
    std_it = std_test_list.begin();
    std::advance(s21_it, 1);
    std::advance(std_it, 1);
      
    s21_it = s21_test_list.erase(s21_it);
    std_it = std_test_list.erase(std_it);
    compare_lists(s21_test_list, std_test_list);
    EXPECT_EQ(*s21_it, *std_it);
      
    // Удаление последнего элемента
    s21_it = s21_test_list.end();
    std_it = std_test_list.end();
    --s21_it;
    --std_it;
      
    s21_it = s21_test_list.erase(s21_it);
    std_it = std_test_list.erase(std_it);
    compare_lists(s21_test_list, std_test_list);
    EXPECT_EQ(s21_it, s21_test_list.end());
    EXPECT_EQ(std_it, std_test_list.end());
  } catch (const std::exception& e) {
    FAIL() << "Exception thrown during erase test: " << e.what();
  }
}

TEST_F(S21ListTest, PushBackTest) {
  try {
    s21_list_->push_back(6);
    std_list_->push_back(6);
    compare_lists(*s21_list_, *std_list_);
      
    s21_empty_list_->push_back(1);
    std_empty_list_->push_back(1);
    compare_lists(*s21_empty_list_, *std_empty_list_);
  } catch (const std::exception& e) {
    FAIL() << "Exception thrown during push_back test: " << e.what();
  }
}

TEST_F(S21ListTest, PopBackTest) {
  try {
    if (!s21_list_->empty() && !std_list_->empty()) {
      s21_list_->pop_back();
      std_list_->pop_back();
      compare_lists(*s21_list_, *std_list_);
    }
      
    // Пустой список
    s21_empty_list_->pop_back(); // Не должно быть ошибки
    EXPECT_TRUE(s21_empty_list_->empty());
      
    // Создаем новый список для тестирования опустошения
    s21::list<int> temp_s21_list({1, 2, 3});
    std::list<int> temp_std_list({1, 2, 3});
      
    // Опустошение списка
    while (!temp_s21_list.empty()) {
      temp_s21_list.pop_back();
      temp_std_list.pop_back();
    }
    compare_lists(temp_s21_list, temp_std_list);
  } catch (const std::exception& e) {
    FAIL() << "Exception thrown during pop_back test: " << e.what();
  }
}

TEST_F(S21ListTest, PushFrontTest) {
  try {
    s21_list_->push_front(0);
    std_list_->push_front(0);
    compare_lists(*s21_list_, *std_list_);
      
    s21_empty_list_->push_front(1);
    std_empty_list_->push_front(1);
    compare_lists(*s21_empty_list_, *std_empty_list_);
  } catch (const std::exception& e) {
    FAIL() << "Exception thrown during push_front test: " << e.what();
  }
}

TEST_F(S21ListTest, PopFrontTest) {
  try {
    if (!s21_list_->empty() && !std_list_->empty()) {
      s21_list_->pop_front();
      std_list_->pop_front();
      compare_lists(*s21_list_, *std_list_);
    }
      
    // Пустой список
    s21_empty_list_->pop_front(); // Не должно быть ошибки
    EXPECT_TRUE(s21_empty_list_->empty());
      
    // Создаем новый список для тестирования опустошения
    s21::list<int> temp_s21_list({1, 2, 3});
    std::list<int> temp_std_list({1, 2, 3});
      
    // Опустошение списка
    while (!temp_s21_list.empty()) {
      temp_s21_list.pop_front();
      temp_std_list.pop_front();
    }
    compare_lists(temp_s21_list, temp_std_list);
  } catch (const std::exception& e) {
    FAIL() << "Exception thrown during pop_front test: " << e.what();
  }
}

TEST_F(S21ListTest, SwapTest) {
  try {
    s21::list<int> s21_other({10, 20, 30});
    std::list<int> std_other({10, 20, 30});
      
    s21_list_->swap(s21_other);
    std_list_->swap(std_other);
      
    compare_lists(*s21_list_, *std_list_);
    compare_lists(s21_other, std_other);
      
    // Свап с пустым списком
    s21_list_->swap(*s21_empty_list_);
    std_list_->swap(*std_empty_list_);
      
    compare_lists(*s21_list_, *std_list_);
    compare_lists(*s21_empty_list_, *std_empty_list_);
      
    // Восстановим данные для следующих тестов
    s21_list_->swap(*s21_empty_list_);
    std_list_->swap(*std_empty_list_);
      
    // Свап с самим собой
    s21_list_->swap(*s21_list_);
    compare_lists(*s21_list_, *std_list_);
  } catch (const std::exception& e) {
    FAIL() << "Exception thrown during swap test: " << e.what();
  }
}

TEST_F(S21ListTest, MergeTest) {
  try {
    s21::list<int> s21_other({0, 3, 6, 9});
    std::list<int> std_other({0, 3, 6, 9});

    s21_list_->merge(s21_other);
    std_list_->merge(std_other);

    compare_lists(*s21_list_, *std_list_);
    EXPECT_TRUE(s21_other.empty());

    // Проверка порядка после слияния
    std::list<int> expected({0, 1, 2, 3, 3, 4, 5, 6, 9});
    compare_lists(*s21_list_, expected);

    // Слияние с пустым списком
    s21_list_->merge(*s21_empty_list_);
    EXPECT_EQ(s21_list_->size(), 9);

    // Слияние пустого с непустым
    s21_empty_list_->merge(*s21_list_);
    EXPECT_EQ(s21_empty_list_->size(), 9);
    EXPECT_TRUE(s21_list_->empty());

    // Восстановим данные для следующих тестов
    s21_list_->swap(*s21_empty_list_);

    // Слияние с самим собой
    s21::list<int> temp_list(*s21_list_);
    temp_list.merge(temp_list);
    EXPECT_EQ(temp_list.size(), 9);
  } catch (const std::exception& e) {
    FAIL() << "Exception thrown during merge test: " << e.what();
  }
}

TEST_F(S21ListTest, SpliceTest) {
  try {
    // Splice полного списка
    s21::list<int> s21_other({10, 20, 30});
    std::list<int> std_other({10, 20, 30});
    
    auto s21_it = s21_list_->cbegin();
    auto std_it = std_list_->cbegin();
    std::advance(s21_it, 2);
    std::advance(std_it, 2);
    
    s21_list_->splice(s21_it, s21_other);
    std_list_->splice(std_it, std_other);
    
    compare_lists(*s21_list_, *std_list_);
    EXPECT_TRUE(s21_other.empty());

    // Splice одного элемента
    s21_other = s21::list<int>({100, 200, 300});
    std_other = std::list<int>({100, 200, 300});
    
    s21_it = s21_list_->cbegin();
    std_it = std_list_->cbegin();
    
    auto s21_other_it = s21_other.cbegin();
    auto std_other_it = std_other.cbegin();
    std::advance(s21_other_it, 1);
    std::advance(std_other_it, 1);
    
    s21_list_->splice(s21_it, s21_other, s21_other_it);
    std_list_->splice(std_it, std_other, std_other_it);
    
    compare_lists(*s21_list_, *std_list_);
    EXPECT_EQ(s21_other.size(), 2);
    
    // Splice диапазона
    s21_it = s21_list_->cend();
    std_it = std_list_->cend();
    
    auto s21_first = s21_other.cbegin();
    auto std_first = std_other.cbegin();
    
    s21_list_->splice(s21_it, s21_other, s21_first, s21_other.cend());
    std_list_->splice(std_it, std_other, std_first, std_other.cend());
    
    compare_lists(*s21_list_, *std_list_);
    EXPECT_TRUE(s21_other.empty());
    
    // Splice пустого списка
    s21_list_->splice(s21_list_->cbegin(), *s21_empty_list_);
    std_list_->splice(std_list_->cbegin(), *std_empty_list_);
    compare_lists(*s21_list_, *std_list_);
    
    // Splice в пустой список
    s21_empty_list_->splice(s21_empty_list_->cbegin(), *s21_list_);
    std_empty_list_->splice(std_empty_list_->cbegin(), *std_list_);
    compare_lists(*s21_empty_list_, *std_empty_list_);
    EXPECT_TRUE(s21_list_->empty());
    
    // Восстановим данные для следующих тестов
    s21_list_->swap(*s21_empty_list_);
    std_list_->swap(*std_empty_list_);
  } catch (const std::exception& e) {
    FAIL() << "Exception thrown during splice test: " << e.what();
  }
}

TEST_F(S21ListTest, ReverseTest) {
  try {
    s21_list_->reverse();
    std_list_->reverse();
    compare_lists(*s21_list_, *std_list_);

    // Пустой список
    s21_empty_list_->reverse();
    EXPECT_TRUE(s21_empty_list_->empty());

    // Список с одним элементом
    s21::list<int> s21_single({1});
    std::list<int> std_single({1});
    s21_single.reverse();
    std_single.reverse();
    compare_lists(s21_single, std_single);
  } catch (const std::exception& e) {
    FAIL() << "Exception thrown during reverse test: " << e.what();
  }
}

TEST_F(S21ListTest, UniqueTest) {
  try {
    s21::list<int> s21_dupes({1, 1, 2, 2, 2, 3, 4, 4, 5, 5, 5});
    std::list<int> std_dupes({1, 1, 2, 2, 2, 3, 4, 4, 5, 5, 5});
    
    s21_dupes.unique();
    std_dupes.unique();
    
    compare_lists(s21_dupes, std_dupes);
    EXPECT_EQ(s21_dupes.size(), 5);
    
    // Уже уникальный список
    s21_list_->unique();
    std_list_->unique();
    compare_lists(*s21_list_, *std_list_);
    
    // Пустой список
    s21_empty_list_->unique();
    EXPECT_TRUE(s21_empty_list_->empty());
    
    // Список с одним элементом
    s21::list<int> s21_single({1});
    s21_single.unique();
    EXPECT_EQ(s21_single.size(), 1);
    
    // Все элементы одинаковые
    s21::list<int> s21_same({5, 5, 5, 5, 5});
    std::list<int> std_same({5, 5, 5, 5, 5});
    s21_same.unique();
    std_same.unique();
    compare_lists(s21_same, std_same);
    EXPECT_EQ(s21_same.size(), 1);
  } catch (const std::exception& e) {
    FAIL() << "Exception thrown during unique test: " << e.what();
  }
}

TEST_F(S21ListTest, SortTest) {
  try {
    s21::list<int> s21_unsorted({5, 3, 1, 4, 2});
    std::list<int> std_unsorted({5, 3, 1, 4, 2});

    s21_unsorted.sort();
    std_unsorted.sort();

    compare_lists(s21_unsorted, std_unsorted);

    // Уже отсортированный список
    s21_unsorted.sort();
    compare_lists(s21_unsorted, std_unsorted);

    // Пустой список
    s21_empty_list_->sort();
    EXPECT_TRUE(s21_empty_list_->empty());

    // Список с одним элементом
    s21::list<int> s21_single({1});
    s21_single.sort();
    EXPECT_EQ(s21_single.size(), 1);
    EXPECT_EQ(*s21_single.begin(), 1);

    // Обратно отсортированный список
    s21::list<int> s21_reverse({5, 4, 3, 2, 1});
    std::list<int> std_reverse({5, 4, 3, 2, 1});
    s21_reverse.sort();
    std_reverse.sort();
    compare_lists(s21_reverse, std_reverse);

    // Список с повторяющимися элементами
    s21::list<int> s21_dupes({3, 1, 3, 2, 1});
    std::list<int> std_dupes({3, 1, 3, 2, 1});
    s21_dupes.sort();
    std_dupes.sort();
    compare_lists(s21_dupes, std_dupes);
  } catch (const std::exception& e) {
      FAIL() << "Exception thrown during sort test: " << e.what();
  }
}

// Тесты на пользовательские типы данных
class TestClass {
public:
  int value;

  TestClass() : value(0) {}
  explicit TestClass(int val) : value(val) {}

  bool operator==(const TestClass& other) const {
      return value == other.value;
  }

  bool operator<(const TestClass& other) const {
      return value < other.value;
  }

  bool operator>(const TestClass& other) const {
      return value > other.value;
  }
};

TEST(S21ListClassTest, CustomTypeTest) {
  try {
    s21::list<TestClass> s21_custom;
    s21_custom.push_back(TestClass(1));
    s21_custom.push_back(TestClass(2));
    s21_custom.push_back(TestClass(3));

    EXPECT_EQ(s21_custom.size(), 3);
    EXPECT_EQ(s21_custom.front().value, 1);
    EXPECT_EQ(s21_custom.back().value, 3);

    // Сортировка пользовательского типа
    s21::list<TestClass> s21_unsorted;
    s21_unsorted.push_back(TestClass(5));
    s21_unsorted.push_back(TestClass(3));
    s21_unsorted.push_back(TestClass(1));
    s21_unsorted.push_back(TestClass(4));
    s21_unsorted.push_back(TestClass(2));

    s21_unsorted.sort();

    auto it = s21_unsorted.begin();
    EXPECT_EQ(it->value, 1); ++it;
    EXPECT_EQ(it->value, 2); ++it;
    EXPECT_EQ(it->value, 3); ++it;
    EXPECT_EQ(it->value, 4); ++it;
    EXPECT_EQ(it->value, 5);
  } catch (const std::exception& e) {
    FAIL() << "Exception thrown in custom type test: " << e.what();
  }
}