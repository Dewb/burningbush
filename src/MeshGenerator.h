//
//  MeshGenerator.h
//  burningbush
//
//  Created by Dewb on 12/28/14.
//
//

#ifndef __burningbush__MeshGenerator__
#define __burningbush__MeshGenerator__

#include "LSystem.h"
#include <ofMesh.h>

class MeshGeneratorState {
public:
    MeshGeneratorState() : heading(ofVec3f(0, 1, 0)) {}
    MeshGeneratorState(const MeshGeneratorState& other);
    ofVec3f position;
    vector<ofVec3f> pointHistory;
    ofVec3f heading;
    ofVec3f left;
    ofVec3f up;
    float angle;
    float segmentLength;
    float segmentRadius;
    ofPtr<ofMesh> mesh;
};

class MeshGenerator : public Generator<MeshGeneratorState> {
public:
    MeshGenerator();
    virtual void begin(MeshGeneratorState& state);
    virtual void end(MeshGeneratorState& state);    
};

#endif /* defined(__burningbush__MeshGenerator__) */
