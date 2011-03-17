
#include "log.h"
#include <gtest/gtest.h>

TEST(log_unittest, test_interface) {
  //log::scoped_enter enter("log_unittest");
  //log::write(log::error, "very bad error");
  //log::write(log::warning, "this is bad but not error");
  //log::tweet("tweet message goes here");

  //log(log::error);

  log(log::warning) << 34 << ", more";
  log(log::error) << "very bad thing";

}

