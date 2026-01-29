#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <utility>

template <typename T> class MyVector {
private:
  std::unique_ptr<T[]> elements; // 指向动态数组的指针
  size_t capacity_;              // 数组的容量
  size_t size_;

  // 扩展数组容量
  void reserve(size_t new_cap) {
    if (new_cap <= capacity_)
      return;

    auto new_buf = std::make_unique<T[]>(new_cap);

    for (std::size_t i = 0; i < size_; ++i) {
      new_buf[i] = elements[i];
    }

    elements = std::move(new_buf);
    capacity_ = new_cap;
  }

  void swap(MyVector &other) noexcept {
    std::swap(elements, other.elements);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

public:
  // 构造函数
  // default user provied constructor
  MyVector() : elements(nullptr), capacity_(0), size_(0) {};

  MyVector(std::initializer_list<T> ilist)
      : elements(std::make_unique<T[]>(ilist.size())), size_(ilist.size()),
        capacity_(ilist.size()) {
    std::size_t i = 0;
    for (const auto &elem : ilist) {
      elements[i++] = elem;
    }
  };

  // 析构函数
  ~MyVector() = default;

  // 拷贝构造函数
  MyVector(const MyVector &other)
      : size_(other.size_), capacity_(other.capacity_) {
    if (capacity_ > 0) {
      elements = std::make_unique<T[]>(capacity_);
      std::copy(other.elements.get(), other.elements.get() + size_,
                elements.get());
    }
  }

  // 拷贝赋值操作符
  MyVector &operator=(const MyVector &other) {
    if (this == &other)
      return *this;

    MyVector tmp(other); // 复用拷贝构造
    swap(tmp);           // 交换资源
    return *this;
  }

  // Move constructor
  MyVector(MyVector &&other) noexcept
      : elements(std::exchange(other.elements, std::unique_ptr<T[]>{})),
        size_(std::exchange(other.size_, 0)),
        capacity_(std::exchange(other.capacity_, 0)) {}

  // Move assignment
  MyVector &operator=(MyVector &&other) {
    if (this == other)
      return *this;

    elements.reset();
    elements = std::exchange(other.elements, std::unique_ptr<T[]>{});
    size_ = std::exchange(other.size_, 0);
    capacity_ = std::exchange(other.capacity_, 0);
    return *this;
  }
  T &operator[](std::size_t pos) { return elements[pos]; }
  const T &operator[](std::size_t pos) const { return elements[pos]; }

  // member function
  //  添加元素到数组末尾
  void push(const T &value) {
    if (size_ == capacity_) {
      // 如果数组已满，扩展容量
      reserve(capacity_ == 0 ? 1 : 2 * capacity_);
    }
    elements[size_++] = value;
  };

  // 删除数组末尾的元素
  void pop() {
    if (size_ > 0) {
      --size_;
    }
  };

  // 在指定位置插入元素
  void insert(size_t index, const T &value) {
    if (index > size_) {
      throw std::out_of_range("Index out of range");
    }
    if (size_ == capacity_) {
      reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    for (size_t i = size_; i > index; --i) {
      elements[i] = elements[i - 1];
    }
    elements[index] = value;
    ++size_;
  };
  // 清空数组
  void clear() { size_ = 0; }

  // 添加元素到数组末尾
  void push_back(const T &value) {
    if (size_ == capacity_) {
      // 如果数组已满，扩展容量
      reserve(capacity_ == 0 ? 1 : 2 * capacity_);
    }
    elements[size_++] = value;
  }

  // 获取数组中元素的个数
  size_t size() const { return size_; }

  // 获取数组的容量
  size_t capacity() const { return capacity_; }

  int get(int index) const {
    if (index < 0 || static_cast<size_t>(index) >= size_) {
      return -1;
    }
    return elements[index];
  };

  // 打印数组中的元素
  void printElements() const {
    for (size_t i = 0; i < size_; ++i) {
      std::cout << elements[i] << " ";
    }
    std::cout << std::endl;
  }

  class iterator {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = value_type *;
    using reference = value_type &;

    iterator() noexcept : ptr_(nullptr) {}
    explicit iterator(pointer p) noexcept : ptr_(p) {}

    reference operator*() const noexcept { return *ptr_; }
    pointer operator->() const noexcept { return ptr_; }

    iterator operator+(difference_type n) const noexcept {
      return iterator(ptr_ + n);
    }
    iterator operator-(difference_type n) const noexcept {
      return iterator(ptr_ - n);
    }

    friend difference_type operator-(const iterator &a,
                                     const iterator &b) noexcept {
      return a.ptr_ - b.ptr_;
    }

    iterator &operator++() noexcept {
      ++ptr_;
      return *this;
    }
    iterator operator++(int) noexcept {
      iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    iterator &operator--() noexcept {
      --ptr_;
      return *this;
    }
    iterator operator--(int) noexcept {
      iterator tmp(*this);
      --(*this);
      return tmp;
    }

    reference operator[](difference_type n) const noexcept { return ptr_[n]; }

    // friend bool operator==(const iterator &a, const iterator &b) {}
    // friend bool operator!=(const iterator &a, const iterator &b) {}

    // friend bool operator<(const iterator &a, const iterator &b) {}
    // friend bool operator>(const iterator &a, const iterator &b) {}
    // friend bool operator<=(const iterator &a, const iterator &b) {}
    // friend bool operator>=(const iterator &a, const iterator &b) {}

    auto operator<=>(const iterator &) const = default;

  private:
    pointer ptr_;
  };

  class const_iterator {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type *;
    using reference = const value_type &;

    const_iterator() noexcept : ptr_(nullptr) {}
    explicit const_iterator(pointer p) noexcept : ptr_(p) {}

    reference operator*() const noexcept { return *ptr_; }
    pointer operator->() const noexcept { return ptr_; }

    const_iterator &operator++() noexcept {
      ++ptr_;
      return *this;
    }
    const_iterator operator++(int) noexcept {
      const_iterator tmp(*this);
      ++(*this);
      return tmp;
    }
    const_iterator &operator--() noexcept {
      --ptr_;
      return *this;
    }
    const_iterator operator--(int) noexcept {
      const_iterator tmp(*this);
      --(*this);
      return tmp;
    }

    const_iterator operator+(difference_type n) const noexcept {
      return const_iterator(ptr_ + n);
    }
    const_iterator operator-(difference_type n) const noexcept {
      return const_iterator(ptr_ - n);
    }

    difference_type operator-(const const_iterator &other) const noexcept {
      return ptr_ - other.ptr_;
    }

    reference operator[](difference_type n) const noexcept { return ptr_[n]; }

    auto operator<=>(const const_iterator &) const = default;

  private:
    pointer ptr_;
  };

  // 迭代器 interface
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  iterator begin() noexcept { return iterator(elements.get()); };
  iterator end() noexcept { return iterator(elements.get() + size_); };

  const_iterator begin() const noexcept {
    return const_iterator(elements.get());
  }
  const_iterator end() const noexcept {
    return const_iterator(elements.get() + size_);
  }

  const_iterator cbegin() const noexcept {
    return const_iterator(elements.get());
  }
  const_iterator cend() const noexcept { return end(); }

  const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(begin());
  }
};