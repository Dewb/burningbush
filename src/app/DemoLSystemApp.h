#pragma once

#include "ofMain.h"
#include "LineGenerator.h"
#include "MeshGenerator.h"
#include "VectorFileGenerator.h"
#include "TurntableCam.h"

template<typename T>
class ofPtrArray : public vector<ofPtr<T> > {
public:
};

typedef enum {
    GeneratorTypeLine,
    GeneratorTypeMesh
} GeneratorType;


class DemoLSystemApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    
    void updateMesh();
    void saveVectorFile();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

protected:
    ofxTurntableCam cam;
    ofPolyRenderMode polyRenderMode;
    bool viewDirty;
    
    unsigned int drawListIndex;
    ofLight headlight;
    ofMaterial material;

    ofPtr<ofMesh> mesh;
    
    LineGenerator line_gen;
    MeshGenerator mesh_gen;
    VectorFileGenerator vector_gen;

    int currentSystem;
    int iterationAdjustment;

    vector<pair<LSystem, GeneratorType> > systems;
    ofPtrArray<ColorBook> colorBooks;
};
