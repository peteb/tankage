
#include <utils/mat.h>
#include <gtest/gtest.h>


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

