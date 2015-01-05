#include "DemoLSystemApp.h"

template <typename T>
string to_string(T t, int precision = 2)
{
    ostringstream ss;
    ss.precision(precision);
    ss << t;
    return ss.str();
}

//--------------------------------------------------------------
void DemoLSystemApp::setup(){
    
    LSystem system;

    system.reset();
    system.axiom = "F1F1F1";
    system.ignoreContext = "+-F";
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
    system.setProperty("edgeLength", 4.0);
    systems.push_back(system);
    
    system.reset();
    system.axiom = "F1F1F1";
    system.ignoreContext = "+-F";
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
    system.setProperty("edgeLength", 4.0);
    systems.push_back(system);
    
    system.reset();
    system.axiom = "F1F1F1";
    system.ignoreContext = "+-F";
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
    system.setProperty("edgeLength", 8.0);
    systems.push_back(system);
    
    system.reset();
    system.axiom = "F";
    system.addRule('F', "F[+F]F[-F]F");
    system.addRule('F', "F[+F]F");
    system.addRule('F', "F[-F]F");
    system.setProperty("N", 5);
    system.setProperty("angle", 25.7);
    system.setProperty("edgeLength", 2.0);
    systems.push_back(system);
    
    system.reset();
    system.axiom = "F";
    system.addRule('F', "FF-[-F+F+F]+[+F-F-F]");
    system.setProperty("N", 4);
    system.setProperty("angle", 22.5);
    system.setProperty("edgeLength", 10);
    systems.push_back(system);
    
    system.reset();
    system.axiom = "X";
    system.addRule('X', "F-[[X]+X]+F[+FX]-X");
    system.addRule('F', "FF");
    system.setProperty("N", 6);
    system.setProperty("angle", 22.5);
    system.setProperty("edgeLength", 3);
    systems.push_back(system);
    
    system.reset();
    system.axiom = "F+F+F+F";
    system.addRule('F', "F+f-FF+F+FF+Ff+FF-f+FF-F-FF-Ff-FFF");
    system.addRule('f', "ffffff");
    system.setProperty("N", 2);
    system.setProperty("angle", 90);
    system.setProperty("edgeLength", 9);
    systems.push_back(system);
    
    lastLineSystem = systems.size() - 1;
    
    system.reset();
    system.axiom = "A";
    system.ignoreContext = "&L!A/S'\"^+-`|";
    system.addRule('A', "[&FL!A]/////'[&FL!A]///////'[&FL!A]");
    system.addRule('F', "S/////F");
    system.addRule('S', "FL");
    system.addRule('L', "[\"''^^{-f+`f+`f-|-f+'f+'f}]");
    system.setProperty("N", 7);
    system.setProperty("angle", 22.5);
    system.setProperty("segmentLength", 4.5);
    system.setProperty("segmentRadius", 2);
    system.setProperty("colorBook", 0);
    systems.push_back(system);
    
    system.reset();
    system.axiom = "A";
    system.addRule('A', "B-F+CFC+F-D&F^D-F+&&CFC+F+B//");
    system.addRule('B', "A&F^CFB^F^D^^-F-D^|F^B|FC^F^A//");
    system.addRule('C', "|D^|F^B-F+C^F^A&&FA&F^C+F+B^F^D//");
    system.addRule('D', "|CFB-F+B|FA&F^A&&FB-F+B|FC//");
    system.setProperty("N", 3);
    system.setProperty("angle", 90);
    system.setProperty("segmentLength", 10);
    system.setProperty("segmentRadius", 2);
    system.setProperty("shiny", 1);
    systems.push_back(system);
    
    system.reset();
    system.axiom = "P";
    system.addRule('P', "I+[P+O]--//[--L]I[++L]-[PO]++PO");
    system.addRule('I', "FS[//&&L][//^^L]FS");
    system.addRule('S', "S[//&&L][//^^L]FS");
    system.addRule('S', "SFS");
    system.addRule('S', "S");
    system.addRule('L', "['{+f-ff-f+|+f-ff-f}]");
    system.addRule('O', "[&&&D''/W////W////W////W////W]");
    system.addRule('D', "FF");
    system.addRule('W', "['^!F][{&&&&-f+f|-f+f}]");
    system.setProperty("N", 6);
    system.setProperty("angle", 18.0);
    system.setProperty("segmentLength", 3.0);
    system.setProperty("segmentRadius", 0.5);
    system.setProperty("colorBook", 1);
    systems.push_back(system);
    
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
    
    ofEnableDepthTest();
    ofEnableSmoothing();
    ofEnableAntiAliasing();
    
    //cam.setRotation(0.0, 0.0);
    //cam.setupPerspective(false);
    
    ofEnableAlphaBlending();
    ofEnableAntiAliasing();
    ofSetSmoothLighting(true);
    
    mode = Line;
    polyRenderMode = OF_MESH_FILL;
    
    headlight.setPointLight();
    headlight.setSpecularColor(ofColor(255, 255, 255));
    headlight.setDiffuseColor(ofColor(180, 100, 180));
    material.setShininess(7);
    
    drawListIndex = glGenLists(1);
    
    updateMesh();
}

