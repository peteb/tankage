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

//namespace {
//template<typename T>
//void *WriteType(void *pos, void *end, const T &val) {
//  T *conv = reinterpret_cast<T *>(pos);
//  assert(conv + 1 <= end && "not enough room for data");
//  *conv = val;
//  return conv + 1;
//}
//  
//template<typename T>
//const void *ReadType(const void *pos, const void *end, T &val) {
//  const T *conv = reinterpret_cast<const T *>(pos);
//  assert(conv + 1 <= end && "not enough room for data");
//  val = *conv;
//  return conv + 1;
//}  
//}

Packer::Packer(std::vector<unsigned char> &data) 
  : _data(data)
{
  _badbit = false;
}

void Packer::writeByte(char value) {
  _data.push_back(value);
}

void Packer::writeShort(short value) {
  short net_value = htons(value);
  _data.push_back((net_value >> 8) & 0xFF);  
  _data.push_back(net_value & 0xFF);
}

void Packer::writeInt(int value) {
  int net_value = htonl(value);
  _data.push_back(net_value & 0xFF);
  _data.push_back((net_value >> 8) & 0xFF);  
  _data.push_back((net_value >> 16) & 0xFF);  
  _data.push_back((net_value >> 24) & 0xFF);  
}

void Packer::writeString(const std::string &value) {
  if (value.size() > 0xFFFF) {
    _badbit = true;
    return;
  }
  
  writeShort((short)value.size());
  _data.reserve(_data.size() + value.size());
  _data.insert(_data.end(), value.begin(), value.end());
}

void Packer::writeData(const char *data, size_t size) {
  if (size > 0xFFFF) {
    _badbit = true;
    return;
  }
  
  writeShort((short)size);
  _data.insert(_data.end(), data, data + size);
}

void Packer::append(const Packer &packer) {
  _data.insert(_data.end(), packer._data.begin(), packer._data.end());  
}


//
//void Packer::writeData(const void *data, size_t size) {
//  assert(static_cast<char *>(_pos) + size < _end && "not enough room for data");
//  memcpy(_pos, data, size);
//  _pos = static_cast<char *>(_pos) + size;  
//}

//size_t Packer::size() const {
//  return static_cast<const char *>(_pos) - static_cast<const char *>(_start);
//}
//
//void Packer::advance(size_t amount) {
//  _pos = static_cast<char *>(_pos) + amount;
//}
//
//void Packer::reset() {
//  _pos = _start;
//}

Unpacker::Unpacker(const std::vector<unsigned char> &data) 
  : _data(data)
{
  _pos = 0;
  _badbit = false;
}

bool Unpacker::verifySize(size_t size) {
  if (_pos + size > _data.size()) {
    _badbit = true;
    return false;
  }
  
  return true;
}

bool Unpacker::bad() const {
  return _badbit;
}

char Unpacker::readByte() {
  if (!verifySize(1))
    return 0;
  
  return _data[_pos++];
}

short Unpacker::readShort() {
  if (!verifySize(2))
    return 0;
  
  short net_value = 0;
  net_value |= _data[_pos++] << 8;
  net_value |= _data[_pos++];

  return ntohs(net_value);
}

int Unpacker::readInt() {
  if (!verifySize(2))
    return 4;

  int net_value = _data[_pos++];
  net_value |= _data[_pos++] << 8;
  net_value |= _data[_pos++] << 16;
  net_value |= _data[_pos++] << 24;

  return ntohl(net_value);
}

std::pair<const unsigned char *, size_t> Unpacker::readData() {
  unsigned short size = readShort();
  std::pair<const unsigned char *, size_t> ret(0, 0);
  if (_badbit || !verifySize(size))
    return ret;
  
  ret = std::make_pair(&_data[_pos], size);
  _pos += size;
  
  return ret;
}

std::string Unpacker::readString() {
  unsigned short size = readShort();
  std::string ret;
  if (_badbit || !verifySize(size))
    return ret;
  
  ret.assign(&_data[_pos], &_data[_pos + size]);
  _pos += size;

  return ret;
}

//const void *Unpacker::readData(size_t size) {
//  const char *data = reinterpret_cast<const char *>(_pos);
//  assert(data + size <= _end && "not enough room for data");
//  _pos = data + size;
//  return data;
//}
//
