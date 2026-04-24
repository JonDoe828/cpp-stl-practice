#include <catch2/catch_test_macros.hpp>

#include "vector.hpp"

TEST_CASE("my_stl::vector basic push and size") {
  my_stl::vector<int> v;
  REQUIRE(v.size() == 0);

  v.push_back(10);
  v.push_back(20);
  v.push_back(30);

  REQUIRE(v.size() == 3);
  REQUIRE(v[0] == 10);
  REQUIRE(v[1] == 20);
  REQUIRE(v[2] == 30);
}

TEST_CASE("my_stl::vector insert at arbitrary position") {
  my_stl::vector<int> v;
  v.push_back(20);
  v.push_back(30);
  v.push_back(40);

  v.insert(0, 10); // insert at head

  REQUIRE(v.size() == 4);
  REQUIRE(v[0] == 10);
  REQUIRE(v[1] == 20);
  REQUIRE(v[2] == 30);
  REQUIRE(v[3] == 40);
}

TEST_CASE("my_stl::vector pop removes last element") {
  my_stl::vector<int> v;
  v.push_back(10);
  v.push_back(20);
  v.push_back(30);

  v.pop_back();

  REQUIRE(v.size() == 2);
  REQUIRE(v[0] == 10);
  REQUIRE(v[1] == 20);
}

TEST_CASE("my_stl::vector clear resets vector") {
  my_stl::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);

  v.clear();

  REQUIRE(v.size() == 0);
  REQUIRE_THROWS_AS(v.at(0), std::out_of_range);
}

TEST_CASE("my_stl::vector empty reflects current size") {
  my_stl::vector<int> v;
  REQUIRE(v.empty());

  v.push_back(42);
  REQUIRE_FALSE(v.empty());

  v.clear();
  REQUIRE(v.empty());
}

TEST_CASE("my_stl::vector front and back access first and last element") {
  my_stl::vector<int> v;
  v.push_back(10);
  v.push_back(20);
  v.push_back(30);

  REQUIRE(v.front() == 10);
  REQUIRE(v.back() == 30);

  v.front() = 11;
  v.back() = 31;

  REQUIRE(v[0] == 11);
  REQUIRE(v[2] == 31);
}

TEST_CASE("my_stl::vector front and back throw on empty vector") {
  my_stl::vector<int> v;

  REQUIRE_THROWS_AS(v.front(), std::out_of_range);
  REQUIRE_THROWS_AS(v.back(), std::out_of_range);
}

TEST_CASE("my_stl::vector data exposes contiguous storage") {
  my_stl::vector<int> v;
  REQUIRE(v.data() == nullptr);

  v.push_back(7);
  v.push_back(8);
  v.push_back(9);

  REQUIRE(v.data() == &v[0]);
  REQUIRE(v.data()[1] == 8);
}

TEST_CASE("my_stl::vector const accessors support const vector") {
  my_stl::vector<int> v;
  v.push_back(5);
  v.push_back(6);

  const my_stl::vector<int> &cv = v;

  REQUIRE(cv.front() == 5);
  REQUIRE(cv.back() == 6);
  REQUIRE(cv.data() == &cv[0]);
}

TEST_CASE("my_stl::vector iterator implicitly converts to const_iterator") {
  my_stl::vector<int> v;
  v.push_back(10);
  v.push_back(20);

  my_stl::vector<int>::const_iterator cit = v.begin();

  REQUIRE(*cit == 10);
  REQUIRE(cit + 1 == v.cbegin() + 1);
}

TEST_CASE("my_stl::vector erase removes a single middle element") {
  my_stl::vector<int> v;
  v.push_back(10);
  v.push_back(20);
  v.push_back(30);
  v.push_back(40);

  auto it = v.erase(v.begin() + 1);

  REQUIRE(v.size() == 3);
  REQUIRE(v[0] == 10);
  REQUIRE(v[1] == 30);
  REQUIRE(v[2] == 40);
  REQUIRE(it == v.begin() + 1);
  REQUIRE(*it == 30);
}

TEST_CASE("my_stl::vector erase last element returns end") {
  my_stl::vector<int> v;
  v.push_back(10);
  v.push_back(20);
  v.push_back(30);

  auto it = v.erase(v.begin() + 2);

  REQUIRE(v.size() == 2);
  REQUIRE(v[0] == 10);
  REQUIRE(v[1] == 20);
  REQUIRE(it == v.end());
}

