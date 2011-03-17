
#include "log.h"
#include <gtest/gtest.h>

struct my_log_consumer : public log::log_consumer {
  void write(log::log_level level, const std::string &line) {
    stream << line;
  }
  std::stringstream stream; 
};

TEST(log_unittest, test_logging) {
  // registering the consumer
  my_log_consumer consumer;
  log::register_consumer(&consumer);

  log(log::warning) << "Not so bad: " << 12;
  EXPECT_EQ(consumer.stream.str(), "WA Not so bad: 12");
  consumer.stream.str(""); // strange way to clean it
} 

