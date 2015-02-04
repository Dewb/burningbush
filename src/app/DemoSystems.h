//
//  DemoSystems.h
//  burningbush
//
//  Created by Michael Dewberry on 1/31/15.
//
//

#ifndef burningbush_DemoSystems_h
#define burningbush_DemoSystems_h

#include <DemoLSystemApp.h>

using namespace std;

void createSystems(vector<pair<LSystem, GeneratorType> >& systems) {
    
    LSystem system;

    system.reset();
    system.setAxiom("E[CV]B");
    system.addRule("B", "[B][B]").setProbability(0.55);
    system.addRule("B", "P_dS").setProbability(0.15);
    system.addRule("B", "P_lS").setProbability(0.15);
    system.addRule("B", "P_rS").setProbability(0.15);
    system.addRule("P_d", "C[+F][-F]P_d").setProbability(0.9);
    system.addRule("P_d", "C[+F][-F]-P_l").setProbability(0.05);
    system.addRule("P_d", "C[+F][-F]+P_r").setProbability(0.05);
    system.addRule("P_l", "C_n[+F]P_l").setProbability(0.9);
    system.addRule("P_l", "C_n[+F]-P_d").setProbability(0.1);
    system.addRule("P_r", "C_n[-F]P_r").setProbability(0.9);
    system.addRule("P_r", "C_n[-F]+P_d").setProbability(0.1);
    system.addRule("C_n", "+C").setProbability(0.2);
    system.addRule("C_n", "-C").setProbability(0.2);
    system.addRule("C_n", "C").setProbability(0.6);
    system.setProperty("N", 3);
    systems.push_back(make_pair(system, GeneratorTypeFloorplan));
    
    system.reset();
    system.setTitle("\"Row of Trees\" parametric OL-system from Mandlebrot 1982 (ABOP p.47)");
    system.setAxiom("F(1,0)");
    system.addRule("F(x,t)", "F(x*0.3,2)+F(x*0.45826,1)--F(x*0.45826,1)+F(x*0.7,0)").setCondition("t=0");
    system.addRule("F(x,t)", "F(x,t-1)").setCondition("t>0");
    system.setProperty("N", 10);
    system.setProperty("angle", 86);
    system.setProperty("edgeLength", 500.0);
    system.setProperty("heading", 90);
    system.setProperty("position_x", -0.8);
    system.setProperty("position_y", 0.5);
    systems.push_back(make_pair(system, GeneratorTypeLine));
    
    system.reset();
    system.setTitle("2L-system from Hogeweg and Hesper 1974 (ABOP p.34)");
    system.setAxiom("F1F1F1");
    system.ignoreForContext("+-F");
    system.addRule("0", '0', "0", "0");
    system.addRule("0", '0', "1", "1[+F1F1]");
    system.addRule("0", '1', "0", "1");
    system.addRule("0", '1', "1", "1");
    system.addRule("1", '0', "0", "0");
    system.addRule("1", '0', "1", "1F1");
    system.addRule("1", '1', "0", "0");
    system.addRule("1", '1', "1", "0");
    system.addRule('+', "-");
    system.addRule('-', "+");
    system.setProperty("N", 30);
    system.setProperty("angle", 22.5);
    system.setProperty("edgeLength", 5.0);
    systems.push_back(make_pair(system, GeneratorTypeLine));
    
    system.reset();
    system.setTitle("2L-system from Hogeweg and Hesper 1974 (ABOP p.34)");
    system.setAxiom("F1F1F1");
    system.ignoreForContext("+-F");
    system.addRule("0", '0', "0", "1");
    system.addRule("0", '0', "1", "1[-F1F1]");
    system.addRule("0", '1', "0", "1");
    system.addRule("0", '1', "1", "1");
    system.addRule("1", '0', "0", "0");
    system.addRule("1", '0', "1", "1F1");
    system.addRule("1", '1', "0", "1");
    system.addRule("1", '1', "1", "0");
    system.addRule('+', "-");
    system.addRule('-', "+");
    system.setProperty("N", 30);
    system.setProperty("angle", 22.5);
    system.setProperty("edgeLength", 7.0);
    systems.push_back(make_pair(system, GeneratorTypeLine));
    
    system.reset();
    system.setTitle("2L-system from Hogeweg and Hesper 1974 (ABOP p.34)");
    system.setAxiom("F1F1F1");
    system.ignoreForContext("+-F");
    system.addRule("0", '0', "0", "0");
    system.addRule("0", '0', "1", "1");
    system.addRule("0", '1', "0", "0");
    system.addRule("0", '1', "1", "1[+F1F1]");
    system.addRule("1", '0', "0", "0");
    system.addRule("1", '0', "1", "1F1");
    system.addRule("1", '1', "0", "0");
    system.addRule("1", '1', "1", "0");
    system.addRule('+', "-");
    system.addRule('-', "+");
    system.setProperty("N", 26);
    system.setProperty("angle", 25.75);
    system.setProperty("edgeLength", 10.0);
    systems.push_back(make_pair(system, GeneratorTypeLine));
    
    system.reset();
    system.setTitle("Stochastic OL-system example (ABOP p.29)");
    system.setAxiom("F");
    system.addRule('F', "F[+F]F[-F]F").setProbability(0.333);
    system.addRule('F', "F[+F]F").setProbability(0.333);
    system.addRule('F', "F[-F]F").setProbability(0.334);
    system.setProperty("N", 6);
    system.setProperty("angle", 25.7);
    system.setProperty("edgeLength", 3.0);
    systems.push_back(make_pair(system, GeneratorTypeLine));
    
    system.reset();
    system.setTitle("Branching OL-system example (ABOP p.25)");
    system.setAxiom("F");
    system.addRule('F', "FF-[-F+F+F]+[+F-F-F]");
    system.setProperty("N", 4);
    system.setProperty("angle", 22.5);
    system.setProperty("edgeLength", 10);
    systems.push_back(make_pair(system, GeneratorTypeLine));
    
    system.reset();
    system.setTitle("Branching OL-system example (ABOP p.25)");
    system.setAxiom("X");
    system.addRule('X', "F-[[X]+X]+F[+FX]-X");
    system.addRule('F', "FF");
    system.setProperty("N", 6);
    system.setProperty("angle", 22.5);
    system.setProperty("edgeLength", 3);
    systems.push_back(make_pair(system, GeneratorTypeLine));
    
    system.reset();
    system.setTitle("Hexagonal Gosper curve from Gardner 1976 (ABOP p.12)");
    system.setAxiom("F_l");
    system.addRule("F_l", "F_l+F_r++F_r-F_l--F_lF_l-F_r+");
    system.addRule("F_r", "-F_l+F_rF_r++F_r+F_l--F_l-F_r");
    system.setProperty("N", 4);
    system.setProperty("angle", 60);
    system.setProperty("edgeLength", 7);
    system.setProperty("position_x", 0.33);
    system.setProperty("position_y", -0.25);
    systems.push_back(make_pair(system, GeneratorTypeLine));
    
    system.reset();
    system.setTitle("Islands and lakes example from Mandlebrot 1982 (ABOP p.9)");
    system.setAxiom("F+F+F+F");
    system.addRule('F', "F+f-FF+F+FF+Ff+FF-f+FF-F-FF-Ff-FFF");
    system.addRule('f', "ffffff");
    system.setProperty("N", 2);
    system.setProperty("angle", 90);
    system.setProperty("edgeLength", 9);
    system.setProperty("position_x", 0.33);
    system.setProperty("position_y", 0.33);
    systems.push_back(make_pair(system, GeneratorTypeLine));
    
    system.reset();
    system.setTitle("Penrose Tiling method #1");
    system.setAxiom("[F_a]++[F_a]++[F_a]++[F_a]++[F_a]");
    system.addRule("F_a", "+F_b--F_d[---F_c--F_a]+");
    system.addRule("F_b", "-F_c++F_a[+++F_b++F_d]-");
    system.addRule("F_c", "F_b++F_d----F_a[-F_b----F_c]++");
    system.addRule("F_d", "--F_b++++F_c[+F_d++++F_a]--F_a");
    system.setProperty("N", 6);
    system.setProperty("angle", 36);
    system.setProperty("edgeLength", 10);
    system.setProperty("position_x", 0);
    system.setProperty("position_y", 0);
    systems.push_back(make_pair(system, GeneratorTypeLine));
    
    /*
     system.reset();
     system.setAxiom("'[f_a]++[f_a]++[f_a]++[f_a]++[f_a]");
     system.addRule("f_a", "+{f_b--f_d[---f_c--f_a]}f_b--f_d+");
     system.addRule("f_b", "-{'f_c++f_a[+++f_b++f_d]}f_c++f_a-");
     system.addRule("f_c", "f_b++{f_d----f_a[-f_b----f_c]}f_d----f_a++");
     system.addRule("f_d", "--{f_b++++f_c[+f_d++++f_a]}f_b++++f_c--f_a");
     system.setProperty("N", 5);
     system.setProperty("angle", 36);
     system.setProperty("segmentLength", 10);
     system.setProperty("segmentRadius", 1);
     system.setProperty("colorBook", 2);
     systems.push_back(make_pair(system, GeneratorTypeMesh));
     */
    
    system.reset();
    system.setTitle("Penrose Tiling method #2");
    system.setAxiom("x");
    system.addRule("x", "@!+{F[|\"'y]--F['|x][|+@f#''x]---['''x]F--[\"''y]F}");
    system.addRule("y", "@!++{[\"x]F|+F[|'y]-[''y]F|+F[|\"'x]}");
    system.addRule("F", "f");
    system.setProperty("N", 5);
    system.setProperty("angle", 36);
    system.setProperty("segmentLength", 150);
    system.setProperty("segmentRadius", 1.2);
    system.setProperty("colorBook", 3);
    systems.push_back(make_pair(system, GeneratorTypeMesh));
    
    
    system.reset();
    system.setTitle("3D example (ABOP p.26) with color and context modifications");
    system.setAxiom("A");
    system.ignoreForContext("&L!A/S'\"^+-`|");
    system.addRule('A', "[&FL!A]/////'[&FL!A]///////'[&FL!A]");
    system.addRule('F', "S/////F");
    system.addRule('S', "FL");
    system.addRule("FFFFFFFFFFFFFF", 'L', "", "[\"''^^{-F+`F+`F-|-F+'F+'F}]");
    system.setProperty("N", 7);
    system.setProperty("angle", 22.5);
    system.setProperty("segmentLength", 4.5);
    system.setProperty("segmentRadius", 2);
    system.setProperty("colorBook", 0);
    systems.push_back(make_pair(system, GeneratorTypeMesh));
    
    system.reset();
    system.setTitle("3D extension of Hilbert curve from Stevens/Lehar/Perston 1983 (ABOP p.20)");
    system.setAxiom("A");
    system.addRule('A', "B-F+CFC+F-D&F^D-F+&&CFC+F+B//");
    system.addRule('B', "A&F^CFB^F^D^^-F-D^|F^B|FC^F^A//");
    system.addRule('C', "|D^|F^B-F+C^F^A&&FA&F^C+F+B^F^D//");
    system.addRule('D', "|CFB-F+B|FA&F^A&&FB-F+B|FC//");
    system.setProperty("N", 3);
    system.setProperty("angle", 90);
    system.setProperty("segmentLength", 10);
    system.setProperty("segmentRadius", 2);
    system.setProperty("shiny", 1);
    systems.push_back(make_pair(system, GeneratorTypeMesh));
    
    system.reset();
    system.setTitle("3D stochastic example (ABOP p.27, p.30)");
    system.setAxiom("P");
    system.addRule('P', "I+[P+O]--//[--L]I[++L]-[PO]++PO");
    system.addRule('I', "FS[//&&L][//^^L]FS");
    system.addRule('S', "S[//&&L][//^^L]FS").setProbability(0.333);
    system.addRule('S', "SFS").setProbability(0.333);
    system.addRule('S', "S").setProbability(0.334);
    system.addRule('L', "['{+F-FF-F+|+F-FF-F}]");
    system.addRule('O', "[&&&D''/W////W////W////W////W]");
    system.addRule('D', "FF");
    system.addRule('W', "['^!F][{&&&&-F+F|-F+F}]");
    system.setProperty("N", 6);
    system.setProperty("angle", 18.0);
    system.setProperty("segmentLength", 3.0);
    system.setProperty("segmentRadius", 0.5);
    system.setProperty("colorBook", 1);
    systems.push_back(make_pair(system, GeneratorTypeMesh));
}

