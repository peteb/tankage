
#include <utils/ring_buffer.h>
#include <gtest/gtest.h>
#include <string>


TEST(utils_unittest, test_ring_buffer) {
  ring_buffer<std::string> ring(2); 
  ring.push_back("tokachu");
  ring.push_back("hashmander");
  
  ring_buffer<std::string>::iterator it = ring.begin(); 
  EXPECT_EQ(*it, "tokachu");   
  EXPECT_EQ(*(++it), "hashmander");   

  ring.push_back("herble");
  EXPECT_EQ(std::distance(ring.begin(), ring.end()), (size_t)2);

  it = ring.begin();
  EXPECT_EQ(*it, "hashmander");
  EXPECT_EQ(*(++it), "herble");
  EXPECT_EQ(std::distance(ring.rbegin(), ring.rend()), (size_t)2);

  ring_buffer<std::string>::reverse_iterator rit = ring.rbegin(); 
  EXPECT_EQ(*rit, "herble");
  EXPECT_EQ(*(++rit), "hashmander");
 
  for (size_t i(0); i != 101; ++i) {
    ring.push_back("tokachu");
  } 
  EXPECT_EQ(std::distance(ring.begin(), ring.end()), (size_t)2);

  ring.pop_front(ring.begin());
  EXPECT_EQ(*ring.begin(), "herble");
  EXPECT_EQ(std::distance(ring.begin(), ring.end()), (size_t)1);
} // test_ring_buffer

