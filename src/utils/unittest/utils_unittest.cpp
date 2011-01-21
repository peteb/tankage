
#include <utils/vec.h>
#include <utils/rect.h>
#include <utils/mat.h>

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
    vec2 v1(1.1f, 2.2f), v2(3.3f, 4.4f);
    rect r1, r2(v1, v2), r3(v1, 5.5f, 6.6f), r4(7.7f, 8.8f);

    EXPECT_EQ(std::string(r1.origin),"0, 0");
    EXPECT_EQ(std::string(r1.halfSize), "0, 0");

    EXPECT_EQ(std::string(r2.origin), "1.1, 2.2");
    EXPECT_EQ(std::string(r2.halfSize), "3.3, 4.4");

    EXPECT_EQ(std::string(r3.origin), "1.1, 2.2");
    EXPECT_EQ(std::string(r3.halfSize), "5.5, 6.6");

    EXPECT_EQ(std::string(r4.origin), "0, 0");
    EXPECT_EQ(std::string(r4.halfSize), "3.85, 4.4");

    vec2 v3, v4;
    r2.getCoords(v3, v4);
    EXPECT_EQ(std::string(v3), "-2.2, -2.2");
    EXPECT_EQ(std::string(v4), "4.4, 6.6");

    EXPECT_FLOAT_EQ(r2.width(), 6.6f);
    EXPECT_FLOAT_EQ(r2.height(), 8.8f);

    rect r5(vec2(3.0f, 3.0f), vec2(1.0f, 1.0f));

    // RCGN kaspars: Great! This is collision detection, right?
    EXPECT_FALSE(rect::intersect(r5, rect(2.0f, 2.0f)));
    EXPECT_FALSE(rect::intersect(r5, rect(3.99f, 3.99f)));
    EXPECT_TRUE(rect::intersect(r5, rect(4.0f, 4.0f)));
    EXPECT_TRUE(rect::intersect(r5, rect(6.0f, 6.0f)));
} // test_rect

