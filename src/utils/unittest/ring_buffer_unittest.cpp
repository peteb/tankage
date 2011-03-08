
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

  ring.erase(ring.begin());
  EXPECT_EQ(*ring.begin(), "tokachu");
  EXPECT_EQ(std::distance(ring.begin(), ring.end()), (size_t)1);

  // Testing erasing
  ring_buffer<std::string> new_ring(2);
  new_ring.push_back("herble");
  new_ring.push_back("tokachu");
  EXPECT_EQ(*(new_ring.begin()), "herble");
  new_ring.erase(new_ring.begin());
  EXPECT_EQ(*(new_ring.begin()), "tokachu");
  new_ring.erase(new_ring.begin());
  EXPECT_TRUE(new_ring.begin() == new_ring.end());

  ring_buffer<int> int_ring(100);
  for (int i = 0; i < 100; ++i) {
    int_ring.push_back(i);
  }

  // Locate number 40
  ring_buffer<int>::iterator element40 = std::find(
    int_ring.begin(), int_ring.end(), 40);

  EXPECT_EQ(*element40, 40);
  EXPECT_EQ(std::distance(int_ring.begin(), element40), 40);
  *element40 = 1337;
  int_ring.erase(int_ring.begin(), element40);
  EXPECT_EQ(std::distance(int_ring.begin(), int_ring.end()), 60);
  
} // test_ring_buffer

