#ifndef UTILS_RING_BUFFER_H
#define UTILS_RING_BUFFER_H

#include <vector>

#include <iostream>

template<typename T>
class ring_buffer {
public:
  typedef std::vector<T> container_type;
  typedef typename container_type::value_type value_type;
  typedef typename container_type::size_type size_type;
  typedef typename container_type::reference reference;
  typedef typename container_type::const_reference const_reference;

  class iterator_base : public std::iterator<
    std::forward_iterator_tag, T, ptrdiff_t, const T *, const T&>
  {
  public:
    iterator_base(size_type pos, container_type *source)
      : pos(pos), source(source) {}
    
    const reference operator *() const {return (*source)[pos]; }
    const reference operator ->() const {return (*source)[pos]; }
    bool operator != (const iterator_base &other) const {return pos != other.pos; }
    bool operator == (const iterator_base &other) const {return !(*this != other); }

    friend class ring_buffer;
    
  protected:
    size_type pos;
    container_type *source;
  };

  class iterator : public iterator_base {
  public:
    iterator(size_type pos, container_type *source)
      : iterator_base(pos, source) {}
    
    iterator &operator++() {
      iterator_base::pos = (iterator_base::pos + 1) % iterator_base::source->size();
      return *this;
    }
  };
  
  class reverse_iterator : public iterator_base {
  public:
    reverse_iterator(size_type pos, container_type *source)
      : iterator_base(pos, source) {}
    
    reverse_iterator &operator++() {
      if (iterator_base::pos == 0) {
        iterator_base::pos = iterator_base::source->size() - 1;
      }
      else {
        --iterator_base::pos;
      }
      return *this;
    }

    iterator base() const {
      iterator iter(iterator_base::pos, iterator_base::source);
      ++iter;
      return iter;
    }
  };
  
  ring_buffer(size_type n, const T &val = T())
    : c(n + 1, val), tail(0, &c), head(0, &c) {}

  void push_back(const T &value) {
    *head = value;
    ++head;
    if (head == tail) {
      ++tail;
    }
  }

  void pop_front(const iterator &to) {
    tail.pos = to.pos;
  }

  iterator &begin() {return tail; }
  iterator &end() {return head; }

  reverse_iterator rbegin() {
    reverse_iterator iter(head.pos, &c);
    ++iter;    
    return iter;
  }

  reverse_iterator rend() {
    reverse_iterator iter(tail.pos, &c);
    ++iter;
    return iter;
  }
  
private:
  container_type c;
  iterator tail, head;
};

#endif // !UTILS_RING_BUFFER_H
