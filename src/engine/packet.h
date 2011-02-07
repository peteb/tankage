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
  PacketWriter(PacketData &out)
    : _out(out)
  {
  }

  void writeU8(uint8_t val) {
    _out.push_back(val);
  }
  
private:
  PacketData &_out;
};

class PacketReader {
public:
  PacketReader(const PacketData &in)
    : _in(in)
  {
    _pos = 0;
  }

  uint8_t readU8() {
    assert(_pos < _in.size() && "trying to read outside data");
    return _in[_pos++];
  }
  
private:
  const PacketData &_in;
  size_t _pos;
};

#endif // !ENGINE_PACKET_H
