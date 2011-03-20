#include <utils/packer.h>

#include <arpa/inet.h>
#include <cassert>

namespace {
template<typename T>
void *WriteType(void *pos, void *end, const T &val) {
  T *conv = reinterpret_cast<T *>(pos);
  assert(conv + 1 <= end && "not enough room for data");
  *conv = val;
  return conv + 1;
}
  
template<typename T>
const void *ReadType(const void *pos, const void *end, T &val) {
  const T *conv = reinterpret_cast<const T *>(pos);
  assert(conv + 1 <= end && "not enough room for data");
  val = *conv;
  return conv + 1;
}  
}

Packer::Packer(void *start, void *end) 
  : _pos(start)
  , _end(end)
{
}

void Packer::writeShort(short value) {
  WriteType(_pos, _end, htons(value));
}

void Packer::writeInt(int value) {
  WriteType(_pos, _end, htonl(value));
}

void Packer::writeString(const std::string &value) {
  const size_t strsize = value.size();
  writeInt(strsize);
  char *data = reinterpret_cast<char *>(_pos);
  assert(data + strsize <= _end && "not enough room for string");
  strncpy(data, value.c_str(), strsize);
  _pos = data + strsize;
}


Unpacker::Unpacker(const void *data, size_t size) 
  : _pos(data)
  , _end(reinterpret_cast<const char *>(data) + size)
{
}

short Unpacker::readShort() {
  short ret;
  _pos = ReadType(_pos, _end, ret);
  return ret;
}
