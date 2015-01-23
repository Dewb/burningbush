//
//  FloorplanGenerator.h
//  burningbush
//
//  Created by Dewb on 1/22/15.
//
//

#ifndef __burningbush__FloorplanGenerator__
#define __burningbush__FloorplanGenerator__

#include "LSystemGenerator.h"
#include <ofTypes.h>

class Space {
public:
    string function;
    bool isCirculation;
    ofVec2f dimensions;
    ofVec2f position;
    void setPosition(float x, float y) { position.x = x; position.y = y; }
    ofVec2f getPosition() const { return position; }
    ofVec2f getWorldDimensions() const { return dimensions; }
};
std::ostream& operator<<(std::ostream& os, const Space& space);

typedef pair<Space, int> SpaceRequirement;
typedef vector<SpaceRequirement> SpaceRequirementList;

class FloorplanGeneratorState {
public:
    FloorplanGeneratorState();
    //FloorplanGeneratorState(const FloorplanGeneratorState& other);
    
    float angle;
    
    float heading;
    ofVec2f position;
    ofVec2f previousPosition;
    
    int lastSpaceIndex;
    ofPtr<vector<Space> > results;

    float circulationWidth;
    SpaceRequirementList functionalSpaceTypes;
    
};

class FloorplanGenerator : public Generator<FloorplanGeneratorState> {
public:
    FloorplanGenerator();
    virtual void begin(FloorplanGeneratorState& state);
    virtual void end(FloorplanGeneratorState& state);
};


#endif /* defined(__burningbush__FloorplanGenerator__) */
