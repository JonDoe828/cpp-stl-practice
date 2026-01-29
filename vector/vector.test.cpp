#include <catch2/catch_test_macros.hpp>

#include "vector.hpp"

TEST_CASE("MyVector basic push and size") {
  MyVector<int> v;
  REQUIRE(v.size() == 0);

  v.push(10);
  v.push(20);
  v.push(30);

  REQUIRE(v.size() == 3);
  REQUIRE(v.get(0) == 10);
  REQUIRE(v.get(1) == 20);
  REQUIRE(v.get(2) == 30);
}

TEST_CASE("MyVector get with invalid index") {
  MyVector<int> v;
  v.push(10);

  REQUIRE(v.get(-1) == -1);
  REQUIRE(v.get(1) == -1);
}

TEST_CASE("MyVector insert at arbitrary position") {
  MyVector<int> v;
  v.push(20);
  v.push(30);
  v.push(40);

  v.insert(0, 10); // insert at head

  REQUIRE(v.size() == 4);
  REQUIRE(v.get(0) == 10);
  REQUIRE(v.get(1) == 20);
  REQUIRE(v.get(2) == 30);
  REQUIRE(v.get(3) == 40);
}

TEST_CASE("MyVector pop removes last element") {
  MyVector<int> v;
  v.push(10);
  v.push(20);
  v.push(30);

  v.pop();

  REQUIRE(v.size() == 2);
  REQUIRE(v.get(0) == 10);
  REQUIRE(v.get(1) == 20);
}

TEST_CASE("MyVector clear resets vector") {
  MyVector<int> v;
  v.push(1);
  v.push(2);
  v.push(3);

  v.clear();

  REQUIRE(v.size() == 0);
  REQUIRE(v.get(0) == -1);
}

TEST_CASE("MyVector iterator traversal") {
  MyVector<int> v;
  v.push(10);
  v.push(20);
  v.push(30);

  int expected = 10;
  for (auto it = v.begin(); it != v.end(); ++it) {
    REQUIRE(*it == expected);
    expected += 10;
  }
}

TEST_CASE("MyVector foreach traversal") {
  MyVector<int> v;
  v.push(10);
  v.push(20);
  v.push(30);

  int expected = 10;
  for (int x : v) {
    REQUIRE(x == expected);
    expected += 10;
  }
}

TEST_CASE("MyVector iterator equality comparison") {
  MyVector<int> v;
  v.push(1);
  v.push(2);

  auto it1 = v.begin();
  auto it2 = v.begin();
  auto it3 = v.end();

  REQUIRE(it1 == it2);
  REQUIRE_FALSE(it1 != it2);

  REQUIRE(it1 != it3);
  REQUIRE_FALSE(it1 == it3);
}

TEST_CASE("MyVector iterator ordering comparison") {
  MyVector<int> v;
  v.push(10);
  v.push(20);
  v.push(30);

  auto it0 = v.begin();
  auto it1 = it0 + 1;
  auto it2 = it0 + 2;

  REQUIRE(it0 < it1);
  REQUIRE(it1 < it2);

  REQUIRE(it2 > it1);
  REQUIRE(it1 > it0);
}

TEST_CASE("MyVector iterator <= >= comparison") {
  MyVector<int> v;
  v.push(5);
  v.push(6);

  auto it0 = v.begin();
  auto it1 = it0 + 1;

  REQUIRE(it0 <= it0);
  REQUIRE(it0 <= it1);
  REQUIRE(it1 >= it1);
  REQUIRE(it1 >= it0);
}

TEST_CASE("MyVector iterator difference") {
  MyVector<int> v;
  v.push(1);
  v.push(2);
  v.push(3);
  v.push(4);

  auto it0 = v.begin();
  auto it2 = it0 + 2;
  auto it4 = v.end();

  REQUIRE(it2 - it0 == 2);
  REQUIRE(it4 - it0 == 4);
  REQUIRE(it0 - it2 == -2);
}

TEST_CASE("MyVector iterator comparison during traversal") {
  MyVector<int> v;
  v.push(10);
  v.push(20);
  v.push(30);

  auto it = v.begin();
  auto end = v.end();

  int expected = 10;
  for (; it != end; ++it) {
    REQUIRE(*it == expected);
    expected += 10;
  }

  REQUIRE(it == end);
}

TEST_CASE("MyVector const_iterator comparison") {
  MyVector<int> v;
  v.push(1);
  v.push(2);

  const MyVector<int> &cv = v;

  auto it1 = cv.begin();
  auto it2 = cv.begin();

  REQUIRE(it1 == it2);
  REQUIRE_FALSE(it1 != it2);
}