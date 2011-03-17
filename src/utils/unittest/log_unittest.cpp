
#include "log.h"
#include <gtest/gtest.h>

class my_log_consumer : public log::log_consumer {
  void write(const std::string& line) {
    //std::cout << line << std::endl;
  }
};


TEST(log_unittest, test_interface) {
  // registering the consumer
  my_log_consumer consumer;
  //log::register_consumer(&consumer);
  // some test logs
  log(log::warning) << 34 << ", more";
  log(log::error) << "very bad thing";
 
}

