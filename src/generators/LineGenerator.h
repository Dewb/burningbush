//
//  LineGenerator.h
//  burningbush
//
//  Created by Dewb on 12/28/14.
//
//

#ifndef __burningbush__LineGenerator__
#define __burningbush__LineGenerator__

#include "LSystemGenerator.h"
#include <ofGraphics.h>

class LineGeneratorState {
public:
    LineGeneratorState();
    
    ofVec2f position;
    ofVec2f previousPosition;
    float heading;
    float angle;
    float edgeLength;
};

class LineGenerator : public Generator<LineGeneratorState> {
public:
    LineGenerator();
};

#endif /* defined(__burningbush__MeshGenerator__) */
