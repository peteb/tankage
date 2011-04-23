
#include <utils/packer.h>
#include <gtest/gtest.h>
#include <algorithm>

TEST(Packer, PackBasicTypes) {
  std::vector<unsigned char> buffer;
  Packer packer(buffer);

  EXPECT_EQ(0u, buffer.size());

  packer.writeByte(0xAB);
  EXPECT_EQ(1u, buffer.size());
  
  packer.writeShort(0x1337);
  EXPECT_EQ(3u, buffer.size());

  packer.writeByte(0xBA);
  EXPECT_EQ(4u, buffer.size());

  packer.writeInt(42);
  EXPECT_EQ(8u, buffer.size());
  
  packer.writeString("san jose"); 
  EXPECT_EQ(18u, buffer.size());

  packer.writeByte(0x00);
  EXPECT_EQ(19u, buffer.size());
}

TEST(Packer, UnpackBasicTypes) {
  std::vector<unsigned char> buffer;
  Packer packer(buffer);

  packer.writeByte(0xAB);
  packer.writeShort(0x1337);
  packer.writeByte(0xBA);
  packer.writeInt(42);
  packer.writeString("san jose"); 
  packer.writeByte(0x00);
  packer.writeShort(-12345);
  packer.writeShort(0xFFFF);
  
  std::vector<unsigned char> buffer_copy(buffer);
  Unpacker unpacker(buffer);
  EXPECT_EQ(23u, buffer.size());
  
  EXPECT_EQ(static_cast<char>(0xAB), unpacker.readByte());
  EXPECT_EQ(0x1337, unpacker.readShort());
  EXPECT_EQ(static_cast<char>(0xBA), unpacker.readByte());
  EXPECT_EQ(42, unpacker.readInt());
  EXPECT_STREQ("san jose", unpacker.readString().c_str());
  EXPECT_EQ(0x00, unpacker.readByte());
  EXPECT_EQ(-12345, unpacker.readShort());
  EXPECT_EQ(0xFFFF, (unsigned short)unpacker.readShort());
  
  EXPECT_EQ(false, unpacker.bad());
  EXPECT_EQ(true, std::equal(buffer.begin(), buffer.end(), buffer_copy.begin()));
}

TEST(Packer, UnpackNegative) {
  std::vector<unsigned char> buffer;

  {
    Unpacker unpacker(buffer);
    EXPECT_EQ(false, unpacker.bad());
    unpacker.readByte();
    EXPECT_EQ(true, unpacker.bad());    
  }

  {
    Unpacker unpacker(buffer);
    EXPECT_EQ(false, unpacker.bad());
    unpacker.readShort();
    EXPECT_EQ(true, unpacker.bad());    
  }
  
  {
    Unpacker unpacker(buffer);
    EXPECT_EQ(false, unpacker.bad());
    unpacker.readInt();
    EXPECT_EQ(true, unpacker.bad());    
  }

  {
    Unpacker unpacker(buffer);
    EXPECT_EQ(false, unpacker.bad());
    unpacker.readString();
    EXPECT_EQ(true, unpacker.bad());    
  }
  
  Packer packer(buffer);
  
  packer.writeByte(0xAB);
  packer.writeShort(0x1337);
  packer.writeByte(0xBA);
  packer.writeInt(42);
  packer.writeString("san jose"); 
  packer.writeByte(0x00);
  
  Unpacker unpacker(buffer);
  EXPECT_EQ(false, unpacker.bad());
  unpacker.readByte();
  unpacker.readShort();
  unpacker.readByte();
  unpacker.readInt();
  unpacker.readString();
  unpacker.readByte();
  
  {
    Unpacker second(unpacker);
    EXPECT_EQ(false, second.bad());   
    second.readByte();
    EXPECT_EQ(true, second.bad());   
  }
  
  {
    Unpacker second(unpacker);
    EXPECT_EQ(false, second.bad());   
    second.readShort();
    EXPECT_EQ(true, second.bad());   
  }

  {
    Unpacker second(unpacker);
    EXPECT_EQ(false, second.bad());   
    second.readInt();
    EXPECT_EQ(true, second.bad());   
  }  

  {
    Unpacker second(unpacker);
    EXPECT_EQ(false, second.bad());   
    second.readString();
    EXPECT_EQ(true, second.bad());   
  }    
}

TEST(Packer, MultiPackers) {
  std::vector<unsigned char> main;
  Packer main_pack(main);
  main_pack.writeString("greenman cometh");
  
  for (size_t i = 0; i < 10; ++i) {
    std::vector<unsigned char> sub;
    Packer sub_pack(sub);
    sub_pack.writeInt(i);
  
    main_pack.append(sub_pack);
  }
  
  EXPECT_EQ(57u, main.size());
  
  Unpacker unpack(main);
  EXPECT_STREQ("greenman cometh", unpack.readString().c_str());
  EXPECT_EQ(0, unpack.readInt());
  EXPECT_EQ(false, unpack.bad());
  EXPECT_EQ(1, unpack.readInt());
  EXPECT_EQ(2, unpack.readInt());
}

TEST(Packer, BigData) {
  std::vector<unsigned char> msg;

  char *data = static_cast<char *>(malloc(0xFFFF));
  for (size_t i = 0; i < 0xFFFF; ++i) {
    data[i] = (char)i;
  }
  
  {
    Packer pack(msg);
    pack.writeData(data, 0xFFFF);    
  }
  
  {
    Unpacker unpack(msg);
    std::pair<const char *, size_t> data_read = unpack.readData();
    
    EXPECT_EQ(0xFFFFu, data_read.second);
    
    bool same = true;
    for (size_t i = 0; i < data_read.second; ++i) {
      if (data_read.first[i] != data[i])
        same = false;
    }
    
    EXPECT_TRUE(same);
  }
}

TEST(Packer, BadData) {
  std::vector<unsigned char> data;
  Packer pack(data);
  pack.writeShort((unsigned short)-1);
  
  Unpacker unpack(data);
  std::pair<const char *, size_t> read_data = unpack.readData();
  EXPECT_EQ(0u, read_data.second);
  EXPECT_TRUE(unpack.bad());
}

TEST(Packer, String) {
  // mostly tested through data tests
}