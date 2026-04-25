#pragma once

#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <utility>

namespace my_stl {

template <typename T> class list {
private:
  struct Node {
    Node *prev;
    Node *next;
    T value;

    Node(Node *prev, Node *next, const T &value)
        : prev(prev), next(next), value(value) {}
    Node(Node *prev, Node *next, T &&value)
        : prev(prev), next(next), value(std::move(value)) {}
  };

public:
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;

  class const_iterator;

  class iterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;

    iterator() noexcept = default;

    reference operator*() const noexcept { return node_->value; }
    pointer operator->() const noexcept { return std::addressof(node_->value); }

    iterator &operator++() noexcept {
      node_ = node_->next;
      return *this;
    }
    iterator operator++(int) noexcept {
      iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    iterator &operator--() noexcept {
      node_ = node_ == nullptr ? owner_->tail_ : node_->prev;
      return *this;
    }
    iterator operator--(int) noexcept {
      iterator tmp(*this);
      --(*this);
      return tmp;
    }

    bool operator==(const iterator &other) const noexcept {
      return owner_ == other.owner_ && node_ == other.node_;
    }
    bool operator!=(const iterator &other) const noexcept {
      return !(*this == other);
    }

  private:
    list *owner_{nullptr};
    Node *node_{nullptr};

    iterator(list *owner, Node *node) noexcept : owner_(owner), node_(node) {}

    friend class list;
    friend class const_iterator;
  };

  class const_iterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T *;
    using reference = const T &;

    const_iterator() noexcept = default;
    const_iterator(const iterator &it) noexcept
        : owner_(it.owner_), node_(it.node_) {}

    reference operator*() const noexcept { return node_->value; }
    pointer operator->() const noexcept { return std::addressof(node_->value); }

    const_iterator &operator++() noexcept {
      node_ = node_->next;
      return *this;
    }
    const_iterator operator++(int) noexcept {
      const_iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    const_iterator &operator--() noexcept {
      node_ = node_ == nullptr ? owner_->tail_ : node_->prev;
      return *this;
    }
    const_iterator operator--(int) noexcept {
      const_iterator tmp(*this);
      --(*this);
      return tmp;
    }

    bool operator==(const const_iterator &other) const noexcept {
      return owner_ == other.owner_ && node_ == other.node_;
    }
    bool operator!=(const const_iterator &other) const noexcept {
      return !(*this == other);
    }
    bool operator==(const iterator &other) const noexcept {
      return owner_ == other.owner_ && node_ == other.node_;
    }
    bool operator!=(const iterator &other) const noexcept {
      return !(*this == other);
    }

    friend bool operator==(const iterator &lhs,
                           const const_iterator &rhs) noexcept {
      return rhs == lhs;
    }
    friend bool operator!=(const iterator &lhs,
                           const const_iterator &rhs) noexcept {
      return !(lhs == rhs);
    }

  private:
    const list *owner_{nullptr};
    Node *node_{nullptr};

    const_iterator(const list *owner, Node *node) noexcept
        : owner_(owner), node_(node) {}

    friend class list;
  };

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  list() = default;
  explicit list(size_type count) : list() {
    for (size_type i = 0; i < count; ++i) {
      push_back(T{});
    }
  }
  list(size_type count, const T &value) : list() {
    for (size_type i = 0; i < count; ++i) {
      push_back(value);
    }
  }
  list(std::initializer_list<T> values) : list() {
    for (const auto &value : values) {
      push_back(value);
    }
  }

  ~list() { clear(); }

  list(const list &other) : list() {
    try {
      for (const auto &value : other) {
        push_back(value);
      }
    } catch (...) {
      clear();
      throw;
    }
  }

  list &operator=(const list &other) {
    if (this == &other) {
      return *this;
    }

    list tmp(other);
    swap(tmp);
    return *this;
  }

  list(list &&other) noexcept
      : head_(std::exchange(other.head_, nullptr)),
        tail_(std::exchange(other.tail_, nullptr)),
        size_(std::exchange(other.size_, 0)) {}

  list &operator=(list &&other) noexcept {
    if (this == &other) {
      return *this;
    }

    clear();
    head_ = std::exchange(other.head_, nullptr);
    tail_ = std::exchange(other.tail_, nullptr);
    size_ = std::exchange(other.size_, 0);
    return *this;
  }

  list &operator=(std::initializer_list<T> values) {
    list tmp(values);
    swap(tmp);
    return *this;
  }

  iterator begin() noexcept { return iterator(this, head_); }
  iterator end() noexcept { return iterator(this, nullptr); }

  const_iterator begin() const noexcept { return const_iterator(this, head_); }
  const_iterator end() const noexcept { return const_iterator(this, nullptr); }

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

  reference front() {
    if (empty()) {
      throw std::out_of_range("list::front on empty list");
    }
    return head_->value;
  }
  const_reference front() const {
    if (empty()) {
      throw std::out_of_range("list::front on empty list");
    }
    return head_->value;
  }

  reference back() {
    if (empty()) {
      throw std::out_of_range("list::back on empty list");
    }
    return tail_->value;
  }
  const_reference back() const {
    if (empty()) {
      throw std::out_of_range("list::back on empty list");
    }
    return tail_->value;
  }

  bool empty() const noexcept { return size_ == 0; }
  size_type size() const noexcept { return size_; }

  void clear() noexcept {
    Node *cur = head_;
    while (cur != nullptr) {
      Node *next = cur->next;
      delete cur;
      cur = next;
    }

    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
  }

  void push_back(const T &value) {
    insert_node_before(nullptr, new Node(nullptr, nullptr, value));
  }
  void push_back(T &&value) {
    insert_node_before(nullptr, new Node(nullptr, nullptr, std::move(value)));
  }

  void push_front(const T &value) {
    insert_node_before(head_, new Node(nullptr, nullptr, value));
  }
  void push_front(T &&value) {
    insert_node_before(head_, new Node(nullptr, nullptr, std::move(value)));
  }

  void pop_back() {
    if (empty()) {
      throw std::out_of_range("list::pop_back on empty list");
    }

    Node *old = tail_;
    unlink_node(old);
    delete old;
    --size_;
  }

  void pop_front() {
    if (empty()) {
      throw std::out_of_range("list::pop_front on empty list");
    }

    Node *old = head_;
    unlink_node(old);
    delete old;
    --size_;
  }

  void remove(const T &value) {
    Node *cur = head_;
    while (cur != nullptr) {
      Node *next = cur->next;

      if (cur->value == value) {
        unlink_node(cur);
        delete cur;
        --size_;
      }

      cur = next;
    }
  }

  reference get(size_type index) {
    if (index >= size_) {
      throw std::out_of_range("list::get index out of range");
    }
    return node_at(index)->value;
  }
  const_reference get(size_type index) const {
    if (index >= size_) {
      throw std::out_of_range("list::get index out of range");
    }
    return node_at(index)->value;
  }

  void print() {
    for (Node *cur = head_; cur != nullptr; cur = cur->next) {
      std::cout << cur->value;
      if (cur->next != nullptr) {
        std::cout << ' ';
      }
    }
    std::cout << '\n';
  }

  iterator insert(const_iterator pos, const T &value) {
    validate_position(pos, "list::insert position out of range");
    Node *node = new Node(nullptr, nullptr, value);
    insert_node_before(pos.node_, node);
    return iterator(this, node);
  }
  iterator insert(const_iterator pos, T &&value) {
    validate_position(pos, "list::insert position out of range");
    Node *node = new Node(nullptr, nullptr, std::move(value));
    insert_node_before(pos.node_, node);
    return iterator(this, node);
  }

  iterator erase(const_iterator pos) {
    validate_position(pos, "list::erase position out of range");
    if (pos.node_ == nullptr) {
      throw std::out_of_range("list::erase cannot erase end");
    }

    Node *next = pos.node_->next;
    unlink_node(pos.node_);
    delete pos.node_;
    --size_;
    return iterator(this, next);
  }

  iterator erase(const_iterator first, const_iterator last) {
    validate_position(first, "list::erase range out of range");
    validate_position(last, "list::erase range out of range");

    size_type count = 0;
    for (Node *cur = first.node_; cur != last.node_; cur = cur->next) {
      if (cur == nullptr) {
        throw std::out_of_range("list::erase range out of range");
      }
      ++count;
    }

    Node *cur = first.node_;
    while (cur != last.node_) {
      Node *next = cur->next;
      unlink_node(cur);
      delete cur;
      cur = next;
    }

    size_ -= count;
    return iterator(this, last.node_);
  }

  void swap(list &other) noexcept {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
  }

private:
  Node *head_{nullptr};
  Node *tail_{nullptr};
  size_type size_{0};

  void insert_node_before(Node *pos, Node *node) noexcept {
    node->next = pos;

    if (pos == nullptr) {
      node->prev = tail_;
      tail_ = node;
    } else {
      node->prev = pos->prev;
      pos->prev = node;
    }

    if (node->prev == nullptr) {
      head_ = node;
    } else {
      node->prev->next = node;
    }

    ++size_;
  }

  void unlink_node(Node *node) noexcept {
    if (node->prev == nullptr) {
      head_ = node->next;
    } else {
      node->prev->next = node->next;
    }

    if (node->next == nullptr) {
      tail_ = node->prev;
    } else {
      node->next->prev = node->prev;
    }
  }

  Node *node_at(size_type index) noexcept {
    if (index < size_ / 2) {
      Node *cur = head_;
      for (size_type i = 0; i < index; ++i) {
        cur = cur->next;
      }
      return cur;
    }

    Node *cur = tail_;
    for (size_type i = size_ - 1; i > index; --i) {
      cur = cur->prev;
    }
    return cur;
  }

  const Node *node_at(size_type index) const noexcept {
    if (index < size_ / 2) {
      const Node *cur = head_;
      for (size_type i = 0; i < index; ++i) {
        cur = cur->next;
      }
      return cur;
    }

    const Node *cur = tail_;
    for (size_type i = size_ - 1; i > index; --i) {
      cur = cur->prev;
    }
    return cur;
  }

  void validate_position(const const_iterator &pos, const char *message) const {
    if (pos.owner_ != this) {
      throw std::out_of_range(message);
    }
  }
};

} // namespace my_stl