ofVec3f getMeshCenter(ofPtr<ofMesh> mesh) {
    if(mesh->getVertices().size() == 0) {
        return ofVec3f(0, 0, 0);
    }
    
    ofVec3f minV(MAXFLOAT, MAXFLOAT, MAXFLOAT);
    ofVec3f maxV(-MAXFLOAT, -MAXFLOAT, -MAXFLOAT);
    for(unsigned int i = 0; i < mesh->getVertices().size(); i++) {
        minV.x = min(minV.x, mesh->getVertices()[i].x);
        minV.y = min(minV.y, mesh->getVertices()[i].y);
        minV.z = min(minV.z, mesh->getVertices()[i].z);
        maxV.x = max(maxV.x, mesh->getVertices()[i].x);
        maxV.y = max(maxV.y, mesh->getVertices()[i].y);
        maxV.z = max(maxV.z, mesh->getVertices()[i].z);
    }
    return 0.5 * (minV + maxV);
}

//--------------------------------------------------------------
void DemoLSystemApp::update(){
    if (mesh) {
        cam.setTarget(getMeshCenter(mesh));
    }
    cam.setDistance(250);
    headlight.setPosition(cam.getPosition());
}

//--------------------------------------------------------------
void DemoLSystemApp::draw(){
    ofBackground(0, 0, 80);
    ofSetColor(200, 200, 180);
    
    string title;
    LSystem& system = systems[currentSystem];
    int iterations = fmax(0.0, system.getProperty("N") + iterationAdjustment);

    if (mode == Line) {
        title = "LineGenerator";
        glLineWidth(0.5);
        glCallList(drawListIndex);
    } else if (mode == Mesh) {
        title = "MeshGenerator";
        cam.begin();
        ofSetLineWidth(0.1);
        if (system.getProperty("shiny")) {
            ofEnableLighting();
            headlight.enable();
            material.begin();
        }
        mesh->draw(polyRenderMode);
        ofDisableLighting();
        material.end();
        cam.end();
    }
    
    ofDrawBitmapString(system.axiom, 20, 25);
    int n = 0;
    for (auto& ruleGroup : system.rules) {
        for (auto& rule : ruleGroup.second) {
            if (ruleGroup.second.isStochastic()) {
                ofDrawBitmapString(to_string(rule.probability/3.0), 40, 40 + n);
            }
            
            ofDrawBitmapString(to_string(rule.predecessor) + " -----> " + rule.successor, 20, 45 + n);
            
            n += 15;
        }
    }
    ofDrawBitmapString("N = " + to_string(iterations), 20, 45 + n + 15);
    ofDrawBitmapString(title, 20, ofGetHeight() - 25);
    ofDrawBitmapString("SPACE to cycle modes", ofGetWidth() - 185, ofGetHeight() - 25);
    if (system.isStochastic()) {
        ofDrawBitmapString("R to reseed stochastic system", ofGetWidth() - 257, ofGetHeight() - 42);
    }
}

void DemoLSystemApp::updateMesh() {
    
    LSystem& system = systems[currentSystem];
    int iterations = fmax(0.0, system.getProperty("N") + iterationAdjustment);

    if (currentSystem <= lastLineSystem) {
        mode = Line;
        
        glNewList(drawListIndex, GL_COMPILE);
        
        LineGeneratorState state;
        state.angle = system.getProperty("angle");
        state.edgeLength = system.getProperty("edgeLength");
        state.heading = ofVec3f(0, -1);
        state.position = ofVec2f(ofGetWidth()/2, ofGetHeight() * 0.8);
        
        ofSetLineWidth(0.75);
        line_gen.generate(system, state, iterations);
        
        glEndList();
        
    } else {
        mode = Mesh;
        
        MeshGeneratorState state;
        state.left = state.heading.crossed(state.up);
        state.angle = system.getProperty("angle");
        state.segmentLength = system.getProperty("segmentLength");
        state.segmentRadius = system.getProperty("segmentRadius");
        if (system.hasProperty("colorBook") && system.getProperty("colorBook") < colorBooks.size()) {
            state.colorBook = colorBooks[system.getProperty("colorBook")];
        }
        
        mesh_gen.generate(systems[currentSystem], state, iterations);
        mesh = state.mesh;
    }
}

//--------------------------------------------------------------
void DemoLSystemApp::keyPressed(int key){
    if (key == ' ') {
        
        currentSystem = (currentSystem + 1) % systems.size();
        iterationAdjustment = 0;
        
        updateMesh();

    } else if (key == 'm') {
        if (polyRenderMode == OF_MESH_FILL) {
            polyRenderMode = OF_MESH_WIREFRAME;
        } else if (polyRenderMode == OF_MESH_POINTS) {
            polyRenderMode = OF_MESH_FILL;
        } else if (polyRenderMode == OF_MESH_WIREFRAME) {
            polyRenderMode = OF_MESH_POINTS;
        }
    } else if (key == '+') {
        iterationAdjustment++;
        updateMesh();
    } else if (key == '-') {
        iterationAdjustment--;
        updateMesh();
    } else if (key == 'r') {
        systems[currentSystem].reseed();
        updateMesh();
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
