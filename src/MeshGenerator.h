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
#include <ofMesh.h>

class ColorBook {
public:
    ColorBook();
    void add(const ofColor& c, int number = 1);
    void addGradient(const ofColor& c1, const ofColor& c2, int steps = 3);
    void addGradient(const ofColor& next, int steps = 3);
    void nextSeries();
    const ofColor& getColor(int series, int index);
    int getNumSeries() const;
    int getNumColors(int series) const;
protected:
    vector<vector<ofColor> > colors;
};

class MeshGeneratorState {
public:
    MeshGeneratorState();
    MeshGeneratorState(const MeshGeneratorState& other);

    float angle;
    float segmentLength;
    float segmentRadius;
    
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
    
    ofPtr<ofMesh> mesh;
};

class MeshGenerator : public Generator<MeshGeneratorState> {
public:
    MeshGenerator();
    virtual void begin(MeshGeneratorState& state);
    virtual void end(MeshGeneratorState& state);    
};

#endif /* defined(__burningbush__MeshGenerator__) */
