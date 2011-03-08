#ifndef UTILS_RING_BUFFER_H
#define UTILS_RING_BUFFER_H

#include <vector>
#include <cassert>

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
  protected:
    iterator_base(size_type pos, container_type *source)
      : pos(pos), source(source) {}
    
  public:
    const reference operator *() const {return (*source)[pos]; }
    const value_type *operator ->() const {return &((*source)[pos]); }
    bool operator != (const iterator_base &other) const {return pos != other.pos; }
    bool operator == (const iterator_base &other) const {return !(*this != other); }

    friend class ring_buffer;
    
  protected:
    size_type pos;
    container_type *source; // TODO: possible opt: just point to the data, the
                            // iterators can be invalidated
  };

  class iterator : public iterator_base {
    using iterator_base::pos;
    using iterator_base::source;

  public:
    iterator(size_type pos, container_type *source)
      : iterator_base(pos, source) {}
    
    iterator &operator++() {
      pos = (pos + 1) % source->size();
      return *this;
    }
  };
  
  class reverse_iterator : public iterator_base {
    using iterator_base::pos;
    using iterator_base::source;
    
  public:
    reverse_iterator(size_type pos, container_type *source)
      : iterator_base(pos, source) {}
    
    reverse_iterator &operator++() {
      if (pos == 0) {
        pos = source->size() - 1;
      }
      else {
        --pos;
      }
      return *this;
    }

    iterator base() const {
      iterator iter(pos, source);
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

  void erase(const iterator &beg) {erase(beg, ++iterator(beg)); }
  void erase(const iterator &beg, const iterator &e) {
    assert(beg.pos == tail.pos && "implementation only supports erasing from begin");
    tail.pos = e.pos;
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
