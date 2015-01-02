#pragma once

#include "ofMain.h"
#include "LineGenerator.h"
#include "MeshGenerator.h"
#include "TurntableCam.h"


typedef enum {
    Line,
    Mesh
} Mode;

class DemoLSystemApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    vector<LSystem> systems;
    LineGenerator line_gen;
    MeshGenerator mesh_gen;
    ofPtr<ofMesh> mesh;
    ofPolyRenderMode polyRenderMode;
    ofxTurntableCam cam;
    Mode mode;
};
