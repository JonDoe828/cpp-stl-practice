#include "list.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("basic", "[MyList]") {
  MyList<int> lst;
  lst.push_back(1);
  REQUIRE(lst.front() == 1);
}

TEST_CASE("rvalue push_back makes list non-empty", "[MyList]") {
  MyList<int> lst;

  // 这句会优先调用 push_back(T&&)
  lst.push_back(1);

  REQUIRE_FALSE(lst.empty());
  REQUIRE(lst.size() == 1);
  REQUIRE(lst.front() == 1);
  REQUIRE(lst.back() == 1);
}

TEST_CASE("pop_back after two push_back does not crash", "[MyList]") {
  MyList<int> lst;

  lst.push_back(1);
  lst.push_back(2);

  REQUIRE(lst.size() == 2);
  REQUIRE(lst.front() == 1);
  REQUIRE(lst.back() == 2);

  // 你现在崩溃的点基本就会在这里炸
  REQUIRE_NOTHROW(lst.pop_back());

  REQUIRE(lst.size() == 1);
  REQUIRE(lst.back() == 1);
}

TEST_CASE("mix push_front + pop_back keeps links consistent", "[MyList]") {
  MyList<int> lst;

  lst.push_front(2); // [2]
  lst.push_front(1); // [1,2]
  lst.pop_back();    // -> [1]

  REQUIRE(lst.size() == 1);
  REQUIRE(lst.front() == 1);
  REQUIRE(lst.back() == 1);
}