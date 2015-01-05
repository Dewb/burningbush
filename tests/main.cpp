#include "LSystem.h"
#include <gtest/gtest.h>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(LSystem, SimpleContext) {
    LSystem system;
    system.axiom = "baaaaaaaa";
    system.addRule("b", 'a', "", "b");
    system.addRule('b', "a");

    EXPECT_EQ("aaaaaaaab", system.generate(8, true));
}

