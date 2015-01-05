#include "LSystem.h"
#include <gtest/gtest.h>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(LSystem, ParallelReplacement) {
    LSystem system;
    system.axiom = "abc+";
    system.addRule('a', "b+");
    system.addRule('b', "c+");
    system.addRule('c', "d+");
    system.addRule('+', "/");
    
    EXPECT_EQ("b+c+d+/", system.generate(1, true));
}

TEST(LSystem, SimpleContext) {
    LSystem system;
    system.axiom = "baaaaaaaa";
    system.addRule("b", 'a', "", "b");
    system.addRule('b', "a");

    EXPECT_EQ("aaaaaaaab", system.generate(8, true));
}

TEST(LSystem, LeftContextOnEdge) {
    LSystem system;
    system.axiom = "aaabb";
    system.addRule("aaa", 'b', "", "c");
    
    EXPECT_EQ("aaacb", system.generate(1, true));
}

TEST(LSystem, RightContextOnEdge) {
    LSystem system;
    system.axiom = "bbaaa";
    system.addRule("", 'b', "aaa", "c");
    
    EXPECT_EQ("bcaaa", system.generate(1, true));
}

TEST(LSystem, LeftContextOverEdge) {
    LSystem system;
    system.axiom = "aaabb";
    system.addRule("aaaa", 'b', "", "c");
    
    EXPECT_EQ("aaabb", system.generate(1, true));
}

TEST(LSystem, RightContextOverEdge) {
    LSystem system;
    system.axiom = "bbaaa";
    system.addRule("", 'b', "aaaa", "c");
    
    EXPECT_EQ("bbaaa", system.generate(1, true));
}

TEST(LSystem, ContextSkipBranches) {
    LSystem system;
    system.axiom = "a[x]b[x]c";
    system.addRule("a", 'b', "c", "+");
    
    EXPECT_EQ("a[x]+[x]c", system.generate(1, true));
}

TEST(LSystem, RightContextEnterBranch) {
    LSystem system;
    system.axiom = "a[x]b[x]c";
    system.addRule("a", 'b', "x", "+");
    
    EXPECT_EQ("a[x]+[x]c", system.generate(1, true));
}
