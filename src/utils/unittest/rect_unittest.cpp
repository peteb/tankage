
#include <utils/rect.h>
#include <gtest/gtest.h>


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

