#include "DemoLSystemApp.h"
#include "DemoSystems.h"
#include "ParagraphFormatter.h"
#include "ofMeshResult.h"
#include "VectorFileMeshResult.h"


void DemoLSystemApp::setup() {
    
    createSystems(systems);
    createColorBooks(colorBooks);

    ofEnableDepthTest();
    ofEnableSmoothing();
    ofEnableAntiAliasing();
    
    ofEnableAlphaBlending();
    ofEnableAntiAliasing();
    ofSetSmoothLighting(true);
    ofBackground(0, 0, 80);
    ofSetBackgroundAuto(false);
    
    polyRenderMode = OF_MESH_FILL;
    
    headlight.setPointLight();
    headlight.setAttenuation(0.1, 0.001, 0.000001);
    headlight.setSpecularColor(ofColor(255, 255, 255));
    headlight.setDiffuseColor(ofColor(180, 100, 180));
    material.setShininess(7);
    
    cam.setPosition(-25, 100, 75);
    
    drawListIndex = glGenLists(1);
    
    currentSystem = 0;
    
    updateMesh();

    oscReceiver.setup(7777);
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

void DemoLSystemApp::update(){
    cam.setDistance(250);
    headlight.setPosition(-100, 100, 100);
    processOscInput();
}

void DemoLSystemApp::processOscInput() {

    LSystem& system = systems[currentSystem].first;
    bool changed = false;

    while(oscReceiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        oscReceiver.getNextMessage(&m);

        if(m.getAddress().substr(0, 17) == "/system/property/"){
            string propName = m.getAddress().substr(17);
            if (system.hasProperty(propName)) {
                float oldValue = system.getProperty(propName);
                float newValue = m.getArgAsFloat(0);
                if (oldValue != newValue) {
                    system.setProperty(propName, newValue);
                    changed = true;
                }
            }
        }
    }

    if (changed) {
        updateMesh();
    }
}

void DemoLSystemApp::draw(){
    if (!viewDirty)
        return;
    
    // First few draw calls seem to be ineffective, for some reason
    if (ofGetElapsedTimeMillis() > 500)
        viewDirty = false;
    
    ofBackground(0, 0, 80);
    ofSetColor(200, 200, 180);
    
    string generatorName;
    LSystem& system = systems[currentSystem].first;
    GeneratorType mode = systems[currentSystem].second;
    
    int iterations = fmax(0.0, system.getProperty("N") + iterationAdjustment);

    if (mode == GeneratorTypeLine) {
        generatorName = "LineGenerator";
        glLineWidth(0.5);
        glCallList(drawListIndex);
    } else if (mode == GeneratorTypeFloorplan) {
        generatorName = "FloorplanGenerator";
        glLineWidth(0.5);
        glEnable(GL_NORMALIZE);
        glPushMatrix();
        glTranslatef(400, 400, 0);
        glScalef(8.0, 8.0, 8.0);
        glCallList(drawListIndex);
        glPopMatrix();
    } else if (mode == GeneratorTypeMesh) {
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
            string left = "", right = "", cond = "";
            if (!rule.leftContext.empty()) {
                left = to_string(rule.leftContext) + " < ";
            }
            if (!rule.rightContext.empty()) {
                right = " > " + to_string(rule.rightContext);
            }
            if (!rule.parametricCondition.empty()) {
                cond = " : " + rule.parametricCondition;
            }

            string super = "";
            if (ruleGroup.second.isStochastic()) {
                super = to_string(rule.probability);
            }
            string beforeArrow = left + to_string(rule.predecessor) + right + cond;
            para.printLine(beforeArrow + arrow + to_string(rule.successor), super, beforeArrow.size() + 2);
        }
    }
    
    para.breakParagraph();
    para.printLine("N = " + to_string(iterations));
    if (system.hasProperty("angle")) {
        para.printLine("angle = " + to_string(system.getProperty("angle")));
    }
    if (system.hasProperty("colorBook")) {
        para.printLine("color book #" + to_string(system.getProperty("colorBook")));
    }
    if (system.isStochastic()) {
        para.printLine("seed = " + to_string(system.getSeed()));
    }
    
    para.restart(20, ofGetHeight() - 25, ParagraphFormatter::LowerLeft);
    para.printLine(generatorName);
    para.printLine(system.getTitle());

    para.restart(ofGetWidth() - 20, ofGetHeight() - 25, ParagraphFormatter::LowerRight);
    
    para.printLine("SPACE to cycle systems");
    para.printLine("+/- to adjust iteration count");
    if (system.isStochastic()) {
        para.printLine("R to reseed stochastic system");
    }
}

