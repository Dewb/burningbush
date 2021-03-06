#pragma once

#include "ofMain.h"
#include "LineGenerator.h"
#include "MeshGenerator.h"
#include "VectorFileGenerator.h"
#include "TurntableCam.h"
#include "ofxOsc.h"
#include "SyphonTextRenderer.h"
#include "ofxShadersFX.h"

template<typename T>
class ofPtrArray : public vector<ofPtr<T> > {
public:
};

typedef enum {
    GeneratorTypeNone = -1,
    GeneratorTypeLine,
    GeneratorTypeMesh,
    GeneratorTypeText
} GeneratorType;


class DemoLSystemApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    
    void updateMesh(bool rerunSystem = true);
    void saveVectorFile();
    void saveMesh();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void processOscInput();

protected:
    bool showUI;

    LSystemOptions options;

    ofxSyphonServer syphonServer;
    ofxOscReceiver oscReceiver;

    ofxTurntableCam cam;
    ofPolyRenderMode polyRenderMode;
    bool viewDirty;
    
    unsigned int drawListIndex;
    ofLight headlight;
    ofMaterial material;
    ofShader shader;
    ofxShadersFX::Lighting::LightingShader lightShader;

    ofPtr<ofMesh> mesh;
    
    LineGenerator line_gen;
    MeshGenerator mesh_gen;
    VectorFileGenerator vector_gen;

    int currentSystem;
    int iterationAdjustment;

    vector<pair<LSystem, GeneratorType> > systems;
    ofPtrArray<ColorBook> colorBooks;

    SyphonTextRenderer haikuRenderer;
    
    float cameraRotationSpeed;
    float cameraZoom;
};
