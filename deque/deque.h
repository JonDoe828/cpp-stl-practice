#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <utility>

namespace my_stl {

template <typename T> class deque {
public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  deque() = default;
  // 数量构造
  explicit deque(size_type count)
      : data_(count == 0 ? nullptr : std::make_unique<T[]>(count)),
        capacity_(count), size_(count), front_(0) {}
  deque(size_type count, const T &value)
      : data_(count == 0 ? nullptr : std::make_unique<T[]>(count)),
        capacity_(count), size_(count), front_(0) {
    for (size_type i = 0; i < size_; ++i) {
      data_[i] = value;
    }
  }
  // 初始化列表
  deque(std::initializer_list<T> init)
      : data_(init.size() == 0 ? nullptr : std::make_unique<T[]>(init.size())),
        capacity_(init.size()), size_(init.size()), front_(0) {
    size_type i = 0;
    for (const auto &value : init) {
      data_[i++] = value;
    }
  }

  ~deque() = default;

  // 拷贝构造函数
  deque(const deque &other)
      : data_(other.capacity_ == 0 ? nullptr
                                   : std::make_unique<T[]>(other.capacity_)),
        capacity_(other.capacity_), size_(other.size_), front_(0) {
    for (size_type i = 0; i < size_; ++i) {
      data_[i] = other[i];
    }
  }

  // Move constructor
  deque(deque &&other) noexcept
      : data_(std::exchange(other.data_, std::unique_ptr<T[]>{})),
        capacity_(std::exchange(other.capacity_, 0)),
        size_(std::exchange(other.size_, 0)),
        front_(std::exchange(other.front_, 0)) {}

  // 拷贝赋值操作符
  deque &operator=(const deque &other) {
    if (this == &other)
      return *this;

    deque tmp(other); // 复用拷贝构造
    swap(tmp);        // 交换资源
    return *this;
  }

  // Move assignment
  deque &operator=(deque &&other) noexcept {
    if (this == &other)
      return *this;

    data_ = std::exchange(other.data_, std::unique_ptr<T[]>{});
    size_ = std::exchange(other.size_, 0);
    capacity_ = std::exchange(other.capacity_, 0);
    front_ = std::exchange(other.front_, 0);
    return *this;
  }

  deque &operator=(std::initializer_list<T> init) {
    deque tmp(init);
    swap(tmp);
    return *this;
  }

  class iterator {
    friend class deque;
    friend class const_iterator;

  public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;

    iterator() noexcept : owner_(nullptr), index_(0) {}
    reference operator*() const { return (*owner_)[index_]; }
    pointer operator->() const { return std::addressof((*owner_)[index_]); }

    iterator &operator++() {
      ++index_;
      return *this;
    }
    iterator operator++(int) {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    iterator &operator--() {
      --index_;
      return *this;
    }
    iterator operator--(int) {
      iterator tmp = *this;
      --(*this);
      return tmp;
    }

    iterator &operator+=(difference_type n) {
      index_ += n;
      return *this;
    }
    iterator &operator-=(difference_type n) {
      index_ -= n;
      return *this;
    }

    iterator operator+(difference_type n) const {
      return iterator(owner_, index_ + n);
    }
    iterator operator-(difference_type n) const {
      return iterator(owner_, index_ - n);
    }
    reference operator[](difference_type n) const { return *(*this + n); }
    difference_type operator-(const iterator &rhs) const {
      return static_cast<difference_type>(index_) -
             static_cast<difference_type>(rhs.index_);
    }

    friend iterator operator+(difference_type n, const iterator &it) {
      return it + n;
    }

    auto operator<=>(const iterator &) const = default;

  private:
    deque *owner_;
    size_type index_;

    iterator(deque *owner, size_type index) : owner_(owner), index_(index) {}
  };
  class const_iterator {
    friend class deque;

  public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = const T *;
    using reference = const T &;

    const_iterator() noexcept : owner_(nullptr), index_(0) {}
    const_iterator(const iterator &it) : owner_(it.owner_), index_(it.index_) {}

    reference operator*() const { return (*owner_)[index_]; }
    pointer operator->() const { return std::addressof((*owner_)[index_]); }
    const_iterator &operator++() {
      ++index_;
      return *this;
    }
    const_iterator operator++(int) {
      const_iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    const_iterator &operator--() {
      --index_;
      return *this;
    }
    const_iterator operator--(int) {
      const_iterator tmp = *this;
      --(*this);
      return tmp;
    }
    const_iterator &operator+=(difference_type n) {
      index_ += n;
      return *this;
    }
    const_iterator &operator-=(difference_type n) {
      index_ -= n;
      return *this;
    }
    const_iterator operator+(difference_type n) const {
      return const_iterator(owner_, index_ + n);
    }
    const_iterator operator-(difference_type n) const {
      return const_iterator(owner_, index_ - n);
    }
    reference operator[](difference_type n) const { return *(*this + n); }
    difference_type operator-(const const_iterator &rhs) const {
      return static_cast<difference_type>(index_) -
             static_cast<difference_type>(rhs.index_);
    }

    friend const_iterator operator+(difference_type n,
                                    const const_iterator &it) {
      return it + n;
    }

    auto operator<=>(const const_iterator &) const = default;

  private:
    const deque *owner_;
    size_type index_;

    const_iterator(const deque *owner, size_type index)
        : owner_(owner), index_(index) {}
  };

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  reference operator[](size_type pos) { return data_[physical_index(pos)]; }
  const_reference operator[](size_type pos) const {
    return data_[physical_index(pos)];
  }

  reference at(size_type pos) {
    if (pos >= size_) {
      throw std::out_of_range("deque::at out of range");
    }
    return (*this)[pos];
  }
  const_reference at(size_type pos) const {
    if (pos >= size_) {
      throw std::out_of_range("deque::at out of range");
    }
    return (*this)[pos];
  }

  reference front() {
    if (empty()) {
      throw std::out_of_range("deque::front on empty deque");
    }
    return (*this)[0];
  }
  const_reference front() const {
    if (empty()) {
      throw std::out_of_range("deque::front on empty deque");
    }
    return (*this)[0];
  }

  reference back() {
    if (empty()) {
      throw std::out_of_range("deque::back on empty deque");
    }
    return (*this)[size_ - 1];
  }
  const_reference back() const {
    if (empty()) {
      throw std::out_of_range("deque::back on empty deque");
    }
    return (*this)[size_ - 1];
  }

  iterator begin() noexcept { return iterator(this, 0); }
  iterator end() noexcept { return iterator(this, size_); }

  const_iterator begin() const noexcept { return const_iterator(this, 0); }
  const_iterator end() const noexcept { return const_iterator(this, size_); }

  const_iterator cbegin() const noexcept { return begin(); }
  const_iterator cend() const noexcept { return end(); }

  reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
  reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
  const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(cend());
  }
  const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(cbegin());
  }

  const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator(cend());
  }
  const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(cbegin());
  }

  bool empty() const noexcept { return size_ == 0; }
  size_type size() const noexcept { return size_; }

  void clear() noexcept {
    size_ = 0;
    front_ = 0;
  }

  void push_back(const T &value) {
    if (size_ == capacity_) {
      reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
    }

    data_[physical_index(size_)] = value;
    ++size_;
  }
  void push_back(T &&value) {
    if (size_ == capacity_) {
      reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
    }

    data_[physical_index(size_)] = std::move(value);
    ++size_;
  }
  void push_front(const T &value) {
    if (size_ == capacity_) {
      reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
    }

    front_ = (front_ + capacity_ - 1) % capacity_;
    data_[front_] = value;
    ++size_;
  }
  void push_front(T &&value) {
    if (size_ == capacity_) {
      reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
    }

    front_ = (front_ + capacity_ - 1) % capacity_;
    data_[front_] = std::move(value);
    ++size_;
  }

  void pop_back() {
    if (empty()) {
      throw std::out_of_range("deque::pop_back on empty deque");
    }

    --size_;

    if (size_ == 0) {
      front_ = 0;
    }
  }
  void pop_front() {
    if (empty()) {
      throw std::out_of_range("deque::pop_front on empty deque");
    }

    front_ = (front_ + 1) % capacity_;
    --size_;

    if (size_ == 0) {
      front_ = 0;
    }
  }

  iterator insert(const_iterator pos, const T &value) {
    size_type index = pos.index_;

    if (index > size_) {
      throw std::out_of_range("deque::insert position out of range");
    }

    if (size_ == capacity_) {
      reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
    }

    for (size_type i = size_; i > index; --i) {
      (*this)[i] = std::move((*this)[i - 1]);
    }

    (*this)[index] = value;
    ++size_;

    return iterator(this, index);
  }
  iterator insert(const_iterator pos, T &&value) {
    size_type index = pos.index_;

    if (index > size_) {
      throw std::out_of_range("deque::insert position out of range");
    }

    if (size_ == capacity_) {
      reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
    }

    for (size_type i = size_; i > index; --i) {
      (*this)[i] = std::move((*this)[i - 1]);
    }

    (*this)[index] = std::move(value);
    ++size_;

    return iterator(this, index);
  }

  iterator erase(const_iterator pos) { return erase(pos, pos + 1); }
  iterator erase(const_iterator first, const_iterator last) {
    size_type first_index = first - cbegin();
    size_type last_index = last - cbegin();

    if (first_index > last_index || last_index > size_) {
      throw std::out_of_range("deque::erase range out of range");
    }

    size_type count = last_index - first_index;

    if (count == 0) {
      return begin() + static_cast<difference_type>(first_index);
    }

    for (size_type i = first_index; i + count < size_; ++i) {
      (*this)[i] = std::move((*this)[i + count]);
    }

    size_ -= count;

    if (size_ == 0) {
      front_ = 0;
    }

    return begin() + static_cast<difference_type>(first_index);
  }

  void resize(size_type count) { resize(count, T{}); }
  void resize(size_type count, const T &value) {
    while (size_ > count) {
      pop_back();
    }

    while (size_ < count) {
      push_back(value);
    }
  }

  void swap(deque &other) noexcept {
    std::swap(data_, other.data_);
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
    std::swap(front_, other.front_);
  }

private:
  std::unique_ptr<T[]> data_;
  size_type capacity_{0};
  size_type size_{0};
  size_type front_{0};

  size_type physical_index(size_type logical_index) const noexcept {
    return (front_ + logical_index) % capacity_;
  }
  void reallocate(size_type new_capacity) {
    auto new_data = std::make_unique<T[]>(new_capacity);

    for (size_type i = 0; i < size_; ++i) {
      new_data[i] = std::move((*this)[i]);
    }

    data_ = std::move(new_data);
    capacity_ = new_capacity;
    front_ = 0;
  }
};
} // namespace my_stl
