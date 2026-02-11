#include <cstddef>
#include <iterator>
#include <utility>
template <typename T> class MyList {
private:
  struct Node {
    Node *prev;
    Node *next;
    T value;

    Node(Node *prev, Node *next, const T &v)
        : prev(nullptr), next(nullptr), value(v) {}
    Node(Node *prev, Node *next, T &&v)
        : prev(nullptr), next(nullptr), value(std::move(v)) {}
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
  reference front() {
    if (empty()) {
      throw std::out_of_range("MyList::front on empty list");
    }
    return head_->value;
  };
  const_reference front() const {
    if (empty()) {
      throw std::out_of_range("MyList::front on empty list");
    }
    return head_->value;
  };

  reference back() {
    if (empty()) {
      throw std::out_of_range("MyList::front on empty list");
    }
    return tail_->value;
  };
  const_reference back() const {
    if (empty()) {
      throw std::out_of_range("MyList::front on empty list");
    }
    return tail_->value;
  };

  bool empty() const noexcept { return size_ == 0; };
  size_type size() const noexcept { return size_; };

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

  void push_back(const T &value) {
    Node *node = new Node(tail_, nullptr, value);

    if (empty()) {
      head_ = tail_ = node;
    } else {
      tail_->next = node;
      node->prev = tail_;
      tail_ = node;
    }

    ++size_;
  };
  void push_back(T &&value) {
    Node *node = new Node(tail_, nullptr, std::move(value));

    if (empty()) {
      head_ = tail_ = node;
    } else {
      tail_->next = node;
      node->prev = tail_;
      tail_ = node;
    }

    ++size_;
  };

  void push_front(const T &value) {
    Node *node = new Node(nullptr, head_, value);
    if (empty()) {
      head_ = tail_ = node;
    } else {
      head_->prev = node;
      node->next = head_;
      head_ = node;
    }

    ++size_;
  };
  void push_front(T &&value) {
    Node *node = new Node(nullptr, head_, std::move(value));
    if (empty()) {
      head_ = tail_ = node;
    } else {
      head_->prev = node;
      node->next = head_;
      head_ = node;
    }

    ++size_;
  };

  void pop_back() {
    if (empty()) {
      throw std::out_of_range("MyList::pop_back");
    }
    Node *old = tail_;

    if (size_ == 1) {
      head_ = tail_ = nullptr;
    } else {
      tail_ = tail_->prev;
      tail_->next = nullptr;
    }

    delete old;
    --size_;
  };
  void pop_front() {
    if (empty()) {
      throw std::out_of_range("MyList::pop_front");
    }
    Node *old = head_;

    if (size_ == 1) {
      head_ = tail_ = nullptr;
    } else {
      head_ = head_->next;
      head_->prev = nullptr;
    }

    delete old;
    --size_;
  };

  void remove(const T &value) {
    Node *cur = head_;
    while (cur) {
      Node *next = cur->next;
      if (cur->value == value) {
        // 1
        if (cur == head_) {
          head_ = cur->next;
          if (head_) {
            head_->prev = nullptr;
          } else {
            // 链表变空
            tail_ = nullptr;
          }
        }
        // 2
        else if (cur == tail_) {
          tail_ = cur->prev;
          tail_->next = nullptr;
        }
        // 3
        else {
          cur->prev->next = cur->next;
          cur->next->prev = cur->prev;
        }

        delete cur;
        --size_;
      }
      cur = next;
    }
  };

  T &get(size_type index) {
    if (index >= size_) {
      throw std::out_of_range("MyList::get index out of range");
    }
    Node *cur = head_;
    for (size_type i = 0; i < index; ++i) {
      cur = cur->next;
    }
    return cur->value;
  };
  const T &get(size_type index) const {
    if (index >= size_) {
      throw std::out_of_range("MyList::get index out of range");
    }

    Node *cur = head_;
    for (size_type i = 0; i < index; ++i) {
      cur = cur->next;
    }
    return cur->value;
  }

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