#include <utils/packer.h>

#include <cassert>
#include <limits>
#include <cstring>

/* Maybe fix this into the CMake? */
#ifdef _WIN32
#include <WinSock2.h>
#else 
#include <arpa/inet.h>
#endif

#undef max // some better way to get rid of this one?

// FIXME: the asserts here should probably set a badstate or throw exception

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
  , _start(start)
  , _end(end)
{
}

Packer::Packer(const Packer &other) 
  : _pos(other._pos)
  , _start(other._pos)
  , _end(other._end)
{  
}

void Packer::writeByte(char value) {
  _pos = WriteType(_pos, _end, value);
}

void Packer::writeShort(short value) {
  _pos = WriteType(_pos, _end, htons(value));
}

void Packer::writeInt(int value) {
  _pos = WriteType(_pos, _end, htonl(value));
}

void Packer::writeString(const std::string &value) {
  assert(value.size() < std::numeric_limits<unsigned short>::max() && "too big string");
  const unsigned short strsize = value.size();
  writeShort(strsize);
  char *data = reinterpret_cast<char *>(_pos);
  assert(data + strsize <= _end && "not enough room for string");
  strncpy(data, value.c_str(), strsize);
  _pos = data + strsize;
}

void Packer::writeData(const Packer &packer) {
  assert(static_cast<char *>(_pos) + packer.size() < _end && "not enough room for data");
  memcpy(_pos, packer._start, packer.size());
  _pos = static_cast<char *>(_pos) + packer.size();
}

size_t Packer::size() const {
  return static_cast<const char *>(_pos) - static_cast<const char *>(_start);
}

void Packer::advance(size_t amount) {
  _pos = static_cast<char *>(_pos) + amount;
}

void Packer::reset() {
  _pos = _start;
}

Unpacker::Unpacker(const void *data, const void *end) 
  : _pos(data)
  , _end(end)
{
}

char Unpacker::readByte() {
  char ret;
  _pos = ReadType(_pos, _end, ret);
  return ret;
}

short Unpacker::readShort() {
  short ret;
  _pos = ReadType(_pos, _end, ret);
  return ntohs(ret);
}

int Unpacker::readInt() {
  int ret;
  _pos = ReadType(_pos, _end, ret);
  return ntohl(ret);
}

std::string Unpacker::readString() {
  const unsigned short strsize = readShort();
  const char *data = reinterpret_cast<const char *>(_pos);
  assert(data + strsize <= _end && "not enough room for string");
  _pos = data + strsize;
  return std::string(data, strsize);
}

