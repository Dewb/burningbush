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
    
    int N = 5;

    system.axiom = "X";
    system.addRule("X", "F-[[X]+X]+F[+FX]+X-X");
    system.addRule("F", "FF");
    
    MeshGeneratorState state;
    state.position = ofVec3f(0, 0, 0);
    state.heading = ofVec3f(0, 1, 0);
    state.angle = 27.5;
    state.edgeLength = 10 / N;
    mesh_gen.generate(system, state, N);
    mesh = state.mesh;
    
    ofEnableDepthTest();
    
    cam.setTarget(ofVec3f(0, mesh->getCentroid().y * 0.75, 0));
    cam.setRotation(0.0, 0.0);
    cam.setupPerspective(false);
    cam.setDistance(N * 50);
    
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

    if (mode == Line) {
        title = "LineGenerator";
        LineGeneratorState state;
        state.angle = 17.5;
        state.edgeLength = 1.5;
        state.position = ofVec2f(ofGetWidth()/2, ofGetHeight() * 0.8);
        state.heading = ofVec3f(0, -1);
        line_gen.generate(system, state, 5);
    } else if (mode == Mesh) {
        title = "MeshGenerator";
        cam.begin();
        mesh->draw();
        cam.end();
    }
    
    ofDrawBitmapString(system.axiom, 20, 25);
    int n = 0;
    for (auto iter = system.rules.rbegin(); iter != system.rules.rend(); ++iter) {
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
