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
    
    void forward(LineGeneratorState& state, FloatParams& params) {
        float length = state.edgeLength;
        if (params.size() > 0) {
            length *= params[0];
        }
        state.previousPosition = state.position;
        state.position += state.heading.normalized() * state.edgeLength;
    }
    
    void forward_draw(LineGeneratorState& state, FloatParams& params) {
        forward(state, params);
        ofLine(state.previousPosition.x, state.previousPosition.y, state.position.x, state.position.y);
    }

    void rotate_cw(LineGeneratorState& state, FloatParams&) {
        state.heading.rotate(state.angle);
    }

    void rotate_ccw(LineGeneratorState& state, FloatParams&) {
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

