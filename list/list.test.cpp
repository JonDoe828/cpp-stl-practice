#include "list.hpp"
#include <catch2/catch_test_macros.hpp>

#include <stdexcept>
#include <utility>

namespace {
struct NoDefault {
  int value;

  explicit NoDefault(int value) : value(value) {}
};
} // namespace

TEST_CASE("basic", "[list]") {
  my_stl::list<int> lst;
  lst.push_back(1);
  REQUIRE(lst.front() == 1);
}

TEST_CASE("rvalue push_back makes list non-empty", "[list]") {
  my_stl::list<int> lst;

  // 这句会优先调用 push_back(T&&)
  lst.push_back(1);

  REQUIRE_FALSE(lst.empty());
  REQUIRE(lst.size() == 1);
  REQUIRE(lst.front() == 1);
  REQUIRE(lst.back() == 1);
}

TEST_CASE("sentinel node does not require default-constructible values",
          "[list]") {
  my_stl::list<NoDefault> lst;
  REQUIRE(lst.empty());

  NoDefault one(1);
  lst.push_back(one);
  lst.push_front(NoDefault(0));

  REQUIRE(lst.size() == 2);
  REQUIRE(lst.front().value == 0);
  REQUIRE(lst.back().value == 1);
  REQUIRE(lst.begin()->value == 0);
}

TEST_CASE("constructors copy and move preserve list contents", "[list]") {
  my_stl::list<int> filled(3, 7);
  REQUIRE(filled.size() == 3);
  REQUIRE(filled.front() == 7);
  REQUIRE(filled.back() == 7);

  my_stl::list<int> listed{1, 2, 3};
  REQUIRE(listed.size() == 3);
  REQUIRE(listed.get(0) == 1);
  REQUIRE(listed.get(1) == 2);
  REQUIRE(listed.get(2) == 3);

  my_stl::list<int> copied(listed);
  listed.pop_front();
  REQUIRE(copied.size() == 3);
  REQUIRE(copied.front() == 1);
  REQUIRE(copied.back() == 3);

  my_stl::list<int> assigned;
  assigned = copied;
  REQUIRE(assigned.size() == 3);
  REQUIRE(assigned.front() == 1);
  REQUIRE(assigned.back() == 3);

  my_stl::list<int> moved(std::move(assigned));
  REQUIRE(moved.size() == 3);
  REQUIRE(moved.front() == 1);
  REQUIRE(moved.back() == 3);
  REQUIRE(assigned.empty());

  my_stl::list<int> move_assigned;
  move_assigned = std::move(moved);
  REQUIRE(move_assigned.size() == 3);
  REQUIRE(move_assigned.get(1) == 2);
  REQUIRE(moved.empty());
}

TEST_CASE("swap exchanges list contents", "[list]") {
  my_stl::list<int> first{1, 2, 3};
  my_stl::list<int> second{8, 9};

  first.swap(second);

  REQUIRE(first.size() == 2);
  REQUIRE(first.front() == 8);
  REQUIRE(first.back() == 9);
  REQUIRE(second.size() == 3);
  REQUIRE(second.front() == 1);
  REQUIRE(second.back() == 3);

  my_stl::list<int> empty;
  first.swap(empty);

  REQUIRE(first.empty());
  REQUIRE(empty.size() == 2);
  REQUIRE(empty.front() == 8);
  REQUIRE(empty.back() == 9);
}

TEST_CASE("iterators walk forward and backward", "[list]") {
  my_stl::list<int> lst{1, 2, 3};

  auto it = lst.begin();
  REQUIRE(*it == 1);
  ++it;
  REQUIRE(*it == 2);
  it++;
  REQUIRE(*it == 3);
  ++it;
  REQUIRE(it == lst.end());

  --it;
  REQUIRE(*it == 3);
  it--;
  REQUIRE(*it == 2);

  const my_stl::list<int> &clst = lst;
  my_stl::list<int>::const_iterator cit = clst.begin();
  REQUIRE(cit == lst.begin());
  REQUIRE(lst.begin() == cit);
  REQUIRE(*cit == 1);
}

TEST_CASE("reverse iterators walk backward", "[list]") {
  my_stl::list<int> lst{1, 2, 3};

  auto it = lst.rbegin();
  REQUIRE(*it == 3);
  ++it;
  REQUIRE(*it == 2);
  ++it;
  REQUIRE(*it == 1);
  ++it;
  REQUIRE(it == lst.rend());
}

TEST_CASE("insert supports front middle and end positions", "[list]") {
  my_stl::list<int> lst{1, 4};

  int zero = 0;
  auto inserted = lst.insert(lst.begin(), zero);
  REQUIRE(*inserted == 0);

  int two = 2;
  auto middle = lst.begin();
  ++middle;
  ++middle;
  inserted = lst.insert(middle, std::move(two));
  REQUIRE(*inserted == 2);

  inserted = lst.insert(lst.end(), 5);
  REQUIRE(*inserted == 5);

  REQUIRE(lst.size() == 5);
  REQUIRE(lst.get(0) == 0);
  REQUIRE(lst.get(1) == 1);
  REQUIRE(lst.get(2) == 2);
  REQUIRE(lst.get(3) == 4);
  REQUIRE(lst.get(4) == 5);
}

TEST_CASE("erase removes single nodes and ranges", "[list]") {
  my_stl::list<int> lst{0, 1, 2, 3, 4, 5};

  auto middle = lst.begin();
  ++middle;
  ++middle;
  auto it = lst.erase(middle);
  REQUIRE(*it == 3);
  REQUIRE(lst.size() == 5);
  REQUIRE(lst.get(0) == 0);
  REQUIRE(lst.get(1) == 1);
  REQUIRE(lst.get(2) == 3);
  REQUIRE(lst.get(3) == 4);
  REQUIRE(lst.get(4) == 5);

  auto first = lst.begin();
  ++first;
  auto last = lst.end();
  --last;
  it = lst.erase(first, last);
  REQUIRE(*it == 5);
  REQUIRE(lst.size() == 2);
  REQUIRE(lst.front() == 0);
  REQUIRE(lst.back() == 5);

  it = lst.erase(lst.begin(), lst.begin());
  REQUIRE(*it == 0);
  REQUIRE(lst.size() == 2);

  it = lst.erase(lst.begin(), lst.end());
  REQUIRE(lst.empty());
  REQUIRE(it == lst.end());
}

TEST_CASE("empty access and invalid erases throw", "[list]") {
  my_stl::list<int> lst;

  REQUIRE_THROWS_AS(lst.front(), std::out_of_range);
  REQUIRE_THROWS_AS(lst.back(), std::out_of_range);
  REQUIRE_THROWS_AS(lst.pop_front(), std::out_of_range);
  REQUIRE_THROWS_AS(lst.pop_back(), std::out_of_range);
  REQUIRE_THROWS_AS(lst.erase(lst.end()), std::out_of_range);
  REQUIRE_THROWS_AS(lst.get(0), std::out_of_range);
}

TEST_CASE("pop_back after two push_back does not crash", "[list]") {
  my_stl::list<int> lst;

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

TEST_CASE("mix push_front + pop_back keeps links consistent", "[list]") {
  my_stl::list<int> lst;

  lst.push_front(2); // [2]
  lst.push_front(1); // [1,2]
  lst.pop_back();    // -> [1]

  REQUIRE(lst.size() == 1);
  REQUIRE(lst.front() == 1);
  REQUIRE(lst.back() == 1);
}
