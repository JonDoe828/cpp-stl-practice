#include <catch2/catch_test_macros.hpp>

#include "vector.hpp"

TEST_CASE("MyVector basic push and size") {
  MyVector v;
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
  MyVector v;
  v.push(10);

  REQUIRE(v.get(-1) == -1);
  REQUIRE(v.get(1) == -1);
}

TEST_CASE("MyVector insert at arbitrary position") {
  MyVector v;
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
  MyVector v;
  v.push(10);
  v.push(20);
  v.push(30);

  v.pop();

  REQUIRE(v.size() == 2);
  REQUIRE(v.get(0) == 10);
  REQUIRE(v.get(1) == 20);
}

TEST_CASE("MyVector clear resets vector") {
  MyVector v;
  v.push(1);
  v.push(2);
  v.push(3);

  v.clear();

  REQUIRE(v.size() == 0);
  REQUIRE(v.get(0) == -1);
}

TEST_CASE("MyVector iterator traversal") {
  MyVector v;
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
  MyVector v;
  v.push(10);
  v.push(20);
  v.push(30);

  int expected = 10;
  for (int x : v) {
    REQUIRE(x == expected);
    expected += 10;
  }
}