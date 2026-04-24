#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <utility>

namespace my_stl {

template <typename T> class vector {
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

  void swap(vector &other) noexcept {
    std::swap(elements, other.elements);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

public:
  using value_type = T;
  using size_type = std::size_t;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;
  // 构造函数
  // default user provied constructor
  vector() : elements(nullptr), capacity_(0), size_(0) {};

  vector(std::initializer_list<T> ilist)
      : elements(std::make_unique<T[]>(ilist.size())), size_(ilist.size()),
        capacity_(ilist.size()) {
    std::size_t i = 0;
    for (const auto &elem : ilist) {
      elements[i++] = elem;
    }
  };

  // 析构函数
  ~vector() = default;

  // 拷贝构造函数
  vector(const vector &other) : size_(other.size_), capacity_(other.capacity_) {
    if (capacity_ > 0) {
      elements = std::make_unique<T[]>(capacity_);
      std::copy(other.elements.get(), other.elements.get() + size_,
                elements.get());
    }
  }

  // 拷贝赋值操作符
  vector &operator=(const vector &other) {
    if (this == &other)
      return *this;

    vector tmp(other); // 复用拷贝构造
    swap(tmp);         // 交换资源
    return *this;
  }

  // Move constructor
  vector(vector &&other) noexcept
      : elements(std::exchange(other.elements, std::unique_ptr<T[]>{})),
        size_(std::exchange(other.size_, 0)),
        capacity_(std::exchange(other.capacity_, 0)) {}

  // Move assignment
  vector &operator=(vector &&other) {
    if (this == &other)
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

  // 删除数组末尾的元素
  void pop_back() {
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

  bool empty() const noexcept { return size_ == 0; }

  reference front() {
    if (empty())
      throw std::out_of_range("vector::front on empty vector");
    return elements[0];
  }
  const_reference front() const {
    if (empty())
      throw std::out_of_range("vector::front on empty vector");
    return elements[0];
  }

  reference back() {
    if (empty())
      throw std::out_of_range("vector::back on empty vector");
    return elements[size_ - 1];
  }
  const_reference back() const {
    if (empty())
      throw std::out_of_range("vector::back on empty vector");
    return elements[size_ - 1];
  }

  pointer data() noexcept { return elements.get(); }
  const_pointer data() const noexcept { return elements.get(); }

  // 打印数组中的元素
  void printElements() const {
    for (size_t i = 0; i < size_; ++i) {
      std::cout << elements[i] << " ";
    }
    std::cout << std::endl;
  }

  // at
  T &at(std::size_t pos) {
    if (pos >= size_)
      throw std::out_of_range("SmartContainer::at out of range");
    return elements[pos];
  };
  const T &at(std::size_t pos) const {
    if (pos >= size_)
      throw std::out_of_range("SmartContainer::at out of range");
    return elements[pos];
  };

  class const_iterator;

  class iterator {
    friend class const_iterator;

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
    const_iterator(const iterator &other) noexcept : ptr_(other.ptr_) {}

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

  iterator begin() noexcept { return iterator(data()); };
  iterator end() noexcept {
    pointer p = data();
    return iterator(size_ == 0 ? p : p + size_);
  };

  const_iterator begin() const noexcept { return const_iterator(data()); }
  const_iterator end() const noexcept {
    const_pointer p = data();
    return const_iterator(size_ == 0 ? p : p + size_);
  }

  const_iterator cbegin() const noexcept { return const_iterator(data()); }
  const_iterator cend() const noexcept { return end(); }

  const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(begin());
  }

  iterator erase(const_iterator pos) { return erase(pos, pos + 1); }
  iterator erase(const_iterator first, const_iterator last) {
    size_type first_index = first - cbegin();
    size_type last_index = last - cbegin();
    size_type count = last_index - first_index;

    if (count == 0) {
      return begin() + static_cast<std::ptrdiff_t>(first_index);
    }

    for (size_type i = first_index; i + count < size_; ++i) {
      elements[i] = std::move(elements[i + count]);
    }

    size_ -= count;
    return begin() + static_cast<std::ptrdiff_t>(first_index);
  }
};
} // namespace my_stl
