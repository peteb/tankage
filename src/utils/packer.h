#ifndef UTILS_PACKER_H
#define UTILS_PACKER_H

#include <vector>
#include <string>

class Packer {
  void * _pos;
  void * const _end;

public:
  Packer(void *start, void *end);
  
  void writeShort(short value);
  void writeInt(int value);
  void writeString(const std::string &value);
};

class Unpacker {
  const void * _pos;
  const void * const _end;
  
public:
  Unpacker(const void *data, const void *end);
  
  short readShort();
  /*int readInt();
  std::string readString();
  const void *readData();*/
};

#endif // !UTILS_PACKER_H
