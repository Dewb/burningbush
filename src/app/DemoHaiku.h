//
//  DemoHaiku.h
//  burningbush
//
//  Created by Michael Dewberry on 3/13/15.
//
//

#ifndef burningbush_DemoHaiku_h
#define burningbush_DemoHaiku_h

LSystem createHaikuSystem() {

    LSystem system;
    system.setAxiom("1\n2\n3");

    system.addRule("1", "A_2 H_3");
    system.addRule("1", "A_1 H_4");
    system.addRule("1", "A_2 H_3");
    system.addRule("1", "Q_1 A_1 N_2 V_1");

    system.addRule("2", "T_2 G_3 D_2");
    system.addRule("2", "T_2 A_5");
    system.addRule("2", "A_1 H_3 D_3");
    system.addRule("2", "A_2 H_3 D_2");
    
    system.addRule("3", "A_1 N_1 D_2 V_1");
    system.addRule("3", "T_2 H_3");
    system.addRule("3", "H_3 P_2");
    system.addRule("3", "G_3 D_2");

    system.addRule("H_3", "N_2 V_1");
    system.addRule("H_3", "N_1 V_2");
    system.addRule("H_4", "N_2 V_2");
    system.addRule("H_4", "N_1 V_3");
    system.addRule("H_3", "S_2 C_1");
    system.addRule("H_3", "S_1 C_2");
    system.addRule("H_4", "S_2 C_2");
    system.addRule("H_4", "S_1 C_3");
    system.addRule("H_4", "N_4");

    system.addRule("Q_1", "three").setProbability(0.15);
    system.addRule("Q_1", "five").setProbability(0.15);
    system.addRule("Q_1", "A_1").setProbability(0.7);

    system.addRule("A_1", "pink");
    system.addRule("A_1", "green");
    system.addRule("A_1", "blue");
    system.addRule("A_1", "dark");
    system.addRule("A_1", "pale");
    system.addRule("A_1", "spring");
    system.addRule("A_2", "winter");
    system.addRule("A_2", "summer");
    system.addRule("A_2", "cherry");
    system.addRule("A_2", "moonlit");
    system.addRule("A_2", "fragile");
    system.addRule("A_2", "dappled");
    system.addRule("A_2", "restless");
    system.addRule("A_2", "blushing");
    system.addRule("A_2", "floating");
    system.addRule("A_2", "unseen");
    system.addRule("A_2", "alone");
    system.addRule("A_2", "gentle");
    system.addRule("A_2", "cautious");
    system.addRule("A_2", "shadowed");
    system.addRule("A_3", "delicate");
    system.addRule("A_3", "fanciful");
    system.addRule("A_3", "delightful");
    system.addRule("A_3", "vigilant");
    system.addRule("A_5", "illuminated");
    system.addRule("A_5", "unmistakable");
    system.addRule("A_5", "inseparable");
    system.addRule("A_5", "precipitating");
    system.addRule("A_5", "indescribable");
    system.addRule("A_5", "serendipitous");
    system.addRule("A_5", "still pirouetting");

    system.addRule("S_1", "spring");
    system.addRule("S_1", "rain");
    system.addRule("S_1", "wind");
    system.addRule("S_1", "air");
    system.addRule("S_2", "winter");
    system.addRule("S_2", "summer");
    system.addRule("S_2", "twilight");
    system.addRule("S_2", "forest");
    system.addRule("S_2", "starlight");
    system.addRule("S_2", "moonlight");
    system.addRule("S_2", "springtime");

    system.addRule("N_1", "leaves");
    system.addRule("N_1", "stars");
    system.addRule("N_1", "gusts");
    system.addRule("N_1", "rays");
    system.addRule("N_1", "buds");
    system.addRule("N_1", "twigs");
    system.addRule("N_2", "raindrops");
    system.addRule("N_2", "blossoms");
    system.addRule("N_2", "petals");
    system.addRule("N_2", "sisters");
    system.addRule("N_2", "brothers");
    system.addRule("N_2", "children");
    system.addRule("N_2", "flowers");
    system.addRule("N_2", "dancers");
    system.addRule("N_2", "moonbeams");
    system.addRule("N_2", "eddies");
    system.addRule("N_2", "branches");
    system.addRule("N_2", "flowers");
    system.addRule("N_4", "cherry blossoms");
    system.addRule("N_4", "cherry petals");

    system.addRule("V_1", "breathe");
    system.addRule("V_1", "fall");
    system.addRule("V_1", "twirl");
    system.addRule("V_1", "run");
    system.addRule("V_1", "cry");
    system.addRule("V_1", "speak");
    system.addRule("V_1", "dance");
    system.addRule("V_1", "sprout");
    system.addRule("V_1", "swell");
    system.addRule("V_2", "emerge");
    system.addRule("V_2", "unfurl");
    system.addRule("V_2", "alight");
    system.addRule("V_2", "ripple");
    system.addRule("V_2", "settle");
    system.addRule("V_2", "shiver");
    system.addRule("V_2", "relax");
    system.addRule("V_2", "awake");
    system.addRule("V_2", "waver");
    system.addRule("V_3", "undulate");
    system.addRule("V_3", "saturate");
    system.addRule("V_3", "escalate");
    system.addRule("V_3", "reinforce");

    system.addRule("C_1", "breathes");
    system.addRule("C_1", "falls");
    system.addRule("C_1", "twirls");
    system.addRule("C_1", "runs");
    system.addRule("C_1", "cries");
    system.addRule("C_1", "speaks");
    system.addRule("C_1", "creeps");
    system.addRule("C_1", "sprouts");
    system.addRule("C_1", "swells");
    system.addRule("C_1", "peeks");
    system.addRule("C_1", "blinks");
    system.addRule("C_2", "dances");
    system.addRule("C_2", "unfurls");
    system.addRule("C_2", "catches");
    system.addRule("C_2", "dissolves");
    system.addRule("C_2", "wavers");
    system.addRule("C_2", "alights");
    system.addRule("C_2", "ripples");
    system.addRule("C_2", "settles");
    system.addRule("C_2", "shivers");
    system.addRule("C_3", "relaxes");
    system.addRule("C_3", "awakens");
    system.addRule("C_3", "emerges");
    system.addRule("C_3", "undulates");
    system.addRule("C_3", "saturates");
    system.addRule("C_3", "escalates");

    system.addRule("T_2", "morning");
    system.addRule("T_2", "twilight");
    system.addRule("T_2", "tonight");
    system.addRule("T_2", "at dusk");
    system.addRule("T_2", "at dawn");
    system.addRule("T_2", "at last");
    system.addRule("T_2", "and soon");
    system.addRule("T_2", "long past");

    system.addRule("P_2", "on wind");
    system.addRule("P_2", "through trees");
    system.addRule("P_2", "to soil");
    system.addRule("P_2", "in leaves");
    system.addRule("P_2", "in clouds");
    system.addRule("P_2", "in mist");
    system.addRule("P_2", "in warmth");

    system.addRule("G_3", "blossoming");
    system.addRule("G_3", "unfolding");
    system.addRule("G_3", "expanding");
    system.addRule("G_3", "revealing");
    system.addRule("G_3", "fluttering");
    system.addRule("G_3", "swirling past");
    system.addRule("G_3", "raining down");
    system.addRule("G_3", "trickling down");
    system.addRule("G_3", "closing up");

    system.addRule("D_2", "P_2");
    system.addRule("D_2", "slowly");
    system.addRule("D_2", "quickly");
    system.addRule("D_2", "in dance");
    system.addRule("D_2", "in bloom");
    system.addRule("D_3", "into spring");
    system.addRule("D_3", "together");
    system.addRule("D_3", "hungrily");
    system.addRule("D_3", "cautiously");
    system.addRule("D_3", "silently");
    system.addRule("D_3", "into air");
    system.addRule("D_3", "above trees");

    return system;
}

#endif
