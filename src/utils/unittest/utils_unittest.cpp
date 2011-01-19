
#include <vec.h>
#include <rect.h>
#include <mat.h>

#include <gtest/gtest.h>

/* QUESTION kaspars: I'm lazy so I did compare using strings which prooves to
 * save a lot of code lines, is it good enough, though?
 * Internal ss rounding there finds difference between 3.0f and 3.00001f which
 * seems to be enough, also it is epsilon absolute error 0.00001 mentioned in
 * http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm */
TEST(utils_unittest, test_vec)
{
    // constructors
    vec2 v1(1.1f, 2.2f), v2(3.0f, 4.0f);

    EXPECT_EQ(std::string(v1), "1.1, 2.2");
    EXPECT_EQ(std::string(v2), "3, 4");

    // operators
    EXPECT_EQ(std::string(v1 + v2), "4.1, 6.2");
    EXPECT_EQ(std::string(v1 - v2),"-1.9, -1.8");
    EXPECT_EQ(std::string(v1 * 2.0f), "2.2, 4.4");
    EXPECT_EQ(std::string(v1 * v2), "3.3, 8.8");

    v1 += v2;
    EXPECT_EQ(std::string(v1), "4.1, 6.2");
    v1 -= v2;
    EXPECT_EQ(std::string(v1), "1.1, 2.2");

    // methods
    EXPECT_FLOAT_EQ(v2.magnitude(), 5.0f);
    EXPECT_EQ(std::string(v2.normalize()), "0.6, 0.8");

    // static methods
    EXPECT_FLOAT_EQ(dot(v1, v2), 2.42f);
    EXPECT_EQ(std::string(vec2::Zero()), "0, 0");
    EXPECT_EQ(std::string(vec2::Identity()), "0, 0");
} // test_vec

/** COMMENT kaspars: Identity and Zero probably are too obvious to test */
TEST(utils_unittest, test_mat)
{
	vec2 v1(1.1f, 2.2f), v2(3.0f, 4.0f);
	mat2 m1(v1, v2), m2(2.1, 2.2, 2.3, 2.4);

	EXPECT_FLOAT_EQ(1.1f, m1.elements[0][0]);
	EXPECT_FLOAT_EQ(2.2f, m1.elements[0][1]);
	EXPECT_FLOAT_EQ(3.0f, m1.elements[1][0]);
	EXPECT_FLOAT_EQ(4.0f, m1.elements[1][1]);

	EXPECT_FLOAT_EQ(2.1f, m2.elements[0][0]);	
	EXPECT_FLOAT_EQ(2.2f, m2.elements[0][1]);	
	EXPECT_FLOAT_EQ(2.3f, m2.elements[1][0]);	
	EXPECT_FLOAT_EQ(2.4f, m2.elements[1][1]);	

	v1 = m1 * v2;	
	EXPECT_FLOAT_EQ(v1.x, 3.0f*1.1f+4.0f*3.0f);
	EXPECT_FLOAT_EQ(v1.y, 3.0f*2.2f+4.0f*4.0f);

	// Used online matrix multiplicator app, check this out =]
	// http://www.bluebit.gr/matrix-calculator/matrix_multiplication.aspx
	mat2 m3 = m1 * m2;
	EXPECT_FLOAT_EQ(m3.elements[0][0], 7.37f);
	EXPECT_FLOAT_EQ(m3.elements[0][1], 7.7f);
	EXPECT_FLOAT_EQ(m3.elements[1][0], 15.5f);
	EXPECT_FLOAT_EQ(m3.elements[1][1], 16.2f);

	m1 *= m2;
	EXPECT_EQ(m1.elements[0][0], m3.elements[0][0]);
	EXPECT_EQ(m1.elements[0][1], m3.elements[0][1]);
	EXPECT_EQ(m1.elements[1][0], m3.elements[1][0]);
	EXPECT_EQ(m1.elements[1][1], m3.elements[1][1]);

	EXPECT_FLOAT_EQ(m3.x().x, 7.37f);
	EXPECT_FLOAT_EQ(m3.x().y, 7.7f);
	EXPECT_FLOAT_EQ(m3.y().x, 15.5f);
	EXPECT_FLOAT_EQ(m3.y().y, 16.2f);
} // test_mat

TEST(utils_unittest, test_rect)
{

} // test_rect
