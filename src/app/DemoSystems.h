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
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

void parseSystems(vector<pair<LSystem, GeneratorType> >& systems) {
    LSystem system;
    ProductionRule* pLastRule = NULL;

    string line, token, data, data1, data2;

    ifstream file;
    file.open("data/systems.txt");

    while (file.is_open() && !file.eof()) {
        getline(file, line);
        if (line.empty() || line[0] == '#') {
            continue;
        }

        int cr = line.find("\\n");
        while (cr != string::npos)
        {
            line.replace(cr, 2, "\n");
            cr = line.find("\\n", cr);
        }

        int firstSpace = line.find(" ");
        int secondSpace = line.find(" ", firstSpace + 1);
        token = line.substr(0, firstSpace);
        data = line.substr(firstSpace + 1, string::npos);
        data1 = line.substr(firstSpace + 1, secondSpace - firstSpace - 1);
        data2 = line.substr(secondSpace + 1, string::npos);

        if (token == "system") {
            system.reset();
            system.setTitle(data);
        } else if (token == "axiom") {
            system.setAxiom(data1);
        } else if (token == "rule") {
            pLastRule = &system.addRule(data1, data2);
        } else if (token == "probability") {
            if (pLastRule) {
                float f = atof(data1.c_str());
                pLastRule->setProbability(f);
            }
        } else if (token == "condition") {
            if (pLastRule) {
                pLastRule->setCondition(data);
            }
        } else if (token == "property") {
            float f = atof(data2.c_str());
            system.setProperty(data1, f);
        } else if (token == "generate") {
            if (data1 == "mesh") {
                systems.push_back(make_pair(system, GeneratorTypeMesh));
            } else if (data1 == "line") {
                systems.push_back(make_pair(system, GeneratorTypeLine));
            } else if (data1 == "text") {
                systems.push_back(make_pair(system, GeneratorTypeText));
            } else if (data1 == "none") {
                systems.push_back(make_pair(system, GeneratorTypeNone));
            }
        } else {
            cout << "WARNING: Unrecognized token " << token << endl;
        }

    }

    file.close();
}

