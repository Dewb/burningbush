//
//  MeshGenerator.cpp
//  burningbush
//
//  Created by Dewb on 12/28/14.
//
//

#include "LineGenerator.h"

LineGeneratorState::LineGeneratorState()
: position(ofVec2f(0, 0))
, previousPosition(ofVec2f(0, 0))
, heading(ofVec2f(1, 0))
, angle(90)
, edgeLength(4.0)
{
}

namespace {
    
    void forward(LineGeneratorState& state) {
        state.previousPosition = state.position;
        state.position += state.heading.normalized() * state.edgeLength;
    }
    
    void forward_draw(LineGeneratorState& state) {
        state.previousPosition = state.position;
        state.position += state.heading.normalized() * state.edgeLength;
        ofLine(state.previousPosition.x, state.previousPosition.y, state.position.x, state.position.y);
    }

    void rotate_cw(LineGeneratorState& state) {
        state.heading.rotate(state.angle);
    }

    void rotate_ccw(LineGeneratorState& state) {
        state.heading.rotate(-state.angle);
    }
    
}

LineGenerator::LineGenerator() {
    add(Symbol('F').action(forward_draw));
    add(Symbol('f').action(forward));
    add(Symbol('+').action(rotate_ccw));
    add(Symbol('-').action(rotate_cw));
    add(Symbol('[').startsGroup());
    add(Symbol(']').endsGroup());
}

