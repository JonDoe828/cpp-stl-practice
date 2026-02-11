#include <cstddef>
#include <iterator>
template <typename T> class MyDeque {
public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;
  using size_type = std::size_t;

  class iterator {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;

    iterator(pointer ptr) : ptr_(ptr) {}
    reference operator*() const { return *ptr_; }
    pointer operator->() { return ptr_; }
    iterator &operator++() {
      ++ptr_;
      return *this;
    }
    iterator operator++(int) {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    iterator &operator--() {
      --ptr_;
      return *this;
    }
    iterator operator--(int) {
      iterator tmp = *this;
      --(*this);
      return tmp;
    }
    iterator &operator+=(difference_type n) {
      ptr_ += n;
      return *this;
    }
    iterator &operator-=(difference_type n) {
      ptr_ -= n;
      return *this;
    }
    iterator operator+(difference_type n) const { return iterator(ptr_ + n); }
    iterator operator-(difference_type n) const { return iterator(ptr_ - n); }
    difference_type operator-(const iterator &rhs) const {
      return ptr_ - rhs.ptr_;
    }

    auto operator<=>(const iterator &) const = default;

  private:
    pointer ptr_;
  };
  class const_iterator {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = const T *;
    using reference = const T &;

    const_iterator(pointer ptr) : ptr_(ptr) {}
    reference operator*() const { return *ptr_; }
    pointer operator->() { return ptr_; }
    const_iterator &operator++() {
      ++ptr_;
      return *this;
    }
    const_iterator operator++(int) {
      const_iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    const_iterator &operator--() {
      --ptr_;
      return *this;
    }
    const_iterator operator--(int) {
      const_iterator tmp = *this;
      --(*this);
      return tmp;
    }
    const_iterator &operator+=(difference_type n) {
      ptr_ += n;
      return *this;
    }
    const_iterator &operator-=(difference_type n) {
      ptr_ -= n;
      return *this;
    }
    const_iterator operator+(difference_type n) const {
      return const_iterator(ptr_ + n);
    }
    const_iterator operator-(difference_type n) const {
      return const_iterator(ptr_ - n);
    }
    difference_type operator-(const const_iterator &rhs) const {
      return ptr_ - rhs.ptr_;
    }

  private:
    pointer ptr_;
  };

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  iterator begin() {}
  iterator end() {}
  const_iterator begin() const {}
  const_iterator end() const {}

  const_iterator cbegin() const { return begin(); }
  const_iterator cend() const { return end(); }

  reverse_iterator rbegin() { return reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }
  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  const_reverse_iterator crbegin() const {
    return const_reverse_iterator(cbegin());
  }
  const_reverse_iterator crend() const {
    return const_reverse_iterator(cbegin());
  }

private:
};