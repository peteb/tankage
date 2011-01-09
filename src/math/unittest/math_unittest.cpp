
#include <vec2.h>
#include <rect.h>
#include <mat2.h>

#include <gtest/gtest.h>

/* QUESTION kaspars: I'm lazy so I did compare using strings which prooves to
 * save a lot of code lines, is it good enough, though?
 * Internal ss rounding there finds difference between 3.0f and 3.00001f which
 * seems to be enough, also it is epsilon absolute error 0.00001 mentioned in
 * http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
 */
TEST(math_unittest, test_vec2)
{
    // constructors
    vec2 v1, v2(1.1f, 2.2f), v3(3.0f, 4.0f);

    EXPECT_EQ(std::string(v1), "0, 0");
    EXPECT_EQ(std::string(v2), "1.1, 2.2");
    EXPECT_EQ(std::string(v3), "3, 4");

    // operators
    EXPECT_EQ(std::string(v2 + v3), "4.1, 6.2");
    EXPECT_EQ(std::string(v2 - v3),"-1.9, -1.8");
    EXPECT_EQ(std::string(v2 * 2.0f), "2.2, 4.4");
    EXPECT_EQ(std::string(v2 * v3), "3.3, 8.8");

    v2 += v3;
    EXPECT_EQ(std::string(v2), "4.1, 6.2");
    v2 -= v3;
    EXPECT_EQ(std::string(v2), "1.1, 2.2");

    // methods
    EXPECT_FLOAT_EQ(v3.getMagnitude(), 5.0f);
    EXPECT_EQ(std::string(v3.normalize()), "0.6, 0.8");

    // static methods
    EXPECT_FLOAT_EQ(vec2::DotProduct(v2, v3), 2.42f);
    EXPECT_EQ(std::string(vec2::Zero()), "0, 0");
    EXPECT_EQ(std::string(vec2::Identity()), "0, 0");
} // test_vec2

