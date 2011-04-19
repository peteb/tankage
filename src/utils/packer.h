#ifndef UTILS_PACKER_H
#define UTILS_PACKER_H

#include <vector>
#include <string>
#include <utility>

class Packer {
  std::vector<unsigned char> &_data;
  bool _badbit;
  
public:
  Packer(std::vector<unsigned char> &data);
  
//  size_t size() const;
//  void advance(size_t amount);
//  void reset();
//  
  void writeByte(char value);
  void writeShort(short value);
  void writeInt(int value);
  void writeString(const std::string &value);  
  void writeData(const char *data, size_t size);
  void append(const Packer &packer);
//  void writeData(const void *data, size_t size);
};

class Unpacker {
  const std::vector<unsigned char> &_data;
  size_t _pos;
  bool _badbit;
  
  bool verifySize(size_t size);
  
public:
  Unpacker(const std::vector<unsigned char> &data);
  
  bool bad() const;
  char readByte();
  short readShort();
  int readInt();
  std::string readString();
  std::pair<const unsigned char *, size_t> readData();
//  const void *readData(size_t size);
};

#endif // !UTILS_PACKER_H
