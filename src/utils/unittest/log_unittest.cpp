
#include "log.h"
#include <gtest/gtest.h>

struct MyLogConsumer { 
  void operator()(Log::Severity severity, const std::string &line) {
	stream << line;
  }
  static std::stringstream stream; 
};

std::stringstream MyLogConsumer::stream;

TEST(utils_unittest, test_logging) {
  // create and register consumer
  Log::registerConsumer(MyLogConsumer());
  // do any logging afterwards
  Log(ERROR) << "bad error occurred";
  EXPECT_NE(std::string::npos, MyLogConsumer::stream.str().find("bad error"));
  MyLogConsumer::stream.str("");
} // test_logging

