#include "DemoLSystemApp.h"
#include "DemoSystems.h"
#include "ParagraphFormatter.h"
#include "ofMeshResult.h"
#include "VectorFileMeshResult.h"


void DemoLSystemApp::setup() {

    ofQuaternion q, qo(0,.76081,0.64897,0), qr;
    qr.makeRotate(95, ofVec3f(0,0,1));
    q = qo.inverse();
    q.normalize();
    
    parseSystems(systems);
    createSystems(systems);
    createColorBooks(colorBooks);

    ofEnableDepthTest();
    ofEnableSmoothing();
    ofEnableAntiAliasing();
    
    ofEnableAlphaBlending();
    ofEnableAntiAliasing();
    ofSetSmoothLighting(true);
    ofBackground(0, 0, 0);
    ofSetBackgroundAuto(false);
    
    polyRenderMode = OF_MESH_FILL;
    
    headlight.setPointLight();
    headlight.setAttenuation(0.1, 0.001, 0.000001);
    headlight.setSpecularColor(ofColor(255, 255, 255));
    headlight.setDiffuseColor(ofColor(180, 100, 180));
    material.setShininess(7);

    shader.load("vert.glsl", "frag.glsl");
    lightShader.useCamera(&cam);
    lightShader.useLight(&headlight);
    
    cam.setPosition(-25, 50, 75);
    
    drawListIndex = glGenLists(1);
    
    currentSystem = 0;
    
    updateMesh();

    oscReceiver.setup(7777);
    showUI = true;

#ifdef USE_SYPHON
	syphonServer.setName("output");
#endif

#define DOME_SIZE 1024

#ifdef USE_SPOUT
	spoutSender.init("burningbush", DOME_SIZE, DOME_SIZE);
	spoutFbo.allocate(DOME_SIZE, DOME_SIZE, GL_RGBA);
#endif

    cameraRotationSpeed = 0.2;
    cameraZoom = 1.0;

    for (int i = 0; i < systems.size(); i++) {
        if (systems[i].second == GeneratorTypeText) {
            haikuRenderer.initialize(&systems[i].first, 3, "haiku", 650, 180, "Raleway-Black.ttf", 28, 15);
            haikuRenderer.hide();
        }
    }

	domemaster.setup();
	domemaster.setCameraPosition(-20, 40, 70);
	domemaster.resize(DOME_SIZE, DOME_SIZE);

    options.useCache = false;
}

