#ifndef GAME_CLIENT_SNAPSHOT_H
#define GAME_CLIENT_SNAPSHOT_H

#include <vector>

template<typename T>
class Snapshot {
public:
  typedef std::vector<T> container_type;
  typedef typename container_type::const_iterator const_iterator;
  
  Snapshot(int tick = 0) : _tick(tick) {}
  
  void push(class Unpacker &msg) {
    _snaps.push_back(T().read(msg));
  }
  
  const_iterator begin() const {return _snaps.begin(); }
  const_iterator end() const {return _snaps.end(); }
  const_iterator find(int id) const {
    const_iterator it = begin(), it_e = end();
    for (; it != it_e && it->id != id; ++it);
    return it;
  }
  
private:
  container_type _snaps;
  int _tick;
};

#endif // !GAME_CLIENT_SNAPSHOT_H
