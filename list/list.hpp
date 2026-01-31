#include <cstddef>
#include <iterator>
#include <utility>
template <typename T> class MyList {
private:
  struct Node {
    Node *prev;
    Node *next;
    T value;

    Node(const T &v) : prev(nullptr), next(nullptr), value(v) {}
    Node(T &&v) : prev(nullptr), next(nullptr), value(std::move(v)) {}
  };

  void swap(MyList &other) noexcept {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
  }

public:
  class iterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;

    iterator() noexcept = default;
    reference operator*() const { return *node_; };
    pointer operator->() const { return node_; };

    iterator &operator++() {
      node_ = node_->next;
      return *this;
    };
    iterator operator++(int) {
      iterator tmp(*this);
      node_ = node_->next;
      return tmp;
    };
    iterator &operator--() {
      node_ = node_->prev;
      return *this;
    };
    iterator operator--(int) {
      iterator tmp(*this);
      node_ = node_->prev;
      return tmp;
    };

    bool operator==(const iterator &other) const noexcept {
      return node_ == other.node_;
    };
    // bool operator!=(const iterator &other) const noexcept {
    //   return !(*this == other);
    // };

    // auto operator<=>(const MyList &) const = default;

  private:
    Node *node_;

    explicit iterator(Node *p) : node_(p) {};
    friend class list;
  };
  class const_iterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T *;
    using reference = const T &;

    const_iterator() noexcept = default;
    const_iterator(const iterator &it) : node_(it.node_) {};

    reference operator*() const { return *node_; };
    pointer operator->() const { return node_; };

    const_iterator &operator++() {
      node_ = node_->next;
      return *this;
    };
    const_iterator operator++(int) {
      const_iterator tmp(*this);
      node_ = node_->next;
      return tmp;
    };
    const_iterator &operator--() {
      node_ = node_->prev;
      return *this;
    };
    const_iterator operator--(int) {
      const_iterator tmp(*this);
      node_ = node_->prev;
      return tmp;
    };

    bool operator==(const const_iterator &other) const noexcept {
      return node_ == other.node_;
    };
    // bool operator!=(const const_iterator &other) const noexcept;

    // auto operator<=>(const MyList &) const = default;

  private:
    Node *node_;

    explicit const_iterator(Node *p) : node_(p) {};
    friend class list;
  };

  using size_type = size_t;
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  // iterator access interface
  iterator begin() noexcept { return iterator(head_->next); }
  iterator end() noexcept { return iterator(tail_); }
  const_iterator begin() const noexcept { return const_iterator(head_->next); }
  const_iterator end() const noexcept { return const_iterator(tail_); }

  const_iterator cbegin() const noexcept { return begin(); }
  const_iterator cend() const noexcept { return end(); }

  reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
  reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
  const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }

  const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator(rbegin());
  }
  const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(rend());
  }

  // constructor
  MyList() : head_(nullptr), tail_(nullptr), size_(0) {}
  explicit MyList(size_type count) : MyList() {
    for (size_type i = 0; i < count; ++i) {
      push_back(T{});
    }
  };
  MyList(size_type count, const T &value) : MyList() {
    for (size_type i = 0; i < count; ++i) {
      push_back(value);
    }
  };
  MyList(std::initializer_list<T> ilist) : MyList() {
    for (const auto &elem : ilist) {
      push_back(elem);
    }
  };
  // destructor
  ~MyList() { clear(); };
  // copy constructor
  MyList(const MyList &other) : MyList() {
    for (Node *cur = other.head_; cur != nullptr; cur = cur->next) {
      push_back(cur->value);
    }
  };
  // copy assginment
  MyList &operator=(const MyList &other) {
    if (this == &other)
      return *this;
    MyList tmp(other);
    swap(tmp);
    return *this;
  };
  // move constructor
  MyList(MyList &&other) noexcept
      : head_(std::exchange(other.head_, nullptr)),
        tail_(std::exchange(other.tail_, nullptr)),
        size_(std::exchange(other.size_, 0)) {};
  // move assignment
  MyList &operator=(MyList &&other) noexcept {
    if (this == &other)
      return *this;
    clear();
    head_ = std::exchange(other.head_, nullptr);
    tail_ = std::exchange(other.tail_, nullptr);
    size_ = std::exchange(other.size_, 0);
    return *this;
  };

  //  Member function
  reference front();
  const_reference front() const;

  reference back();
  const_reference back() const;

  bool empty() const noexcept;
  size_type size() const noexcept;

  void clear() noexcept {
    Node *cur = head_;
    while (cur) {
      Node *next = cur->next;
      delete cur;
      cur = next;
    }
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
  };

  void push_back(const T &value);
  void push_back(T &&value);

  void push_front(const T &value);
  void push_front(T &&value);

  void pop_back();
  void pop_front();

  void remove();
  void get();
  void print();

  iterator insert(const_iterator pos, const T &value);
  iterator insert(const_iterator pos, T &&value);

  iterator erase(const_iterator pos);
  iterator erase(const_iterator first, const_iterator last);

private:
  Node *head_;
  Node *tail_;
  size_type size_;
};