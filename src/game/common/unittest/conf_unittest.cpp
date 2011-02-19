
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

TEST(ConfUnittest, TestAll) {
  // verify constructor and get property
  Conf conf("../src/game/common/unittest/test.cosnf");
  EXPECT_EQ(conf.property("control", "keyUp"), "W");  
  // init consumer
  TestConsumer consumer;
  conf.registerConsumer("control", &consumer);
  // verify consumer
  conf.updateProperty("control", "keyUp", "Spiderman");
  EXPECT_EQ(consumer.name, "keyUp"); 
  EXPECT_EQ(consumer.value, "Spiderman"); 
} // TestAll

