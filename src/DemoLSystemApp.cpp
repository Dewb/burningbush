#include "DemoLSystemApp.h"
#include "ParagraphFormatter.h"


//--------------------------------------------------------------
void DemoLSystemApp::createSystems() {
    
    LSystem system;

    system.reset();
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
    systems.push_back(system);
    
    system.reset();
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
    systems.push_back(system);
    
    system.reset();
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
    systems.push_back(system);
    
    system.reset();
    system.setAxiom("F");
    system.addRule('F', "F[+F]F[-F]F", 0.333);
    system.addRule('F', "F[+F]F", 0.333);
    system.addRule('F', "F[-F]F", 0.334);
    system.setProperty("N", 6);
    system.setProperty("angle", 25.7);
    system.setProperty("edgeLength", 3.0);
    systems.push_back(system);
    
    system.reset();
    system.setAxiom("F");
    system.addRule('F', "FF-[-F+F+F]+[+F-F-F]");
    system.setProperty("N", 4);
    system.setProperty("angle", 22.5);
    system.setProperty("edgeLength", 10);
    systems.push_back(system);
    
    system.reset();
    system.setAxiom("X");
    system.addRule('X', "F-[[X]+X]+F[+FX]-X");
    system.addRule('F', "FF");
    system.setProperty("N", 6);
    system.setProperty("angle", 22.5);
    system.setProperty("edgeLength", 3);
    systems.push_back(system);
    
    system.reset();
    system.setAxiom("F_l");
    system.addRule("F_l", "F_l+F_r++F_r-F_l--F_lF_l-F_r+");
    system.addRule("F_r", "-F_l+F_rF_r++F_r+F_l--F_l-F_r");
    system.setProperty("N", 4);
    system.setProperty("angle", 60);
    system.setProperty("edgeLength", 7);
    system.setProperty("position_x", 0.33);
    system.setProperty("position_y", -0.25);
    systems.push_back(system);
    
    system.reset();
    system.setAxiom("F+F+F+F");
    system.addRule('F', "F+f-FF+F+FF+Ff+FF-f+FF-F-FF-Ff-FFF");
    system.addRule('f', "ffffff");
    system.setProperty("N", 2);
    system.setProperty("angle", 90);
    system.setProperty("edgeLength", 9);
    system.setProperty("position_x", 0.33);
    system.setProperty("position_y", 0.33);
    systems.push_back(system);
    
    lastLineSystem = systems.size() - 1;
    
    system.reset();
    system.setAxiom("A");
    system.ignoreForContext("&L!A/S'\"^+-`|");
    system.addRule('A', "[&FL!A]/////'[&FL!A]///////'[&FL!A]");
    system.addRule('F', "S/////F");
    system.addRule('S', "FL");
    system.addRule("FFFFFFFFFFFFFF", 'L', "", "[\"''^^{-f+`f+`f-|-f+'f+'f}]");
    system.setProperty("N", 7);
    system.setProperty("angle", 22.5);
    system.setProperty("segmentLength", 4.5);
    system.setProperty("segmentRadius", 2);
    system.setProperty("colorBook", 0);
    systems.push_back(system);
    
    system.reset();
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
    systems.push_back(system);
    
    system.reset();
    system.setAxiom("P");
    system.addRule('P', "I+[P+O]--//[--L]I[++L]-[PO]++PO");
    system.addRule('I', "FS[//&&L][//^^L]FS");
    system.addRule('S', "S[//&&L][//^^L]FS", 0.333);
    system.addRule('S', "SFS", 0.333);
    system.addRule('S', "S", 0.334);
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
}

void DemoLSystemApp::setup() {
    
    createSystems();

    ofEnableDepthTest();
    ofEnableSmoothing();
    ofEnableAntiAliasing();
    
    ofEnableAlphaBlending();
    ofEnableAntiAliasing();
    ofSetSmoothLighting(true);
    
    mode = Line;
    polyRenderMode = OF_MESH_FILL;
    
    normalFont.reset(new ofTrueTypeFont());
    normalFont->loadFont("Courier New Bold.ttf", 12, false, false, true);
    smallFont.reset(new ofTrueTypeFont());
    smallFont->loadFont("verdana.ttf", 4, false, false, true);
    
    headlight.setPointLight();
    headlight.setAttenuation(0.1, 0.1, 0.1);
    headlight.setSpecularColor(ofColor(255, 255, 255));
    headlight.setDiffuseColor(ofColor(180, 100, 180));
    material.setShininess(7);
    
    drawListIndex = glGenLists(1);
    
    updateMesh();
}

