#include <algorithm>
#include <cstddef>
#include <memory>
#include <stdexcept>

class MyVector {
private:
  std::unique_ptr<int[]> elements; // 指向动态数组的指针
  size_t capacity_;                // 数组的容量
  size_t size_;

  // 扩展数组容量
  void reserve(size_t new_cap) {
    if (new_cap <= capacity_)
      return;

    auto new_buf = std::make_unique<int[]>(new_cap);

    for (size_t i = 0; i < size_; ++i) {
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
  MyVector() : elements(nullptr), capacity_(0), size_(0) {};

  // 析构函数
  ~MyVector() = default;

  // 拷贝构造函数
  MyVector(const MyVector &other)
      : size_(other.size_), capacity_(other.capacity_) {
    if (capacity_ > 0) {
      elements = std::make_unique<int[]>(capacity_);
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

  // 添加元素到数组末尾
  void push(const int &value) {
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
  void insert(size_t index, const int &value) {
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
  void push_back(const int &value) {
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

  // 迭代器
  // 使用迭代器遍历数组的开始位置
  int *begin() { return elements.get(); };
  // 使用迭代器遍历数组的结束位置
  int *end() { return elements.get() + size_; };

  // 使用迭代器遍历数组的开始位置（const版本）
  const int *begin() const { return elements.get(); }

  // 使用迭代器遍历数组的结束位置（const版本）
  const int *end() const { return elements.get() + size_; }
};