
#include <game/common/conf.h>
#include <gtest/gtest.h>
#include <iostream>

class TestConsumer : public ConfConsumer  {
public:
  void updateConf(const std::string &name, const std::string &value) {
    this->name = name;
    this->value = value;       
  }
  std::string name, value;
};

TEST(ConfUnittest, TestProperty) {
  // non-existing configuration file
  EXPECT_ANY_THROW(Conf("tron"));
  // verify constructor and get property
  Conf conf("../src/game/common/unittest/test.conf");
  EXPECT_EQ(conf.property("control", "keyUp"), "W");  
  EXPECT_EQ(conf.property("california", "city"), "san-jose");  
  // not existing node
  EXPECT_ANY_THROW(conf.property("batman", "weight"));
} // TestProperty

TEST(ConfTest, TestConsumer) {
  Conf conf("../src/game/common/unittest/test.conf");
  // init consumers
  TestConsumer consumer1, consumer2, consumer3;
  conf.registerConsumer("control", &consumer1);
  conf.registerConsumer("control", &consumer2);
  conf.registerConsumer("california", &consumer3);
  // verify consumers after control update
  conf.updateProperty("control", "keyUp", "Spiderman");
  EXPECT_EQ(consumer1.name, "keyUp"); 
  EXPECT_EQ(consumer1.value, "Spiderman"); 
  EXPECT_EQ(consumer2.name, consumer1.name); 
  EXPECT_EQ(consumer2.value, consumer1.value); 
  EXPECT_EQ(consumer3.name, "");
  EXPECT_EQ(consumer3.value, "");
  // verify consumers after california update 
  conf.updateProperty("california", "city", "san-francisco");
  EXPECT_EQ(consumer3.name, "city");
  EXPECT_EQ(consumer3.value, "san-francisco");
  EXPECT_EQ(consumer1.name, "keyUp"); 
  EXPECT_EQ(consumer1.value, "Spiderman"); 
  EXPECT_EQ(consumer2.name, consumer1.name); 
  EXPECT_EQ(consumer2.value, consumer1.value); 
} // TestConsumer

