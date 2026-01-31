#include "list.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
int main() {
  {
    std::cout << "=== Basic push_back / front / back ===\n";
    MyList<int> lst;
    assert(lst.empty());
    assert(lst.size() == 0);

    lst.push_back(1);
    assert(!lst.empty());
    assert(lst.size() == 1);
    assert(lst.front() == 1);
    assert(lst.back() == 1);

    lst.push_back(2);
    lst.push_back(3);
    assert(lst.size() == 3);
    assert(lst.front() == 1);
    assert(lst.back() == 3);
  }

  {
    std::cout << "=== push_front ===\n";
    MyList<int> lst;
    lst.push_front(2);
    lst.push_front(1);
    lst.push_front(0);

    assert(lst.size() == 3);
    assert(lst.front() == 0);
    assert(lst.back() == 2);
  }

  {
    std::cout << "=== pop_front ===\n";
    MyList<int> lst;
    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);

    lst.pop_front();
    assert(lst.size() == 2);
    assert(lst.front() == 2);

    lst.pop_front();
    assert(lst.size() == 1);
    assert(lst.front() == 3);

    lst.pop_front();
    assert(lst.empty());
  }

  {
    std::cout << "=== pop_back ===\n";
    MyList<int> lst;
    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);

    lst.pop_back();
    assert(lst.size() == 2);
    assert(lst.back() == 2);

    lst.pop_back();
    assert(lst.size() == 1);
    assert(lst.back() == 1);

    lst.pop_back();
    assert(lst.empty());
  }

  {
    std::cout << "=== remove(value) ===\n";
    MyList<int> lst;
    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(2);
    lst.push_back(3);
    lst.push_back(2);

    lst.remove(2);
    assert(lst.size() == 2);
    assert(lst.front() == 1);
    assert(lst.back() == 3);
  }

  {
    std::cout << "=== remove all ===\n";
    MyList<int> lst;
    lst.push_back(5);
    lst.push_back(5);
    lst.push_back(5);

    lst.remove(5);
    assert(lst.empty());
  }

  {
    std::cout << "=== copy constructor ===\n";
    MyList<int> a;
    a.push_back(1);
    a.push_back(2);

    MyList<int> b(a); // copy ctor
    assert(b.size() == 2);
    assert(b.front() == 1);
    assert(b.back() == 2);

    a.pop_front();
    assert(b.front() == 1); // deep copy 验证
  }

  {
    std::cout << "=== copy assignment ===\n";
    MyList<int> a;
    a.push_back(1);
    a.push_back(2);

    MyList<int> b;
    b.push_back(9);

    b = a;
    assert(b.size() == 2);
    assert(b.front() == 1);
    assert(b.back() == 2);
  }

  {
    std::cout << "=== move constructor ===\n";
    MyList<int> a;
    a.push_back(1);
    a.push_back(2);

    MyList<int> b(std::move(a));
    assert(b.size() == 2);
    assert(b.front() == 1);
    assert(b.back() == 2);
    assert(a.empty()); // 被 move 走
  }

  {
    std::cout << "=== move assignment ===\n";
    MyList<int> a;
    a.push_back(1);
    a.push_back(2);

    MyList<int> b;
    b.push_back(9);

    b = std::move(a);
    assert(b.size() == 2);
    assert(b.front() == 1);
    assert(b.back() == 2);
    assert(a.empty());
  }

  std::cout << "\nAll tests passed ✅\n";
}