void createSystems(vector<pair<LSystem, GeneratorType> >& systems) {
    
    LSystem system;

    system.reset();
    system.setTitle("Starflake (Dewb, from a concept by cosinezero)");
    system.setAxiom("P(50.0)");
    //system.addRule("P(x)", "P(x/3)[f(x)+(180-a)[P(x/3)F(x)+(a)](s)]");
    //system.addRule("P(x)", "'P(x/3)[F(x)+(180-360/s)[P(x/3)F(x)+(360/s)](s)]");
    //system.addRule("P(r)", "'P(r/3.0)[F(r)+(180-(s-2)*90/s)[P(r/3.0)F(2*r*sin(3.14159/s))+(360.0/s)](s)]");
    system.addRule("P(r)", "'[!P(r*f)]['[f(r)[+(x)&(y)/(z)!P(r*f)]+(180-(s-2)*90/s)F(2*r*sin(3.14159/s))]+(360/s)](s)");
    system.setProperty("N", 1);
    system.setProperty("s", 6.0);
    system.setProperty("f", 1.0/3.0);
    system.setProperty("x", 0);
    system.setProperty("y", 0);
    system.setProperty("z", 0);
    system.setProperty("angle", 30);
    system.setProperty("edgeLength", 1.0);
    system.setProperty("heading", 0);
    system.setProperty("position_x", 0);
    system.setProperty("position_y", 0);
    system.setProperty("segmentLength", 0.15);
    system.setProperty("segmentRadius", 0.2);
    system.setProperty("tropism", 0.0);
    system.setProperty("colorBook", 3);
    system.setProperty("shiny", 1);
    systems.push_back(make_pair(system, GeneratorTypeMesh));

    system.reset();
    system.setTitle("Starflake X6 (Dewb, from a concept by cosinezero)");
    system.setAxiom("P(1.0)");
    //system.addRule("P", "[@P][f++++[@P]F++[@P]F++[@P]F++[@P]F++[@P]F++[@P]F]");
    //system.addRule("P", "[@P][f++++[@P]F++[@P]F++[@P]F++[@P]F++[@P]F++[@P]F]");
    system.addRule("P(x)", "P(x/3)[f(x)++++P(x/3)F(x)++P(x/3)F(x)++P(x/3)F(x)++P(x/3)F(x)++P(x/3)F(x)++P(x/3)F(x)]");
    //system.addRule("F(x)", "F(x/3)+(90)F(x/6)-(90)F(x/3)-(90)F(x/6)+(90)F(x/3)").setProbability(0.1);
    system.setProperty("N", 1);
    system.setProperty("angle", 30);
    system.setProperty("edgeLength", 100.0);
    system.setProperty("heading", 0);
    system.setProperty("position_x", 0);
    system.setProperty("position_y", 0);
    system.setProperty("segmentLength", 20.0);
    system.setProperty("segmentLengthGrowthFactor", 1.0/3.0);
    system.setProperty("segmentRadius", 0.1);
    system.setProperty("tropism", 0.0);
    system.setProperty("colorBook", 4);
    systems.push_back(make_pair(system, GeneratorTypeMesh));

    system.reset();
    system.setTitle("Bonsai (by Dewb)");
    system.setAxiom("!(1)+(45)F(48)/(19)VA");
    system.addRule("A", "/(60)[VF(12)B]/(60)[VF(8)B]/(19)F(4)B");
    system.addRule("B", "/(19)[VF(8)C]/(189)[VF(8)C]/(19)F(8)C");
    system.addRule("C", "/(19)[VXC]/(19)XC").setProbability(0.8);
    system.addRule("C", "").setProbability(0.2);
    system.addRule("X", "F(4)^(45)F(4)").setProbability(0.4);
    system.addRule("X", "F(4)").setProbability(0.6);
    system.addRule("V", "F(0.5)-(45)F(0.5)").setProbability(0.5);
    system.setProperty("N", 9);
    system.setProperty("angle", 18.95);
    system.setProperty("segmentLength", 0.5);
    system.setProperty("segmentRadius", 0.1);
    system.setProperty("tropism", 0.0);
    system.setProperty("colorBook", 4);
    systems.push_back(make_pair(system, GeneratorTypeMesh));

    system.reset();
    system.setTitle("Cherry Tree (by Dewb)");
    system.setAxiom("!(0.132)F_t(300)/(45)X");
    system.addRule("X", "B(1)!(0.132)F_t(50)[&F_t(50)A(1)]R[&F_t(50)A(1)]R[&F_t(50)A(1)]");
    system.addRule("A(n)", "B(n)!(0.132)S[&SA(n+1)]R[&SA(n+1)]R[&SA(n+1)]").setCondition("n < 6");
    system.addRule("R", "/(94.74)").setProbability(0.5);
    system.addRule("R", "/(132.63)").setProbability(0.5);
    system.addRule("S", "F(50)").setProbability(0.3);
    system.addRule("S", "F(30)").setProbability(0.3);
    system.addRule("S", "").setProbability(0.3);
    system.addRule("F(l)", "F(l*1.249)").setProbability(0.5);
    system.addRule("F(l)", "/[f(l/3)[B(l/10)]f(l/4)[/B(l/10)]]F(l)").setProbability(0.125);
    system.addRule("F(l)", "/[f(2*l/3)[B(l/10)]f(l/6)[/B(l/10)]]F(l)").setProbability(0.125);
    system.addRule("F(l)", "/[f(l/5)[B(l/10)]/(15)f(l/5)[/B(l/10)]f(2*l/5)[/B(l/10)]]F(l)").setProbability(0.125);
    system.addRule("F(l)", "/[f(l/4)[B(l/10)]/(30)f(l/12)[B(l/10)]f(l/5)[/B(l/10)]]F(l)").setProbability(0.125);
    system.addRule("!(w)", "!(w*1.732)");
    system.addRule("B(x)", "[T(0)//&(60)L][T(0)//^(60)L]").setCondition("x < 5");
    system.addRule("B(x)", "[T(0)O]").setCondition("x >= 5");
    system.addRule("L", "[@(0.09)\"{+(18)G(5)-(18)'G(10)-(18)'G(5)+(270)'G(5)-(18)'G(10)-(18)'G(5)}]");
    system.addRule("O", "+(90)g[&&D/W/(72)W/(72)W/(72)W/(72)W]");
    //system.addRule("D", "[@(0.05)\"\"FF]");
    system.addRule("W", "[@(0.1)!(0.1)\"\"/(20)&(20)G(0.15)][\"\"\"@(0.05){&(40)-(68.2)G(2.6926)+(46.4)'G(2.6926)+(31.3)'G(3.0414)+(46.8)'G(3.6056)+(67.4)'G(3.6056)+(46.8)`G(3.0414)+(31.3)`G(2.6926)+(46.4)`G(2.6926)}{\"&(39)-(45)G(1.4142)+(66.8)G(2.6926)+(136.4)G(2.6926)+(66.8)G(1.4142)}]");
    system.setProperty("N", 9);
    system.setProperty("angle", 18.95);
    system.setProperty("segmentLength", 15.0);
    system.setProperty("segmentRadius", 0.1);
    system.setProperty("tropism", 0.0);
    system.setProperty("colorBook", 4);
    systems.push_back(make_pair(system, GeneratorTypeMesh));

    
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

    cb.reset(new ColorBook());
    cb->add(ofColor::black);
    cb->nextSeries();
    cb->add(ofColor::black);
    cb->addGradient(ofColor::darkGreen, 5);
    cb->nextSeries();
    cb->add(ofColor::darkRed);
    cb->nextSeries();
    cb->add(ofColor::red);
    cb->add(ofColor::pink);
    cb->addGradient(ofColor::white, 3);
    cb->nextSeries();
    cb->add(ofColor::red);
    colorBooks.push_back(cb);
}



#endif
