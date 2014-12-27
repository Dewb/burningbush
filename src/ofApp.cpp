#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    
//    int N = 5;
//    l.angle = 25.7;
//    l.edgeLength = 10 / N;
//    l.axiom = Rule("F");
//    l.rules.insert(std::pair<Rule, Rule>("F", "F[+F]F[-F]F"));
//    l.generate(N);

//    int N = 4;
//    l.angle = 22.5;
//    l.edgeLength = 10 / N;
//    l.axiom = Rule("F");
//    l.rules.insert(std::pair<Rule, Rule>("F", "FF-[-F+F+F]+[+F-F-F]"));
//    l.generate(N);
    
    int N = 5;
    l.angle = 22.5;
    l.edgeLength = 10 / N;
    l.axiom = Rule("X");
    l.rules.insert(std::pair<Rule, Rule>("X", "F-[[X]+X]+F[+FX]-X"));
    l.rules.insert(std::pair<Rule, Rule>("F", "FF"));
    l.generate(N);
    
    ofEnableDepthTest();
    
    cam.setTarget(ofVec3f(0, l.mesh.getCentroid().y * 0.75, 0));
    cam.setRotation(0.0, 0.0);
    cam.setupPerspective(false);
    cam.setDistance(N * 75);
    
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
    l.mesh.draw();
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
