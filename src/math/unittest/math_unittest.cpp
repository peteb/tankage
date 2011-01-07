
#include <mat2.h>
#include <rect.h>
#include <vec2.h>

#include <gtest/gtest.h>

TEST(math_unittest, test_vec2) 
{
	// QUESTION kaspars: Is string comparing good enough?
	EXPECT_EQ(std::string(vec2()), "0, 0");	
	EXPECT_EQ(std::string(vec2(3.0f, 4.0f)), "3, 4");

	vec2 v1(1.1f, 2.2f), v2(3.0f, 4.0f);
	EXPECT_EQ(std::string(v1 + v2), "4.1, 6.2");
	EXPECT_EQ(std::string(v1 - v2),"-1.9, -1.8");
	EXPECT_EQ(std::string(v1 * 2.0f), "2.2, 4.4");
	EXPECT_EQ(std::string(v1 * v2), "3.3, 8.8");
	
	v1 += v2;
	EXPECT_EQ(std::string(v1), "4.1, 6.2");
	v1 -= v2;
	EXPECT_EQ(std::string(v1), "1.1, 2.2");	

	EXPECT_EQ(v2.getMagnitude(), 5.0f);
	EXPECT_EQ(std::string(v2.normalize()), "0.6, 0.8");	

	// QUESTION kaspars: Is rounding good enough?	
	// EXPECT_EQ(vec2::DotProduct(v1, v2), 2.42f) fails
	// ~src/math/unittest/math_unittest.cpp: Failure
	// Value of: 2.42f
	//   Actual: 2.4200000762939453
	// Expected: vec2::DotProduct(v1, v2)
	// Which is: 2.4199998378753662
	std::stringstream ss;
	ss << std::setprecision(5) << vec2::DotProduct(v1, v2);
	EXPECT_EQ(ss.str(), "2.42");

	EXPECT_EQ(std::string(vec2::Zero()), "0, 0");
	EXPECT_EQ(std::string(vec2::Identity()), "0, 0");
} // test_vec2

