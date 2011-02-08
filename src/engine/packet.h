#ifndef ENGINE_PACKET_H
#define ENGINE_PACKET_H

extern "C" {
#include <stdint.h>
}

#include <vector>
#include <cassert>

typedef std::vector<uint8_t> PacketData;

class PacketWriter {
public:
  // Fixme: should use a pos instead, and resize when needed. not push_back.
  
  PacketWriter(PacketData &out)
    : _out(out)
  {
  }

  void writeU8(uint8_t val) {
    _out.push_back(val);
  }

  void writeU32(uint32_t val) {
    size_t pos = _out.size();
    _out.resize(_out.size() + sizeof(uint32_t));
    *reinterpret_cast<uint32_t *>(&_out[pos]) = val;
  }
  
private:
  PacketData &_out;
};

class PacketReader {
public:
  PacketReader(const void *data, size_t size)
    : _data(data)
    , _size(size)
  {
    _rpos = 0;
  }

  uint8_t readU8() {
    assert(_rpos < _size && "trying to read outside data");
    return static_cast<const char *>(_data)[_rpos++];
  }

  uint32_t readU32() {
    assert(_rpos < _size && "trying to read outside data");
    uint32_t ret = *((uint32_t *)((const char *)_data + _rpos));
    _rpos += sizeof(uint32_t);
    return ret;
  }
  
private:
  const void *_data;
  size_t _size;
  size_t _rpos;
};

#endif // !ENGINE_PACKET_H
