
#include "log.h"
#include <gtest/gtest.h>

struct my_log_consumer { 
  void operator()(tankage::log::severity_t severity, const std::string &line) {
	stream << line;
  }
  static std::stringstream stream; 
};

std::stringstream my_log_consumer::stream;

TEST(utils_unittest, test_logging) {
  // create and register consumer
  tankage::log::register_consumer(my_log_consumer());
  // do any logging afterwards
  tanklog(error) << "bad error occurred";
  EXPECT_NE(std::string::npos, my_log_consumer::stream.str().find("bad error"));
  my_log_consumer::stream.str("");
} // test_logging

