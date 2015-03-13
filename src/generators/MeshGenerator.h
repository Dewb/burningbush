//
//  MeshGenerator.h
//  burningbush
//
//  Created by Dewb on 12/28/14.
//
//

#ifndef __burningbush__MeshGenerator__
#define __burningbush__MeshGenerator__

#include "LSystemGenerator.h"
#include "ColorBook.h"
#include <ofTypes.h>


class MeshResult {
public:
    virtual void addSegment(const ofVec3f& pt1, const ofVec3f& pt2, float radius, const ofColor& color) = 0;
    virtual void addPolygon(const vector<ofVec3f>& polyPoints, const vector<ofColor>& colors) = 0;
    virtual void begin() {}
    virtual void end() {}
};

class MeshGeneratorState {
public:
    MeshGeneratorState();
    MeshGeneratorState(const MeshGeneratorState& other);

    float angle;
    float segmentLength;
    float segmentRadius;
    float tropism;
    
    ofVec3f heading;
    ofVec3f left;
    ofVec3f up;

    ofVec3f position;
    vector<ofVec3f> pointHistory;
    vector<ofColor> colorHistory;
    ofPtr<ColorBook> colorBook;
    int currentColor;
    int currentColorSeries;
    
    ofColor getCurrentColor();
    bool inPolygonNode;
    
    ofPtr<MeshResult> result;
};

class MeshGenerator : public Generator<MeshGeneratorState> {
public:
    MeshGenerator();
    virtual void begin(MeshGeneratorState& state);
    virtual void end(MeshGeneratorState& state);
};

#endif /* defined(__burningbush__MeshGenerator__) */
