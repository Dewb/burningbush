#include "LSystem.h"
#include <gtest/gtest.h>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(LSystem, ParallelReplacement) {
    LSystem system;
    system.setAxiom("abc+");
    system.addRule('a', "b+");
    system.addRule('b', "c+");
    system.addRule('c', "d+");
    system.addRule('+', "/");
    
    EXPECT_EQ("b+c+d+/", to_string(system.generate(1, true)));
}

TEST(LSystem, SimpleContext) {
    LSystem system;
    system.setAxiom("baaaaaaaa");
    system.addRule("b", 'a', "", "b");
    system.addRule('b', "a");

    EXPECT_EQ("aaaaaaaab", to_string(system.generate(8, true)));
}

TEST(LSystem, LeftContextOnEdge) {
    LSystem system;
    system.setAxiom("aaabb");
    system.addRule("aaa", 'b', "", "c");
    
    EXPECT_EQ("aaacb", to_string(system.generate(1, true)));
}

TEST(LSystem, RightContextOnEdge) {
    LSystem system;
    system.setAxiom("bbaaa");
    system.addRule("", 'b', "aaa", "c");
    
    EXPECT_EQ("bcaaa", to_string(system.generate(1, true)));
}

TEST(LSystem, LeftContextOverEdge) {
    LSystem system;
    system.setAxiom("aaabb");
    system.addRule("aaaa", 'b', "", "c");
    
    EXPECT_EQ("aaabb", to_string(system.generate(1, true)));
}

TEST(LSystem, RightContextOverEdge) {
    LSystem system;
    system.setAxiom("bbaaa");
    system.addRule("", 'b', "aaaa", "c");
    
    EXPECT_EQ("bbaaa", to_string(system.generate(1, true)));
}

TEST(LSystem, ContextSkipBranches) {
    LSystem system;
    system.setAxiom("a[x]b[x]c");
    system.addRule("a", 'b', "c", "+");
    
    EXPECT_EQ("a[x]+[x]c", to_string(system.generate(1, true)));
}

TEST(LSystem, LeftContextSkipNestedBranches) {
    LSystem system;
    system.setAxiom("aaa[xy[xxyb][[xxb]cc]]b");
    system.addRule("aaa", 'b', "", "+"); // should match
    system.addRule("b", 'c', "", "+");   // should not match
    
    EXPECT_EQ("aaa[xy[xxyb][[xxb]cc]]+", to_string(system.generate(1, true)));
}

TEST(LSystem, RightContextEnterBranch) {
    LSystem system;
    system.setAxiom("a[x]b[x]c");
    system.addRule("a", 'b', "x", "+");
    
    EXPECT_EQ("a[x]+[x]c", to_string(system.generate(1, true)));
}

TEST(LSystem, RightContextEnterNestedBranch) {
    LSystem system;
    system.setAxiom("a[x]b[x[y[za]f]d]c");
    system.addRule("a", 'b', "xyz", "+");
    system.addRule("b", 'x', "yf", "+");
    
    EXPECT_EQ("a[x]+[+[y[za]f]d]c", to_string(system.generate(1, true)));
}

TEST(LSystem, SubscriptMatching) {
    LSystem system;
    system.setAxiom("F_c/F/F_a");
    system.addRule("F_a", "+F_b");
    system.addRule("F_b", "-F_a");
    
    EXPECT_EQ("F_c/F/+-+-F_a", to_string(system.generate(4, true)));
}