TEST_CASE("my_stl::vector erase range removes middle block") {
  my_stl::vector<int> v;
  v.push_back(10);
  v.push_back(20);
  v.push_back(30);
  v.push_back(40);
  v.push_back(50);

  auto it = v.erase(v.begin() + 1, v.begin() + 4);

  REQUIRE(v.size() == 2);
  REQUIRE(v[0] == 10);
  REQUIRE(v[1] == 50);
  REQUIRE(it == v.begin() + 1);
  REQUIRE(*it == 50);
}

TEST_CASE("my_stl::vector erase whole range clears vector") {
  my_stl::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);

  auto it = v.erase(v.begin(), v.end());

  REQUIRE(v.empty());
  REQUIRE(v.size() == 0);
  REQUIRE(it == v.end());
}

TEST_CASE("my_stl::vector erase empty range returns unchanged position") {
  my_stl::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);

  auto it = v.erase(v.begin() + 1, v.begin() + 1);

  REQUIRE(v.size() == 3);
  REQUIRE(v[0] == 1);
  REQUIRE(v[1] == 2);
  REQUIRE(v[2] == 3);
  REQUIRE(it == v.begin() + 1);
  REQUIRE(*it == 2);
}

TEST_CASE("my_stl::vector iterator traversal") {
  my_stl::vector<int> v;
  v.push_back(10);
  v.push_back(20);
  v.push_back(30);

  int expected = 10;
  for (auto it = v.begin(); it != v.end(); ++it) {
    REQUIRE(*it == expected);
    expected += 10;
  }
}

TEST_CASE("my_stl::vector foreach traversal") {
  my_stl::vector<int> v;
  v.push_back(10);
  v.push_back(20);
  v.push_back(30);

  int expected = 10;
  for (int x : v) {
    REQUIRE(x == expected);
    expected += 10;
  }
}

TEST_CASE("my_stl::vector iterator equality comparison") {
  my_stl::vector<int> v;
  v.push_back(1);
  v.push_back(2);

  auto it1 = v.begin();
  auto it2 = v.begin();
  auto it3 = v.end();

  REQUIRE(it1 == it2);
  REQUIRE_FALSE(it1 != it2);

  REQUIRE(it1 != it3);
  REQUIRE_FALSE(it1 == it3);
}

TEST_CASE("my_stl::vector iterator ordering comparison") {
  my_stl::vector<int> v;
  v.push_back(10);
  v.push_back(20);
  v.push_back(30);

  auto it0 = v.begin();
  auto it1 = it0 + 1;
  auto it2 = it0 + 2;

  REQUIRE(it0 < it1);
  REQUIRE(it1 < it2);

  REQUIRE(it2 > it1);
  REQUIRE(it1 > it0);
}

TEST_CASE("my_stl::vector iterator <= >= comparison") {
  my_stl::vector<int> v;
  v.push_back(5);
  v.push_back(6);

  auto it0 = v.begin();
  auto it1 = it0 + 1;

  REQUIRE(it0 <= it0);
  REQUIRE(it0 <= it1);
  REQUIRE(it1 >= it1);
  REQUIRE(it1 >= it0);
}

TEST_CASE("my_stl::vector iterator difference") {
  my_stl::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);

  auto it0 = v.begin();
  auto it2 = it0 + 2;
  auto it4 = v.end();

  REQUIRE(it2 - it0 == 2);
  REQUIRE(it4 - it0 == 4);
  REQUIRE(it0 - it2 == -2);
}

TEST_CASE("my_stl::vector iterator comparison during traversal") {
  my_stl::vector<int> v;
  v.push_back(10);
  v.push_back(20);
  v.push_back(30);

  auto it = v.begin();
  auto end = v.end();

  int expected = 10;
  for (; it != end; ++it) {
    REQUIRE(*it == expected);
    expected += 10;
  }

  REQUIRE(it == end);
}

TEST_CASE("my_stl::vector const_iterator comparison") {
  my_stl::vector<int> v;
  v.push_back(1);
  v.push_back(2);

  const my_stl::vector<int> &cv = v;

  auto it1 = cv.begin();
  auto it2 = cv.begin();

  REQUIRE(it1 == it2);
  REQUIRE_FALSE(it1 != it2);
}
