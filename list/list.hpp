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
  struct NodeBase {
    NodeBase *prev;
    NodeBase *next;

    NodeBase() noexcept : prev(this), next(this) {}
    NodeBase(NodeBase *prev, NodeBase *next) noexcept
        : prev(prev), next(next) {}
  };

  struct Node : NodeBase {
    T value;

    explicit Node(const T &value) : NodeBase(nullptr, nullptr), value(value) {}
    explicit Node(T &&value)
        : NodeBase(nullptr, nullptr), value(std::move(value)) {}
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

    reference operator*() const noexcept {
      return static_cast<Node *>(node_)->value;
    }
    pointer operator->() const noexcept {
      return std::addressof(static_cast<Node *>(node_)->value);
    }

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
      node_ = node_->prev;
      return *this;
    }
    iterator operator--(int) noexcept {
      iterator tmp(*this);
      --(*this);
      return tmp;
    }

    bool operator==(const iterator &other) const noexcept {
      return node_ == other.node_;
    }
    bool operator!=(const iterator &other) const noexcept {
      return !(*this == other);
    }

  private:
    NodeBase *node_{nullptr};

    explicit iterator(NodeBase *node) noexcept : node_(node) {}

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
    const_iterator(const iterator &it) noexcept : node_(it.node_) {}

    reference operator*() const noexcept {
      return static_cast<const Node *>(node_)->value;
    }
    pointer operator->() const noexcept {
      return std::addressof(static_cast<const Node *>(node_)->value);
    }

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
      node_ = node_->prev;
      return *this;
    }
    const_iterator operator--(int) noexcept {
      const_iterator tmp(*this);
      --(*this);
      return tmp;
    }

    bool operator==(const const_iterator &other) const noexcept {
      return node_ == other.node_;
    }
    bool operator!=(const const_iterator &other) const noexcept {
      return !(*this == other);
    }
    bool operator==(const iterator &other) const noexcept {
      return node_ == other.node_;
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
    const NodeBase *node_{nullptr};

    explicit const_iterator(const NodeBase *node) noexcept : node_(node) {}

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

  list(list &&other) noexcept : list() { move_from(other); }

  list &operator=(list &&other) noexcept {
    if (this == &other) {
      return *this;
    }

    clear();
    move_from(other);
    return *this;
  }

  list &operator=(std::initializer_list<T> values) {
    list tmp(values);
    swap(tmp);
    return *this;
  }

  iterator begin() noexcept { return iterator(sentinel_.next); }
  iterator end() noexcept { return iterator(&sentinel_); }

  const_iterator begin() const noexcept {
    return const_iterator(sentinel_.next);
  }
  const_iterator end() const noexcept { return const_iterator(&sentinel_); }

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
    return static_cast<Node *>(sentinel_.next)->value;
  }
  const_reference front() const {
    if (empty()) {
      throw std::out_of_range("list::front on empty list");
    }
    return static_cast<const Node *>(sentinel_.next)->value;
  }

  reference back() {
    if (empty()) {
      throw std::out_of_range("list::back on empty list");
    }
    return static_cast<Node *>(sentinel_.prev)->value;
  }
  const_reference back() const {
    if (empty()) {
      throw std::out_of_range("list::back on empty list");
    }
    return static_cast<const Node *>(sentinel_.prev)->value;
  }

  bool empty() const noexcept { return size_ == 0; }
  size_type size() const noexcept { return size_; }

  void clear() noexcept {
    NodeBase *cur = sentinel_.next;
    while (cur != &sentinel_) {
      NodeBase *next = cur->next;
      delete static_cast<Node *>(cur);
      cur = next;
    }

    reset_sentinel();
    size_ = 0;
  }

  void push_back(const T &value) {
    insert_node_before(&sentinel_, new Node(value));
  }
  void push_back(T &&value) {
    insert_node_before(&sentinel_, new Node(std::move(value)));
  }

  void push_front(const T &value) {
    insert_node_before(sentinel_.next, new Node(value));
  }
  void push_front(T &&value) {
    insert_node_before(sentinel_.next, new Node(std::move(value)));
  }

  void pop_back() {
    if (empty()) {
      throw std::out_of_range("list::pop_back on empty list");
    }

    erase_node(sentinel_.prev);
  }

  void pop_front() {
    if (empty()) {
      throw std::out_of_range("list::pop_front on empty list");
    }

    erase_node(sentinel_.next);
  }

  void remove(const T &value) {
    NodeBase *cur = sentinel_.next;
    while (cur != &sentinel_) {
      NodeBase *next = cur->next;

      if (static_cast<Node *>(cur)->value == value) {
        erase_node(cur);
      }

      cur = next;
    }
  }

  reference get(size_type index) {
    if (index >= size_) {
      throw std::out_of_range("list::get index out of range");
    }
    return static_cast<Node *>(node_at(index))->value;
  }
  const_reference get(size_type index) const {
    if (index >= size_) {
      throw std::out_of_range("list::get index out of range");
    }
    return static_cast<const Node *>(node_at(index))->value;
  }

  void print() {
    for (NodeBase *cur = sentinel_.next; cur != &sentinel_; cur = cur->next) {
      std::cout << static_cast<Node *>(cur)->value;
      if (cur->next != &sentinel_) {
        std::cout << ' ';
      }
    }
    std::cout << '\n';
  }

  iterator insert(const_iterator pos, const T &value) {
    Node *node = new Node(value);
    insert_node_before(mutable_node(pos), node);
    return iterator(node);
  }
  iterator insert(const_iterator pos, T &&value) {
    Node *node = new Node(std::move(value));
    insert_node_before(mutable_node(pos), node);
    return iterator(node);
  }

  iterator erase(const_iterator pos) {
    NodeBase *node = mutable_node(pos);
    if (node == &sentinel_) {
      throw std::out_of_range("list::erase cannot erase end");
    }

    NodeBase *next = node->next;
    erase_node(node);
    return iterator(next);
  }

  iterator erase(const_iterator first, const_iterator last) {
    NodeBase *finish = mutable_node(last);
    NodeBase *cur = mutable_node(first);

    while (cur != finish) {
      if (cur == &sentinel_) {
        throw std::out_of_range("list::erase range out of range");
      }

      NodeBase *next = cur->next;
      erase_node(cur);
      cur = next;
    }

    return iterator(finish);
  }

  void swap(list &other) noexcept {
    if (this == &other) {
      return;
    }

    std::swap(sentinel_.next, other.sentinel_.next);
    std::swap(sentinel_.prev, other.sentinel_.prev);
    std::swap(size_, other.size_);

    fix_sentinel_links();
    other.fix_sentinel_links();
  }

