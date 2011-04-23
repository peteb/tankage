#ifndef UTILS_PACKER_H
#define UTILS_PACKER_H

#include <vector>
#include <string>
#include <utility>

class Packer {  
public:
  Packer(std::vector<unsigned char> &data);
  
  void writeByte(char value);
  void writeShort(short value);
  void writeInt(int value);
  void writeString(const std::string &value);  
  void writeData(const char *data, size_t size);
  void append(const Packer &packer);
  
private:
  std::vector<unsigned char> &_data;
  bool _badbit;
};

class Unpacker {
public:
  Unpacker(const std::vector<unsigned char> &data);
  
  bool bad() const;
  char readByte();
  short readShort();
  int readInt();
  std::string readString();
  std::pair<const char *, size_t> readData();

private:  
  bool verifySize(size_t size);

  const std::vector<unsigned char> &_data;
  size_t _pos;
  bool _badbit;  
};

#endif // !UTILS_PACKER_H
