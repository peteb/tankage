#ifndef UTILS_PACKER_H
#define UTILS_PACKER_H

#include <vector>
#include <string>

class Packer {
  void * _pos;
  void * _start;
  void * const _end;

public:
  Packer(void *start, void *end);
  
  size_t size() const;
  
  void writeByte(char value);
  void writeShort(short value);
  void writeInt(int value);
  void writeString(const std::string &value);
};

class Unpacker {
  const void * _pos;
  const void * const _end;
  
public:
  Unpacker(const void *data, const void *end);
  
  char readByte();
  short readShort();
  int readInt();
  std::string readString();
};

#endif // !UTILS_PACKER_H