ofVec3f getMeshCenter(ofPtr<ofMesh> mesh) {
    auto& v = mesh->getVertices();
    if(v.size() == 0) {
        return ofVec3f(0, 0, 0);
    }
    ofVec3f minV(MAXFLOAT, MAXFLOAT, MAXFLOAT);
    ofVec3f maxV(-MAXFLOAT, -MAXFLOAT, -MAXFLOAT);
    for(unsigned int i = 0; i < v.size(); i++) {
        minV.x = min(minV.x, v[i].x);
        minV.y = min(minV.y, v[i].y);
        minV.z = min(minV.z, v[i].z);
        maxV.x = max(maxV.x, v[i].x);
        maxV.y = max(maxV.y, v[i].y);
        maxV.z = max(maxV.z, v[i].z);
    }
    return 0.5 * (minV + maxV);
}

//--------------------------------------------------------------
void DemoLSystemApp::update(){
    cam.setDistance(250);
    //headlight.setPosition(cam.getPosition());
    headlight.setPosition(0, 0, 0);
}

//--------------------------------------------------------------
void DemoLSystemApp::draw(){
    ofBackground(0, 0, 80);
    ofSetColor(200, 200, 180);
    
    string generatorName;
    LSystem& system = systems[currentSystem];
    int iterations = fmax(0.0, system.getProperty("N") + iterationAdjustment);

    if (mode == Line) {
        generatorName = "LineGenerator";
        glLineWidth(0.5);
        glCallList(drawListIndex);
    } else if (mode == Mesh) {
        generatorName = "MeshGenerator";
        cam.begin();
        ofSetLineWidth(0.1);
        if (system.getProperty("shiny")) {
            ofEnableLighting();
            headlight.enable();
            //material.begin();
        }
        mesh->draw(polyRenderMode);
        ofDisableLighting();
        //material.end();
        cam.end();
    }
    
    ParagraphFormatter para(20, 25, NULL, NULL);
    
    para.printLine(to_string(system.getAxiom()));
    para.breakParagraph();
    
    string arrow = " --> ";
    if (system.isStochastic()) {
        arrow = " ------> ";
    }
    
    for (auto& ruleGroup : system.getRules()) {
        for (auto& rule : ruleGroup.second) {
            string left = "", right = "";
            if (!rule.leftContext.empty()) {
                left = to_string(rule.leftContext) + " < ";
            }
            if (!rule.rightContext.empty()) {
                right = " > " + to_string(rule.rightContext);
            }

            string super = "";
            if (ruleGroup.second.isStochastic()) {
                super = to_string(rule.probability);
            }
            string beforeArrow = left + to_string(rule.predecessor) + right;
            para.printLine(beforeArrow + arrow + to_string(rule.successor), super, beforeArrow.size() + 2);
        }
    }
    
    para.breakParagraph();
    para.printLine("N = " + to_string(iterations));
    if (system.isStochastic()) {
        para.printLine("seed = " + to_string(system.getSeed()));
    }
    
    para.restart(20, ofGetHeight() - 25, ParagraphFormatter::LowerLeft);
    para.printLine(generatorName);
    para.printLine(system.getTitle());

    para.restart(ofGetWidth() - 20, ofGetHeight() - 25, ParagraphFormatter::LowerRight);
    
    para.printLine("SPACE to cycle modes");
    if (system.isStochastic()) {
        para.printLine("R to reseed stochastic system");
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
        if (system.hasProperty("position_x")) {
            state.position.x = ofGetWidth() * (0.5 + 0.5 * system.getProperty("position_x"));
        }
        if (system.hasProperty("position_y")) {
            state.position.y = ofGetHeight() * (0.5 + 0.5 * system.getProperty("position_y"));
        }
        
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
        
        cam.setTarget(getMeshCenter(mesh));
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
