#include <utils/packer.h>
#include <gtest/gtest.h>

TEST(PackerTest, TestBasicPackUnpack) {
  char buffer[512]; // very fast buffer allocated on stack =]

  Packer packer(buffer, buffer + 512);
  packer.writeShort(17);
  packer.writeInt(42);
  packer.writeString("san-jose"); 
  EXPECT_EQ(packer.size(), 
    sizeof(short) + sizeof(int) + sizeof(short) + sizeof(char) * 8);

  Unpacker unpacker(buffer, buffer + 512); 
  EXPECT_EQ(unpacker.readShort(), 17);
  EXPECT_EQ(unpacker.readInt(), 42);

  EXPECT_EQ(unpacker.readString(), "san-jose");
}

