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

TEST(LSystem, ParametricBasicParsing) {
    RuleString rs("A(min(x,y),(a+(b*c)/2),2)");
    
    EXPECT_EQ(rs.size(), 1);
    EXPECT_EQ(rs.front().symbol, "A");
    EXPECT_EQ(rs.front().parameters.size(), 3);
    if (rs.front().parameters.size() == 3) {
        EXPECT_EQ(rs.front().parameters[0], "min(x,y)");
        EXPECT_EQ(rs.front().parameters[1], "(a+(b*c)/2)");
        EXPECT_EQ(rs.front().parameters[2], "2");
    }
}

TEST(LSystem, ParametricBasicMatching) {
    LSystem system;
    system.setAxiom("A(3,2)B(5)A_b(1,2)A_c(5,7)");
    system.addRule("A(x,y)", "F(x*y)");
    system.addRule("A_b(x,y)", "+A(x-1,y-1)");
    system.addRule("A_c(x,y)", "-A(x+1,y+1)");
    system.addRule("B(x,y)", "f");
    
    EXPECT_EQ("F(6)B(5)+F(0)-F(48)", to_string(system.generate(2, true)));
}

TEST(LSystem, ParametricConditionalMatching) {
    LSystem system;
    system.setAxiom("A(6)");
    system.addRule("A(n)", "+A(n-1)").setCondition("n > 0");
    system.addRule("A(n)", "F").setCondition("n == 0");
    
    EXPECT_EQ("++++++F", to_string(system.generate(7, true)));
}

TEST(LSystem, ParametricSystemProperties) {
    LSystem system;
    system.setAxiom("A(3)");
    system.setProperty("x", 2);
    system.addRule("A(n)", "+(x)A(n-1)B(n)").setCondition("n > 0");
    system.addRule("B(n)", "C(x)").setCondition("n == x");

    EXPECT_EQ("+(2)+(2)+(2)A(0)B(1)C(2)B(3)", to_string(system.generate(7, true)));
}


TEST(LSystem, ParametricMultipleFormalParmaters) {
    LSystem system;
    system.setAxiom("A(3,2)");
    system.addRule("A(x,y)", "B(x,y)A(x+1,y+1)");

    EXPECT_EQ("B(3,2)B(4,3)A(5,4)", to_string(system.generate(2, true)));
}

#include "DemoHaiku.h"

TEST(LSystem, HaikuTest) {

    srand(time(0));
    LSystem system = createHaikuSystem();

    int count = 10;
    while (count--) {
        system.reseed(rand());
        cout << system.generate(3, false) << "\n\n";
    }

    EXPECT_EQ(true, true);
}


