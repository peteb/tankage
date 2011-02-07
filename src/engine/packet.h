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
  PacketReader(const PacketData &in)
    : _in(in)
  {
    _rpos = 0;
  }

  uint8_t readU8() {
    assert(_rpos < _in.size() && "trying to read outside data");
    return _in[_rpos++];
  }
  
private:
  const PacketData &_in;
  size_t _rpos;
};

#endif // !ENGINE_PACKET_H
