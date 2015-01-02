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
    ofVec3f position;
    ofVec3f previousPosition;
    ofVec3f heading;
    float angle;
    float edgeLength;
};

class MeshGenerator : public Generator<ofMesh, MeshGeneratorState> {
public:
    MeshGenerator();
    virtual void begin(ofMesh& mesh);
    virtual void end(ofMesh& mesh);
};

#endif /* defined(__burningbush__MeshGenerator__) */
