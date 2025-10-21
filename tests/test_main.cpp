#include <gtest/gtest.h>

// Simple add function for testing
int add(int a, int b) {
    return a + b;
}

// Simple test
TEST(BasicTest, AddFunction) {
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(0, 0), 0);
    EXPECT_EQ(add(-1, 1), 0);
}
