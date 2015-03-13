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

TEST(LSystem, HaikuTest) {

    LSystem system;
    system.setAxiom("1\n2\n3");
    system.addRule("1", "A(2) H(3)").setProbability(1.0/4.0);
    system.addRule("1", "A(1) H(4)").setProbability(1.0/4.0);
    system.addRule("1", "A(2) H(3)").setProbability(1.0/4.0);
    system.addRule("1", "Q(1) A(1) N(2) V(1)").setProbability(1.0/4.0);
    system.addRule("2", "T(2) G(3) D(2)").setProbability(1.0/4.0);
    system.addRule("2", "T(2) A(5)").setProbability(1.0/4.0);
    system.addRule("2", "A(1) H(3) D(3)").setProbability(1.0/4.0);
    system.addRule("2", "A(2) H(3) D(2)").setProbability(1.0/4.0);
    system.addRule("3", "A(1) N(1) D(2) V(1)").setProbability(1.0/4.0);
    system.addRule("3", "T(2) H(3)").setProbability(1.0/4.0);
    system.addRule("3", "H(3) P(2)").setProbability(1.0/4.0);
    system.addRule("3", "G(3) D(2)").setProbability(1.0/4.0);

    system.addRule("H(k)", "N(2) V(1)").setCondition("k = 3");
    system.addRule("H(k)", "N(1) V(2)").setCondition("k = 3");
    system.addRule("H(k)", "N(2) V(2)").setCondition("k = 4");
    system.addRule("H(k)", "N(1) V(3)").setCondition("k = 4");
    system.addRule("H(k)", "S(2) C(1)").setCondition("k = 3");
    system.addRule("H(k)", "S(1) C(2)").setCondition("k = 3");
    system.addRule("H(k)", "S(2) C(2)").setCondition("k = 4");
    system.addRule("H(k)", "S(1) C(3)").setCondition("k = 4");

    system.addRule("Q(k)", "three").setCondition("k == 1");
    system.addRule("Q(k)", "five").setCondition("k == 1");
    system.addRule("Q(k)", "A(1)").setCondition("k == 1");
    system.addRule("Q(k)", "A(1)").setCondition("k == 1");
    system.addRule("Q(k)", "A(1)").setCondition("k == 1");

    system.addRule("A(k)", "pink").setCondition("k == 1");
    system.addRule("A(k)", "green").setCondition("k == 1");
    system.addRule("A(k)", "blue").setCondition("k == 1");
    system.addRule("A(k)", "dark").setCondition("k == 1");
    system.addRule("A(k)", "pale").setCondition("k == 1");
    system.addRule("A(k)", "spring").setCondition("k == 1");
    system.addRule("A(k)", "winter").setCondition("k == 2");
    system.addRule("A(k)", "summer").setCondition("k == 2");
    system.addRule("A(k)", "cherry").setCondition("k == 2");
    system.addRule("A(k)", "moonlit").setCondition("k == 2");
    system.addRule("A(k)", "fragile").setCondition("k == 2");
    system.addRule("A(k)", "dappled").setCondition("k == 2");
    system.addRule("A(k)", "restless").setCondition("k == 2");
    system.addRule("A(k)", "blushing").setCondition("k == 2");
    system.addRule("A(k)", "floating").setCondition("k == 2");
    system.addRule("A(k)", "unseen").setCondition("k == 2");
    system.addRule("A(k)", "alone").setCondition("k == 2");
    system.addRule("A(k)", "gentle").setCondition("k == 2");
    system.addRule("A(k)", "cautious").setCondition("k == 2");
    system.addRule("A(k)", "shadowed").setCondition("k == 2");
    system.addRule("A(k)", "delicate").setCondition("k == 3");
    system.addRule("A(k)", "fanciful").setCondition("k == 3");
    system.addRule("A(k)", "delightful").setCondition("k == 3");
    system.addRule("A(k)", "vigilant").setCondition("k == 3");
    system.addRule("A(k)", "illuminated").setCondition("k == 5");
    system.addRule("A(k)", "unmistakable").setCondition("k == 5");
    system.addRule("A(k)", "inseparable").setCondition("k == 5");
    system.addRule("A(k)", "precipitating").setCondition("k == 5");
    system.addRule("A(k)", "indescribable").setCondition("k == 5");
    system.addRule("A(k)", "serendipitous").setCondition("k == 5");
    system.addRule("A(k)", "still pirouetting").setCondition("k == 5");

    system.addRule("S(k)", "spring").setCondition("k == 1");
    system.addRule("S(k)", "rain").setCondition("k == 1");
    system.addRule("S(k)", "wind").setCondition("k == 1");
    system.addRule("S(k)", "air").setCondition("k == 1");
    system.addRule("S(k)", "winter").setCondition("k == 2");
    system.addRule("S(k)", "summer").setCondition("k == 2");
    system.addRule("S(k)", "twilight").setCondition("k == 2");
    system.addRule("S(k)", "forest").setCondition("k == 2");
    system.addRule("S(k)", "starlight").setCondition("k == 2");
    system.addRule("S(k)", "moonlight").setCondition("k == 2");
    system.addRule("S(k)", "springtime").setCondition("k == 2");

    system.addRule("N(k)", "leaves").setCondition("k == 1");
    system.addRule("N(k)", "stars").setCondition("k == 1");
    system.addRule("N(k)", "gusts").setCondition("k == 1");
    system.addRule("N(k)", "rays").setCondition("k == 1");
    system.addRule("N(k)", "buds").setCondition("k == 1");
    system.addRule("N(k)", "twigs").setCondition("k == 1");
    system.addRule("N(k)", "raindrops").setCondition("k == 2");
    system.addRule("N(k)", "blossoms").setCondition("k == 2");
    system.addRule("N(k)", "petals").setCondition("k == 2");
    system.addRule("N(k)", "sisters").setCondition("k == 2");
    system.addRule("N(k)", "brothers").setCondition("k == 2");
    system.addRule("N(k)", "children").setCondition("k == 2");
    system.addRule("N(k)", "flowers").setCondition("k == 2");
    system.addRule("N(k)", "dancers").setCondition("k == 2");
    system.addRule("N(k)", "moonbeams").setCondition("k == 2");
    system.addRule("N(k)", "eddies").setCondition("k == 2");
    system.addRule("N(k)", "branches").setCondition("k == 2");
    system.addRule("N(k)", "flowers").setCondition("k == 2");

    system.addRule("V(k)", "breathe").setCondition("k == 1");
    system.addRule("V(k)", "fall").setCondition("k == 1");
    system.addRule("V(k)", "twirl").setCondition("k == 1");
    system.addRule("V(k)", "run").setCondition("k == 1");
    system.addRule("V(k)", "cry").setCondition("k == 1");
    system.addRule("V(k)", "speak").setCondition("k == 1");
    system.addRule("V(k)", "dance").setCondition("k == 1");
    system.addRule("V(k)", "sprout").setCondition("k == 1");
    system.addRule("V(k)", "swell").setCondition("k == 1");
    system.addRule("V(k)", "emerge").setCondition("k == 2");
    system.addRule("V(k)", "unfurl").setCondition("k == 2");
    system.addRule("V(k)", "alight").setCondition("k == 2");
    system.addRule("V(k)", "ripple").setCondition("k == 2");
    system.addRule("V(k)", "settle").setCondition("k == 2");
    system.addRule("V(k)", "shiver").setCondition("k == 2");
    system.addRule("V(k)", "relax").setCondition("k == 2");
    system.addRule("V(k)", "awake").setCondition("k == 2");
    system.addRule("V(k)", "waver").setCondition("k == 2");
    system.addRule("V(k)", "undulate").setCondition("k == 3");
    system.addRule("V(k)", "saturate").setCondition("k == 3");
    system.addRule("V(k)", "escalate").setCondition("k == 3");
    system.addRule("V(k)", "reinforce").setCondition("k == 3");

    system.addRule("C(k)", "breathes").setCondition("k == 1");
    system.addRule("C(k)", "falls").setCondition("k == 1");
    system.addRule("C(k)", "twirls").setCondition("k == 1");
    system.addRule("C(k)", "runs").setCondition("k == 1");
    system.addRule("C(k)", "cries").setCondition("k == 1");
    system.addRule("C(k)", "speaks").setCondition("k == 1");
    system.addRule("C(k)", "creeps").setCondition("k == 1");
    system.addRule("C(k)", "sprouts").setCondition("k == 1");
    system.addRule("C(k)", "swells").setCondition("k == 1");
    system.addRule("C(k)", "peeks").setCondition("k == 1");
    system.addRule("C(k)", "blinks").setCondition("k == 1");
    system.addRule("C(k)", "dances").setCondition("k == 2");
    system.addRule("C(k)", "unfurls").setCondition("k == 2");
    system.addRule("C(k)", "catches").setCondition("k == 2");
    system.addRule("C(k)", "dissolves").setCondition("k == 2");
    system.addRule("C(k)", "wavers").setCondition("k == 2");
    system.addRule("C(k)", "alights").setCondition("k == 2");
    system.addRule("C(k)", "ripples").setCondition("k == 2");
    system.addRule("C(k)", "settles").setCondition("k == 2");
    system.addRule("C(k)", "shivers").setCondition("k == 2");
    system.addRule("C(k)", "relaxes").setCondition("k == 3");
    system.addRule("C(k)", "awakens").setCondition("k == 3");
    system.addRule("C(k)", "emerges").setCondition("k == 3");
    system.addRule("C(k)", "undulates").setCondition("k == 3");
    system.addRule("C(k)", "saturates").setCondition("k == 3");
    system.addRule("C(k)", "escalates").setCondition("k == 3");

    system.addRule("T(k)", "morning").setCondition("k == 2");
    system.addRule("T(k)", "twilight").setCondition("k == 2");
    system.addRule("T(k)", "tonight").setCondition("k == 2");
    system.addRule("T(k)", "at dusk").setCondition("k == 2");
    system.addRule("T(k)", "at dawn").setCondition("k == 2");
    system.addRule("T(k)", "at last").setCondition("k == 2");
    system.addRule("T(k)", "and soon").setCondition("k == 2");
    system.addRule("T(k)", "long past").setCondition("k == 2");

    system.addRule("P(k)", "on wind").setCondition("k == 2");
    system.addRule("P(k)", "through trees").setCondition("k == 2");
    system.addRule("P(k)", "to soil").setCondition("k == 2");
    system.addRule("P(k)", "in leaves").setCondition("k == 2");
    system.addRule("P(k)", "in clouds").setCondition("k == 2");
    system.addRule("P(k)", "in mist").setCondition("k == 2");
    system.addRule("P(k)", "in warmth").setCondition("k == 2");

    system.addRule("G(k)", "blossoming").setCondition("k == 3");
    system.addRule("G(k)", "unfolding").setCondition("k == 3");
    system.addRule("G(k)", "expanding").setCondition("k == 3");
    system.addRule("G(k)", "revealing").setCondition("k == 3");
    system.addRule("G(k)", "fluttering").setCondition("k == 3");
    system.addRule("G(k)", "swirling past").setCondition("k == 3");
    system.addRule("G(k)", "raining down").setCondition("k == 3");
    system.addRule("G(k)", "trickling down").setCondition("k == 3");
    system.addRule("G(k)", "closing up").setCondition("k == 3");

    system.addRule("D(k)", "P(2)").setCondition("k == 2");
    system.addRule("D(k)", "slowly").setCondition("k == 2");
    system.addRule("D(k)", "quickly").setCondition("k == 2");
    system.addRule("D(k)", "in dance").setCondition("k == 2");
    system.addRule("D(k)", "in bloom").setCondition("k == 2");
    system.addRule("D(k)", "into spring").setCondition("k == 3");
    system.addRule("D(k)", "together").setCondition("k == 3");
    system.addRule("D(k)", "hungrily").setCondition("k == 3");
    system.addRule("D(k)", "cautiously").setCondition("k == 3");
    system.addRule("D(k)", "silently").setCondition("k == 3");
    system.addRule("D(k)", "into air").setCondition("k == 3");
    system.addRule("D(k)", "above trees").setCondition("k == 3");

    srand(time(0));

    int count = 10;
    while (count--) {
        system.reseed(rand());
        cout << system.generate(3, false) << "\n\n";
    }

    EXPECT_EQ(true, true);
}


