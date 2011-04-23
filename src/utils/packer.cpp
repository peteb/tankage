#include <utils/packer.h>
#include <utils/log.h>

#ifdef _WIN32
#include <WinSock2.h>
#else 
#include <arpa/inet.h>
#endif

#undef max // some better way to get rid of this one?

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
  writeData(value.c_str(), value.size());
}

void Packer::writeData(const char *data, size_t size) {
  if (size > 0xFFFF) {
    _badbit = true;
    return;
  }
  
  writeShort((unsigned short)size);
  _data.insert(_data.end(), data, data + size);
}

void Packer::append(const Packer &packer) {
  _data.insert(_data.end(), packer._data.begin(), packer._data.end());  
}



Unpacker::Unpacker(const std::vector<unsigned char> &data) 
  : _data(data)
{
  _pos = 0;
  _badbit = false;
}

bool Unpacker::verifySize(size_t size) {
  if (_badbit) {
    Log(DEBUG) << "unpacker already in bad state";
    return false;
  }
  
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

std::pair<const char *, size_t> Unpacker::readData() {
  unsigned short size = readShort();
  std::pair<const char *, size_t> ret(0, 0);
  if (_badbit || !verifySize(size))
    return ret;
  
  ret = std::make_pair(reinterpret_cast<const char *>(&_data[_pos]), size);
  _pos += size;
  
  return ret;
}

std::string Unpacker::readString() {
  std::pair<const char *, size_t> data = readData();
  std::string ret;
  if (_badbit)
    return ret;
  
  ret.assign((const char *)data.first, data.second);

  return ret;
}