ofVec3f getMeshCenter(ofPtr<ofMesh> mesh) {
    auto& v = mesh->getVertices();
    if(v.size() == 0) {
        return ofVec3f(0, 0, 0);
    }
    ofVec3f minV(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    ofVec3f maxV(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
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
    cam.setDistance(100 * pow(2, -cameraZoom));
    headlight.setPosition(-100, 100, 100);
    processOscInput();
    if (fabs(cameraRotationSpeed) > FLT_EPSILON) {
        cam.setRotation(cameraRotationSpeed * 0.001, 0);
        viewDirty = true;
    }
}

bool oscPropertyChange(LSystem& system, string propName, float value)
{
    if (system.hasProperty(propName)) {
        float base = 0.0;
        float factor = 1.0;
        if (propName == "angle") {
            base = 20;
            factor = 40;
        } else if (propName == "tropism") {
            base = 0.10;
            factor = 0.5;
        }
        float oldValue = system.getProperty(propName);
        float newValue = base + value * factor;
        if (oldValue != newValue) {
            system.setProperty(propName, newValue);
            return true;
        }
    }
    return false;
}

void DemoLSystemApp::processOscInput() {

    LSystem& system = systems[currentSystem].first;
    bool meshChanged = false;

    while(oscReceiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        oscReceiver.getNextMessage(&m);
        float value = m.getArgAsFloat(0);

        if(m.getAddress().substr(0, 24) == "/currentSystem/property/"){
            string propName = m.getAddress().substr(24);
            meshChanged = oscPropertyChange(system, propName, value);
        }

        if(m.getAddress().substr(0, 19) == "/system/1/property/"){
            string propName = m.getAddress().substr(19);
            if (systems.size() > 0) {
                meshChanged = oscPropertyChange(systems[0].first, propName, value);
            }
        }

        if(m.getAddress().substr(0, 19) == "/system/2/property/"){
            string propName = m.getAddress().substr(19);
            if (systems.size() > 1) {
                meshChanged = oscPropertyChange(systems[1].first, propName, value);
            }
        }

        if(m.getAddress().substr(0, 19) == "/system/3/property/"){
            string propName = m.getAddress().substr(19);
            if (systems.size() > 2) {
                meshChanged = oscPropertyChange(systems[2].first, propName, value);
            }
        }

        if(m.getAddress() == "/camera/rotation/x/speed"){
            cameraRotationSpeed = (m.getArgAsFloat(0) - 0.5) * 2.0;
        }

        if(m.getAddress() == "/haiku"){
            bool on = !(m.getArgAsFloat(0) > 0);
            if (on) {
                haikuRenderer.newText();
                haikuRenderer.show();
            } else {
                haikuRenderer.hide();
            }
            viewDirty = true;
        }

        if (m.getAddress() == "/currentSystem/reseed" && m.getArgAsFloat(0) > 0) {
            systems[currentSystem].first.reseed();
            meshChanged = true;
        }

        if(m.getAddress() == "/camera/zoom"){
            cameraZoom = (m.getArgAsFloat(0) - 0.5) * 2.0;
            viewDirty = true;
        }
    }

    if (meshChanged) {
        updateMesh();
    }
}

void DemoLSystemApp::draw() {
	if (!viewDirty)
		return;

	// First few draw calls seem to be ineffective, for some reason
	if (ofGetElapsedTimeMillis() > 500)
		viewDirty = false;

	cam.begin();
	drawScene();
	cam.end();

	for (int i = 0; i<domemaster.renderCount; i++) {
		domemaster.begin(i);
		drawScene();
		domemaster.end(i);
	}


#ifdef USE_SYPHON
	syphonServer.publishScreen();
#endif

#ifdef USE_SPOUT
	//spoutTexture.loadScreenData(0, 0, spoutTexture.getWidth(), spoutTexture.getHeight());
	
	spoutFbo.begin();
	ofClear(ofColor(0, 0, 0, 0));
	domemaster.draw();
	spoutFbo.end();

	spoutSender.send(spoutFbo.getTextureReference(0), true);
#endif

	haikuRenderer.update();

	if (false) {
		// now that we've published our syphon outputs, draw a background
		ofSetColor(60, 10, 140, 255);
		ofRect(ofGetWidth() * -2, ofGetHeight() * -2, -999, ofGetWidth() * 4, ofGetHeight() * 4);
	}

	ofSetColor(200, 200, 180);
	ofDisableDepthTest();
	haikuRenderer.draw(20, ofGetHeight() - 200);
	ofEnableDepthTest();

	ofSetColor(200, 200, 180);


	if (showUI) {

		string generatorName;
		LSystem& system = systems[currentSystem].first;
		GeneratorType mode = systems[currentSystem].second;

		if (mode == GeneratorTypeLine) {
			generatorName = "LineGenerator";
			glLineWidth(0.5);
			glCallList(drawListIndex);
		}
		else if (mode == GeneratorTypeMesh) {
			generatorName = "MeshGenerator";
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

		for (auto& prop : system.getProperties()) {
			para.printLine(prop + " = " + to_string(system.getProperty(prop)));
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
}

void DemoLSystemApp::drawScene() {

	//ofBackground(0, 0, 100, 255);
	ofBackground(0, 0, 0, 0);

	ofPushMatrix();
	ofPushStyle();

	LSystem& system = systems[currentSystem].first;
	GeneratorType mode = systems[currentSystem].second;

	int iterations = fmax(0.0, system.getProperty("N") + iterationAdjustment);

	if (mode == GeneratorTypeLine) {
		glLineWidth(0.5);
		glCallList(drawListIndex);
	}
	else if (mode == GeneratorTypeMesh) {
		//shader.begin();
		//shader.setUniformMatrix4f("normalMatrix", ofMatrix4x4::getTransposedOf(cam.getModelViewMatrix().getInverse()));

		//lightShader.begin();

		ofSetLineWidth(0.1);
		if (system.getProperty("shiny")) {
			ofEnableLighting();
			headlight.enable();
			//material.begin();
		}
		mesh->draw(polyRenderMode);
		ofDisableLighting();
		//material.end();

		//lightShader.end();
		//shader.end();
	}

	ofPopStyle();
	ofPopMatrix();
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
        vector_gen.generate(system, state, iterations, options);
    } else if (mode == GeneratorTypeMesh) {
        
        MeshGeneratorState state;
        state.left = state.heading.crossed(state.up);
        state.angle = system.getProperty("angle");
        state.segmentLength = system.getProperty("segmentLength");
        state.segmentLengthGrowthFactor = system.getProperty("segmentLengthGrowthFactor");
        state.segmentRadius = system.getProperty("segmentRadius");
        state.tropism = system.getProperty("tropism");
        if (system.hasProperty("colorBook") && system.getProperty("colorBook") < colorBooks.size()) {
            state.colorBook = colorBooks[system.getProperty("colorBook")];
        }
        
        auto result = new VectorFileMeshResult();
        result->filename = fileResult.getPath();
        result->scale = 500;
        state.result.reset(result);
        
        mesh_gen.generate(system, state, iterations, options);
    }

}

void DemoLSystemApp::saveMesh() {
    LSystem& system = systems[currentSystem].first;
    GeneratorType mode = systems[currentSystem].second;

    if (mode == GeneratorTypeMesh) {
        int iterations = fmax(0.0, system.getProperty("N") + iterationAdjustment);

        std::ostringstream ss;
        ss << (system.getTitle().empty() ? "lsystem" : system.getTitle());
        ss << "-";
        ss << system.getSeed();
        ss << "-iter";
        ss << iterations;
        ss << ".ply";

        auto fileResult = ofSystemSaveDialog(ss.str(), "Save model");
        if (!fileResult.bSuccess) {
            return;
        }
        mesh->save(fileResult.getPath());
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
        line_gen.generate(system, state, iterations, options);
        
        glEndList();
        
    } else if (mode == GeneratorTypeMesh) {
        
        MeshGeneratorState state;
        state.left = state.heading.crossed(state.up);
        state.angle = system.getProperty("angle");
        state.segmentLength = system.getProperty("segmentLength");
        state.segmentLengthGrowthFactor = system.getProperty("segmentLengthGrowthFactor");
        state.segmentRadius = system.getProperty("segmentRadius");
        state.tropism = system.getProperty("tropism");
        if (system.hasProperty("colorBook") && system.getProperty("colorBook") < colorBooks.size()) {
            state.colorBook = colorBooks[system.getProperty("colorBook")];
        }
        
        auto result = new ofMeshResult();
        state.result.reset(result);

//        if (reuseRuleStr) {
//            mesh_gen.generate(ruleStr, system, state, iterations, options);
//        }
        mesh_gen.generate(system, state, iterations, options);
        mesh = result->mesh;

        if (!system.hasProperty("centered") || system.getProperty("centered") != 0) {
            cam.setTarget(getMeshCenter(mesh));
        } else {
            cam.setTarget(ofVec3f(0, 0, 0));
        }
    }
    
    viewDirty = true;
}

void DemoLSystemApp::keyPressed(int key){
    if (key == ' ') {

        do {
            currentSystem = (currentSystem + 1) % systems.size();
        } while (systems[currentSystem].second != GeneratorTypeLine &&
                 systems[currentSystem].second != GeneratorTypeMesh);

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
    } else if (key == 'S') {
        saveMesh();
    } else if (key == 'R') {
        systems.clear();
        parseSystems(systems);
        updateMesh(true);
    } else if (key == '?') {
        showUI = !showUI;
        viewDirty = true;
    } else if (key == 'h') {
        haikuRenderer.show();
        haikuRenderer.newText();
        viewDirty = true;
    } else if (key == 'l') {
        options.logging = !options.logging;
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