void DemoLSystemApp::saveVectorFile() {
    
    LSystem& system = systems[currentSystem].first;
    GeneratorType mode = systems[currentSystem].second;
    
    int iterations = fmax(0.0, system.getProperty("N") + iterationAdjustment);
    
    string defaultName = system.getTitle().empty() ? "lsystem.eps" : system.getTitle() + ".eps";
    auto fileResult = ofSystemSaveDialog(defaultName, "Save EPS file");
    if (!fileResult.bSuccess) {
        return;
    }
    
    if (mode == GeneratorTypeLine) {
        VectorFileGeneratorState state;
        state.filename = fileResult.getPath();
        state.angle = system.getProperty("angle");
        state.edgeLength = system.getProperty("edgeLength");
        state.heading = 0;
        state.position = ofVec2f(ofGetWidth()/2, ofGetHeight() * 0.8);
        if (system.hasProperty("heading")) {
            state.heading = system.getProperty("heading");
        }
        if (system.hasProperty("position_x")) {
            state.position.x = ofGetWidth() * (0.5 + 0.5 * system.getProperty("position_x"));
        }
        if (system.hasProperty("position_y")) {
            state.position.y = ofGetHeight() * (0.5 + 0.5 * system.getProperty("position_y"));
        }
        
        ofSetLineWidth(0.5);
        vector_gen.generate(system, state, iterations);
    } else if (mode == GeneratorTypeMesh) {
        
        MeshGeneratorState state;
        state.left = state.heading.crossed(state.up);
        state.angle = system.getProperty("angle");
        state.segmentLength = system.getProperty("segmentLength");
        state.segmentRadius = system.getProperty("segmentRadius");
        if (system.hasProperty("colorBook") && system.getProperty("colorBook") < colorBooks.size()) {
            state.colorBook = colorBooks[system.getProperty("colorBook")];
        }
        
        auto result = new VectorFileMeshResult();
        result->filename = fileResult.getPath();
        result->scale = 500;
        state.result.reset(result);
        
        mesh_gen.generate(system, state, iterations);
    }

}

void DemoLSystemApp::updateMesh(bool rerunSystem) {
    
    LSystem& system = systems[currentSystem].first;
    GeneratorType mode = systems[currentSystem].second;

    int iterations = fmax(0.0, system.getProperty("N") + iterationAdjustment);

    if (mode == GeneratorTypeLine) {
        
        glNewList(drawListIndex, GL_COMPILE);
        
        LineGeneratorState state;
        state.angle = system.getProperty("angle");
        state.edgeLength = system.getProperty("edgeLength");
        state.heading = 0;
        state.position = ofVec2f(ofGetWidth()/2, ofGetHeight() * 0.8);
        if (system.hasProperty("heading")) {
            state.heading = system.getProperty("heading");
        }
        if (system.hasProperty("position_x")) {
            state.position.x = ofGetWidth() * (0.5 + 0.5 * system.getProperty("position_x"));
        }
        if (system.hasProperty("position_y")) {
            state.position.y = ofGetHeight() * (0.5 + 0.5 * system.getProperty("position_y"));
        }
        
        ofSetLineWidth(0.5);
        line_gen.generate(system, state, iterations);
        
        glEndList();
    
    } else if (mode == GeneratorTypeFloorplan) {

        glNewList(drawListIndex, GL_COMPILE);

        FloorplanGeneratorState state;
        state.position = ofVec2f(0, 0);
        state.results.reset(new vector<Space>());
        floorplan_gen.generate(system, state, iterations);

        glEndList();
    
    } else if (mode == GeneratorTypeMesh) {
        
        MeshGeneratorState state;
        state.left = state.heading.crossed(state.up);
        state.angle = system.getProperty("angle");
        state.segmentLength = system.getProperty("segmentLength");
        state.segmentRadius = system.getProperty("segmentRadius");
        if (system.hasProperty("colorBook") && system.getProperty("colorBook") < colorBooks.size()) {
            state.colorBook = colorBooks[system.getProperty("colorBook")];
        }
        
        auto result = new ofMeshResult();
        state.result.reset(result);
        
        mesh_gen.generate(system, state, iterations);
        mesh = result->mesh;
        
        cam.setTarget(getMeshCenter(mesh));
    }
    
    viewDirty = true;
}

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
        viewDirty = true;
    } else if (key == '+') {
        iterationAdjustment++;
        updateMesh();
    } else if (key == '-') {
        iterationAdjustment--;
        updateMesh();
    } else if (key == 'r') {
        systems[currentSystem].first.reseed();
        updateMesh();
    } else if (key == 's') {
        saveVectorFile();
    }
}

void DemoLSystemApp::keyReleased(int key){
}

void DemoLSystemApp::mouseMoved(int x, int y ){
}

void DemoLSystemApp::mouseDragged(int x, int y, int button){
    cam.mouseDragged(x, y, button);
    viewDirty = true;
}

void DemoLSystemApp::mousePressed(int x, int y, int button){
    cam.mousePressed(x, y, button);
    viewDirty = true;
}

void DemoLSystemApp::mouseReleased(int x, int y, int button){
    cam.mouseReleased(x, y, button);
    viewDirty = true;
}

void DemoLSystemApp::windowResized(int w, int h){

}

void DemoLSystemApp::gotMessage(ofMessage msg){
}

void DemoLSystemApp::dragEvent(ofDragInfo dragInfo){
}
