#include <catch2/catch_test_macros.hpp>

#include <stdexcept>
#include <utility>

#include "deque.h"

TEST_CASE("my_stl::deque can be default constructed") {
  my_stl::deque<int> d;
  (void)d;
  SUCCEED();
}

TEST_CASE("my_stl::deque push_back and push_front preserve logical order") {
  my_stl::deque<int> d;

  d.push_back(20);
  d.push_back(30);
  d.push_front(10);

  REQUIRE(d.size() == 3);
  REQUIRE(d.front() == 10);
  REQUIRE(d.back() == 30);
  REQUIRE(d[0] == 10);
  REQUIRE(d[1] == 20);
  REQUIRE(d[2] == 30);
}

TEST_CASE("my_stl::deque pop_back and pop_front update ends") {
  my_stl::deque<int> d;

  d.push_back(10);
  d.push_back(20);
  d.push_front(5);

  d.pop_front();
  REQUIRE(d.front() == 10);
  REQUIRE(d.size() == 2);

  d.pop_back();
  REQUIRE(d.back() == 10);
  REQUIRE(d.size() == 1);

  d.pop_back();
  REQUIRE(d.empty());
  REQUIRE_THROWS_AS(d.pop_back(), std::out_of_range);
  REQUIRE_THROWS_AS(d.pop_front(), std::out_of_range);
}

TEST_CASE("my_stl::deque reverse iterators walk backward") {
  my_stl::deque<int> d;
  d.push_back(1);
  d.push_back(2);
  d.push_back(3);

  auto it = d.rbegin();

  REQUIRE(*it == 3);
  ++it;
  REQUIRE(*it == 2);
  ++it;
  REQUIRE(*it == 1);
  ++it;
  REQUIRE(it == d.rend());
}

TEST_CASE("my_stl::deque constructors create expected contents") {
  my_stl::deque<int> defaults(3);
  REQUIRE(defaults.size() == 3);
  REQUIRE(defaults[0] == 0);
  REQUIRE(defaults[1] == 0);
  REQUIRE(defaults[2] == 0);

  my_stl::deque<int> filled(3, 7);
  REQUIRE(filled.size() == 3);
  REQUIRE(filled.front() == 7);
  REQUIRE(filled.back() == 7);

  my_stl::deque<int> listed{1, 2, 3};
  REQUIRE(listed.size() == 3);
  REQUIRE(listed[0] == 1);
  REQUIRE(listed[1] == 2);
  REQUIRE(listed[2] == 3);
}

TEST_CASE("my_stl::deque copy and move preserve logical order") {
  my_stl::deque<int> wrapped;
  wrapped.push_back(1);
  wrapped.push_back(2);
  wrapped.push_back(3);
  wrapped.push_back(4);
  wrapped.pop_front();
  wrapped.pop_front();
  wrapped.push_back(5);
  wrapped.push_back(6);

  my_stl::deque<int> copied(wrapped);
  REQUIRE(copied.size() == 4);
  REQUIRE(copied[0] == 3);
  REQUIRE(copied[1] == 4);
  REQUIRE(copied[2] == 5);
  REQUIRE(copied[3] == 6);

  my_stl::deque<int> assigned;
  assigned = copied;
  REQUIRE(assigned.size() == 4);
  REQUIRE(assigned[0] == 3);
  REQUIRE(assigned[3] == 6);

  my_stl::deque<int> moved(std::move(wrapped));
  REQUIRE(moved.size() == 4);
  REQUIRE(moved.front() == 3);
  REQUIRE(moved.back() == 6);
  REQUIRE(wrapped.empty());

  my_stl::deque<int> move_assigned;
  move_assigned = std::move(moved);
  REQUIRE(move_assigned.size() == 4);
  REQUIRE(move_assigned[1] == 4);
  REQUIRE(moved.empty());
}

TEST_CASE("my_stl::deque initializer_list assignment replaces contents") {
  my_stl::deque<int> d{1, 2, 3};

  d = {8, 9};

  REQUIRE(d.size() == 2);
  REQUIRE(d.front() == 8);
  REQUIRE(d.back() == 9);
}

TEST_CASE("my_stl::deque insert and erase update logical order") {
  my_stl::deque<int> d{1, 4};

  d.insert(d.begin() + 1, 2);
  int three = 3;
  d.insert(d.begin() + 2, std::move(three));
  d.insert(d.begin(), 0);
  d.insert(d.end(), 5);

  REQUIRE(d.size() == 6);
  for (my_stl::deque<int>::size_type i = 0; i < d.size(); ++i) {
    REQUIRE(d[i] == static_cast<int>(i));
  }

  auto it = d.erase(d.begin() + 2);
  REQUIRE(*it == 3);
  REQUIRE(d.size() == 5);
  REQUIRE(d[0] == 0);
  REQUIRE(d[1] == 1);
  REQUIRE(d[2] == 3);
  REQUIRE(d[3] == 4);
  REQUIRE(d[4] == 5);

  it = d.erase(d.begin() + 1, d.begin() + 4);
  REQUIRE(*it == 5);
  REQUIRE(d.size() == 2);
  REQUIRE(d[0] == 0);
  REQUIRE(d[1] == 5);

  it = d.erase(d.begin() + 1, d.begin() + 1);
  REQUIRE(*it == 5);
  REQUIRE(d.size() == 2);
}

TEST_CASE("my_stl::deque resize and swap work together") {
  my_stl::deque<int> d{1, 2};

  d.resize(4, 9);
  REQUIRE(d.size() == 4);
  REQUIRE(d[0] == 1);
  REQUIRE(d[1] == 2);
  REQUIRE(d[2] == 9);
  REQUIRE(d[3] == 9);

  d.resize(1);
  REQUIRE(d.size() == 1);
  REQUIRE(d.front() == 1);

  my_stl::deque<int> other{7, 8};
  d.swap(other);

  REQUIRE(d.size() == 2);
  REQUIRE(d[0] == 7);
  REQUIRE(d[1] == 8);
  REQUIRE(other.size() == 1);
  REQUIRE(other.front() == 1);
}