void createColorBooks(ofPtrArray<ColorBook>& colorBooks) {
    
    ofPtr<ColorBook> cb(new ColorBook());
    cb->addGradient(ofColor::saddleBrown, ofColor::brown, 4);
    cb->addGradient(ofColor(180, 110, 60), 5);
    cb->addGradient(ofColor(210, 130, 90), 3);
    cb->nextSeries();
    cb->addGradient(ofColor::yellow, ofColor::orangeRed, 7);
    cb->addGradient(ofColor::red, 3);
    cb->add(ofColor(230, 0, 0));
    cb->addGradient(ofColor(160, 0, 0), 3);
    cb->nextSeries();
    cb->addGradient(ofColor::darkGreen, ofColor::forestGreen, 4);
    cb->addGradient(ofColor::lawnGreen, 4);
    cb->addGradient(ofColor::greenYellow, 6);
    colorBooks.push_back(cb);
    
    cb.reset(new ColorBook());
    cb->add(ofColor(0, 160, 0));
    cb->add(ofColor(80, 200, 0));
    cb->add(ofColor::lavenderBlush);
    cb->add(ofColor::white);
    colorBooks.push_back(cb);
    
    cb.reset(new ColorBook());
    cb->add(ofColor::white);
    cb->add(ofColor::red);
    cb->add(ofColor::orange);
    cb->add(ofColor::maroon);
    cb->add(ofColor::olive);
    colorBooks.push_back(cb);
    
    cb.reset(new ColorBook());
    cb->add(ofColor::cornsilk);
    cb->addGradient(ofColor::blueViolet, 8);
    cb->nextSeries();
    cb->add(ofColor::paleGreen);
    cb->addGradient(ofColor::forestGreen, 8);
    colorBooks.push_back(cb);
}



#endif
