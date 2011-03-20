#ifndef UTILS_PACKER_H
#define UTILS_PACKER_H

#include <vector>

class Packer {
  std::vector<char> &_data;
  
public:
  Packer(std::vector<char> &data);
  
  void writeShort(short value);
/*  void writeInt(int value);
  void writeString(const std::string &value);
  void writeData(const void *data, size_t size);*/
};

/*class Unpacker {
public:
  Unpacker(const void *data, size_t size);
  
  short readShort();
  int readInt();
  std::string readString();
  const void *readData();
};*/

#endif // !UTILS_PACKER_H
