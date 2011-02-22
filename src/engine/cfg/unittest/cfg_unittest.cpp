
#include <engine/cfg/cfg.h>
#include <gtest/gtest.h>
#include <iostream>

class TestConsumer : public ConfigConsumer  {
public:
  void updateConfig(const std::string &name, const std::string &value) {
    this->name = name;
    this->value = value;       
  }
  std::string name, value;
};

TEST(CfgUnittest, TestProperty) {
  // non-existing configiguration file
  EXPECT_ANY_THROW(Engine::Config("tron"));
  // verify constructor and get property
  Engine::Config config("../src/engine/cfg/unittest/cfg_unittest.cfg");
  EXPECT_EQ(config.property("control", "keyUp", ""), "W");  
  EXPECT_EQ(config.property("california", "city", ""), "san-jose");  
  // not existing node
  EXPECT_ANY_THROW(config.property("batman", "weight", ""));
  // not existing property
  EXPECT_EQ(config.property("control", "batman", "signal"), "signal");
} // TestProperty

TEST(CfgUnittest, TestConsumer) {
  Engine::Config config("../src/engine/cfg/unittest/cfg_unittest.cfg");
  // init consumers
  TestConsumer consumer1, consumer2, consumer3;
  config.registerConsumer("control", &consumer1);
  config.registerConsumer("control", &consumer2);
  config.registerConsumer("california", &consumer3);
  // verify consumers after control update
  config.updateProperty("control", "keyUp", "Spiderman");
  EXPECT_EQ(consumer1.name, "keyUp"); 
  EXPECT_EQ(consumer1.value, "Spiderman"); 
  EXPECT_EQ(consumer2.name, consumer1.name); 
  EXPECT_EQ(consumer2.value, consumer1.value); 
  EXPECT_EQ(consumer3.name, "");
  EXPECT_EQ(consumer3.value, "");
  // verify consumers after california update 
  config.updateProperty("california", "city", "san-francisco");
  EXPECT_EQ(consumer3.name, "city");
  EXPECT_EQ(consumer3.value, "san-francisco");
  EXPECT_EQ(consumer1.name, "keyUp"); 
  EXPECT_EQ(consumer1.value, "Spiderman"); 
  EXPECT_EQ(consumer2.name, consumer1.name); 
  EXPECT_EQ(consumer2.value, consumer1.value); 
} // TestConsumer

