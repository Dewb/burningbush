#include "DemoLSystemApp.h"

//--------------------------------------------------------------
void DemoLSystemApp::setup(){

    
//    int N = 5;
//    l.angle = 25.7;
//    l.edgeLength = 10 / N;
//    l.axiom = Rule("F");
//    l.rules.insert(std::pair<Rule, Rule>("F", "F[+F]F[-F]F"));

//    int N = 4;
//    l.angle = 22.5;
//    l.edgeLength = 10 / N;
//    l.axiom = Rule("F");
//    l.rules.insert(std::pair<Rule, Rule>("F", "FF-[-F+F+F]+[+F-F-F]"));
    
//    int N = 5;
//    l.angle = 22.5;
//    l.edgeLength = 10 / N;
//    l.axiom = Rule("X");
//    l.rules.insert(std::pair<Rule, Rule>("X", "F-[[X]+X]+F[+FX]-X"));
//    l.rules.insert(std::pair<Rule, Rule>("F", "FF"));
    
//    int N = 4;
//    l.angle = 22.5;
//    l.edgeLength = 10 / N;
//    l.axiom = Rule("X");
//    l.rules.insert(std::pair<Rule, Rule>("X", "F-[[-X]+XX]+F[+F+XF+FX]-X"));
//    l.rules.insert(std::pair<Rule, Rule>("F", "FF"));
    
    LSystem system;
    system.axiom = "X";
    system.addRule("X", "F-[[X]+X]+F[+FX]-X");
    system.addRule("F", "FF");
    system.setProperty("angle", 22.5);
    systems.push_back(system);
    
    system.reset();
    system.axiom = "F+F+F+F";
    system.addRule("F", "F+f-FF+F+FF+Ff+FF-f+FF-F-FF-Ff-FFF");
    system.addRule("f", "ffffff");
    system.setProperty("angle", 90);
    systems.push_back(system);
    
    system.reset();
    system.axiom = "A";
    system.addRule("A", "[&FL!A]/////'[&FL!A]///////'[&FL!A]");
    system.addRule("F", "S/////F");
    system.addRule("S", "FL");
    system.addRule("L", "['''^^{-f+f+f-|-f+f+f}]");
    system.setProperty("angle", 22.5);
    systems.push_back(system);
    
    system.reset();
    system.axiom = "A";
    system.addRule("A", "B-F+CFC+F-D&F^D-F+&&CFC+F+B//");
    system.addRule("B", "A&F^CFB^F^D^^-F-D^|F^B|FC^F^A//");
    system.addRule("C", "|D^|F^B-F+C^F^A&&FA&F^C+F+B^F^D//");
    system.addRule("D", "|CFB-F+B|FA&F^A&&FB-F+B|FC//");
    system.setProperty("angle", 90);
    systems.push_back(system);
    
    MeshGeneratorState state;
    state.position = ofVec3f(0, 0, 0);
    state.heading = ofVec3f(1, 0, 0);
    state.up = ofVec3f(0, 1, 0);
    state.left = state.heading.crossed(state.up);
    state.angle = 90;
    state.segmentLength = 10;
    state.segmentRadius = 1;
    mesh_gen.generate(systems[3], state, 3);
    mesh = state.mesh;
    
    ofEnableDepthTest();
    
    cam.setTarget(ofVec3f(0, mesh->getCentroid().y * 0.75, 0));
    cam.setRotation(0.0, 0.0);
    cam.setupPerspective(false);
    cam.setDistance(250);
    
    ofEnableAlphaBlending();
    ofEnableAntiAliasing();
    
    mode = Line;
}

//--------------------------------------------------------------
void DemoLSystemApp::update(){
}

//--------------------------------------------------------------
void DemoLSystemApp::draw(){
    ofBackground(0, 0, 80);
    ofSetColor(200, 200, 180);
    ofSetLineWidth(1.5);
    
    string title;
    int system = 0;

    if (mode == Line) {
        system = 0;
        title = "LineGenerator";
        LineGeneratorState state;
        state.angle = 22.5;
        state.edgeLength = 2.5;
        state.position = ofVec2f(ofGetWidth()/2, ofGetHeight() * 0.8);
        state.heading = ofVec3f(0, -1);
        line_gen.generate(systems[system], state, 5);
    } else if (mode == Mesh) {
        system = 3;
        title = "MeshGenerator";
        cam.begin();
        mesh->draw();
        cam.end();
    }
    
    ofDrawBitmapString(systems[system].axiom, 20, 25);
    int n = 0;
    for (auto iter = systems[system].rules.begin(); iter != systems[system].rules.end(); ++iter) {
        ofDrawBitmapString(iter->first + " -> " + iter->second, 20, 45 + n);
        n += 15;
    }
    ofDrawBitmapString(title, 20, ofGetHeight() - 25);
}

//--------------------------------------------------------------
void DemoLSystemApp::keyPressed(int key){
    if (key == 'm') {
        if (mode == Line) {
            mode = Mesh;
        } else if (mode == Mesh) {
            mode = Line;
        }
    }
}

//--------------------------------------------------------------
void DemoLSystemApp::keyReleased(int key){

}

//--------------------------------------------------------------
void DemoLSystemApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void DemoLSystemApp::mouseDragged(int x, int y, int button){
    cam.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void DemoLSystemApp::mousePressed(int x, int y, int button){
    cam.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void DemoLSystemApp::mouseReleased(int x, int y, int button){
    cam.mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void DemoLSystemApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void DemoLSystemApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void DemoLSystemApp::dragEvent(ofDragInfo dragInfo){

}
