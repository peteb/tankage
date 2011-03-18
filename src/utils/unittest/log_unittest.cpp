
#include "log.h"
#include <gtest/gtest.h>

struct my_log_consumer : public tankage::log::log_consumer {
  void write(tankage::log::severity_t severity, const std::string &line) {
	stream << line;
  }
  std::stringstream stream; 
};

TEST(log_unittest, test_logging) {
  // create and register consumer
  my_log_consumer consumer;
  tankage::log::register_consumer(&consumer);
  // do any logging afterwards
  tlog(error) << "bad error occurred";
  EXPECT_NE(std::string::npos, consumer.stream.str().find("bad error"));
  consumer.stream.str("");
} // test_logging

