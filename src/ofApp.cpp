#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    
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
    MeshGeneratorState state;
    state.position = ofVec3f(0, 0, 0);
    state.heading = ofVec3f(0, 1, 0);
    state.angle = 27.5;
    state.edgeLength = 10 / N;
    mg.setInitialState(state);
    
    mg.getSystem().axiom = "X";
    mg.getSystem().addRule("X", "F-[[X]+X]+F[+FX]+X-X");
    mg.getSystem().addRule("F", "FF");
    
    mesh = mg.generate(N);
    
    ofEnableDepthTest();
    
    cam.setTarget(ofVec3f(0, mesh.getCentroid().y * 0.75, 0));
    cam.setRotation(0.0, 0.0);
    cam.setupPerspective(false);
    cam.setDistance(N * 50);
    
    ofEnableAlphaBlending();
    ofEnableAntiAliasing();
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 80);
    ofSetColor(200, 200, 180);
    ofSetLineWidth(1.5);
    
    cam.begin();
    //ofDrawAxis(30.0);
    mesh.draw();
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    cam.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    cam.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    cam.mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