private:
  NodeBase sentinel_;
  size_type size_{0};

  void reset_sentinel() noexcept {
    sentinel_.next = &sentinel_;
    sentinel_.prev = &sentinel_;
  }

  void fix_sentinel_links() noexcept {
    if (empty()) {
      reset_sentinel();
      return;
    }

    sentinel_.next->prev = &sentinel_;
    sentinel_.prev->next = &sentinel_;
  }

  void move_from(list &other) noexcept {
    if (other.empty()) {
      reset_sentinel();
      size_ = 0;
      return;
    }

    sentinel_.next = other.sentinel_.next;
    sentinel_.prev = other.sentinel_.prev;
    sentinel_.next->prev = &sentinel_;
    sentinel_.prev->next = &sentinel_;
    size_ = std::exchange(other.size_, 0);
    other.reset_sentinel();
  }

  NodeBase *mutable_node(const_iterator pos) noexcept {
    return const_cast<NodeBase *>(pos.node_);
  }

  void insert_node_before(NodeBase *pos, NodeBase *node) noexcept {
    node->prev = pos->prev;
    node->next = pos;
    pos->prev->next = node;
    pos->prev = node;
    ++size_;
  }

  void erase_node(NodeBase *node) noexcept {
    node->prev->next = node->next;
    node->next->prev = node->prev;
    delete static_cast<Node *>(node);
    --size_;
  }

  NodeBase *node_at(size_type index) noexcept {
    if (index < size_ / 2) {
      NodeBase *cur = sentinel_.next;
      for (size_type i = 0; i < index; ++i) {
        cur = cur->next;
      }
      return cur;
    }

    NodeBase *cur = sentinel_.prev;
    for (size_type i = size_ - 1; i > index; --i) {
      cur = cur->prev;
    }
    return cur;
  }

  const NodeBase *node_at(size_type index) const noexcept {
    if (index < size_ / 2) {
      const NodeBase *cur = sentinel_.next;
      for (size_type i = 0; i < index; ++i) {
        cur = cur->next;
      }
      return cur;
    }

    const NodeBase *cur = sentinel_.prev;
    for (size_type i = size_ - 1; i > index; --i) {
      cur = cur->prev;
    }
    return cur;
  }
};

} // namespace my_stl
