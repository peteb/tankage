
#include <engine/cfg/cfg.h>
#include <gtest/gtest.h>
#include <iostream>

class TestConsumer : public CfgConsumer  {
public:
  void updateCfg(const std::string &name, const std::string &value) {
    this->name = name;
    this->value = value;       
  }
  std::string name, value;
};

TEST(CfgUnittest, TestProperty) {
  // non-existing cfgiguration filehttp://www.ericsson.com/careers
  EXPECT_ANY_THROW(Configuration::Cfg("tron"));
  // verify constructor and get property
  Configuration::Cfg cfg("../src/engine/cfg/unittest/cfg_unittest.cfg");
  EXPECT_EQ(cfg.property("control", "keyUp"), "W");  
  EXPECT_EQ(cfg.property("california", "city"), "san-jose");  
  // not existing node
  EXPECT_ANY_THROW(cfg.property("batman", "weight"));
} // TestProperty

TEST(CfgUnittest, TestConsumer) {
  Configuration::Cfg cfg("../src/engine/cfg/unittest/cfg_unittest.cfg");
  // init consumers
  TestConsumer consumer1, consumer2, consumer3;
  cfg.registerConsumer("control", &consumer1);
  cfg.registerConsumer("control", &consumer2);
  cfg.registerConsumer("california", &consumer3);
  // verify consumers after control update
  cfg.updateProperty("control", "keyUp", "Spiderman");
  EXPECT_EQ(consumer1.name, "keyUp"); 
  EXPECT_EQ(consumer1.value, "Spiderman"); 
  EXPECT_EQ(consumer2.name, consumer1.name); 
  EXPECT_EQ(consumer2.value, consumer1.value); 
  EXPECT_EQ(consumer3.name, "");
  EXPECT_EQ(consumer3.value, "");
  // verify consumers after california update 
  cfg.updateProperty("california", "city", "san-francisco");
  EXPECT_EQ(consumer3.name, "city");
  EXPECT_EQ(consumer3.value, "san-francisco");
  EXPECT_EQ(consumer1.name, "keyUp"); 
  EXPECT_EQ(consumer1.value, "Spiderman"); 
  EXPECT_EQ(consumer2.name, consumer1.name); 
  EXPECT_EQ(consumer2.value, consumer1.value); 
} // TestConsumer

